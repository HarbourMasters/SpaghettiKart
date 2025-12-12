#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <mk64.h>
#include <stdio.h>

#include "skybox_and_splitscreen.h"
#include "code_800029B0.h"
#include <common_structs.h>
#include "memory.h"
#include "../camera.h"
#include <assets/models/common_data.h>
#include "render_player.h"
#include "code_80057C60.h"
#include "menu_items.h"
#include "actors.h"
#include "render_courses.h"
#include "math_util.h"
#include "main.h"
#include "menus.h"
#include "port/Engine.h"
#include "engine/Matrix.h"
#include "engine/tracks/Track.h"
#include "engine/editor/Editor.h"
#include "port/Game.h"
#include "math_util.h"
#include "port/interpolation/FrameInterpolation.h"
#include "engine/CoreMath.h"

Vp D_802B8880[] = {
    { { { 640, 480, 511, 0 }, { 640, 480, 511, 0 } } },
};

Vtx D_802B8890[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

Vtx D_802B8910[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

Vtx D_802B8990[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

Vtx D_802B8A10[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

void func_802A3730(ScreenContext* arg0) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s32 screenWidth = arg0->screenWidth * 2;
    s32 screenHeight = arg0->screenHeight * 2;
    s32 screenStartX = arg0->screenStartX * 4;
    s32 screenStartY = arg0->screenStartY * 4;

    arg0->viewport.vp.vscale[0] = screenWidth;
    arg0->viewport.vp.vscale[1] = screenHeight;
    arg0->viewport.vp.vscale[2] = 511;
    arg0->viewport.vp.vscale[3] = 0;

    arg0->viewport.vp.vtrans[0] = screenStartX;
    arg0->viewport.vp.vtrans[1] = screenStartY;
    arg0->viewport.vp.vtrans[2] = 511;
    arg0->viewport.vp.vtrans[3] = 0;

    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&arg0->viewport));

    screenWidth /= 4;
    screenHeight /= 4;

    screenStartX /= 4;
    screenStartY /= 4;

    lrx = screenStartX + screenWidth;
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }

    lry = screenStartY + screenHeight;
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    ulx = 0;
    uly = 0;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
}

UNUSED void func_802A38AC(void) {
}

void func_802A38B4(void) {
    init_rdp();
    select_framebuffer();

    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);

    if (gQuitToMenuTransitionCounter != 0) {
        gQuitToMenuTransitionCounter--;
        return;
    }
    gGamestateNext = gGotoMode;
    gGamestate = 255;
    gIsInQuitToMenuTransition = 0;
    gQuitToMenuTransitionCounter = 0;
    gFadeModeSelection = 1;

    switch (gGotoMode) {
        case START_MENU_FROM_QUIT:
            if (gMenuSelection != LOGO_INTRO_MENU) {
                gMenuSelection = START_MENU;
            }
            break;
        case MAIN_MENU_FROM_QUIT:
            gMenuSelection = MAIN_MENU;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            gMenuSelection = CHARACTER_SELECT_MENU;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            gMenuSelection = COURSE_SELECT_MENU;
            break;
    }
}

void func_802A39E0(ScreenContext* arg0) {
    s32 ulx = arg0->screenStartX - (arg0->screenWidth / 2);
    s32 uly = arg0->screenStartY - (arg0->screenHeight / 2);
    s32 lrx = arg0->screenStartX + (arg0->screenWidth / 2);
    s32 lry = arg0->screenStartY + (arg0->screenHeight / 2);

    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    if (ulx >= lrx) {
        lrx = ulx + 2;
    }
    if (uly >= lry) {
        lry = uly + 2;
    }

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    gDPSetFillColor(gDisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    gDPFillRectangle(gDisplayListHead++, ulx, uly, lrx - 1, lry - 1);

    gDPPipeSync(gDisplayListHead++);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer])); // 0x1FFFFFFF
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
}

/**
 * Initialize the z-buffer for the current frame.
 */
void init_z_buffer(void) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    gDPSetFillColor(gDisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(gDisplayListHead++, 0, 0, 319, 239);
    gDPPipeSync(gDisplayListHead++);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer]));
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
}

