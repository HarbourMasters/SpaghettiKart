#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"
}

class ACoin : public AActor {
public:


    // Constructor
    ACoin(const float pos[3]);

    virtual ~ACoin() override = default;

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void Collision(Player* player, AActor* actor) override;
    virtual bool IsMod() override;

    bool PickedUp = false;
    uint32_t Timer = 0;

};
