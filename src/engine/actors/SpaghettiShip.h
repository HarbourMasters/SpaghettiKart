#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"
#include "engine/Actor.h"
#include "engine/World.h"

extern "C" {
#include "common_structs.h"
#include "assets/textures/other_textures.h"
}

class ASpaghettiShip : public AActor {
public:
    explicit ASpaghettiShip(const SpawnParams& params);
    virtual ~ASpaghettiShip() = default;

    // This is simply a helper function to keep Spawning code clean
    static inline ASpaghettiShip* Spawn(FVector pos, IRotator rot, FVector scale) {
        SpawnParams params = {
            .Name = "hm:spaghetti_ship",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
        };
        return static_cast<ASpaghettiShip*>(gWorldInstance.AddActor(new ASpaghettiShip(params)));
    }

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual bool IsMod() override;

    IRotator WheelRot = {0, 0, 0};
};