/**
 * Sets the initial RDP (Reality Display Processor) rendering settings.
 **/
void init_rdp(void) {
    gDPPipeSync(gDisplayListHead++);
    gDPPipelineMode(gDisplayListHead++, G_PM_1PRIMITIVE);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetTextureLOD(gDisplayListHead++, G_TL_TILE);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPSetTextureDetail(gDisplayListHead++, G_TD_CLAMP);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gDPSetTextureConvert(gDisplayListHead++, G_TC_FILT);
    gDPSetCombineKey(gDisplayListHead++, G_CK_NONE);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetColorDither(gDisplayListHead++, G_CD_DISABLE);
    gDPPipeSync(gDisplayListHead++);
    gSPClipRatio(gDisplayListHead++, FRUSTRATIO_1);
}

UNUSED void func_802A40A4(void) {
}
UNUSED void func_802A40AC(void) {
}
UNUSED void func_802A40B4(void) {
}
UNUSED void func_802A40BC(void) {
}
UNUSED void func_802A40C4(void) {
}
UNUSED void func_802A40CC(void) {
}
UNUSED void func_802A40D4(void) {
}
UNUSED void func_802A40DC(void) {
}

UNUSED s32 set_viewport2(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gScreenOneCtx->viewport));
    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++,
                       G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH | G_CLIPPING);
}

void set_viewport(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
}

/**
 * Tells the RDP which of the three framebuffers it shall draw to.
 */
void select_framebuffer(void) {
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer]));
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1) << 0x10 |
                                            GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1));
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void func_802A4300(void) {

    if (gActiveScreenMode == SCREEN_MODE_1P) {
        return;
    }
    if (D_800DC5B0 != 0) {
        return;
    }

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer]));
    gDPSetFillColor(gDisplayListHead++, 0x00010001);
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPPipeSync(gDisplayListHead++);

    switch (gActiveScreenMode) {
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            gDPFillRectangle(gDisplayListHead++, 157, 0, 159, 239);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            gDPFillWideRectangle(gDisplayListHead++, OTRGetDimensionFromLeftEdge(0), 119, OTRGetGameRenderWidth(), 121);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            gDPFillRectangle(gDisplayListHead++, 157, 0, 159, 239);
            gDPFillWideRectangle(gDisplayListHead++, OTRGetDimensionFromLeftEdge(0), 119, OTRGetGameRenderWidth(), 121);
            break;
    }
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

UNUSED Gfx D_802B8A90[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetFillColor(0x00000000),
    gsDPFillRectangle(0, 0, 319, 239),
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};

void func_802A450C(Vtx* skybox) {
    s32 i;

    if (bFog) {

        if (gFogColour.r < 0) {
            gFogColour.r = 0;
        }

        if (gFogColour.g < 0) {
            gFogColour.g = 0;
        }

        if (gFogColour.b < 0) {
            gFogColour.b = 0;
        }

        if (gFogColour.r > 255) {
            gFogColour.r = 255;
        }

        if (gFogColour.g > 255) {
            gFogColour.g = 255;
        }

        if (gFogColour.b > 255) {
            gFogColour.b = 255;
        }

        for (i = 0; i < 8; i++) {
            skybox[i].v.cn[0] = (s16) gFogColour.r;
            skybox[i].v.cn[1] = (s16) gFogColour.g;
            skybox[i].v.cn[2] = (s16) gFogColour.b;
        }
        return;
    }

    SkyboxColours* prop = (SkyboxColours*) &CM_GetProps()->Skybox;

    skybox[0].v.cn[0] = prop->TopRight.r;
    skybox[0].v.cn[1] = prop->TopRight.g;
    skybox[0].v.cn[2] = prop->TopRight.b;

    skybox[1].v.cn[0] = prop->BottomRight.r;
    skybox[1].v.cn[1] = prop->BottomRight.g;
    skybox[1].v.cn[2] = prop->BottomRight.b;

    skybox[2].v.cn[0] = prop->BottomLeft.r;
    skybox[2].v.cn[1] = prop->BottomLeft.g;
    skybox[2].v.cn[2] = prop->BottomLeft.b;

    skybox[3].v.cn[0] = prop->TopLeft.r;
    skybox[3].v.cn[1] = prop->TopLeft.g;
    skybox[3].v.cn[2] = prop->TopLeft.b;

    // Floor
    skybox[4].v.cn[0] = prop->FloorTopRight.r;
    skybox[4].v.cn[1] = prop->FloorTopRight.g;
    skybox[4].v.cn[2] = prop->FloorTopRight.b;

    skybox[5].v.cn[0] = prop->FloorBottomRight.r;
    skybox[5].v.cn[1] = prop->FloorBottomRight.g;
    skybox[5].v.cn[2] = prop->FloorBottomRight.b;

    skybox[6].v.cn[0] = prop->FloorBottomLeft.r;
    skybox[6].v.cn[1] = prop->FloorBottomLeft.g;
    skybox[6].v.cn[2] = prop->FloorBottomLeft.b;

    skybox[7].v.cn[0] = prop->FloorTopLeft.r;
    skybox[7].v.cn[1] = prop->FloorTopLeft.g;
    skybox[7].v.cn[2] = prop->FloorTopLeft.b;
}

