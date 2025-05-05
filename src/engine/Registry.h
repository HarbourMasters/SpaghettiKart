// #pragma once

// #include "Course.h"
// #include "Cup.h"
// #include <unordered_map>
// #include "AllActors.h"
// #include "Actor.h"
// #include "objects/Object.h"

// class AActor; // <-- Forward delare

// template <class T> class Registry {
//   public:
//   Registry();
//   void Add(std::string name, std::function<T*()>);
  
//   template <typename... Args>
//   void AddArgs(std::string id);
//   T* Get(std::string id);
// private:
//   std::unordered_map<std::string, std::function<T*()>> ContentVault;
//   std::map<std::string, std::function<T*(Args&&...)>> ArgsVault;
// };

// void AddCourse(std::string id, Course* course);
// void AddCup(std::string id, Cup* cup);
// void AddActor(std::string id, AActor* actor);

// void AddStockContent();

// extern Registry<Course> Courses;
// extern Registry<Cup> Cups;
// extern Registry<AActor> Actors;
