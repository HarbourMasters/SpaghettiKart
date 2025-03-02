#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"
#include "World.h"
#include "engine/particles/StarEmitter.h"

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

class OTrophy : public OObject {
public:
    enum TrophyType {
        BRONZE,
        SILVER,
        GOLD,
        BRONZE_150,
        SILVER_150,
        GOLD_150,
    };

    enum Behaviour {
        PODIUM_CEREMONY,
        STATIONARY,
        ROTATE, // A dual-axis opposing rotation
        ROTATE2, // A single-axis rotation
        GO_FISH,
    };

    explicit OTrophy(const FVector& pos, TrophyType trophy, Behaviour bhv);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_80086700(s32 objectIndex);
    void func_80086940(s32 objectIndex);
    void func_80086C14(s32 objectIndex);
    void func_80086C6C(s32 objectIndex);

private:
    StarEmitter* _emitter;

    TrophyType _trophy;
    FVector _spawnPos;
    Behaviour _bhv;
    int8_t *_toggleVisibility;
    Vec3f _oldPos;
    bool _isMod = false;
};
