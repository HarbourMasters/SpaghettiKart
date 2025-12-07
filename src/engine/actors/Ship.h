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

class AShip : public AActor {
public:

    enum Skin {
        GHOSTSHIP,
        SHIP2,
        SHIP3,
    };

    explicit AShip(const SpawnParams& params);
    virtual ~AShip() = default;

    // This is simply a helper function to keep Spawning code clean
    static inline AShip* Spawn(FVector pos, IRotator rot, FVector scale, int16_t skin) {
        SpawnParams params = {
            .Name = "hm:ship",
            .Type = skin, // which ship model to use
            .Location = pos,
            .Rotation = rot,
            .Scale = scale,
        };
        return static_cast<AShip*>(gWorldInstance.AddActor(new AShip(params)));
    }

    AShip::Skin SpawnSkin = Skin::GHOSTSHIP;

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void BeginPlay() override;
    virtual bool IsMod() override;
    virtual void DrawEditorProperties() override;
private:
    Gfx* _skin;
};
