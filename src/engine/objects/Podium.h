#pragma once

#include <libultraship.h>
#include <vector>

#include "World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "common_structs.h"
#include "objects.h"
#include "course_offsets.h"
#include "some_data.h"
}

class OPodium : public OObject {
  public:
    enum Behaviour : uint16_t {};

  public:
    explicit OPodium(const FVector& pos);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_8008629C(s32 objectIndex, s32 arg1);
    void func_80086424(s32 objectIndex);
    void func_80086528(s32 objectIndex, s32 arg1);

  private:
    s32 _idx;
    FVector _pos;
};
