#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

extern "C" {
#include "common_structs.h"
}


// Note that this doesn't seem to work right
// Use add_actor_to_empty_slot(test, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
// to spawn stock actors
class ATree : public AActor {
public:

    Gfx* Displaylist;
    const char* Tlut;
    f32 DrawDistance;
    f32 MinDrawDistance;
    s16 Flags = -0x8000 | 0x4000;
    s16 State = 0x43;
    f32 BoundingBoxSize = 3.0f;
    f32 Unk_08 = 20.0f;

    virtual ~ATree() = default;

    // Set tlut to NULL if not using a tlut
    explicit ATree(Vec3f pos, Gfx* displaylist, f32 drawDistance, f32 minDrawDistance, const char* tlut);

    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void Collision(Player*, AActor*) override;
    virtual void Destroy() override;
};
