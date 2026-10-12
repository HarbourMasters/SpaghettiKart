#pragma once

#include <libultraship.h>

#include "GameCamera.h"

extern "C" {
#include "camera.h"
}

class FreeCamera : public GameCamera {
public:
    FreeCamera(FVector pos, s16 rot, u32 mode);

    void Tick() override;
    void SetViewProjection() override;
    void SetActive(bool state) override;
};
