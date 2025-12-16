#include <libultraship.h>
#include <libultra/gbi.h>
#include <common_structs.h>
#include "code_800029B0.h"
#include "mk64.h"
#include "main.h"
#include <port/interpolation/FrameInterpolation.h>
#include <port/interpolation/matrix.h>
#include "engine/Matrix.h"

#include "collision_viewer.h"
#include "math_util.h"

#include "assets/textures/other_textures.h"
#include "assets/models/common_data.h"

//! @todo decide to use colours or textures and fix displaylists because they are buggy

void render_collision(void) {
    Mat4 matrix;
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPPipeSync(gDisplayListHead++);
    gDPSetOtherMode(gDisplayListHead++,
                    G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_AA_ZB_OPA_SURF | G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);

    mtxf_identity(matrix);
    AddObjectMatrix(matrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    for (size_t i = 0; i < gCollisionMeshCount; i++) {
        // Load vertices for this tri

        switch (gCollisionMesh[i].surfaceType) {
            case ASPHALT:

                //     gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                //     gDPTileSync(gDisplayListHead++);
                //    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                //    G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 6, G_TX_NOLOD);
                //     gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x00FC, 0x007C);
                //     gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTextureRoad1);
                //     gDPTileSync(gDisplayListHead++);
                //     gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                //     G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                //     G_TX_NOLOD); gDPLoadSync(gDisplayListHead++); gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0,
                //     0, 2047, 128); gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                //     gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                gCollisionMesh[i].vtx1->v.cn[0] = 50;
                gCollisionMesh[i].vtx1->v.cn[1] = 50;
                gCollisionMesh[i].vtx1->v.cn[2] = 50;
                gCollisionMesh[i].vtx2->v.cn[0] = 50;
                gCollisionMesh[i].vtx2->v.cn[1] = 50;
                gCollisionMesh[i].vtx2->v.cn[2] = 50;
                gCollisionMesh[i].vtx3->v.cn[0] = 50;
                gCollisionMesh[i].vtx3->v.cn[1] = 50;
                gCollisionMesh[i].vtx3->v.cn[2] = 50;
                break;
            case DIRT: // gTexture64619C
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture64AF50);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                // gCollisionMesh[i].vtx1->v.cn[0] = 255;
                // gCollisionMesh[i].vtx1->v.cn[1] = 248;
                // gCollisionMesh[i].vtx1->v.cn[2] = 220;
                // gCollisionMesh[i].vtx2->v.cn[0] = 255;
                // gCollisionMesh[i].vtx2->v.cn[1] = 248;
                // gCollisionMesh[i].vtx2->v.cn[2] = 220;
                // gCollisionMesh[i].vtx3->v.cn[0] = 255;
                // gCollisionMesh[i].vtx3->v.cn[1] = 248;
                // gCollisionMesh[i].vtx3->v.cn[2] = 220;
                break;
            case SAND:
                // gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                // gDPTileSync(gDisplayListHead++);
                // gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                // G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                // gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                // gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture67BEE8);
                // gDPTileSync(gDisplayListHead++);
                // gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                // G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                // G_TX_NOLOD); gDPLoadSync(gDisplayListHead++); gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0,
                // 1023, 256); gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                // gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                // gCollisionMesh[i].vtx1->v.cn[0] = 255;
                // gCollisionMesh[i].vtx1->v.cn[1] = 121;
                // gCollisionMesh[i].vtx1->v.cn[2] = 25;
                // gCollisionMesh[i].vtx2->v.cn[0] = 255;
                // gCollisionMesh[i].vtx2->v.cn[1] = 121;
                // gCollisionMesh[i].vtx2->v.cn[2] = 25;
                // gCollisionMesh[i].vtx3->v.cn[0] = 255;
                // gCollisionMesh[i].vtx3->v.cn[1] = 121;
                // gCollisionMesh[i].vtx3->v.cn[2] = 25;
                break;
            case STONE:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture6528DC);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                // gCollisionMesh[i].vtx1->v.cn[0] = 128;
                // gCollisionMesh[i].vtx1->v.cn[1] = 128;
                // gCollisionMesh[i].vtx1->v.cn[2] = 128;
                // gCollisionMesh[i].vtx2->v.cn[0] = 128;
                // gCollisionMesh[i].vtx2->v.cn[1] = 128;
                // gCollisionMesh[i].vtx2->v.cn[2] = 128;
                // gCollisionMesh[i].vtx3->v.cn[0] = 128;
                // gCollisionMesh[i].vtx3->v.cn[1] = 128;
                // gCollisionMesh[i].vtx3->v.cn[2] = 128;
                break;
            case SNOW:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture677F04);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                break;
            case SAND_OFFROAD:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture67B9B0);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                break;
            case GRASS: // gTextureGrass1

                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTextureGrass1);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);

                // gCollisionMesh[i].vtx1->v.cn[0] = 50;
                // gCollisionMesh[i].vtx1->v.cn[1] = 180;
                // gCollisionMesh[i].vtx1->v.cn[2] = 50;
                // gCollisionMesh[i].vtx2->v.cn[0] = 50;
                // gCollisionMesh[i].vtx2->v.cn[1] = 180;
                // gCollisionMesh[i].vtx2->v.cn[2] = 50;
                // gCollisionMesh[i].vtx3->v.cn[0] = 50;
                // gCollisionMesh[i].vtx3->v.cn[1] = 180;
                // gCollisionMesh[i].vtx3->v.cn[2] = 50;
                break;
            case ICE:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture643B3C);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                break;
            case WET_SAND:
                // gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                // gDPTileSync(gDisplayListHead++);
                // gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                // G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                // gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x00FC);
                // gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture66DD38);
                // gDPTileSync(gDisplayListHead++);
                // gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                // G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                // G_TX_NOLOD); gDPLoadSync(gDisplayListHead++); gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0,
                // 1023, 256); gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                // gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                break;
            case SNOW_OFFROAD:
                gCollisionMesh[i].vtx1->v.cn[0] = 80;
                gCollisionMesh[i].vtx1->v.cn[1] = 80;
                gCollisionMesh[i].vtx1->v.cn[2] = 80;
                gCollisionMesh[i].vtx2->v.cn[0] = 80;
                gCollisionMesh[i].vtx2->v.cn[1] = 80;
                gCollisionMesh[i].vtx2->v.cn[2] = 80;
                gCollisionMesh[i].vtx3->v.cn[0] = 80;
                gCollisionMesh[i].vtx3->v.cn[1] = 80;
                gCollisionMesh[i].vtx3->v.cn[2] = 80;
                break;
            case CLIFF:
                break;
            case DIRT_OFFROAD:
                break;
            case TRAIN_TRACK: // gTextureRailroadTrack
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 6, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x00FC, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTextureRailroadTrack);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 2047, 128);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                break;
            case CAVE:
                break;
            case OUT_OF_BOUNDS:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture64313C);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                // gCollisionMesh[i].vtx1->v.cn[0] = 255;
                // gCollisionMesh[i].vtx1->v.cn[1] = 0;
                // gCollisionMesh[i].vtx1->v.cn[2] = 0;
                // gCollisionMesh[i].vtx2->v.cn[0] = 255;
                // gCollisionMesh[i].vtx2->v.cn[1] = 0;
                // gCollisionMesh[i].vtx2->v.cn[2] = 0;
                // gCollisionMesh[i].vtx3->v.cn[0] = 255;
                // gCollisionMesh[i].vtx3->v.cn[1] = 0;
                // gCollisionMesh[i].vtx3->v.cn[2] = 0;
                break;
            case BOOST_RAMP_WOOD:
            case BOOST_RAMP_ASPHALT:
            case RAMP:
                gCollisionMesh[i].vtx1->v.cn[0] = 255;
                gCollisionMesh[i].vtx1->v.cn[1] = 255;
                gCollisionMesh[i].vtx1->v.cn[2] = 0;
                gCollisionMesh[i].vtx2->v.cn[0] = 255;
                gCollisionMesh[i].vtx2->v.cn[1] = 255;
                gCollisionMesh[i].vtx2->v.cn[2] = 0;
                gCollisionMesh[i].vtx3->v.cn[0] = 255;
                gCollisionMesh[i].vtx3->v.cn[1] = 255;
                gCollisionMesh[i].vtx3->v.cn[2] = 0;
                break;
            case WOOD_BRIDGE:
            case ROPE_BRIDGE:
            case BRIDGE:
                gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
                gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gTexture676FB0);
                gDPTileSync(gDisplayListHead++);
                gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                           G_TX_NOLOD);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
                // gCollisionMesh[i].vtx1->v.cn[0] = 0;
                // gCollisionMesh[i].vtx1->v.cn[1] = 255;
                // gCollisionMesh[i].vtx1->v.cn[2] = 0;
                // gCollisionMesh[i].vtx2->v.cn[0] = 0;
                // gCollisionMesh[i].vtx2->v.cn[1] = 255;
                // gCollisionMesh[i].vtx2->v.cn[2] = 0;
                // gCollisionMesh[i].vtx3->v.cn[0] = 0;
                // gCollisionMesh[i].vtx3->v.cn[1] = 255;
                // gCollisionMesh[i].vtx3->v.cn[2] = 0;
                break;
        }

        gSPVertex(gDisplayListHead++, gCollisionMesh[i].vtx1, 1, 0);
        gSPVertex(gDisplayListHead++, gCollisionMesh[i].vtx2, 1, 1);
        gSPVertex(gDisplayListHead++, gCollisionMesh[i].vtx3, 1, 2);

        // Draw tri
        gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    }
}
