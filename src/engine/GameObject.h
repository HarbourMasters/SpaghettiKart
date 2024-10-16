#pragma once

#include <libultraship.h>

extern "C" {
    #include "camera.h"
    #include "objects.h"
}

class GameObject {
public:
    uint8_t uuid[16];
    Object o;

    virtual ~GameObject() = default;

    explicit GameObject();

    virtual void Tick();
    virtual void Draw(Camera* camera);
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
};
