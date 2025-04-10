#pragma once

#include <libultraship/libultraship.h>
#include "CoreMath.h"
#include "GameObject.h"

namespace Editor {

class WaterVolumeObject : public GameObject {
public:
    WaterVolumeObject(const char* name, FVector* pos);

    virtual void Draw() override;
    virtual void Tick() override;
    virtual void Load() override;

    static size_t NumWaterVolumes;
    FVector WaterPos = FVector(0, 100, 0);
    IRotator WaterRot = IRotator(0, 0, 0);
	FVector WaterScale = FVector(0.1, 0.1, 0.1);
    s32 _despawnFlag = 0;
};
}
