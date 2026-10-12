#pragma once

#include <libultraship.h>

#include "GameCamera.h"

extern "C" {
#include "camera.h"
}

class LookBehindCamera : public GameCamera {
public:
    LookBehindCamera(FVector pos, s16 rot, u32 mode);
    void Tick() override;
};
