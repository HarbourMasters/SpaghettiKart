#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "World.hpp"

class GameObject {
public:
    uint8_t uuid[16];
    Object *o;

    virtual ~GameObject() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit GameObject();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
    //virtual World* GetWorld();
};

#endif // GAMEOBJECT_H