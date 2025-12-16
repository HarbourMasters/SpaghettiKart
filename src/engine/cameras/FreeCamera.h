#pragma once

#include <libultraship.h>

#include "GameCamera.h"

extern "C" {
#include "camera.h"
}

class FreeCamera : public GameCamera {
public:
    FreeCamera(FVector pos, s16 rot, u32 mode);

    virtual void Tick() override;
    virtual void SetViewProjection() override;
    virtual void SetActive(bool state) override;
};
