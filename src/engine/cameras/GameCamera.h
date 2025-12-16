#pragma once

#include <libultraship.h>
#include "CoreMath.h"

extern "C" {
#include "camera.h"
}

class GameCamera {
public:
    GameCamera();
    GameCamera(FVector spawn, s16 rot, u32 mode);
    ~GameCamera() {
        _count--;
    }

    enum class ProjectionMode {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    ProjectionMode ProjMode;

    virtual void Tick();
    virtual void SetViewProjection();
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
    static size_t _count;
};
