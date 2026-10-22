#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/CoreMath.h"
#include "engine/registry/RegisterContent.h"
#include "engine/Actor.h"
#include "engine/World.h"

extern "C" {
#include "common_structs.h"
#include "assets/textures/other_textures.h"
}

class AStarship : public AActor {
public:
    explicit AStarship(const SpawnParams& params);
    ~AStarship() override = default;

    // This is simply a helper function to keep Spawning code clean
    static AStarship* Spawn(FVector pos, IRotator rot, FVector scale, f32 speed, f32 radius) {
        SpawnParams params = {
            .Name = "hm:starship",
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
            .Speed = speed,
            .SpeedB = radius,
        };
        return dynamic_cast<AStarship*>(AddActorToWorld<AStarship>(params));
    }

    float SpeedB;

    void SetSpawnParams(SpawnParams& params) override;
    void Tick() override;
    bool IsMod() override;
    void BeginPlay() override;
    void DrawEditorProperties() override;
};
