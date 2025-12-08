#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "tracks/Track.h"

class Track; // <-- Forward declare

class Cup {
public:
    std::string Id;
    const char* Name;
    u8 *Thumbnail;
    size_t CursorPosition = 0; // Track index in cup
    std::vector<std::shared_ptr<Track>> Courses;

    explicit Cup(std::string id, const char* name, std::vector<std::shared_ptr<Track>> courses);

    virtual void ShuffleCourses();

    virtual void Next();
    virtual void Previous();
    virtual void SetTrack(size_t position);
    virtual std::shared_ptr<Track> GetTrack();
    virtual size_t GetSize();
};