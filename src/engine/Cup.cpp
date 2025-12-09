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
