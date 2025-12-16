#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

class ABanana : public AActor {
public:

    uint16_t PlayerId;

    // Constructor
    ABanana(const SpawnParams& params);
    virtual ~ABanana() override = default;

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void Collision(Player*, AActor*) override;
    virtual void Destroy() override;
};
