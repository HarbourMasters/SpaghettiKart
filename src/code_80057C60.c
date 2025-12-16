/**
 * @file code_80057C60.c
 * @warning there are too many variables here
 */

#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <mk64.h>
#include <align_asset_macro.h>

#include "camera.h"
#include "code_80057C60.h"
#include "main.h"
#include "actors.h"
#include "code_800029B0.h"
#include "racing/memory.h"
#include <defines.h>
#include "math_util.h"
#include "math_util_2.h"
#include "code_80005FD0.h"
#include "render_player.h"
#include "render_objects.h"
#include "code_8006E9C0.h"
#include "update_objects.h"
#include "code_80086E70.h"
#include "effects.h"
#include <assets/models/data_800E8700.h>
#include "skybox_and_splitscreen.h"
#include <assets/models/common_data.h>
#include "audio/external.h"
#include "objects.h"
#include "bomb_kart.h"
#include "menus.h"
#include <assets/textures/other_textures.h>
#include "spawn_players.h"
#include "sounds.h"
#include "data/some_data.h"
#include <assets/textures/some_data.h>
#include "port/Game.h"
#include "engine/Matrix.h"
#include "engine/editor/Editor.h"
#include "port/interpolation/FrameInterpolation.h"

//! @warning this macro is undef'd at the end of this file
#define MAKE_RGB(r, g, b) (((r) << 0x10) | ((g) << 0x08) | (b << 0x00))

s32 D_80165590;
s32 D_80165594;
s32 D_80165598;
s32 D_8016559C;
UNUSED s32 D_801655A0;
s32 D_801655A4;
UNUSED s32 D_801655A8;
s32 D_801655AC;
UNUSED s32 D_801655B0;
s32 D_801655B4;
UNUSED s32 D_801655B8;
s32 D_801655BC;
s32 D_801655C0;
s32 D_801655C4;
s32 D_801655CC;
UNUSED s32 D_801655D0[2];
s32 D_801655D8;
UNUSED s32 D_801655DC[2];
s32 D_801655E8;
UNUSED s32 D_801655EC;
s32 D_801655F0;
UNUSED s32 D_801655F4;
s32 D_801655F8;
UNUSED s32 D_80165600[2];
s32 D_80165608;
UNUSED s32 D_80165610[2];
s32 D_80165618;
UNUSED s32 D_80165620[2];
s32 D_80165628;
UNUSED s32 D_80165630[2];
u32 D_80165638;
UNUSED s32 D_80165640[2];
u32 D_80165648;
UNUSED u32 D_80165650[2];
u32 D_80165658[8];
s32 D_80165678;
UNUSED s32 D_80165680[12];
u16 D_801656B0;
UNUSED s32 D_801656B8[2];
u16 D_801656C0;
UNUSED s32 D_801656C8[2];
u16 D_801656D0;
UNUSED s32 D_801656D8[2];
u16 D_801656E0;
UNUSED s32 D_801656E8[2];
s16 D_801656F0;
UNUSED s32 D_801656F8[4];
s16 D_80165708;
UNUSED s32 D_8016570C;
s16 D_80165710;
s16 D_80165730;
UNUSED s32 D_80165734;
//! Tracking a count of some object type, don't know what object type yet
s16 D_80165738;
UNUSED s32 D_8016573C;
s16 D_80165740;
UNUSED s32 D_80165744;
s16 D_80165748;
UNUSED s32 D_8016574C;

s16 gNumActiveThwomps;
s32 D_80165754;
ThwompSpawn* gThowmpSpawnList;

Vec4s D_80165760;
UNUSED s16 D_80165768;
s8 D_8016576A;
Vec4s D_80165770;
UNUSED s32 D_80165778;
Vec4s D_80165780;
UNUSED s32 D_80165788;
s16 D_8016578C;
UNUSED s16 D_8016578E;
s16 D_80165790;
UNUSED s16 D_80165792;
s16 D_80165794;
UNUSED s32 D_80165798;
s8 D_8016579C;
u16 D_8016579E;
UNUSED s16 D_801657A0;
//! Something related to the rotation(?) of ice in Sherbet Land
u16 D_801657A2;
UNUSED s32 D_801657A4;
UNUSED s16 D_801657A8[3];
s8 D_801657AE;
UNUSED s8 D_801657AF;
//! HUD related
s8 gHUDDisable;
UNUSED s8 D_801657B1;
s8 D_801657B2;
UNUSED s8 D_801657B3;
s8 D_801657B4;
s8 D_801657B8[16];
s8 D_801657C8;
s8 D_801657D0[8];
s8 D_801657D8;
UNUSED s16 D_801657DA[2];
UNUSED s8 D_801657E0;
s8 D_801657E1;
s8 D_801657E2;
s8 D_801657E3;
s8 gHUDModes;
s8 D_801657E5;
bool D_801657E6;
u8 D_801657E7;
bool D_801657E8;
UNUSED s32 D_801657EC;
bool D_801657F0;
UNUSED s32 D_801657F4;
bool D_801657F8;
s32 D_801657FC;
s8 D_80165800[2];
s32 D_80165804;
s8 D_80165808;
s32 D_8016580C;
bool D_80165810;
s32 D_80165814;
bool D_80165818;
s32 D_8016581C;
s8 D_80165820;
UNUSED s32 D_80165824;
s8 D_80165828;
Vec3su D_8016582C;
s8 D_80165832[2];
Vec3su D_80165834;
UNUSED s32 D_8016583A;
s8 D_80165840[3];
UNUSED s32 D_80165848[6];
s32 D_80165860;
UNUSED s32 D_80165864;
UNUSED s32 D_80165868;
s32 D_8016586C;
UNUSED s32 D_80165870[2];
s32 D_80165878;
s32 D_8016587C;
UNUSED s32 D_80165884;
s8 D_80165888;
UNUSED s32 D_8016588C;
s8 D_80165890;
UNUSED s32 D_80165894;
s8 D_80165898;
s32 D_8016589C;
UNUSED s32 D_801658A0[2];
s8 D_801658A8;
UNUSED s32 D_801658B0[3];
s8 D_801658BC;
UNUSED s32 D_801658C0;
UNUSED s16 D_801658C4;
s8 D_801658C6;
UNUSED s32 D_801658C8;
UNUSED s16 D_801658CC;
s8 D_801658CE;
UNUSED s32 D_801658D0;
UNUSED s16 D_801658D4;
s8 D_801658D6;
UNUSED s32 D_801658D8;
s8 D_801658DC;
UNUSED s32 D_801658E0;
s8 D_801658E4;
UNUSED s32 D_801658E8;
s8 D_801658EC;
UNUSED s32 D_801658F0;
s8 D_801658F4;
UNUSED s32 D_801658F8;
UNUSED s8 D_801658FC;
u8 sRandomItemIndex;
s8 D_801658FE;
u8 gControllerRandom;
s16 D_80165900;
UNUSED s32 D_80165904;
s8 D_80165908;
UNUSED s32 D_80165910[96];
s8 D_80165A90;
UNUSED s32 D_80165AA0[95];
UNUSED s32 D_80165C14;
Object gObjectList[OBJECT_LIST_SIZE];
UNUSED s32 D_80183D58;
s32 objectListSize;
Mtx D_80183D60;
/**
 * Use unknown. An object is reserved and its index is saved to
 * this variable, but it appears to go unreferenced
 **/
s32 D_80183DA0;
f32 D_80183DA8[4];
//! Lakitu?
s32 gIndexLakituList[4];
f32 D_80183DC8[4];
//! Indexes for the objects associated with the Bomb Karts
s32 gIndexObjectBombKart[NUM_BOMB_KARTS_MAX];
UNUSED s32 D_80183DF8[16];
//! Next free spot in gObjectParticle1? Wraps back around to 0 if it gets bigger than gObjectParticle1_SIZE
s32 gNextFreeObjectParticle1;
Vec3f D_80183E40;
//! Next free spot in gObjectParticle2? Wraps back around to 0 if it gets bigger than gObjectParticle2_SIZE
s32 gNextFreeObjectParticle2;
Vec3f D_80183E50;
//! Next free spot in gObjectParticle3?
s32 gNextFreeObjectParticle3;
UNUSED s32 D_80183E60[3];
//! Next free spot in gObjectParticle4? Wraps back around to 0 if it gets bigger than gObjectParticle4_SIZE
s32 gNextFreeObjectParticle4;
Vec3f D_80183E70;
//! Next free spot in gLeafParticle? Wraps back around to 0 if it gets bigger than gLeafParticle_SIZE
s32 gNextFreeLeafParticle;
Vec3su D_80183E80;
//! Appears to be a list of object list indices for the Item Window part of the HUD
s32 gItemWindowObjectByPlayerId[4];
Vec3su D_80183E98;
/**
 * Snowmen bodies in FrappeSnowland
 * Crabs in Koopa Troopa Beach
 * Hot air balloon in Luigi Raceway?
 * Neon signs in Rainbow Road?
 * Thwomps in Bower's Castle?
 * Penguins in Sherbet Land?
 * Flag Poles in Yoshi Valley?
 */
s32 indexObjectList1[32];
UNUSED s32 D_80183F20[2];
/**
 * Snowmen heads in Frappe Snowland
 * Chain Chomps in RaindbowRoad?
 * Trophy in award ceremony?
 * Seagulls in Koopa Troopa Beach?
 * Hedgehogs in Yoshi Valley?
 * Spawn for big fire breath in Bowser's Castle
 */
s32 indexObjectList2[32];
/**
 * Seemingly a list of textures for Lakitu
 * Never explicitly given data, data appears to be placed here
 * via some type of DMA.
 * I'm also not certain about its dimensions
 * I think the entires in this array are way over-sized
 */
const char* gLakituTextureBuffer[4][2];
/**
 * Boos in Banshee Boardwalk
 * Spawners for the 4 small fire breaths inside Bowser's Castle
 */
s32 indexObjectList3[32];
//! Seemingly a pointer to Lakitu texture(s)
const char** gLakituTexturePtr;
/**
 * Unused list of object indices
 */
s32 indexObjectList4[32];
//! Array of (4) Collisions?
Collision D_8018C0B0[4];
/**
 * List of object list indices used for:
 *   Moles in Moo Moo Farm
 *   Snow flakes in Frappe Snowland
 *   Segments of the fire breath from the statues in Bowser's Castle
 *   Potentially other things
 */
s32 gObjectParticle1[gObjectParticle2_SIZE];
Collision D_8018C3B0;
/**
 * List of object list indices used for:
 * - Bats in Banshee's Boardwalk (but only 1 player mode?)
 * - Train index 0 smoke in Kalimari Desert
 * - Ferry index 0 smoke in DK Jungle
 */
s32 gObjectParticle2[gObjectParticle2_SIZE];
// Maybe some unused Collision?
UNUSED Collision D_8018C5F0;
/**
 * List of object list indices used for:
 * - Train index 1 smoke in Kalimari Desert
 * - Ferry index 1 smoke in DK Jungle
 */
s32 gObjectParticle3[gObjectParticle3_SIZE];
Collision D_8018C830;
/**
 * List of object list indices. Used both for the fires in the DK Jungle cave
 * and, seemingly for the trail that shells leave behind them.
 * I think they're using the same texture, which would explain the dual use
 */
s32 gObjectParticle4[gObjectParticle4_SIZE];
/**
 * Seemingly a list of object list indices used for the leaves that sometimes fall
 * trees when you bonk into them
 */
s32 gLeafParticle[gLeafParticle_SIZE];
hud_player playerHUD[4];
/**
 * List of object list indices used by the clouds and stars in some stages
 * Also used for snowflakes like gObjectParticle1? Not sure what's up with that
 */
s32 D_8018CC80[D_8018CC80_SIZE];
struct_D_8018CE10 D_8018CE10[8];
//! Unknown object index, only set for Kalimari Desert, never read
s32 D_8018CF10;
Camera* D_8018CF14;
s16 D_8018CF18;
Player* D_8018CF1C;
s16 D_8018CF20;
UNUSED s32 D_8018CF24;
Player* D_8018CF28[8];
s16 D_8018CF48;
s16 D_8018CF50[8];
s16 D_8018CF60;
//! This may be a list of tilemap flags on a per-camera basis
s16 D_8018CF68[8];
s16 D_8018CF78;
/**
 * List of half-word character IDs indicating each character's
 * place in the current Grand Prix race's standings
 */
s16 gGPCurrentRaceCharacterIdByRank[8];
s16 D_8018CF90;
s16 D_8018CF98[8];
s16 D_8018CFA8;
s8 D_8018CFAC[4];
s16 D_8018CFB0;
s8 D_8018CFB4[4];
s16 D_8018CFB8;
s8 D_8018CFBC[4];
s16 D_8018CFC0;
s8 D_8018CFC4[4];
s16 D_8018CFC8;
f32 D_8018CFCC;
s16 D_8018CFD0;
f32 D_8018CFD4;
s16 D_8018CFD8;

s16 D_800E4730[] = { 0x00ff, 0x0000, 0x0000, 0x00ff, 0x00ff, 0x0000, 0x0000, 0x00ff, 0x0000, 0x0032, 0x00ff, 0x00ff,
                     0x0000, 0x0000, 0x00ff, 0x00ff, 0x0032, 0x00ff, 0x00ff, 0x0028, 0x0028, 0x0032, 0x00ff, 0x0064,
                     0x0082, 0x000f, 0x00ff, 0x0000, 0x0000, 0x0000,
                     // I'm not convinced these aren't just padding, but stuff doesn't match
                     // without them :/
                     0x0000 };

u8** D_800E4770[] = {
    &D_8018D420, &D_8018D424, &D_8018D428, &D_8018D428, &D_8018D42C, &D_8018D42C,
    &D_8018D430, &D_8018D430, &D_8018D434, &D_8018D434, &D_8018D434, &D_8018D434,
};

u8** D_800E47A0[] = {
    &D_8018D438, &D_8018D43C, &D_8018D440, &D_8018D444, &D_8018D448, &D_8018D44C, &D_8018D450, &D_8018D454,
    &D_8018D458, &D_8018D45C, &D_8018D460, &D_8018D464, &D_8018D468, &D_8018D46C, &D_8018D470,
};

s32 D_800E47DC[] = {
    MAKE_RGB(0xFB, 0xFF, 0xFB), MAKE_RGB(0xA0, 0x60, 0x11), MAKE_RGB(0xE0, 0xC0, 0x90), MAKE_RGB(0xD0, 0xB0, 0x80),
    MAKE_RGB(0x90, 0x70, 0x40), MAKE_RGB(0xC0, 0x70, 0x10), MAKE_RGB(0xD0, 0xF0, 0xFF), MAKE_RGB(0xE0, 0x90, 0x30),
    MAKE_RGB(0xC0, 0x90, 0x30), MAKE_RGB(0x60, 0x40, 0x20), MAKE_RGB(0xF0, 0xD0, 0xB0), MAKE_RGB(0xA0, 0x80, 0x30),
};

s32 D_800E480C[] = {
    MAKE_RGB(0xB0, 0xB0, 0xB0), MAKE_RGB(0x80, 0x40, 0x11), MAKE_RGB(0xB0, 0x80, 0x50), MAKE_RGB(0xA0, 0x70, 0x40),
    MAKE_RGB(0x60, 0x30, 0x11), MAKE_RGB(0x80, 0x40, 0x10), MAKE_RGB(0x70, 0x90, 0xA0), MAKE_RGB(0xA0, 0x60, 0x30),
    MAKE_RGB(0xA0, 0x70, 0x10), MAKE_RGB(0x30, 0x10, 0x11), MAKE_RGB(0xB0, 0xA0, 0x80), MAKE_RGB(0x80, 0x60, 0x10),
};

// UI Code?
void func_80057C60(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_80183D60), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
}

void func_80057CE4(void) {
    switch (D_8018D21C) {
        case 0:
            func_802A3730(gScreenOneCtx);
            break;
        case 1:
            func_802A3730(gScreenOneCtx);
            break;
        case 2:
            func_802A3730(gScreenTwoCtx);
            break;
        case 3:
            func_802A3730(gScreenOneCtx);
            break;
        case 4:
            func_802A3730(gScreenTwoCtx);
            break;
        case 8:
            func_802A3730(gScreenOneCtx);
            break;
        case 9:
            func_802A3730(gScreenTwoCtx);
            break;
        case 10:
            func_802A3730(gScreenThreeCtx);
            break;
        case 11:
            func_802A3730(gScreenFourCtx);
            break;
    }
}

void func_80057DD0(void) {
    if (D_801657B2 != 0) {
        func_8004C024(0xF, 0xB, 0x122, 0, 0xFF, 0, 0xFF);
        func_8004C148(0x131, 0xB, 0xDA, 0, 0xFF, 0, 0xFF);
        func_8004C024(0xF, 0xE5, 0x122, 0, 0xFF, 0, 0xFF);
        func_8004C148(0xF, 0xB, 0xDA, 0, 0xFF, 0, 0xFF);
        func_8004C024(0x16, 0x10, 0x114, 0xFF, 0, 0, 0xFF);
        func_8004C148(0x12A, 0x10, 0xD0, 0xFF, 0, 0, 0xFF);
        func_8004C024(0x16, 0xE0, 0x114, 0xFF, 0, 0, 0xFF);
        func_8004C148(0x16, 0x10, 0xD0, 0xFF, 0, 0, 0xFF);
        func_8004C024(0x18, 0x15, 0x110, 0, 0, 0xFF, 0xFF);
        func_8004C148(0x128, 0x15, 0xC4, 0, 0, 0xFF, 0xFF);
        func_8004C024(0x18, 0xDB, 0x110, 0, 0, 0xFF, 0xFF);
        func_8004C148(0x18, 0x15, 0xC4, 0, 0, 0xFF, 0xFF);
    }
}

void func_80057FC4(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if ((gHUDDisable != 0)) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, D_0D0076F8);
    set_matrix_hud_screen();

    if ((D_801657C8 != 0)) {
        return;
    }

    switch (arg0) {
        case 0:
            func_80051EBC();
            break;
        case 1:
            func_80051EF8();
            break;
        case 2:
            func_80051F9C();
            break;
        case 3:
            func_80052044();
            break;
        case 4:
            func_80052080();
            break;
    }
}

void render_object(ScreenContext* screen) {
    UNUSED Gfx* temp_v1;

    if (gHUDDisable != 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, screen->camera->perspectiveMatrix,
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, screen->camera->lookAtMatrix,
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);


    render_object_for_player(screen->camera->cameraId);
}

void render_player_snow_effect(Camera* camera) {
    UNUSED Gfx* temp_v1;

    if (gHUDDisable != 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

    gSPMatrix(gDisplayListHead++, camera->perspectiveMatrix,
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, camera->lookAtMatrix,
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    if (gGamestate != ENDING) {
        render_snowing_effect(camera->cameraId);
    }
}

void render_object_for_player(s32 cameraId) {
    CM_DrawObjects(cameraId);
    CM_DrawParticles(cameraId);
    CM_DrawTrackObjects(cameraId);
    CM_DrawEditor();

    render_object_smoke_particles(cameraId);
    render_object_leaf_particle(cameraId);

    if (D_80165730 != 0) {
        // render_balloons_grand_prix(cameraId);
    }
    if (gModeSelection == BATTLE) {
        CM_DrawBattleBombKarts(cameraId);
    }
}

void render_snowing_effect(s32 playerId) {
    if (IsFrappeSnowland()) {
        if (gGamestate != 9) {
            if ((D_8015F894 == 0) && (gPlayerCountSelection1 == 1)) {
                render_object_snowflakes_particles();
            }
        } else {
            render_object_snowflakes_particles();
        }
    }
    if (CM_GetProps()->LakituTowType == 1) {
        render_ice_block(playerId);
    }
}

void func_80058BF4(void) {
    gSPDisplayList(gDisplayListHead++, D_0D0076F8);
}

void func_80058C20(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(gDisplayListHead++, D_0D0076F8);

    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                func_80058F48();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!gDemoMode) {
                    func_80059358();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!gDemoMode) {
                    func_800593F0();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!gDemoMode) {
                    func_800594F0();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!gDemoMode) {
                    func_80059528();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!gDemoMode) {
                    func_800596A8();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!gDemoMode) {
                    func_80059710();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!gDemoMode) {
                    func_80059750();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!gDemoMode) && (gPlayerCountSelection1 == 4)) {
                    func_800597B8();
                }
                break;
        }
    }
}

void render_hud(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(gDisplayListHead++, D_0D0076F8);
    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                func_80058F78();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!gDemoMode) {
                    render_hud_2p_horizontal_player_two_horizontal_player_one();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!gDemoMode) {
                    render_hud_2p_horizontal_player_two();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!gDemoMode) {
                    render_hud_2p_vertical_player_one();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!gDemoMode) {
                    render_hud_2p_vertical_player_two();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!gDemoMode) {
                    render_hud_1p_multi();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!gDemoMode) {
                    render_hud_2p_multi();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!gDemoMode) {
                    render_hud_3p_multi();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!gDemoMode) && (gPlayerCountSelection1 == 4)) {
                    render_hud_4p_multi();
                }
                break;
        }
    }
}

void func_80058F48(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
    }
}

void func_80058F78(void) {
    if (gHUDDisable == 0) {

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("HudMatrix", 0);

        set_matrix_hud_screen();
        if ((!gDemoMode) && (gIsHUDVisible != 0) && (D_801657D8 == 0)) {
            draw_item_window(PLAYER_ONE);
            if (gHUDModes != 2) {
                render_hud_timer(PLAYER_ONE);
                draw_simplified_lap_count(PLAYER_ONE);
                func_8004EB38(0);
                if (D_801657E6 != false) {
                    if (CVarGetInteger("gEnableDigitalSpeedometer", false) == true) {
                        render_digital_speedometer(PLAYER_ONE);
                    }
                    render_speedometer(PLAYER_ONE);
                }
            }
        }

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }
}

void func_80059024(void) {
}

void func_8005902C(void) {

    if (D_8018D2AC != 0) {
        switch (gPlayerCountSelection1) {
            case 2:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                break;
            case 3:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                func_8004EB30(PLAYER_THREE);
                break;
            case 4:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                func_8004EB30(PLAYER_THREE);
                func_8004EB30(PLAYER_FOUR);
                break;
        }
    }
}

void func_800590D4(void) {
    if (D_8018D2A4 != 0) {
        if (gModeSelection != BATTLE) {
            switch (gPlayerCountSelection1) {
                case 1:
                    if (gModeSelection != TIME_TRIALS) {
                        func_8004E800(PLAYER_ONE);
                        break;
                    }
                    break;
                case 2:
                    func_8004E800(PLAYER_ONE);
                    func_8004E800(PLAYER_TWO);
                    break;
                case 3:
                    func_8004E998(PLAYER_ONE);
                    func_8004E998(PLAYER_TWO);
                    func_8004E998(PLAYER_THREE);
                    break;
                case 4:
                    func_8004E998(PLAYER_ONE);
                    func_8004E998(PLAYER_TWO);
                    func_8004E998(PLAYER_THREE);
                    func_8004E998(PLAYER_FOUR);
                    break;
            }
        }
    }
}

void func_800591B4(void) {

    if ((gHUDDisable == 0) && (D_800DC5B8 != 0)) {
        func_80057C60();
        gSPDisplayList(gDisplayListHead++, D_0D0076F8);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);

        if (gIsHUDVisible != 0) {
            if (D_801657D8 == 0) {
                if (D_801657F0 != false) {
                    func_800514BC();
                }
                if ((!gDemoMode) && (D_801657E8 != false)) {
                    if (D_80165800[0] != 0) {
                        func_8004EE54(0);
                        if (gModeSelection != BATTLE) {
                            set_minimap_finishline_position(0);
                        }
                        func_8004F3E4(0);
                    }
                    if ((gScreenModeSelection == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) && (D_80165800[1] != 0)) {
                        func_8004EE54(1);
                        if (gModeSelection != BATTLE) {
                            set_minimap_finishline_position(1);
                        }
                        func_8004F3E4(1);
                    }
                }
            }
            if ((gHUDModes != 2) && (gModeSelection == GRAND_PRIX) && (D_8018D2BC != 0)) {
                func_80050320();
            }
            func_800590D4();
        }
        func_8005902C();
        func_80057DD0();
        func_80057CE4();
    }
}

void func_80059358(void) {
}

void render_hud_2p_horizontal_player_two_horizontal_player_one(void) {
    if (gHUDDisable == 0) {
        render_hud_timer(PLAYER_ONE);
        if (playerHUD[PLAYER_ONE].lapCount != 3) {
            draw_hud_2d_texture_32x8(playerHUD[PLAYER_ONE].lapX, playerHUD[PLAYER_ONE].lapY,
                                     (u8*) common_texture_hud_lap); // draw the lap word
            draw_lap_count(playerHUD[PLAYER_ONE].lapX + 0xC, playerHUD[PLAYER_ONE].lapY - 4,
                           playerHUD[PLAYER_ONE].alsoLapCount);
            draw_item_window(PLAYER_ONE);
        }
    }
}

