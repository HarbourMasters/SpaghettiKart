#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "tracks/Track.h"
#include "registry/Registry.h"

class Track; // <-- Forward declare

class Cup {
public:
    std::string Id;
    const char* Name;
    u8 *Thumbnail;
    size_t CursorPosition = 0; // Track index in cup
    std::vector<std::string> mTracks;

    explicit Cup(std::string id, const char* name, std::vector<std::string> tracks);

    // Valide que tous les IDs de tracks existent dans le registre
    void ValidateTrackIds(const Registry<TrackInfo>& registry) const;

    virtual void ShuffleTracks();

    virtual void Next();
    virtual void Previous();
    virtual void SetTrack(size_t position);
    virtual std::string GetTrack();
    virtual size_t GetSize();
};
