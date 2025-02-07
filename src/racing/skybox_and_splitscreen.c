#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <mk64.h>

#include "skybox_and_splitscreen.h"
#include "code_800029B0.h"
#include <common_structs.h>
#include "memory.h"
#include "../camera.h"
#include <assets/common_data.h>
#include "render_player.h"
#include "code_80057C60.h"
#include "menu_items.h"
#include "actors.h"
#include "render_courses.h"
#include "math_util.h"
#include "main.h"
#include "menus.h"
#include "port/Engine.h"
#include "engine/courses/Course.h"
#include "port/Game.h"
#include "math_util.h"

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

void func_802A3730(struct UnkStruct_800DC5EC* arg0) {
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

void func_802A39E0(struct UnkStruct_800DC5EC* arg0) {
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
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_800DC5EC->viewport));
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

    if (D_800DC5BC != 0) {

        if (D_801625EC < 0) {
            D_801625EC = 0;
        }

        if (D_801625F4 < 0) {
            D_801625F4 = 0;
        }

        if (D_801625F0 < 0) {
            D_801625F0 = 0;
        }

        if (D_801625EC > 255) {
            D_801625EC = 255;
        }

        if (D_801625F4 > 255) {
            D_801625F4 = 255;
        }

        if (D_801625F0 > 255) {
            D_801625F0 = 255;
        }

        for (i = 0; i < 8; i++) {
            skybox[i].v.cn[0] = (s16) D_801625EC;
            skybox[i].v.cn[1] = (s16) D_801625F4;
            skybox[i].v.cn[2] = (s16) D_801625F0;
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

void func_802A487C(Vtx* arg0, UNUSED struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                   UNUSED f32* arg4) {

    init_rdp();
    if (GetCourse() != GetRainbowRoad()) {

        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
        guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListHead++, &arg0[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4A0C(Vtx* vtx, struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED f32* arg4) {
    //! @todo Confirm if this crash still happens and fix if so
    s32 id = arg1 - D_8015F480;
    arg1->camera = &cameras[id]; // bad fix of bowser castle crash where camera get an invalid value
    Camera* camera = arg1->camera;
    s16 temp_t5;
    f32 temp_f0;
    UNUSED s32 pad[2];
    UNUSED u16 pad2;
    u16 sp128;
    Mat4 matrix1;
    Mat4 matrix2;
    Mat4 matrix3;
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

    temp_t5 = 120 - (s16) sp5C[1];
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
    gSPMatrix(gDisplayListHead++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (GetCourse() == GetRainbowRoad()) {
        gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4D18(void) {
    if (gGamestate != RACING) {
        D_8015014C = 6800.0f;
        D_80150150 = 3.0f;
    } else {
        // switch (gCurrentCourseId) {
        //     case COURSE_BOWSER_CASTLE:
        //     case COURSE_BANSHEE_BOARDWALK:
        //     case COURSE_RAINBOW_ROAD:
        //     case COURSE_BLOCK_FORT:
        //     case COURSE_SKYSCRAPER:
        //         D_8015014C = 2700.0f;
        //         D_80150150 = 2.0f;
        //         break;
        //     case COURSE_CHOCO_MOUNTAIN:
        //     case COURSE_DOUBLE_DECK:
        //         D_8015014C = 1500.0f;
        //         D_80150150 = 2.0f;
        //         break;
        //     case COURSE_KOOPA_BEACH:
        //         D_8015014C = 5000.0f;
        //         D_80150150 = 1.0f;
        //         break;
        //     case COURSE_WARIO_STADIUM:
        //         D_8015014C = 4800.0f;
        //         D_80150150 = 10.0f;
        //         break;
        //     case COURSE_MARIO_RACEWAY:
        //     case COURSE_YOSHI_VALLEY:
        //     case COURSE_FRAPPE_SNOWLAND:
        //     case COURSE_ROYAL_RACEWAY:
        //     case COURSE_LUIGI_RACEWAY:
        //     case COURSE_MOO_MOO_FARM:
        //     case COURSE_TOADS_TURNPIKE:
        //     case COURSE_SHERBET_LAND:
        //     case COURSE_DK_JUNGLE:
        //         D_8015014C = 4500.0f;
        //         D_80150150 = 9.0f;
        //         break;
        //     case COURSE_KALIMARI_DESERT:
        //         D_8015014C = 7000.0f;
        //         D_80150150 = 10.0f;
        //         break;
        //     default:
        //         D_8015014C = 6800.0f;
        //         D_80150150 = 3.0f;
        //         break;
        // }
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
            func_8001F394(gPlayerOne, &gCameraZoom[0]);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            func_8001F394(gPlayerOne, &gCameraZoom[0]);
            func_8001F394(gPlayerTwo, &gCameraZoom[1]);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            func_8001F394(gPlayerOne, &gCameraZoom[0]);
            func_8001F394(gPlayerTwo, &gCameraZoom[1]);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            func_8001F394(gPlayerOne, &gCameraZoom[0]);
            func_8001F394(gPlayerTwo, &gCameraZoom[1]);
            func_8001F394(gPlayerThree, &gCameraZoom[2]);
            func_8001F394(gPlayerFour, &gCameraZoom[3]);
            break;
    }
}

void func_802A5004(void) {

    init_rdp();
    func_802A3730(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);

    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    func_802A39E0(D_800DC5F0);
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80057FC4(2);
        func_802A487C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80093A30(2);
    }
}

void func_802A50EC(void) {

    init_rdp();
    func_802A3730(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    func_802A39E0(D_800DC5EC);
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        func_80057FC4(1);
        func_802A487C((Vtx*) D_802B8890, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        func_80093A30(1);
    }
}

void func_802A51D4(void) {

    init_rdp();
    func_802A39E0(D_800DC5EC);
    func_802A3730(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        func_80057FC4(3);
        func_802A487C((Vtx*) D_802B8890, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        func_80093A30(3);
    }
}

void func_802A52BC(void) {

    init_rdp();
    func_802A39E0(D_800DC5F0);
    func_802A3730(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80057FC4(4);
        func_802A487C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80093A30(4);
    }
}

void func_802A53A4(void) {

    move_segment_table_to_dmem();
    init_rdp();
    func_802A3730(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    init_z_buffer();
    select_framebuffer();
    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        if (gGamestate != CREDITS_SEQUENCE) {
            func_80057FC4(0);
        }
        func_802A487C((Vtx*) D_802B8890, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        func_80093A30(0);
    }
}

void func_802A54A8(void) {

    init_rdp();
    func_802A39E0(D_800DC5EC);
    func_802A3730(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8890, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        func_80057FC4(8);
        func_802A487C((Vtx*) D_802B8890, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        func_80093A30(8);
    }
}

void func_802A5590(void) {

    init_rdp();
    func_802A39E0(D_800DC5F0);
    func_802A3730(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80057FC4(9);
        func_802A487C((Vtx*) D_802B8910, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        func_80093A30(9);
    }
}

void func_802A5678(void) {

    init_rdp();
    func_802A39E0(D_800DC5F4);
    func_802A3730(D_800DC5F4);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (D_800DC5B4 != 0) {
        func_802A4A0C((Vtx*) D_802B8990, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[2]);
        func_80057FC4(10);
        func_802A487C((Vtx*) D_802B8990, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[2]);
        func_80093A30(10);
    }
}

void func_802A5760(void) {

    init_rdp();

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gPlayerCountSelection1 == 3) {

        gDPPipeSync(gDisplayListHead++);
        func_802A39E0(D_800DC5F8);
        gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
        gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                         VIRTUAL_TO_PHYSICAL(gPhysicalFramebuffers[sRenderingFramebuffer]));
        gDPSetFillColor(gDisplayListHead++, 0x00010001);
        gDPPipeSync(gDisplayListHead++);
        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 160, 120, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPFillRectangle(gDisplayListHead++, 160, 120, OTRGetDimensionFromRightEdge(320), SCREEN_HEIGHT - 1);
        gDPPipeSync(gDisplayListHead++);
        gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

        func_802A3730(D_800DC5F8);

    } else {
        func_802A3730(D_800DC5F8);
        func_802A39E0(D_800DC5F8);

        if (D_800DC5B4 != 0) {
            func_802A4A0C(D_802B8A10, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[3]);
            func_80057FC4(11);
            func_802A487C(D_802B8A10, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[3]);
            func_80093A30(11);
        }
    }
}

void render_screens(s32 mode, s32 cameraId, s32 playerId) {
    UNUSED s32 pad[4];
    u16 perspNorm;
    UNUSED s32 pad2[2];
    UNUSED s32 pad3;
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

    struct UnkStruct_800DC5EC* screen = &D_8015F480[screenId];
    Camera* camera = &cameras[cameraId];

    if (screenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    }

    init_rdp();
    func_802A3730(screen);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    guPerspective(&gGfxPool->mtxPersp[cameraId], &perspNorm, gCameraZoom[cameraId], gScreenAspect,
                  CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);

    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[cameraId]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gGfxPool->mtxLookAt[cameraId], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[cameraId]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[cameraId]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(screen);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[cameraId]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(screen);
    render_object(mode);
    switch (screenId) {
        case 0:
            render_players_on_screen_one();
            break;
        case 1:
            render_players_on_screen_two();
            break;
        case 2:
            render_players_on_screen_three();
            break;
        case 3:
            render_players_on_screen_four();
            break;
    }
    func_8029122C(screen, playerId);

    switch (playerId) {
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
    render_player_snow_effect(mode);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(mode);
    }
    func_80093A5C(mode);
    if (D_800DC5B8 != 0) {
        render_hud(mode);
    }

    // Do not increment in single player mode
    if (mode != RENDER_SCREEN_MODE_1P_PLAYER_ONE) {
        gNumScreens += 1;
    }
}

void func_802A74BC(void) {
    struct UnkStruct_800DC5EC* wrapper = &D_8015F480[0];
    Player* player = &gPlayers[0];
    Camera* camera = &cameras[0];
    struct Controller* controller = &gControllers[0];

    // struct? size = 0x10. unk++ doesn't work cause s32 too small.
    s32* unk = &D_8015F790[0];
    s32 i;

    for (i = 0; i < 4; i++) {
        wrapper->controllers = controller;
        wrapper->camera = camera;
        wrapper->player = player;
        wrapper->unkC = unk;
        wrapper->screenWidth = 4;
        wrapper->screenHeight = 4;
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

// Handles copying framebuffer to course texture data region for the Jumbotron in the port
// This only supports framebuffers and copy regions being within 320x240
// Any attempt to support larger sizes would require reworking course data
void copy_jumbotron_fb_port(s32 ulx, s32 uly, s16 portionToDraw, u16* source, u16* target) {
    // Add CVar if we want to expose a user toggle for only updating 1/6 of the jumbotron per frame
    u8 updateWholeJumbo = true;

    if (portionToDraw == -1 || updateWholeJumbo) {
        copy_framebuffer(ulx, uly, 64, 32, source, target);
        copy_framebuffer(ulx + 64, uly, 64, 32, source, target + (64 * 32 * 1));
        copy_framebuffer(ulx, uly + 32, 64, 32, source, target + (64 * 32 * 2));
        copy_framebuffer(ulx + 64, uly + 32, 64, 32, source, target + (64 * 32 * 3));
        copy_framebuffer(ulx, uly + 64, 64, 32, source, target + (64 * 32 * 4));
        copy_framebuffer(ulx + 64, uly + 64, 64, 32, source, target + (64 * 32 * 5));
    } else {
        switch (portionToDraw) {
            case 0:
                copy_framebuffer(ulx, uly, 64, 32, source, target);
                break;
            case 1:
                copy_framebuffer(ulx + 64, uly, 64, 32, source, target + (64 * 32 * 1));
                break;
            case 2:
                copy_framebuffer(ulx, uly + 32, 64, 32, source, target + (64 * 32 * 2));
                break;
            case 3:
                copy_framebuffer(ulx + 64, uly + 32, 64, 32, source, target + (64 * 32 * 3));
                break;
            case 4:
                copy_framebuffer(ulx, uly + 64, 64, 32, source, target + (64 * 32 * 4));
                break;
            case 5:
                copy_framebuffer(ulx + 64, uly + 64, 64, 32, source, target + (64 * 32 * 5));
                break;
            default:
                break;
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
#if TARGET_N64
    copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x8800));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x9800));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xA800));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xB800));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xC800));
    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xD800));
#else
    copy_jumbotron_fb_port(D_800DC5DC, D_800DC5E0, -1, PHYSICAL_TO_VIRTUAL(gPortFramebuffers[temp_v0]),
                           (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x8800));
#endif
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
#if TARGET_N64
    copy_framebuffer(D_800DC5DC, D_800DC5E0, 0x40, 0x20, (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xF800));
    copy_framebuffer(D_800DC5DC + 0x40, D_800DC5E0, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x10800));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x11800));
    copy_framebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x12800));
    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x13800));
    copy_framebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x14800));
#else
    copy_jumbotron_fb_port(D_800DC5DC, D_800DC5E0, -1, (u16*) PHYSICAL_TO_VIRTUAL(gPortFramebuffers[temp_v0]),
                           (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xF800));
#endif
}