void func_800593F0(void) {
}

void render_hud_2p_horizontal_player_two(void) {
    if (gHUDDisable == 0) {
        render_hud_timer(PLAYER_TWO);
        if (playerHUD[PLAYER_TWO].lapCount != 3) {
            draw_hud_2d_texture_32x8(playerHUD[PLAYER_TWO].lapX, playerHUD[PLAYER_TWO].lapY,
                                     (u8*) common_texture_hud_lap);
            draw_lap_count(playerHUD[PLAYER_TWO].lapX + 0xC, playerHUD[PLAYER_TWO].lapY - 4,
                           playerHUD[PLAYER_TWO].alsoLapCount);
            draw_item_window(PLAYER_TWO);
        }
    }
}

void draw_simplified_hud(s32 playerId) {
    if ((gModeSelection != BATTLE) && (D_80165800[playerId] == 0) && (gIsHUDVisible != 0)) {
        render_hud_timer(playerId);
        draw_simplified_lap_count(playerId);
    }
    draw_item_window(playerId);
}

void func_800594F0(void) {
}

void render_hud_2p_vertical_player_one(void) {
    if (gHUDDisable == 0) {
        draw_simplified_hud(PLAYER_ONE);
    }
}

void func_80059528(void) {
}

void render_hud_2p_vertical_player_two(void) {
    if (gHUDDisable == 0) {
        draw_simplified_hud(PLAYER_TWO);
    }
}

void render_hud_lap_3p_4p(s32 playerId) {
    if (gModeSelection != BATTLE) {
        if (D_801657F8 && gIsHUDVisible) {
            // draw_hud_2d_texture_32x8(playerHUD[playerId].lapX, playerHUD[playerId].lapY, (u8*)
            // common_texture_hud_lap);
            draw_hud_2d_texture(playerHUD[playerId].lapX, playerHUD[playerId].lapY, 32, 8, common_texture_hud_lap);
            draw_lap_count(playerHUD[playerId].lapX - 12, playerHUD[playerId].lapY + 4,
                           playerHUD[playerId].alsoLapCount);
        }
        if (gHUDModes == 2) {
            if (playerHUD[playerId].unk_74 && D_80165608) {
                func_80047910(playerHUD[playerId].unk_6C, playerHUD[playerId].unk_6E, 0, 1.0f,
                              (u8*) common_tlut_portrait_bomb_kart_and_question_mark, common_texture_portrait_bomb_kart,
                              D_0D005AE0, 0x20, 0x20, 0x20, 0x20);
            }
        }
    }
    func_8004E6C4(playerId);
}

void func_800596A8(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
    }
}

void render_hud_1p_multi(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
        render_hud_lap_3p_4p(PLAYER_ONE);
    }
}

void func_80059710(void) {
}

void render_hud_2p_multi(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
        render_hud_lap_3p_4p(PLAYER_TWO);
    }
}

void func_80059750(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
    }
}

void render_hud_3p_multi(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
        render_hud_lap_3p_4p(PLAYER_THREE);
    }
}

void func_800597B8(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
    }
}

void render_hud_4p_multi(void) {
    if (gHUDDisable == 0) {
        set_matrix_hud_screen();
        render_hud_lap_3p_4p(PLAYER_FOUR);
    }
}

void func_80059820(s32 playerId) {
    hud_player* temp_v0;

    D_8018CF1C = &gPlayerOne[playerId];
    D_8018CF14 = &camera1[playerId];
    temp_v0 = &playerHUD[playerId];
    temp_v0->posXInt = (s32) D_8018CF1C->pos[0];
    temp_v0->posYInt = (s32) D_8018CF1C->pos[1];
    temp_v0->posZInt = (s32) D_8018CF1C->pos[2];
}

void randomize_seed_from_controller(s32 arg0) {
    struct Controller* controller = &gControllerOne[arg0];

    if ((controller->button & A_BUTTON) != 0) {
        gControllerRandom++;
    }
    if ((controller->button & B_BUTTON) != 0) {
        gControllerRandom++;
    }
    if ((controller->button & R_TRIG) != 0) {
        gControllerRandom++;
    }
}

void func_8005994C(void) {
    D_8018D214 = true;
}

void func_8005995C(void) {
    s32 i;
    Player* player = gPlayerOne;
    for (i = 0; i < 4; i++) {
        if ((D_80165890 != 0) && (player->type & PLAYER_INVISIBLE_OR_BOMB)) {
            player->currentItemCopy = ITEM_MUSHROOM;

            playerHUD[i].unk_75 = 2;
        }
        if ((player->type & PLAYER_INVISIBLE_OR_BOMB) && (player->currentItemCopy == ITEM_NONE)) {
            if (playerHUD[i].unk_75) {
                player->currentItemCopy = ITEM_MUSHROOM;
                --playerHUD[i].unk_75;
            }
        }
        ++player;
    }
    D_80165890 = 0;
}

void func_80059A88(s32 playerId) {
    func_80059820(playerId);
    if (!gDemoMode) {
        // update_object_lakitu(playerId); // Moved to CM_TickObjects60fps
        func_8007BB9C(playerId);
    }
}

void func_80059AC8(void) {
    s32 i;

    if ((gIsGamePaused == false) && (Editor_IsPaused() == false)) {
        func_8008C1D8(&D_80165678);
        gRaceFrameCounter++;
        for (i = 0; i < gPlayerCount; i++) {
            D_8018CF68[i] = func_8008A890(&camera1[i]);
        }

        for (i = 0; i < NUM_PLAYERS; i++) {
            func_800892E0(i);
        }

        switch (gScreenModeSelection) {
            case SCREEN_MODE_1P:
                if (gGamestate != CREDITS_SEQUENCE) {
                    func_80059A88(PLAYER_ONE);
                    if (gModeSelection == TIME_TRIALS) {
                        func_8005995C();
                    }
                } else {
                    func_80059820(PLAYER_ONE);
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                func_80059A88(PLAYER_ONE);
                func_80059A88(PLAYER_TWO);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                func_80059A88(PLAYER_ONE);
                func_80059A88(PLAYER_TWO);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                func_80059A88(PLAYER_ONE);
                func_80059A88(PLAYER_TWO);
                func_80059A88(PLAYER_THREE);
                func_80059A88(PLAYER_FOUR);
                break;
        }

        CM_TickObjects60fps();
    }
}

void func_80059C50(void) {
    s32 someIndex;
    s32 playerId;

    func_8005A3C0();
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        playerId = gGPCurrentRacePlayerIdByRank[someIndex];
        // I hate this dumb pointer access here
        gGPCurrentRaceCharacterIdByRank[someIndex] = (gPlayerOne + playerId)->characterId;
    }
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        D_8018CF98[someIndex] = gGPCurrentRaceRankByPlayerId[someIndex];
    }
}

void func_80059D00(void) {

    func_8005A99C();
    func_8005A3C0();
    func_8005A380();

    if (D_801657AE == 0) {
        switch (gScreenModeSelection) {
            case SCREEN_MODE_1P:
                randomize_seed_from_controller(PLAYER_ONE);
                if (D_8018D214 == false) {
                    func_80059820(PLAYER_ONE);
                    func_8005B914();
                    if (!gDemoMode) {
                        // func_8007AA44(0);
                    }
                    func_80078C70(0);
                    if (playerHUD[PLAYER_ONE].raceCompleteBool == 0) {
                        func_8005C360((gPlayerOne->speed / 18.0f) * 216.0f);
                    }
                    func_8005D0FC(PLAYER_ONE);
                } else {
                    func_80059820(PLAYER_ONE);
                    func_80078C70(1);
                    func_80059820(PLAYER_TWO);
                    func_80078C70(2);
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                randomize_seed_from_controller(PLAYER_ONE);
                randomize_seed_from_controller(PLAYER_TWO);
                func_80059820(PLAYER_ONE);
                func_8005D0FC(PLAYER_ONE);
                if (!gDemoMode) {
                    // func_8007AA44(0);
                }
                func_80078C70(1);
                func_8005D1F4(0);
                func_80059820(PLAYER_TWO);
                func_8005D0FC(PLAYER_TWO);
                if (!gDemoMode) {
                    // func_8007AA44(1);
                }
                func_80078C70(2);
                func_8005D1F4(1);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                randomize_seed_from_controller(PLAYER_ONE);
                randomize_seed_from_controller(PLAYER_TWO);
                func_80059820(PLAYER_ONE);
                func_8005D0FC(PLAYER_ONE);
                if (!gDemoMode) {
                    // func_8007AA44(0);
                }
                func_80078C70(3);
                func_8005D1F4(0);
                func_80059820(PLAYER_TWO);
                func_8005D0FC(PLAYER_TWO);
                if (!gDemoMode) {
                    // func_8007AA44(1);
                }
                func_80078C70(4);
                func_8005D1F4(1);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                randomize_seed_from_controller(PLAYER_ONE);
                randomize_seed_from_controller(PLAYER_TWO);
                randomize_seed_from_controller(PLAYER_THREE);
                randomize_seed_from_controller(PLAYER_FOUR);
                func_80059820(PLAYER_ONE);
                func_8005D0FC(PLAYER_ONE);
                if (!gDemoMode) {
                    // func_8007AA44(0);
                }
                func_8005D1F4(0);
                func_80059820(PLAYER_TWO);
                func_8005D0FC(PLAYER_TWO);
                if (!gDemoMode) {
                    // func_8007AA44(1);
                }
                func_8005D1F4(1);
                func_80059820(PLAYER_THREE);
                func_8005D0FC(PLAYER_THREE);
                if (!gDemoMode) {
                    // func_8007AA44(2);
                }
                func_8005D1F4(2);
                if (gPlayerCountSelection1 == 4) {
                    func_80059820(PLAYER_FOUR);
                    func_8005D0FC(PLAYER_FOUR);
                    if ((!gDemoMode) && (gPlayerCountSelection1 == 4)) {
                        // func_8007AA44(3);
                    }
                    func_8005D1F4(3);
                }
                break;
        }
        tick_objects();
        CM_TickObjects();
        CM_TickParticles();
        func_800744CC();
    }
}

void func_8005A070(void) {
    func_8008C1D8(&D_80165678);
    gMatrixHudCount = 0;
    D_801655C0 = 0;
    func_80041D34();
    if (gIsGamePaused == false && (Editor_IsPaused() == false)) {
        func_8005C728();
        if (gGamestate == ENDING) {
            // func_80086604();
            // func_80086D80();
            // update_cheep_cheep(1);
            // func_80077640();
            CM_TickObjects();
            CM_TickParticles();
        } else if (gGamestate == CREDITS_SEQUENCE) {
            func_80059820(PLAYER_ONE);
            func_80078C70(0);
            CM_TickObjects();
            CM_TickParticles();
        } else { // normal gameplay
            func_80059D00();
        }
    }
    func_8008C204();
    func_8008C1E0(&D_80165678, (s32) &D_801655F0);
}

void func_8005A14C(s32 playerId) {
    s32 objectIndex;
    s32 lapCount;
    Player* player;
    UNUSED s32 stackPadding;

    player = &gPlayerOne[playerId];
    objectIndex = D_8018CE10[playerId].objectIndex;
    lapCount = gLapCountByPlayerId[playerId];
    if (player->type & PLAYER_EXISTS) {
        if (player->effects & 0x204C0) {
            gObjectList[objectIndex].direction_angle[2] += 0x1000;
        } else {
            if (gObjectList[objectIndex].direction_angle[2] != 0) {
                gObjectList[objectIndex].direction_angle[2] += 0x1000;
            }
        }
        if (player->effects & LIGHTNING_EFFECT) {
            f32_step_towards(&gObjectList[objectIndex].sizeScaling, 0.3f, 0.02f);
        } else {
            f32_step_towards(&gObjectList[objectIndex].sizeScaling, 0.6f, 0.02f);
        }
        if (player->effects & HIT_EFFECT) {
            u16_step_up_towards(&gObjectList[objectIndex].direction_angle[0], 0x0C00U, 0x0100U);
        } else {
            u16_step_down_towards(&gObjectList[objectIndex].direction_angle[0], 0, 0x00000100);
        }
        if (player->effects & 0x03000000) {
            func_80087D24(objectIndex, 6.0f, 1.5f, 0.0f);
        } else {
            f32_step_towards(&gObjectList[objectIndex].offset[1], 0.0f, 1.0f);
        }
        if ((player->type & PLAYER_INVISIBLE_OR_BOMB) || (player->effects & BOO_EFFECT)) {
            gObjectList[objectIndex].primAlpha = 0x0050;
        } else {
            gObjectList[objectIndex].primAlpha = 0x00FF;
        }
        if (lapCount >= 3) {
            gObjectList[objectIndex].direction_angle[2] = 0;
            gObjectList[objectIndex].direction_angle[1] = 0;
            gObjectList[objectIndex].direction_angle[0] = 0;
            gObjectList[objectIndex].offset[2] = 0.0f;
            gObjectList[objectIndex].offset[1] = 0.0f;
            gObjectList[objectIndex].offset[0] = 0.0f;
            gObjectList[objectIndex].sizeScaling = 0.6f;
            gObjectList[objectIndex].primAlpha = 0x00FF;
        }
    }
}

void func_8005A380(void) {
    s32 temp_s0;
    for (temp_s0 = 0; temp_s0 < NUM_PLAYERS; ++temp_s0) {
        func_8005A14C(temp_s0);
    }
}

void func_8005A3C0(void) {
    bool b = false;
    if ((gGamestate != ENDING) && (gGamestate != CREDITS_SEQUENCE) && !D_8018D204) {
        switch (gPlayerCountSelection1) {
            case 1:
                if (gControllerOne->buttonPressed & R_CBUTTONS) {
                    if (++gHUDModes >= 3) {
                        gHUDModes = 0;
                    }
                    if (gHUDModes == 2) {
                        D_801657E8 = false;
                        D_801657E6 = false;
                        D_801657F0 = true;
                    } else if (gHUDModes == 1) {
                        D_801657E8 = false;
                        D_801657E6 = true;
                        D_801657F0 = false;
                    } else {
                        D_801657E8 = true;
                        D_801657E6 = false;
                        D_801657F0 = false;
                    }
                    b = true;
                }
                break;
            case 2:
                if (gModeSelection != BATTLE) {
                    if (gControllerOne->buttonPressed & R_CBUTTONS) {
                        D_80165800[0] = (D_80165800[0] + 1) & 1;
                        b = true;
                    }
                    if (gControllerTwo->buttonPressed & R_CBUTTONS) {
                        D_80165800[1] = (D_80165800[1] + 1) & 1;
                        b = true;
                    }
                    if (D_80165800[0] && D_80165800[1]) {
                        D_801657F0 = false;
                    } else {
                        D_801657F0 = true;
                    }
                    if (gDemoMode) {
                        D_801657F0 = false;
                    }
                }
                break;
            case 3:
                if ((gControllerOne->buttonPressed & R_CBUTTONS) || (gControllerTwo->buttonPressed & R_CBUTTONS) ||
                    (gControllerThree->buttonPressed & R_CBUTTONS)) {
                    if (gModeSelection != BATTLE) {
                        D_801657F0 = (D_801657F0 + 1) & 1;
                    }
                    gHUDModes = (gHUDModes + 1) & 1;
                    b = true;
                }
                break;
            case 4:
                if ((gControllerOne->buttonPressed & R_CBUTTONS) || (gControllerTwo->buttonPressed & R_CBUTTONS) ||
                    (gControllerThree->buttonPressed & R_CBUTTONS) || (gControllerFour->buttonPressed & R_CBUTTONS)) {
                    gHUDModes = (gHUDModes + 1) & 1;
                    D_801657F8 = (D_801657F8 + 1) & 1;
                    D_80165800[0] = (D_80165800[0] + 1) & 1;
                    if (gModeSelection != BATTLE) {
                        D_801657F0 = (D_801657F0 + 1) & 1;
                    }
                    b = true;
                }
                break;
        }
        if (b) {
            func_8006F824(1);
        }
    }
}

void func_8005A71C(void) {
    // if (IsBowsersCastle()) {
    // func_80081210();
    //}
}

void tick_objects(void) {

    CM_TickTrackObjects();

    func_80076F2C();
    if (!IsFrappeSnowland()) {
        update_leaf();
    }
}

void func_8005A99C(void) {
    if (D_8018D170 == 0) {
        if (D_8018D178 == 0) {
            if (gPlayerCountSelection1 == 1) {
                func_8005AA34();
            }
            if (gPlayerCountSelection1 == 3) {
                D_801657E8 = true;
            }
            if (Editor_IsEnabled() == false) {
                gIsHUDVisible = (s32) 1;
            }
            D_8018D170 = (s32) 1;
            D_8018D190 = (s32) 1;
            D_8018D204 = 0;
            return;
        }
        --D_8018D178;
    }
}

void func_8005AA34(void) {
    D_8018D1CC = 1;
    D_8018D1A0 = 0;
}

void func_8005AA4C(void) {
    ++D_8018D1CC;
    D_8018D1A0 = 0;
}

void func_8005AA6C(s32 arg0) {
    D_8018D1CC = arg0;
    D_8018D1A0 = 0;
}

void func_8005AA80(void) {
    D_8018D1CC = 0;
    D_8018D1A0 = 0;
}

void func_8005AA94(s32 arg0) {
    if (D_8018D1A0 == 0) {
        D_8018D1D4 = arg0;
        D_8018D1A0 = 1;
    }

    --D_8018D1D4;
    if (D_8018D1D4 < 0) {
        D_8018D1A0 = 0;
        func_8005AA4C();
    }
}

void func_8005AAF0(void) {
    D_8018D1B4 = 1;
    D_8018D1A0 = 0;
    func_8005AA4C();
}

void func_8005AB20(void) {
    if ((gModeSelection == GRAND_PRIX) && (gPlayerCountSelection1 == 1)) {
        func_8005AA6C(20);
    }
}

void func_8005AB60(void) {
    switch (playerHUD[PLAYER_ONE].unk_78) {
        case 0:
            break;
        case 1:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x106, 0x10);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xB6, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
                playerHUD[PLAYER_ONE].unk_79 = 1;
            }
            break;
        case 2:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x116, 4);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xC6, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 3:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x106, 4);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xB6, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 4:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x10E, 4);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xBE, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 5:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x106, 4);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xB6, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 6:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x10A, 2);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xBA, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 7:
            s16_step_towards(&playerHUD[PLAYER_ONE].speedometerX, 0x106, 2);
            if (s16_step_towards(&playerHUD[PLAYER_ONE].speedometerY, 0xB6, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_78++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_78 = 0;
            break;
    }
    if ((playerHUD[PLAYER_ONE].unk_79 != 0) && (playerHUD[PLAYER_ONE].unk_79 == 1)) {
        if (++D_801657E7 >= 0x10) {
            D_801657E7 = 0;
            D_8016579E = 0xDD00;
            playerHUD[PLAYER_ONE].unk_79 = 0U;
        } else {
            D_8016579E = D_800E55B0[D_801657E7] + 0xDD00;
        }
    }
    switch (playerHUD[PLAYER_ONE].unk_80) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x38, 8) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x38, 8) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x38, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].slideItemBoxY, 0x40, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_80++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_80 = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].unk_7A) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].timerX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7A++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7A = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].unk_7D) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapX, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7D++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7D = 0;
            break;
    }
    D_8018CFEC = (f32) (playerHUD[PLAYER_ONE].speedometerX + 0x18);
    D_8018CFF4 = (f32) (playerHUD[PLAYER_ONE].speedometerY + 6);
    switch (playerHUD[PLAYER_ONE].unk_7B) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7B++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7B = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].unk_7E) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage1X, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7E++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7E = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].unk_7C) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7C++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7C = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].unk_7F) {
        case 0:
            break;
        case 1:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 2:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 3:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 4:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 5:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 6:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 7:
            if (s16_step_towards(&playerHUD[PLAYER_ONE].lapAfterImage2X, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].unk_7F++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].unk_7F = 0;
            break;
    }
}

void func_8005B7A0(void) {
    f32 temp_f0;
    f32* temp_s2;
    f32* temp_s3;
    f32* temp_s4;
    UNUSED f32* var_s1;
    s32 var_s0;

    s16_step_towards(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 0x10);
    s16_step_towards(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 0x10);
    s16_step_towards(&playerHUD[PLAYER_ONE].lap3CompletionTimeX, 0xE4, 0x10);
    s16_step_towards(&playerHUD[PLAYER_ONE].totalTimeX, 0xE4, 0x10);
    for (var_s0 = 0; var_s0 < NUM_PLAYERS; var_s0++) {
        temp_s2 = &D_8018D028[var_s0];
        temp_s3 = &D_8018D0C8[var_s0];
        temp_s4 = &D_8018D078[var_s0];
        if (D_8018D050[var_s0] >= 0.0f) {
            f32_step_towards(temp_s2, *temp_s3, *temp_s4);
            temp_f0 = *temp_s2;
            if (temp_f0 == *temp_s3) {
                *temp_s4 = 0.0f;
            }
            if ((f64) temp_f0 <= -32.0) {
                D_8018D050[var_s0] = -32.0f;
            }
        }
    }
}

