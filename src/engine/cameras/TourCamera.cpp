#include <libultraship/libultraship.h>
#include "TourCamera.h"
#include "port/Engine.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"
#include "engine/World.h"
#include "port/audio/HMAS.h"

extern "C" {
#include "main.h"
#include "camera.h"
#include "code_800029B0.h"
#include "ceremony_and_credits.h"
#include "common_structs.h"
#include "spawn_players.h"
#include "math_util.h"
#include "external.h"
}

TourCamera::TourCamera(FVector pos, s16 rot, u32 mode) : GameCamera() {
    _camera->renderMode = RENDER_FULL_SCENE;
    ProjMode = ProjectionMode::PERSPECTIVE;
    Vec3f spawn = {pos.x, pos.y, pos.z};
    freecam_init(spawn, rot, mode, _camera->cameraId);

    Type = TOUR_TYPE::SEQUENTIAL;
    ShotIndex = 0;
    KeyFrameIndex = 0;
    KeyFrameProgress = 0;
    bShotComplete = false;
    bTourComplete = false;
    bActive = false;
    bActivateAudio = true;
    Alpha = 0;
}

void TourCamera::Reset() {
    Type = TOUR_TYPE::SEQUENTIAL;
    KeyFrameIndex = 0;
    KeyFrameProgress = 0.0f;
    bShotComplete = false;
}

void TourCamera::NextShot() {
    TourCamera::Reset();
    bShotComplete = false;
    _camera->pos[0] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].Pos.x;
    _camera->pos[1] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].Pos.y;
    _camera->pos[2] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].Pos.z;
    _camera->lookAt[0] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].LookAt.x;
    _camera->lookAt[1] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].LookAt.y;
    _camera->lookAt[2] = gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].LookAt.z;
}

void TourCamera::Stop() {
    printf("[TourCamera] End of Track Tour\n");
    gTourComplete = true;
    CM_ResetAudio();

    D_8015F480[0].pendingCamera = &cameras[0];
    bActive = false;
    bTourComplete = true;

    CM_CameraSetActive(0, true);
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = RACING;
    if(HMAS_IsPlaying(HMAS_MUSIC)) {
        HMAS_Stop(HMAS_MUSIC);
    }
}

void TourCamera::Tick() {
    if (!bActive) { return; }
    if (
          (nullptr == _camera) ||
          (bTourComplete) ||
          (ShotIndex >= gWorldInstance.GetCurrentCourse()->TourShots.size())
       ) {
        Alpha += 5;
        if (Alpha == 255) {
            TourCamera::Stop();
        }
        return;
    }

    f32 extraArg; // Calculates camera up. Likely for preventing some camera flip related issues

    if (bShotComplete) {
        switch(Type) {
            case TOUR_TYPE::SEQUENTIAL:
                TourCamera::NextShot();
                break;
            case TOUR_TYPE::RANDOM:
                break;
            case TOUR_TYPE::LOOP:
                break;
        }
    }

    if (bActivateAudio) {
        bActivateAudio = false;
        enum MusicSeq sequence = CM_GetProps()->Sequence;

        if (sequence != MUSIC_SEQ_UNKNOWN) {
            play_sequence(sequence);
        }
    }

    bool done = TourCamera::MoveCameraAlongSpline(&extraArg, gWorldInstance.GetCurrentCourse()->TourShots[ShotIndex].Frames);

    // Advance to the next camera shot
    if (done) {
        ShotIndex += 1;
    }

    //printf("cam %f %f %f prog %f idx %d\n", camera->pos[0], camera->pos[1], camera->pos[2], Progress, KeyFrameIndex);

    bShotComplete = done;

    // This is required for actors billboarding to work.
    _camera->rot[1] = atan2s(_camera->lookAt[0] - _camera->pos[0], _camera->lookAt[2] - _camera->pos[2]);
}

