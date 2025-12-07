#include <libultraship.h>
#include <libultra/gbi.h>
#include "GameCamera.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"
#include "port/Game.h"

extern "C" {
#include "main.h"
#include "code_800029B0.h"
#include "camera.h"
}

size_t GameCamera::_count = 0;

GameCamera::GameCamera() {
    _camera = &cameras[_count];
    _camera->cameraId = _count;
    _camera->perspectiveMatrix = &PerspectiveMatrix;
    _camera->lookAtMatrix = &LookAtMatrix;

    _count += 1;
}

GameCamera::GameCamera(FVector pos, s16 rot, u32 mode) {
    _camera = &cameras[_count];
    _camera->cameraId = _count;
    _camera->perspectiveMatrix = &PerspectiveMatrix;
    _camera->lookAtMatrix = &LookAtMatrix;
    switch(gGamestate) {
        case RACING:
            _camera->renderMode = RENDER_TRACK_SECTIONS;
            break;
        case ENDING:
        case CREDITS_SEQUENCE:
            _camera->renderMode = RENDER_FULL_SCENE;
            break;
    }
    ProjMode = ProjectionMode::PERSPECTIVE;
    bActive = true;
    if (gGamestate != CREDITS_SEQUENCE) {
        Vec3f spawn = {pos.x, pos.y, pos.z};
        camera_init(spawn, rot, mode, _camera->cameraId);
    }

    _count += 1;
}

void GameCamera::Tick() {
    if (!bActive) { return; }
    if (nullptr == _camera) {
        bActive = false;
        return;
    }

    func_8001EE98(&gPlayers[_camera->playerId], _camera, _camera->cameraId);
}

void GameCamera::SetActive(bool state) {
    bActive = state;
}

bool GameCamera::IsActive() {
    return bActive;
}

Camera* GameCamera::Get() {
    return _camera;
}

void GameCamera::SetProjectionMode(GameCamera::ProjectionMode mode) {
    ProjMode = mode;
}

Mtx* GameCamera::GetPerspMatrix() {
    return &PerspectiveMatrix;
}

Mtx* GameCamera::GetLookAtMatrix() {
    return &LookAtMatrix;
}

void GameCamera::SetViewProjection() {
    u16 perspNorm;

    // Tag the camera for the interpolation engine
    FrameInterpolation_RecordOpenChild("camera",
                                       (FrameInterpolation_GetCameraEpoch() | ((_camera->cameraId << 8))));

    // Calculate camera perspective (camera movement/location)
    guPerspective(&PerspectiveMatrix, &perspNorm, gCameraFOV[_camera->cameraId], gScreenAspect,
                  CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, &PerspectiveMatrix,
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    // Calculate the camera lookAt (camera rotation)
    guLookAt(&LookAtMatrix, _camera->pos[0], _camera->pos[1], _camera->pos[2], _camera->lookAt[0],
             _camera->lookAt[1], _camera->lookAt[2], _camera->up[0], _camera->up[1], _camera->up[2]);
    gSPMatrix(gDisplayListHead++, &LookAtMatrix,
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    FrameInterpolation_RecordCloseChild();
}
