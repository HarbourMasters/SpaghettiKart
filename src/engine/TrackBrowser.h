#ifndef TRACKBROWSER_H
#define TRACKBROWSER_H

#include <libultraship.h>
#include "port/Game.h"


#ifdef __cplusplus
#include <vector>
/**
 * Allows exploring tracks in the menus 
 */
class TrackBrowser {
private:
    // Holds all available tracks
    std::vector<const TrackInfo*> mTracks;
    size_t mTrackIndex = 0;

public:
    static TrackBrowser* Instance;

    TrackBrowser(const Registry<TrackInfo>& registry) {
        mTracks = registry.GetAllInfo();
        std::sort(mTracks.begin(), mTracks.end(), [](const TrackInfo* a, const TrackInfo* b) {
            return a->Id < b->Id;
        });
        Instance = this;
    }

    void FindCustomTracks();

    void Refresh(const Registry<TrackInfo>& registry) {
        mTracks.clear();
        mTracks = registry.GetAllInfo();
        std::sort(mTracks.begin(), mTracks.end(), [](const TrackInfo* a, const TrackInfo* b) {
            return a->Id < b->Id;
        });
        mTrackIndex = 0;
    }

    void Reset() {
        mTrackIndex = 0;
    }

    void SetTrack(std::string name) {
        if (gTrackRegistry.Find(name)) {
            gTrackRegistry.Invoke(name);
        } else {
            throw std::runtime_error("[World] [SetTrack()] Track name not found in Track list: " + name);
        }
    }

    void NextTrack() {
        if (mTracks.empty()) return;

        mTrackIndex = (mTrackIndex + 1) % mTracks.size();
        gTrackRegistry.Invoke(mTracks[mTrackIndex]->ResourceName);
    }

    void PreviousTrack() {
        if (mTracks.empty()) return;

        mTrackIndex = (mTrackIndex + mTracks.size() - 1) % mTracks.size();
        gTrackRegistry.Invoke(mTracks[mTrackIndex]->ResourceName);
    }

    size_t GetTrackIndex() {
        return mTrackIndex;
    }

    const char* GetTrackName() {
        if (mTracks.empty()) return "";

        if (mTracks[mTrackIndex]) {
            return mTracks[mTrackIndex]->Name.c_str();
        }
        return "";
    }

    const char* GetTrackDebugName() {
        if (mTracks.empty()) return "";

        if (mTracks[mTrackIndex]) {
            return mTracks[mTrackIndex]->DebugName.c_str();
        }
        return "";
    }

    const char* GetTrackLength() {
        if (mTracks.empty()) return "";

        if (mTracks[mTrackIndex]) {
            return mTracks[mTrackIndex]->Length.c_str();
        }
        return "";
    }

    /**
     * The index setters and getters here are for legacy code support
     * Try not to rely too heavily on these functions especially for custom content.
     * 
     * The content at an index may not be guaranteed if other clients have different custom content.
     * Even the same content could be loaded at a different index
     * 
     * The location of stock content *should* be consistent across all clients.
     */

    void SetTrackByIdx(size_t trackIndex) {
        if (trackIndex >= mTracks.size()) {
            printf("[TrackBrowser] [SetTrackById] Error: trackIndex %zu out of bounds (max %zu)\n", trackIndex, mTracks.size());
            return;
        }
        if (nullptr == mTracks[mTrackIndex]) {
            printf("[TrackBrowser] [SetTrackById] Error: TrackInfo at index %zu is null\n", mTrackIndex);
            return;
        }
        mTrackIndex = trackIndex;
        gTrackRegistry.Invoke(mTracks[mTrackIndex]->ResourceName);
    }

    const char* GetTrackNameByIdx(size_t trackIndex) {
        if (trackIndex >= mTracks.size()) {
            printf("[TrackBrowser] [GetTrackNameByIdx] Error: trackIndex %zu out of bounds (max %zu)\n", trackIndex, mTracks.size());
            return "";
        }
        if (nullptr == mTracks[trackIndex]) {
            printf("[TrackBrowser] [GetTrackNameByIdx] Error: TrackInfo at index %zu is null\n", trackIndex);
            return "";
        }
        return mTracks[trackIndex]->Name.c_str();
    }

    const char* GetTrackDebugNameByIdx(size_t trackIndex) {
        if (trackIndex >= mTracks.size()) {
            printf("[TrackBrowser] [GetTrackDebugNameByIdx] Error: trackIndex %zu out of bounds (max %zu)\n", trackIndex, mTracks.size());
            return "";
        }
        if (nullptr == mTracks[trackIndex]) {
            printf("[TrackBrowser] [GetTrackDebugNameByIdx] Error: TrackInfo at index %zu is null\n", trackIndex);
            return "";
        }
        return mTracks[trackIndex]->DebugName.c_str();
    }

    const char* GetTrackLengthByIdx(size_t trackIndex) {
        if (trackIndex >= mTracks.size()) {
            printf("[TrackBrowser] [GetTrackLengthByIdx] Error: trackIndex %zu out of bounds (max %zu)\n", trackIndex, mTracks.size());
            return "";
        }
        if (nullptr == mTracks[trackIndex]) {
            printf("[TrackBrowser] [GetTrackLengthByIdx] Error: TrackInfo at index %zu is null\n", trackIndex);
            return "";
        }
        return mTracks[trackIndex]->Length.c_str();
    }

    const char* GetMinimapTextureByIdx(size_t trackIndex) {
        if (trackIndex >= mTracks.size()) {
            printf("[TrackBrowser] [GetTrackMinimapTextureByIdx] Error: trackIndex %zu out of bounds (max %zu)\n", trackIndex, mTracks.size());
            return NULL;
        }
        if (nullptr == mTracks[trackIndex]) {
            printf("[TrackBrowser] [GetTrackMinimapTextureByIdx] Error: TrackInfo at index %zu is null\n", trackIndex);
            return NULL;
        }
        return mTracks[trackIndex]->MinimapTexture;
    }
};

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void TrackBrowser_SetTrack(const char* name);
void TrackBrowser_SetTrackFromCup(void); // <-- Not in TrackBrowser class
void TrackBrowser_NextTrack(void);
void TrackBrowser_PreviousTrack(void);
size_t TrackBrowser_GetTrackIndex(void);
const char* TrackBrowser_GetTrackName(void);
const char* TrackBrowser_GetTrackDebugName(void);
const char* TrackBrowser_GetTrackLength(void);
void TrackBrowser_SetTrackByIdx(size_t trackIndex);
const char* TrackBrowser_GetTrackNameByIdx(size_t trackIndex);
const char* TrackBrowser_GetTrackDebugNameByIdx(size_t trackIndex);
const char* TrackBrowser_GetTrackLengthByIdx(size_t trackIndex);
const char* TrackBrowser_GetMinimapTextureByIdx(size_t trackIndex);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TRACKBROWSER_H