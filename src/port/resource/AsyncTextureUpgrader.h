#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

namespace Ship {
struct File;
}
namespace Fast {
class Texture;
}

namespace MK64 {

/**
 * Streams texture-pack (PNG) replacements in without stalling the render path.
 *
 * The texture factory returns the original binary texture immediately and
 * enqueues the replacement here; a worker thread decodes the PNG, and
 * ApplyCompleted() (called between frames on the main thread) swaps the
 * decoded pixels into the live Fast::Texture and invalidates the old GPU
 * cache entry. Until the swap lands, the original art draws — the same
 * pop-in behavior hi-res packs have under N64Recomp/RT64.
 */
class AsyncTextureUpgrader {
  public:
    static AsyncTextureUpgrader& Instance();

    void Enqueue(std::shared_ptr<Fast::Texture> texture, std::shared_ptr<Ship::File> imageFile, uint16_t origWidth,
                 uint16_t origHeight);

    // First-touch prediction: when one replaced texture in a directory loads,
    // decode the whole directory in the background (kart animation folders,
    // font sheets, ...) so sibling textures are already HD when first drawn.
    // Each directory is prefetched at most once per ResetPrefetch().
    void PrefetchSiblings(const std::string& resourcePath);

    // Forget which directories were prefetched (call when the resource cache
    // is evicted, e.g. on the alt-assets toggle).
    void ResetPrefetch();

    // Main thread, between frames. `invalidate` evicts a data pointer from the
    // renderer's texture cache (Interpreter::TextureCacheDelete).
    void ApplyCompleted(const std::function<void(const uint8_t*)>& invalidate);

    void Shutdown();

  private:
    AsyncTextureUpgrader() = default;
    ~AsyncTextureUpgrader();

    struct Job {
        std::shared_ptr<Fast::Texture> texture;
        std::shared_ptr<Ship::File> imageFile;
        uint16_t origWidth;
        uint16_t origHeight;
    };
    struct Decoded {
        std::shared_ptr<Fast::Texture> texture;
        uint8_t* pixels; // new[] — ownership passes to the texture on apply
        int width;
        int height;
        uint16_t origWidth;
        uint16_t origHeight;
    };

    void WorkerLoop();
    void EnsureWorker();
    void PrefetchLoop();
    void EnsurePrefetchWorkers();

    std::mutex mMutex;
    std::condition_variable mCondVar;
    std::deque<Job> mPending;
    std::deque<Decoded> mDecoded;
    std::thread mWorker;
    bool mWorkerRunning = false;
    bool mStop = false;

    std::condition_variable mPrefetchCondVar;
    std::deque<std::string> mPrefetchDirs;
    std::unordered_set<std::string> mQueuedDirs;
    std::vector<std::thread> mPrefetchWorkers;

    // Old binary pixel buffers, kept alive for the session: the renderer's
    // texture cache and TMEM mirrors may still reference their addresses for
    // a short window after the swap, and keeping them mapped also prevents
    // the allocator from reusing an address that is still a live cache key.
    std::vector<uint8_t*> mRetiredBuffers;
};

} // namespace MK64
