#include <libultraship.h>
#include <macros.h>
#include "main.h"
#include <mk64.h>
#include <assets/models/common_data.h>
#include "src/port/interpolation/matrix.h"

#include "math_util.h"

extern s16 D_800E43A8;

// rsp init
UNUSED void gfx_func_80040D00(void) {
    D_800E43A8 = 0;

    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(gGfxPool), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gIdentityMatrix), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
