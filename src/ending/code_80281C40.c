#include <libultraship.h>
#include <macros.h>

#include "code_80281C40.h"
#include "main.h"
#include "racing/math_util.h"
#include "models/tracks/royal_raceway/royal_raceway_displaylists.h"
#include "racing/skybox_and_splitscreen.h"
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
#include "engine/tracks/Track.h"
#include "port/Game.h"

struct PodiumDebugPrintData {
    s16 x;
    s16 y;
    s32 value;
    const char* str;
}; // 0xC

/** bss **/
s32 gGotoMenu;
s32 D_80287554;
s32 D_80281C40_pad[2];
static struct PodiumDebugPrintData sDebugPrintData[30];

void podium_debug_draw_prints(void) {
    s32 i;

    for (i = 0; i < gPodiumDebugPrintIdx; i++) {
        render_podium_debug_prints([i].x, sDebugPrintData[i].y, (char*) sDebugPrintData[i].str, sDebugPrintData[i].value);
    }
}

// Allows printing a str and value
UNUSED void podium_debug_print(s32 x, s32 y, const char* str, s32 value) {
    if (gPodiumDebugPrintIdx < 30) {
        sDebugPrintData[gPodiumDebugPrintIdx].x     = x;
        sDebugPrintData[gPodiumDebugPrintIdx].y     = y;
        sDebugPrintData[gPodiumDebugPrintIdx].value = value;
        sDebugPrintData[gPodiumDebugPrintIdx].str   = str;
        gPodiumDebugPrintIdx += 1;
    }
}

extern Gfx D_80284F70[];
extern Gfx D_80284EE0[];

void render_podium_ceremony(void) {
    Camera* camera = gScreenOneCtx->camera;
    UNUSED s32 pad[3];
    u16 perspNorm;
    Mat4 matrix;
    UNUSED s32 pad2[3];

    race_begin_viewport(gScreenOneCtx, 0);
    if ((CVarGetInteger("gDrawSky", true) == true)) {
        CM_RaceDrawSky(gScreenOneCtx, 0);
        func_80093A30(0); // Fill box for thunderbolt?
    }
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
    guPerspective(camera->perspectiveMatrix, &perspNorm, camera->fieldOfView, gScreenAspect, CM_GetProps()->NearPersp, CM_GetProps()->FarPersp,
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
    render_object(gScreenOneCtx);
    func_80021B0C();
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    func_80093F10();
    ceremony_transition_sliding_borders();

    // Example of debug print if it was used
    // podium_debug_print(10, 10, "my test str", 5);
    podium_debug_draw_prints();
    init_rdp();
}