bool TourCamera::MoveCameraAlongSpline(f32* arg1, std::vector<KeyFrame>& keyFrame) {
    bool finished = false;
    Mat4 matrix;
    Mat4 lookAtMatrix;
    s32 i = 0;
    f32 u = KeyFrameProgress;
    f32 progressChange;
    f32 firstSpeed = 0;
    f32 secondSpeed = 0;
    s32 keyFrameIdx = KeyFrameIndex;

    f32 unk = 1.0f;

    if (KeyFrameIndex < 0) {
        u = 0;
    }

    // Check for end of the spline
    if ( (keyFrameIdx + 3) >= keyFrame.size() ) {
        return true;
    }

    for (i = 0; i < 4; i++) {
        matrix[i][0] = keyFrame[keyFrameIdx + i].Pos.x;
        matrix[i][1] = keyFrame[keyFrameIdx + i].Pos.y;
        matrix[i][2] = keyFrame[keyFrameIdx + i].Pos.z;
        matrix[i][3] = unk * 256.0f; //spline[segment + i].unk4 * 256.0f;

        lookAtMatrix[i][0] = keyFrame[keyFrameIdx + i].LookAt.x;
        lookAtMatrix[i][1] = keyFrame[keyFrameIdx + i].LookAt.y;
        lookAtMatrix[i][2] = keyFrame[keyFrameIdx + i].LookAt.z;
        lookAtMatrix[i][3] = unk * 256.0f; //spline[segment + i].unk4 * 256.0f;
    }

    evaluate_cubic_spline(u, _camera->pos, arg1, matrix[0], matrix[1], matrix[2], matrix[3]);
    evaluate_cubic_spline(u, _camera->lookAt, arg1, lookAtMatrix[0], lookAtMatrix[1], lookAtMatrix[2], lookAtMatrix[3]);

    if (keyFrame[KeyFrameIndex + 1].Duration != 0) {
        firstSpeed = 1.0f / keyFrame[KeyFrameIndex + 1].Duration;
    }

    if (keyFrame[KeyFrameIndex + 2].Duration != 0) {
        secondSpeed = 1.0f / keyFrame[KeyFrameIndex + 2].Duration;
    }

    progressChange = (((secondSpeed - firstSpeed)) * KeyFrameProgress + firstSpeed);
    if (1 <= (KeyFrameProgress += progressChange)) {
        KeyFrameIndex++;
        if ( (KeyFrameIndex + 3 ) >= keyFrame.size()) {
            KeyFrameIndex = 0;
            finished = true;
        }
        KeyFrameProgress--;
    }
    return finished;
}

bool TourCamera::IsTourComplete() {
    return bTourComplete;
}

void TourCamera::SetViewProjection() {
    u16 perspNorm;
    Mat4 matrix;

    Mat4 persp;
    Mat4 lookAt;

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_CULL_BOTH | G_CULL_FRONT);

    // Perspective (camera movement)
    FrameInterpolation_RecordOpenChild("tourcam_persp", FrameInterpolation_GetCameraEpoch());
    guPerspective(&PerspectiveMatrix, &perspNorm, 40, gScreenAspect,
                  CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, &PerspectiveMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    FrameInterpolation_RecordCloseChild();

    // LookAt (camera rotation)
    FrameInterpolation_RecordOpenChild("tourcam_lookAt", FrameInterpolation_GetCameraEpoch());
    guLookAt(&LookAtMatrix, _camera->pos[0], _camera->pos[1], _camera->pos[2], _camera->lookAt[0],
             _camera->lookAt[1], _camera->lookAt[2], _camera->up[0], _camera->up[1], _camera->up[2]);
    gSPMatrix(gDisplayListHead++, &LookAtMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    FrameInterpolation_RecordCloseChild();

    gDPPipeSync(gDisplayListHead++);
}

void TourCamera::Draw() {
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0, 0, 0, Alpha);
    gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPFillWideRectangle(gDisplayListHead++, OTRGetRectDimensionFromLeftEdge(0), 0, OTRGetGameRenderWidth(), 239);
    gDPPipeSync(gDisplayListHead++);
}
