#pragma once

#include <libultraship.h>
#include <vector>
#include "RegisterContent.h"
#include "engine/objects/Object.h"
#include "CoreMath.h"
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

/**
 * @arg start x and z spawn location
 * @arg end x and z patrol location
 * 
 * Crab patrols between start and end.
 * The game automatically places the actor on the surface of the tracks geometry.
 * Therefore, providing a Y height is unnecessary.
 * 
 * Crab appears to have a maximum patrolling distance and will patrol between
 * end --> max distance rather than start --> end or start --> max distance.
 */
class OCrab : public OObject {
public:
    // This is simply a helper function to keep Spawning code clean
    static inline OCrab* Spawn(const FVector2D& start, const FVector2D& end) {
        SpawnParams params = {
            .Name = "mk:crab",
            .PatrolStart = start,
            .PatrolEnd = end,
        };
        return static_cast<OCrab*>(AddObjectToWorld<OCrab>(params));
    }

    explicit OCrab(const SpawnParams& params);

    ~OCrab() {
        _count--;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void DrawEditorProperties() override;

    void DrawModel(s32 cameraId);

    void init_ktb_crab(s32 objectIndex);
    void func_80082B34(s32 objectIndex);
    void func_80082C30(s32 objectIndex);
    void func_80082E18(s32 objectIndex);

private:
    FVector2D _start;
    FVector2D _end;
    static size_t _count;
    s32 _idx;
};
