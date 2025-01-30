#pragma once

#include <libultraship.h>
#include <vector>
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
 * The game automatically places the actor on the course surface.
 * Therefore, providing a Y height is unnecessary.
 * 
 * Crab appears to have a maximum patrolling distance and will patrol between
 * end --> max distance rather than start --> end or start --> max distance.
 */
class OCrab : public OObject {
public:
    explicit OCrab(const FVector2D& start, const FVector2D& end);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
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
    s32 _objectIndex;
};
