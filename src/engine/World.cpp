#include <libultraship.h>
#include "World.h"
#include "Cup.h"

extern "C" {
   #include "camera.h"
   #include "objects.h"
   #include "main.h"
}

World::World() {}

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
