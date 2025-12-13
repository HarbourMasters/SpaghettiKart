#pragma once

#include <libultraship.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <set>

struct TrackInfo {
    int32_t Id;
    std::string Path; // Path to the custom track
    std::string ResourceName;
    std::string Name;
    std::string DebugName;
    std::string Length;
    const char* MinimapTexture;
    // std::string PreviewTexture;
};

struct ActorInfo {
    int32_t Id;
    std::string ResourceName;
    std::string Name;
    std::set<std::string> Tags; // Category for filtering
};

/**
 * TInfo must have a ResourceName member of type std::string
 * This should be a unique string such as hm:cloud or hm:harbour
 * user_name:my_mod
 * 
 * TArgs the parameters passed into the callback function
 * 
 * Example Usage:
 * 
 * Registry<ActorInfo> gActorRegistry;
 * 
 * ActorInfo actorInfo {
 *     .ResourceName = "hm:cloud",
 * };
 * SpawnParams params;
 * params.Location = FVector(0, 0, 0);
 * 
 * gActorRegistry.Add(actorInfo, [](params) {
 *     GetWorld()->AddActor(new ACloud(params));
 * });
 * 
 * gActorRegistry.Invoke("hm:cloud", params);
 * 
 */

template<typename TInfo, typename... TArgs>
// ^ Scary template key word
class Registry {
public:
    /**
     * Callback explanation
     * 
     * This is just a lambda function.
     * 
     * Usage: gRegistry.Add("hm:harbour", [](parameters) { // My code here })
     */
    using Callback = std::function<void(TArgs...)>;

    void Add(TInfo& info, Callback func) {
        info.Id = mCounter++;
        // Needs to allow overwriting to support mod hot-reloading
        mMap[info.ResourceName] = Entry{info, std::move(func)};
    }

    const TInfo* GetInfo(const std::string& resourceName) const {
        auto it = mMap.find(resourceName);
        return (it != mMap.end()) ? &it->second.Info : nullptr;
    }

    void Invoke(const std::string& resourceName, TArgs... args) {
        auto it = mMap.find(resourceName);
        if (it != mMap.end() && it->second.Func) {
            // Using C++ variadic template expansion to call the function
            printf("[Registry] Invoking %s\n", resourceName.c_str());
            it->second.Func(std::forward<TArgs>(args)...);
        } else {
            printf("[Registry] Error: %s not found or function is null.\n", resourceName.c_str());
        }
    }

    bool Find(const std::string& resourceName) const {
        return mMap.find(resourceName) != mMap.end();
    }

    std::vector<const TInfo*> GetAllInfo() const {
        std::vector<const TInfo*> list;
        list.reserve(mMap.size());
        for (auto& pair : mMap) {
            list.push_back(&pair.second.Info);
        }
        return list;
    }

    void Clear() {
        mMap.clear();
        mCounter = 0;
    }

private:
    int32_t mCounter;
    struct Entry {
        TInfo Info;
        Callback Func;
    };

    // key: str, value: lambda function
    std::unordered_map<std::string, Registry::Entry> mMap;
};