void func_8005B914(void) {
    s32 i;
    UNUSED s32 unk;

    switch (D_8018D1CC) {
        case 0:
            break;
        case 0x1:
            func_8005AAF0();
            break;
        case 0x2:
            if (gModeSelection == TIME_TRIALS) {
                playerHUD[PLAYER_ONE].unk_80 = 1;
            }
            playerHUD[PLAYER_ONE].unk_78 = 1;
            playerHUD[PLAYER_ONE].unk_7A = 1;
            playerHUD[PLAYER_ONE].unk_7D = 1;
            func_8005AA4C();
            break;
        case 0x3:
            func_8005AA94(0);
            break;
        case 0x4:
            playerHUD[PLAYER_ONE].unk_7B = 1;
            playerHUD[PLAYER_ONE].unk_7E = 1;
            func_8005AA4C();
            break;
        case 0x5:
            func_8005AA94(0);
            break;
        case 0x6:
            playerHUD[PLAYER_ONE].unk_7C = 1;
            playerHUD[PLAYER_ONE].unk_7F = 1;
            func_8005AA4C();
            func_8005AA80();
            break;
        case 0x14:
            D_8018D078[0] = 16.0f;
            func_8005AA4C();
            break;
        case 0x15:
            func_8005AA94(4);
            break;
        case 0x16:
            D_8018D078[1] = 16.0f;
            func_8005AA4C();
            break;
        case 0x17:
            func_8005AA94(4);
            break;
        case 0x18:
            D_8018D078[2] = 16.0f;
            func_8005AA4C();
            break;
        case 0x19:
            func_8005AA94(4);
            break;
        case 0x1A:
            D_8018D078[3] = 16.0f;
            func_8005AA4C();
            break;
        case 0x1B:
            func_8005AA94(0xA);
            break;
        case 0x1C:
            func_8005AA80();
            break;
        case 0x64:
            func_8005AA4C();
            break;
        case 0x65:
            func_8005AA94(0x3A);
            break;
        case 0x66:
            D_8018D078[0] = -8.0f;
            D_8018D0C8[0] = -32.0f;
            func_8005AA4C();
            break;
        case 0x67:
            func_8005AA94(4);
            break;
        case 0x68:
            D_8018D078[1] = -8.0f;
            D_8018D0C8[1] = -32.0f;
            func_8005AA4C();
            break;
        case 0x69:
            func_8005AA94(4);
            break;
        case 0x6A:
            D_8018D078[2] = -8.0f;
            D_8018D0C8[2] = -32.0f;
            func_8005AA4C();
            break;
        case 0x6B:
            func_8005AA94(4);
            break;
        case 0x6C:
            D_8018D078[3] = -8.0f;
            D_8018D0C8[3] = -32.0f;
            func_8005AA4C();
            break;
        case 0x6D:
            func_8005AA94(0xA);
            break;
        case 0x6E:
            for (i = 0; i < NUM_PLAYERS; i += 4) {
                D_8018D050[i] = -32.0f;
                D_8018D050[i + 1] = -32.0f;
                D_8018D050[i + 2] = -32.0f;
                D_8018D050[i + 3] = -32.0f;
            }
            D_8018D028[0] = 360.0f;
            D_8018D050[0] = 110.0f;
            D_8018D0C8[0] = 44.0f;
            D_8018D078[0] = -16.0f;
            D_801657E2 = 1;
            func_8005AA4C();
            break;
        case 0x6F:
            func_8005AA94(4);
            break;
        case 0x70:
            D_8018D028[1] = 360.0f;
            D_8018D050[1] = 110.0f;
            D_8018D0C8[1] = 76.0f;
            D_8018D078[1] = -16.0f;
            func_8005AA4C();
            break;
        case 0x71:
            func_8005AA94(4);
            break;
        case 0x72:
            D_8018D028[2] = 360.0f;
            D_8018D050[2] = 110.0f;
            D_8018D0C8[2] = 108.0f;
            D_8018D078[2] = -16.0f;
            func_8005AA4C();
            break;
        case 0x73:
            func_8005AA94(4);
            break;
        case 0x74:
            D_8018D028[3] = 360.0f;
            D_8018D050[3] = 110.0f;
            D_8018D0C8[3] = 140.0f;
            D_8018D078[3] = -16.0f;
            func_8005AA4C();
            break;
        case 0x75:
            func_8005AA94(4);
            break;
        case 0x76:
            D_8018D028[4] = 360.0f;
            D_8018D050[4] = 110.0f;
            D_8018D0C8[4] = 180.0f;
            D_8018D078[4] = -16.0f;
            func_8005AA4C();
            break;
        case 0x77:
            func_8005AA94(4);
            break;
        case 0x78:
            D_8018D028[5] = 360.0f;
            D_8018D050[5] = 110.0f;
            D_8018D0C8[5] = 212.0f;
            D_8018D078[5] = -16.0f;
            func_8005AA4C();
            break;
        case 0x79:
            func_8005AA94(4);
            break;
        case 0x7A:
            D_8018D028[6] = 360.0f;
            D_8018D050[6] = 110.0f;
            D_8018D0C8[6] = 244.0f;
            D_8018D078[6] = -16.0f;
            func_8005AA4C();
            break;
        case 0x7B:
            func_8005AA94(4);
            break;
        case 0x7C:
            D_8018D028[7] = 360.0f;
            D_8018D050[7] = 110.0f;
            D_8018D0C8[7] = 276.0f;
            D_8018D078[7] = -16.0f;
            func_8005AA4C();
            break;
        case 0x7D:
            func_8005AA94(0xA);
            break;
        case 0x7E:
            for (i = 0; i < NUM_PLAYERS; i++) {
                D_8018D078[i] = 0.0f;
            }
            func_8005AA4C();
            break;
        case 0x7F:
            func_8005AA94(0x82);
            break;
        case 0x80:
            if (gGPCurrentRaceRankByPlayerId[0] < 4) {
                func_8005AA6C(0x8C);
            } else {
                func_8005AA6C(0x82);
            }
            break;
        case 0x82:
            func_8005AA80();
            break;
        case 0x8C:
            D_8018D078[0] = -16.0f;
            D_8018D0C8[0] = -32.0f;
            func_8005AA4C();
            break;
        case 0x8D:
            func_8005AA94(4);
            break;
        case 0x8E:
            D_8018D078[1] = -16.0f;
            D_8018D0C8[1] = -32.0f;
            func_8005AA4C();
            break;
        case 0x8F:
            func_8005AA94(4);
            break;
        case 0x90:
            D_8018D078[2] = -16.0f;
            D_8018D0C8[2] = -32.0f;
            func_8005AA4C();
            break;
        case 0x91:
            func_8005AA94(4);
            break;
        case 0x92:
            D_8018D078[3] = -16.0f;
            D_8018D0C8[3] = -32.0f;
            func_8005AA4C();
            break;
        case 0x93:
            func_8005AA94(4);
            break;
        case 0x94:
            D_8018D078[4] = -16.0f;
            D_8018D0C8[4] = -32.0f;
            func_8005AA4C();
            break;
        case 0x95:
            func_8005AA94(4);
            break;
        case 0x96:
            D_8018D078[5] = -16.0f;
            D_8018D0C8[5] = -32.0f;
            func_8005AA4C();
            break;
        case 0x97:
            func_8005AA94(4);
            break;
        case 0x98:
            D_8018D078[6] = -16.0f;
            D_8018D0C8[6] = -32.0f;
            func_8005AA4C();
            break;
        case 0x99:
            func_8005AA94(4);
            break;
        case 0x9A:
            D_8018D078[7] = -16.0f;
            D_8018D0C8[7] = -32.0f;
            func_8005AA4C();
            break;
        case 0x9B:
            func_8005AA94(0x14);
            break;
        case 0x9C:
            func_8005AA80();
            break;
    }
    if (D_8018D1CC < 0x64) {
        func_8005AB60();
    } else if (D_8018D1CC < 0xC8) {
        func_8005B7A0();
    }
    if ((D_8018D1CC != 0) && (D_8018D1CC >= 20) && (D_8018D1CC < 0x1E)) {
        for (i = 0; i < 4; i++) {
            f32_step_towards(&D_8018D028[i], D_8018D0C8[i], D_8018D078[i]);
            if (D_8018D028[i] == D_8018D0C8[i]) {
                D_8018D078[i] = 0.0f;
            }
        }
    }
}

void func_8005C360(f32 arg0) {
    if (!playerHUD[PLAYER_ONE].unk_79) {
        u16 v;
        if (arg0 < 10.0) {
            v = (u16) (128.0f * arg0) + 0xDD00;
        } else if (arg0 < 20.0) {
            v = (u16) ((arg0 - 10.0) * 256.0) + 0xE200;
        } else {
            v = (u16) ((arg0 - 20.0) * 268.8) + 0xEC00;
        }
        if (arg0 == D_8018CFE4) {
            if (arg0 > 5.0f) {
                if (++D_801657E7 == 8) {
                    D_801657E7 = 0;
                }
            } else {
                D_801657E7 = 0;
            }
        }
        D_8016579E = v + D_800E55A0[D_801657E7];
        D_8018CFE4 = arg0;
    }
}

void func_8005C64C(UNUSED s32* arg0) {
}

void func_8005C654(s32* arg0) {
    *arg0 = 0;
}

void func_8005C65C(s32 arg0) {
    D_8018D2C8[arg0] = 1;
}

void func_8005C674(s8 index, s16* x, s16* y, s16* z) {
    s16* src = &D_800E4730[index * 3];
    *x = *src++;
    *y = *src++;
    *z = *src++;
}

void func_8005C6B4(s8 arg0, s16* arg1, s16* arg2, s16* arg3) {
    switch (arg0) {
        case 0:
            *arg1 = 0xFF;
            *arg2 = 0x40;
            *arg3 = 0x40;
            break;
        case 1:
            *arg1 = 0xFF;
            *arg2 = 0xFF;
            *arg3 = 0x40;
            break;
        case 2:
            *arg1 = 0x40;
            *arg2 = 0x40;
            *arg3 = 0xFF;
            break;
    }
}

void func_8005C728(void) {
    s16 x;
    s16 y;
    s16 z;
    s32 temp_t7;

    temp_t7 = ++D_8018D400;
    D_8018D40C = temp_t7 & 0x3F; // temp_t7 % 64
    D_8018D410 = temp_t7 & 0x1F; // temp_t7 % 32
    D_80165590 = temp_t7 & 0xF;  // temp_t7 % 16
    D_80165594 = temp_t7 & 7;    // temp_t7 % 8
    D_80165598 = temp_t7 & 3;    // temp_t7 % 4
    D_8016559C = temp_t7 & 1;    // temp_t7 % 2
    if (D_8018D40C == 0) {
        D_801655A4 += 1;
        D_801655D8 ^= 1;
    }
    if (D_8018D410 == 0) {
        D_801655AC += 1;
        D_801655E8 ^= 1;
    }
    if (D_80165590 == 0) {
        D_801655B4 += 1;
        D_801655F8 ^= 1;
    }
    if (D_80165594 == 0) {
        D_801655BC += 1;
        D_80165608 ^= 1;
    }
    if (D_80165598 == 0) {
        D_801655C4 += 1;
        D_80165618 ^= 1;
    }
    if (D_8016559C == 0) {
        D_801655CC += 1;
        D_80165628 ^= 1;
    }
    if (--D_8018D2AC < 0) {
        D_8018D2AC = 0;
    }
    D_801658A8 += 1;
    if (D_801658A8 >= 7) {
        D_801658A8 = 0;
    }
    func_8005C674(D_801658A8, &x, &y, &z);
    D_801656C0 = x / 2;
    D_801656D0 = y / 2;
    D_801656E0 = z / 2;
    func_8005C980();
}

void func_8005C980(void) {
    s32 var_v0;
    s32 sp0 = 0; //! @warning this variable was uninitialized in decomp. This was likely supposed to be random!
    s32 temp_v1;
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        temp_v1 = gGPCurrentRaceRankByPlayerId[var_v0];
        if (D_80165590 == 0) {
            D_8018CF98[var_v0] = temp_v1;
        }
        D_8018CF28[temp_v1] = &gPlayerOne[sp0];
        if (sp0 == 0) {
            D_80165794 = temp_v1;
        }
    }

    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        sp0 = gGPCurrentRacePlayerIdByRank[var_v0];
        D_8018CF50[var_v0] = sp0;
        if (D_80165590 == 0) {
            gGPCurrentRaceCharacterIdByRank[var_v0] = (gPlayerOne + sp0)->characterId;
        }
    }

    if (--D_8018D314 <= 0) {
        D_8018D314 = D_8018D3F4;
        D_8018D3E4 = D_800E55D0[D_8018D3F8][0];
        D_8018D3E8 = D_800E55D0[D_8018D3F8][1];
        D_8018D3EC = D_800E55D0[D_8018D3F8][2];
        if (++D_8018D3F8 == 6) {
            D_8018D3F8 = 0;
        }
    }
}

void func_8005CB60(s32 playerId, s32 lapCount) {
    s32 temp_a0_2;
    UNUSED s32 stackPadding;
    s8* huh;
    s8* huhthedeuce;
    Player* player;

    player = &gPlayerOne[playerId];
    huh = &playerHUD[playerId].alsoLapCount;
    huhthedeuce = &playerHUD[playerId].lapCount;
    if (playerHUD[playerId].lapCount < D_8018D320) {
        playerHUD[playerId].someTimer = (u32) (s32) (gCourseTimer * 100.0f);
        if (*huh < lapCount) {
            temp_a0_2 = gTimePlayerLastTouchedFinishLine[playerId] * 100.0f;
            playerHUD[playerId].timeLastTouchedFinishLine = temp_a0_2;
            playerHUD[playerId].lapCompletionTimes[*huh] = temp_a0_2;
            if (*huh == 0) {
                playerHUD[playerId].lapDurations[*huh] = playerHUD[playerId].timeLastTouchedFinishLine;
            } else {
                playerHUD[playerId].lapDurations[*huh] =
                    playerHUD[playerId].lapCompletionTimes[*huh] - playerHUD[playerId].lapCompletionTimes[*huh - 1];
            }
            playerHUD[playerId].someTimer1 = playerHUD[playerId].lapDurations[*huh];
            playerHUD[playerId].blinkTimer = 0x003C;
            if (lapCount == 3) {
                playerHUD[playerId].someTimer = playerHUD[playerId].lapCompletionTimes[*huh];
            }
            if (gModeSelection == (s32) 1) {
                if (D_80165638 >= playerHUD[playerId].someTimer1) {
                    if (D_80165638 != playerHUD[playerId].someTimer1) {
                        D_80165658[0] = D_80165658[1] = 0;
                    }
                    func_800C90F4(0U, (player->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    D_80165638 = playerHUD[playerId].someTimer1;
                    D_80165658[lapCount - 1] = 1;
                    D_801657E3 = 1;
                }
                if ((lapCount == 3) && ((u32) playerHUD[playerId].someTimer < (u32) D_80165648)) {
                    D_801657E5 = 1;
                }
            }
            *huh += 1;
            if (D_8018D320 == *huh) {
                *huh = D_8018D320 - 1;
            }
            *huhthedeuce += 1;
            if (1) {}
            switch (*huhthedeuce) { /* switch 1; irregular */
                case 0:             /* switch 1 */
                    break;
                case 1:                                   /* switch 1 */
                    CM_ActivateSecondLapLakitu(playerId); // func_80079084(playerId);
                    func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF0, 0x15));
                    if ((IsLuigiRaceway()) && (D_80165898 == 0) && (gModeSelection != (s32) TIME_TRIALS)) {
                        D_80165898 = 1;
                    }
                    break;
                case 2:                                  /* switch 1 */
                    CM_ActivateFinalLapLakitu(playerId); // func_800790B4(playerId);
                    break;
                case 3: /* switch 1 */
                    if ((D_8018D114 == 0) || (D_8018D114 == 1)) {
                        gHUDModes = 0;
                        D_801657E6 = 0;
                        D_801657F0 = 0;
                        D_801657E8 = 1;
                        D_80165800[0] = 1;
                        D_80165800[1] = 1;
                        D_8018D204 = (s32) 1;
                    }
                    playerHUD[playerId].raceCompleteBool = 1;
                    if (D_8018D114 == 2) {
                        D_80165800[playerId] = 0;
                    }
                    if (IsYoshiValley()) {
                        playerHUD[playerId].unk_81 = 1;
                    }
                    playerHUD[playerId].lap1CompletionTimeX = 0x0140;
                    playerHUD[playerId].lap2CompletionTimeX = 0x01E0;
                    playerHUD[playerId].lap3CompletionTimeX = 0x0280;
                    playerHUD[playerId].totalTimeX = 0x0320;
                    D_8016587C = (s32) 1;
                    if (D_8018D20C == 0) {
                        CM_ActivateFinishLakitu(playerId); // func_80079054(playerId);
                        D_8018D20C = 1;
                        if (gPlayerCount == (s8) 1) {
                            D_8018D1CC = 0x00000064;
                        }
                    }
                    break;
            }
        }
    } else {
        f32_step_towards(&playerHUD[playerId].rankScaling, 1.0f, 0.125f);
        switch (gScreenModeSelection) { /* irregular */
            case 0:
                s16_step_towards(&playerHUD[playerId].slideRankX, 0x001C, 7);
                if (D_8018D1FC != 0) {
                    s16_step_towards(&playerHUD[playerId].slideRankY, -0x0028, 1);
                } else {
                    s16_step_towards(&playerHUD[playerId].slideRankY, -0x0010, 4);
                }
                break;
            case 2:
                s16_step_towards(&playerHUD[playerId].slideRankX, 0x001C, 7);
                s16_step_towards(&playerHUD[playerId].slideRankY, -0x0010, 4);
                break;
            case 1:
                s16_step_towards(&playerHUD[playerId].slideRankX, 0x001C, 7);
                s16_step_towards(&playerHUD[playerId].slideRankY, -0x0010, 4);
                s16_step_towards(&playerHUD[playerId].lap1CompletionTimeX, 0x00E4, 0x0010);
                s16_step_towards(&playerHUD[playerId].lap2CompletionTimeX, 0x00E4, 0x0010);
                s16_step_towards(&playerHUD[playerId].lap3CompletionTimeX, 0x00E4, 0x0010);
                s16_step_towards(&playerHUD[playerId].totalTimeX, 0x00E4, 0x0010);
                break;
            case 3:
                if ((playerId & 1) == 1) {
                    s16_step_towards(&playerHUD[playerId].slideRankX, -8, 2);
                } else {
                    s16_step_towards(&playerHUD[playerId].slideRankX, 8, 2);
                }
                s16_step_towards(&playerHUD[playerId].slideRankY, -0x0010, 4);
                break;
        }
    }
    if (playerHUD[playerId].blinkTimer == 0) {
        playerHUD[playerId].someTimer1 = playerHUD[playerId].someTimer;
        D_801657E3 = 0;
        return;
    }
    if (D_80165594 == 0) {
        playerHUD[playerId].blinkState += 1;
        playerHUD[playerId].blinkState &= 1;
    }
    playerHUD[playerId].blinkTimer -= 1;
    if (playerHUD[playerId].blinkTimer == 0) {
        playerHUD[playerId].blinkState = 0;
    }
}

void func_8005D0FC(s32 playerId) {
    if (gModeSelection != BATTLE) {
        switch (playerId) { /* irregular */
            case PLAYER_ONE:
                func_8005CB60(playerId, gLapCountByPlayerId[PLAYER_ONE]);
                break;
            case PLAYER_TWO:
                func_8005CB60(playerId, gLapCountByPlayerId[PLAYER_TWO]);
                break;
            case PLAYER_THREE:
                func_8005CB60(playerId, gLapCountByPlayerId[PLAYER_THREE]);
                break;
            case PLAYER_FOUR:
                func_8005CB60(playerId, gLapCountByPlayerId[PLAYER_FOUR]);
                break;
        }
    }
}

void func_8005D18C(void) {
    if ((gModeSelection == GRAND_PRIX) && (gPlayerCountSelection1 == TIME_TRIALS)) {
        D_801657D8 = 1;
        D_8018D2BC = 0;
        D_8018D2A4 = 0;
        if (gGPCurrentRaceRankByPlayerId[0] >= 4) {
            D_8018D1FC = 1;
            D_8018D2A4 = 1;
            D_8018D2BC = 1;
        }
    }
}

void func_8005D1F4(s32 cameraId) {

    CM_BombKartsWaypoint(cameraId);
    // s32 playerWaypoint;
    // s32 bombWaypoint;
    // s32 var_a2;
    // s32 waypointDiff;

    // if (gModeSelection == 2) {
    //     playerWaypoint = gNearestPathPointByPlayerId[cameraId];
    //     playerHUD[cameraId].unk_74 = 0;
    //     for (var_a2 = 0; var_a2 < NUM_BOMB_KARTS_VERSUS; var_a2++) {
    //         if ((gBombKarts[var_a2].state == BOMB_STATE_EXPLODED) ||
    //             (gBombKarts[var_a2].state == BOMB_STATE_INACTIVE)) {
    //             continue;
    //         }
    //         bombWaypoint = gBombKarts[var_a2].waypointIndex;
    //         waypointDiff = bombWaypoint - playerWaypoint;
    //         if ((waypointDiff < -5) || (waypointDiff > 0x1E)) {
    //             continue;
    //         }
    //         playerHUD[cameraId].unk_74 = 1;
    //         break;
    //     }
    // }
}

// Appears to load GP Mode race staging balloons and kart shadows.
void func_8005D290(void) {
    D_8018D420 = gTexture69B03C;
    D_8018D424 = gTexture69B140;
    D_8018D478 = gTexture69C1E8;
    D_8018D480 = gTextureSpeechBubble;
    D_8018D484 = gTextureMusicNote;
    D_8018D48C = gTextureSmokePuff;
    D_8018D490 = D_8018D48C;
    D_8018D4BC = gTextureBalloon1;
    D_8018D4C0 = gTextureBalloon2;
    D_8018D49C = gTexture69C9C4;
    D_8018D4A0 = gTextureBoingExclamation;
    D_8018D438 = gTexture69CB84;
    D_8018D43C = gTexture69CCEC;
    D_8018D440 = gTexture69CEB8;
    D_8018D444 = gTexture69D148;
    D_8018D448 = gTexture69D4E0;
    D_8018D44C = gTexture69D8FC;
    D_8018D450 = gTexture69DCB4;
    D_8018D454 = gTexture69DFA0;
    D_8018D458 = gTexture69E25C;
    D_8018D45C = gTexture69E518;
    D_8018D460 = gTexture69E7A8;
    D_8018D464 = gTexture69EA18;
    D_8018D468 = gTexture69EC54;
    D_8018D46C = gTexture69EE38;
    D_8018D470 = gTexture69EFE0;
    gTextureLoadedLightningBolt0 = gTextureLightningBolt0;
    gTextureLoadedLightningBolt1 = gTextureLightningBolt1;
}

void reset_player_particle_pool(Player* player) {
    s32 i;

    for (i = 0; i < 10; ++i) {
        player->particlePool0[i].isAlive = 0;
        player->particlePool0[i].unk_01E = 0;
        player->particlePool0[i].type = 0;
    }

    for (i = 0; i < 10; ++i) {
        player->particlePool3[i].isAlive = 0;
        player->particlePool3[i].unk_01E = 0;
        player->particlePool3[i].type = 0;
    }

    for (i = 0; i < 10; ++i) {
        player->particlePool1[i].isAlive = 0;
        player->particlePool1[i].unk_01E = 0;
        player->particlePool1[i].type = 0;
    }

    for (i = 0; i < 10; ++i) {
        player->particlePool2[i].isAlive = 0;
        player->particlePool2[i].unk_01E = 0;
        player->particlePool2[i].type = 0;
    }
}

void set_particle_position_and_rotation(Player* player, UnkPlayerStruct258* arg1, f32 x, f32 y, f32 z, s8 surfaceType, s8 arg6) {
    arg1->pos[2] = z;
    arg1->pos[0] = x;
    arg1->pos[1] = y;
    arg1->unk_020 = -player->rotation[1];
    arg1->surfaceType = surfaceType;
    arg1->unk_010 = arg6;
}

s32 init_particle_player(UnkPlayerStruct258* arg0, s8 type, f32 arg2) {
    arg0->isAlive = true;
    arg0->type = type;
    arg0->unk_01E = 0;
    arg0->scale = arg2;
}

s32 set_particle_colour(UnkPlayerStruct258* arg0, s32 arg1, s16 arg2) {
    arg0->red = (u8) (arg1 >> 16);
    arg0->green = (u8) (arg1 >> 8);
    arg0->blue = (u8) arg1;
    arg0->alpha = arg2;
}

s32 set_particle_colour_randomly_varried(UnkPlayerStruct258* arg0, s32 arg1, s16 arg2) {
    s32 temp_v0;
    temp_v0 = random_int(0x30);

    arg0->red = (u8) ((u8) (arg1 >> 0x10) - temp_v0);
    arg0->green = (u8) ((u8) (arg1 >> 8) - temp_v0);
    arg0->blue = (u8) ((u8) arg1 - temp_v0);
    arg0->alpha = arg2;
}

void set_drift_particles(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_lo;

    if (player->unk_0C0 >= 0) {
        set_particle_position_and_rotation(player, &player->particlePool1[arg1], player->tyres[BACK_LEFT].pos[0],
                      player->tyres[BACK_LEFT].baseHeight + 2.0f, player->tyres[BACK_LEFT].pos[2],
                      player->tyres[BACK_LEFT].surfaceType, 1);
    } else {
        set_particle_position_and_rotation(player, &player->particlePool1[arg1], player->tyres[BACK_RIGHT].pos[0],
                      player->tyres[BACK_RIGHT].baseHeight + 2.0f, player->tyres[BACK_RIGHT].pos[2],
                      player->tyres[BACK_RIGHT].surfaceType, 0);
    }

    temp_lo = player->unk_0C0 / 182;
    if ((temp_lo >= 7) || (temp_lo < -6)) {
        init_particle_player(&player->particlePool1[arg1], DRIFT_PARTICLE, 0.35f);
        if (player->driftState == 0) {
            set_particle_colour(&player->particlePool1[arg1], 0xFFFFFF, 0x70);
        }

        if (player->driftState == 1) {
            set_particle_colour(&player->particlePool1[arg1], 0xFFFF00, 0x70);
        }

        if (player->driftState >= 2) {
            set_particle_colour(&player->particlePool1[arg1], 0xFF9600, 0x70);
        }

        if (player->driftState >= 2) {
            // Why not put this in previous if statement?
            player->particlePool1[arg1].unk_040 = 2;
            return;
        }

        player->particlePool1[arg1].unk_040 = player->driftState;
    }
}

void check_drift_particles_setup_valid(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) &&
        ((player->particlePool1[arg2].unk_01E >= 3) || (player->particlePool1[arg2].isAlive == 0))) {
        set_drift_particles(player, arg1, arg2, arg3, arg4);
    } else if (player->particlePool1[arg2].unk_01E >= 3) {
        set_drift_particles(player, arg1, arg2, arg3, arg4);
    }
}

UNUSED void func_8005DAD0(void) {
}

void func_8005DAD8(UnkPlayerStruct258* arg0, s16 arg1, s16 arg2, s16 arg3) {
    arg0->red = arg1;
    arg0->alpha = arg3;
    arg0->unk_040 = arg2;
}

