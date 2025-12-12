#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "CoreMath.h"
#include "engine/World.h"
#include "RegisterContent.h"

extern "C" {
#include "common_structs.h"
}

class AMarioSign : public AActor {
public:

    virtual ~AMarioSign() = default;
    explicit AMarioSign(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static inline AMarioSign* Spawn(FVector pos, IRotator rot, FVector velocity, FVector scale) {
        SpawnParams params = {
            .Name = "mk:mario_sign",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Velocity = velocity,
            .Speed = 182,
        };
        return static_cast<AMarioSign*>(AddActorToWorld<AMarioSign>(params));
    }

    virtual bool IsMod() override;
    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
