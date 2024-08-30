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
#include "code_80091750.h"
#include "actors.h"
#include "render_courses.h"
#include "math_util.h"
#include "main.h"
#include "menus.h"
#include "port/Engine.h"

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
    D_800E86A4 = 1;

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
            gMenuSelection = PLAYER_SELECT_MENU;
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
            gDPFillRectangle(gDisplayListHead++, 0, 119, 319, 121);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            gDPFillRectangle(gDisplayListHead++, 157, 0, 159, 239);
            gDPFillRectangle(gDisplayListHead++, 0, 119, 319, 121);
            break;
    }
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

struct Skybox {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

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

struct Skybox sSkyColors[] = {
#include "assets/course_metadata/sSkyColors.inc.c"

};

// struct Skybox sSkyColors[] = {
//     {128, 4280, 6136, 216, 7144, 32248},
//     {255, 255, 255, 255, 255, 255},
//     {48, 1544, 49528, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0},
//     {113, 70, 255, 255, 184, 99},
//     {28, 11, 90, 0, 99, 164},
//     {48, 1688, 54136, 216, 7144, 32248},
//     {238, 144, 255, 255, 224, 240},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 18, 255, 197, 211, 255},
//     {0, 2, 94, 209, 65, 23},
//     {195, 231, 255, 255, 0xc0, 0},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 0, 0, 0, 0, 0},
//     {20, 30, 56, 40, 60, 110},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 0, 0, 0, 0, 0},
//     {113, 70, 255, 255, 184, 99},
//     {255, 174, 0, 255, 229, 124},
//     {0, 0, 0, 0, 0, 0},
//     {238, 144, 255, 255, 224, 240},
// };

struct Skybox sSkyColors2[] = {
#include "assets/course_metadata/sSkyColors2.inc.c"
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
    skybox[0].v.cn[0] = sSkyColors[gCurrentCourseId].unk0;
    skybox[0].v.cn[1] = sSkyColors[gCurrentCourseId].unk2;
    skybox[0].v.cn[2] = sSkyColors[gCurrentCourseId].unk4;
    skybox[1].v.cn[0] = sSkyColors[gCurrentCourseId].unk6;
    skybox[1].v.cn[1] = sSkyColors[gCurrentCourseId].unk8;
    skybox[1].v.cn[2] = sSkyColors[gCurrentCourseId].unkA;
    skybox[2].v.cn[0] = sSkyColors[gCurrentCourseId].unk6;
    skybox[2].v.cn[1] = sSkyColors[gCurrentCourseId].unk8;
    skybox[2].v.cn[2] = sSkyColors[gCurrentCourseId].unkA;
    skybox[3].v.cn[0] = sSkyColors[gCurrentCourseId].unk0;
    skybox[3].v.cn[1] = sSkyColors[gCurrentCourseId].unk2;
    skybox[3].v.cn[2] = sSkyColors[gCurrentCourseId].unk4;
    skybox[4].v.cn[0] = sSkyColors2[gCurrentCourseId].unk0;
    skybox[4].v.cn[1] = sSkyColors2[gCurrentCourseId].unk2;
    skybox[4].v.cn[2] = sSkyColors2[gCurrentCourseId].unk4;
    skybox[5].v.cn[0] = sSkyColors2[gCurrentCourseId].unk6;
    skybox[5].v.cn[1] = sSkyColors2[gCurrentCourseId].unk8;
    skybox[5].v.cn[2] = sSkyColors2[gCurrentCourseId].unkA;
    skybox[6].v.cn[0] = sSkyColors2[gCurrentCourseId].unk6;
    skybox[6].v.cn[1] = sSkyColors2[gCurrentCourseId].unk8;
    skybox[6].v.cn[2] = sSkyColors2[gCurrentCourseId].unkA;
    skybox[7].v.cn[0] = sSkyColors2[gCurrentCourseId].unk0;
    skybox[7].v.cn[1] = sSkyColors2[gCurrentCourseId].unk2;
    skybox[7].v.cn[2] = sSkyColors2[gCurrentCourseId].unk4;
}

void func_802A487C(Vtx* arg0, UNUSED struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                   UNUSED f32* arg4) {

    init_rdp();
    if (gCurrentCourseId != COURSE_RAINBOW_ROAD) {

        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
        guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, LOAD_ASSET(D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListHead++, &arg0[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4A0C(Vtx* vtx, struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED f32* arg4) {
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
    func_802B5564(matrix1, &sp128, camera->unk_B4, gScreenAspect, D_80150150, D_8015014C, 1.0f);
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
    gSPMatrix(gDisplayListHead++, LOAD_ASSET(D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (gCurrentCourseId == COURSE_RAINBOW_ROAD) {
        gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void func_802A4D18(void) {
    if (gGamestate != 4) {
        D_8015014C = 6800.0f;
        D_80150150 = 3.0f;
    } else {
        switch (gCurrentCourseId) {
            case COURSE_BOWSER_CASTLE:
            case COURSE_BANSHEE_BOARDWALK:
            case COURSE_RAINBOW_ROAD:
            case COURSE_BLOCK_FORT:
            case COURSE_SKYSCRAPER:
                D_8015014C = 2700.0f;
                D_80150150 = 2.0f;
                break;
            case COURSE_CHOCO_MOUNTAIN:
            case COURSE_DOUBLE_DECK:
                D_8015014C = 1500.0f;
                D_80150150 = 2.0f;
                break;
            case COURSE_KOOPA_BEACH:
                D_8015014C = 5000.0f;
                D_80150150 = 1.0f;
                break;
            case COURSE_WARIO_STADIUM:
                D_8015014C = 4800.0f;
                D_80150150 = 10.0f;
                break;
            case COURSE_MARIO_RACEWAY:
            case COURSE_YOSHI_VALLEY:
            case COURSE_FRAPPE_SNOWLAND:
            case COURSE_ROYAL_RACEWAY:
            case COURSE_LUIGI_RACEWAY:
            case COURSE_MOO_MOO_FARM:
            case COURSE_TOADS_TURNPIKE:
            case COURSE_SHERBET_LAND:
            case COURSE_DK_JUNGLE:
                D_8015014C = 4500.0f;
                D_80150150 = 9.0f;
                break;
            case COURSE_KALAMARI_DESERT:
                D_8015014C = 7000.0f;
                D_80150150 = 10.0f;
                break;
            default:
                D_8015014C = 6800.0f;
                D_80150150 = 3.0f;
                break;
        }
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
        gDPFillRectangle(gDisplayListHead++, 160, 120, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
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

void render_player_one_1p_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[4];
    u16 perspNorm;
    UNUSED s32 pad2[2];
#ifdef VERSION_EU
    f32 sp9C;
#endif
    UNUSED s32 pad3;
    Mat4 matrix;

#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    func_802A53A4();
    init_rdp();
    func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
}

void render_player_one_2p_screen_vertical(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    func_802A50EC();
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    init_rdp();
    func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    D_8015F788 += 1;
}

void render_player_two_2p_screen_vertical(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    func_802A5004();
    init_rdp();
    func_802A3730(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    func_80058BF4();
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    D_8015F788 += 1;
}

void render_player_one_2p_screen_horizontal(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    func_802A51D4();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    init_rdp();
    func_802A3730(D_800DC5EC);
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    D_8015F788 += 1;
}

void render_player_two_2p_screen_horizontal(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    func_802A52BC();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    init_rdp();
    func_802A3730(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    D_8015F788 += 1;
}

void render_player_one_3p_4p_screen(void) {
    Camera* camera = camera1;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    func_802A54A8();
    init_rdp();
    func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    D_8015F788 += 1;
}

void render_player_two_3p_4p_screen(void) {
    Camera* camera = camera2;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    func_802A5590();
    init_rdp();
    func_802A3730(D_800DC5F0);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    D_8015F788 += 1;
}

void render_player_three_3p_4p_screen(void) {
    Camera* camera = camera3;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    func_802A5678();
    init_rdp();
    func_802A3730(D_800DC5F4);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[2], &perspNorm, gCameraZoom[2], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[2], &perspNorm, gCameraZoom[2], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[2], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F4);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F4);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    render_players_on_screen_three();
    func_8029122C(D_800DC5F4, PLAYER_THREE);
    func_80021D40();
    render_item_boxes(D_800DC5F4);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    D_8015F788 += 1;
}

void render_player_four_3p_4p_screen(void) {
    Camera* camera = camera4;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    func_802A5760();
    if (gPlayerCountSelection1 == 3) {
        func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        if (D_800DC5B8 != 0) {
            render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        }
        D_8015F788 += 1;
        return;
    }

    init_rdp();
    func_802A3730(D_800DC5F8);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[3], &perspNorm, gCameraZoom[3], sp9C, D_80150150, D_8015014C, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[3], &perspNorm, gCameraZoom[3], gScreenAspect, D_80150150, D_8015014C, 1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[3], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F8);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F8);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    render_players_on_screen_four();
    func_8029122C(D_800DC5F8, PLAYER_FOUR);
    func_80021DA8();
    render_item_boxes(D_800DC5F8);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    D_8015F788 += 1;
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
}
