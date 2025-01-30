#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

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

class OFlagpole : public OObject {
public:
    explicit OFlagpole(const FVector& pos, s16 direction);

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
    FVector _pos;
    s16 _direction;
    static size_t _count;
    size_t _idx;
    s32 _objectIndex;
};
