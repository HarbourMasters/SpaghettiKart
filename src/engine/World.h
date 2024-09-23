#pragma once

#include <libultraship.h>
#include "GameObject.h"
#include "Cup.h"

extern "C" {
#include "camera.h"
#include "objects.h"
};

class Cup; // <-- Forward declaration

class World {
public:
    //Actor actors;
    virtual ~World() = default;
    explicit World();

    // virtual Actor* SpawnActor(std::unique_ptr<GameActor> actor);

    virtual Object* SpawnObject(std::unique_ptr<GameObject> object);


    virtual void UpdateObjects();
    virtual void RenderObjects(Camera *camera);
    virtual void ExpiredObjects();
    virtual void DestroyObjects();
    virtual Object *GetObjectByIndex(size_t);

    Cup* AddCup();
    const char* GetCupName();
    virtual u32 GetCupIndex();
    virtual void SetCupIndex(int16_t courseId);
    virtual u32 NextCup();
    virtual u32 PreviousCup();

    World* GetWorld();

    std::vector<std::unique_ptr<Cup>> Cups;
    size_t CupIndex = 0;

    std::vector<std::unique_ptr<GameObject>> GameObjects;
    // std::vector<std::unique_ptr<GameActor>> GameActors;

};

extern World gWorldInstance;

extern "C" {
    
}
