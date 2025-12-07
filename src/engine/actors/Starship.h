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

class AStarship : public AActor {
public:
    explicit AStarship(const SpawnParams& params);
    virtual ~AStarship() = default;

    // This is simply a helper function to keep Spawning code clean
    static inline AStarship* Spawn(FVector pos, IRotator rot, FVector scale, f32 speed, f32 radius) {
        SpawnParams params = {
            .Name = "hm:starship",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Speed = speed,
            .SpeedB = radius,
        };
        return static_cast<AStarship*>(gWorldInstance.AddActor(new AStarship(params)));
    }

    float SpeedB;

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual bool IsMod() override;
    virtual void BeginPlay() override;
    virtual void DrawEditorProperties() override;
};
