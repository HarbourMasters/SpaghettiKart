#pragma once

#include <libultraship.h>
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"
#include "engine/sky/SkyActor.h"

extern "C" {
#include "common_structs.h"
#include "code_800029B0.h"
}

/**
 * Skybox clouds
 *
 * @cloudVariant The cloud texture to use
 */
class SkyCloud : public SkyActor {
public:
    SkyCloud(ScreenContext* screen, u16 cloudVariant, u16 posY, u16 rotY, u16 scalePercent);

    ~SkyCloud() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Draw(ScreenContext* ctx, s32 arg0) override;
    virtual void Tick() override;
private:
    static size_t _count;
    size_t _idx;
};
