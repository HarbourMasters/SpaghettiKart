#include <libultraship.h>
#include "World.h"
#include "Cup.h"

extern "C" {
   #include "camera.h"
   #include "objects.h"
}

World::World() {}

Cup* World::AddCup(std::unique_ptr<Cup> cup) {
    Cup* tmp = cup.get();
    Cups.push_back(std::move(cup));
    return tmp;
}

u32 World::NextCup() {
    if (Cups.size() - 1) {
        return ++this->CupIndex;
    }
}

u32 World::PreviousCup() {
    if (Cups.size() > 0) {
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

World gWorldInstance;

World* GetWorld() {
    return &gWorldInstance;
}

extern "C" {
    u32 WorldNextCup() {
        return gWorldInstance.NextCup();
    }

    u32 WorldPreviousCup() {
        return gWorldInstance.PreviousCup();
    }
}