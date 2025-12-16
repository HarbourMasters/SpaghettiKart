#pragma once

#include <libultraship.h>
#include "RegisterContent.h"
#include "engine/Actor.h"
#include "CoreMath.h"

class AWarioSign : public AActor {
public:

    virtual ~AWarioSign() = default;
    explicit AWarioSign(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static AWarioSign* Spawn(FVector pos, IRotator rot, FVector velocity, FVector scale) {
        SpawnParams params = {
            .Name = "mk:wario_sign",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Velocity = velocity,
            .Speed = 182,
        };
        return dynamic_cast<AWarioSign*>(AddActorToWorld<AWarioSign>(params));
    }

    virtual bool IsMod() override;
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
