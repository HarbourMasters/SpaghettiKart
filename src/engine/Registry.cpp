// #include <functional>
// #include <unordered_map>
// #include <string>

// #include "Registry.h"
// #include "engine/CoreMath.h"

// extern "C" {
// #include "actors.h"
// #include "actor_types.h"
// }

// template<typename... TArgs>
// void Registry<TArgs...>::Add(const std::string_view name, Callback func) {
//     mMap[name].func = std::move(func);
// }

// template<typename... TArgs>
// void Registry<TArgs...>::Invoke(const std::string name, TArgs... args) {
//     auto it = mMap.find(name);
//     if (it != mMap.end() && it->second.func) {
//         // Using C++ variadic template expansion to call the function
//         printf("[Registry] Invoking %s\n", name.c_str());
//         it->second.func(std::forward<TArgs>(args)...);
//     } else {
//         printf("[Registry] Error: %s not found or function is null.\n", name.c_str());
//     }
// }

// template<typename... TArgs>
// void Registry<TArgs...>::Find(const std::string_view name) {
//     return mMap.find(name) != mMap.end();
// }
