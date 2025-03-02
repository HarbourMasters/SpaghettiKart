#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"
#include "CoreMath.h"
#include "MoleGroup.h"

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

class OMoleGroup;

class OMole : public OObject {
public:
    explicit OMole(FVector pos, OMoleGroup* group);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void func_80054E10(s32 objectIndex);
    void func_80054EB8();
    void func_80054F04(s32 cameraId);
    void render_object_moles(s32 cameraId);
    void func_80054D00(s32 objectIndex, s32 cameraId);
    void func_800821AC(s32 objectIndex, s32 arg1);
    void func_80081D34(s32 objectIndex);
    void func_80081AFC(s32 objectIndex, s32 arg1);
    void func_80081A88(s32 objectIndex);
    void func_80081924(s32 objectIndex);
    void func_80081848(s32 objectIndex);
    void func_80081790(s32 objectIndex);
    void func_8008153C(s32 objectIndex);

    s32 _moleIndex;
private:
    static size_t _count;
    size_t _idx;
    OMoleGroup* _group;
};