void setup_tyre_particles(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 stackPadding;
    s32 surfaceType;
    s32 var_t3;
    f32 x;
    f32 z;
    f32 y;
    s32 randval;
    static s32 test = 8;

    surfaceType = 0x000000FF;
    randval = random_int(test);
    if ((randval == 0) || (randval == 4)) {
        x = player->tyres[BACK_LEFT].pos[0];
        z = player->tyres[BACK_LEFT].baseHeight + 2.0f;
        y = player->tyres[BACK_LEFT].pos[2];
        var_t3 = 1;
        surfaceType = player->tyres[BACK_LEFT].surfaceType;
    }
    if ((randval == 2) || (randval == 6)) {
        x = player->tyres[BACK_RIGHT].pos[0];
        z = player->tyres[BACK_RIGHT].baseHeight + 2.0f;
        y = player->tyres[BACK_RIGHT].pos[2];
        var_t3 = 0;
        surfaceType = player->tyres[BACK_RIGHT].surfaceType;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                        func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                    }
                    if (IsKalimariDesert()) {
                        func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                    }
                    if (IsMooMooFarm()) {
                        func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                    }
                    if (IsWarioStadium()) {
                        func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                    }
                    if (IsYoshiValley()) {
                        func_8005DAD8(&player->particlePool1[arg1], 10, 0, 0x0080);
                    }
                    if (IsDkJungle()) {
                        func_8005DAD8(&player->particlePool1[arg1], 11, 0, 0x0080);
                    }
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool1[arg1], 10, 0, 0x0080);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool1[arg1], 11, 0, 0x0080);
                }
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GRASS_PARTICLE, 1.0f);
                    set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                    player->particlePool1[arg1].red -= arg1 * 8;
                    player->particlePool1[arg1].green -= arg1 * 8;
                    player->particlePool1[arg1].blue -= arg1 * 8;
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GRASS_PARTICLE, 1.0f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                player->particlePool1[arg1].red -= arg1 * 8;
                player->particlePool1[arg1].green -= arg1 * 8;
                player->particlePool1[arg1].blue -= arg1 * 8;
            }
            player->particlePool1[arg1].pos[1] -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                set_particle_colour_randomly_varried(&player->particlePool1[arg1], 0x00FFA54F, 0x00AF);
                func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                if (((((player->speed / 18.0f) * 216.0f) >= 30.0f) &&
                     ((((player->unk_0C0 / 182) > 0x14) || ((player->unk_0C0 / 182) < (-0x14))))) ||
                    ((player->previousSpeed - player->speed) >= 0.04)) {
                    set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                    init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                    func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                    player->particlePool1[arg1].green = random_int(0x0010U);
                }
            } else if ((player->particlePool1[arg2].unk_01E > 0) &&
                       (((((player->speed / 18.0f) * 216.0f) >= 30.0f) &&
                         (((player->unk_0C0 / 182) >= 0x15) || ((player->unk_0C0 / 182) < -0x14))) ||
                        ((player->previousSpeed - player->speed) >= 0.04))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, z, y, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], GROUND_PARTICLE, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        default:
            break;
    }
}

void func_8005EA94(Player* player, s16 arg1, s32 arg2, s8 arg3, UNUSED s8 arg4) {
    s32 temp_v0;
    s32 surfaceType;
    s32 var_t1;
    f32 x;
    f32 y;
    f32 z;
    static s32 test = 10;

    surfaceType = 0x000000FF;
    temp_v0 = random_int(test);
    if ((temp_v0 == 0) || (temp_v0 == 8)) {
        if ((gPlayerWaterLevel[arg3] - player->tyres[BACK_LEFT].baseHeight) >= 3.5) {
            x = player->tyres[BACK_LEFT].pos[0];
            y = player->tyres[BACK_LEFT].baseHeight + 2.0f;
            z = player->tyres[BACK_LEFT].pos[2];
            var_t1 = 1;
            surfaceType = 0;
        }
    }
    if ((temp_v0 == 2) || (temp_v0 == 6)) {
        if ((gPlayerWaterLevel[arg3] - player->tyres[BACK_RIGHT].baseHeight) >= 3.5) {
            x = player->tyres[BACK_RIGHT].pos[0];
            y = player->tyres[BACK_RIGHT].baseHeight + 2.0f;
            z = player->tyres[BACK_RIGHT].pos[2];
            var_t1 = 0;
            surfaceType = 0;
        }
    }
    if (1) {}
    if (surfaceType == 0) {
        if ((arg1 == 0) &&
            ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
            if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 9, 0.8f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00AF);
            }
        } else if ((player->particlePool1[arg2].unk_01E > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
            set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
            init_particle_player(&player->particlePool1[arg1], 9, 0.8f);
            set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00AF);
        }
    }
}

void func_8005ED48(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_v0;
    s32 surfaceType;
    s32 var_t3;
    f32 x;
    f32 y;
    f32 z;
    static s32 test = 8;

    surfaceType = 0x000000FF;
    temp_v0 = random_int(test);
    if ((temp_v0 == 2) || (temp_v0 == 4)) {
        x = player->tyres[BACK_LEFT].pos[0];
        y = player->tyres[BACK_LEFT].baseHeight + 2.0f;
        z = player->tyres[BACK_LEFT].pos[2];
        var_t3 = 1;
        surfaceType = player->tyres[BACK_LEFT].surfaceType;
    }
    if ((temp_v0 == 0) || (temp_v0 == 6)) {
        x = player->tyres[BACK_RIGHT].pos[0];
        y = player->tyres[BACK_RIGHT].baseHeight + 2.0f;
        z = player->tyres[BACK_RIGHT].pos[2];
        var_t3 = 0;
        surfaceType = player->tyres[BACK_RIGHT].surfaceType;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool1[arg1], 10, 0, 0x0080);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool1[arg1], 11, 0, 0x0080);
                }
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000A, 0, 0x0080);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000B, 0, 0x0080);
                }
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.1f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                player->particlePool1[arg1].red -= arg1 * 8;
                player->particlePool1[arg1].green -= arg1 * 8;
                player->particlePool1[arg1].blue -= arg1 * 8;
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.1f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                player->particlePool1[arg1].red -= arg1 * 8;
                player->particlePool1[arg1].green -= arg1 * 8;
                player->particlePool1[arg1].blue -= arg1 * 8;
            }
            player->particlePool1[arg1].pos[1] -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                init_particle_player(&player->particlePool1[arg1], 5, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        default:
            break;
    }
}

#ifdef NON_MATCHING
// Its close, there's some register allocation issue though
// Permuter hasn't found anything
// https://decomp.me/scratch/WjMqd

void func_8005F90C(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_t1;
    u8 surfaceType;
    f32 x;
    f32 y;
    f32 z;

    var_t1 = 0;
    if ((player->effects & 0x80) == 0x80) {
        x = player->pos[0];
        y = player->pos[1] - player->boundingBoxSize;
        z = player->pos[2];
        var_t1 = 1;
        surfaceType = player->tyres[BACK_LEFT].surfaceType;
    } else {
        x = player->pos[0];
        y = player->pos[1] - player->boundingBoxSize;
        z = player->pos[2];
        surfaceType = player->tyres[BACK_RIGHT].surfaceType;
    }

    //! @todo This likely needs to be implemented for custom courses
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000A, 0, 0x0080);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000B, 0, 0x0080);
                }
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool1[arg1], 1, 0, 0x0080);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool1[arg1], 7, 0, 0x0080);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool1[arg1], 8, 0, 0x0080);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool1[arg1], 9, 0, 0x0080);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000A, 0, 0x0080);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool1[arg1], 0x000B, 0, 0x0080);
                }
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.1f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                player->particlePool1[arg1].red -= arg1 * 8;
                player->particlePool1[arg1].green -= arg1 * 8;
                player->particlePool1[arg1].blue -= arg1 * 8;
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.1f);
                set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
                player->particlePool1[arg1].red -= arg1 * 8;
                player->particlePool1[arg1].green -= arg1 * 8;
                player->particlePool1[arg1].blue -= arg1 * 8;
            }
            player->particlePool1[arg1].pos[1] -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 2, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 3, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 4, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 5, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 6, 1, 0x00A8);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                player->particlePool1[arg1].green = random_int(0x0010U);
            } else if (player->particlePool1[arg2].unk_01E > 0) {
                set_particle_position_and_rotation(player, &player->particlePool1[arg1], x, y, z, surfaceType, var_t1);
                init_particle_player(&player->particlePool1[arg1], 4, 0.46f);
                func_8005DAD8(&player->particlePool1[arg1], 0, 0, 0x0080);
                player->particlePool1[arg1].green = random_int(0x0010U);
            }
            break;
        default:
            break;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80057C60/func_8005F90C.s")
#endif

void func_80060504(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 thing1;
    s16 thing2;
    UNUSED s32 thing3;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 var_f0;
    s32 var_v0;
    s32 temp_v0;
    UNUSED s32 test;

    if ((player->unk_044 & 0x20) == 0x20) {
        var_v0 = 5;
    } else {
        var_v0 = 0xE;
    }
    temp_v0 = random_int(var_v0);
    if ((temp_v0 == 1) || (temp_v0 == 2) || (temp_v0 == 3)) {
        if ((arg1 == 0) &&
            ((player->particlePool0[arg2].unk_01E > 0) || (player->particlePool0[arg2].isAlive == 0))) {
            sp4C = player->pos[1] - 2.5;
            sp48 = player->pos[2];
            sp50 = player->pos[0];
            set_particle_position_and_rotation(player, &player->particlePool0[arg1], sp50, sp4C, sp48, 0, 0);
            init_particle_player(&player->particlePool0[arg1], 1, 0.5f);
        } else if (player->particlePool0[arg2].unk_01E > 0) {
            sp4C = player->pos[1] - 2.5;
            sp48 = player->pos[2];
            sp50 = player->pos[0];
            set_particle_position_and_rotation(player, &player->particlePool0[arg1], sp50, sp4C, sp48, 0, 0);
            init_particle_player(&player->particlePool0[arg1], 1, 0.5f);
        }
    }
    player->particlePool0[arg1].unk_024 = 0.0f;
    if ((player->unk_044 & 0x20) == 0x20) {
        player->particlePool0[arg1].unk_040 = 0;
        if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
            set_particle_colour(&player->particlePool0[arg1], 0x00FFFF00, 0x0080);
            player->particlePool0[arg1].red = 1;
        } else {
            set_particle_colour(&player->particlePool0[arg1], 0x00FFFFFF, 0x0070);
            player->particlePool0[arg1].red = 0;
        }
    } else {
        player->particlePool0[arg1].unk_040 = 1;
        if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
            set_particle_colour(&player->particlePool0[arg1], 0x00FFFF00, 0x0080);
            player->particlePool0[arg1].red = 1;
        } else {
            set_particle_colour(&player->particlePool0[arg1], 0x00FFFFFF, 0x0070);
            player->particlePool0[arg1].red = 0;
        }
    }
    thing2 = (player->particlePool0[arg1].unk_020 - (player->unk_0C0 / 2));
    if (player->particlePool0[arg1].unk_040 == 0) {
        var_f0 = -((player->unk_098 / 3000.0f) + 0.1);
    } else {
        var_f0 = -((player->unk_098 / 5000.0f) + 0.1);
    }
    func_80062B18(&sp50, &sp4C, &sp48, 0.0f, 4.5f, (player->particlePool0[arg1].unk_01E * var_f0) + -5.5, -thing2,
                  -player->unk_206 * 2);
    player->particlePool0[arg1].pos[0] = player->pos[0] + sp50;
    sp4C = sp4C + (player->pos[1] - player->boundingBoxSize);
    player->particlePool0[arg1].pos[2] = player->pos[2] + sp48;
    player->particlePool0[arg1].pos[1] = player->particlePool0[arg1].unk_024 + sp4C;
    player->particlePool0[arg1].unk_010 = 0;
}