Mtx gIdentityMatrix2 = {
    toFixedPointMatrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0),
};

void func_802A487C(Vtx* arg0, UNUSED ScreenContext* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                   UNUSED f32* arg4) {

    init_rdp();
    if (!IsRainbowRoad()) {

        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
        guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, &gIdentityMatrix2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListHead++, &arg0[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4A0C(Vtx* vtx, ScreenContext* arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED f32* arg4) {
    Camera* camera = arg1->camera;
    s16 temp_t5;
    f32 temp_f0;
    UNUSED s32 pad[2];
    UNUSED u16 pad2;
    u16 sp128;
    Mat4 matrix1 = { 0 };
    Mat4 matrix2 = { 0 };
    Mat4 matrix3 = { 0 };
    Vec3f sp5C;
    f32 sp58;

    func_802A450C(vtx);
    // Widescreen skybox
    vtx[0].v.ob[0] = OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH);
    vtx[1].v.ob[0] = OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH);
    vtx[2].v.ob[0] = OTRGetRectDimensionFromLeftEdge(0);
    vtx[3].v.ob[0] = OTRGetRectDimensionFromLeftEdge(0);

    vtx[4].v.ob[0] = OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH);
    vtx[5].v.ob[0] = OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH);
    vtx[6].v.ob[0] = OTRGetRectDimensionFromLeftEdge(0);
    vtx[7].v.ob[0] = OTRGetRectDimensionFromLeftEdge(0);

    sp5C[0] = 0.0f;
    sp5C[1] = 0.0f;
    sp5C[2] = 30000.0f;
    func_802B5564(matrix1, &sp128, camera->unk_B4, gScreenAspect, CM_GetProps()->NearPersp, CM_GetProps()->FarPersp,
                  1.0f);
    func_802B5794(matrix2, camera->pos, camera->lookAt);
    mtxf_multiplication(matrix3, matrix1, matrix2);

    sp58 = ((matrix3[0][3] * sp5C[0]) + (matrix3[1][3] * sp5C[1]) + (matrix3[2][3] * sp5C[2])) + matrix3[3][3];

    mtxf_translate_vec3f_mat4(sp5C, matrix3);

    temp_f0 = (1.0 / sp58);

    sp5C[0] *= temp_f0;
    sp5C[1] *= temp_f0;

    sp5C[0] *= 160.0f;
    sp5C[1] *= 120.0f;

    temp_t5 = 120 - (s32) sp5C[1];
    arg1->cameraHeight = temp_t5;
    vtx[1].v.ob[1] = temp_t5;
    vtx[2].v.ob[1] = temp_t5;
    vtx[4].v.ob[1] = temp_t5;
    vtx[7].v.ob[1] = temp_t5;

    init_rdp();
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
    guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &gIdentityMatrix2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (IsRainbowRoad()) {
        gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4D18(void) {
    // This may not be originally accurate
    // Because the near/far persp values are a track member variable now.
    // Regardless, this would likely only effected the menus.
    if (gGamestate != RACING) {
        D_8015014C = 6800.0f;
        D_80150150 = 3.0f;
    }

    switch (gScreenModeSelection) { /* switch 1; irregular */
        case SCREEN_MODE_1P:        /* switch 1 */
            gScreenAspect = 1.33333334f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL: /* switch 1 */
            gScreenAspect = 0.66666667f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: /* switch 1 */
            gScreenAspect = 2.66666667f;
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN: /* switch 1 */
            gScreenAspect = 1.33333334f;
            return;
    }
}

void func_802A4EF4(void) {
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            func_8001F394(gPlayerOne, &gCameraFOV[0]);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            func_8001F394(gPlayerOne, &gCameraFOV[0]);
            func_8001F394(gPlayerTwo, &gCameraFOV[1]);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            func_8001F394(gPlayerOne, &gCameraFOV[0]);
            func_8001F394(gPlayerTwo, &gCameraFOV[1]);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            func_8001F394(gPlayerOne, &gCameraFOV[0]);
            func_8001F394(gPlayerTwo, &gCameraFOV[1]);
            func_8001F394(gPlayerThree, &gCameraFOV[2]);
            func_8001F394(gPlayerFour, &gCameraFOV[3]);
            break;
    }
}

