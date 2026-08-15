#include "AsyncTextureUpgrader.h"

#include "fast/interpreter.h"
#include "fast/resource/type/Texture.h"
#include "ship/resource/File.h"
#include "ship/Context.h"
#include "ship/resource/ResourceManager.h"
#include "ship/resource/archive/ArchiveManager.h"
#include "spdlog/spdlog.h"
#include <libultraship.h>
#include <stb_image.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include <cctype>
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
    // Caller holds mMutex. Decoding is CPU-bound and embarrassingly parallel;
    // a single worker made scheduling zero-sum (prioritizing one sprite set
    // starved everything else), so run a small pool instead. Sized to the
    // machine (quarter of the cores, 1..4); workers sleep outside streaming
    // bursts, so steady-state racing pays nothing. gTexturePack.DecodeThreads
    // overrides for unusual setups (0 = auto).
    static const size_t kDecodeThreads = [] {
        const int32_t forced = CVarGetInteger("gTexturePack.DecodeThreads", 0);
        if (forced > 0) {
            return (size_t)std::min(forced, 8);
        }
        const unsigned hc = std::thread::hardware_concurrency();
        return (size_t)std::clamp(hc / 2u, 1u, 8u);
    }();
    if (mDecodeWorkers.size() < kDecodeThreads && mPending.size() > mDecodeWorkers.size()) {
        mDecodeWorkers.emplace_back(&AsyncTextureUpgrader::WorkerLoop, this);
    }
}

// 0 disables caching. Tiered by physical RAM: plenty of headroom on 32GB+
// machines (unified memory on Apple Silicon), conservative at 16GB, off below.
// gTexturePack.CacheMB overrides (-1 forces off).
static size_t CacheLimitBytes() {
    static const size_t limit = [] {
        size_t bytes = 0;
        const int32_t forcedMB = CVarGetInteger("gTexturePack.CacheMB", 0);
        const int ramMB = SDL_GetSystemRAM();
        if (forcedMB > 0) {
            bytes = (size_t)forcedMB * 1024 * 1024;
        } else if (forcedMB == 0) {
            if (ramMB >= 32 * 1024) {
                bytes = (size_t)ramMB / 4 * 1024 * 1024;
            } else if (ramMB >= 16 * 1024) {
                bytes = (size_t)2048 * 1024 * 1024;
            }
        }
        SPDLOG_INFO("AsyncTextureUpgrader: system RAM {} MB, decoded-cache limit {} MB{}", ramMB,
                    bytes / (1024 * 1024), forcedMB != 0 ? " (gTexturePack.CacheMB override)" : "");
        return bytes;
    }();
    return limit;
}

void AsyncTextureUpgrader::Enqueue(std::shared_ptr<Fast::Texture> texture, std::shared_ptr<Ship::File> imageFile,
                                   uint16_t origWidth, uint16_t origHeight) {
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (mStop) {
            return;
        }
        const CacheEntry* cached = nullptr;
        auto cacheIt = mDecodedCache.find(texture->GetInitData()->Path);
        if (cacheIt != mDecodedCache.end()) {
            cached = &cacheIt->second;
        }
        std::string group;
        auto setIt = mFrameSetOfPath.find(texture->GetInitData()->Path);
        if (setIt != mFrameSetOfPath.end()) {
            group = setIt->second;
            mInflightByGroup[group]++;
        }
        if (!group.empty()) {
            // Registered frame sets decode ahead of the general queue: the set
            // can only swap once its slowest member lands, so its members must
            // not wait behind the whole scene's textures.
            mPending.push_front({ std::move(texture), std::move(imageFile), origWidth, origHeight, mGeneration, group, cached });
        } else {
            mPending.push_back({ std::move(texture), std::move(imageFile), origWidth, origHeight, mGeneration, group, cached });
        }
        EnsureWorker();
    }
    mCondVar.notify_one();
}

