#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

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

class OMole : public OObject {
  public:
    enum Behaviour : uint16_t {};

  public:
    explicit OMole(Vec3f pos);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void func_80054E10(s32 objectIndex);
    void func_80054EB8();
    void func_80054F04(s32 cameraId);
    void render_object_moles(s32 cameraId);
    void func_80054D00(s32 objectIndex, s32 cameraId);
    void func_800821AC(s32 objectIndex, s32 arg1);
    void func_80081FF4(s32 objectIndex, s32 arg1);
    void func_80081D34(s32 objectIndex);
    void func_80081AFC(s32 objectIndex, s32 arg1);
    void func_80081A88(s32 objectIndex);
    void func_80081924(s32 objectIndex);
    void func_80081848(s32 objectIndex);
    void func_80081790(s32 objectIndex);

  private:
    s32 _idx;
    s32 _state;
    s32 _timer;
    s32 _status;
    bool _toggle;

    SplineData* spline;
};