void func_800608E0(Player* player, s16 arg1, UNUSED s32 arg2, s8 arg3, UNUSED s8 arg4) {
    f32 var_f0;
    f32 sp50;
    f32 sp4C;
    f32 sp48;

    var_f0 = 8.0f - (gPlayerWaterLevel[arg3] - player->pos[1]);
    if ((f64) var_f0 <= 0.0) {
        var_f0 = 0.0f;
    }
    sp4C = (gPlayerWaterLevel[arg3] - player->pos[1]) - 3.0f;
    if ((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) && (!IsKoopaTroopaBeach())) {
        var_f0 = 2.5f;
        sp4C = (f32) ((f64) (gPlayerWaterLevel[arg3] - player->pos[1]) + 0.1);
    }
    set_particle_position_and_rotation(player, &player->particlePool0[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    init_particle_player(&player->particlePool0[arg1], 3, var_f0);
    if ((IsBowsersCastle()) || (IsBigDonut())) {
        set_particle_colour(&player->particlePool0[arg1], 0, 0x00AF);
    } else {
        set_particle_colour(&player->particlePool0[arg1], 0x00FFFFFF, 0x00CF);
    }
    func_80062B18(&sp50, &sp4C, &sp48, 0.0f, sp4C,
                  ((-player->particlePool0[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 10.0f) + -4.0f,
                  -player->particlePool0[arg1].unk_020, -player->unk_206 * 2);
    player->particlePool0[arg1].pos[0] = player->pos[0] + sp50;
    player->particlePool0[arg1].pos[2] = player->pos[2] + sp48;
    player->particlePool0[arg1].pos[1] = player->pos[1] + sp4C;
}

void func_80060B14(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((!IsSkyscraper()) && (!IsRainbowRoad())) {
        if ((arg1 == 0) &&
            ((player->particlePool0[arg2].unk_01E > 0) || (player->particlePool0[arg2].isAlive == 0))) {
            func_800608E0(player, arg1, arg2, arg3, arg4);
        } else if (player->particlePool0[arg2].unk_01E > 0) {
            func_800608E0(player, arg1, arg2, arg3, arg4);
        }
    }
}

void func_80060BCC(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 pad;
    s32 sp4C;
    f32 sp48;
    f32 sp44;

    if (IsSkyscraper()) {
        return;
    }
    if (IsRainbowRoad()) {
        return;
    }
    sp54 = random_int(0x0168U) - 0xB4;
    sp4C = random_int(6U);
    sp44 = random_int(6U);
    sp48 = random_int(3U);
    if (player != gPlayerOne) {
        return;
    }
    if ((arg1 == 0) &&
        ((player->particlePool1[arg2].unk_01E > 0) || (player->particlePool1[arg2].isAlive == 0))) {
        set_particle_position_and_rotation(player, &player->particlePool1[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        init_particle_player(&player->particlePool1[arg1], 0x0B, 0.4f);
        set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
        player->particlePool1[arg1].pos[2] = player->pos[2] + (coss(sp54 * 0xB6) * -1.8);
        player->particlePool1[arg1].pos[0] = player->pos[0] + (sins(sp54 * 0xB6) * -1.8);
        player->particlePool1[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + sp4C + 2.0f;
        player->particlePool1[arg1].unk_018 = sp44 + 1.0f;
        player->particlePool1[arg1].scale = (sp48 + 2.0f) / 10.0f;
    } else if (player->particlePool1[arg2].unk_01E > 0) {
        set_particle_position_and_rotation(player, &player->particlePool1[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        init_particle_player(&player->particlePool1[arg1], 0x0B, 0.4f);
        set_particle_colour(&player->particlePool1[arg1], 0x00FFFFFF, 0x00FF);
        player->particlePool1[arg1].pos[2] = player->pos[2] + (coss(sp54 * 0xB6) * -1.8);
        player->particlePool1[arg1].pos[0] = player->pos[0] + (sins(sp54 * 0xB6) * -1.8);
        player->particlePool1[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + sp4C + 2.0f;
        player->particlePool1[arg1].unk_018 = sp44 + 1.0f;
        player->particlePool1[arg1].scale = (sp48 + 2.0f) / 10.0f;
    }
}

void func_80060F50(Player* player, s16 arg1, UNUSED s32 arg2, s8 arg3, UNUSED s8 arg4) {
    set_particle_position_and_rotation(player, &player->particlePool0[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    init_particle_player(&player->particlePool0[arg1], 5, 4.0f);

    if ((IsBowsersCastle()) || (IsBigDonut())) {
        set_particle_colour(&player->particlePool0[arg1], 0xFF0000, 0xFF);
    } else {
        set_particle_colour(&player->particlePool0[arg1], 0xFFFFFF, 0xFF);
    }

    player->particlePool0[arg1].pos[2] =
        player->pos[2] + (coss(player->particlePool0[arg1].unk_020) * -5.8);
    player->particlePool0[arg1].pos[0] =
        player->pos[0] + (sins(player->particlePool0[arg1].unk_020) * -5.8);
    player->particlePool0[arg1].pos[1] = gPlayerWaterLevel[arg3];
    player->waterInteractionFlags &= ~WATER_JUST_ENTERED_DEEP_LIQUID;
}

void func_80061094(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    if (arg1 == 0) {
        set_particle_position_and_rotation(player, &player->particlePool0[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
        init_particle_player(&player->particlePool0[arg1], 6, 3.8f);
        set_particle_colour(&player->particlePool0[arg1], 0xFFFFFF, 0xFF);
        player->particlePool0[arg1].red = 0;
        player->particlePool0[arg1].green = 0;
        player->particlePool0[arg1].blue = 0;
    }
}

void func_80061130(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    set_particle_position_and_rotation(player, &player->particlePool0[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    init_particle_player(&player->particlePool0[arg1], 7, 0.6f);
    set_particle_colour(&player->particlePool0[arg1], 0xFFFFFF, 0xD0);

    player->particlePool0[arg1].pos[2] =
        player->pos[2] + (coss(player->particlePool0[arg1].unk_020) * 6.0f);
    player->particlePool0[arg1].pos[0] =
        player->pos[0] + (sins(player->particlePool0[arg1].unk_020) * 6.0f);
    player->particlePool0[arg1].pos[1] = player->pos[1] - 5.0f;
    player->particlePool0[arg1].unk_040 = 0;
    player->particlePool0[arg1].unk_024 = 0.0f;
}

void func_80061224(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) &&
        ((player->particlePool0[arg2].unk_01E > 0) || (player->particlePool0[arg1].isAlive == 0))) {
        func_80061130(player, arg1, arg2, arg3, arg4);
    } else if (player->particlePool0[arg2].unk_01E >= 2) {
        func_80061130(player, arg1, arg2, arg3, arg4);
        if (arg1 == 9) {
            player->unk_044 &= ~0x0200;
        }
    }
}

void func_800612F8(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        player->particlePool3[var_s2].isAlive = 1;
        player->particlePool3[var_s2].unk_028 = player->pos[1] + 5.0f;
        player->particlePool3[var_s2].unk_020 = (0x1C70 * var_s2) - player->rotation[1];
        player->particlePool3[var_s2].unk_024 = (random_int(0x0064U) / 100.0f) + 1.5;
        player->particlePool3[var_s2].green = 0;
        player->particlePool3[var_s2].type = 1;
        player->particlePool3[var_s2].unk_01E = 0;
        player->particlePool3[var_s2].alpha = 0x00FF;
        player->particlePool3[var_s2].pos[2] = player->pos[2];
        player->particlePool3[var_s2].pos[0] = player->pos[0];
    }
    player->unk_046 &= ~0x0008;
}

void func_80061430(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 7; var_s2++) {
        player->particlePool3[var_s2].isAlive = 1;
        player->particlePool3[var_s2].unk_028 = player->pos[1] - 4.0f;
        player->particlePool3[var_s2].unk_020 = (0x1C70 * var_s2) - player->rotation[1];
        // ???
        player->particlePool3[var_s2].unk_024 = (random_int(0x0064U) / 100.0f) + 1.9;
        player->particlePool3[var_s2].unk_024 = (random_int(0x0064U) / 100.0f) + 1.5;
        player->particlePool3[var_s2].green = 0;
        player->particlePool3[var_s2].type = 9;
        player->particlePool3[var_s2].unk_01E = 0;
        player->particlePool3[var_s2].alpha = 0x00FF;
        player->particlePool3[var_s2].pos[2] = player->pos[2];
        player->particlePool3[var_s2].pos[0] = player->pos[0];
    }
    player->unk_044 &= ~0x1000;
}

void func_800615AC(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    f32 temp_f0;
    f32 sp28[10] = { -182.0f, 182.0f, 364.0f, -364.0f, 546.0f, -546.0f, 728.0f, -728.0f, 910.0f, -910.0f };

    if (random_int(3U) == 2.0f) {
        player->particlePool3[arg1].isAlive = 1;
        player->particlePool3[arg1].pos[0] = player->pos[0];
        player->particlePool3[arg1].pos[2] = player->pos[2];
        player->particlePool3[arg1].unk_020 = -player->rotation[1] + sp28[arg1];
        player->particlePool3[arg1].unk_018 = random_int(1U) + 2.0f;
        temp_f0 = random_int(4U);
        temp_f0 -= test;
        player->particlePool3[arg1].surfaceType = temp_f0;
        player->particlePool3[arg1].pos[1] = player->pos[1] + temp_f0;
        player->particlePool3[arg1].scale = 0.15f;
        player->particlePool3[arg1].type = 5;
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].alpha = 0x00FF;
        player->particlePool3[arg1].red = 0;
    }
}

void func_80061754(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    s32 sp54;
    s16 temp_s1;
    s32 sp4C;
    f32 sp48;
    f32 sp44;

    sp54 = random_int(0x0168U) - 0xB4;
    sp4C = random_int(6U);
    temp_s1 = random_int(0x0060U);
    sp44 = random_int(6U);
    sp48 = random_int(2U);
    set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    init_particle_player(&player->particlePool3[arg1], 6, 1.0f);
    if ((player->effects & HIT_BY_ITEM_EFFECT) || ((player->effects) & 0x01000000) || ((player->effects) & 0x400) ||
        ((player->effects) & BOO_EFFECT)) {
        set_particle_colour(&player->particlePool3[arg1], 0x00FFFFFF, 0x00A0);
        player->particlePool3[arg1].red -= temp_s1;
        player->particlePool3[arg1].green -= temp_s1;
        player->particlePool3[arg1].blue -= temp_s1;
    } else {
        set_particle_colour(&player->particlePool3[arg1], 0, 0x00A0);
        player->particlePool3[arg1].red += temp_s1;
        player->particlePool3[arg1].green += temp_s1;
        player->particlePool3[arg1].blue += temp_s1;
    }
    player->particlePool3[arg1].pos[2] = player->pos[2] + (coss(sp54 * 0xB6) * -5.0f);
    player->particlePool3[arg1].pos[0] = player->pos[0] + (sins(sp54 * 0xB6) * -5.0f);
    player->particlePool3[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + sp4C + 2.0f;
    player->particlePool3[arg1].unk_018 = sp44 + 1.0f;
    player->particlePool3[arg1].scale = sp48 + 1.0f;
}

void func_8006199C(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) &&
        ((player->particlePool3[arg2].unk_01E > 0) || (player->particlePool3[arg2].isAlive == 0))) {
        func_80061754(player, arg1, arg2, (s32) arg3, arg4);
    } else if (player->particlePool3[arg2].unk_01E > 0) {
        func_80061754(player, arg1, arg2, (s32) arg3, arg4);
    }
}

void func_80061A34(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 stackPadding0;
    s32 sp4C;
    f32 sp48;
    UNUSED s32 stackPadding1;

    sp54 = random_int(0x0168U) - 0xB4;
    sp4C = random_int(6U);
    random_int(6U);
    sp48 = (f32) random_int(3U);
    if ((arg1 == 0) &&
        ((player->particlePool3[arg2].unk_01E > 0) || (player->particlePool3[arg2].isAlive == 0))) {
        set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        init_particle_player(&player->particlePool3[arg1], 7, 1.0f);
        set_particle_colour(&player->particlePool3[arg1], 0x00FFFFFF, 0x00FF);
        player->particlePool3[arg1].pos[2] = player->pos[2] + (coss(sp54 * 0xB6) * -2.0);
        player->particlePool3[arg1].pos[0] = player->pos[0] + (sins(sp54 * 0xB6) * -2.0);
        player->particlePool3[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + sp4C + 2.0f;
        player->particlePool3[arg1].scale = (sp48 + 2.0f) / 10.0f;
    } else if (player->particlePool3[arg2].unk_01E > 0) {
        set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        init_particle_player(&player->particlePool3[arg1], 7, 1.0f);
        set_particle_colour(&player->particlePool3[arg1], 0x00FFFFFF, 0x00FF);
        player->particlePool3[arg1].pos[2] = player->pos[2] + (coss(sp54 * 0xB6) * -2.0);
        player->particlePool3[arg1].pos[0] = player->pos[0] + (sins(sp54 * 0xB6) * -2.0);
        player->particlePool3[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + (f32) sp4C + 2.0f;
        player->particlePool3[arg1].scale = (sp48 + 2.0f) / 10.0f;
    }
}

void func_80061D4C(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    f32 sp20[10] = { -182.0f, 182.0f, 364.0f, -364.0f, 546.0f, -546.0f, 728.0f, -728.0f, 910.0f, -910.0f };

    if (random_int(3U) == 2.0f) {
        player->particlePool3[arg1].isAlive = 1;
        player->particlePool3[arg1].pos[0] = player->pos[0];
        player->particlePool3[arg1].pos[1] = player->pos[1] + 2.0f;
        player->particlePool3[arg1].pos[2] = player->pos[2];
        player->particlePool3[arg1].unk_020 = -player->rotation[1] + sp20[arg1];
        player->particlePool3[arg1].unk_018 = random_int(3U) + 2.0f;
        player->particlePool3[arg1].surfaceType = random_int(4U);
        player->particlePool3[arg1].surfaceType -= test;
        player->particlePool3[arg1].scale = 0.4f;
        player->particlePool3[arg1].type = 2;
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].alpha = 0x00FF;
    }
}

void func_80061EF4(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 stackPadding0;
    s32 var_t0 = 0x000000FF;
    s32 var_t1;
    s32 temp_v1;
    f32 var_f2;
    UNUSED s32 stackPadding1;

    if (1) {};
    temp_v1 = random_int(8U) & 1;
    if (temp_v1 == 1) {
        var_t1 = 1;
        var_t0 = 0;
        var_f2 = player->pos[1];
    }
    if (temp_v1 == 0) {
        var_t1 = 0;
        var_t0 = 0;
        var_f2 = player->pos[1];
    }
    if (var_t0 == 0) {
        if ((arg1 == 0) &&
            ((player->particlePool3[arg2].unk_01E > 0) || (player->particlePool3[arg2].isAlive == 0))) {
            set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, var_f2, 0.0f, (s8) var_t0, (s8) var_t1);
            init_particle_player(&player->particlePool3[arg1], 3, 0.5f);
            set_particle_colour(&player->particlePool3[arg1], 0x00FFFFFF, 0x0060);
            player->particlePool3[arg1].unk_020 = 0;
            if (player->particlePool3[arg1].unk_010 == 1) {
                player->particlePool3[arg1].unk_020 += 0x888;
            } else {
                player->particlePool3[arg1].unk_020 -= 0x888;
            }
            player->particlePool3[arg1].pos[2] =
                player->pos[2] +
                (coss(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
            player->particlePool3[arg1].pos[0] =
                player->pos[0] +
                (sins(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
        } else if (player->particlePool3[arg2].unk_01E > 0) {
            set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, var_f2, 0.0f, (s8) var_t0, (s8) var_t1);
            init_particle_player(&player->particlePool3[arg1], 3, 0.5f);
            set_particle_colour(&player->particlePool3[arg1], 0x00FFFFFF, 0x0060);
            player->particlePool3[arg1].unk_020 = 0;
            if (player->particlePool3[arg1].unk_010 == 1) {
                player->particlePool3[arg1].unk_020 += 0x888;
            } else {
                player->particlePool3[arg1].unk_020 -= 0x888;
            }
            player->particlePool3[arg1].pos[2] =
                player->pos[2] +
                (coss(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
            player->particlePool3[arg1].pos[0] =
                player->pos[0] +
                (sins(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
        }
    }
}

void func_800621BC(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_v1;
    s32 phi_t0;
    s32 phi_t1;
    Player* new_var;
    f32 phi_f2;
    Player* new_var2;

    phi_t0 = 0xFF;
    temp_v1 = random_int(8) & 1;
    if (temp_v1 == 1) {
        phi_t1 = 1;
        phi_t0 = 0;
        phi_f2 = player->pos[1];
    }

    if (temp_v1 == 0) {
        phi_t1 = 0;
        if (1) {
            phi_t0 = 0;
        }
        phi_f2 = player->pos[1];
    }

    if (phi_t0 == 0) {
        if ((arg1 == 0) &&
            ((player->particlePool3[arg2].unk_01E > 0) || (player->particlePool3[arg2].isAlive == 0))) {
            set_particle_position_and_rotation(player, &player->particlePool3[arg1], 0.0f, phi_f2, 0.0f, phi_t0, phi_t1);
            init_particle_player(&player->particlePool3[arg1], 8, 1.0f);
            set_particle_colour(&player->particlePool3[arg1], 0xFFFF20, 0xFF);

            player->particlePool3[arg1].unk_020 = 0;
            if (player->particlePool3[arg1].unk_010 == 1) {
                player->particlePool3[arg1].unk_020 += 2184;
            } else {
                player->particlePool3[arg1].unk_020 -= 2184;
            }

            player->particlePool3[arg1].pos[2] =
                player->pos[2] +
                (coss((player->particlePool3[arg1].unk_020 - player->rotation[1]) - player->unk_0C0) * 5.0f);
            player->particlePool3[arg1].pos[0] =
                player->pos[0] +
                (sins((player->particlePool3[arg1].unk_020 - player->rotation[1]) - player->unk_0C0) * 5.0f);
            return;
        }

        new_var2 = player;
        if (new_var2->particlePool3[arg2].unk_01E > 0) {
            set_particle_position_and_rotation(new_var2, &new_var2->particlePool3[arg1], 0.0f, phi_f2, 0.0f, phi_t0, phi_t1);
            init_particle_player(&new_var2->particlePool3[arg1], 8, 1.0f);
            set_particle_colour(&new_var2->particlePool3[arg1], 0xFFFF20, 0xFF);
            new_var2->particlePool3[arg1].unk_020 = 0;
            if (new_var2->particlePool3[arg1].unk_010 == 1) {
                new_var2->particlePool3[arg1].unk_020 += 2184;
            } else {
                new_var2->particlePool3[arg1].unk_020 -= 2184;
            }

            new_var = new_var2;
            new_var->particlePool3[arg1].pos[2] =
                new_var->pos[2] +
                (coss((new_var->particlePool3[arg1].unk_020 - new_var->rotation[1]) - new_var->unk_0C0) * 5.0f);
            new_var->particlePool3[arg1].pos[0] =
                new_var->pos[0] +
                (sins((new_var->particlePool3[arg1].unk_020 - new_var->rotation[1]) - new_var->unk_0C0) * 5.0f);
        }
    }
}

void func_80062484(Player* player, UnkPlayerStruct258* arg1, s32 arg2) {
    arg1->isAlive = 1;
    arg1->pos[1] = player->unk_074 + 1.0f;
    arg1->pos[2] = player->pos[2];
    arg1->pos[0] = player->pos[0];
    arg1->unk_020 = (arg2 * 0x1998) - player->rotation[1];
    arg1->type = 4;
    arg1->unk_01E = 0;
}

void func_800624D8(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s1;

    switch (player->surfaceType) {
        case DIRT:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                if ((IsChocoMountain()) || (IsRoyalRaceway())) {
                    func_8005DAD8(&player->particlePool3[var_s1], 1, 0, 0x00A8);
                }
                if (IsKalimariDesert()) {
                    func_8005DAD8(&player->particlePool3[var_s1], 7, 0, 0x00A8);
                }
                if (IsMooMooFarm()) {
                    func_8005DAD8(&player->particlePool3[var_s1], 8, 0, 0x00A8);
                }
                if (IsWarioStadium()) {
                    func_8005DAD8(&player->particlePool3[var_s1], 9, 0, 0x00A8);
                }
                if (IsYoshiValley()) {
                    func_8005DAD8(&player->particlePool3[var_s1], 0x000A, 0, 0x00A8);
                }
                if (IsDkJungle()) {
                    func_8005DAD8(&player->particlePool3[var_s1], 0x000B, 0, 0x00A8);
                }
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case GRASS:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 2, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case SAND_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 2, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 3, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case WET_SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 4, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case DIRT_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 5, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case SNOW:
        case SNOW_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 6, 1, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 0, 0, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
        default:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                func_8005DAD8(&player->particlePool3[var_s1], 0, 0, 0x00A8);
                func_80062484(player, &player->particlePool3[var_s1], var_s1);
            }
            player->unk_044 &= ~0x0100;
            break;
    }
}

void func_800628C0(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].unk_020 = -player->rotation[1];
    player->particlePool2[index].type = 2;
    player->particlePool2[index].unk_01E = 0;
    player->particlePool2[index].scale = 0.2f;
}

void func_80062914(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].unk_020 = -player->rotation[1];
    player->particlePool2[index].type = 4;
    player->particlePool2[index].unk_01E = 0;
    player->particlePool2[index].scale = 1.0f;
}

void func_80062968(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].unk_020 = -player->rotation[1];
    player->particlePool2[index].type = 5;
    player->particlePool2[index].unk_01E = 0;
    player->particlePool2[index].scale = 0.2f;
}

void func_800629BC(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].unk_020 = -player->rotation[1];
    player->particlePool2[index].type = 6;
    player->particlePool2[index].unk_01E = 0;
    player->particlePool2[index].scale = 0.2f;
    player->particlePool2[index].pos[1] = 0.0f;
}

void func_80062A18(Player* player, s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].type = 3;
    player->particlePool2[arg1 /* arg1 instead of arg3 */].scale = 0.2f;
    player->particlePool2[index].unk_01E = 1;
    player->particlePool2[index].unk_020 = 0;
    player->unk_0B6 &= ~0x0080;
    player->particlePool2[index].pos[2] = player->pos[2];
    player->particlePool2[index].pos[0] = player->pos[0];
    player->particlePool2[index].pos[1] = (player->pos[1] + 4.0f);
}

void func_80062AA8(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].isAlive = 1;
    player->particlePool2[index].type = 5;
    player->particlePool2[index].scale = 0.1f;
    player->particlePool2[index].unk_01E = 0;
    player->particlePool2[index].pos[1] = (player->pos[1] + player->boundingBoxSize) - 2.5;
}

void func_80062B18(f32* arg0, f32* arg1, f32* arg2, f32 arg3, f32 arg4, f32 arg5, u16 arg6, u16 arg7) {
    UNUSED f32 pad;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 temp_f20;

    //  Apply the matrix multiplication:
    //  Matrix is the Jacobian for cartesian to spherical coordinates?
    //  Get the change in r, theta, and phi for a change in x, y, z?
    //
    // | arg0 |     | cos(arg6)*cos(arg7)  cos(arg6)*sin(arg7)   -sin(arg6) |     | arg3 |
    // |      |     |                                                       |     |      |
    // | arg1 |  =  | -sin(arg7)           cos(arg7)                 0      |  X  | arg4 |
    // |      |     |                                                       |     |      |
    // | arg2 |     | sin(arg6)*cos(arg7)  sin(arg6)*sin(arg7)    cos(arg6) |     | arg5 |
    //
    sp28 = sins(arg7);
    sp2C = coss(arg6);
    sp30 = coss(arg7);
    temp_f20 = coss(arg6);
    //    = arg3 * cos(arg6) * cos(arg7) + arg4 * cos(arg6) * sin(arg7) - arg5 * sin(arg6)
    *arg0 = (((arg3 * temp_f20) * sp30) + (arg4 * sp2C) * sp28) - (sins(arg6) * arg5);

    temp_f20 = sins(arg7);
    //    = -arg3 * sin(arg7) + arg4 * cos(arg7)
    *arg1 = (coss(arg7) * arg4) - (arg3 * temp_f20);

    sp28 = sins(arg7);
    sp2C = sins(arg6);
    sp30 = coss(arg7);
    temp_f20 = sins(arg6);
    //    = arg3 * sin(arg6) * cos(arg7) + arg4 * sin(arg6) * sin(arg7) + arg5 * cos(arg6)
    *arg2 = (coss(arg6) * arg5) + (((arg3 * temp_f20) * sp30) + ((arg4 * sp2C) * sp28));
}

void func_80062C74(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
    f32 sp48[8] = { 4.5f, 4.5f, 4.5f, 4.5f, 4.5f, 5.5f, 4.5f, 6.5f };
    f32 var_f6;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s16 thing;

    player->particlePool0[arg1].unk_01E += 1;
    if (player->particlePool0[arg1].unk_01E == 0x000C) {
        player->particlePool0[arg1].isAlive = 0;
        player->particlePool0[arg1].unk_01E = 0;
        player->particlePool0[arg1].type = 0;
    }
    player->particlePool0[arg1].unk_018 = 2.0f;
    if (player->particlePool0[arg1].unk_040 == 0) {
        player->particlePool0[arg1].scale = player->particlePool0[arg1].scale + 0.07;
        player->particlePool0[arg1].unk_024 = player->particlePool0[arg1].unk_024 + 0.3;
        if (player->particlePool0[arg1].unk_01E >= 3) {
            player->particlePool0[arg1].alpha -= 3;
        }
        if (player->particlePool0[arg1].alpha <= 0) {
            player->particlePool0[arg1].alpha = 0;
        }
    } else {
        player->particlePool0[arg1].scale = player->particlePool0[arg1].scale + 0.1;
        player->particlePool0[arg1].unk_024 = player->particlePool0[arg1].unk_024 + 0.3;
        if (player->particlePool0[arg1].unk_01E >= 3) {
            player->particlePool0[arg1].alpha -= 2;
        }
        if (player->particlePool0[arg1].alpha <= 0) {
            player->particlePool0[arg1].alpha = 0;
        }
    }
    thing = player->particlePool0[arg1].unk_020 - (player->unk_0C0 / 2);
    if (player->particlePool0[arg1].unk_040 == 0) {
        var_f6 = -((player->unk_098 / 5000.0f) + 0.1);
    } else {
        var_f6 = -((player->unk_098 / 6000.0f) + 0.1);
    }
    if (((player->effects & BOOST_EFFECT) == BOOST_EFFECT) && (player->particlePool0[arg1].unk_01E >= 6)) {
        player->particlePool0[arg1].scale = player->particlePool0[arg1].scale + 0.06;
    }
    player->particlePool0[arg1].unk_010++;
    if (player->particlePool0[arg1].unk_010 >= 3) {
        player->particlePool0[arg1].unk_010 = 0;
    }
    func_80062B18(&sp40, &sp38, &sp3C, 0.0f, sp48[player->characterId],
                  (player->particlePool0[arg1].unk_01E * var_f6) + -5.5, -thing, -player->unk_206 * 2);
    player->particlePool0[arg1].pos[0] = player->pos[0] + sp40;
    sp38 = (player->pos[1] - player->boundingBoxSize) + sp38;
    player->particlePool0[arg1].pos[2] = player->pos[2] + sp3C;
    player->particlePool0[arg1].pos[1] = player->particlePool0[arg1].unk_024 + sp38;
}

void func_80062F98(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    f32 temp_f0;

    temp_f0 = player->particlePool1[arg1].unk_018 / 10.0f;
    ++player->particlePool1[arg1].unk_01E;
    player->particlePool1[arg1].pos[1] += temp_f0;
    if ((player->unk_0CA & 1) == 1) {
        player->particlePool1[arg1].pos[1] += (temp_f0 + 0.3);
        if ((player->particlePool1[arg1].unk_01E == 0x10) ||
            ((gPlayerWaterLevel[arg2] - player->particlePool1[arg1].pos[1]) < 3.0f)) {
            player->particlePool1[arg1].isAlive = 0;
            player->particlePool1[arg1].unk_01E = 0;
            player->particlePool1[arg1].type = 0;
        }
    } else if ((player->particlePool1[arg1].unk_01E == 0xA) ||
               ((gPlayerWaterLevel[arg2] - player->particlePool1[arg1].pos[1]) < 3.0f)) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }
}

void set_oob_splash_particle_position(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    ++player->particlePool0[arg1].unk_01E;
    player->particlePool0[arg1].pos[2] = player->pos[2] + coss(player->particlePool0[arg1].unk_020) * -5.8;
    player->particlePool0[arg1].pos[0] = player->pos[0] + sins(player->particlePool0[arg1].unk_020) * -5.8;
    player->particlePool0[arg1].pos[1] = gPlayerWaterLevel[arg2];
    if (player->particlePool0[arg1].unk_01E == 15) {
        player->particlePool0[arg1].isAlive = 0;
        player->particlePool0[arg1].unk_01E = 0;
        player->particlePool0[arg1].type = 0;
    }
}

void func_800631A8(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->particlePool0[arg1].unk_01E;
    if ((s32) player->particlePool0[arg1].unk_01E < 9) {
        if ((player->particlePool0[arg1].unk_01E & 1) != 0) {
            player->particlePool0[arg1].red = 8;
        } else {
            player->particlePool0[arg1].red = 0;
        }
    } else if (((player->particlePool0[arg1].unk_01E & 1) != 0) ||
               ((player->particlePool0[arg1].unk_01E >= 9) && (player->particlePool0[arg1].unk_01E < 12))) {
        player->particlePool0[arg1].red = 0xFF;
    } else if ((player->particlePool0[arg1].unk_01E & 2) != 0) {
        player->particlePool0[arg1].red = 8;
    } else {
        player->particlePool0[arg1].red = 0;
    }
    player->particlePool0[arg1].green = 0;
    player->particlePool0[arg1].blue = 0;
    if ((s32) player->particlePool0[arg1].unk_01E >= 0x19) {
        player->particlePool0[arg1].isAlive = 0;
        player->particlePool0[arg1].unk_01E = 0;
        player->particlePool0[arg1].type = 0;
    }
}

void func_80063268(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->particlePool0[arg1].unk_01E >= 0x1E) {
        player->particlePool0[arg1].unk_040 += 0x1FFE;
    } else {
        player->particlePool0[arg1].unk_040 += 0x1554;
    }

    player->particlePool0[arg1].unk_024 += 0.25;
    player->particlePool0[arg1].pos[2] =
        player->pos[2] +
        (coss((player->particlePool0[arg1].unk_020 + player->particlePool0[arg1].unk_040)) * 5.5);
    player->particlePool0[arg1].pos[0] =
        player->pos[0] +
        (sins((player->particlePool0[arg1].unk_020 + player->particlePool0[arg1].unk_040)) * 5.5);
    player->particlePool0[arg1].pos[1] = ((player->pos[1] - 5.0f) + player->particlePool0[arg1].unk_024);
    ++player->particlePool0[arg1].unk_01E;
    player->particlePool0[arg1].scale += 0.05;
    player->particlePool0[arg1].alpha -= 5;

    if ((s32) player->particlePool0[arg1].alpha <= 0) {
        player->particlePool0[arg1].alpha = 0;
    }

    if ((s32) player->particlePool0[arg1].unk_01E >= 0x28) {
        player->particlePool0[arg1].isAlive = 0;
        player->particlePool0[arg1].unk_01E = 0;
        player->particlePool0[arg1].type = 0;
    }
}

void func_80063408(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->particlePool1[arg1].unk_010 == 1) {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_LEFT].pos[2] +
            (player->particlePool1[arg1].unk_01E * -7) * coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_LEFT].pos[0] +
            (player->particlePool1[arg1].unk_01E * -7) * sins(player->particlePool1[arg1].unk_020);
    } else {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_RIGHT].pos[2] +
            (player->particlePool1[arg1].unk_01E * -7) * coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_RIGHT].pos[0] +
            (player->particlePool1[arg1].unk_01E * -7) * sins(player->particlePool1[arg1].unk_020);
    }

    ++player->particlePool1[arg1].unk_01E;
    player->particlePool1[arg1].pos[1] += 1.0f;

    if (((player->effects & 0x80) != 0) || ((player->effects & 0x40) != 0)) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
    }

    if (player->particlePool1[arg1].unk_01E == 8) {
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].scale += 0.08;
    if (player->particlePool1[arg1].unk_01E >= 4) {
        player->particlePool1[arg1].alpha -= 16;
    }

    if (player->particlePool1[arg1].alpha <= 0) {
        player->particlePool1[arg1].alpha = 0;
    }
}

void func_800635D4(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    if (player->particlePool1[arg1].unk_010 == 1) {
        if ((player->effects & LIGHTNING_EFFECT)) {
            func_80062B18(&sp44, &sp40, &sp3C, -2.0f, 0.0f,
                          (-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16,
                          -player->particlePool1[arg1].unk_020, 2 * -player->unk_206);
            player->particlePool1[arg1].pos[0] = player->tyres[BACK_LEFT].pos[0] + sp44;
            player->particlePool1[arg1].pos[2] = player->tyres[BACK_LEFT].pos[2] + sp3C;
        } else {
            player->particlePool1[arg1].pos[2] =
                player->tyres[BACK_LEFT].pos[2] +
                ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16) *
                    coss(player->particlePool1[arg1].unk_020);
            player->particlePool1[arg1].pos[0] =
                player->tyres[BACK_LEFT].pos[0] +
                ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16) *
                    sins(player->particlePool1[arg1].unk_020);
        }
    } else if ((player->effects & LIGHTNING_EFFECT)) {
        func_80062B18(&sp44, &sp40, &sp3C, 2.0f, 0.0f,
                      (-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16,
                      -player->particlePool1[arg1].unk_020, 2 * -player->unk_206);
        player->particlePool1[arg1].pos[0] = player->tyres[BACK_RIGHT].pos[0] + sp44;
        player->particlePool1[arg1].pos[2] = player->tyres[BACK_RIGHT].pos[2] + sp3C;
    } else {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_RIGHT].pos[2] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16) *
                coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_RIGHT].pos[0] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 16) *
                sins(player->particlePool1[arg1].unk_020);
    }

    ++player->particlePool1[arg1].unk_01E;
    player->particlePool1[arg1].pos[1] += 0.2;
    if (((player->effects & 0x80) != 0) || ((player->effects & 0x40) != 0)) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
    }

    if (player->particlePool1[arg1].unk_01E == 8) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].scale += 0.1;
    if (player->particlePool1[arg1].unk_040 == 0) {
        if (player->particlePool1[arg1].unk_01E >= 4) {
            player->particlePool1[arg1].alpha -= 12;
        }
        if (player->particlePool1[arg1].alpha <= 0) {
            player->particlePool1[arg1].alpha = 0;
        }
    } else {
        if (player->particlePool1[arg1].unk_01E >= 4) {
            player->particlePool1[arg1].alpha -= 16;
        }
        if (player->particlePool1[arg1].alpha <= 0) {
            player->particlePool1[arg1].alpha = 0;
        }
    }
}

void func_800639DC(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->particlePool1[arg1].unk_010 == 1) {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_LEFT].pos[2] +
            (-1.8f * player->particlePool1[arg1].unk_01E) * coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_LEFT].pos[0] +
            (-1.8f * player->particlePool1[arg1].unk_01E) * sins(player->particlePool1[arg1].unk_020);
    } else {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_RIGHT].pos[2] +
            (-1.8f * player->particlePool1[arg1].unk_01E) * coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_RIGHT].pos[0] +
            (-1.8f * player->particlePool1[arg1].unk_01E) * sins(player->particlePool1[arg1].unk_020);
    }
    ++player->particlePool1[arg1].unk_01E;
    player->particlePool1[arg1].pos[1] += 0.3;
    if (player->particlePool1[arg1].unk_01E == 8) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].scale += 0.15;
    if (player->particlePool1[arg1].unk_040 == 0) {
        if ((s32) player->particlePool1[arg1].unk_01E >= 4) {
            --player->particlePool1[arg1].alpha;
        }
        if ((s32) player->particlePool1[arg1].alpha <= 0) {
            player->particlePool1[arg1].alpha = 0;
        }
    } else {
        if ((s32) player->particlePool1[arg1].unk_01E >= 4) {
            player->particlePool1[arg1].alpha -= 16;
        }
        if ((s32) player->particlePool1[arg1].alpha <= 0) {

            player->particlePool1[arg1].alpha = 0;
        }
    }
}

void func_80063BD4(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->particlePool1[arg1].unk_010 == 1) {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_LEFT].pos[2] +
            (-2 * player->particlePool1[arg1].unk_01E * coss(player->particlePool1[arg1].unk_020));
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_LEFT].pos[0] +
            (-2 * player->particlePool1[arg1].unk_01E * sins(player->particlePool1[arg1].unk_020));
    } else {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_RIGHT].pos[2] +
            (-2 * player->particlePool1[arg1].unk_01E * coss(player->particlePool1[arg1].unk_020));
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_RIGHT].pos[0] +
            (-2 * player->particlePool1[arg1].unk_01E * sins(player->particlePool1[arg1].unk_020));
    }

    ++player->particlePool1[arg1].unk_01E;
    player->particlePool1[arg1].pos[1] += 0.2;
    if (player->particlePool1[arg1].unk_01E == 8) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].unk_018 = 2.0f;
    player->particlePool1[arg1].scale -= 0.06;
}

