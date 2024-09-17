#pragma once

#include <libultraship.h>
#include "GameObject.h"
#include "Cup.h"

extern "C" {
#include "camera.h"
#include "objects.h"
};

class World {
public:
    //Actor actors;
    virtual ~World() = default;
    explicit World();

    

    // virtual Actor* SpawnActor(std::unique_ptr<GameActor> actor);

    virtual Object* SpawnObject(std::unique_ptr<GameObject> object);
    
    virtual Cup* AddCup(std::unique_ptr<Cup> cup);
    
    virtual void UpdateObjects();
    virtual void RenderObjects(Camera *camera);
    virtual void ExpiredObjects();
    virtual void DestroyObjects();
    virtual Object *GetObjectByIndex(size_t);

    std::vector<std::unique_ptr<Cup>> Cups;

    std::vector<std::unique_ptr<GameObject>> GameObjects;
    // std::vector<std::unique_ptr<GameActor>> GameActors;

};

extern World gWorldInstance;

extern "C" {
    
}