void func_802A5004(void) {

    init_rdp();
    func_802A3730(gScreenTwoCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);

    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    func_802A39E0(gScreenTwoCtx);
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80057FC4(2);
        func_802A487C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80093A30(2);
    }
}

void func_802A50EC(void) {

    init_rdp();
    func_802A3730(gScreenOneCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    func_802A39E0(gScreenOneCtx);
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, gScreenOneCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[0]);
        func_80057FC4(1);
        func_802A487C((Vtx*) D_802B8890, gScreenOneCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[0]);
        func_80093A30(1);
    }
}

void func_802A51D4(void) {

    init_rdp();
    func_802A39E0(gScreenOneCtx);
    func_802A3730(gScreenOneCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, gScreenOneCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[0]);
        func_80057FC4(3);
        func_802A487C((Vtx*) D_802B8890, gScreenOneCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[0]);
        func_80093A30(3);
    }
}

void func_802A52BC(void) {

    init_rdp();
    func_802A39E0(gScreenTwoCtx);
    func_802A3730(gScreenTwoCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80057FC4(4);
        func_802A487C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80093A30(4);
    }
}

void func_802A53A4(void) {
    init_rdp();
    func_802A3730(gScreenOneCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    init_z_buffer();
    select_framebuffer();
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, gScreenOneCtx, 0x140, 0xF0, &gCameraFOV[0]);
        if (gGamestate != CREDITS_SEQUENCE) {
            func_80057FC4(0);
        }
        func_802A487C((Vtx*) D_802B8890, gScreenOneCtx, 0x140, 0xF0, &gCameraFOV[0]);
        func_80093A30(0);
    }
}

void func_802A54A8(void) {

    init_rdp();
    func_802A39E0(gScreenOneCtx);
    func_802A3730(gScreenOneCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, gScreenOneCtx, 0x140, 0xF0, &gCameraFOV[0]);
        func_80057FC4(8);
        func_802A487C((Vtx*) D_802B8890, gScreenOneCtx, 0x140, 0xF0, &gCameraFOV[0]);
        func_80093A30(8);
    }
}

void func_802A5590(void) {

    init_rdp();
    func_802A39E0(gScreenTwoCtx);
    func_802A3730(gScreenTwoCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80057FC4(9);
        func_802A487C((Vtx*) D_802B8910, gScreenTwoCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[1]);
        func_80093A30(9);
    }
}

void func_802A5678(void) {

    init_rdp();
    func_802A39E0(gScreenThreeCtx);
    func_802A3730(gScreenThreeCtx);

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8990, gScreenThreeCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[2]);
        func_80057FC4(10);
        func_802A487C((Vtx*) D_802B8990, gScreenThreeCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[2]);
        func_80093A30(10);
    }
}

