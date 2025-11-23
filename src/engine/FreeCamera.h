#pragma once

#include <libultraship.h>

#include "GameCamera.h"

extern "C" {
#include "camera.h"
}

class FreeCamera : public GameCamera {
public:
    FreeCamera(Camera* camera, f32 posX, f32 posY, f32 posZ, u32 arg4, s32 cameraId);

    virtual void Tick() override;
    virtual void Setup() override;
    virtual void SetActive(bool state) override;
    void Deactivate();
};
