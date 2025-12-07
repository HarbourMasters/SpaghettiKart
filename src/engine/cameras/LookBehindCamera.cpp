#include <libultraship.h>
#include <libultra/gbi.h>
#include "LookBehindCamera.h"
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

LookBehindCamera::LookBehindCamera(FVector pos, s16 rot, u32 mode) : GameCamera() {
    _camera->renderMode = RENDER_TRACK_SECTIONS;
    ProjMode = ProjectionMode::PERSPECTIVE;
    bActive = true;

    // Look behind
    Vec3f spawn = {pos.x, pos.y, pos.z};
    camera_init(spawn, rot, mode, _camera->cameraId);

    // Flip camera to look backwards
    _camera->unk_30[2] = -_camera->unk_30[2];
    _camera->unk_3C[2] = -_camera->unk_3C[2];
}

void LookBehindCamera::Tick() {
    if (!bActive) { return; }

    if (nullptr == _camera) {
        bActive = false;
        return;
    }

    func_8001EE98(&gPlayers[_camera->playerId], _camera, _camera->cameraId);
}
