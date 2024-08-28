#include <libultraship.h>
#include <macros.h>
#include <memory.h>
#include <defines.h>
#include <mk64.h>
#include <stubs.h>

#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "code_80086E70.h"
#include "update_objects.h"
#include "objects.h"
#include "bomb_kart.h"
#include "save.h"
#include <assets/common_data.h>
#include <sounds.h>
#include <decode.h>
#include "audio/external.h"
#include "courses/all_course_data.h"
#include "main.h"
#include "menus.h"
#include <assets/other_textures.h>
#include "render_objects.h"
#include "code_80091750.h"
#include "src/data/some_data.h"
#include "effects.h"
#include <assets/boo_frames.h>

void init_hud(void) {

    reset_object_variable();
    func_8006FA94();

    switch (gScreenModeSelection) {
        case SCREEN_MODE_1P:
            init_hud_one_player();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            init_hud_two_player_vertical();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            init_hud_two_player_horizontal();
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            init_hud_three_four_player();
            break;
    }
    func_80070148();
}

void reset_object_variable(void) {
    s32 i;
    s32 j;
    func_8006EB10();
    clear_object_list();
    bzero(playerHUD, HUD_PLAYERS_SIZE * sizeof(hud_player));

    for (i = 0; i < HUD_PLAYERS_SIZE; i++) {
        playerHUD[i].lapCount = 0;
        playerHUD[i].alsoLapCount = 0;
        playerHUD[i].unk_81 = 0;
    }
    for (j = 0; j < HUD_PLAYERS_SIZE; j++) {
        playerHUD[j].raceCompleteBool = 0;
    }
}