void func_802A5760(void) {

    init_rdp();

    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gPlayerCountSelection1 == 3) {

        gDPPipeSync(gDisplayListHead++);
        func_802A39E0(gScreenFourCtx);
        gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
        gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                         VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer]));
        gDPSetFillColor(gDisplayListHead++, 0x00010001);
        gDPPipeSync(gDisplayListHead++);
        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 160, 120, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPFillRectangle(gDisplayListHead++, 160, 120, OTRGetDimensionFromRightEdge(320), SCREEN_HEIGHT - 1);
        gDPPipeSync(gDisplayListHead++);
        gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

        func_802A3730(gScreenFourCtx);

    } else {
        func_802A3730(gScreenFourCtx);
        func_802A39E0(gScreenFourCtx);

        if (D_800DC5B4 != 0) {
            func_802A4A0C(D_802B8A10, gScreenFourCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[3]);
            func_80057FC4(11);
            func_802A487C(D_802B8A10, gScreenFourCtx, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraFOV[3]);
            func_80093A30(11);
        }
    }
}

void render_screens(s32 mode, s32 cameraId, s32 playerId) {
    Mat4 matrix;

    s32 screenId = 0;
    s32 screenMode = SCREEN_MODE_1P;

    switch (mode) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            func_802A53A4();
            screenId = 0;
            screenMode = SCREEN_MODE_1P;
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
            func_802A51D4();
            screenId = 0;
            screenMode = SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL;
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
            func_802A52BC();
            screenId = 1;
            screenMode = SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL;
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
            func_802A50EC();
            screenId = 0;
            screenMode = SCREEN_MODE_2P_SPLITSCREEN_VERTICAL;
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            func_802A5004();
            screenId = 1;
            screenMode = SCREEN_MODE_2P_SPLITSCREEN_VERTICAL;
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
            func_802A54A8();
            screenId = 0;
            screenMode = SCREEN_MODE_3P_4P_SPLITSCREEN;
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
            func_802A5590();
            screenId = 1;
            screenMode = SCREEN_MODE_3P_4P_SPLITSCREEN;
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
            func_802A5678();
            screenId = 2;
            screenMode = SCREEN_MODE_3P_4P_SPLITSCREEN;
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            func_802A5760();
            screenId = 3;
            screenMode = SCREEN_MODE_3P_4P_SPLITSCREEN;
            if (gPlayerCountSelection1 == 3) {
                func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
                if (D_800DC5B8 != 0) {
                    render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
                }
                gNumScreens += 1;
                return;
            }
            break;
    }

    ScreenContext* screen = &gScreenContexts[screenId];
    Camera* camera = screen->camera;
    cameraId = camera->cameraId;
    // CM_GetCamera(cameraId);

    if (NULL == camera) {
        printf("NO CAMERA SELECTED\n");
        return;
    }

    if (screenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    }

    init_rdp();
    func_802A3730(screen);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    // Setup camera perspective and lookAt
    CM_SetViewProjection(camera);

    // Create a matrix for the track and game objects
    FrameInterpolation_RecordOpenChild("track", TAG_TRACK((cameraId | (playerId << 2))));
    Mat4 trackMatrix;
    mtxf_identity(trackMatrix);
    if (gIsMirrorMode != 0) {
        trackMatrix[0][0] = -1.0f;
        // trackMatrix[1][1] = -1.0f;
        // trackMatrix[2][2] = -1.0f;
    }
    render_set_position(trackMatrix, 0);

    // Draw track geography
    render_track(screen);
    FrameInterpolation_RecordCloseChild();

    // Draw dynamic game objects
    render_course_actors(screen);
    CM_DrawActors(camera);
    CM_DrawStaticMeshActors();
    render_object(screen);

    if (CM_IsTourEnabled() == false) {
        render_players(camera, screenId);
    }

    func_8029122C(screen, playerId); // Track water related

    switch (playerId) { // Render player particles or some effect
        case 0:
            func_80021B0C();
            break;
        case 1:
            func_80021C78();
            break;
        case 2:
            func_80021D40();
            break;
        case 3:
            func_80021DA8();
            break;
    };

    render_item_boxes(screen);
    render_player_snow_effect(camera);
    func_80058BF4(); // Setup texture modes
    if (D_800DC5B8 != 0) {
        func_80058C20(mode); // Setup hud matrix
    }
    func_80093A5C(mode); // Perhaps pause render?
    if (D_800DC5B8 != 0) {
        render_hud(mode);
    }

    // Do not increment in single player mode
    if (mode != RENDER_SCREEN_MODE_1P_PLAYER_ONE) {
        gNumScreens += 1;
    }
}

