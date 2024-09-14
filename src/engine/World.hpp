#ifndef WORLD_H
#define WORLD_H

#include <libultraship.h>
#include "GameObject.hpp"


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

    std::vector<std::unique_ptr<GameObject>> GameObjects;
    // std::vector<GameActor> GameActors;

};

#endif // WORLD_H