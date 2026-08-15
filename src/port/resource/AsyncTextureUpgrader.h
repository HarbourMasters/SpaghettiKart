#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
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

    // Register an explicit sprite frame set (e.g. Lakitu's rotation angles):
    // replacements for these paths are swapped in together. Callable from C
    // via AsyncTextureUpgrader_RegisterFrameSet.
    void RegisterFrameSet(const char* const* names, size_t count);

    // Forget which directories were prefetched and drop all queued/decoded
    // work from before the reset (call when the resource cache is evicted,
    // e.g. on the alt-assets toggle). Without the drain, rapid toggling piles
    // stale decode jobs for evicted textures in front of the single decode
    // worker and live textures stop upgrading.
    void ResetPrefetch();

    // Course teardown: evict the finished course's replacements (everything
    // under textures/tracks/) so long sessions stay flat on low-end devices;
    // karts/HUD/common stay resident, decoded cache kept (RAM-bounded).
    void EvictTrackTextures();

    // Main thread, between frames. `invalidate` evicts a data pointer from the
    // renderer's texture cache (Interpreter::TextureCacheDelete).
    void ApplyCompleted(const std::function<void(const uint8_t*)>& invalidate);

    void Shutdown();

  private:
    AsyncTextureUpgrader() = default;
    ~AsyncTextureUpgrader();

    struct CacheEntry {
        uint8_t* pixels;
        int width;
        int height;
        uint16_t origWidth;
        uint16_t origHeight;
        size_t bytes;
    };
    struct Job {
        std::shared_ptr<Fast::Texture> texture;
        std::shared_ptr<Ship::File> imageFile;
        uint16_t origWidth;
        uint16_t origHeight;
        uint64_t generation;
        std::string group;
        const CacheEntry* cached = nullptr;
    };
    struct Decoded {
        std::shared_ptr<Fast::Texture> texture;
        uint8_t* pixels; // new[] — ownership passes to the texture on apply
        int width;
        int height;
        uint16_t origWidth;
        uint16_t origHeight;
        uint64_t generation;
        std::string group;
    };

    void WorkerLoop();
    void FinishJobLocked(bool cacheHit); // mMutex held; logs a batch summary when the queue drains
    void EnsureWorker();
    void PrefetchLoop();
    void EnsurePrefetchWorkers();

    std::mutex mMutex;
    std::condition_variable mCondVar;
    std::deque<Job> mPending;
    std::deque<Decoded> mDecoded;
    // Frames of one sprite set (same name minus trailing digits) swap in
    // together: completed frames wait here until the whole group has decoded,
    // so multi-frame sprites (e.g. Lakitu's 16 rotation angles) pop to HD as
    // one coherent object instead of flickering in frame by frame.
    std::deque<Decoded> mHeldByGroup;
    std::unordered_map<std::string, int> mInflightByGroup;
    // path -> group id for explicitly registered frame sets only; textures
    // outside a registered set swap individually (course tiles must not batch).
    std::unordered_map<std::string, std::string> mFrameSetOfPath;
    // Session cache of decoded replacements: the first toggle pays the decode
    // cost, every later toggle is a memcpy. Sized from system RAM (unified on
    // Apple Silicon), see CacheLimitBytes(); entries live until Shutdown.
    std::unordered_map<std::string, CacheEntry> mDecodedCache;
    size_t mDecodedCacheBytes = 0;
    // Instrumentation: per-drain counts of cache hits vs fresh decodes.
    size_t mBatchHits = 0;
    size_t mBatchDecodes = 0;
    int mInflightJobs = 0;
    // Bumped by ResetPrefetch(); work stamped with an older generation targets
    // textures that were evicted by the toggle and is dropped, not applied.
    uint64_t mGeneration = 0;
    std::vector<std::thread> mDecodeWorkers;
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
