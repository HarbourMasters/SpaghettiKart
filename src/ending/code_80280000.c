#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <debug.h>
#include <mk64.h>

#include <main.h>
#include <segments.h>
#include <code_800029B0.h>
#include "camera.h"
#include "memory.h"
#include "math_util.h"
#include "code_80280000.h"
#include "code_80281780.h"
#include "skybox_and_splitscreen.h"
#include "menu_items.h"
#include "code_8006E9C0.h"
#include "code_800029B0.h"
#include "ceremony_and_credits.h"
#include "podium_ceremony_actors.h"
#include "code_80281C40.h"
#include "code_80057C60.h"
#include "actors.h"
#include "render_courses.h"
#include "main.h"
#include "render_player.h"

#include "engine/courses/Course.h"
#include "engine/Matrix.h"
#include "port/Game.h"

s32 D_802874A0;
// s32 D_802874A4[5];

void func_80280000(void) {
    func_802966A0();
    func_80059AC8();
    func_80059AC8();
    func_8005A070();
}

void func_80280038(Camera* camera) {
    u16 perspNorm;
    UNUSED s32 pad;
    Mat4 matrix;

    ClearMatrixPools();
    Editor_ClearMatrix();

    gMatrixObjectCount = 0;
    gMatrixEffectCount = 0;
    gMatrixHudCount = 0;
    init_rdp();
    func_802A53A4();
    init_rdp();
    func_80057FC4(0);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective(camera->perspectiveMatrix , &perspNorm, gCameraFOV[0], gScreenAspect, CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, camera->perspectiveMatrix,
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(camera->lookAtMatrix, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    gSPMatrix(gDisplayListHead++, camera->lookAtMatrix,
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gCurrentCourseId = gCreditsCourseId;
    SetCourseById(gCreditsCourseId);
    mtxf_identity(matrix);
    render_set_position(matrix, 0);
    render_course(D_800DC5EC);
    render_course_actors(D_800DC5EC);
    CM_DrawActors(camera);
    CM_DrawStaticMeshActors();
    render_object(D_800DC5EC);
    render_player_snow_effect(camera);
    ceremony_transition_sliding_borders();
    func_80281C40();
    init_rdp();
    func_80093F10();
    init_rdp();
}

void func_80280268(s32 courseId) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    D_802874A0 = 1;
    if ((courseId < 0) || ((courseId >= NUM_COURSES - 1))) {
        courseId = 0;
    }
    gCreditsCourseId = courseId;
}

void credits_loop(void) {
    Editor_ClearMatrix();
    CM_TickEditor();
    Camera* camera = D_800DC5EC->camera;

    f32 temp_f12;
    f32 temp;
    f32 temp_f14;

    D_802874A0 = 0;
    if (gIsInQuitToMenuTransition) {
        gQuitToMenuTransitionCounter--;
        if (gQuitToMenuTransitionCounter == 0) {
            gIsInQuitToMenuTransition = 0;
            gGamestateNext = CREDITS_SEQUENCE;
            gGamestate = 255;
        }
    } else {

        D_802874FC = 0;
        func_80283648(camera);
        temp_f12 = camera->lookAt[0] - camera->pos[0];
        temp = camera->lookAt[1] - camera->pos[1];
        temp_f14 = camera->lookAt[2] - camera->pos[2];
        camera->rot[1] = atan2s(temp_f12, temp_f14);
        camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), temp);
        camera->rot[2] = 0;
        if (D_802874A0 != 0) {
            D_800DC5E4++;
        } else {
            func_80280000();
            func_80280038(camera);
#if DVDL
            display_dvdl();
#endif
            gDPFullSync(gDisplayListHead++);
            gSPEndDisplayList(gDisplayListHead++);
        }
    }
}

void load_credits(void) {
    CM_CleanCameras();
    Vec3f spawn = {0.0f, 0.0f, 0.0f};
    Camera* camera = CM_AddCamera(spawn, 0, 0);
    if (NULL == camera) {
        return;
    }

    CM_AttachCamera(camera, PLAYER_ONE);
    D_800DC5EC->camera = camera;
    camera->renderMode = RENDER_FULL_SCENE;
    camera->unk_B4 = 60.0f;
    gCameraFOV[0] = 60.0f;


    gCurrentCourseId = gCreditsCourseId;
    SetCourseById(gCreditsCourseId);
    D_800DC5B4 = 1;
    func_802A4D18();
    set_screen();
    D_800DC5EC->screenWidth = SCREEN_WIDTH;
    D_800DC5EC->screenHeight = SCREEN_HEIGHT;
    D_800DC5EC->screenStartX = SCREEN_WIDTH / 2;
    D_800DC5EC->screenStartY = SCREEN_HEIGHT / 2;
    gScreenModeSelection = SCREEN_MODE_1P;
    gActiveScreenMode = SCREEN_MODE_1P;
    gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
    load_course(gCurrentCourseId);
    gFreeMemoryCourseAnchor = gNextFreeMemoryAddress;

    gCourseMinX = -0x15A1;
    gCourseMinY = -0x15A1;
    gCourseMinZ = -0x15A1;

    gCourseMaxX = 0x15A1;
    gCourseMaxY = 0x15A1;
    gCourseMaxZ = 0x15A1;
    D_8015F59C = 0;
    D_8015F5A0 = 0;
    D_8015F58C = 0;
    gCollisionMeshCount = 0;
    D_800DC5BC = 0;
    D_800DC5C8 = 0;
    gCollisionMesh = (CollisionTriangle*) gNextFreeMemoryAddress;
    camera->pos[0] = 1400.0f;
    camera->pos[1] = 300.0f;
    camera->pos[2] = 1400.0f;
    camera->lookAt[0] = 0.0f;
    camera->lookAt[1] = 0.0f;
    camera->lookAt[2] = 0.0f;
    camera->up[0] = 0.0f;
    camera->up[1] = 1.0f;
    camera->up[2] = 0.0f;
    init_cinematic_camera();
    credits_spawn_actors();
    init_hud();
    func_80093E60();
    func_80092688();
    if (D_800DC5EC) {}
    D_801625F8 = ((uintptr_t) gHeapEndPtr - gNextFreeMemoryAddress);
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