void func_8006EB10(void) {
    s32 i;

    for (i = 0; i < gObjectParticle1_SIZE; i++) {
        gObjectParticle1[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        gObjectParticle2[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        gObjectParticle3[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle4_SIZE; i++) {
        gObjectParticle4[i] = NULL_OBJECT_ID;
    }

    // clang-format off
    // Has to be on one line, because IDO hates you :)
    for (i = 0; i < gLeafParticle_SIZE; i++) { gLeafParticle[i] = NULL_OBJECT_ID; }
    // clang-format on

    D_8018CF18 = D_8018CF20 = D_8018CF48 = D_8018CF60 = D_8018CF78 = D_8018CF90 = D_8018CFA8 = 0;
    D_8018CFB0 = D_8018CFB8 = D_8018CFC0 = D_8018CFC8 = D_8018CFD0 = D_8018CFD8 = D_8018CFE0 = 0;
    D_8018D018 = 0;
    D_8018D010 = 0;
    D_8018D008 = 0;
    D_8018D000 = 0;
    D_8018CFF8 = 0;
    D_8018CFF0 = 0;
    D_8018CFE8 = 0;
    D_8018D110 = 0;
    D_8018D0E8 = 0;
    D_8018D0C0 = 0;
    D_8018D020 = D_8018D048 = D_8018D070 = D_8018D098 = 0;
    gNextFreeObjectParticle1 = gNextFreeObjectParticle2 = gNextFreeObjectParticle3 = gNextFreeObjectParticle4 =
        gNextFreeLeafParticle = 0;
}

void clear_object_list() {
    bzero(gObjectList, OBJECT_LIST_SIZE * sizeof(Object));
    objectListSize = -1;
}

u8* func_8006ED94(u8* devAddr, u8* baseAddress, u32 size, u32 offset) {
#ifdef TARGET_N64
    u8** tempAddress;
    u8* address;
    address = baseAddress + offset;

    size = ALIGN16(size);
    osInvalDCache(address, (size));
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[((u32) devAddr) & 0xFFFFFF], address,
                 size, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    tempAddress = &address;
    mio0decode(*tempAddress, (u8*) baseAddress);
#else
    baseAddress = devAddr;
    // memcpy doesn't seem to be applicable here.
    // memcpy(baseAddress, devAddr, size);
    // baseAddress = devAddr;
#endif
    return baseAddress;
}

void func_8006EE44(void) {
    u8* d_gTextureLogoMarioKart64 = LOAD_ASSET(gTextureLogoMarioKart64);
    D_8018D1E0 = func_8006ED94((u8*) d_gTextureLogoMarioKart64, (u8*) D_8018D9B0, 0x79E1, 0x20000);
}

// Some kind of initalization for the Item Window part of the HUD
void init_item_window(s32 objectIndex) {
    ItemWindowObjects* temp_v0;

    temp_v0 = (ItemWindowObjects*) &gObjectList[objectIndex];
    temp_v0->currentItem = ITEM_NONE;
    temp_v0->itemDisplay = temp_v0->currentItem;
    temp_v0->tlutList = (u8*) common_tlut_item_window_none;
    temp_v0->activeTLUT = (u8*) common_tlut_item_window_none;
    temp_v0->textureList = common_texture_item_window_none;
    temp_v0->activeTexture = common_texture_item_window_none;
    temp_v0->unk_04C = -1;
    temp_v0->unk_09C = 0x00A0;  // Screen X position
    temp_v0->unk_09E = -0x0020; // Screen Y position
    temp_v0->sizeScaling = 1.0f;
}

void func_8006EEE8(s32 courseId) {
    D_8018D240 = (uintptr_t) dma_textures(gCourseOutlineTextures[courseId], D_800E5520[courseId], D_800E5520[courseId]);
    // This is incredibly dumb. D_800E5548 ought to be something more like
    // `u16 D_800E5548[][2]` but that doesn't match for some insane reason
    D_8018D2B0 = D_800E5548[courseId * 2];
    D_8018D2B8 = D_800E5548[courseId * 2 + 1];
}

void func_8006EF60(void) {
    s32 i;
    // `huh`'s and `i`'s types have to differ, for some reason
    s16 huh;
    u8* wut;

    wut = D_8018D9B4 + 0xFFFF0000;
    // clang-format off
    // God forgive me for my sins...
    huh = 0x14; if (0) {} for (i = 0; i < huh; i++) { D_8018D248[i] = func_8006ED94(gCourseOutlineTextures[i], wut, D_800E5520[i], D_800E5520[i]); wut += D_800E5520[i]; }
    // clang-format on
}

void func_8006F008(void) {
    D_801655C8 = 0;
    xOrientation = 1.0f;
    if (gIsMirrorMode != 0) {
        xOrientation = -1.0f;
    }
    D_8018D2C0[0] = 257;
    D_8018D2D8[0] = 170;
    D_8018D300 = 255;
    D_8018D308 = 255;
    D_8018D310 = 255;
    D_8018D318 = 255;
    if (gCurrentCourseId < NUM_COURSES - 1) {
        func_8006EEE8((s32) gCurrentCourseId);
    }
    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            D_8018D220 = (void*) dma_textures(gTextureExhaust5, 0x443, 0x1000);
            D_8018D2A0 = 0.022f;
            D_8018D2E0 = 6;
            D_8018D2E8 = 28;
            D_8018D2C0[0] = 260;
            D_8018D2D8[0] = 170;
            D_80165718 = 0;
            D_80165720 = 5;
            D_80165728 = -240;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            D_8018D2A0 = 0.022f;
            D_8018D2C0[0] = 265;
            D_8018D2E0 = 19;
            D_8018D2E8 = 37;
            break;
        case COURSE_BOWSER_CASTLE:
            D_8018D2C0[0] = 265;
            D_8018D2A0 = 0.0174f;
            D_8018D2E0 = 12;
            D_8018D2E8 = 48;
            break;
        case COURSE_BANSHEE_BOARDWALK:
            D_80165880 = (void*) dma_textures(gTextureGhosts, 0x4CC2, 0xD980);
            D_8018D2A0 = 0.016f;
            D_8018D2C0[0] = 0x0106;
            D_8018D2E0 = 55;
            D_8018D2E8 = 39;
            break;
        case COURSE_YOSHI_VALLEY:
            D_8018D220 = (void*) dma_textures(gTextureExhaust0, 0x479, 0xC00);
            D_8018D2A0 = 0.018f;
            D_8018D2E0 = 61;
            D_8018D2E8 = 38;
            break;
        case COURSE_FRAPPE_SNOWLAND:
            D_8018D2C0[0] = 262;
            D_8018D2A0 = 0.016f;
            D_8018D2E0 = 36;
            D_8018D2E8 = 40;
            D_8018D300 = 72;
            D_8018D308 = 100;
            D_8018D310 = 255;
            break;
        case COURSE_KOOPA_BEACH:
            D_8018D220 = (void*) dma_textures(gTextureExhaust3, 0x3C8U, 0x1000);
            D_8018D2A0 = 0.014f;
            D_8018D2C0[0] = 268;
            D_8018D2E0 = 40;
            D_8018D2E8 = 21;
            break;
        case COURSE_ROYAL_RACEWAY:
            D_8018D220 = (void*) dma_textures(gTextureExhaust4, 0x3F8, 0x1000);
            D_8018D2C0[0] = 262;
            D_8018D2A0 = 0.014f;
            D_8018D2E0 = 37;
            D_8018D2E8 = 50;
            D_80165718 = -64;
            D_80165720 = 5;
            D_80165728 = -330;
            break;
        case COURSE_LUIGI_RACEWAY:
            D_8018D220 = (void*) dma_textures(gTextureExhaust2, 0x4F4U, 0xC00);
            D_8018D2A0 = 0.0155f;
            D_8018D2C0[0] = 271;
            D_8018D2E0 = 45;
            D_8018D2E8 = 60;
            D_80165718 = -140;
            D_80165720 = -44;
            D_80165728 = -215;
            break;
        case COURSE_MOO_MOO_FARM:
            D_8018D220 = (void*) dma_textures(gTextureExhaust0, 0x479, 0xC00);
            D_8018D2A0 = 0.0155f;
            D_8018D2C0[0] = 271;
            D_8018D2E0 = 18;
            D_8018D2E8 = 36;
            break;
        case COURSE_TOADS_TURNPIKE:
            D_8018D2A0 = 0.013f;
            D_8018D2C0[0] = 252;
            D_8018D2E0 = 57;
            D_8018D2E8 = 44;
            break;
        case COURSE_KALAMARI_DESERT:
            D_8018D2C0[0] = 263;
            D_8018D2D8[0] = 165;
            D_8018D220 = (void*) dma_textures(gTextureExhaust5, 0x443, 0x1000);
            D_8018D2A0 = 0.015f;
            D_8018D2E0 = 55;
            D_8018D2E8 = 27;
            break;
        case COURSE_SHERBET_LAND:
            D_8018D220 = (void*) dma_textures(gTextureExhaust1, 0x485, 0xC00);
            D_8018D2A0 = 0.015f;
            D_8018D2C0[0] = 262;
            D_8018D2E0 = 52;
            D_8018D2E8 = 33;
            D_8018D300 = 72;
            D_8018D308 = 100;
            D_8018D310 = 255;
            break;
        case COURSE_RAINBOW_ROAD:
            D_8018D2A0 = 0.0103f;
            D_8018D2C0[0] = 261;
            D_8018D2D8[0] = 166;
            D_8018D2E0 = 39;
            D_8018D2E8 = 55;
            break;
        case COURSE_WARIO_STADIUM:
            D_8018D2A0 = 0.0155f;
            D_8018D2C0[0] = 0x0106;
            D_8018D2E0 = 53;
            D_8018D2E8 = 35;
            break;
        case COURSE_BLOCK_FORT:
            D_8018D2A0 = 0.0335f;
            D_8018D2E0 = 32;
            D_8018D2E8 = 32;
            break;
        case COURSE_SKYSCRAPER:
            D_8018D2A0 = 0.0445f;
            D_8018D2E0 = 32;
            D_8018D2E8 = 32;
            break;
        case COURSE_DOUBLE_DECK:
            D_8018D2A0 = 0.0285f;
            D_8018D2E0 = 32;
            D_8018D2E8 = 32;
            break;
        case COURSE_DK_JUNGLE:
            D_8018D2A0 = 0.0155f;
            D_8018D2C0[0] = 255;
            D_8018D2E0 = 29;
            D_8018D2E8 = 47;
            break;
        case COURSE_BIG_DONUT:
            D_8018D2A0 = 0.0257f;
            D_8018D2E0 = 32;
            D_8018D2E8 = 31;
    }
    if (gIsMirrorMode != 0) {
        D_8018D2E0 = D_8018D2B0 - D_8018D2E0;
    }
    if (gPlayerCount == 4) {
        D_8018D2C0[0] = 160;
        D_8018D2D8[0] = 120;
        return;
    }
    if (gPlayerCount == 3) {
        D_8018D2C0[0] = 235;
        D_8018D2D8[0] = 175;
        return;
    }
    if (gPlayerCount == 2) {
        if (gCurrentCourseId != 10) {
            D_8018D2C0[1] = 265;
            D_8018D2C0[0] = D_8018D2C0[1];
        } else {
            D_8018D2C0[1] = 255;
            D_8018D2C0[0] = D_8018D2C0[1];
        }
        D_8018D2D8[0] = 65;
        D_8018D2D8[1] = 180;
    }
}

void func_8006F824(s32 arg0) {
    D_80165808 = D_801657E4;
    D_80165810 = D_801657E6;
    D_80165820 = D_801657F0;
    D_80165818 = D_801657E8;
    D_80165828 = D_801657F8;
    D_80165832[0] = D_80165800[0];
    D_80165832[1] = D_80165800[1];
    if ((arg0 != 0) && (gIsGamePaused == 0)) {
        play_sound2(SOUND_ACTION_PING);
    }
}

void func_8006F8CC(void) {
    if (D_8018EDFC == 0) {
        D_8018EDFC = 1;
        D_801657E4 = 0;
        D_801657E6 = 0;
        D_801657F0 = 0;
        D_801657E8 = 1;
        D_80165800[0] = D_80165800[1] = 1;
        if (gPlayerCount == 4) {
            if (gModeSelection != 3) {
                D_801657E4 = 1;
                D_801657F0 = 1;
                D_801657F8 = 1;
                D_80165800[0] = D_80165800[1] = 0;
            } else {
                D_801657F8 = 0;
                D_80165800[0] = D_80165800[1] = 1;
            }
        } else if (gPlayerCount == 3) {
            D_801657E8 = 0;
            D_801657F8 = 1;
        } else if (gPlayerCount == 2) {
            if (gModeSelection != (s32) 3) {
                D_801657E4 = 1;
                D_801657F0 = 1;
                D_80165800[0] = D_80165800[1] = 0;
            }
            D_8018D2D8[0] = 0x0041;
            D_8018D2D8[1] = 0x00B4;
        }
        func_8006F824(0);
    } else {
        D_801657E4 = D_80165808;
        D_801657E6 = D_80165810;
        D_801657F0 = D_80165820;
        D_801657E8 = D_80165818;
        D_801657F8 = D_80165828;
        D_80165800[0] = D_80165832[0];
        D_80165800[1] = D_80165832[1];
    }
    if (gDemoMode != 0) {
        D_801657F0 = 0;
    }
}

#ifdef NON_MATCHING
// Major register allocation problems in the first for-loop
// Smaller issues elsewhere, probably some one line, multiple variable assignment shenanigans going on
// https://decomp.me/scratch/ohbAc
void func_8006FA94(void) {
    s32 var_a0;
    Player* player;

    func_8006F8CC();
    func_8006F008();
    osSetTime(0);
    D_8018D170 = 0;
    D_8018D190 = 0;
    gIsHUDVisible = 0;
    D_8018D178 = 0;
    D_8018D1CC = 0;
    D_801657E2 = 0;
    D_80165730 = 0;
    D_801658FE = 0;
    /*
    D_801657E5 = 0;
    D_801657E3 = D_801657E5;
    D_801657E1 = D_801657E3;
    */
    D_801657E1 = D_801657E3 = D_801657E5 = 0;
    /*
    D_80165658->unk8 = 0;
    D_80165658->unk4 = 0;
    D_80165658->unk0 = 0;
    */
    D_80165658[0] = D_80165658[1] = D_80165658[2] = 0;
    /*
    D_801658D6 = 0;
    D_801658E4 = D_801658D6;
    D_801658F4 = D_801658E4;
    D_801658EC = D_801658F4;
    D_801658DC = D_801658EC;
    D_801658CE = D_801658DC;
    D_801658C6 = D_801658CE;
    D_801658BC = D_801658C6;
    */
    D_801658BC = D_801658C6 = D_801658CE = D_801658DC = D_801658EC = D_801658F4 = D_801658E4 = D_801658D6 = 0;
    switch (gPlayerCount) { /* irregular */
        case 1:
            if (gModeSelection == 0) {
                D_8018D114 = 0;
                D_8018D178 = 150;
                D_8018D180 = 240;
            } else {
                D_8018D114 = 1;
                D_8018D178 = 10;
                D_8018D180 = 0;
            }
            break;
        case 2:
            if (gScreenModeSelection == 1) {
                if (gModeSelection == 0) {
                    D_8018D114 = 2;
                    D_8018D178 = 150;
                    D_8018D180 = 240;
                    D_8018D2AC = 60;
                } else if (gModeSelection == 2) {
                    D_8018D114 = 3;
                    D_8018D178 = 30;
                    D_8018D180 = 30;
                    D_8018D2AC = 60;
                } else {
                    D_8018D114 = 4;
                    D_8018D178 = 40;
                    D_8018D180 = 40;
                    D_8018D2AC = 60;
                }
            } else if (gModeSelection == 0) {
                D_8018D114 = 5;
            } else if (gModeSelection == 2) {
                D_8018D114 = 6;
            } else {
                D_8018D114 = 7;
            }
            break;
        case 3:
            if (gModeSelection == 2) {
                D_8018D114 = 8;
                D_8018D178 = 100;
                D_8018D180 = 150;
                D_8018D2AC = 60;
            } else {
                D_8018D114 = 9;
                D_8018D178 = 100;
                D_8018D180 = 150;
                D_8018D2AC = 60;
            }
            break;
        case 4:
            if (gModeSelection == 2) {
                D_8018D114 = 10;
                D_8018D178 = 30;
                D_8018D180 = 30;
                D_8018D2AC = 10;
            } else {
                D_8018D114 = 11;
                D_8018D178 = 30;
                D_8018D180 = 30;
                D_8018D2AC = 10;
            }
            break;
    }
    if (gEnableDebugMode == 0) {
        D_8016576A = 0;
        D_8016579C = 0;
    }
    for (var_a0 = 0; var_a0 < gPlayerCount; var_a0++) {
        D_8018CFBC[var_a0] = 0;
        D_8018CFAC[var_a0] = 0;
        D_8018CFC4[var_a0] = 0;
        D_8018CFB4[var_a0] = 0;
    }
    D_8018D204 = 1;
    D_8018D1FC = 0;
    D_8018D224 = 0;
    D_8018D1F0 = D_8018D1F8 = 0;
    D_8018D228 = 0xFF;
    /*
    D_80165628 = 0;
    D_80165618 = 0;
    D_80165608 = D_80165618;
    D_801655F8 = D_80165618;
    D_801655E8 = D_80165618;
    D_801655D8 = D_80165618;
    */
    D_801655D8 = D_801655E8 = D_801655F8 = D_80165608 = D_80165618 = D_80165628 = 0;
    D_8018D160 = 0;
    D_8018D1DC = 0;
    D_8018D1C4 = 0;
    D_8018D1B4 = 0;
    D_8018D1A0 = 0;
    D_8018D168 = 0;
    D_801656F0 = 0;
    D_801657B2 = 0;
    D_801657D8 = D_801657B2;
    D_8018D214 = D_801657D8;
    gHUDDisable = D_8018D214;
    D_801657AE = gHUDDisable;
    D_8018D20C = 0;
    D_8018D2F8 = 0;
    D_8018D2F0 = D_8018D2F8;
    D_8018D320 = 3;
    D_8018D2AC = 0;
    D_8018D2BC = 0;
    D_8018D2B4 = D_8018D2BC;
    D_8018D2A4 = D_8018D2B4;
    D_8018D2C8[0] = 0;
    D_8018D2C8[1] = 0;
    D_8018D2C8[2] = 0;
    D_8018D2C8[3] = 0;
    D_8016581C = 0;
    D_8016580C = 0;
    D_80165814 = 0;
    D_80165804 = 0;
    D_801657FC = 0;
    D_8018D18C = -1;
    D_8018D184 = D_8018D18C;
    D_8018D16C = D_8018D18C;
    D_8018D17C = D_8018D18C;
    D_8018D174 = D_8018D18C;
    player = gPlayerOne;
    for (var_a0 = 0; var_a0 < NUM_PLAYERS; var_a0++) {
        D_8018D0F0[var_a0] = D_8018D050[var_a0] = -32.0f;
        D_8018CE10[var_a0].unk_04[0] = D_8018CE10[var_a0].unk_04[1] = D_8018CE10[var_a0].unk_04[2] = 0.0f;
        D_8018CF50[var_a0] = var_a0;
        D_8018CF28[var_a0] = player;
        player->unk_040 = -1;
        player++;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_8006E9C0/func_8006FA94.s")
#endif

void func_80070148(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 8; var_s0++) {
        find_unused_obj_index(&D_8018CE10[var_s0].objectIndex);
    }
}

void init_object_list_index(void) {
    s32 loopIndex;

    for (loopIndex = 0; loopIndex < SOME_OBJECT_INDEX_LIST_SIZE; loopIndex++) {
        find_unused_obj_index(&indexObjectList1[loopIndex]);
        find_unused_obj_index(&indexObjectList2[loopIndex]);
        find_unused_obj_index(&indexObjectList3[loopIndex]);
        find_unused_obj_index(&indexObjectList4[loopIndex]);
    }

    for (loopIndex = 0; loopIndex < NUM_BOMB_KARTS_VERSUS; loopIndex++) {
        find_unused_obj_index(&gIndexObjectBombKart[loopIndex]);
    }
}

void init_cloud_object(s32 objectIndex, s32 arg1, CloudData* arg2) {
    ItemWindowObjects* temp_v0;

    init_object(objectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &gObjectList[objectIndex];
    temp_v0->unk_0D5 = arg2->subType;
    temp_v0->currentItem = ITEM_NONE;
    temp_v0->direction_angle[1] = arg2->rotY;
    temp_v0->unk_09E = arg2->posY;
    temp_v0->sizeScaling = (f32) arg2->scalePercent / 100.0;
    temp_v0->activeTexture = (u8*) &D_8018D220[arg2->subType];
    func_80073404(objectIndex, 0x40U, 0x20U, D_0D005FB0);
    temp_v0->primAlpha = 0x00FF;
}

void init_clouds(CloudData* cloudList) {
    s32 var_s0 = 0;
    CloudData* test = &cloudList[0];
    do {
        if (1) {}
        init_cloud_object(find_unused_obj_index(&D_8018CC80[D_8018D1F8 + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->rotY != 0xFFFF);
    D_8018D1F8 += var_s0;
    D_8018D1F0 = var_s0;
    D_8018D230 = 0;
}

/**
 * This function is part of the spawning for the "stars" in some stages
 *
 * arg2 is a pointer to some type of spawn data for the stars, although it not super clear
 * what types each element is. It seems like its a bunch of u16's, so maybe a Vec4su?
 *
 * The stars in Wario's Stadium, Toad's Turnpike, and Rainbow Road are not part of the skybox.
 * They are instead objects that seemingly hover in the air around the player
 * They have no true x/y/z position, instead they seem to be kept in a position relative to the
 * player they hang around. There is however an y rotation and y position for where they should be on screen
 * when they are visbile (unk_09E[0] and [1]).
 * sizeScaling is some sort of size scaling on the start texture.
 * unk_0A2 is an alpha value, used to make the star twinkle.
 **/
void init_star_object(s32 objectIndex, s32 arg1, StarData* arg2) {
    ItemWindowObjects* temp_v0;

    init_object(objectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &gObjectList[objectIndex];
    temp_v0->unk_0D5 = arg2->subType;
    temp_v0->currentItem = ITEM_BANANA;
    temp_v0->direction_angle[1] = arg2->rotY;
    temp_v0->unk_09E = arg2->posY;                           // screen Y position
    temp_v0->sizeScaling = (f32) arg2->scalePercent / 100.0; // some type of scaling on the texture
    temp_v0->activeTexture = D_0D0293D8;
    func_80073404(objectIndex, 0x10U, 0x10U, common_vtx_rectangle);
}

void init_stars(StarData* starList) {
    s32 var_s0 = 0;
    StarData* test = &starList[0];
    do {
        if (1) {}
        init_star_object(find_unused_obj_index(&D_8018CC80[D_8018D1F8 + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->rotY != 0xFFFF);
    D_8018D1F8 += var_s0;
    D_8018D1F0 = var_s0;
    D_8018D230 = 1;
}

void func_8007055C(void) {
    s32 var_s0;
    s32 var_s4;

    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            // Uses Kalimari Desert's clouds for initialization?
            init_clouds(gKalimariDesertClouds);
            break;
        case COURSE_YOSHI_VALLEY:
            init_clouds(gYoshiValleyMooMooFarmClouds);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (gPlayerCount == 1) {
                var_s4 = 0x32;
            } else {
                var_s4 = 0x19;
            }
            for (var_s0 = 0; var_s0 < var_s4; var_s0++) {
                find_unused_obj_index(&D_8018CC80[D_8018D1F8 + var_s0]);
            }
            D_8018D1F8 += var_s0;
            D_8018D1F0 = var_s0;
            break;
        case COURSE_KOOPA_BEACH:
            init_clouds(gKoopaTroopaBeachClouds);
            break;
        case COURSE_ROYAL_RACEWAY:
            init_clouds(gRoyalRacewayClouds);
            break;
        case COURSE_LUIGI_RACEWAY:
            init_clouds(gLuigiRacewayClouds);
            break;
        case COURSE_MOO_MOO_FARM:
            init_clouds(gYoshiValleyMooMooFarmClouds);
            break;
        case COURSE_TOADS_TURNPIKE:
            init_stars(gToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_KALAMARI_DESERT:
            init_clouds(gKalimariDesertClouds);
            break;
        case COURSE_SHERBET_LAND:
            init_clouds(gSherbetLandClouds);
            break;
        case COURSE_RAINBOW_ROAD:
            init_stars(gToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_WARIO_STADIUM:
            init_stars(gWarioStadiumStars);
            break;
    }
    func_8008C23C();
}

void func_80070714(void) {
    D_80165730 = 1;
    if (gPlayerCount == ONE_PLAYERS_SELECTED) {
        D_80165738 = 0x64;
        D_80165740 = 0x3C;
        D_80165748 = 0x1E;
        return;
    }
    D_80165738 = 0x32;
    D_80165740 = 0x1E;
    D_80165748 = 0xA;
}

const char* sBoardwalkTexList[] = { gTextureBat1, gTextureBat2, gTextureBat3, gTextureBat4 };

void init_course_object(void) {
    s32 objectId;
    s32 i;

    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            if (gGamestate != 9) {
                if (gModeSelection == GRAND_PRIX) {
                    func_80070714();
                }
                for (i = 0; i < D_80165738; i++) {
                    find_unused_obj_index(&gObjectParticle3[i]);
                    init_object(gObjectParticle3[i], 0);
                }
            }
            break;
        case COURSE_BOWSER_CASTLE:
            gNumActiveThwomps = NUM_THWOMPS_100CC_EXTRA;
            gThowmpSpawnList = gThwompSpawns100CCExtra;
            switch (gCCSelection) { /* switch 1; irregular */
                case CC_100:        /* switch 1 */
                case CC_EXTRA:      /* switch 1 */
                    break;
                case CC_50: /* switch 1 */
                    gNumActiveThwomps = NUM_THWOMPS_50CC;
                    gThowmpSpawnList = gThomwpSpawns50CC;
                    break;
                case CC_150: /* switch 1 */
                    gNumActiveThwomps = NUM_THWOMPS_150CC;
                    gThowmpSpawnList = gThomwpSpawns150CC;
                    break;
            }
            for (i = 0; i < gNumActiveThwomps; i++) {
                objectId = indexObjectList1[i];
                init_object(objectId, 0);
                gObjectList[objectId].origin_pos[0] = gThowmpSpawnList[i].startX * xOrientation;
                gObjectList[objectId].origin_pos[2] = gThowmpSpawnList[i].startZ;
                gObjectList[objectId].unk_0D5 = gThowmpSpawnList[i].unk_4;
                gObjectList[objectId].primAlpha = gThowmpSpawnList[i].unk_6;
            }
            // Handle the big statue's fire breath
            objectId = indexObjectList2[0];
            init_object(objectId, 0);
            gObjectList[objectId].pos[0] = -68.0 * xOrientation;
            gObjectList[objectId].pos[1] = 80.0f;
            gObjectList[objectId].pos[2] = -1840.0f;
            // Handle the smaller statues' fire breath
            for (i = 0; i < NUM_FIRE_BREATHS; i++) {
                objectId = indexObjectList3[i];
                init_object(objectId, 0);
                gObjectList[objectId].pos[0] = gFireBreathsSpawns[i][0] * xOrientation;
                gObjectList[objectId].pos[1] = gFireBreathsSpawns[i][1];
                gObjectList[objectId].pos[2] = gFireBreathsSpawns[i][2];
                gObjectList[objectId].direction_angle[1] = 0;
                if (i % 2U) {
                    gObjectList[objectId].direction_angle[1] += 0x8000;
                }
            }
            for (i = 0; i < 32; i++) {
                delete_object(&indexObjectList4[i]);
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (gGamestate != CREDITS_SEQUENCE) {
                objectId = indexObjectList1[0];
                init_texture_object(objectId, d_course_banshee_boardwalk_bat_tlut, sBoardwalkTexList, 0x20U,
                                    (u16) 0x00000040);
                gObjectList[objectId].orientation[0] = 0;
                gObjectList[objectId].orientation[1] = 0;
                gObjectList[objectId].orientation[2] = 0x8000;
                init_object(indexObjectList1[1], 0);
                init_object(indexObjectList1[2], 0);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            for (i = 0; i < NUM_YV_FLAG_POLES; i++) {
                init_object(indexObjectList1[i], 0);
            }
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_HEDGEHOGS; i++) {
                    objectId = indexObjectList2[i];
                    init_object(objectId, 0);
                    gObjectList[objectId].pos[0] = gObjectList[objectId].origin_pos[0] =
                        gHedgehogSpawns[i].pos[0] * xOrientation;
                    gObjectList[objectId].pos[1] = gObjectList[objectId].surfaceHeight =
                        gHedgehogSpawns[i].pos[1] + 6.0;
                    gObjectList[objectId].pos[2] = gObjectList[objectId].origin_pos[2] = gHedgehogSpawns[i].pos[2];
                    gObjectList[objectId].unk_0D5 = gHedgehogSpawns[i].unk_06;
                    gObjectList[objectId].unk_09C = gHedgehogPatrolPoints[i][0] * xOrientation;
                    gObjectList[objectId].unk_09E = gHedgehogPatrolPoints[i][2];
                }
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            for (i = 0; i < NUM_SNOWFLAKES; i++) {
                find_unused_obj_index(&gObjectParticle1[i]);
            }
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_SNOWMEN; i++) {
                    objectId = indexObjectList2[i];
                    init_object(objectId, 0);
                    gObjectList[objectId].origin_pos[0] = gSnowmanSpawns[i].pos[0] * xOrientation;
                    gObjectList[objectId].origin_pos[1] = gSnowmanSpawns[i].pos[1] + 5.0 + 3.0;
                    gObjectList[objectId].origin_pos[2] = gSnowmanSpawns[i].pos[2];
                    objectId = indexObjectList1[i];
                    init_object(objectId, 0);
                    gObjectList[objectId].origin_pos[0] = gSnowmanSpawns[i].pos[0] * xOrientation;
                    gObjectList[objectId].origin_pos[1] = gSnowmanSpawns[i].pos[1] + 3.0;
                    gObjectList[objectId].origin_pos[2] = gSnowmanSpawns[i].pos[2];
                    gObjectList[objectId].unk_0D5 = gSnowmanSpawns[i].unk_6;
                }
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_CRABS; i++) {
                    objectId = indexObjectList1[i];
                    init_object(objectId, 0);
                    gObjectList[objectId].pos[0] = gObjectList[objectId].origin_pos[0] =
                        gCrabSpawns[i].startX * xOrientation;
                    gObjectList[objectId].unk_01C[0] = gCrabSpawns[i].patrolX * xOrientation;

                    gObjectList[objectId].pos[2] = gObjectList[objectId].origin_pos[2] = gCrabSpawns[i].startZ;
                    gObjectList[objectId].unk_01C[2] = gCrabSpawns[i].patrolZ;
                }
            }
            for (i = 0; i < NUM_SEAGULLS; i++) {
                objectId = indexObjectList2[i];
                init_object(objectId, 0);
                if (i < (NUM_SEAGULLS / 2)) {
                    gObjectList[objectId].unk_0D5 = 0;
                } else {
                    gObjectList[objectId].unk_0D5 = 1;
                }
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            if (gGamestate != CREDITS_SEQUENCE) {
                if (gModeSelection == GRAND_PRIX) {
                    func_80070714();
                }
                for (i = 0; i < D_80165738; i++) {
                    find_unused_obj_index(&gObjectParticle3[i]);
                    init_object(gObjectParticle3[i], 0);
                }
            }
            break;
        case COURSE_LUIGI_RACEWAY:
            if (gGamestate != CREDITS_SEQUENCE) {
                if (gModeSelection == GRAND_PRIX) {
                    func_80070714();
                }
                D_80165898 = 0;
                init_object(indexObjectList1[0], 0);
                for (i = 0; i < D_80165738; i++) {
                    find_unused_obj_index(&gObjectParticle3[i]);
                    init_object(gObjectParticle3[i], 0);
                }
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (gGamestate != CREDITS_SEQUENCE) {
                if ((gPlayerCount == 1) || ((gPlayerCount == 2) && (gModeSelection == VERSUS))) {
                    switch (gCCSelection) { /* switch 2; irregular */
                        case CC_50:         /* switch 2 */
                            D_8018D1C8 = 4;
                            D_8018D1D0 = 6;
                            D_8018D1D8 = 6;
                            break;
                        case CC_100: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 8;
                            break;
                        case CC_150: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 10;
                            break;
                        case CC_EXTRA: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 8;
                            break;
                    }
                } else {
                    D_8018D1C8 = 4;
                    D_8018D1D0 = 6;
                    D_8018D1D8 = 6;
                }
                for (i = 0; i < NUM_GROUP1_MOLES; i++) {
                    D_8018D198[i] = 0;
                    find_unused_obj_index(&indexObjectList1[i]);
                }
                for (i = 0; i < NUM_GROUP2_MOLES; i++) {
                    D_8018D1A8[i] = 0;
                    find_unused_obj_index(&indexObjectList1[i]);
                }
                for (i = 0; i < NUM_GROUP3_MOLES; i++) {
                    D_8018D1B8[i] = 0;
                    find_unused_obj_index(&indexObjectList1[i]);
                }
                for (i = 0; i < NUM_TOTAL_MOLES; i++) {
                    find_unused_obj_index(&gObjectParticle1[i]);
                    objectId = gObjectParticle1[i];
                    init_object(objectId, 0);
                    gObjectList[objectId].pos[0] = gMoleSpawns.asVec3sList[i][0] * xOrientation;
                    gObjectList[objectId].pos[2] = gMoleSpawns.asVec3sList[i][2];
                    func_800887C0(objectId);
                    gObjectList[objectId].sizeScaling = 0.7f;
                }
                for (i = 0; i < gObjectParticle2_SIZE; i++) {
                    find_unused_obj_index(&gObjectParticle2[i]);
                }
            }
            break;
        case COURSE_KALAMARI_DESERT:
            if (gGamestate != CREDITS_SEQUENCE) {
                find_unused_obj_index(&D_8018CF10);
                init_object(D_8018CF10, 0);
                for (i = 0; i < 50; i++) {
                    find_unused_obj_index(&gObjectParticle1[i]);
                }
                for (i = 0; i < 5; i++) {
                    find_unused_obj_index(&gObjectParticle2[i]);
                }
                for (i = 0; i < 32; i++) {
                    find_unused_obj_index(&gObjectParticle3[i]);
                }
            }
            break;
        case COURSE_SHERBET_LAND:
            for (i = 0; i < NUM_PENGUINS; i++) {
                init_object(indexObjectList1[i], 0);
            }
            break;
        case COURSE_RAINBOW_ROAD:
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_NEON_SIGNS; i++) {
                    init_object(indexObjectList1[i], 0);
                }
                for (i = 0; i < NUM_CHAIN_CHOMPS; i++) {
                    init_object(indexObjectList2[i], 0);
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            for (i = 0; i < NUM_TORCHES; i++) {
                init_smoke_particles(i);
                // wtf?
                if (D_8018CF10) {}
            }
            break;
        default:
            break;
    }
}

void init_hud_one_player(void) {
    s32 someIndex;
    f32 something;
    // permuter magic
    long long why;
    s32 one = 1;

    D_8018D140 = 0;
    D_8018D150 = 0;
    D_8018CFCC = 1.0f;
    find_unused_obj_index(&D_80183DA0);
    find_unused_obj_index(&gIndexLakituList[0]);
    find_unused_obj_index(&gIndexLakituList[1]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[0]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[1]);
    init_object_list_index();
    func_8007055C();
    func_8007055C();
    init_course_object();
    playerHUD[PLAYER_ONE].speedometerX = 0x0156;
    playerHUD[PLAYER_ONE].speedometerY = 0x0106;
    D_8018CFEC = playerHUD[PLAYER_ONE].speedometerX + 0x18;
    D_8018CFF4 = playerHUD[PLAYER_ONE].speedometerY + 6;
    D_8016579E = 0xDD00;
    playerHUD[PLAYER_ONE].rankX = 52;
    playerHUD[PLAYER_ONE].rankY = 0x00C8;
    playerHUD[PLAYER_ONE].slideRankX = 0;
    playerHUD[PLAYER_ONE].slideRankY = 0;
    playerHUD[PLAYER_ONE].stagingPosition = gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_ONE].timerX = 0x012C;
    playerHUD[PLAYER_ONE].lapCompletionTimeXs[0] = 0x012C;
    playerHUD[PLAYER_ONE].lapCompletionTimeXs[1] = 0x012C;
    playerHUD[PLAYER_ONE].timerY = 0x0011;
    playerHUD[PLAYER_ONE].lapX = -0x0028;
    playerHUD[PLAYER_ONE].lapAfterImage1X = -0x0028;
    playerHUD[PLAYER_ONE].lapAfterImage2X = -0x0028;
    playerHUD[PLAYER_ONE].lapY = 0x0019;
    playerHUD[PLAYER_ONE].itemBoxX = 0x00A0;
    playerHUD[PLAYER_ONE].itemBoxY = -0x0020;
    playerHUD[PLAYER_ONE].slideItemBoxX = 0;
    playerHUD[PLAYER_ONE].slideItemBoxY = 0;
    // permuter magic
    why = 0x000000A0;
    init_item_window(gItemWindowObjectByPlayerId[0]);
    for (someIndex = 0, something = 35.0f; someIndex < 8; someIndex++, something += 32.0) {
        D_8018D0C8[someIndex] = 40.0f;
        D_8018D028[someIndex] = -24.0f;
        D_8018D050[someIndex] = something;
        D_8018D0F0[someIndex] = something;
        D_8018D0A0[someIndex] = 0.0f;
        D_8018D078[someIndex] = 0.0f;
    }
    D_8018CFD4 = 1.0f;
    D_8018D3D4 = D_8018D3D8 = D_8018D3DC = 0x000000FF;
    D_8018D3E0 = why;
    D_8018D3E4 = 0x000000FF;
    D_8018D3E8 = 0x000000FF;
    D_8018D3EC = 0x000000FF;
    D_8018D3F0 = 0x000000FF;
    D_8018D3F4 = one;
    playerHUD[PLAYER_ONE].unk_4C = 0x0078;
    playerHUD[PLAYER_ONE].unk_4A = 0x00A0;
    playerHUD[PLAYER_ONE].rankScaling = 0.5f;
    D_801656B0 = 0;
    D_80165708 = 0x0028;
    D_8018D00C = 5.0f;
    D_8018D388 = 4;
    D_8018D380 = 0x00A0;
    D_8018D384 = 0x0078;
    D_8018D3C4 = 0x00000032;
    D_8018D3BC = 0x0028;
    D_8018D3C0 = 0x00000050;
    D_801657A2 = 0x0333;
    switch (gModeSelection) { /* irregular */
        case 0:
            D_8018D158 = 8;
            break;
        case 1:
            D_80165638 = (func_800B4F2C() & 0xFFFFF) - 1;
            D_80165648 = func_800B4E24(0) & 0xFFFFF;
            D_80165888 = 1;
            D_80165890 = 1;
            D_8018D158 = 1;
            break;
    }
}

void init_hud_two_player_vertical(void) {
    find_unused_obj_index(&D_80183DA0);

    find_unused_obj_index(&gIndexLakituList[0]);
    find_unused_obj_index(&gIndexLakituList[1]);

    find_unused_obj_index(&gItemWindowObjectByPlayerId[0]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[1]);

    init_object_list_index();
    func_8007055C();
    func_8007055C();
    init_course_object();

    playerHUD[PLAYER_ONE].itemBoxX = -0x52;
    playerHUD[PLAYER_ONE].itemBoxY = 0x32;
    playerHUD[PLAYER_ONE].slideItemBoxX = 0;
    playerHUD[PLAYER_ONE].slideItemBoxY = 0;
    playerHUD[PLAYER_ONE].unk_4A = 0x50;
    playerHUD[PLAYER_ONE].unk_4C = 0x78;
    playerHUD[PLAYER_ONE].rankX = 0x32;
    playerHUD[PLAYER_ONE].rankY = 0xD2;
    playerHUD[PLAYER_ONE].slideRankX = 0;
    playerHUD[PLAYER_ONE].slideRankY = 0;
    playerHUD[PLAYER_ONE].timerX = 0x4B;
    playerHUD[PLAYER_ONE].timerY = 0x10;
    playerHUD[PLAYER_ONE].lapX = 0x67;
    playerHUD[PLAYER_ONE].lapY = 0x28;
    init_item_window(gItemWindowObjectByPlayerId[PLAYER_ONE]);

    playerHUD[PLAYER_TWO].itemBoxX = 0x43;
    playerHUD[PLAYER_TWO].itemBoxY = 0x32;
    playerHUD[PLAYER_TWO].slideItemBoxX = 0;
    playerHUD[PLAYER_TWO].slideItemBoxY = 0;
    playerHUD[PLAYER_TWO].unk_4A = 0xF0;
    playerHUD[PLAYER_TWO].unk_4C = 0x78;
    playerHUD[PLAYER_TWO].rankX = 0xC8;
    playerHUD[PLAYER_TWO].rankY = 0xD2;
    playerHUD[PLAYER_TWO].slideRankX = 0;
    playerHUD[PLAYER_TWO].slideRankY = 0;
    playerHUD[PLAYER_TWO].timerX = 0xDC;
    playerHUD[PLAYER_TWO].timerY = 0x10;
    playerHUD[PLAYER_TWO].lapX = 0xF7;
    playerHUD[PLAYER_TWO].lapY = 0x28;
    init_item_window(gItemWindowObjectByPlayerId[PLAYER_TWO]);

    playerHUD[PLAYER_ONE].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[1];

    playerHUD[PLAYER_ONE].rankScaling = playerHUD[PLAYER_TWO].rankScaling = 0.5f;

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = 0x666;
    switch (gModeSelection) { /* irregular */
        case GRAND_PRIX:
            D_8018D158 = 8;
            break;
        case VERSUS:
            D_8018D158 = 2;
            break;
        case BATTLE:
            D_8018D158 = 2;
            break;
    }
}

void init_hud_two_player_horizontal() {
    find_unused_obj_index(&D_80183DA0);

    find_unused_obj_index(&gIndexLakituList[0]);
    find_unused_obj_index(&gIndexLakituList[1]);

    find_unused_obj_index(&gItemWindowObjectByPlayerId[0]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[1]);

    init_object_list_index();
    func_8007055C();
    func_8007055C();
    init_course_object();

    playerHUD[PLAYER_ONE].itemBoxY = 0x22;
    playerHUD[PLAYER_ONE].itemBoxX = -0x53;
    playerHUD[PLAYER_ONE].slideItemBoxX = 0;
    playerHUD[PLAYER_ONE].slideItemBoxY = 0;
    playerHUD[PLAYER_ONE].unk_4A = 0xA0;
    playerHUD[PLAYER_ONE].unk_4C = 0x3C;
    playerHUD[PLAYER_ONE].rankX = 0x34;
    playerHUD[PLAYER_ONE].rankY = 0x62;
    playerHUD[PLAYER_ONE].slideRankX = 0;
    playerHUD[PLAYER_ONE].slideRankY = 0;
    playerHUD[PLAYER_ONE].timerX = 0xEA;
    playerHUD[PLAYER_ONE].timerY = 0x10;
    playerHUD[PLAYER_ONE].lapX = 0x101;
    playerHUD[PLAYER_ONE].lapY = 0x6A;

    playerHUD[PLAYER_TWO].itemBoxX = -0x53;
    playerHUD[PLAYER_TWO].itemBoxY = 0x8F;
    playerHUD[PLAYER_TWO].slideItemBoxX = 0;
    playerHUD[PLAYER_TWO].slideItemBoxY = 0;
    playerHUD[PLAYER_TWO].unk_4A = 0xA0;
    playerHUD[PLAYER_TWO].unk_4C = 0xB4;
    playerHUD[PLAYER_TWO].rankX = 0x34;
    playerHUD[PLAYER_TWO].rankY = 0xD2;
    playerHUD[PLAYER_TWO].slideRankX = 0;
    playerHUD[PLAYER_TWO].slideRankY = 0;
    playerHUD[PLAYER_TWO].timerX = 0xEA;
    playerHUD[PLAYER_TWO].timerY = 0x7F;
    playerHUD[PLAYER_TWO].lapX = 0x101;
    playerHUD[PLAYER_TWO].lapY = 0xDA;

    if (gModeSelection == BATTLE) {
        playerHUD[PLAYER_ONE].itemBoxY = 0x5E;
        playerHUD[PLAYER_TWO].itemBoxY = 0xD0;
    }

    playerHUD[PLAYER_ONE].rankScaling = playerHUD[PLAYER_TWO].rankScaling = 0.5f;

    playerHUD[PLAYER_ONE].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[1];

    init_item_window(gItemWindowObjectByPlayerId[0]);
    init_item_window((gItemWindowObjectByPlayerId[1]));

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = 0x666;
    switch (gModeSelection) { /* irregular */
        case GRAND_PRIX:
            D_8018D158 = 8;
            return;
        case VERSUS:
            D_8018D158 = 2;
            return;
        case BATTLE:
            D_8018D158 = 2;
            return;
    }
}

void init_hud_three_four_player(void) {
    find_unused_obj_index(&D_80183DA0);

    find_unused_obj_index(&gIndexLakituList[0]);
    find_unused_obj_index(&gIndexLakituList[1]);
    find_unused_obj_index(&gIndexLakituList[2]);
    find_unused_obj_index(&gIndexLakituList[3]);

    find_unused_obj_index(&gItemWindowObjectByPlayerId[0]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[1]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[2]);
    find_unused_obj_index(&gItemWindowObjectByPlayerId[3]);

    init_object_list_index();
    init_course_object();

    playerHUD[PLAYER_ONE].itemBoxX = -0x36;
    playerHUD[PLAYER_ONE].itemBoxY = 0x36;
    playerHUD[PLAYER_ONE].slideItemBoxX = 0;
    playerHUD[PLAYER_ONE].slideItemBoxY = 0;
    playerHUD[PLAYER_ONE].unk_4A = 0x50;
    playerHUD[PLAYER_ONE].unk_4C = 0x3C;
    playerHUD[PLAYER_ONE].rankX = 0x25;
    playerHUD[PLAYER_ONE].rankY = 0x64;
    playerHUD[PLAYER_ONE].slideRankX = 0;
    playerHUD[PLAYER_ONE].slideRankY = 0;
    playerHUD[PLAYER_ONE].lapX = 0x8C;
    playerHUD[PLAYER_ONE].lapY = 0x60;
    playerHUD[PLAYER_ONE].unk_6C = 0xDE;
    playerHUD[PLAYER_ONE].unk_6E = 0xC8;

    playerHUD[PLAYER_TWO].itemBoxX = 0x175;
    playerHUD[PLAYER_TWO].itemBoxY = 0x36;
    playerHUD[PLAYER_TWO].slideItemBoxX = 0;
    playerHUD[PLAYER_TWO].slideItemBoxY = 0;
    playerHUD[PLAYER_TWO].unk_4A = 0xF0;
    playerHUD[PLAYER_TWO].unk_4C = 0x3C;
    playerHUD[PLAYER_TWO].rankX = 0x11A;
    playerHUD[PLAYER_TWO].rankY = 0x64;
    playerHUD[PLAYER_TWO].slideRankX = 0;
    playerHUD[PLAYER_TWO].slideRankY = 0;
    playerHUD[PLAYER_TWO].lapX = 0xB4;
    playerHUD[PLAYER_TWO].lapY = 0x60;
    playerHUD[PLAYER_TWO].unk_6C = 0xC8;
    playerHUD[PLAYER_TWO].unk_6E = 0xC8;

    playerHUD[PLAYER_THREE].itemBoxX = -0x36;
    playerHUD[PLAYER_THREE].itemBoxY = 0x2D;
    playerHUD[PLAYER_THREE].slideItemBoxX = 0;
    playerHUD[PLAYER_THREE].slideItemBoxY = 0;
    playerHUD[PLAYER_THREE].unk_4A = 0x50;
    playerHUD[PLAYER_THREE].unk_4C = 0xB4;
    playerHUD[PLAYER_THREE].rankX = 0x25;
    playerHUD[PLAYER_THREE].rankY = 0xD2;
    playerHUD[PLAYER_THREE].slideRankX = 0;
    playerHUD[PLAYER_THREE].slideRankY = 0;
    playerHUD[PLAYER_THREE].lapX = 0x8C;
    playerHUD[PLAYER_THREE].lapY = 0xD4;
    playerHUD[PLAYER_THREE].unk_6C = 0xDE;
    playerHUD[PLAYER_THREE].unk_6E = 0xC0;

    playerHUD[PLAYER_FOUR].itemBoxX = 0x175;
    playerHUD[PLAYER_FOUR].itemBoxY = 0x2D;
    playerHUD[PLAYER_FOUR].slideItemBoxX = 0;
    playerHUD[PLAYER_FOUR].slideItemBoxY = 0;
    playerHUD[PLAYER_FOUR].unk_4A = 0xF0;
    playerHUD[PLAYER_FOUR].unk_4C = 0xB4;
    playerHUD[PLAYER_FOUR].rankX = 0x11A;
    playerHUD[PLAYER_FOUR].rankY = 0xD2;
    playerHUD[PLAYER_FOUR].slideRankX = 0;
    playerHUD[PLAYER_FOUR].slideRankY = 0;
    playerHUD[PLAYER_FOUR].lapX = 0xB4;
    playerHUD[PLAYER_FOUR].lapY = 0xD4;
    playerHUD[PLAYER_FOUR].unk_6C = 0xC8;
    playerHUD[PLAYER_FOUR].unk_6E = 0xC0;

    if (gModeSelection == BATTLE) {
        playerHUD[PLAYER_ONE].itemBoxY = 0xC8;
        playerHUD[PLAYER_TWO].itemBoxY = 0xC8;
        playerHUD[PLAYER_THREE].itemBoxY = 0xB8;
        playerHUD[PLAYER_FOUR].itemBoxY = 0xB8;
    }

    playerHUD[PLAYER_ONE].rankScaling = playerHUD[PLAYER_TWO].rankScaling = playerHUD[PLAYER_THREE].rankScaling =
        playerHUD[PLAYER_FOUR].rankScaling = 0.5f;

    playerHUD[PLAYER_ONE].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[1];
    playerHUD[PLAYER_THREE].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[2];
    playerHUD[PLAYER_FOUR].stagingPosition = (s16) gGPCurrentRaceRankByPlayerId[3];

    init_item_window(gItemWindowObjectByPlayerId[0]);
    init_item_window(gItemWindowObjectByPlayerId[1]);
    init_item_window(gItemWindowObjectByPlayerId[2]);
    init_item_window(gItemWindowObjectByPlayerId[3]);

    playerHUD[PLAYER_ONE].unknownScaling = playerHUD[PLAYER_TWO].unknownScaling =
        playerHUD[PLAYER_THREE].unknownScaling = playerHUD[PLAYER_FOUR].unknownScaling = 1.5f;

    D_8018D158 = (s32) gPlayerCount;
    D_8018D3C4 = 0x00000014;
    D_8018D3BC = 0x00000010;
    D_8018D3C0 = 0x0000001E;
    D_801657A2 = 0x0888;
}
