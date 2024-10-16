#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

class ABanana : public AActor {
public:

    uint16_t PlayerId;

    // Constructor
    ABanana(uint16_t playerId, const float pos[3], const s16 rot[3], const float velocity[3]);

    virtual ~ABanana() override = default;

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void Collision() override;
    virtual void Destroy() override;
};
