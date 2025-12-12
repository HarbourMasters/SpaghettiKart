#pragma once

#include <libultraship.h>
#include "Object.h"

#include "RegisterContent.h"
#include "World.h"

class OHotAirBalloon : public OObject {
public:
    explicit OHotAirBalloon(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static OHotAirBalloon* Spawn(FVector pos) {
        SpawnParams params = {
            .Name = "mk:hot_air_balloon",
            .Location = pos,
        };
        return dynamic_cast<OHotAirBalloon*>(AddObjectToWorld<OHotAirBalloon>(params));
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
