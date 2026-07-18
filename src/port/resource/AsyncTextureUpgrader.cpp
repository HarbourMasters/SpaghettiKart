#include "AsyncTextureUpgrader.h"

#include "fast/resource/type/Texture.h"
#include "ship/resource/File.h"
#include "ship/Context.h"
#include "ship/resource/ResourceManager.h"
#include "ship/resource/archive/ArchiveManager.h"
#include "spdlog/spdlog.h"
#include <stb_image.h>
#include <algorithm>
#include <cstring>

namespace MK64 {

AsyncTextureUpgrader& AsyncTextureUpgrader::Instance() {
    static AsyncTextureUpgrader instance;
    return instance;
}

AsyncTextureUpgrader::~AsyncTextureUpgrader() {
    Shutdown();
}

void AsyncTextureUpgrader::EnsureWorker() {
    // Caller holds mMutex.
    if (!mWorkerRunning) {
        mWorkerRunning = true;
        mWorker = std::thread(&AsyncTextureUpgrader::WorkerLoop, this);
    }
}

void AsyncTextureUpgrader::Enqueue(std::shared_ptr<Fast::Texture> texture, std::shared_ptr<Ship::File> imageFile,
                                   uint16_t origWidth, uint16_t origHeight) {
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (mStop) {
            return;
        }
        mPending.push_back({ std::move(texture), std::move(imageFile), origWidth, origHeight, mGeneration });
        EnsureWorker();
    }
    mCondVar.notify_one();
}

void AsyncTextureUpgrader::WorkerLoop() {
    for (;;) {
        Job job;
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mCondVar.wait(lock, [this] { return mStop || !mPending.empty(); });
            if (mStop) {
                return;
            }
            job = std::move(mPending.front());
            mPending.pop_front();
        }

        int width = 0, height = 0;
        stbi_uc* decoded = stbi_load_from_memory((const stbi_uc*)job.imageFile->Buffer.get()->data(),
                                                 job.imageFile->Buffer.get()->size(), &width, &height, nullptr, 4);
        if (decoded == nullptr || width <= 0 || height <= 0) {
            SPDLOG_WARN("AsyncTextureUpgrader: failed to decode replacement image for {}",
                        job.texture->GetInitData()->Path);
            if (decoded != nullptr) {
                stbi_image_free(decoded);
            }
            continue; // keep the original binary art
        }

        // Copy into a new[] buffer so ownership matches Texture's destructor.
        const size_t size = (size_t)width * height * 4;
        uint8_t* pixels = new uint8_t[size];
        std::memcpy(pixels, decoded, size);
        stbi_image_free(decoded);

        {
            const std::lock_guard<std::mutex> lock(mMutex);
            if (mStop) {
                delete[] pixels;
                return;
            }
            if (job.generation != mGeneration) {
                // The toggle evicted this texture while it was being decoded.
                delete[] pixels;
                continue;
            }
            mDecoded.push_back(
                { std::move(job.texture), pixels, width, height, job.origWidth, job.origHeight, job.generation });
        }
    }
}

void AsyncTextureUpgrader::EnsurePrefetchWorkers() {
    // Caller holds mMutex.
    if (mPrefetchWorkers.empty()) {
        constexpr int kPrefetchThreads = 2;
        for (int i = 0; i < kPrefetchThreads; i++) {
            mPrefetchWorkers.emplace_back(&AsyncTextureUpgrader::PrefetchLoop, this);
        }
    }
}

void AsyncTextureUpgrader::PrefetchSiblings(const std::string& resourcePath) {
    const size_t slash = resourcePath.find_last_of('/');
    if (slash == std::string::npos || slash == 0) {
        return;
    }
    std::string dir = resourcePath.substr(0, slash);
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (mStop || !mQueuedDirs.insert(dir).second) {
            return;
        }
        mPrefetchDirs.push_back(std::move(dir));
        EnsurePrefetchWorkers();
    }
    mPrefetchCondVar.notify_one();
}

void AsyncTextureUpgrader::ResetPrefetch() {
    const std::lock_guard<std::mutex> lock(mMutex);
    mGeneration++;
    mPrefetchDirs.clear();
    mQueuedDirs.clear();
    // Drop queued decode jobs and undelivered results: they all target texture
    // objects the toggle just evicted from the resource cache.
    mPending.clear();
    for (Decoded& d : mDecoded) {
        delete[] d.pixels;
    }
    mDecoded.clear();
}

