#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "common_structs.h"
#include "objects.h"
#include "course_offsets.h"
#include "some_data.h"
}

class OCheepCheep : public OObject {
  public:
    enum CheepType { RACE, PODIUM_CEREMONY };

    enum Behaviour : uint16_t {};

  public:
    explicit OCheepCheep(const FVector& pos, CheepType type, IPathSpan span);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_8007BBBC(s32 objectIndex);
    void func_8007BD04(s32 playerId);
    void init_var_cheep_cheep(s32 objectIndex);
    void func_8007BEC8(s32 objectIndex);
    void func_8007BFB0(s32 objectIndex);

  private:
    s32 _idx;
    CheepType _type;
    FVector _spawnPos;
    IPathSpan _span;
};
