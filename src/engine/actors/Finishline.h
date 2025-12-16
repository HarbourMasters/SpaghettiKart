#pragma once

#include <libultraship.h>
#include "CoreMath.h"
#include "RegisterContent.h"
#include "engine/Actor.h"

extern "C" {
#include "camera.h"
#include "common_structs.h"
}

class AFinishline : public AActor {
public:
    /**
     * Default behaviour places the finishline at the first waypoint.
     * @arg pos, optional. Sets a custom position
     */
    AFinishline(const SpawnParams& params);

    ~AFinishline() {
        _count--;
    }

    // This is simply a helper function to keep Spawning code clean
    static AFinishline* Spawn(FVector pos, IRotator rot) {
        SpawnParams params = {
            .Name = "mk:finishline",
            .Location = pos,
            .Rotation = rot,
        };
        return dynamic_cast<AFinishline*>(AddActorToWorld<AFinishline>(params));
    }

    static AFinishline* Spawn() {
        SpawnParams params = {
            .Name = "mk:finishline",
        };
        return dynamic_cast<AFinishline*>(AddActorToWorld<AFinishline>(params));
    }

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void BeginPlay() override;
    virtual void Collision(Player* player, AActor* actor) override;
    virtual bool IsMod() override;

    static size_t _count;
    bool PickedUp = false;
    uint32_t Timer = 0;
    
    Player* _player = NULL;
    
    f32 Hop = 3.0f;
    f32 Gravity = 200.0f;

    f32 OldHop = 0;
    f32 OldGravity = 0;

};
