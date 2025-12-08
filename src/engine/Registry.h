#pragma once

#include <libultraship.h>
#include "SpawnParams.h"

class Registry {
public:

    void SpawnActor(SpawnParams& params);
    void SpawnTrack(const std::string& name);
    void AddActor(const std::string& name,
                    std::function<void(const SpawnParams&)> spawnFunc);
    bool FindActor(const std::string& name);
    bool FindTrack(const std::string& name);

    void AddTrack(const std::string& name, std::function<void()> spawnFunc);
private:
    struct ActorEntry {
        std::function<void(const SpawnParams&)> spawnFunc;
    };

    struct TrackEntry {
        std::function<void()> spawnFunc;
    };
    std::unordered_map<std::string, Registry::ActorEntry> Actors;
    std::unordered_map<std::string, Registry::TrackEntry> Tracks;
};
