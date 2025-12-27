#include "Cup.h"
#include "tracks/Track.h"
#include "port/Game.h"

Cup::Cup(std::string id, const char* name, std::vector<std::string> tracks) {
    Id = id;
    Name = name;
    mTracks = tracks;

    if (mTracks.size() != 4) {
        throw std::invalid_argument("A cup must contain exactly 4 tracks.");
    }
}

void Cup::Next() {
    if (CursorPosition < mTracks.size() - 1) {
        CursorPosition++;
    }
}

void Cup::Previous() {
    if (CursorPosition > 0) {
        CursorPosition--;
    }
}

void Cup::SetTrack(size_t position) {
    if ((position < 0) || (position >= mTracks.size())) {
        throw std::invalid_argument("Invalid track index.");
    }
    CursorPosition = position;
}

std::string Cup::GetTrack() {
    return mTracks[CursorPosition];
}

size_t Cup::GetSize() {
    return mTracks.size();
}

// Function to shuffle the tracks randomly
void Cup::ShuffleTracks() {
    // std::random_device rd;
    // std::mt19937 g(rd());
    //std::shuffle(mTracks.begin(), mTracks.end(), g);
}

void Cup::ValidateTrackIds(const Registry<TrackInfo>& registry) const {
    std::vector<std::string> invalidTracks;
    
    for (const auto& trackId : mTracks) {
        if (!registry.Find(trackId)) {
            invalidTracks.push_back(trackId);
        }
    }
    
    if (!invalidTracks.empty()) {
        std::string errorMsg = "Cup '" + Id + "' contains invalid track IDs: ";
        for (size_t i = 0; i < invalidTracks.size(); ++i) {
            if (i > 0) errorMsg += ", ";
            errorMsg += "'" + invalidTracks[i] + "'";
        }
        throw std::invalid_argument(errorMsg);
    }
}