void AsyncTextureUpgrader::FinishJobLocked(bool cacheHit) {
    mInflightJobs--;
    if (cacheHit) {
        mBatchHits++;
    } else {
        mBatchDecodes++;
    }
    if (mPending.empty() && mInflightJobs == 0) {
        SPDLOG_INFO("AsyncTextureUpgrader: batch drained: {} cache hits, {} decodes, cache holds {} MB",
                    mBatchHits, mBatchDecodes, mDecodedCacheBytes / (1024 * 1024));
        mBatchHits = 0;
        mBatchDecodes = 0;
    }
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
            mInflightJobs++;
        }

        if (job.cached != nullptr) {
            // Session-cache hit: a memcpy instead of a PNG decode (~50x faster),
            // which is what makes repeat toggles feel instant.
            const CacheEntry& e = *job.cached;
            uint8_t* copy = new uint8_t[e.bytes];
            std::memcpy(copy, e.pixels, e.bytes);
            const std::lock_guard<std::mutex> lock(mMutex);
            if (!job.group.empty()) {
                auto it = mInflightByGroup.find(job.group);
                if (it != mInflightByGroup.end() && --it->second <= 0) {
                    mInflightByGroup.erase(it);
                }
            }
            FinishJobLocked(true);
            if (job.generation != mGeneration) {
                delete[] copy;
                continue;
            }
            mDecoded.push_back({ std::move(job.texture), copy, e.width, e.height, e.origWidth, e.origHeight,
                                 job.generation, std::move(job.group) });
            continue;
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
            {
                const std::lock_guard<std::mutex> lock(mMutex);
                if (!job.group.empty()) {
                    auto it = mInflightByGroup.find(job.group);
                    if (it != mInflightByGroup.end() && --it->second <= 0) {
                        mInflightByGroup.erase(it);
                    }
                }
                FinishJobLocked(false);
            }
            continue; // keep the original binary art
        }

        // Copy into a new[] buffer so ownership matches Texture's destructor.
        const size_t size = (size_t)width * height * 4;
        uint8_t* pixels = new uint8_t[size];
        std::memcpy(pixels, decoded, size);
        stbi_image_free(decoded);

        // Remember the decoded result for the rest of the session if the
        // machine has room, so later toggles skip the decode entirely.
        if (CacheLimitBytes() > 0) {
            const std::lock_guard<std::mutex> lock(mMutex);
            const std::string& path = job.texture->GetInitData()->Path;
            if (mDecodedCache.find(path) == mDecodedCache.end() &&
                mDecodedCacheBytes + size <= CacheLimitBytes()) {
                uint8_t* keep = new uint8_t[size];
                std::memcpy(keep, pixels, size);
                mDecodedCache[path] = { keep, width, height, job.origWidth, job.origHeight, size };
                mDecodedCacheBytes += size;
            }
        }

        {
            const std::lock_guard<std::mutex> lock(mMutex);
            if (mStop) {
                delete[] pixels;
                return;
            }
            if (!job.group.empty()) {
                auto it = mInflightByGroup.find(job.group);
                if (it != mInflightByGroup.end() && --it->second <= 0) {
                    mInflightByGroup.erase(it);
                }
            }
            FinishJobLocked(false);
            if (job.generation != mGeneration) {
                // The toggle evicted this texture while it was being decoded.
                delete[] pixels;
                continue;
            }
            mDecoded.push_back({ std::move(job.texture), pixels, width, height, job.origWidth, job.origHeight,
                                 job.generation, std::move(job.group) });
        }
    }
}

