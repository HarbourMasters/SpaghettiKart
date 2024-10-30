#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"
}

class ACloud : public AActor {
public:


    // Constructor
    ACloud(Vec3f pos);

    virtual ~ACloud() override = default;

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void Collision(Player* player, AActor* actor) override;
    virtual bool IsMod() override;

    bool PickedUp = false;
    uint32_t Timer = 0;
    
    Player* _player = NULL;
    
    f32 Hop = 3.0f;
    f32 Gravity = 200.0f;

    f32 OldHop = 0;
    f32 OldGravity = 0;

};
