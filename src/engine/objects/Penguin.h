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
#include "course_offsets.h"
}

class OPenguin : public OObject {
public:
    enum PenguinType : uint32_t {
        CHICK,
        ADULT,
        CREDITS,
        EMPEROR
    };

    enum Behaviour : uint16_t {
        DISABLED,
        STRUT, // Emperor penguin
        CIRCLE, // Waddle in a circle
        SLIDE3,
        SLIDE4,
        UNK, // unused
        SLIDE6,
    };

public:
    f32 Diameter = 0.0f; // Waddle in a circle around the spawn point at this diameter.
    uint16_t MirrorModeAngleOffset;

    explicit OPenguin(FVector pos, u16 direction, PenguinType type, Behaviour behaviour);

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void Reset() override;
private:
    void Behaviours(s32 objectIndex);
    void EmperorPenguin(s32 objectIndex);
    void func_80085080(s32 objectIndex);
    void func_8008502C(s32 objectIndex);
    void func_80084D2C(s32 objectIndex, s32 arg1);

    void InitEmperorPenguin(s32 objectIndex);
    void OtherPenguin(s32 objectIndex);
    void InitOtherPenguin(s32 objectIndex);

    static bool _toggle;
    PenguinType _type;
    Behaviour _bhv;
};