// Image-sibling extensions the texture factory recognizes (kept in sync with
// BetterTextureFactory's list).
static const char* kImageExts[] = { ".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG", ".bmp", ".BMP" };

void AsyncTextureUpgrader::PrefetchLoop() {
    for (;;) {
        std::string dir;
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mPrefetchCondVar.wait(lock, [this] { return mStop || !mPrefetchDirs.empty(); });
            if (mStop) {
                return;
            }
            dir = std::move(mPrefetchDirs.front());
            mPrefetchDirs.pop_front();
        }

        auto resourceManager = Ship::Context::GetInstance()->GetResourceManager();
        auto files = resourceManager->GetArchiveManager()->ListFiles(dir + "/*");
        if (files == nullptr) {
            continue;
        }

        size_t loaded = 0;
        for (const std::string& path : *files) {
            {
                const std::lock_guard<std::mutex> lock(mMutex);
                if (mStop) {
                    return;
                }
            }
            if (!resourceManager->IsAltAssetsEnabled()) {
                break; // pack was toggled off mid-prefetch
            }
            for (const char* ext : kImageExts) {
                const size_t extLen = std::strlen(ext);
                if (path.size() > extLen && path.compare(path.size() - extLen, extLen, ext) == 0) {
                    // Loading the base resource runs the normal factory path,
                    // which decodes/queues the replacement off the render
                    // thread; the resource cache dedupes repeat loads.
                    resourceManager->LoadResourceProcess(path.substr(0, path.size() - extLen));
                    loaded++;
                    break;
                }
            }
        }
        if (loaded > 0) {
            SPDLOG_INFO("AsyncTextureUpgrader: prefetched {} replaced textures under {}", loaded, dir);
        }
    }
}

void AsyncTextureUpgrader::ApplyCompleted(const std::function<void(const uint8_t*)>& invalidate) {
    std::deque<Decoded> ready;
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        ready.swap(mDecoded);
    }

    for (Decoded& d : ready) {
        Fast::Texture* tex = d.texture.get();
        uint8_t* oldData = tex->ImageData;

        tex->ImageData = d.pixels;
        tex->ImageDataSize = (uint32_t)((size_t)d.width * d.height * 4);
        tex->Width = (uint16_t)d.width;
        tex->Height = (uint16_t)d.height;
        tex->OrigWidth = d.origWidth;
        tex->OrigHeight = d.origHeight;
        tex->Type = Fast::TextureType::RGBA32bpp;
        // Match what the synchronous replacement loader produced: LOAD_AS_IMG
        // only (binary flags like LOAD_AS_RAW no longer apply), unit scales.
        tex->Flags = TEX_FLAG_LOAD_AS_IMG;
        tex->HByteScale = 1.0f;
        tex->VPixelScale = 1.0f;

        if (oldData != nullptr) {
            if (invalidate) {
                invalidate(oldData);
            }
            if (tex->mImageBuffer) {
                // Old pixels lived inside the shared file buffer; the texture no
                // longer points at it, so just drop our extra reference.
                tex->mImageBuffer.reset();
            } else {
                const std::lock_guard<std::mutex> lock(mMutex);
                mRetiredBuffers.push_back(oldData);
            }
        }
    }
}

void AsyncTextureUpgrader::Shutdown() {
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (mStop) {
            return;
        }
        mStop = true;
    }
    mCondVar.notify_all();
    mPrefetchCondVar.notify_all();
    if (mWorker.joinable()) {
        mWorker.join();
    }
    for (std::thread& t : mPrefetchWorkers) {
        if (t.joinable()) {
            t.join();
        }
    }
    mPrefetchWorkers.clear();

    const std::lock_guard<std::mutex> lock(mMutex);
    for (Decoded& d : mDecoded) {
        delete[] d.pixels;
    }
    mDecoded.clear();
    mPending.clear();
    for (uint8_t* buf : mRetiredBuffers) {
        delete[] buf;
    }
    mRetiredBuffers.clear();
}

} // namespace MK64
