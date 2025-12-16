#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "RegisterContent.h"
#include "World.h"
#include "CoreMath.h"

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
 * OBat
 * 
 * IRotator does not appear to do anything.
 * Could not find where origin_pos was at.
 * So pos does not work either
 * 
 */
class OBat : public OObject {
public:

    // This is simply a helper function to keep Spawning code clean
    static inline OBat* Spawn(const FVector& pos, const IRotator& rot) {
        SpawnParams params = {
            .Name = "mk:bat",
            .Location = pos,
            .Rotation = rot,
        };
        return dynamic_cast<OBat*>(AddObjectToWorld<OBat>(params));
    }

    explicit OBat(const SpawnParams& params);

    ~OBat() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void func_80046F60(u8* tlut, u8* arg1, s32 arg2, s32 arg3, s32 arg4);

    void func_8007D8AC(s32 arg0);
    s32 func_8007D804(s32 objectIndex);
    void func_8007D8D4(s32 objectIndex, s32 arg1);
    void func_8007DA74(s32 objectIndex);
    void func_8007DAF8(s32 objectIndex, s32 arg1);
    void func_80089A04(s32 objectIndex, f32 arg1, f32 arg2);
    void func_8007D794(s32 objectIndex);
    void func_8007DA4C(s32 objectIndex);

private:
    FVector _pos;
    static size_t _count;
    size_t _idx;
};
