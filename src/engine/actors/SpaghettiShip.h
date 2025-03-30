#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "assets/other_textures.h"
}

class ASpaghettiShip : public AActor {
public:
    explicit ASpaghettiShip(FVector pos);
    virtual ~ASpaghettiShip() = default;

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual bool IsMod() override;

    FVector Spawn;
    IRotator WheelRot = {0, 0, 0};
};
