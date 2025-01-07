#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "engine/World.h"

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

/**
 * @arg pos FVector xyz spawn position
 * @arg patrolPoint FVector2D xz patrol to location. Actor automatically calculates the Y value
 * @arg unk unknown. Likely actor type.
 */
class OHedgehog : public OObject {
  public:
    explicit OHedgehog(const FVector& pos, const FVector2D& patrolPoint, s16 unk);

    ~OHedgehog() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void func_800555BC(s32 objectIndex, s32 cameraId);
    void func_8004A870(s32 objectIndex, f32 arg1);

    void func_8008311C(s32 objectIndex, s32 arg1);
    void func_80083248(s32 objectIndex);
    void func_800833D0(s32 objectIndex, s32 arg1);
    void func_80083474(s32 objectIndex);

  private:
    FVector _pos;
    static size_t _count;
    size_t _idx;
};
