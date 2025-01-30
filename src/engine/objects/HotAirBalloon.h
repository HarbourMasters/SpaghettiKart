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

class OHotAirBalloon : public OObject {
public:
    explicit OHotAirBalloon(const FVector& pos);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_80055CCC(s32 objectIndex, s32 cameraId);
    void init_hot_air_balloon(s32 objectIndex);
    void func_80085534(s32 objectIndex);
    void func_80085768(s32 objectIndex);

private:
    FVector _pos;
    bool *_visible;
    s32 _objectIndex;
};
