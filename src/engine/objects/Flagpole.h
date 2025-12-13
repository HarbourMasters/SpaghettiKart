#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "RegisterContent.h"
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

// This used to use directional_angle for rot. It now uses orientation for editor compatibility.
// There doesn't seem to be any reason this actor's behaviour would differ from this
class OFlagpole : public OObject {
public:
    explicit OFlagpole(const SpawnParams& params);

    // This is simply a helper function to keep Spawning code clean
    static OFlagpole* Spawn(FVector pos, s16 direction) {
        IRotator rot;
        rot.Set(0, direction, 0);
        SpawnParams params = {
            .Name = "mk:flagpole",
            .Location = pos,
            .Rotation = rot,
        };
        return dynamic_cast<OFlagpole*>(AddObjectToWorld<OFlagpole>(params));
    }

    ~OFlagpole() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;

    void func_80055164(s32 objectIndex);
    void func_80082F1C(s32 objectIndex);
    void func_80083018(s32 objectIndex);
    void func_80083060(s32 objectIndex);

private:
    static size_t _count;
    size_t _idx;
};