void func_80063D58(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->particlePool1[arg1].unk_010 == 1) {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_LEFT].pos[2] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_LEFT].pos[0] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                sins(player->particlePool1[arg1].unk_020);
    } else {
        player->particlePool1[arg1].pos[2] =
            player->tyres[BACK_RIGHT].pos[2] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                coss(player->particlePool1[arg1].unk_020);
        player->particlePool1[arg1].pos[0] =
            player->tyres[BACK_RIGHT].pos[0] +
            ((-player->particlePool1[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                sins(player->particlePool1[arg1].unk_020);
    }

    ++player->particlePool1[arg1].unk_01E;
    if (player->particlePool1[arg1].unk_01E == 8) {
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].scale += 0.2;
    if (player->particlePool1[arg1].unk_01E >= 4) {
        player->particlePool1[arg1].alpha -= 18;
        player->particlePool1[arg1].pos[1] -= 0.1;
    } else {
        player->particlePool1[arg1].pos[1] += 0.4;
    }

    if (player->particlePool1[arg1].alpha <= 0) {
        player->particlePool1[arg1].alpha = 0;
    }
}

void func_80063FBC(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    if (player->particlePool1[arg1].unk_010 == 1) {
        func_80062B18(&sp3C, &sp34, &sp38, 3.0f, 0.0f,
                      -5.5 -
                          (player->particlePool1[arg1].unk_01E * (((player->speed / 18.0f) * 216.0f) / 15.0f)),
                      -player->particlePool1[arg1].unk_020, 0);
    } else {
        func_80062B18(&sp3C, &sp34, &sp38, -3.0f, 0.0f,
                      -5.5 -
                          (player->particlePool1[arg1].unk_01E * (((player->speed / 18.0f) * 216.0f) / 15.0f)),
                      -player->particlePool1[arg1].unk_020, 0);
    }
    player->particlePool1[arg1].pos[0] = player->pos[0] + sp3C;
    player->particlePool1[arg1].pos[2] = player->pos[2] + sp38;
    player->particlePool1[arg1].pos[1] = (player->pos[1] - player->boundingBoxSize) + sp34;
    player->particlePool1[arg1].unk_01E++;
    if (player->particlePool1[arg1].unk_01E == 6) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }
}

void func_80064184(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    sp40 = gPlayerWaterLevel[arg2] - player->pos[1] - 3.0f;
    if (((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) != 0) && (!IsKoopaTroopaBeach())) {
        sp40 = gPlayerWaterLevel[arg2] - player->pos[1] + 0.1;
    }

    func_80062B18(&sp44, &sp40, &sp3C, 0.0f, sp40,
                  -4.0f + ((-player->particlePool0[arg1].unk_01E * (player->speed / 18.0f) * 216.0f) / 10.0f),
                  -player->particlePool0[arg1].unk_020, 2 * -player->unk_206);
    player->particlePool0[arg1].pos[0] = player->pos[0] + sp44;
    player->particlePool0[arg1].pos[2] = player->pos[2] + sp3C;
    player->particlePool0[arg1].pos[1] = player->pos[1] + sp40;
    ++player->particlePool0[arg1].unk_01E;
    if ((player->particlePool0[arg1].unk_01E == 12) ||
        (gPlayerWaterLevel[arg2] <= (player->pos[1] - player->boundingBoxSize))) {
        player->particlePool0[arg1].isAlive = 0;
        player->particlePool0[arg1].unk_01E = 0;
        player->particlePool0[arg1].type = 0;
    }
    player->particlePool0[arg1].unk_018 = 2.0f;
    player->particlePool0[arg1].scale -= 0.35;
    if (player->particlePool0[arg1].scale < 0.0f) {
        player->particlePool0[arg1].scale = 0.0f;
    }

    player->particlePool0[arg1].alpha -= 22;
    if (player->particlePool0[arg1].alpha <= 0) {
        player->particlePool0[arg1].alpha = 0;
    }
}

void func_800643A8(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    player->particlePool1[arg1].pos[2] = player->pos[2] + (-1.2 * player->particlePool1[arg1].unk_01E *
                                                                 coss(player->particlePool1[arg1].unk_020));
    player->particlePool1[arg1].pos[0] = player->pos[0] + (-1.2 * player->particlePool1[arg1].unk_01E *
                                                                 sins(player->particlePool1[arg1].unk_020));
    player->particlePool1[arg1].pos[1] = player->particlePool1[arg1].pos[1] + 0.5;

    ++player->particlePool1[arg1].unk_01E;
    if (player->particlePool1[arg1].unk_01E == 10) {
        player->particlePool1[arg1].isAlive = 0;
        player->particlePool1[arg1].unk_01E = 0;
        player->particlePool1[arg1].type = 0;
    }

    player->particlePool1[arg1].scale += 0.2;
    player->particlePool1[arg1].alpha -= 8;
    if (player->particlePool1[arg1].alpha <= 0) {
        player->particlePool1[arg1].alpha = 0;
    }
}

void func_800644E8(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 thing2;
    UNUSED s32 stackPadding0;
    s32 thing;
    UNUSED s32 stackPadding1;

    if (player->particlePool3[arg1].unk_01E >= 9) {
        player->particlePool3[arg1].unk_01E = 9;
    }
    thing2 = player->particlePool3[arg1].unk_024;
    thing = player->particlePool3[arg1].unk_01E;
    player->particlePool3[arg1].pos[2] =
        player->pos[2] + (coss(player->particlePool3[arg1].unk_020) * (-0.7 * thing));
    player->particlePool3[arg1].pos[0] =
        player->pos[0] + (sins(player->particlePool3[arg1].unk_020) * (-0.7 * thing));
    player->particlePool3[arg1].unk_01E++;
    player->particlePool3[arg1].pos[1] =
        player->particlePool3[arg1].unk_028 + (f32) ((thing * thing2) - (0.2 * (thing * thing)));
    if (player->particlePool3[arg1].unk_01E == 0x000A) {
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].type = 0;
    }
    if (player->particlePool3[arg1].unk_01E >= 7) {
        player->particlePool3[arg1].alpha -= 0x60;
        if (player->particlePool3[arg1].alpha <= 0) {
            player->particlePool3[arg1].alpha = 0;
        }
    }
}

void func_80064664(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp_f4;
    UNUSED s32 stackPadding0;
    s32 temp_v1;
    UNUSED s32 stackPadding1;

    temp_v1 = player->particlePool3[arg1].unk_01E;
    temp_f4 = player->particlePool3[arg1].unk_024;
    player->particlePool3[arg1].pos[2] =
        player->pos[2] + (coss(player->particlePool3[arg1].unk_020) * (-0.6 * temp_v1));
    player->particlePool3[arg1].pos[0] =
        player->pos[0] + (sins(player->particlePool3[arg1].unk_020) * (-0.6 * temp_v1));
    player->particlePool3[arg1].unk_01E++;
    player->particlePool3[arg1].pos[1] =
        player->particlePool3[arg1].unk_028 + (f32) ((temp_v1 * temp_f4) - (0.1 * (temp_v1 * temp_v1)));
    if (player->particlePool3[arg1].unk_01E == 0x0019) {
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].type = 0;
    }
    if (player->particlePool3[arg1].unk_01E >= 7) {
        player->particlePool3[arg1].alpha -= 0x6;
        if (player->particlePool3[arg1].alpha <= 0) {
            player->particlePool3[arg1].alpha = 0;
        }
    }
}

void func_800647C8(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {

    ++player->particlePool3[arg1].unk_01E;
    player->particlePool3[arg1].pos[2] = player->pos[2] + ((-0.8 * (player->particlePool3[arg1].unk_01E)) *
                                                                 coss(player->particlePool3[arg1].unk_020));
    player->particlePool3[arg1].pos[0] = player->pos[0] + ((-0.8 * (player->particlePool3[arg1].unk_01E)) *
                                                                 sins(player->particlePool3[arg1].unk_020));
    player->particlePool3[arg1].pos[1] = (player->unk_074 + 2.0f);

    if (player->particlePool3[arg1].unk_01E == 14) {
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].type = 0;
    }

    player->particlePool3[arg1].alpha -= 12;
    if (player->particlePool3[arg1].alpha <= 0) {
        player->particlePool3[arg1].alpha = 0;
    }
}

void func_800648E4(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->particlePool3[arg1].unk_01E;
    player->particlePool3[arg1].scale -= 0.06;
    player->particlePool3[arg1].pos[1] += 0.1;
    player->particlePool3[arg1].alpha -= 12;

    if (player->particlePool3[arg1].alpha <= 0) {
        player->particlePool3[arg1].alpha = 0;
    }

    if (player->particlePool3[arg1].unk_01E == 10) {
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].type = 0;
    }
}

void func_80064988(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->particlePool3[arg1].unk_01E;
    player->particlePool3[arg1].pos[1] -= 0.3;

    if (player->particlePool3[arg1].unk_01E == 10) {
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].type = 0;
    }
}

void func_800649F4(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp;
    temp = player->particlePool3[arg1].unk_018;

    player->particlePool3[arg1].pos[2] =
        player->unk_21C +
        (((-temp) * player->particlePool3[arg1].unk_01E) * coss(player->particlePool3[arg1].unk_020));
    player->particlePool3[arg1].pos[0] =
        player->unk_218 +
        (((-temp) * player->particlePool3[arg1].unk_01E) * sins(player->particlePool3[arg1].unk_020));
    player->particlePool3[arg1].pos[1] = player->pos[1] + player->particlePool3[arg1].surfaceType;
    player->particlePool3[arg1].scale += 0.04;

    ++player->particlePool3[arg1].unk_01E;
    if (player->particlePool3[arg1].unk_01E == 12) {
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].type = 0;
    }

    if (player->particlePool3[arg1].unk_01E >= 9) {
        player->particlePool3[arg1].alpha -= 0x10;
        if (player->particlePool3[arg1].alpha <= 0) {
            player->particlePool3[arg1].alpha = 0;
        }
    }
}

void func_80064B30(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp;
    temp = player->particlePool3[arg1].unk_018 * 1.2;

    player->particlePool3[arg1].pos[2] = (player->pos[2] + (-temp * player->particlePool3[arg1].unk_01E) *
                                                                     (coss(player->particlePool3[arg1].unk_020)));
    player->particlePool3[arg1].pos[0] = (player->pos[0] + (-temp * player->particlePool3[arg1].unk_01E) *
                                                                     (sins(player->particlePool3[arg1].unk_020)));
    player->particlePool3[arg1].pos[1] += 0.1;

    ++player->particlePool3[arg1].unk_01E;
    if (player->particlePool3[arg1].unk_01E == 10) {
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].type = 0;
    }

    player->particlePool3[arg1].red += 1820;
    if (player->particlePool3[arg1].unk_01E >= 6) {
        player->particlePool3[arg1].alpha -= 16;
        if (player->particlePool3[arg1].alpha <= 0) {
            player->particlePool3[arg1].alpha = 0;
        }
    }
}

