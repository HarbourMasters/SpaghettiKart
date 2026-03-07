#pragma once

#include <libultraship.h>
#include "engine/registry/RegisterContent.h"
#include "engine/Actor.h"
#include "CoreMath.h"

class AWarioSign : public AActor {
public:

    virtual ~AWarioSign() {
        _count -= 1;
    };
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
private:
    static size_t _count;
    size_t _idx;
};
