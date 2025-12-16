#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "assets/textures/other_textures.h"
}

extern Vtx gBowserStatueVtx[717];
extern Gfx gBowserStatueGfx[162];

// The data for this actor is generated and cut out from the actual track geography
// That generator is currently commented out. So this actor is not usable atm.
class ABowserStatue : public AActor {
public:
    enum Behaviour : int16_t {
        DEFAULT,
        CRUSH
    };

    virtual ~ABowserStatue() = default;
    explicit ABowserStatue(const SpawnParams& params);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual bool IsMod() override;
private:
    ABowserStatue::Behaviour mBehaviour;
};
