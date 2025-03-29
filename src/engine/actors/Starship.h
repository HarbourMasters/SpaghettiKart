#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "assets/other_textures.h"
}

class AStarship : public AActor {
public:
    explicit AStarship(FVector pos);
    virtual ~AStarship() = default;

    virtual void Tick() override;
    virtual bool IsMod() override;

    FVector Spawn;
};
