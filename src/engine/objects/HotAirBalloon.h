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
    explicit OHotAirBalloon(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static inline OHotAirBalloon* Spawn(FVector pos) {
        SpawnParams params = {
            .Name = "mk:hot_air_balloon",
            .Location = pos,
        };
        return static_cast<OHotAirBalloon*>(GetWorld()->AddObject(new OHotAirBalloon(params)));
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void SetSpawnParams(SpawnParams& params) override;

    void func_80055CCC(s32 objectIndex, s32 cameraId);
    void init_hot_air_balloon(s32 objectIndex);
    void func_80085534(s32 objectIndex);
    void func_80085768(s32 objectIndex);

private:
    FVector Pos;
    bool *_visible;
};
