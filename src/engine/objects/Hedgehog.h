#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "RegisterContent.h"
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
 * @arg behaviour unknown, seems unused.
 */
class OHedgehog : public OObject {
public:
    explicit OHedgehog(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static inline OHedgehog* Spawn(const FVector& pos, const FVector2D& patrolPoint, s16 behaviour) {
        SpawnParams params = {
            .Name = "mk:hedgehog",
            .Behaviour = behaviour, // Appears to be unused
            .Location = pos,
            .PatrolEnd = patrolPoint,
        };
        return static_cast<OHedgehog*>(AddObjectToWorld<OHedgehog>(params));
    }

    ~OHedgehog() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void DrawEditorProperties() override;

    void func_800555BC(s32 objectIndex, s32 cameraId);
    void func_8004A870(s32 objectIndex, f32 arg1);

    void func_8008311C(s32 objectIndex, s32 arg1);
    void func_80083248(s32 objectIndex);
    void func_800833D0(s32 objectIndex, s32 arg1);
    void func_80083474(s32 objectIndex);


private:
    FVector2D PatrolEnd;
    static size_t _count;
    size_t _idx;
};
