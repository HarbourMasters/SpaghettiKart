#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "assets/other_textures.h"
}

class AShip : public AActor {
public:

    enum Skin {
        GHOSTSHIP,
        SHIP2,
        SHIP3,
    };

    explicit AShip(FVector pos, AShip::Skin);
    virtual ~AShip() = default;

    virtual void Tick() override;
    virtual bool IsMod() override;

    FVector Spawn;
    //FVector Pos;
    ///IRotator Rot = {0, 0, 0};
    //FVector Scale = {0.4, 0.4, 0.4};
private:
    Gfx* _skin;
};
