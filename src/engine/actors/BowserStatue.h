#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "assets/other_textures.h"
}

extern Vtx gBowserStatueVtx[717];
extern Gfx gBowserStatueGfx[162];

class ABowserStatue : public AActor {
  public:
    enum Behaviour { DEFAULT, CRUSH };

    virtual ~ABowserStatue() = default;
    explicit ABowserStatue(FVector pos, ABowserStatue::Behaviour behaviour);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual bool IsMod() override;

    FVector Pos;

  private:
    ABowserStatue::Behaviour _behaviour;
    f32 scale;
};
