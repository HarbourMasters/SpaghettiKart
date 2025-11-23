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

GameCamera::GameCamera(Camera* camera) {
    _camera = camera;
    _camera->perspectiveMatrix = &PerspectiveMatrix;
    _camera->lookAtMatrix = &LookAtMatrix;
}
GameCamera::GameCamera(Camera* camera, f32 posX, f32 posY, f32 posZ, u32 arg4, s32 cameraId) {
    _camera = camera;
    camera->renderMode = RENDER_TRACK_SECTIONS;
    camera->perspectiveMatrix = &PerspectiveMatrix;
    camera->lookAtMatrix = &LookAtMatrix;
    bActive = true;
    ProjMode = ProjectionMode::PERSPECTIVE;
    //camera_init(posX, posY, posZ, 0, arg4, cameraId);
}

void GameCamera::Tick() {
    if (!bActive) { return; }
    if (nullptr == _camera) {
        bActive = false;
        return;
    }

    func_8001EE98(gPlayerOne, _camera, 0);
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

void GameCamera::Setup() {
    u16 perspNorm;

    // Tag the camera for the interpolation engine
    FrameInterpolation_RecordOpenChild("camera",
                                       (FrameInterpolation_GetCameraEpoch() | ((_camera->cameraId << 8))));

    // Calculate camera perspective (camera movement/location)
    guPerspective(&PerspectiveMatrix, &perspNorm, gCameraZoom[_camera->cameraId], gScreenAspect,
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