// Makes the screen small at the start of a race
void set_screen(void) {
    ScreenContext* wrapper = &gScreenContexts[0];
    Player* player = &gPlayers[0];
    Camera* camera = &cameras[0];
    struct Controller* controller = &gControllers[0];

    // struct? size = 0x10. unk++ doesn't work cause s32 too small.
    s32* unk = &D_8015F790[0];
    s32 i;

    for (i = 0; i < 4; i++) {
        wrapper->controllers = controller;
//        wrapper->camera = &cameras[i]; // Done in spawn_players now
        wrapper->player = player;
        wrapper->unkC = unk;

        // Tick is not enabled in the editor, so the screen needs to begin at the proper size.
        if (((Editor_IsEnabled()) && (Editor_IsPaused()) && (i == PLAYER_ONE)) || CM_IsTourEnabled() == true) {
            wrapper->screenWidth = SCREEN_WIDTH;
            wrapper->screenHeight = SCREEN_HEIGHT;
        } else { // Normal race start, screen is small
            wrapper->screenWidth = 4;
            wrapper->screenHeight = 4;
        }
        wrapper->pathCounter = 1;

        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                if (i == 0) {
                    wrapper->screenStartX = 160;
                }
                wrapper->screenStartY = 120;
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                if (i == 0) {
                    wrapper->screenStartX = 80;
                    wrapper->screenStartY = 120;
                } else if (i == 1) {
                    wrapper->screenStartX = 240;
                    wrapper->screenStartY = 120;
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                if (i == 0) {
                    wrapper->screenStartX = 160;
                    wrapper->screenStartY = 60;
                } else if (i == 1) {
                    wrapper->screenStartX = 160;
                    wrapper->screenStartY = 180;
                }
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (i == 0) {
                    wrapper->screenStartX = 80;
                    wrapper->screenStartY = 60;
                } else if (i == 1) {
                    wrapper->screenStartX = 240;
                    wrapper->screenStartY = 60;
                } else if (i == 2) {
                    wrapper->screenStartX = 80;
                    wrapper->screenStartY = 180;
                } else {
                    wrapper->screenStartX = 240;
                    wrapper->screenStartY = 180;
                }
                break;
        }
        player++;
        camera++;
        wrapper++;
        unk += 0x10;
    }
}

void set_editor_screen(void) {
    ScreenContext* wrapper = &gScreenContexts[0];
    wrapper->controllers = gControllerOne;
    wrapper->camera = gFreecamCamera;
    wrapper->player = gPlayerOne;
    wrapper->unkC = &D_8015F790[0];
    wrapper->screenWidth = SCREEN_WIDTH;
    wrapper->screenHeight = SCREEN_HEIGHT;
    wrapper->pathCounter = 1;
}

void copy_framebuffer(s32 arg0, s32 arg1, s32 width, s32 height, u16* source, u16* target) {
    s32 var_v1;
    s32 var_a1;
    s32 targetIndex;
    s32 sourceIndex;

    targetIndex = 0;
    for (var_v1 = 0; var_v1 < height; var_v1++) {
        sourceIndex = ((arg1 + var_v1) * SCREEN_WIDTH) + arg0;
        for (var_a1 = 0; var_a1 < width; var_a1++, targetIndex++, sourceIndex++) {
            target[targetIndex] = source[sourceIndex];
        }
    }
}

void func_802A7728(void) {
    s16 temp_v0;

    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) sRenderedFramebuffer - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }

    if (NULL == gPhysicalFramebuffers[temp_v0]) {
        return;
    }

    copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture68272C));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682928));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682B24));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682D20));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682F1C));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture683118));
}

void func_802A7940(void) {
    s16 temp_v0;

    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) sRenderedFramebuffer - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }

    if (gPhysicalFramebuffers[temp_v0] == NULL) {
        return;
    }

    copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture68272C));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682928));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682B24));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682D20));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture682F1C));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) LOAD_ASSET_RAW(gTexture683118));
}
