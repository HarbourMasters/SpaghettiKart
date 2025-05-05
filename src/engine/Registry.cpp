// #include <vector>
// #include <functional>
// #include <iostream>
// #include <map>

// #include "Registry.h"
// #include "Course.h"
// #include "port/Game.h"

// template <class T> Registry<T>::Registry() {
// }

// template <class T> void Registry<T>::Add(std::string id, std::function<T*()> fn) {
//     // need to handle duplicate registration
//     ContentVault[id] = fn;
// }

// template <class T>
// void Registry<T>::AddArgs(std::string id) {
//     ArgsVault[id] = [](Args&&... args) -> T* {
//         return new T(std::forward<Args>(args)...);
//     };
// }

// template <class T>
// T* Registry<T>::Get(std::string id) {
//     auto it = ContentVault.find(id);
//     if (it != ContentVault.end()) {
//         return it->second();
//     }
//     return nullptr;
// }

// // Available Registries
// Registry<Course> Courses;
// Registry<Cup> Cups;
// Registry<AActor> Actors;

// void AddCourse(std::string id, Course* course) {
//     Courses.Add(id, [course]() { return course; });
//     course->Id = id;
// }

// void AddCup(std::string id, Cup* cup) {
//     Cups.Add(id, [cup]() { return cup; });
//     //cup->Id = id;
// }

// void AddActor(std::string id, AActor* actor) {
//     Actors.Add(id, [actor]() { return actor; });
// }

// void AddStockContent() {
//     AddActor("mk:banana", new AMarioSign({0, 0, 0}));
// }

// template class Registry<Course>;
// template class Registry<Cup>;
// template class Registry<AActor>;
