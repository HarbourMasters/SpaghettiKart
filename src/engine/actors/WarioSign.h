#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "CoreMath.h"
#include "engine/World.h"

extern "C" {
#include "common_structs.h"
}

class AWarioSign : public AActor {
public:

    virtual ~AWarioSign() = default;
    explicit AWarioSign(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static inline AWarioSign* Spawn(FVector pos, IRotator rot, FVector velocity, FVector scale) {
        SpawnParams params = {
            .Name = "mk:wario_sign",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Velocity = velocity,
            .Speed = 182,
        };
        return static_cast<AWarioSign*>(GetWorld()->AddActor(new AWarioSign(params)));
    }

    virtual bool IsMod() override;
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
