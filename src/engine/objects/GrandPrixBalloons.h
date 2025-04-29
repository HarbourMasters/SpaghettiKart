#pragma once

#include <libultraship.h>
#include <vector>

#include "engine/World.h"
#include "engine/objects/Object.h"

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
 * Grand Prix Balloons OObject
 */
class OGrandPrixBalloons : public OObject {
public:

    explicit OGrandPrixBalloons(const FVector& pos);

    ~OGrandPrixBalloons() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_80053D74(s32 objectIndex, UNUSED s32 arg1, s32 vertexIndex);

    void func_80074924(s32 objectIndex);
    void func_80074D94(s32 objectIndex);
    void func_80074E28(s32 objectIndex);
    void func_80041480(s16* arg0, s16 arg1, s16 arg2, s16* arg3); // Some weird math function

private:
    static size_t _count;
    s32 _idx;
    FVector Pos;
    bool _active;
    size_t _numBalloons;
    size_t _numBalloons2;
    size_t _numBalloons3;
};
