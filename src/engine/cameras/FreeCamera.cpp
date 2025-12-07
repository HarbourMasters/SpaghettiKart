#include <libultraship.h>
#include <libultra/gbi.h>
#include "FreeCamera.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"
#include "port/Game.h"

#include "src/enhancements/freecam/freecam.h"

extern "C" {
#include "main.h"
#include "code_800029B0.h"
#include "camera.h"
#include "code_80057C60.h"
#include "defines.h"
}

FreeCamera::FreeCamera(FVector pos, s16 rot, u32 mode) : GameCamera() {
    _camera->renderMode = RENDER_FULL_SCENE;
    ProjMode = ProjectionMode::PERSPECTIVE;
    Vec3f spawn = {pos.x, pos.y, pos.z};
    freecam_init(spawn, rot, mode, _camera->cameraId);
    bActive = false;
}

void FreeCamera::SetActive(bool state) {
    bActive = state;
    if (state) {
        gIsHUDVisible = false;
        // gPlayerOne->type |= PLAYER_CPU;
    } else {
        gIsHUDVisible = true;
        gPlayerOne->type &= ~PLAYER_CPU;
    }
}

void FreeCamera::Tick() {
    if (!bActive) { return; }
    if (nullptr == _camera) {
        bActive = false;
        return;
    }

    freecam_loop(_camera);
}

void FreeCamera::SetViewProjection() {
    u16 perspNorm;
    Mat4 matrix;

    Mat4 persp;
    Mat4 lookAt;

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_CULL_BOTH | G_CULL_FRONT);

    // Perspective (camera movement)
    FrameInterpolation_RecordOpenChild("freecam_persp", FrameInterpolation_GetCameraEpoch());
    guPerspective(&PerspectiveMatrix, &perspNorm, 40, gScreenAspect,
                  CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, &PerspectiveMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    FrameInterpolation_RecordCloseChild();

    // LookAt (camera rotation)
    FrameInterpolation_RecordOpenChild("freecam_lookAt", FrameInterpolation_GetCameraEpoch());
    guLookAt(&LookAtMatrix,
        _camera->pos[0], _camera->pos[1], _camera->pos[2],
        _camera->lookAt[0], _camera->lookAt[1], _camera->lookAt[2],
        _camera->up[0], _camera->up[1], _camera->up[2]
    );
    gSPMatrix(gDisplayListHead++, &LookAtMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    FrameInterpolation_RecordCloseChild();

    gDPPipeSync(gDisplayListHead++);
}