void AsyncTextureUpgrader::EnsurePrefetchWorkers() {
    // Caller holds mMutex.
    if (mPrefetchWorkers.empty()) {
        // Scaled with the decode pool: prefetch does the archive extraction
        // that feeds the decoders, so two fixed workers would bottleneck a
        // larger pool.
        static const int kPrefetchThreads =
            (int)std::clamp(std::thread::hardware_concurrency() / 4u, 2u, 4u);
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

void AsyncTextureUpgrader::RegisterFrameSet(const char* const* names, size_t count) {
    if (names == nullptr || count < 2) {
        return;
    }
    const std::lock_guard<std::mutex> lock(mMutex);
    // Asset-name symbols carry the __OTR__ signature; resource paths are
    // stored without it, so strip it or the lookups never match.
    auto canonical = [](const char* name) -> std::string {
        static const char kSig[] = "__OTR__";
        if (strncmp(name, kSig, sizeof(kSig) - 1) == 0) {
            return std::string(name + sizeof(kSig) - 1);
        }
        return std::string(name);
    };
    // First name identifies the set.
    std::string groupId = canonical(names[0]);
    for (size_t i = 0; i < count; i++) {
        if (names[i] != nullptr) {
            mFrameSetOfPath[canonical(names[i])] = groupId;
        }
    }
}

extern "C" void AsyncTextureUpgrader_RegisterFrameSet(const char* const* names, size_t count) {
    AsyncTextureUpgrader::Instance().RegisterFrameSet(names, count);
}

void AsyncTextureUpgrader::ResetPrefetch() {
    const std::lock_guard<std::mutex> lock(mMutex);
    mGeneration++;
    mPrefetchDirs.clear();
    mQueuedDirs.clear();
    // Drop queued decode jobs and undelivered results: they all target texture
    // objects the toggle just evicted from the resource cache.
    mPending.clear();
    mInflightByGroup.clear();
    for (Decoded& d : mDecoded) {
        delete[] d.pixels;
    }
    mDecoded.clear();
    for (Decoded& d : mHeldByGroup) {
        delete[] d.pixels;
    }
    mHeldByGroup.clear();
}

// All course art lives under textures/tracks/<course>/ and only one course is
// loaded at a time, so this subtree is exactly "the course that just ended".
// A worker mid-decode on a course texture may still deliver afterward; it
// applies to an orphaned texture object nothing draws, then frees with it.
void AsyncTextureUpgrader::EvictTrackTextures() {
    auto* ctx = Ship::Context::GetRawInstance();
    if (ctx == nullptr || ctx->GetResourceManager() == nullptr ||
        !ctx->GetResourceManager()->IsAltAssetsEnabled()) {
        return;
    }
    static constexpr char kPrefix[] = "textures/tracks/";
    const auto underPrefix = [](const std::string& path) { return path.rfind(kPrefix, 0) == 0; };
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        std::erase_if(mQueuedDirs, underPrefix);
        std::erase_if(mPrefetchDirs, underPrefix);
        std::erase_if(mPending, [&](const Job& job) { return underPrefix(job.texture->GetInitData()->Path); });
        std::erase_if(mDecoded, [&](Decoded& d) {
            if (!underPrefix(d.texture->GetInitData()->Path)) {
                return false;
            }
            delete[] d.pixels;
            return true;
        });
        std::erase_if(mHeldByGroup, [&](Decoded& d) {
            if (!underPrefix(d.texture->GetInitData()->Path)) {
                return false;
            }
            delete[] d.pixels;
            return true;
        });
        std::erase_if(mInflightByGroup, [&](const auto& entry) { return underPrefix(entry.first); });
    }
    ctx->GetResourceManager()->UnloadResources("textures/tracks/*");
    gfx_texture_cache_clear();
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

        auto resourceManager = Ship::Context::GetRawInstance()->GetResourceManager();
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
        // Merge newly decoded frames with any held back for group completion,
        // then hold back whichever groups still have frames in flight.
        for (Decoded& d : mDecoded) {
            mHeldByGroup.push_back(std::move(d));
        }
        mDecoded.clear();
        std::deque<Decoded> stillHeld;
        for (Decoded& d : mHeldByGroup) {
            if (!d.group.empty() && mInflightByGroup.count(d.group)) {
                stillHeld.push_back(std::move(d));
            } else {
                ready.push_back(std::move(d));
            }
        }
        mHeldByGroup.swap(stillHeld);
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
    for (std::thread& t : mDecodeWorkers) {
        if (t.joinable()) {
            t.join();
        }
    }
    mDecodeWorkers.clear();
    for (std::thread& t : mPrefetchWorkers) {
        if (t.joinable()) {
            t.join();
        }
    }
    mPrefetchWorkers.clear();
    for (auto& [path, entry] : mDecodedCache) {
        delete[] entry.pixels;
    }
    mDecodedCache.clear();
    mDecodedCacheBytes = 0;

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
