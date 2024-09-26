#include <libultraship.h>
#include "World.h"
#include "Cup.h"
#include "Course.h"

extern "C" {
   #include "camera.h"
   #include "objects.h"
   #include "main.h"
   #include "engine/Engine.h"
}

World::World() {}

Course* CurrentCourse;

Cup* World::AddCup() {
    // Create a new unique_ptr for Cup
    auto cup = std::make_unique<Cup>();

    // Get raw pointer before moving the ownership
    Cup* tmp = cup.get();

    // Add the Cup to the container
    Cups.push_back(std::move(cup));

    // Return the raw pointer to the Cup
    return tmp;
}

//const char* World::GetCupName() {
//    //return this->Cups[CupIndex].Name;
//}

void World::SetCupIndex(int16_t courseId) {

    this->CupIndex = courseId;
}

u32 World::GetCupIndex() {
    return this->CupIndex;
}

u32 World::NextCup() {
    s32 hack = 1;

    // Prevent battle mode
    if (gModeSelection == GRAND_PRIX) {
        hack = 2;
    }

    if (this->CupIndex < Cups.size() - 2) {
        return ++this->CupIndex;
    }
}

u32 World::PreviousCup() {
    if (CupIndex > 0) {
        return --this->CupIndex;
    }
}

CProperties* World::GetCourseProps() {
    if (Courses[CourseIndex]) {
        return (CProperties*) &Courses[CourseIndex]->Props;
    }
    return nullptr;
}

void World::SetCourse(const char*name) {
    for (size_t i = 0; i < Courses.size(); i++) {
        if (Courses[i]->Props.Name == name) {
            CurrentCourse = Courses[i].get();
            break;
        }
    }
    std::runtime_error("SetCourse() Course name not found in Courses list");
};

void World::NextCourse() {
    if (CourseIndex < Courses.size() - 1) {
        CourseIndex++;
    } else {
        CourseIndex = 0;
    }
    gWorldInstance.CurrentCourse = Courses[CourseIndex].get();
}

void World::PreviousCourse() {
    if (CourseIndex > 0) {
        CourseIndex--;
    } else {
        CourseIndex = Courses.size() - 1;
    }
    gWorldInstance.CurrentCourse = Courses[CourseIndex].get();
}

Object* World::SpawnObject(std::unique_ptr<GameObject> object) {
    GameObject* rawPtr = object.get();
    this->GameObjects.push_back(std::move(object));
    return &rawPtr->o;
}

void World::UpdateObjects() {
    for (const auto& object : this->GameObjects) {
        object->Update();
    }
}

void World::RenderObjects(Camera *camera) {
    for (const auto& object : this->GameObjects) {
        object->Render(camera);
    }
}

void World::ExpiredObjects() {
    this->GameObjects.erase(
        std::remove_if(this->GameObjects.begin(), this->GameObjects.end(),
                        [](const std::unique_ptr<GameObject>& object) { return object->uuid == 0; }), // Example condition
        this->GameObjects.end());
}

void World::DestroyObjects() {

}

Object* World::GetObjectByIndex(size_t index) {
    if (index < this->GameObjects.size()) {
        // Assuming GameActor::a is accessible, use reinterpret_cast if needed
        return reinterpret_cast<Object*>(&this->GameObjects[index]->o);
    }
    return nullptr; // Or handle the error as needed
}