void func_80064C74(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {

    if (player->particlePool3[arg1].unk_010 == 1) {
        player->particlePool3[arg1].unk_020 += 2184;
    } else {
        player->particlePool3[arg1].unk_020 -= 2184;
    }

    player->particlePool3[arg1].pos[2] =
        player->pos[2] +
        (coss(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
    player->particlePool3[arg1].pos[0] =
        player->pos[0] +
        (sins(player->particlePool3[arg1].unk_020 - player->rotation[1] - player->unk_0C0) * 5.0f);
    player->particlePool3[arg1].pos[1] = player->pos[1] - 1.0f;
    player->particlePool3[arg1].scale += 0.4;
    ++player->particlePool3[arg1].unk_01E;

    if (player->particlePool3[arg1].unk_01E == 10) {
        player->particlePool3[arg1].unk_01E = 0;
        player->particlePool3[arg1].isAlive = 0;
        player->particlePool3[arg1].type = 0;
    }
    if (player->particlePool3[arg1].unk_01E >= 5) {
        player->particlePool3[arg1].alpha -= 20;
        if (player->particlePool3[arg1].alpha <= 0) {
            player->particlePool3[arg1].alpha = 0;
        }
    }
}

void func_80064DEC(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {

    player->particlePool2[index].pos[1] = player->pos[1];
    ++player->particlePool2[index].unk_01E;

    if (player->particlePool2[index].unk_01E == 9) {
        player->unk_0B6 &= ~0x0040;
        player->particlePool2[index].isAlive = 0;
        player->particlePool2[index].unk_01E = 0;
        player->particlePool2[index].type = 0;
    }

    player->particlePool2[index].scale += 0.8;
    if (player->particlePool2[index].scale >= (f64) 2.5) {
        player->particlePool2[index].scale = 2.5f;
    }
}

void func_80064EA4(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->particlePool2[index].unk_01E;
    if (player->particlePool2[index].unk_01E < 4) {
        player->particlePool2[index].scale += 1.2;
        if (player->particlePool2[index].scale >= 3.5) {
            player->particlePool2[index].scale = 3.5f;
        }
    } else {
        player->particlePool2[index].scale -= 1.8;
        if (player->particlePool2[index].scale <= 0.0f) {
            player->unk_0B6 &= ~0x1000;
            player->particlePool2[index].isAlive = 0;
            player->particlePool2[index].unk_01E = 0;
            player->particlePool2[index].type = 0;
        }
    }
}

void func_80064F88(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->particlePool2[index].unk_01E;
    player->particlePool2[index].scale += 0.15;

    if (1.2 <= player->particlePool2[index].scale) {
        player->particlePool2[index].scale = 1.2f;
    }
    if (player->particlePool2[index].unk_01E >= 12) {
        player->unk_0B6 &= ~0x0800;
        player->particlePool2[index].isAlive = 0;
        player->particlePool2[index].unk_01E = 0;
        player->particlePool2[index].type = 0;
    }
}

void func_80065030(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->particlePool2[index].unk_01E;

    player->particlePool2[index].pos[1] += 0.8;
    player->particlePool2[index].scale += 0.4;
    if (player->particlePool2[index].scale >= (f64) 1.5) {
        player->particlePool2[index].scale = 1.5f;
    }

    if (player->particlePool2[index].unk_01E >= 12) {
        player->unk_0B6 &= ~0x0100;
        player->particlePool2[index].isAlive = 0;
        player->particlePool2[index].unk_01E = 0;
        player->particlePool2[index].type = 0;
    }
}

void func_800650FC(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->particlePool2[index].pos[2] = (f32) player->pos[2];
    player->particlePool2[index].pos[0] = (f32) player->pos[0];
    player->particlePool2[index].pos[1] = (f32) (player->pos[1] + 4.0f);
    if ((player->effects & 0x80) == 0x80) {
        player->particlePool2[index].unk_020 += 4732;
    } else {
        player->particlePool2[index].unk_020 -= 4732;
    }

    if (((player->effects & 0x80) != 0x80) && ((player->effects & 0x40) != 0x40)) {
        player->particlePool2[index].isAlive = 0;
        player->particlePool2[index].unk_01E = 0;
        player->particlePool2[index].type = 0;
    }

    player->particlePool2[index].scale += 0.08;
    if (player->particlePool2[index].scale >= 1.5) {
        player->particlePool2[index].scale = 1.5f;
    }
}

void func_800651F4(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->particlePool2[index].unk_01E;
    if (player->particlePool2[index].unk_01E < 8) {
        player->particlePool2[index].scale += 0.2;
        if (1.2 <= player->particlePool2[index].scale) {
            player->particlePool2[index].scale = 1.2f;
        }
    } else {
        player->particlePool2[index].scale -= 0.4;
        if (player->particlePool2[index].scale <= 0.0f) {
            player->unk_0B6 &= ~0x0020;
            player->particlePool2[index].isAlive = 0;
            player->particlePool2[index].unk_01E = 0;
            player->particlePool2[index].type = 0;
        }
    }
}

void func_800652D4(Vec3f arg0, Vec3s arg1, f32 arg2) {
    Mat4 mtx;
    mtxf_translate_rotate(mtx, arg0, arg1);
    mtxf_scale(mtx, arg2);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], mtx);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void func_8006538C(Player* player, s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 primColors[] = { MAKE_RGB(0xFB, 0xFF, 0xFB), MAKE_RGB(0xFF, 0xFB, 0x86) };
    s32 envColors[] = { MAKE_RGB(0x89, 0x62, 0x8F), MAKE_RGB(0xFE, 0x01, 0x09) };
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;

    if (player->particlePool0[arg2].isAlive == 1) {
        spB4[0] = player->particlePool0[arg2].pos[0];
        spB4[1] = player->particlePool0[arg2].pos[1];
        spB4[2] = player->particlePool0[arg2].pos[2];
        spAC[0] = 0;
        spAC[1] = player->unk_048[arg3];
        spAC[2] = 0;
        if ((player->effects & STAR_EFFECT) && (((s32) gCourseTimer - gPlayerStarEffectStartTime[arg1]) < 9)) {
            primRed = (primColors[1] >> 0x10) & 0xFF;
            primGreen = (primColors[1] >> 0x08) & 0xFF;
            primBlue = (primColors[1] >> 0x00) & 0xFF;
            envRed = (envColors[1] >> 0x10) & 0xFF;
            envGreen = (envColors[1] >> 0x08) & 0xFF;
            envBlue = (envColors[1] >> 0x00) & 0xFF;
            primAlpha = player->particlePool0[arg2].alpha;
            func_800652D4(spB4, spAC, ((player->particlePool0[arg2].scale * player->size) * 1.4));
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++,
                                common_texture_particle_smoke[player->particlePool0[arg2].unk_010], G_IM_FMT_I,
                                G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        } else {
            primRed = (primColors[player->particlePool0[arg2].red] >> 0x10) & 0xFF;
            primGreen = (primColors[player->particlePool0[arg2].red] >> 0x08) & 0xFF;
            primBlue = (primColors[player->particlePool0[arg2].red] >> 0x00) & 0xFF;
            envRed = (envColors[player->particlePool0[arg2].red] >> 0x10) & 0xFF;
            envGreen = (envColors[player->particlePool0[arg2].red] >> 0x08) & 0xFF;
            envBlue = (envColors[player->particlePool0[arg2].red] >> 0x00) & 0xFF;
            primAlpha = player->particlePool0[arg2].alpha;
            func_800652D4(spB4, spAC, player->particlePool0[arg2].scale * player->size);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++,
                                common_texture_particle_smoke[player->particlePool0[arg2].unk_010], G_IM_FMT_I,
                                G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        gMatrixEffectCount += 1;
    }
}

void func_800658A0(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool0[arg2].isAlive == 1) {
        red = player->particlePool0[arg2].red;
        green = player->particlePool0[arg2].green;
        blue = player->particlePool0[arg2].blue;
        alpha = player->particlePool0[arg2].alpha;
        sp54[0] = player->particlePool0[arg2].pos[0];
        sp54[1] = player->particlePool0[arg2].pos[1];
        sp54[2] = player->particlePool0[arg2].pos[2];
        sp4C[0] = 0;
        sp4C[1] = player->unk_048[arg3];
        sp4C[2] = 0;
        func_800652D4(sp54, sp4C, player->particlePool0[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        func_8004B35C(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        gMatrixEffectCount += 1;
    }
}

#ifdef NON_MATCHING
// Something about the handling of the prim/env colors is off,
// its causing a huge diff. Can't figure out what's up.
void render_player_drift_particles(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 var_s0;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s32 sp8C[] = { 0x00ffffff, 0x00ffff00, 0x00ff9600 };
    if (player->particlePool1[arg2].isAlive == 1) {
        if (player->driftDuration >= 0x32) {
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }
        primRed = player->particlePool1[arg2].red;
        primGreen = player->particlePool1[arg2].green;
        primBlue = player->particlePool1[arg2].blue;
        primAlpha = player->particlePool1[arg2].alpha;
        envRed = (sp8C[player->particlePool1[arg2].unk_040] >> 0x10) & 0xFF;
        envGreen = (sp8C[player->particlePool1[arg2].unk_040] >> 0x08) & 0xFF;
        envBlue = (sp8C[player->particlePool1[arg2].unk_040] >> 0x00) & 0xFF;
        spB4[0] = player->particlePool1[arg2].pos[0];
        spB4[1] = player->particlePool1[arg2].pos[1];
        spB4[2] = player->particlePool1[arg2].pos[2];
        spAC[0] = 0;
        spAC[1] = player->unk_048[arg3];
        spAC[2] = 0;
        func_800652D4(spB4, spAC, player->particlePool1[arg2].scale * player->size);
        if (var_s0 == 0) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, D_0D008DF8);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        gMatrixEffectCount += 1;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80057C60/render_player_drift_particles.s")
#endif

#ifdef NON_MATCHING
// https://decomp.me/scratch/KEz08
//  Something is very wrong with the handling of prim/evn colors, but I can't figuer out what.
void render_player_ground_particles(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spDC;
    Vec3s spD4;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    u8 envRed;
    s16 envGreen;
    s16 envBlue;

    if ((player->particlePool1[arg2].isAlive == 1) && (player->particlePool1[arg2].unk_01E != 0)) {
        spDC[0] = player->particlePool1[arg2].pos[0];
        spDC[1] = player->particlePool1[arg2].pos[1];
        spDC[2] = player->particlePool1[arg2].pos[2];
        spD4[0] = 0;
        spD4[1] = player->unk_048[arg3];
        spD4[2] = 0;
        func_800652D4(spDC, spD4, player->particlePool1[arg2].scale * player->size);
        if ((s32) player->particlePool1[arg2].surfaceType != GRASS) {
            primRed = ((D_800E47DC[player->particlePool1[arg2].red] >> 0x10) & 0xFF) -
                      player->particlePool1[arg2].green;
            primGreen = ((D_800E47DC[player->particlePool1[arg2].red] >> 0x08) & 0xFF) -
                        player->particlePool1[arg2].green;
            primBlue = ((D_800E47DC[player->particlePool1[arg2].red] >> 0x00) & 0xFF) -
                       player->particlePool1[arg2].green;
            envRed = ((D_800E480C[player->particlePool1[arg2].red] >> 0x10) & 0xFF) -
                     player->particlePool1[arg2].green;
            envGreen = ((D_800E480C[player->particlePool1[arg2].red] >> 0x08) & 0xFF) -
                       player->particlePool1[arg2].green;
            envBlue = ((D_800E480C[player->particlePool1[arg2].red] >> 0x00) & 0xFF) -
                      player->particlePool1[arg2].green;
            primAlpha = player->particlePool1[arg2].alpha;
            if (player->particlePool1[arg2].unk_040 == 0) {
                gSPDisplayList(gDisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(gDisplayListHead++, gGroundDust, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gSPDisplayList(gDisplayListHead++, D_0D008E48);
            } else {
                gSPDisplayList(gDisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(gDisplayListHead++, gGroundDust, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
                gSPDisplayList(gDisplayListHead++, D_0D008E48);
            }
        } else {
            primRed = player->particlePool1[arg2].red;
            primGreen = player->particlePool1[arg2].green;
            primBlue = player->particlePool1[arg2].blue;
            gSPDisplayList(gDisplayListHead++, D_0D008C90);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
            gDPLoadTextureBlock(gDisplayListHead++, gGrassParticle, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B35C(primRed, primGreen, primBlue, 0x000000FF);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
            gSPVertex(gDisplayListHead++, D_800E8C00, 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        }
        gMatrixEffectCount += 1;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80057C60/render_player_ground_particles.s")
#endif

void func_800664E0(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool1[arg2].isAlive == 1) {
        red = player->particlePool1[arg2].red;
        green = player->particlePool1[arg2].green;
        blue = player->particlePool1[arg2].blue;
        alpha = player->particlePool1[arg2].alpha;
        sp54[0] = player->particlePool1[arg2].pos[0];
        sp54[1] = player->particlePool1[arg2].pos[1];
        sp54[2] = player->particlePool1[arg2].pos[2];
        sp4C[0] = 0;
        sp4C[1] = player->unk_048[arg3];
        sp4C[2] = 0;
        func_800652D4(sp54, sp4C, player->particlePool1[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        func_8004B35C(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        gMatrixEffectCount += 1;
    }
}

void func_80066714(Player* player, UNUSED s32 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool1[arg2].isAlive == 1) {
        red = player->particlePool1[arg2].red;
        green = player->particlePool1[arg2].green;
        blue = player->particlePool1[arg2].blue;
        alpha = player->particlePool1[arg2].alpha;
        sp5C[0] = player->particlePool1[arg2].pos[0];
        sp5C[1] = player->particlePool1[arg2].pos[1];
        sp5C[2] = player->particlePool1[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = 0;
        func_800652D4(sp5C, sp54, player->particlePool1[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_fire, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(red, green, blue, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8B00, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_80066998(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool0[arg2].isAlive == 1) {
        red = player->particlePool0[arg2].red;
        green = player->particlePool0[arg2].green;
        blue = player->particlePool0[arg2].blue;
        alpha = player->particlePool0[arg2].alpha;
        sp54[0] = player->particlePool0[arg2].pos[0];
        sp54[1] = player->particlePool0[arg2].pos[1];
        sp54[2] = player->particlePool0[arg2].pos[2];
        sp4C[0] = 0x4000;
        sp4C[1] = player->unk_048[arg3];
        sp4C[2] = 0;
        func_800652D4(sp54, sp4C, player->particlePool0[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        func_8004B35C(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        gMatrixEffectCount += 1;
    }
}

Vtx D_800E8900a[][4] = {
    {
        { { { 2, 4, 0 }, 0, { 0, 0 }, { 0, 255, 255, 112 } } },
        { { { 2, 0, 0 }, 0, { 0, 4032 }, { 0, 255, 255, 112 } } },
        { { { 0, 0, 0 }, 0, { 1984, 4032 }, { 255, 255, 0, 112 } } },
        { { { 0, 4, 0 }, 0, { 1984, 0 }, { 0, 255, 255, 112 } } },
    },
    {
        { { { 0, 4, 0 }, 0, { 0, 0 }, { 0, 255, 255, 112 } } },
        { { { 0, 0, 0 }, 0, { 0, 4032 }, { 255, 255, 0, 112 } } },
        { { { -2, 0, 0 }, 0, { 1984, 4032 }, { 0, 255, 255, 112 } } },
        { { { -2, 4, 0 }, 0, { 1984, 0 }, { 0, 255, 255, 112 } } },
    },
    {
        { { { 2, 4, 0 }, 0, { 1984, 0 }, { 0, 255, 255, 112 } } },
        { { { 2, 0, 0 }, 0, { 1984, 4032 }, { 0, 255, 255, 112 } } },
        { { { 0, 0, 0 }, 0, { 0, 4032 }, { 255, 255, 0, 112 } } },
        { { { 0, 4, 0 }, 0, { 0, 0 }, { 0, 255, 255, 112 } } },
    },
    {
        { { { 0, 4, 0 }, 0, { 1984, 0 }, { 0, 255, 255, 112 } } },
        { { { 0, 0, 0 }, 0, { 1984, 4032 }, { 255, 255, 0, 112 } } },
        { { { -2, 0, 0 }, 0, { 0, 4032 }, { 0, 255, 255, 112 } } },
        { { { -2, 4, 0 }, 0, { 0, 0 }, { 0, 255, 255, 112 } } },
    },
};

void func_80066BAC(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spDC;
    Vec3s spD4;
    UNUSED s32 stackPadding;
    // Vtx **A_800E8900 = (Vtx **) LOAD_ASSET(D_800E8900);

    if ((player->particlePool0[arg2].isAlive == 1) && (player->particlePool0[arg2].red != 0x00FF)) {

        if (player->collision.surfaceDistance[2] >= 300.0f) {
            spDC[1] = player->pos[1] + 5.0f;
        } else {
            spDC[1] = player->pos[1] - 3.0f;
        }
        spDC[2] = player->pos[2] + (coss(player->unk_048[arg3]) * -10.0f);
        spDC[0] = player->pos[0] + (sins(player->unk_048[arg3]) * -10.0f);
        if (player->collision.surfaceDistance[2] >= 300.0f) {
            spD4[0] = cameras[arg3].rot[0] - 0x4000;
        } else {
            spD4[0] = 0;
        }
        spD4[1] = player->unk_048[arg3];
        spD4[2] = 0;
        func_800652D4(spDC, spD4, player->particlePool0[arg2].scale * player->size);
        if (player->particlePool0[arg2].red == 0) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900a[0][player->particlePool0[arg2].red], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt1, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900a[1][player->particlePool0[arg2].red], 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt1, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900a[0][player->particlePool0[arg2].red], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900a[1][player->particlePool0[arg2].red], 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        }
        gMatrixEffectCount += 1;
    }
}

void func_80067280(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp7C;
    Vec3s sp74;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool3[arg2].isAlive == 1) {
        red = player->particlePool3[arg2].red;
        green = player->particlePool3[arg2].green;
        blue = player->particlePool3[arg2].blue;
        alpha = player->particlePool3[arg2].alpha;
        sp7C[0] = player->particlePool3[arg2].pos[0];
        sp7C[1] = player->particlePool3[arg2].pos[1];
        sp7C[2] = player->particlePool3[arg2].pos[2];
        sp74[0] = -0x071C;
        sp74[2] = 0;
        if (player->particlePool3[arg2].unk_010 == 1) {
            sp74[1] = player->unk_048[arg3] - 0x2000;
            func_800652D4(sp7C, sp74, player->particlePool3[arg2].scale * player->size);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B35C(red, green, blue, alpha);
            gSPDisplayList(gDisplayListHead++, D_0D008E70);
        } else {
            sp74[1] = player->unk_048[arg3] + 0x2000;
            func_800652D4(sp7C, sp74, player->particlePool3[arg2].scale * player->size);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B35C(red, green, blue, alpha & 0xFFFFFFFF); // huh?
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        gMatrixEffectCount += 1;
    }
}

void render_player_boost_spark_particles(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp8C;
    Vec3s sp84;
    UNUSED s32 stackPadding[4];

    if (player->particlePool3[arg2].isAlive == 1) {
        sp8C[0] = player->particlePool3[arg2].pos[0];
        sp8C[1] = player->particlePool3[arg2].pos[1];
        sp8C[2] = player->particlePool3[arg2].pos[2];
        sp84[0] = 0;
        sp84[1] = player->unk_048[arg3];
        sp84[2] = 0;
        func_800652D4(sp8C, sp84, player->particlePool3[arg2].scale * player->size);
        if (player->particlePool3[arg2].unk_010 == 1) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark[0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32,
                                0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(gDisplayListHead++, D_0D008E70);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark[0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32,
                                0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        gMatrixEffectCount += 1;
    }
}

void render_player_onomatopoeia_whrrrr(Player* player, UNUSED s8 arg1, f32 arg2, UNUSED s8 arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;
    UNUSED s32 stackPadding[2];

    if (player->particlePool2[arg4].isAlive == 1) {
        sp9C[0] = player->particlePool2[arg4].pos[0];
        sp9C[1] = player->particlePool2[arg4].pos[1];
        sp9C[2] = player->particlePool2[arg4].pos[2];
        sp94[0] = 0;
        sp94[1] = player->particlePool2[arg4].unk_020;
        sp94[2] = 0;
        func_800652D4(sp9C, sp94, player->size * arg2);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
        gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaWhrrrr1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8840, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaWhrrrr2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8800, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void render_player_speech_bubble(Player* player, s8 arg1, u8* texture, s8 arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { 0.0f, -1.2f, 0.1f, 1.2f, -1.7f, -0.8f, -0.2f, -1.9f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (player->particlePool2[arg3].isAlive == 1) {
        sp74[0] = 0;
        sp74[1] = player->unk_048[arg1];
        sp74[2] = 0;
        sp7C[0] = player->pos[0] + (sins((0x4000 & 0xFFFFFFFF) - (player->rotation[1] + player->unk_0C0)) * arg4);
        sp7C[1] = player->pos[1] + player->boundingBoxSize - sp54[player->characterId] - 2.0f;
        sp7C[2] = player->pos[2] + (coss((0x4000 & 0xFFFFFFFF) - (player->rotation[1] + player->unk_0C0)) * arg4);
        func_800652D4(sp7C, sp74, player->particlePool2[arg3].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        func_8004B414(red, green, blue, 0x000000FF);
        gSPDisplayList(gDisplayListHead++, D_0D008E20);
        gMatrixEffectCount += 1;
    }
}

void render_music_note(Player* player, s8 arg1, u8* texture, s8 arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { -0.7f, -1.9f, -0.6f, 0.4f, -2.5f, -1.6f, -0.95f, -2.7f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (player->particlePool2[arg3].isAlive == 1) {
        sp74[0] = 0;
        sp74[1] = player->unk_048[arg1];
        sp74[2] = 0;
        sp7C[0] = player->pos[0] + (sins((0x4000 & 0xFFFFFFFF) - (player->rotation[1] + player->unk_0C0)) * arg4);
        sp7C[1] = player->pos[1] + player->boundingBoxSize - sp54[player->characterId] - 2.0f;
        sp7C[2] = player->pos[2] + (coss((0x4000 & 0xFFFFFFFF) - (player->rotation[1] + player->unk_0C0)) * arg4);
        func_800652D4(sp7C, sp74, player->particlePool2[arg3].scale * player->size * 0.8);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        func_8004B414(red, green, blue, 0x000000FF);
        gSPDisplayList(gDisplayListHead++, D_0D008E20);
        gMatrixEffectCount += 1;
    }
}

void render_player_onomatopoeia_crash(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp9C;
    Vec3s sp94;

    if (player->particlePool2[arg4].isAlive == 1) {
        sp9C[1] = player->particlePool2[arg4].pos[1];
        sp9C[2] = player->pos[2] + (coss(player->unk_048[arg3]) * -10.0f);
        sp9C[0] = player->pos[0] + (sins(player->unk_048[arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = player->unk_048[arg3];
        sp94[2] = 0;
        func_800652D4(sp9C, sp94, player->particlePool2[arg4].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaCrash1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8880, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaCrash2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E88C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_80068724(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp84;
    Vec3s sp7C;

    if (player->particlePool2[arg4].isAlive == 1) {
        sp84[1] = player->pos[1] - 3.0f;
        sp84[2] = player->pos[2] + (coss(player->unk_048[arg3]) * -10.0f);
        sp84[0] = player->pos[0] + (sins(player->unk_048[arg3]) * -10.0f);
        sp7C[0] = 0;
        sp7C[1] = player->unk_048[arg3];
        sp7C[2] = 0;
        func_800652D4(sp84, sp7C, player->particlePool2[arg4].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8A00, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureLoadedLightningBolt1, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8A40, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void render_player_onomatopoeia_boing(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    Vec3f sp64;
    Vec3s sp5C;

    sp64[1] = player->pos[1];
    sp64[2] = player->pos[2];
    sp64[0] = player->pos[0];

    if ((player->particlePool2[arg4].isAlive == 1) && (player->animFrameSelector[arg3] < 0xD)) {
        if (gTickVisuals) {
            sp64[1] = player->pos[1] - 3.0f;
            sp64[2] =
                player->pos[2] + ((-2.5 * player->particlePool2[arg4].unk_01E) * coss(player->unk_048[arg3]));
            sp64[0] =
                player->pos[0] + ((-2.5 * player->particlePool2[arg4].unk_01E) * sins(player->unk_048[arg3]));
        }
        sp5C[0] = 0;
        sp5C[1] = player->unk_048[arg3];
        sp5C[2] = 0;
        func_800652D4(sp64, sp5C, player->particlePool2[arg4].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D4A0, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8B40, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void render_player_onomatopoeia_pomp(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;

    if ((player->particlePool2[arg4].isAlive == 1) && ((s32) player->animFrameSelector[arg3] < 0xD)) {
        sp9C[1] = (player->pos[1] - 3.0f) + player->particlePool2[arg4].pos[1];
        sp9C[2] = player->pos[2] + (coss(player->unk_048[arg3]) * -10.0f);
        sp9C[0] = player->pos[0] + (sins(player->unk_048[arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = player->unk_048[arg3];
        sp94[2] = 0;
        func_800652D4(sp9C, sp94, player->particlePool2[arg4].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaPoomp1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8B80, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, gTextureOnomatopoeiaPoomp2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8BC0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_800691B8(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->particlePool3[arg2].isAlive == 1) {
        alpha = player->particlePool3[arg2].alpha;
        sp5C[0] = player->particlePool3[arg2].pos[0];
        sp5C[1] = player->particlePool3[arg2].pos[1];
        sp5C[2] = player->particlePool3[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        player->particlePool3[arg2].green += 0x1C71;
        sp54[2] = player->particlePool3[arg2].green;
        func_800652D4(sp5C, sp54, player->size * 0.5);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, gTexture69C80C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0xFF, 0xFF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount++;
    }
}

void func_80069444(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp74;
    Vec3s sp6C;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    u16 test;
    if (player->particlePool3[arg2].isAlive == 1) {
        primRed = (D_800E47DC[player->particlePool3[arg2].red] >> 0x10) & 0xFF;
        primGreen = (D_800E47DC[player->particlePool3[arg2].red] >> 8) & 0xFF;
        primBlue = D_800E47DC[player->particlePool3[arg2].red] & 0xFF;

        envRed = (D_800E480C[player->particlePool3[arg2].red] >> 0x10) & 0xFF;
        envGreen = (D_800E480C[player->particlePool3[arg2].red] >> 8) & 0xFF;
        envBlue = D_800E480C[player->particlePool3[arg2].red] & 0xFF;
        primAlpha = player->particlePool3[arg2].alpha;

        sp74[0] = player->particlePool3[arg2].pos[0];
        sp74[1] = player->particlePool3[arg2].pos[1];
        sp74[2] = player->particlePool3[arg2].pos[2];
        sp6C[0] = 0;
        sp6C[1] = player->unk_048[arg3];
        sp6C[2] = 0;
        func_800652D4(sp74, sp6C, player->size * 1.5);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, gGroundDust, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        // `test` MUST be a u16
        // `test` MUST be set to `envRed`s value
        // `test` MUST be typecast to s16 when passed as a function parameter
        // What. the. fuck.
        test = envRed;
        func_8004B72C(primRed, primGreen, primBlue, (s16) test, envGreen, envBlue, primAlpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        gMatrixEffectCount += 1;
    }
}

void render_wall_bonk_star_particles(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3, f32 arg4) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->particlePool3[arg2].isAlive == 1) {
        alpha = player->particlePool3[arg2].alpha;
        sp5C[0] = player->particlePool3[arg2].pos[0];
        sp5C[1] = player->particlePool3[arg2].pos[1];
        sp5C[2] = player->particlePool3[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = 0;

        func_800652D4(sp5C, sp54, player->size * arg4);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, gTexture69C80C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_80069938(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->particlePool3[arg2].isAlive == 1) {
        alpha = player->particlePool3[arg2].alpha;
        sp5C[0] = player->particlePool3[arg2].pos[0];
        sp5C[1] = player->particlePool3[arg2].pos[1];
        sp5C[2] = player->particlePool3[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = player->particlePool3[arg2].red;
        func_800652D4(sp5C, sp54, player->particlePool3[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, gTexture69C80C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_80069BA8(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->particlePool3[arg2].isAlive == 1) {
        red = player->particlePool3[arg2].red;
        green = player->particlePool3[arg2].green;
        blue = player->particlePool3[arg2].blue;
        alpha = player->particlePool3[arg2].alpha;
        sp54[0] = player->particlePool3[arg2].pos[0];
        sp54[1] = player->particlePool3[arg2].pos[1];
        sp54[2] = player->particlePool3[arg2].pos[2];
        sp4C[0] = 0;
        sp4C[1] = player->unk_048[arg3];
        sp4C[2] = 0;
        func_800652D4(sp54, sp4C, player->particlePool3[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        func_8004B35C(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        gMatrixEffectCount += 1;
    }
}

void func_80069DB8(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    UNUSED s32 stackPadding[2];

    if (player->particlePool3[arg2].isAlive == 1) {
        sp5C[0] = player->particlePool3[arg2].pos[0];
        sp5C[1] = player->particlePool3[arg2].pos[1];
        sp5C[2] = player->particlePool3[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = 0;
        func_800652D4(sp5C, sp54, player->particlePool3[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D49C, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8740, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void func_8006A01C(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;

    if (player->particlePool0[arg2].isAlive == 1) {
        sp54[0] = player->particlePool1[arg2].pos[0];
        sp54[1] = player->particlePool1[arg2].pos[1];
        sp54[2] = player->particlePool1[arg2].pos[2];
        sp4C[0] = 0;
        sp4C[1] = player->unk_048[arg3];
        sp4C[2] = 0;
        func_800652D4(sp54, sp4C, player->particlePool1[arg2].scale * player->size);

        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D49C, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_0D008B78, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount++;
    }
}

void func_8006A280(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 green;
    s16 blue;

    if (player->particlePool0[arg2].isAlive == 1) {
        red = player->particlePool0[arg2].red;
        green = player->particlePool0[arg2].green;
        blue = player->particlePool0[arg2].blue;
        sp5C[0] = player->particlePool0[arg2].pos[0];
        sp5C[1] = player->particlePool0[arg2].pos[1];
        sp5C[2] = player->particlePool0[arg2].pos[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = 0;
        func_800652D4(sp5C, sp54, player->particlePool0[arg2].scale * player->size);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock_4b(gDisplayListHead++, *D_800E47A0[player->particlePool0[arg2].unk_01E], G_IM_FMT_I,
                               64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        func_8004B414(red, green, blue, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8780, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        gMatrixEffectCount += 1;
    }
}

void init_balloon(Player* player, f32 arg1, f32 arg2, s8 arg3, s8 arg4, s16 arg5) {
    f32 someX;
    f32 someY;
    f32 someZ;

    gPlayerBalloonStatus[arg3][arg4] = 0;
    D_8018D650[arg3][arg4] = 0.1f;
    D_8018D6B0[arg3][arg4] = 0.0f;
    D_8018D710[arg3][arg4] = 0.0f;
    D_8018D770[arg3][arg4] = 0;
    D_8018D7A0[arg3][arg4] = 0;
    D_8018D7D0[arg3][arg4] = 0;
    D_8018D800[arg3][arg4] = 5;
    D_8018D830[arg3][arg4] = 1;
    D_8018D620[arg3][arg4] = -player->rotation[1] - player->unk_0C0;
    func_80062B18(&someX, &someY, &someZ, arg1, 4.0f, arg2 + -3.8, -player->rotation[1], 0);
    gPlayerBalloonPosX[arg3][arg4] = player->pos[0] + someX;
    gPlayerBalloonPosZ[arg3][arg4] = player->pos[2] + someZ;
    gPlayerBalloonPosY[arg3][arg4] = (player->pos[1] - player->boundingBoxSize) + someY;
    gPlayerBalloonStatus[arg3][arg4] |= 1;
    D_8018D860[arg3][arg4] = arg5;
    D_8018D890[arg3][arg4] = 0;
}

void func_8006A7C0(Player* player, f32 arg1, f32 arg2, s8 arg3, s8 arg4) {
    f32 sp80[] = {
        9.0f, 10.0f, 9.0f, 8.0f, 10.0f, 9.5f, 9.5f, 11.0f,
    };
    UNUSED s32 stackPadding0;
    f32 someX;
    f32 someY;
    f32 someZ;
    f32 sp6C;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;

    sp6C = (-(player->speed / 18.0f) * 216.0f) / 10.0f;
    if ((gPlayerBalloonStatus[arg3][arg4] & 2) != 2) {
        D_8018D650[arg3][arg4] += -0.003 + (-player->speed * 0.0006);
        if (D_8018D650[arg3][arg4] >= 0.05) {
            D_8018D650[arg3][arg4] = 0.05f;
        }
        if (D_8018D650[arg3][arg4] <= -0.05) {
            D_8018D650[arg3][arg4] = -0.05f;
        }
        D_8018D6B0[arg3][arg4] += D_8018D650[arg3][arg4];
        if (D_8018D6B0[arg3][arg4] >= 0.06) {
            D_8018D6B0[arg3][arg4] = 0.06f;
        }
        if (D_8018D6B0[arg3][arg4] <= -0.06) {
            D_8018D6B0[arg3][arg4] = -0.06f;
        }
        D_8018D710[arg3][arg4] += D_8018D6B0[arg3][arg4];
        if (D_8018D710[arg3][arg4] < 0.0f) {
            D_8018D650[arg3][arg4] = random_int(0x000BU) / 10;
            D_8018D6B0[arg3][arg4] = 0.0f;
            D_8018D710[arg3][arg4] = 0.0f;
        }
        D_8018D620[arg3][arg4] = -player->rotation[1] - player->unk_0C0;
        move_s16_towards(&D_8018D890[arg3][arg4], player->speed * 182.0f, 0.1f);
    }
    if (D_8018D830[arg3][arg4] == 1) {
        D_8018D770[arg3][arg4] += D_8018D800[arg3][arg4] - player->speed;
    } else {
        D_8018D770[arg3][arg4] += D_8018D800[arg3][arg4] + player->speed;
    }
    if (D_8018D770[arg3][arg4] >= 0xB) {
        D_8018D770[arg3][arg4] = 0x000B;
    }
    if (D_8018D770[arg3][arg4] < -0xA) {
        D_8018D770[arg3][arg4] = -0x000B;
    }
    D_8018D7A0[arg3][arg4] += D_8018D770[arg3][arg4];
    if (D_8018D7A0[arg3][arg4] >= 0x29) {
        D_8018D7A0[arg3][arg4] = 0x0029;
    }
    if (D_8018D7A0[arg3][arg4] < -0x28) {
        D_8018D7A0[arg3][arg4] = -0x0029;
    }
    D_8018D7D0[arg3][arg4] += D_8018D7A0[arg3][arg4];
    if (D_8018D7D0[arg3][arg4] >= 0x38E) {
        D_8018D800[arg3][arg4] = -random_int(8U);
        if (D_8018D830[arg3][arg4] != 1) {
            D_8018D830[arg3][arg4] = 1;
        }
    }
    if (D_8018D7D0[arg3][arg4] < -0x38D) {
        D_8018D800[arg3][arg4] = random_int(8U);
        if (D_8018D830[arg3][arg4] != -1) {
            D_8018D830[arg3][arg4] = -1;
        }
    }
    func_80062B18(&someX, &someY, &someZ, arg1, sp80[player->characterId] - D_8018D710[arg3][arg4],
                  arg2 + -3.2 + (sp6C * 1), -D_8018D620[arg3][arg4], -player->unk_206 * 2);
    if ((gPlayerBalloonStatus[arg3][arg4] & 2) != 2) {
        gPlayerBalloonPosY[arg3][arg4] = (player->pos[1] - player->boundingBoxSize) + someY;
        gPlayerBalloonPosX[arg3][arg4] = player->pos[0] + someX;
        gPlayerBalloonPosZ[arg3][arg4] = player->pos[2] + someZ;
        gPlayerBalloonDepartingTimer[arg3][arg4] = 0;
    } else {
        gPlayerBalloonPosY[arg3][arg4] += 0.2;
        gPlayerBalloonDepartingTimer[arg3][arg4] += 1;
        move_s16_towards(&D_8018D890[arg3][arg4], 0, 0.1f);
        move_s16_towards(&D_8018D860[arg3][arg4], 0, 0.1f);
        if (gPlayerBalloonDepartingTimer[arg3][arg4] >= 0x78) {
            set_player_balloon_to_gone((s32) player, arg3, arg4);
        }
    }
}

void render_battle_balloon(Player* player, s8 arg1, s16 arg2, s8 arg3) {
    Mat4 mtx;
    Vec3f sp134;
    Vec3s sp12C;
    UNUSED s16 stackPadding;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s16 temp_t1;
    f32 xdiff;
    f32 zdiff;
    f32 var_f20;
    s32 primColors[] = {
        MAKE_RGB(0xC8, 0x01, 0x00), MAKE_RGB(0x00, 0x70, 0x01), MAKE_RGB(0x10, 0x79, 0x51), MAKE_RGB(0x00, 0x59, 0x70),
        MAKE_RGB(0x70, 0x55, 0x00), MAKE_RGB(0x7A, 0x7E, 0x00), MAKE_RGB(0x77, 0x2C, 0x24), MAKE_RGB(0x30, 0x14, 0x58),
    };
    s32 envColors[] = {
        MAKE_RGB(0xDC, 0x00, 0x00), MAKE_RGB(0x00, 0x8C, 0x06), MAKE_RGB(0x00, 0x00, 0x51), MAKE_RGB(0x00, 0x00, 0x00),
        MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00),
    };

    primRed = (primColors[player->characterId] >> 0x10) & 0xFF;
    primGreen = (primColors[player->characterId] >> 0x08) & 0xFF;
    primBlue = (primColors[player->characterId] >> 0x00) & 0xFF;
    envRed = (envColors[player->characterId] >> 0x10) & 0xFF;
    envGreen = (envColors[player->characterId] >> 0x08) & 0xFF;
    envBlue = (envColors[player->characterId] >> 0x00) & 0xFF;
    temp_t1 = (((player->unk_048[arg3] + player->rotation[1] + player->unk_0C0) & 0xFFFF) / 128);
    temp_t1 <<= 7;
    if (arg3 == arg1) {
        var_f20 = 0.3f;
    } else {
        // wut?
        xdiff = (var_f20 = player->pos[0] - cameras[arg3].pos[0]);
        zdiff = player->pos[2] - cameras[arg3].pos[2];
        if (gActiveScreenMode != 3) {
            var_f20 = sqrtf((xdiff * xdiff) + (zdiff * zdiff)) / 300.0f;
        } else {
            var_f20 = sqrtf((xdiff * xdiff) + (zdiff * zdiff)) / 200.0f;
        }
        if (var_f20 >= 1.8) {
            var_f20 = 1.8f;
        }
        if (var_f20 <= 0.3) {
            var_f20 = 0.3f;
        }
    }
    sp134[0] = gPlayerBalloonPosX[arg1][arg2];
    sp134[1] = gPlayerBalloonPosY[arg1][arg2];
    sp134[2] = gPlayerBalloonPosZ[arg1][arg2];
    sp12C[0] = -((D_8018D890[arg1][arg2] * 4) * coss(temp_t1));
    sp12C[1] = player->unk_048[arg3];
    sp12C[2] = D_8018D7D0[arg1][arg2] - (D_8018D860[arg1][arg2] * coss(temp_t1)) -
               ((D_8018D890[arg1][arg2] * 8) * sins(temp_t1));

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild((uintptr_t) player, arg1 | arg2 << 16);

    mtxf_translate_rotate(mtx, sp134, sp12C);
    mtxf_scale(mtx, var_f20);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], sp140);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);

    func_8004B614(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(gDisplayListHead++, gTextureBalloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane1, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(gDisplayListHead++, gTextureBalloon2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane2, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();

    gMatrixEffectCount++;
}

void init_all_player_balloons(Player* player, s8 arg1) {
    init_balloon(player, 0.0f, 0.0f, arg1, (s8) 0, (s16) 0);
    init_balloon(player, 1.5f, 2.0f, arg1, (s8) 1, (s16) 0x1C70);
    init_balloon(player, -1.5f, 2.0f, arg1, (s8) 2, (s16) -0x1C70);
    gPlayerBalloonCount[arg1] = 2;
}

void clear_all_player_balloons(UNUSED Player* player, s8 playerIndex) {
    gPlayerBalloonStatus[playerIndex][0] = BALLOON_STATUS_GONE;
    gPlayerBalloonStatus[playerIndex][1] = BALLOON_STATUS_GONE;
    gPlayerBalloonStatus[playerIndex][2] = BALLOON_STATUS_GONE;
}

void pop_player_balloon(Player* player, s8 playerIndex) {
    if (gPlayerBalloonCount[playerIndex] >= 0) {
        gPlayerBalloonStatus[playerIndex][gPlayerBalloonCount[playerIndex]] &= ~1;
        gPlayerBalloonStatus[playerIndex][gPlayerBalloonCount[playerIndex]] |= 2;
        gPlayerBalloonCount[playerIndex]--;
        func_800C9060(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x51));
        if (gPlayerBalloonCount[playerIndex] < 0) {
            func_8008FD4C(player, playerIndex);
        }
    }
}

void set_player_balloon_to_gone(UNUSED s32 arg0, s8 playerIndex, s8 balloonIndex) {
    if (gPlayerBalloonCount[playerIndex] >= 0) {
        gPlayerBalloonStatus[playerIndex][balloonIndex] = BALLOON_STATUS_GONE;
    }
}

void func_8006B9CC(Player* player, s8 arg1) {
    if (gPlayerBalloonStatus[arg1][0] != 0) {
        func_8006A7C0(player, 0.0f, 0.0f, arg1, 0);
    }

    if (gPlayerBalloonStatus[arg1][1] != 0) {

        func_8006A7C0(player, 1.8f, 2.6f, arg1, 1);
    }

    if (gPlayerBalloonStatus[arg1][2] != 0) {
        func_8006A7C0(player, -1.8f, 2.6f, arg1, 2);
    }
}

void func_8006BA94(Player* player, s8 playerIndex, s8 arg2) {
    if (gPlayerBalloonStatus[playerIndex][0] != BALLOON_STATUS_GONE) {
        render_battle_balloon(player, playerIndex, 0, arg2);
    }
    if (gPlayerBalloonStatus[playerIndex][1] != BALLOON_STATUS_GONE) {
        render_battle_balloon(player, playerIndex, 1, arg2);
    }
    if (gPlayerBalloonStatus[playerIndex][2] != BALLOON_STATUS_GONE) {
        render_battle_balloon(player, playerIndex, 2, arg2);
    }
}

/**
 * Used in podium ceremony.
 */
void render_balloon(Vec3f arg0, f32 arg1, s16 arg2, s16 arg3) {
    Mat4 mtx;
    Vec3f spFC;
    Vec3s spF4;
    UNUSED s16 stackPadding;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s32 primColors[] = {
        MAKE_RGB(0xC8, 0x01, 0x00), MAKE_RGB(0x00, 0x70, 0x01), MAKE_RGB(0x10, 0x79, 0x51), MAKE_RGB(0x00, 0x59, 0x70),
        MAKE_RGB(0x70, 0x55, 0x00), MAKE_RGB(0x7A, 0x7E, 0x00), MAKE_RGB(0x77, 0x2C, 0x24), MAKE_RGB(0x30, 0x14, 0x58),
    };
    s32 envColors[] = {
        MAKE_RGB(0xDC, 0x00, 0x00), MAKE_RGB(0x00, 0x8C, 0x06), MAKE_RGB(0x00, 0x00, 0x51), MAKE_RGB(0x00, 0x00, 0x00),
        MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00),
    };

    primRed = (primColors[arg3] >> 0x10) & 0xFF;
    primGreen = (primColors[arg3] >> 0x08) & 0xFF;
    primBlue = (primColors[arg3] >> 0x00) & 0xFF;
    envRed = (envColors[arg3] >> 0x10) & 0xFF;
    envGreen = (envColors[arg3] >> 0x08) & 0xFF;
    envBlue = (envColors[arg3] >> 0x00) & 0xFF;
    spFC[0] = arg0[0];
    spFC[1] = arg0[1];
    spFC[2] = arg0[2];
    spF4[0] = 0;
    spF4[1] = camera1->rot[1];
    spF4[2] = arg2;
    mtxf_translate_rotate(mtx, spFC, spF4);
    mtxf_scale(mtx, arg1);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], sp108);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(gDisplayListHead++, gTLUTOnomatopoeia);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    func_8004B614(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(gDisplayListHead++, gTextureBalloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane1, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    // D_8018D4C0 is correct. But interestingly, IDO seems to set "-0x40" to a different register so the texture still
    // looks fine.
    gDPLoadTextureBlock(gDisplayListHead++, gTextureBalloon2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane2, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gMatrixEffectCount += 1;
}

void func_8006C0C8(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha) {
    Vec3f sp4C;
    Vec3s sp44;
    s16 red;
    s16 green;
    s16 blue;

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = camera1->rot[1];
    sp44[2] = 0;
    func_800652D4(sp4C, sp44, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    // ???????????????????????????????????
    red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????
    func_8004B35C(red, green, blue, alpha);
    gSPDisplayList(gDisplayListHead++, D_0D008E48);
    gMatrixEffectCount += 1;
}

void func_8006C294(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha) {
    Vec3f sp5C;
    Vec3s sp54;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp5C[0] = arg0[0];
    sp5C[1] = arg0[1];
    sp5C[2] = arg0[2];
    sp54[0] = 0;
    sp54[1] = camera1->rot[1];
    sp54[2] = 0;
    func_800652D4(sp5C, sp54, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, gTexture69C80C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    func_8004B35C(red, green, blue, alpha);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
    gSPDisplayList(gDisplayListHead++, D_0D008DA0);
    gMatrixEffectCount += 1;
}

void func_8006C4D4(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha, s16 arg4) {
    Vec3f sp4C;
    Vec3s sp44;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = camera1->rot[1];
    sp44[2] = 0;
    func_800652D4(sp4C, sp44, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark[arg4], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    func_8004B414(red, green, blue, alpha);
    gSPDisplayList(gDisplayListHead++, D_0D008E48);
    gMatrixEffectCount += 1;
}

void func_8006C6AC(Player* player, s16 particleIndex, s8 arg2, s8 arg3) {
    s8 arg2_copy = arg2;
    s32 sp28;

    sp28 = particleIndex - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (player->particlePool1[particleIndex].isAlive == 1) {
        switch (player->particlePool1[particleIndex].type) {
            case 1:
                func_80063408(player, particleIndex, arg2_copy, arg3);
                break;
            case 2:
                func_800635D4(player, particleIndex, arg2_copy, arg3);
                break;
            case 3:
                func_80063BD4(player, particleIndex, arg2_copy, arg3);
                break;
            case 4:
                func_800643A8(player, particleIndex, arg2_copy, arg3);
                break;
            case 5:
                func_800639DC(player, particleIndex, arg2_copy, arg3);
                break;
            case 9:
                func_80063D58(player, particleIndex, arg2_copy, arg3);
                break;
            case 11:
                func_80062F98(player, particleIndex, arg2_copy, arg3);
                break;
            default:
                break;
        }
    } else {
        if (player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) {
            func_80060BCC(player, particleIndex, sp28, arg2_copy, arg3);
        } else if (!(player->effects & 8) && !(player->effects & 2)) {
            if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) &&
                ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN)) {
                check_drift_particles_setup_valid(player, particleIndex, sp28, arg2_copy, arg3);
            } else if (((f64) (gPlayerWaterLevel[arg2_copy] - player->tyres[BACK_RIGHT].baseHeight) >= 3.5) ||
                       ((f64) (gPlayerWaterLevel[arg2_copy] - player->tyres[BACK_LEFT].baseHeight) >= 3.5)) {
                func_8005EA94(player, particleIndex, sp28, arg2_copy, arg3);
            } else if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
                func_8005F90C(player, particleIndex, sp28, arg2_copy, arg3);
            } else if (((player->effects & 0x4000) && !(player->type & PLAYER_START_SEQUENCE)) ||
                       (player->effects & 0x800) || (player->effects & 0x20) || (player->unk_044 & 0x4000)) {
                func_8005ED48(player, particleIndex, sp28, arg2_copy, arg3);
            } else {
                setup_tyre_particles(player, particleIndex, sp28, arg2_copy, arg3);
            }
        }
    }
}

void func_8006C9B8(Player* player, s16 arg1, s8 arg2, s8 arg3) {
    UNUSED s32 stackPadding;
    s32 sp28;
    sp28 = arg1 - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (player->particlePool3[arg1].isAlive == 1) {
        switch (player->particlePool3[arg1].type) {
            case 1:
                func_800644E8(player, arg1, arg2, arg3);
                break;

            case 2:
                func_800649F4(player, arg1, arg2, arg3);
                break;

            case 3:
                func_80064C74(player, arg1, arg2, arg3);
                break;

            case 4:
                func_800647C8(player, arg1, arg2, arg3);
                break;

            case 5:
                func_80064B30(player, arg1, arg2, arg3);
                break;

            case 6:
                func_800648E4(player, arg1, arg2, arg3);
                break;

            case 7:
                func_80064988(player, arg1, arg2, arg3);
                break;

            case 8:
                func_80064C74(player, arg1, arg2, arg3);
                break;

            case 9:
                func_80064664(player, arg1, arg2, arg3);
                break;

            default:
                break;
        }
    } else {
        if (player->unk_044 & 0x1000) {
            func_80061430(player, arg1, sp28, arg2, arg3);
            player->unk_044 &= ~0x0100;
            return;
        }
        if (((((player->unk_0CA & 0x1000) == 0x1000) || ((player->unk_0E0 < 2) && (player->effects & 0x01000000))) ||
             ((player->unk_0E0 < 2) && (player->effects & HIT_BY_ITEM_EFFECT))) ||
            (player->effects & 0x400)) {
            func_8006199C(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
            player->unk_044 &= ~0x0100;
            return;
        }
        if ((player->unk_0CA & 0x2000) == 0x2000) {
            func_80061A34(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
            player->unk_044 &= ~0x0100;
            return;
        }
        if ((player->effects & STAR_EFFECT) && ((((s32) gCourseTimer) - gPlayerStarEffectStartTime[arg2]) < 9)) {
            func_800615AC(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
            player->unk_044 &= ~0x0100;
            return;
        }
        if ((player->unk_046 & 8) == 8) {
            func_800612F8(player, arg1, sp28, arg2, arg3);
            player->unk_044 &= ~0x0100;
            return;
        }
        if (((player->unk_046 & 0x20) == 0x20) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            func_80061D4C(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
            player->unk_044 &= ~0x0100;
            return;
        }
        if ((player->effects & BOOST_EFFECT) && (player->type & PLAYER_HUMAN)) {
            func_800621BC(player, arg1, sp28, arg2, arg3);
            return;
        }
        if (((player->effects & 0x200000) || (player->effects & BOOST_RAMP_ASPHALT_EFFECT)) &&
            ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN)) {
            func_80061EF4(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
            player->unk_044 &= ~0x0100;
            return;
        }
        if ((player->unk_044 & 0x100) == 0x100) {
            func_800624D8(player, arg1, sp28, arg2, arg3);
            player->unk_046 &= ~0x0008;
        }
    }
}

void func_8006CEC0(Player* player, s16 arg1, s8 arg2, s8 arg3) {
    UNUSED u16 temp_v0_3;
    s32 sp20 = arg1;
    if (--sp20 < 0) {
        sp20 = 9;
    }
    if (player->particlePool0[arg1].isAlive == 1) {
        switch (player->particlePool0[arg1].type) {
            case 1:
                func_80062C74(player, arg1, arg2, arg3);
                break;
            case 3:
                func_80064184(player, arg1, arg2, arg3);
                break;
            case 5:
                set_oob_splash_particle_position(player, arg1, arg2, arg3);
                break;
            case 6:
                func_800631A8(player, arg1, arg2, arg3);
                break;
            case 7:
                func_80063268(player, arg1, arg2, arg3);
                break;
        }
    } else {
        if ((player->unk_044 & 0x200) && (player->type & 0x4000)) {
            func_80061224(player, arg1, sp20, arg2, arg3);
            return;
        } else if (((player->effects & 0x40000000) == 0x40000000) && (player->unk_0B0 < 0x32)) {
            func_80061094(player, arg1, sp20, arg2, arg3);
            return;
        } else if ((player->type & 0x4000) == 0x4000) {
            if ((player->waterInteractionFlags & WATER_JUST_ENTERED_DEEP_LIQUID) == WATER_JUST_ENTERED_DEEP_LIQUID) {
                func_80060F50(player, arg1, sp20, arg2, arg3);
                return;
            } else if ((player->waterInteractionFlags & WATER_IS_PARTIALLY_SUBMERGED) || (player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED)) {
                func_80060B14(player, arg1, sp20, arg2, arg3);
                return;
            }
        }
        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                if (((player->effects & 0x04000000) != 0x04000000) && ((player->effects & 0x400) != 0x400) &&
                    ((player->effects & 0x01000000) != 0x01000000)) {
                    if (((player->unk_0CA & 2) != 2) && ((player->unk_0CA & 0x10) != 0x10) && !(player->unk_0CA & 0x100)) {
                        func_80060504(player, arg1, sp20, arg2, arg3);
                    }
                }
                break;
            default:
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (((player->type & 0x4000) != 0) && ((player->effects & 0x04000000) != 0x04000000) &&
                    ((player->effects & 0x400) != 0x400) && ((player->effects & 0x01000000) != 0x01000000)) {
                    if (((player->unk_0CA & 2) != 2) && ((player->unk_0CA & 0x10) != 0x10) && !(player->unk_0CA & 0x100)) {
                        func_80060504(player, arg1, sp20, arg2, arg3);
                    }
                }
                break;
        }
    }
}

void func_8006D194(Player* player, s8 arg1, s8 arg2) {
    if (player->particlePool0[0x14].isAlive == 1) {
        switch (player->particlePool0[0x14].type) {
            case 2:
                func_80064DEC(player, arg1, arg2, 0);
                break;
            case 3:
                func_800650FC(player, arg1, arg2, 0);
                break;
            case 4:
                func_80064EA4(player, arg1, arg2, 0);
                break;
            case 5:
                func_80064F88(player, arg1, arg2, 0);
                break;
            case 6:
                func_80065030(player, arg1, arg2, 0);
                break;
        }
    } else {
        if ((player->unk_0B6 & 0x40) == 0x40) {
            func_800628C0(player, arg1, arg2, 0);
        }
        if ((player->unk_0B6 & 0x800) == 0x800) {
            func_80062968(player, arg1, arg2, 0);
        }
        if ((player->unk_0B6 & 0x1000) == 0x1000) {
            func_80062914(player, arg1, arg2, 0);
        }
        if ((player->unk_0B6 & 0x80) == 0x80) {
            func_80062A18(player, arg1, arg2, 0);
        }
        if ((player->unk_0B6 & 0x100) == 0x100) {
            func_800629BC(player, arg1, arg2, 0);
        }
    }
    if (player->particlePool0[0x15].isAlive == 1) {
        if (player->particlePool0[0x15].type == 5) {
            func_800651F4(player, arg1, arg2, 1);
        }
    } else if ((player->unk_0B6 & 0x20) == 0x20) {
        func_80062AA8(player, arg1, arg2, 1);
    }
}

void func_8006D474(Player* player, s8 playerId, s8 screenId) {
    s16 var_s2;
    if ((player->unk_002 & (8 << (screenId * 4))) == (8 << (screenId * 4))) {
        for (var_s2 = 0; var_s2 < 10; var_s2++) {
            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("SmokeDust", TAG_SMOKE_DUST((playerId << 8) + var_s2));
            switch (player->particlePool0[var_s2].type) {
                case 1:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_8006538C(player, playerId, var_s2, screenId);
                        }
                    } else {
                        func_8006538C(player, playerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_80066BAC(player, playerId, var_s2, screenId);
                        }
                    } else if (screenId == playerId) {
                        func_80066BAC(player, playerId, var_s2, screenId);
                    }
                    break;
            }
            FrameInterpolation_RecordCloseChild();
            FrameInterpolation_RecordOpenChild("SmokeDust", TAG_SMOKE_DUST((playerId << 8) + var_s2 + 30));
            switch (player->particlePool3[var_s2].type) {
                case 1:
                case 9:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_800691B8(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_800691B8(player, playerId, var_s2, screenId);
                    }
                    break;
                case 2:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        render_wall_bonk_star_particles(player, playerId, var_s2, screenId,
                                      player->particlePool3[var_s2].scale);
                    } else if (screenId == playerId) {
                        render_wall_bonk_star_particles(player, playerId, var_s2, screenId,
                                      player->particlePool3[var_s2].scale);
                    }
                    break;
                case 3:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80067280(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_80067280(player, (s32) playerId, var_s2, screenId);
                    }
                    break;
                case 4:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069444(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_80069444(player, playerId, var_s2, screenId);
                    }
                    break;
                case 5:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069938(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_80069938(player, playerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069BA8(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_80069BA8(player, playerId, var_s2, screenId);
                    }
                    break;
                case 7:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069DB8(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        func_80069DB8(player, playerId, var_s2, screenId);
                    }
                    break;
                case 8:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        render_player_boost_spark_particles(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        render_player_boost_spark_particles(player, playerId, var_s2, screenId);
                    }
                    break;
            }
            FrameInterpolation_RecordCloseChild();
            FrameInterpolation_RecordOpenChild("SmokeDust", TAG_SMOKE_DUST((playerId << 8) + var_s2 + 10));
            switch (player->particlePool1[var_s2].type) {
                case DRIFT_PARTICLE:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            render_player_drift_particles(player, playerId, var_s2, screenId);
                        }
                    } else {
                        render_player_drift_particles(player, playerId, var_s2, screenId);
                    }
                    break;
                case GROUND_PARTICLE:
                case GRASS_PARTICLE:
                case 4:
                case 5:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            render_player_ground_particles(player, playerId, var_s2, screenId);
                        }
                    } else {
                        render_player_ground_particles(player, playerId, var_s2, screenId);
                    }
                    break;
                case 9:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_800664E0(player, (s32) playerId, var_s2, screenId);
                        }
                    } else {
                        func_800664E0(player, (s32) playerId, var_s2, screenId);
                    }
                    break;
                case 11:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_8006A01C(player, playerId, var_s2, screenId);
                        }
                    } else if (screenId == playerId) {
                        func_8006A01C(player, playerId, var_s2, screenId);
                    }
                    break;
            }
            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
    }
    if ((gModeSelection == BATTLE) && (player->unk_002 & (2 << (screenId * 4)))) {
        func_8006BA94(player, playerId, screenId);
    }
}

void func_8006DC54(Player* player, s8 arg1, s8 arg2) {
    s16 i;
    s32 bitwiseMask;

    bitwiseMask = 8 << (arg2 * 4);
    if (bitwiseMask == (player->unk_002 & bitwiseMask)) {
        for (i = 0; i < 10; i++) {
            if (player->particlePool0[i].type == 7) {
                func_800658A0(player, arg1, i, arg2);
            }
        }
    }
}

void func_8006DD3C(Player* player, s8 playerId, s8 arg2) {
    s16 temp_s0;
    s32 temp_v0;

    temp_v0 = 8 << (arg2 * 4);
    if (temp_v0 == (player->unk_002 & temp_v0)) {
        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            temp_v0 = player->particlePool0[temp_s0].type;
            if (temp_v0 != 3) {
                if (temp_v0 == 5) {
                    func_8006A280(player, playerId, temp_s0, arg2);
                }
            } else if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                if (arg2 == playerId) {
                    func_80066998(player, playerId, temp_s0, arg2);
                }
            } else {
                func_80066998(player, playerId, temp_s0, arg2);
            }
        }

        if (((player->type & 0x4000) == 0x4000) && (arg2 == playerId)) {
            FrameInterpolation_RecordOpenChild("onomatopoeia", TAG_SMOKE_DUST((playerId << 8) + 20));
            switch (player->particlePool2[0].type) {
                case 2:
                    render_player_onomatopoeia_crash(player, playerId, player->particlePool2[0].scale, arg2, 0);
                    break;
                case 3:
                    render_player_onomatopoeia_whrrrr(player, playerId, player->particlePool2[0].scale, arg2, 0);
                    break;
                case 4:
                    func_80068724(player, playerId, player->particlePool2[0].scale, arg2, 0);
                    break;
                case 5:
                    render_player_onomatopoeia_boing(player, playerId, player->particlePool2[0].scale, arg2, 0);
                    break;
                case 6:
                    render_player_onomatopoeia_pomp(player, playerId, player->particlePool2[0].scale, arg2, 0);
                    break;
            }
            FrameInterpolation_RecordCloseChild();
            if (player->particlePool2[1].type == 5) {
                render_player_speech_bubble(player, arg2, D_8018D480, 1, 1.6f, 0xFFFFFF);
                render_music_note(player, arg2, D_8018D484, 1, 1.6f, 0xFF);
            }
        }
    }
}

void func_8006E058(void) {
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            switch (gModeSelection) {
                case GRAND_PRIX:
                    func_8006E420(gPlayerOne, 0, 0);
                    func_8006E420(gPlayerTwo, 1, 0);
                    func_8006E420(gPlayerThree, 2, 0);
                    func_8006E420(gPlayerFour, 3, 0);
                    func_8006E420(gPlayerFive, 4, 0);
                    func_8006E420(gPlayerSix, 5, 0);
                    func_8006E420(gPlayerSeven, 6, 0);
                    func_8006E420(gPlayerEight, 7, 0);

                    break;
                case TIME_TRIALS:
                    func_8006E420(gPlayerOne, 0, 0);

                    if ((gPlayerTwo->type & 0x100) == 0x100) {
                        func_8006E420(gPlayerTwo, 1, 0);
                    }

                    if ((gPlayerThree->type & 0x100) == 0x100) {
                        func_8006E420(gPlayerThree, 2, 0);
                        break;
                    }

                    break;
                case VERSUS:
                case BATTLE:
                    func_8006E420(gPlayerOne, 0, 0);
                    func_8006E420(gPlayerTwo, 1, 0);

                    if (gPlayerCountSelection1 >= 3) {
                        func_8006E420(gPlayerThree, 2, 0);
                    }

                    if (gPlayerCountSelection1 == 4) {
                        func_8006E420(gPlayerFour, 3, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (gModeSelection) {
                case GRAND_PRIX:
                    func_8006E420(gPlayerOne, 0, 0);
                    func_8006E420(gPlayerTwo, 1, 0);
                    func_8006E420(gPlayerThree, 2, 0);
                    func_8006E420(gPlayerFour, 3, 0);
                    func_8006E420(gPlayerFive, 4, 0);
                    func_8006E420(gPlayerSix, 5, 0);
                    func_8006E420(gPlayerSeven, 6, 0);
                    func_8006E420(gPlayerEight, 7, 0);

                    break;
                case VERSUS:
                case BATTLE:
                    func_8006E420(gPlayerOne, 0, 0);
                    func_8006E420(gPlayerTwo, 1, 0);

                    break;
                case TIME_TRIALS:
                    func_8006E420(gPlayerOne, 0, 0);

                    if ((gPlayerTwo->type & 0x8000) == 0x8000) {
                        func_8006E420(gPlayerTwo, 1, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == gModeSelection) || (BATTLE == gModeSelection)) {
                func_8006E420(gPlayerOne, 0, 0);
                func_8006E420(gPlayerTwo, 1, 0);
                func_8006E420(gPlayerThree, 2, 0);

                if (gPlayerCountSelection1 == 4) {
                    func_8006E420(gPlayerFour, 3, 0);
                }
            }

            break;
    }
}

void func_8006E420(Player* player, s8 arg1, s8 arg2) {
    s16 temp_s0;

    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
            func_8006D194(player, arg1, arg2);
        }

        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            func_8006CEC0(player, temp_s0, arg1, arg2);
            if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) || (gGamestate == ENDING)) {
                func_8006C9B8(player, temp_s0, arg1, arg2);
            }
            func_8006C6AC(player, temp_s0, arg1, arg2);
        }

        if (gModeSelection == BATTLE) {
            func_8006B9CC(player, arg1);
        }
    }
}

void render_kart_particles(Player* player, s8 playerId, s8 screenId) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (playerId == screenId) {
                func_8006D474(player, playerId, screenId);
            }
        } else {
            func_8006D474(player, playerId, screenId);
        }
        func_8006DC54(player, playerId, screenId);
    }
}

void func_8006E7CC(Player* player, s8 arg1, s8 arg2) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (arg1 == arg2) {
                func_8006DD3C(player, arg1, arg2);
            }
        } else {
            func_8006DD3C(player, arg1, arg2);
        }
    }
}

void func_8006E848(Player* player, s8 arg1, s8 arg2) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (arg1 == arg2) {
                func_8006DD3C(player, arg1, arg2);
            }
        } else {
            func_8006DD3C(player, arg1, arg2);
        }
    }
}

void func_8006E8C4(Player* player, s8 arg1, s8 arg2) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (arg1 == arg2) {
                func_8006DD3C(player, arg1, arg2);
            }
        } else {
            func_8006DD3C(player, arg1, arg2);
        }
    }
}

void func_8006E940(Player* player, s8 arg1, s8 arg2) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (arg1 == arg2) {
                func_8006DD3C(player, arg1, arg2);
            }
        } else {
            func_8006DD3C(player, arg1, arg2);
        }
    }
}

/***************************************************************/
/* Just a random 10 laying around in data, nothing to see here */
/***************************************************************/
s32 some_unused_data = 10;

#undef MAKE_RGB
