#include <libultraship.h>
#include <macros.h>

#include "code_80281C40.h"
#include "main.h"
#include "math_util.h"
#include "models/tracks/royal_raceway/royal_raceway_displaylists.h"
#include "skybox_and_splitscreen.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "menu_items.h"
#include "podium_ceremony_actors.h"
#include "ceremony_and_credits.h"
#include "podium_ceremony_actors.h"
#include "render_player.h"
#include "defines.h"
#include "memory.h"

#include "engine/Matrix.h"
#include "engine/courses/Course.h"
#include "port/Game.h"

struct UnkStruct80287560 {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
}; // 0xC

/** bss **/
s32 gGotoMenu;
s32 D_80287554;
s32 D_80281C40_pad[2];
struct UnkStruct80287560 D_80287560[30];

void func_80281C40(void) {
    s32 i;

    for (i = 0; i < D_802874FC; i++) {
        func_800579F8(D_80287560[i].unk0, D_80287560[i].unk2, (char*) D_80287560[i].unk8, D_80287560[i].unk4);
    }
}

void func_80281CB4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (D_802874FC < 0x1E) {
        D_80287560[D_802874FC].unk0 = arg0;
        D_80287560[D_802874FC].unk2 = arg1;
        D_80287560[D_802874FC].unk4 = arg3;
        D_80287560[D_802874FC].unk8 = arg2;
        D_802874FC++;
    }
}

extern Gfx D_80284F70[];
extern Gfx D_80284EE0[];

void func_80281D00(void) {
    Camera* camera = D_800DC5EC->camera;
    UNUSED s32 pad[3];
    u16 perspNorm;
    Mat4 matrix;
    UNUSED s32 pad2[3];

    func_802A53A4();
    init_rdp();
    if (gGotoMenu != 0xFFFF) {
        clear_framebuffer(0);
        if (D_80287554 >= 4) {
            gIsInQuitToMenuTransition = 0;
            gGamestateNext = gGotoMenu;
        }
        D_80287554++;
        return;
    }
    func_8028150C();
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective(camera->perspectiveMatrix, &perspNorm, gCameraFOV[0], gScreenAspect, CM_GetProps()->NearPersp, CM_GetProps()->FarPersp,
                  1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, camera->perspectiveMatrix,
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(camera->lookAtMatrix, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    gSPMatrix(gDisplayListHead++, camera->lookAtMatrix,
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    mtxf_identity(matrix);
    render_set_position(matrix, 0);

    gSPDisplayList(gDisplayListHead++, D_80284F70);
    render_players(camera, PLAYER_ONE);
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    update_actors_loop();
    render_object(D_800DC5EC);
    func_80021B0C();
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    func_80093F10();
    ceremony_transition_sliding_borders();
    func_80281C40();
    init_rdp();
}
