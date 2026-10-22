#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

class ABanana : public AActor {
public:

    uint16_t PlayerId;

    // Constructor
    ABanana(const SpawnParams& params);
    ~ABanana() override = default;

    // Virtual functions to be overridden by derived classes
    void Tick() override;
    void Draw(Camera*) override;
    void Collision(Player*, AActor*) override;
    void Destroy() override;
};
