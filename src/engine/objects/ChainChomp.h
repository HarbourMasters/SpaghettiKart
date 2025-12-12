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

/**
 * Animation crash for custom courses
 */
class OChainChomp : public OObject {
public:
    explicit OChainChomp();
    explicit OChainChomp(const SpawnParams& params);


    ~OChainChomp() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_80055AB8(s32 objectIndex, s32 cameraId);

    void func_80085878(s32 objectIndex, s32 arg1);
    void func_800859C8(s32 objectIndex, s32 arg1);

private:
    FVector _pos;
    static size_t _count;
    size_t _idx;
};
