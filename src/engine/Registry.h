#pragma once

#include <libultraship.h>

template<typename... TArgs>
// ^ Scary template key word
class Registry {
public:
    /**
     * Callback explanation
     * 
     * This is just a lambda function.
     * 
     * Usage: gRegistry.Add("hm:harbour", []() { // My code here })
     */
    using Callback = std::function<void(TArgs...)>;

    void Add(const std::string& name, Callback func) {
        mMap[name].func = std::move(func);
    }

    void Invoke(const std::string name, TArgs... args) {
        auto it = mMap.find(name);
        if (it != mMap.end() && it->second.func) {
            // Using C++ variadic template expansion to call the function
            printf("[Registry] Invoking %s\n", name.c_str());
            it->second.func(std::forward<TArgs>(args)...);
        } else {
            printf("[Registry] Error: %s not found or function is null.\n", name.c_str());
        }
    }

    void Find(const std::string_view name) {
        return mMap.find(name) != mMap.end();
    }

private:
    struct Entries {
        Callback func;
    };

    // key: str, value: lambda function
    std::unordered_map<std::string, Registry::Entries> mMap;
};
