#include <functional>
#include <unordered_map>
#include <string>

#include "Registry.h"
#include "engine/CoreMath.h"

extern "C" {
#include "actors.h"
#include "actor_types.h"
}

/**
 * Lambda define explanation
 * 
 * C++ syntax sucks. All you need to know, is that we pass a user defined function into the registry
 * with the specified number of parameters.
 * When this function is called it runs whatever code you told it to run. As such,
 * This code should send the basic parameters required to spawn the object, preferably via a SpawnParams struct
 * rather than hard-coded parameters such as location, rotation, and scale. This way, the parameters can be adjusted without
 * re-writing the registry code.
 * 
 * Usage:
 * AddActor("hm:cloud", [](const SpawnParams& params) { // My code here })
 * 
 * A new registry is required if you need different lambda parameters.
 * 
 * AddTrack("hm:harbour", []() { // My code here }) does not require a parameter. So this needs to be a new registry.
 * 
 * Check RegisteredEntries.cpp for further example usage.
 * 
 */
#define Lambda(param) std::function<void(param)>

void Registry::AddActor(const std::string& name, Lambda(const SpawnParams&) spawnFunc) {
    Actors[name] = { spawnFunc };
}

void Registry::AddTrack(const std::string& name, Lambda() spawnFunc) {
    Tracks[name] = { spawnFunc };
}

void Registry::SpawnTrack(const std::string& name) {
    auto it = Tracks.find(name);
    if (it != Tracks.end() && it->second.spawnFunc) {
        printf("[Registry] Found track %s\n", name.c_str());
        it->second.spawnFunc();
    }
}

void Registry::SpawnActor(SpawnParams& params) {
    auto it = Actors.find(params.Name);
    if (it != Actors.end() && it->second.spawnFunc) {
        printf("[Registry] Spawned %s\n", params.Name.c_str());
        it->second.spawnFunc(params);
    }
}

// @arg name Must be a resource name such as mk:car
bool Registry::FindActor(const std::string& name) {
    return Actors.find(name) != Actors.end();
}

// @arg name Must be a resource name such as hm:harbour
bool Registry::FindTrack(const std::string& name) {
    return Tracks.find(name) != Tracks.end();
}

#undef Lambda
