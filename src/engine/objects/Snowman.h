#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "common_structs.h"
#include "objects.h"
#include "camera.h"
#include "some_data.h"
}

class OSnowman : public OObject {
  public:
    explicit OSnowman(const FVector& pos);

    ~OSnowman() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void DrawHead(s32);
    void DrawBody(s32);

    void func_80083868(s32);
    void func_80083948(s32);
    void func_80083A94(s32);
    void func_80083B0C(s32);
    void func_80083C04(s32);
    void func_80083BE4(s32);
    void func_800836F0(Vec3f);
    bool func_80073D0C(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
    void func_80083538(s32 objectIndex, Vec3f arg1, s32 arg2, s32 arg3);
    void func_8008379C(s32 objectIndex);

  private:
    FVector _pos;
    static size_t _count;
    size_t _idx;
};
