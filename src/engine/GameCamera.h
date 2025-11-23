#pragma once

#include <libultraship.h>

extern "C" {
#include "camera.h"
}

class GameCamera {
public:
    GameCamera(Camera* camera);
    GameCamera(Camera* camera, f32 posX, f32 posY, f32 posZ, u32 arg4, s32 cameraId);

    enum class ProjectionMode {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    ProjectionMode ProjMode;

    virtual void Tick();
    virtual void Setup();
    virtual void SetActive(bool state);

    void SetProjectionMode(GameCamera::ProjectionMode mode);
    bool IsActive();
    Camera* Get();

    Mtx* GetPerspMatrix();
    Mtx* GetLookAtMatrix();

protected:
    Mtx PerspectiveMatrix;
    Mtx LookAtMatrix;
    bool bActive;
    Camera* _camera;
};
