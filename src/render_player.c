#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include <stubs.h>
#include <common_structs.h>
#include <defines.h>
#include "code_800029B0.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
#include "main.h"
#include "decode.h"
#include "kart_dma.h"
#include "objects.h"
#include "render_player.h"
#include "code_80057C60.h"
#include "effects.h"
#include "buffers.h"
#include "waypoints.h"
#include "player_controller.h"
#include "render_objects.h"
#include <assets/models/common_data.h>
#include "skybox_and_splitscreen.h"
#include "spawn_players.h"
#include <assets/textures/karts/mario_kart.h>
#include <assets/textures/karts/luigi_kart.h>
#include <assets/textures/karts/peach_kart.h>
#include <assets/textures/karts/yoshi_kart.h>
#include <assets/textures/karts/toad_kart.h>
#include <assets/textures/karts/bowser_kart.h>
#include <assets/textures/karts/wario_kart.h>
#include <assets/textures/karts/donkeykong_kart.h>
#include "port/Game.h"
#include "engine/Matrix.h"
#include "port/interpolation/FrameInterpolation.h"
#include "port/Engine.h"

s8 gRenderingFramebufferByPlayer[] = { 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02 };

s32 gPlayersToRenderCount = 0;

// Can't find anything that actually references these...
UNUSED void* D_800DDB5C[3] = { gFramebuffer0, gFramebuffer1, gFramebuffer2 };

s16 gPlayersToRenderPlayerId[8];
s16 gPlayersToRenderScreenId[8];
Player* gPlayersToRenderPlayer[8];
s16 gMatrixEffectCount;
s32 D_80164AF4[3];
struct_D_802F1F80* gPlayerPalette;
static const char* sKartTexture;
u16 gPlayerRedEffect[8];
u16 gPlayerGreenEffect[8];
u16 gPlayerBlueEffect[8];
u16 gPlayerCyanEffect[8];
u16 gPlayerMagentaEffect[8];
u16 gPlayerYellowEffect[8];
// Likely an unused colour effect.
UNUSED u16 gPlayerWhiteEffect[8];
s32 D_80164B80[296];
s16 D_80165020[40];
Vec3f gPlayerLastVelocity[8];
s16 gLastAnimFrameSelector[4][8];
s16 gLastAnimGroupSelector[4][8];
s16 D_80165150[4][8];
s16 D_80165190[4][8];
s16 D_801651D0[4][8];

void func_8001F980(s32* arg0, s32* arg1) {
    if ((gDemoMode == 1) || (D_80164A28 != 0) || (D_8015F894 != 0)) {
        *arg0 = 0xAA;
    } else {
        *arg0 = 0;
    }
    if (D_80164A28 != 0) {
        *arg1 = 0xAA;
        return;
    }
    *arg1 = 0;
}

void func_8001F9E4(Player* player, Camera* camera, s8 screenId) {
    UNUSED s32 pad;
    s32 sp30;
    s32 sp2C;
    UNUSED s32 pad2;

    get_player_index_for_player(player);
    func_8001F980(&sp30, &sp2C);

    player->unk_002 &= ~(2 << (screenId * 4));
    player->unk_002 &= ~(8 << (screenId * 4));

    if (check_player_camera_collision(player, camera, (f32) (D_80165578 + sp30), (f32) (D_8016557A + sp2C)) == 1) {
        player->unk_002 |= 2 << (screenId * 4);
    }
    if (check_player_camera_collision(player, camera, (f32) D_80165580, (f32) D_80165582) == 1) {
        player->unk_002 |= 8 << (screenId * 4);
    }
}

u16 check_player_camera_collision(Player* player, Camera* camera, f32 arg2, f32 arg3) {
    UNUSED f32 pad[6];
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    s16 var_v0;
    u16 ret;

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        return true;
    }

    ret = false;
    switch (gActiveScreenMode) { /* irregular */
        case SCREEN_MODE_1P:
            var_v0 = 0x293C;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            var_v0 = 0x3058;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_v0 = 0x1FFE;
            break;
        default:
            if (1) {}
            var_v0 = 0x1FFE;
            break;
    }
    sp4C = (arg2 * coss((camera->rot[1] - var_v0))) + camera->pos[2];
    sp58 = (arg2 * sins((camera->rot[1] - var_v0))) + camera->pos[0];
    sp48 = (arg2 * coss((camera->rot[1] + var_v0))) + camera->pos[2];
    sp54 = (arg2 * sins((camera->rot[1] + var_v0))) + camera->pos[0];
    sp44 = (arg3 * coss((camera->rot[1] + 0x5FFA))) + camera->pos[2];
    sp50 = (arg3 * sins((camera->rot[1] + 0x5FFA))) + camera->pos[0];

    sp64 = ((sp4C - player->pos[2]) * (sp54 - player->pos[0])) - ((sp48 - player->pos[2]) * (sp58 - player->pos[0]));
    sp60 = ((sp48 - player->pos[2]) * (sp50 - player->pos[0])) - ((sp44 - player->pos[2]) * (sp54 - player->pos[0]));
    sp5C = ((sp44 - player->pos[2]) * (sp58 - player->pos[0])) - ((sp4C - player->pos[2]) * (sp50 - player->pos[0]));

    if (((sp64 >= 0) && (sp60 >= 0) && (sp5C >= 0)) || (((sp64) <= 0) && (sp60 <= 0) && (sp5C <= 0))) {
        ret = true;
    }
    return ret;
}

u16 func_8001FD78(Player* player, f32 posX, UNUSED f32 arg2, f32 posZ) {
    f32 sp64;
    f32 sp60;
    f32 sp5c;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 temp_f14;
    f32 thing0;
    f32 thing1;
    u16 ret;

    ret = false;

    sp58 = (70.0f * coss(((player->unk_0C0 - player->rotation[1]) - 0x71C))) + player->pos[2];
    sp64 = (70.0f * sins(((player->unk_0C0 - player->rotation[1]) - 0x71C))) + player->pos[0];
    sp54 = (70.0f * coss(((player->unk_0C0 - player->rotation[1]) + 0x71C))) + player->pos[2];
    sp60 = (70.0f * sins(((player->unk_0C0 - player->rotation[1]) + 0x71C))) + player->pos[0];
    sp50 = (10.0f * coss(((player->unk_0C0 - player->rotation[1]) + 0x1C70))) + player->pos[2];
    sp5c = (10.0f * sins(((player->unk_0C0 - player->rotation[1]) + 0x1C70))) + player->pos[0];

    temp_f14 = ((sp58 - posZ) * (sp60 - posX)) - ((sp54 - posZ) * (sp64 - posX));
    thing0 = ((sp54 - posZ) * (sp5c - posX)) - ((sp50 - posZ) * (sp60 - posX));
    thing1 = ((sp50 - posZ) * (sp64 - posX)) - ((sp58 - posZ) * (sp5c - posX));
    if (((temp_f14 >= 0) && (thing0 >= 0) && (thing1 >= 0)) || ((temp_f14 <= 0) && (thing0 <= 0) && (thing1 <= 0))) {
        ret = true;
    }
    return ret;
}

/**
 * @brief one of his function is to put the frame to load in to a list to load it to be only applied for the next frame
 */
void init_render_player(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    UNUSED s32 pad[4];
    s32 sp4C;
    s32 sp48;
    UNUSED s32 pad2;
    s32 temp_v0;
    s32 temp_v0_2;

    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        func_8001F9E4(player, camera, screenId);
        temp_v0 = 2 << (screenId << 2);
        if (temp_v0 == (player->unk_002 & temp_v0)) {
            if (!(player->type & PLAYER_START_SEQUENCE)) {
                func_8002934C(player, camera, screenId, playerId);
            } else {
                func_8002934C(player, camera, screenId, playerId);
                player->slopeAccel = 0;
                player->unk_206 = 0;
                player->unk_050[screenId] = 0;
            }
        }
        func_8001F980(&sp4C, &sp48);
        temp_v0_2 = 1 << (screenId << 2);
        if ((temp_v0 == (player->unk_002 & temp_v0)) && (temp_v0_2 == (player->unk_002 & temp_v0_2))) {
            if ((check_player_camera_collision(player, camera, D_80165570 + sp4C, D_80165572 + sp48) == 1) & 0xFFFF) {
                gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                gPlayersToRenderPlayer[gPlayersToRenderCount] = player;
                gPlayersToRenderCount += 1;
                D_80165190[screenId][playerId] = 0;
                gLastAnimFrameSelector[screenId][playerId] = player->animFrameSelector[screenId];
                gLastAnimGroupSelector[screenId][playerId] = player->animGroupSelector[screenId];
                D_80165150[screenId][playerId] = player->unk_0A8;
                D_801651D0[screenId][playerId] += 1;
                if (D_801651D0[screenId][playerId] == 2) {
                    D_801651D0[screenId][playerId] = 0;
                }
            } else {
                if ((check_player_camera_collision(player, camera, D_80165574 + sp4C, D_80165576) == 1) & 0xFFFF) {
                    if ((sRenderingFramebuffer == gRenderingFramebufferByPlayer[playerId]) ||
                        ((gLastAnimFrameSelector[screenId][playerId] - player->animFrameSelector[screenId]) > 0x13) ||
                        ((gLastAnimFrameSelector[screenId][playerId] - player->animFrameSelector[screenId]) < -0x13) ||
                        (D_80165190[screenId][playerId] == (s16) 1U)) {
                        gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                        gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                        gPlayersToRenderPlayer[gPlayersToRenderCount] = player;
                        gPlayersToRenderCount += 1;
                        gLastAnimFrameSelector[screenId][playerId] = player->animFrameSelector[screenId];
                        gLastAnimGroupSelector[screenId][playerId] = player->animGroupSelector[screenId];
                        D_80165150[screenId][playerId] = player->unk_0A8;
                        D_80165190[screenId][playerId] = 0;
                        D_801651D0[screenId][playerId] += 1;
                        if (D_801651D0[screenId][playerId] == 2) {
                            D_801651D0[screenId][playerId] = 0;
                        }
                    }
                } else {
                    if (((gLastAnimFrameSelector[screenId][playerId] - player->animFrameSelector[screenId]) > 0x13) ||
                        ((gLastAnimFrameSelector[screenId][playerId] - player->animFrameSelector[screenId]) < -0x13) ||
                        (D_80165190[screenId][playerId] == (s16) 1U)) {
                        gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                        gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                        gPlayersToRenderPlayer[gPlayersToRenderCount] = player;
                        gPlayersToRenderCount += 1;
                        gLastAnimFrameSelector[screenId][playerId] = (s16) player->animFrameSelector[screenId];
                        gLastAnimGroupSelector[screenId][playerId] = player->animGroupSelector[screenId];
                        D_80165150[screenId][playerId] = player->unk_0A8;
                        D_80165190[screenId][playerId] = 0;
                        D_801651D0[screenId][playerId] += 1;
                        if (D_801651D0[screenId][playerId] == 2) {
                            D_801651D0[screenId][playerId] = 0;
                        }
                    }
                }
            }
        }
    }
}

/**
 * This function used to be split for each player screen.
 * It is combined now.
 * 
 * Check buffers.c
 * // [nothing][screen][player]
 * ALIGNED8 struct_D_802DFB80 gEncodedKartTexture[2][2][8];
 * 
 * The buffer is sized for two screens. In 3P/4P mode, the player and screen indexes
 * need to be adjusted like so:
 * 
 * Screen 3 uses buffer[unused][screen 1][slots 5-8]
 * Screen 4 uses buffer[unused][screen 2][slots 5-8]
 * 
 * Due to this remapping, 3P/4P modes only support upto four racers.
 * 
 */
void load_kart_texture_and_render_kart_particles(s32 screenIdx) {
    s16 i;
    s32 screenOffset = (screenIdx >= 2) ? -2: 0;
    s32 playerOffset = (screenIdx >= 2) ? 4 : 0;

    load_kart_texture_non_blocking(
        gPlayersToRenderPlayer[0],
        gPlayersToRenderPlayerId[0] + playerOffset,
        gPlayersToRenderScreenId[0],
        gPlayersToRenderScreenId[0] + screenOffset,
        D_801651D0[gPlayersToRenderScreenId[0]][gPlayersToRenderPlayerId[0]]
    );

    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    for (i = 1; i < gPlayersToRenderCount; i++) {
        load_kart_texture_non_blocking(
            gPlayersToRenderPlayer[i],
            gPlayersToRenderPlayerId[i] + playerOffset,
            gPlayersToRenderScreenId[i],
            gPlayersToRenderScreenId[i] + screenOffset,
            D_801651D0[gPlayersToRenderScreenId[i]][gPlayersToRenderPlayerId[i]]
        );

        strcpy(
            D_802BFB80.arraySize8[D_801651D0[gPlayersToRenderScreenId[i - 1]]
                                 [gPlayersToRenderPlayerId[i - 1]]]
                                 [gPlayersToRenderScreenId[i - 1] + screenOffset]
                                 [gPlayersToRenderPlayerId[i - 1] + playerOffset]
            .pixel_index_array,
            gEncodedKartTexture[D_801651D0[gPlayersToRenderScreenId[i - 1]]
                               [gPlayersToRenderPlayerId[i - 1]]]
                               [gPlayersToRenderScreenId[i - 1] + screenOffset]
                               [gPlayersToRenderPlayerId[i - 1] + playerOffset]
            .unk_00
        );
    }

    strcpy(
        D_802BFB80.arraySize8[D_801651D0[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                             [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                             [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] + screenOffset]
                             [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + playerOffset]
        .pixel_index_array,
        gEncodedKartTexture[D_801651D0[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                           [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                           [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] + screenOffset]
                           [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + playerOffset]
        .unk_00
    );
}

void try_rendering_player(Player* player, s8 playerId, s8 screenIdx) {

    if (((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) && ((player->type & PLAYER_UNKNOWN_0x40) == 0)) {
        if ((player->unk_002 & 2 << (screenIdx * 4)) == 2 << (screenIdx * 4)) {
            render_player(player, playerId, screenIdx);
        }
    }
}

void render_players(Camera* camera, s32 screenIdx) {
    UNUSED s32 pad;
    UNUSED char* sp3C[8] = {
        "S_MARIO", "S_LUIZI", "S_YOSSY", "S_KINOP", "S_DONKY", "S_WARIO", "S_PEACH", "S_KUPPA",
    };
    UNUSED char* sp1C[8] = {
        "J_MARIO", "J_LUIZI", "J_YOSSY", "J_KINOP", "J_DONKY", "J_WARIO", "J_PEACH", "J_KUPPA",
    };

    gPlayersToRenderCount = 0;
    init_render_player(gPlayerOne, camera, PLAYER_ONE, screenIdx);
    init_render_player(gPlayerTwo, camera, PLAYER_TWO, screenIdx);
    init_render_player(gPlayerThree, camera, PLAYER_THREE, screenIdx);
    init_render_player(gPlayerFour, camera, PLAYER_FOUR, screenIdx);
    if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        init_render_player(gPlayerFive, camera, PLAYER_FIVE, screenIdx);
        init_render_player(gPlayerSix, camera, PLAYER_SIX, screenIdx);
        init_render_player(gPlayerSeven, camera, PLAYER_SEVEN, screenIdx);
        init_render_player(gPlayerEight, camera, PLAYER_EIGHT, screenIdx);
    }

    // This call moved here to sync kart texture and wheel texture tlut loading/rendering
    if (gPlayersToRenderCount != 0) {
        load_kart_texture_and_render_kart_particles(screenIdx);
    }

    try_rendering_player(gPlayerOne, PLAYER_ONE, screenIdx);
    try_rendering_player(gPlayerTwo, PLAYER_TWO, screenIdx);
    try_rendering_player(gPlayerThree, PLAYER_THREE, screenIdx);
    try_rendering_player(gPlayerFour, PLAYER_FOUR, screenIdx);
    if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        try_rendering_player(gPlayerFive, PLAYER_FIVE, screenIdx);
        try_rendering_player(gPlayerSix, PLAYER_SIX, screenIdx);
        try_rendering_player(gPlayerSeven, PLAYER_SEVEN, screenIdx);
        try_rendering_player(gPlayerEight, PLAYER_EIGHT, screenIdx);
    }

    // Old call which results in out of sync kart textures
    // load_kart_texture_and_render_kart_particle_on_screen_one();
    render_kart_particles(gPlayerOne, PLAYER_ONE, screenIdx);
    render_kart_particles(gPlayerTwo, PLAYER_TWO, screenIdx);
    render_kart_particles(gPlayerThree, PLAYER_THREE, screenIdx);
    render_kart_particles(gPlayerFour, PLAYER_FOUR, screenIdx);
    if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        render_kart_particles(gPlayerFive, PLAYER_FIVE, screenIdx);
        render_kart_particles(gPlayerSix, PLAYER_SIX, screenIdx);
        render_kart_particles(gPlayerSeven, PLAYER_SEVEN, screenIdx);
        render_kart_particles(gPlayerEight, PLAYER_EIGHT, screenIdx);
    }
    gPlayersToRenderCount = 0;
}

// Hello, you've found the location of the bulk of the data section for this file
// For reasons beyound human comprehension it MUST be placed somewhere below
// render_players_on_screen_one

s32 junk[] = { 0, 0, 0 };

Vtx* gPlayerVtx[] = { gPlayerOneVtx,  gPlayerTwoVtx, gPlayerThreeVtx, gPlayerFourVtx,
                      gPlayerFiveVtx, gPlayerSixVtx, gPlayerSevenVtx, gPlayerEightVtx };

f32 gCharacterSize[] = { MARIO_SIZE, LUIGI_SIZE, YOSHI_SIZE, TOAD_SIZE, DK_SIZE, WARIO_SIZE, PEACH_SIZE, BOWSER_SIZE };

u32 gKartMarioWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartMarioWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartLuigiWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartLuigiWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartBowserWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartBowserWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartToadWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartToadWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

// Stopped doing this properly here and just copy pasted the below hoping it would work

u32 gKartYoshiWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartYoshiWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartDKWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartDKWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartPeachWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartPeachWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

u32 gKartWarioWheels0[] = { 672, 588, 504, 420, 336, 252, 168, 84, 0 };
u32 gKartWarioWheels1[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

// Replace the below ptrs with the indexes into mario_kart_wheels
// u8 **gKartMarioWheels0[] = {
//     gKartMario168Wheel0, gKartMario147Wheel0, gKartMario126Wheel0, gKartMario105Wheel0,
//     gKartMario084Wheel0, gKartMario063Wheel0, gKartMario042Wheel0, gKartMario021Wheel0,
//     gKartMario000Wheel0
// };

// u8 **gKartMarioWheels1[] = {
//     gKartMario269Wheel0, gKartMario269Wheel0, gKartMario249Wheel0, gKartMario229Wheel0,
//     gKartMario229Wheel0, gKartMario229Wheel0, gKartMario209Wheel0, gKartMario189Wheel0,
//     gKartMario189Wheel0
// };

// u8 **gKartLuigiWheels0[] = {
//     gKartLuigi168Wheel0, gKartLuigi147Wheel0, gKartLuigi126Wheel0, gKartLuigi105Wheel0,
//     gKartLuigi084Wheel0, gKartLuigi063Wheel0, gKartLuigi042Wheel0, gKartLuigi021Wheel0,
//     gKartLuigi000Wheel0
// };

// u8 **gKartLuigiWheels1[] = {
//     gKartLuigi269Wheel0, gKartLuigi269Wheel0, gKartLuigi249Wheel0, gKartLuigi229Wheel0,
//     gKartLuigi229Wheel0, gKartLuigi229Wheel0, gKartLuigi209Wheel0, gKartLuigi189Wheel0,
//     gKartLuigi189Wheel0
// };

// u8 **gKartBowserWheels0[] = {
//     gKartBowser168Wheel0, gKartBowser147Wheel0, gKartBowser126Wheel0, gKartBowser105Wheel0,
//     gKartBowser084Wheel0, gKartBowser063Wheel0, gKartBowser042Wheel0, gKartBowser021Wheel0,
//     gKartBowser000Wheel0
// };

// u8 **gKartBowserWheels1[] = {
//     gKartBowser269Wheel0, gKartBowser269Wheel0, gKartBowser249Wheel0, gKartBowser229Wheel0,
//     gKartBowser229Wheel0, gKartBowser229Wheel0, gKartBowser209Wheel0, gKartBowser189Wheel0,
//     gKartBowser189Wheel0
// };

// u8 **gKartToadWheels0[] = {
//     gKartToad168Wheel0, gKartToad147Wheel0, gKartToad126Wheel0, gKartToad105Wheel0,
//     gKartToad084Wheel0, gKartToad063Wheel0, gKartToad042Wheel0, gKartToad021Wheel0,
//     gKartToad000Wheel0
// };

// u8 **gKartToadWheels1[] = {
//     gKartToad269Wheel0, gKartToad269Wheel0, gKartToad249Wheel0, gKartToad229Wheel0,
//     gKartToad229Wheel0, gKartToad229Wheel0, gKartToad209Wheel0, gKartToad189Wheel0,
//     gKartToad189Wheel0
// };

// u8 **gKartYoshiWheels0[] = {
//     gKartYoshi168Wheel0, gKartYoshi147Wheel0, gKartYoshi126Wheel0, gKartYoshi105Wheel0,
//     gKartYoshi084Wheel0, gKartYoshi063Wheel0, gKartYoshi042Wheel0, gKartYoshi021Wheel0,
//     gKartYoshi000Wheel0
// };

// u8 **gKartYoshiWheels1[] = {
//     gKartYoshi269Wheel0, gKartYoshi269Wheel0, gKartYoshi249Wheel0, gKartYoshi229Wheel0,
//     gKartYoshi229Wheel0, gKartYoshi229Wheel0, gKartYoshi209Wheel0, gKartYoshi189Wheel0,
//     gKartYoshi189Wheel0
// };

// u8 **gKartDKWheels0[] = {
//     gKartDK168Wheel0, gKartDK147Wheel0, gKartDK126Wheel0, gKartDK105Wheel0,
//     gKartDK084Wheel0, gKartDK063Wheel0, gKartDK042Wheel0, gKartDK021Wheel0,
//     gKartDK000Wheel0
// };

// u8 **gKartDKWheels1[] = {
//     gKartDK269Wheel0, gKartDK269Wheel0, gKartDK249Wheel0, gKartDK229Wheel0,
//     gKartDK229Wheel0, gKartDK229Wheel0, gKartDK209Wheel0, gKartDK189Wheel0,
//     gKartDK189Wheel0
// };

// u8 **gKartPeachWheels0[] = {
//     gKartPeach168Wheel0, gKartPeach147Wheel0, gKartPeach126Wheel0, gKartPeach105Wheel0,
//     gKartPeach084Wheel0, gKartPeach063Wheel0, gKartPeach042Wheel0, gKartPeach021Wheel0,
//     gKartPeach000Wheel0
// };

// u8 **gKartPeachWheels1[] = {
//     gKartPeach269Wheel0, gKartPeach269Wheel0, gKartPeach249Wheel0, gKartPeach229Wheel0,
//     gKartPeach229Wheel0, gKartPeach229Wheel0, gKartPeach209Wheel0, gKartPeach189Wheel0,
//     gKartPeach189Wheel0
// };

// u8 **gKartWarioWheels0[] = {
//     gKartWario168Wheel0, gKartWario147Wheel0, gKartWario126Wheel0, gKartWario105Wheel0,
//     gKartWario084Wheel0, gKartWario063Wheel0, gKartWario042Wheel0, gKartWario021Wheel0,
//     gKartWario000Wheel0
// };

// u8 **gKartWarioWheels1[] = {
//     gKartWario269Wheel0, gKartWario269Wheel0, gKartWario249Wheel0, gKartWario229Wheel0,
//     gKartWario229Wheel0, gKartWario229Wheel0, gKartWario209Wheel0, gKartWario189Wheel0,
//     gKartWario189Wheel0
// };

u32* gPaletteWheel0Offset[] = { gKartMarioWheels0, gKartLuigiWheels0, gKartYoshiWheels0, gKartToadWheels0,
                      gKartDKWheels0,    gKartWarioWheels0, gKartPeachWheels0, gKartBowserWheels0 };

u32* gPaletteWheel1Offset[] = { gKartMarioWheels1, gKartLuigiWheels1, gKartYoshiWheels1, gKartToadWheels1,
                      gKartDKWheels1,    gKartWarioWheels1, gKartPeachWheels1, gKartBowserWheels1 };

const char** wheelPtr[] = {
    mario_kart_wheels,      luigi_kart_wheels, yoshi_kart_wheels, toad_kart_wheels,
    donkeykong_kart_wheels, wario_kart_wheels, peach_kart_wheels, bowser_kart_wheels,
};

s32 D_800DDE74[] = { 96, 128, 192, 256, 288, 384, 512, 544, 576, 0, 0 };

void func_80021B0C(void) {
    func_8006E7CC(gPlayerOne, 0, 0);
    func_8006E7CC(gPlayerTwo, 1, 0);
    func_8006E7CC(gPlayerThree, 2, 0);
    func_8006E7CC(gPlayerFour, 3, 0);
    if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        func_8006E7CC(gPlayerFive, 4, 0);
        func_8006E7CC(gPlayerSix, 5, 0);
        func_8006E7CC(gPlayerSeven, 6, 0);
        func_8006E7CC(gPlayerEight, 7, 0);
    }
    if (gGamestate == ENDING) {
        if (gPlayerOne->unk_044 & 0x2000) {
            render_player_shadow_credits(gPlayerOne, 0, 0);
        }
        if (gPlayerTwo->unk_044 & 0x2000) {
            render_player_shadow_credits(gPlayerTwo, 1, 0);
        }
        if (gPlayerThree->unk_044 & 0x2000) {
            render_player_shadow_credits(gPlayerThree, 2, 0);
        }
        if (gPlayerFour->unk_044 & 0x2000) {
            render_player_shadow_credits(gPlayerFour, 3, 0);
        }
    }
}

void func_80021C78(void) {
    func_8006E848(gPlayerOne, 0, 1);
    func_8006E848(gPlayerTwo, 1, 1);
    func_8006E848(gPlayerThree, 2, 1);
    func_8006E848(gPlayerFour, 3, 1);
    if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        func_8006E848(gPlayerFive, 4, 1);
        func_8006E848(gPlayerSix, 5, 1);
        func_8006E848(gPlayerSeven, 6, 1);
        func_8006E848(gPlayerEight, 7, 1);
    }
}

void func_80021D40(void) {
    func_8006E8C4(gPlayerOne, 0, 2);
    func_8006E8C4(gPlayerTwo, 1, 2);
    func_8006E8C4(gPlayerThree, 2, 2);
    func_8006E8C4(gPlayerFour, 3, 2);
}

void func_80021DA8(void) {
    func_8006E940(gPlayerOne, 0, 3);
    func_8006E940(gPlayerTwo, 1, 3);
    func_8006E940(gPlayerThree, 2, 3);
    func_8006E940(gPlayerFour, 3, 3);
}

void mtxf_translate_rotate(Mat4 dest, Vec3f pos, Vec3s orientation) {
    UNUSED f32 pad[3];
    f32 sinX = sins(orientation[0]);
    f32 cosX = coss(orientation[0]);
    f32 sinY = sins(orientation[1]);
    f32 cosY = coss(orientation[1]);
    f32 sinZ = sins(orientation[2]);
    f32 cosZ = coss(orientation[2]);

    FrameInterpolation_RecordTranslateRotate(dest, pos, orientation);

    dest[0][0] = (cosY * cosZ) + ((sinX * sinY) * sinZ);
    dest[1][0] = (-cosY * sinZ) + ((sinX * sinY) * cosZ);
    dest[2][0] = cosX * sinY;
    dest[3][0] = pos[0];
    dest[0][1] = cosX * sinZ;
    dest[1][1] = cosX * cosZ;
    dest[2][1] = -sinX;
    dest[3][1] = pos[1];
    dest[0][2] = (-sinY * cosZ) + ((sinX * cosY) * sinZ);
    dest[1][2] = (sinY * sinZ) + ((sinX * cosY) * cosZ);
    dest[2][2] = cosX * cosY;
    dest[3][2] = pos[2];
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

UNUSED void func_80021F50(Mat4 arg0, Vec3f arg1) {
    arg0[3][0] += arg1[0];
    arg0[3][1] += arg1[1];
    arg0[3][2] += arg1[2];
}

/**
 * This function writes a fixed-point value to each Mtx entry. This is not how the Mtx struct works.
 * The first half of Mtx only holds s16 whole numbers and the second half holds the s16 decimal (fractional) parts.
 * See convert_to_fixed_point_matrix() for correct calculations. Note that each Mtx entry is the size of s32.
 * This means each Mtx entry holds two s16 values.
 * The first sixteen entries contain only the integer parts and the second sixteen entries hold only the decimal
 * (fractional) parts.
 */
UNUSED void failed_fixed_point_matrix_conversion(Mtx* dest, Mat4 src) {
    f32 toFixed = 65536.0f;
    dest->m[0][0] = src[0][0] * toFixed;
    dest->m[0][1] = src[0][1] * toFixed;
    dest->m[0][2] = src[0][2] * toFixed;
    dest->m[0][3] = src[0][3] * toFixed;
    dest->m[1][0] = src[1][0] * toFixed;
    dest->m[1][1] = src[1][1] * toFixed;
    dest->m[1][2] = src[1][2] * toFixed;
    dest->m[1][3] = src[1][3] * toFixed;
    dest->m[2][0] = src[2][0] * toFixed;
    dest->m[2][1] = src[2][1] * toFixed;
    dest->m[2][2] = src[2][2] * toFixed;
    dest->m[2][3] = src[2][3] * toFixed;
    dest->m[3][0] = src[3][0] * toFixed;
    dest->m[3][1] = src[3][1] * toFixed;
    dest->m[3][2] = src[3][2] * toFixed;
    dest->m[3][3] = src[3][3] * toFixed;
}

/**
 * Takes a floating-point matrix and converts it to an s15.16 internal matrix.
 * Each Mtx entry is a size of s32 that holds two values.
 * The first 16 entries hold only the integer values and the second 16 entries hold only the decimal (fractional) parts.
 * In simpler words, the integer and decimal gets split up and stored in their own section.
 * Mtx is setup this way due to hardware restrictions of the n64 or as an optimization.
 *
 * @param Mtx A new internal fixed-point matrix.
 * @param Mat4 An array of f32
 * Mat4 to Mtx explanation: https://blarg.ca/2020/10/11/fixed-point-math.
 */
void convert_to_fixed_point_matrix(Mtx* dest, Mat4 src) {
#ifdef AVOID_UB
    // Use os function guMtxF2L instead. This helps little-endian systems.
    FrameInterpolation_RecordMatrixMtxFToMtx((MtxF*)src, dest);
    guMtxF2L(src, dest);
#else
    f32 toFixed = 65536.0f; // 2 ^ 16
    dest->m[0][0] = ((s32) (src[0][0] * toFixed) & 0xFFFF0000) | (((s32) (src[0][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][1] = ((s32) (src[0][2] * toFixed) & 0xFFFF0000) | (((s32) (src[0][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][2] = ((s32) (src[1][0] * toFixed) & 0xFFFF0000) | (((s32) (src[1][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][3] = ((s32) (src[1][2] * toFixed) & 0xFFFF0000) | (((s32) (src[1][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][0] = ((s32) (src[2][0] * toFixed) & 0xFFFF0000) | (((s32) (src[2][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][1] = ((s32) (src[2][2] * toFixed) & 0xFFFF0000) | (((s32) (src[2][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][2] = ((s32) (src[3][0] * toFixed) & 0xFFFF0000) | (((s32) (src[3][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][3] = ((s32) (src[3][2] * toFixed) & 0xFFFF0000) | (((s32) (src[3][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[2][0] = ((s32) (src[0][0] * toFixed) << 0x10) | ((s32) (src[0][1] * toFixed) & 0xFFFF);
    dest->m[2][1] = ((s32) (src[0][2] * toFixed) << 0x10) | ((s32) (src[0][3] * toFixed) & 0xFFFF);
    dest->m[2][2] = ((s32) (src[1][0] * toFixed) << 0x10) | ((s32) (src[1][1] * toFixed) & 0xFFFF);
    dest->m[2][3] = ((s32) (src[1][2] * toFixed) << 0x10) | ((s32) (src[1][3] * toFixed) & 0xFFFF);
    dest->m[3][0] = ((s32) (src[2][0] * toFixed) << 0x10) | ((s32) (src[2][1] * toFixed) & 0xFFFF);
    dest->m[3][1] = ((s32) (src[2][2] * toFixed) << 0x10) | ((s32) (src[2][3] * toFixed) & 0xFFFF);
    dest->m[3][2] = ((s32) (src[3][0] * toFixed) << 0x10) | ((s32) (src[3][1] * toFixed) & 0xFFFF);
    dest->m[3][3] = ((s32) (src[3][2] * toFixed) << 0x10) | ((s32) (src[3][3] * toFixed) & 0xFFFF);
#endif
}

bool adjust_angle(s16* angle, s16 targetAngle, s16 step) {
    s16 temp_v0;

    temp_v0 = targetAngle - *angle;
    if (step < 0) { // abs of step
        step *= -1;
    }

    if (temp_v0 > 0) {
        temp_v0 -= step;
        if (temp_v0 >= 0) {
            *angle = targetAngle - temp_v0;
        } else {
            *angle = targetAngle;
        }
    } else {
        temp_v0 += step;
        if (temp_v0 <= 0) {
            *angle = targetAngle - temp_v0;
        } else {
            *angle = targetAngle;
        }
    }
    if (targetAngle == *angle) {
        return false;
    }
    return true;
}

void move_s32_towards(s32* startingValue, s32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

/**
  * Function: move_f32_towards

  * Parameters:
  *     f32 *startingValue - Pointer to the float that will be modified
  *     f32 targetValue    - Float value to move startingValue towards
  *     f32 somePercent    - The percent of the difference between startingValue
                            and targetValue to actually move

  * Moves a given startingValue the given somePercent towards the targetValue

  * f32 *thing = 2500;
  * move_f32_towards(thing, 500, 0.75f);
  * thing now has a value of 1000

  * If after the move startingValue is inside of the range [-0.001, 0.001],
  * force it to exactly 0.0f

  * This is probably a precision thing. The scaling with somePercent likely
  * can't hit exactly 0 with any reliability, so they force it to 0 if you're
  * in a small range around it. Why they only do this for 0 is anyone's guess though
**/
void move_f32_towards(f32* startingValue, f32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
    if ((*startingValue < 0.001) && (-0.001 < *startingValue)) {
        *startingValue = 0.0f;
    }
}

void move_s16_towards(s16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void move_u16_towards(u16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void func_80022744(void) {
    func_8006E058();
    func_8002276C();
}

void func_8002276C(void) {
    switch (gActiveScreenMode) { /* irregular */
        case SCREEN_MODE_1P:
            switch (gModeSelection) { /* switch 1; irregular */
                case GRAND_PRIX:      /* switch 1 */
                    func_80022A98(gPlayerOne, 0);
                    func_80022A98(gPlayerTwo, 1);
                    func_80022A98(gPlayerThree, 2);
                    func_80022A98(gPlayerFour, 3);
                    func_80022A98(gPlayerFive, 4);
                    func_80022A98(gPlayerSix, 5);
                    func_80022A98(gPlayerSeven, 6);
                    func_80022A98(gPlayerEight, 7);
                    break;
                case TIME_TRIALS: /* switch 1 */
                    func_80022A98(gPlayerOne, 0);
                    if ((gPlayerTwo->type & 0x100) == 0x100) {
                        func_80022A98(gPlayerTwo, 1);
                    }
                    if ((gPlayerThree->type & 0x100) == 0x100) {
                        func_80022A98(gPlayerThree, 2);
                    }
                    break;
                case VERSUS: /* switch 1 */
                case BATTLE: /* switch 1 */
                    func_80022A98(gPlayerOne, 0);
                    func_80022A98(gPlayerTwo, 1);
                    if (gPlayerCountSelection1 >= 3) {
                        func_80022A98(gPlayerThree, 2);
                    }
                    if (gPlayerCountSelection1 == 4) {
                        func_80022A98(gPlayerFour, 3);
                    }
                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (gModeSelection) { /* switch 2; irregular */
                case GRAND_PRIX:      /* switch 2 */
                    func_80022A98(gPlayerOne, 0);
                    func_80022A98(gPlayerTwo, 1);
                    func_80022A98(gPlayerThree, 2);
                    func_80022A98(gPlayerFour, 3);
                    func_80022A98(gPlayerFive, 4);
                    func_80022A98(gPlayerSix, 5);
                    func_80022A98(gPlayerSeven, 6);
                    func_80022A98(gPlayerEight, 7);
                    break;
                case VERSUS: /* switch 2 */
                case BATTLE: /* switch 2 */
                    func_80022A98(gPlayerOne, 0);
                    func_80022A98(gPlayerTwo, 1);
                    break;
                case TIME_TRIALS: /* switch 2 */
                    func_80022A98(gPlayerOne, 0);
                    break;
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == gModeSelection) || (BATTLE == gModeSelection)) {
                func_80022A98(gPlayerOne, 0);
                func_80022A98(gPlayerTwo, 1);
                func_80022A98(gPlayerThree, 2);
                if (gPlayerCountSelection1 == 4) {
                    func_80022A98(gPlayerFour, 3);
                }
            }
            break;
    }
}

void func_80022A98(Player* player, s8 arg1) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        func_80026A48(player, arg1);
        func_800235AC(player, arg1);
        if (((player->effects & HIT_EFFECT) == HIT_EFFECT) || ((player->effects & 0x08000000) == 0x08000000)) {
            if ((player->effects & HIT_EFFECT) == HIT_EFFECT) {
                func_80022B50(player, arg1);
            }
            if ((player->effects & 0x08000000) == 0x08000000) {
                func_80022BC4(player, arg1);
            }
        } else {
            func_80022DB4(player, arg1);
        }
        update_player_kart_hop(player);
    }
}

void func_80022B50(Player* player, UNUSED s8 arg1) {
    f32 temp_f0;
    s16 var_v0;

    var_v0 = player->unk_DB4.unk2;
    temp_f0 = player->unk_DB4.unk10;
    if (var_v0 < 5) {
        var_v0++;
    }

    player->unk_DA4 = (var_v0 * temp_f0) - (0.7 * (var_v0 * var_v0));
    player->unk_DB4.unk10 = temp_f0;
    player->unk_DB4.unk2 = var_v0;
}

void func_80022BC4(Player* player, UNUSED s8 arg1) {
    f32 temp_f0 = player->unk_DB4.unk10;
    s16 temp_v0 = player->unk_DB4.unk2;
    s16 temp_f16;

    temp_v0++;

    temp_f16 = (temp_v0 * temp_f0) - (0.5 * (temp_v0 * temp_v0));

    if ((temp_v0 != 0) && (temp_f16 < 0)) {
        temp_f0 *= 0.8;
        temp_v0 = 0;
        if (temp_f0 <= 0.1) {
            player->effects &= 0xF7FFFFFF;
            temp_f0 = 0.0f;
        }
    }
    if (temp_f16 <= 0) {
        temp_f16 = 0;
    }
    player->unk_DA4 = temp_f16;
    player->unk_DB4.unk10 = temp_f0;
    player->unk_DB4.unk2 = temp_v0;
}

void func_80022CA8(Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = player->unk_DA4;

    gPlayerVtx[playerId][flipOffset + 0x0].v.ob[1] = 18 - (temp_v0 * 2.3);
    gPlayerVtx[playerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x3].v.ob[1] = 18 - (temp_v0 * 2.3);
    gPlayerVtx[playerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Seems to stretch/warp a specific players texture for a
 * short period of time. Perhaps does not do anything
 **/
void func_80022D60(UNUSED Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    gPlayerVtx[playerId][flipOffset].v.ob[1] = 21;
    gPlayerVtx[playerId][flipOffset + 0x3].v.ob[1] = 21;
}

void func_80022DB4(Player* player, UNUSED s8 arg1) {
    f32 temp_f0 = player->unk_DB4.unkC;
    s16 temp_v0 = player->unk_DB4.unk18;
    s16 temp_f16;

    temp_v0++;

    temp_f16 = (temp_v0 * temp_f0) - (0.7 * (temp_v0 * temp_v0));

    if ((temp_v0 != 0) && (temp_f16 < 0)) {
        temp_f0 *= 0.8;
        temp_v0 = 0;
        if (temp_f0 <= 0.1) {
            temp_f0 = 0.0f;
        }
    }
    if (temp_f16 <= 0) {
        temp_f16 = 0;
    }
    player->unk_DB4.unk1E = temp_f16;
    player->unk_DB4.unkC = temp_f0;
    player->unk_DB4.unk18 = temp_v0;
}

void func_80022E84(Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = player->unk_DB4.unk1E;

    gPlayerVtx[playerId][flipOffset + 0x0].v.ob[1] = 18 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x3].v.ob[1] = 18 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    gPlayerVtx[playerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Sets player shading/colour.
 */
void change_player_color_effect_rgb(UNUSED Player* player, s8 arg1, s32 arg2, f32 alpha) {
    gPlayerRedEffect[arg1] =
        (s16) ((f32) gPlayerRedEffect[arg1] - ((gPlayerRedEffect[arg1] - ((arg2 >> 16) & 0xFF)) * alpha));

    gPlayerGreenEffect[arg1] =
        (s16) ((f32) gPlayerGreenEffect[arg1] - ((gPlayerGreenEffect[arg1] - ((arg2 >> 8) & 0xFF)) * alpha));

    gPlayerBlueEffect[arg1] =
        (s16) ((f32) gPlayerBlueEffect[arg1] - ((gPlayerBlueEffect[arg1] - (arg2 & 0xFF)) * alpha));
}

void change_player_color_effect_cmy(UNUSED Player* player, s8 arg1, s32 arg2, f32 arg3) {
    move_u16_towards(&gPlayerCyanEffect[arg1], (arg2 >> 16) & 0xFF, arg3);
    move_u16_towards(&gPlayerMagentaEffect[arg1], (arg2 >> 8) & 0xFF, arg3);
    move_u16_towards(&gPlayerYellowEffect[arg1], arg2 & 0xFF, arg3);
}

/**
 * Activates in the tunnel to shade the player a bit darker
 * Sort of an atmospheric effect.
 */
bool is_player_under_light_luigi_raceway(Player* player, s8 arg1) {
    if (IsLuigiRaceway()) {
        if (((gNearestPathPointByPlayerId[arg1] >= 0x14F) && (gNearestPathPointByPlayerId[arg1] < 0x158)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x15E) && (gNearestPathPointByPlayerId[arg1] < 0x164)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x169) && (gNearestPathPointByPlayerId[arg1] < 0x170)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x174) && (gNearestPathPointByPlayerId[arg1] < 0x17A)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x17E) &&
             (gNearestPathPointByPlayerId[arg1] < 0x184))) { // under a light in the tunnel
            change_player_color_effect_rgb(player, arg1, COLOR_LIGHT, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0xE0, 0.3f);
            D_80164B80[arg1] = 0;
            return true;
        }
        return false;
    }
    return false;
}

void render_light_environment_on_player(Player* player, s8 arg1) {
    if (IsBowsersCastle()) {
        if (((gNearestPathPointByPlayerId[arg1] >= 0x15) && (gNearestPathPointByPlayerId[arg1] < 0x2A)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x14D) && (gNearestPathPointByPlayerId[arg1] < 0x15C)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x1D1) && (gNearestPathPointByPlayerId[arg1] < 0x1E4)) ||
            (player->collision.surfaceDistance[2] >= 500.0f)) { // over lava
            change_player_color_effect_rgb(player, arg1, COLOR_LAVA, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0x004040, 0.3f);
            D_80164B80[arg1] = 0;
        } else if (((gNearestPathPointByPlayerId[arg1] >= 0xF1) && (gNearestPathPointByPlayerId[arg1] < 0xF5)) ||
                   ((gNearestPathPointByPlayerId[arg1] >= 0xFB) && (gNearestPathPointByPlayerId[arg1] < 0xFF)) ||
                   ((gNearestPathPointByPlayerId[arg1] >= 0x105) && (gNearestPathPointByPlayerId[arg1] < 0x109)) ||
                   ((gNearestPathPointByPlayerId[arg1] >= 0x10F) && (gNearestPathPointByPlayerId[arg1] < 0x113)) ||
                   ((gNearestPathPointByPlayerId[arg1] >= 0x145) && (gNearestPathPointByPlayerId[arg1] < 0x14A)) ||
                   ((gNearestPathPointByPlayerId[arg1] >= 0x15E) &&
                    (gNearestPathPointByPlayerId[arg1] < 0x163))) { // under a lamp
            change_player_color_effect_rgb(player, arg1, COLOR_LIGHT, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0xE0, 0.3f);
            D_80164B80[arg1] = 0;
        } else { // normal color
            change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0, 0.3f);
            D_80164B80[arg1] = 0;
        }
    } else if (IsBansheeBoardwalk()) {
        if (((gNearestPathPointByPlayerId[arg1] >= 0xD) && (gNearestPathPointByPlayerId[arg1] < 0x15)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x29) && (gNearestPathPointByPlayerId[arg1] < 0x39)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x46) && (gNearestPathPointByPlayerId[arg1] < 0x4E)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x5F) && (gNearestPathPointByPlayerId[arg1] < 0x67)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x7B) && (gNearestPathPointByPlayerId[arg1] < 0x86)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x9D) && (gNearestPathPointByPlayerId[arg1] < 0xA6)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0xB9) && (gNearestPathPointByPlayerId[arg1] < 0xC3)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0xB9) && (gNearestPathPointByPlayerId[arg1] < 0xC3)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0xD7) && (gNearestPathPointByPlayerId[arg1] < 0xE1)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x10E) && (gNearestPathPointByPlayerId[arg1] < 0x119)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x154) && (gNearestPathPointByPlayerId[arg1] < 0x15F)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x1EF) && (gNearestPathPointByPlayerId[arg1] < 0x1F7)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x202) && (gNearestPathPointByPlayerId[arg1] < 0x209)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x216) && (gNearestPathPointByPlayerId[arg1] < 0x21D)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x230) && (gNearestPathPointByPlayerId[arg1] < 0x23A)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x24C) && (gNearestPathPointByPlayerId[arg1] < 0x256)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x288) && (gNearestPathPointByPlayerId[arg1] < 0x269)) ||
            ((gNearestPathPointByPlayerId[arg1] >= 0x274) &&
             (gNearestPathPointByPlayerId[arg1] < 0x27E))) { // under a lamp
            change_player_color_effect_rgb(player, arg1, COLOR_LIGHT, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0x0000E0, 0.3f);
            D_80164B80[arg1] = 0;
        } else {
            change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0, 0.3f);
            D_80164B80[arg1] = 0;
        }
    } else {
        change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.3f);
        change_player_color_effect_cmy(player, arg1, 0, 0.3f);
        D_80164B80[arg1] = 0;
    }
}

void func_800235AC(Player* player, s8 arg1) {
    s32 temp;

    if (((player->type & PLAYER_INVISIBLE_OR_BOMB) == PLAYER_INVISIBLE_OR_BOMB) && (player == gPlayerThree)) {
        change_player_color_effect_rgb(player, arg1, COLOR_LIGHT, 0.3f);
        change_player_color_effect_cmy(player, arg1, 0xE0, 0.3f);
        D_80164B80[arg1] = 0;
        return;
    }

    if (((player->unk_0CA & 0x10) == 0x10) && ((player->unk_0CA & 4) == 4)) {
        change_player_color_effect_rgb(player, arg1, 0x646464, 0.5f);
        change_player_color_effect_cmy(player, arg1, 0xFF0000, 0.1f);
        return;
    }
    if ((player->unk_0CA & 4) == 4) {
        change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 1.0f);
        change_player_color_effect_cmy(player, arg1, 0, 1.0f);
        return;
    }
    if ((player->unk_0CA & 0x10) == 0x10) {
        change_player_color_effect_rgb(player, arg1, 0x646464, 0.5f);
        change_player_color_effect_cmy(player, arg1, 0xFF0000, 0.1f);
        return;
    }
    if ((player->unk_0CA & 0x20) == 0x20) {
        change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.1f);
        change_player_color_effect_cmy(player, arg1, 0, 0.1f);
        return;
    }

    if (((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) && ((s32) player->unk_0B0 < 0x78)) {
        D_80164B80[arg1] += 5;
        if (D_80164B80[arg1] >= 0x1E) {
            D_80164B80[arg1] = 0;
        }
        if ((D_80164B80[arg1] >= 0) && (D_80164B80[arg1] < 0xB)) {
            change_player_color_effect_rgb(player, arg1, 0x808080, 0.8f);
            change_player_color_effect_cmy(player, arg1, 0, 0.8f);
        }
        if ((D_80164B80[arg1] >= 0xB) && (D_80164B80[arg1] < 0x15)) {
            change_player_color_effect_rgb(player, arg1, 0x70, 0.8f);
            change_player_color_effect_cmy(player, arg1, 0, 0.8f);
        }
        if ((D_80164B80[arg1] >= 0x15) && (D_80164B80[arg1] < 0x1F)) {
            change_player_color_effect_rgb(player, arg1, 0x8F8F00, 0.8f);
            change_player_color_effect_cmy(player, arg1, 0, 0.8f);
        }
        return;
    }
    if ((player->effects & STAR_EFFECT) != 0) {
        temp = (s32) gCourseTimer - gPlayerStarEffectStartTime[arg1];
        if (temp <= 8) {

            if (temp >= 7) {
                D_80164B80[arg1] += 10;
            } else {
                D_80164B80[arg1] += 5;
            }
            if (D_80164B80[arg1] >= 40) {
                D_80164B80[arg1] = 0;
            }
            if ((D_80164B80[arg1] >= 0) && (D_80164B80[arg1] <= 10)) {
                change_player_color_effect_rgb(player, arg1, 0x70, 0.8f);
                change_player_color_effect_cmy(player, arg1, 0, 0.8f);
            }
            if ((D_80164B80[arg1] >= 0xB) && (D_80164B80[arg1] <= 20)) {
                change_player_color_effect_rgb(player, arg1, 0x707000, 0.8f);
                change_player_color_effect_cmy(player, arg1, 0, 0.8f);
            }
            if ((D_80164B80[arg1] >= 0x15) && (D_80164B80[arg1] <= 30)) {
                change_player_color_effect_rgb(player, arg1, 0x700000, 0.8f);
                change_player_color_effect_cmy(player, arg1, 0, 0.8f);
            }
            if (D_80164B80[arg1] >= 0x1F) {
                change_player_color_effect_rgb(player, arg1, 0x7000, 0.8f);
                change_player_color_effect_cmy(player, arg1, 0, 0.8f);
            }
            return;
        }
    }
    if (is_player_under_light_luigi_raceway(player, arg1) != true) {
        if (((player->tyres[BACK_RIGHT].unk_14 & 1) == 1) || ((player->tyres[BACK_RIGHT].unk_14 & 2) == 2) ||
            ((player->tyres[FRONT_LEFT].unk_14 & 3) == 3)) {
            change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0x6F6F6F, 0.3f);
            return;
        }
        render_light_environment_on_player(player, arg1);
        if ((player->unk_0CA & 0x1000) == 0x1000) {
            change_player_color_effect_rgb(player, arg1, COLOR_BLACK, 0.3f);
            change_player_color_effect_cmy(player, arg1, 0xF0F0F0, 0.3f);
        }
    }
}

void func_80023BF0(Player* player, s8 playerId, s8 screenId, s8 arg3) {
    if (((player->effects & HIT_EFFECT) == HIT_EFFECT) || ((player->effects & 0x8000000) == 0x8000000)) {
        func_80022CA8(player, playerId, screenId, arg3);
    } else {
        func_80022E84(player, playerId, screenId, arg3);
    }
    if ((player->unk_0CA & 2) == 2) {
        func_80022D60(player, playerId, screenId, arg3);
    }
}

Vtx gKartShadowVertices[] = {
    { { { 9, 0, 9 }, 0, { 4032, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 9, 0, -9 }, 0, { 4032, 4032 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -9, 0, -9 }, 0, { 0, 4032 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -9, 0, 9 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

void render_player_shadow(Player* player, s8 playerId, s8 screenId) {
    Mat4 mtx;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    Vec3f spB4;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f pad2;
    f32 var_f2;

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("Kart Shadow", TAG_ITEM_ADDR(player));
    temp_t9 = (u16) (player->unk_048[screenId] + player->rotation[1] + player->unk_0C0) / 128; // << 7) & 0xFFFF;
    spC0 = -player->rotation[1] - player->unk_0C0;

    spB0 = -coss(temp_t9 << 7) * 2;
    spAC = -sins(temp_t9 << 7) * 2;

    if (((player->effects & 0x01000000) == 0x01000000) || ((player->effects & 0x400) == 0x400) ||
        ((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
        ((player->effects & 0x400) == 0x400) || ((player->unk_0CA & 2) == 2) ||
        ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) || ((player->effects & 0x10000) == 0x10000) ||
        ((player->effects & 8) == 8)) {

        var_f2 = (f32) (1.0 - ((f64) player->collision.surfaceDistance[2] * 0.02));
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
        spB4[0] = player->collision.orientationVector[0];
        spB4[2] = player->collision.orientationVector[2];
        spB4[1] = player->collision.orientationVector[1];

        spCC[0] = player->pos[0] + ((spB0 * sins(spC0)) + (spAC * coss(spC0)));
        spCC[1] = player->unk_074 + 1.0f;
        spCC[2] = player->pos[2] + ((spB0 * coss(spC0)) - (spAC * sins(spC0)));
        set_transform_matrix(mtx, spB4, spCC, (spC0 + player->unk_042),
                             gCharacterSize[player->characterId] * player->size * var_f2);
    } else {
        spC4[0] = player->slopeAccel;
        spC4[1] = spC0;
        spC4[2] = player->unk_206 * 2;

        spCC[0] = player->pos[0] + ((spB0 * sins(spC0)) + (spAC * coss(spC0)));
        spCC[1] = player->unk_074 + 1.0f;
        spCC[2] = player->pos[2] + ((spB0 * coss(spC0)) - (spAC * sins(spC0)));
        mtxf_translate_rotate(mtx, spCC, spC4);
        mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    }
    convert_to_fixed_point_matrix(GetShadowMatrix(playerId + (screenId * 8)), mtx);

    gSPMatrix(gDisplayListHead++, GetShadowMatrix(playerId + (screenId * 8)),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, gTextureKartShadow, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    func_8004B414(0, 0, 0, 0xFF);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, gKartShadowVertices, 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}

void render_player_shadow_credits(Player* player, s8 playerId, s8 screenId) {
    Mat4 mtx;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    UNUSED Vec3f pad2;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f pad3;
    Vec3f sp94 = { 9.0f, 7.0f, 5.0f };

    temp_t9 = (u16) (player->unk_048[screenId] + player->rotation[1] + player->unk_0C0) / 128;
    spC0 = -player->rotation[1] - player->unk_0C0;

    spB0 = -coss(temp_t9 << 7) * 3;
    spAC = -sins(temp_t9 << 7) * 3;

    spC4[0] = 0;
    spC4[1] = spC0;
    spC4[2] = 0;

    spCC[0] = player->pos[0] + ((spB0 * sins(spC0)) + (spAC * coss(spC0)));
    spCC[2] = player->pos[2] + ((spB0 * coss(spC0)) - (spAC * sins(spC0)));
    spCC[1] = gObjectList[indexObjectList1[playerId]].pos[1] + sp94[playerId];

    mtxf_translate_rotate(mtx, spCC, spC4);
    mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    convert_to_fixed_point_matrix(GetShadowMatrix(playerId + (screenId * 8)), mtx);

    gSPMatrix(gDisplayListHead++, GetShadowMatrix(playerId + (screenId * 8)),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, gTextureKartShadow, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    func_8004B414(0, 0, 0, 0x000000D0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, gKartShadowVertices, 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

typedef u8** kart_texture_t;

extern kart_texture_t** gKartTextureTable0[];
extern kart_texture_t** gKartTextureTable1[];
extern u8** gKartTextureTumbles[];

void render_kart(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 mtx;
    UNUSED s32 pad2[17];
    Vec3f sp154;
    Vec3s sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    s16 temp_v1;
    s16 thing;

    FrameInterpolation_RecordOpenChild("player_kart", playerId | screenId << 8);
    if (player->unk_044 & 0x2000) {
        sp14C[0] = 0;
        sp14C[1] = player->unk_048[screenId];
        sp14C[2] = 0;
        func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[screenId], player->unk_050[screenId]);
        sp154[1] = (player->pos[1] - player->boundingBoxSize) + (sp144 - 2.0);
        sp154[0] = player->pos[0] + sp148;
        sp154[2] = player->pos[2] + sp140;
    } else {
        thing = (u16) (player->unk_048[screenId] + player->rotation[1] + player->unk_0C0);
        temp_v1 = player->unk_0CC[screenId] * sins(thing);
        if ((player->effects & 8) == 8) {
            sp14C[0] = cameras[screenId].rot[0] - 0x4000;
        } else {
            sp14C[0] = -temp_v1 * 0.8;
        }
        sp14C[1] = player->unk_048[screenId];
        sp14C[2] = player->unk_050[screenId];
        if (((s32) player->effects & HIT_EFFECT) == HIT_EFFECT) {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 8.0f, 0.0f, -player->unk_048[screenId],
                          player->unk_050[screenId]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->hopVerticalOffset;
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        } else {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[screenId],
                          player->unk_050[screenId]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->hopVerticalOffset + (sp144 - 2.0);
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        }
    }
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[D_801651D0[screenId][playerId]][screenId][playerId];
#else
    gPlayerPalette =
        (struct_D_802F1F80*) &gPlayerPalettesList[D_801651D0[screenId][playerId]][screenId][playerId * 0x100];
#endif
    if ((screenId == 0) || (screenId == 1)) {
        sKartTexture = gEncodedKartTexture[D_801651D0[screenId][playerId]][screenId][playerId].unk_00;
        // gross hack to match original behavior
        if (sKartTexture == NULL) {
            load_kart_texture(player, playerId, screenId, screenId, 1);
            sKartTexture = gEncodedKartTexture[D_801651D0[screenId][playerId]][screenId][playerId].unk_00;
        }
    } else {
        sKartTexture = gEncodedKartTexture[D_801651D0[screenId][playerId]][screenId - 1][playerId - 4].unk_00;
    }
    mtxf_translate_rotate(mtx, sp154, sp14C);
    mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    convert_to_fixed_point_matrix(GetKartMatrix(playerId + (screenId * 8)), mtx);

    if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
        if (screenId == playerId) {
            gSPMatrix(gDisplayListHead++, GetKartMatrix(playerId + (screenId * 8)),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          (s32) player->alpha);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        } else {
            gSPMatrix(gDisplayListHead++, GetKartMatrix(playerId + (screenId * 8)),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          gPlayerOtherScreensAlpha[playerId]);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        }
    } else if (((player->unk_0CA & 4) == 4) || (player->soundEffects & 0x08000000) ||
               (player->soundEffects & 0x04000000)) {
        gSPMatrix(gDisplayListHead++, GetKartMatrix(playerId + (screenId * 8)),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->alpha);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    } else {
        gSPMatrix(gDisplayListHead++, GetKartMatrix(playerId + (screenId * 8)),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    }

    // Render kart
    gDPLoadTextureBlock(gDisplayListHead++, sKartTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &gPlayerVtx[playerId][flipOffset], 8, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSP2Triangles(gDisplayListHead++, 4, 5, 6, 4, 4, 6, 7, 4);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}

void render_ghost(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 mtx;
    UNUSED s32 pad2[17];
    Vec3f spDC;
    Vec3s spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    UNUSED s32 pad3;
    s16 spC2;
    s16 thing;

    if (screenId) {}
    if (D_8015F890 == 1) {
        spC2 = 0x00FF;
    } else {
        spC2 = 0x0070;
    }
    thing = (u16) (player->unk_048[screenId] - player->rotation[1]);
    spD4[0] = (-(s16) (sins(thing) * (0.0f * 0.0f)) * 0.8);
    spD4[1] = player->unk_048[screenId];
    spD4[2] = player->unk_050[screenId];
    func_80062B18(&spD0, &spCC, &spC8, 0, 1.5f, 0, -player->unk_048[screenId], player->unk_050[screenId]);
    spDC[1] = (player->pos[1] - player->boundingBoxSize) + (spCC - 2.0);
    spDC[0] = player->pos[0] + spD0;
    spDC[2] = player->pos[2] + spC8;
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[D_801651D0[screenId][playerId]][screenId][playerId];
#else
    gPlayerPalette =
        (struct_D_802F1F80*) &gPlayerPalettesList[D_801651D0[screenId][playerId]][screenId][playerId * 0x100];
#endif
    if ((screenId == 0) || (screenId == 1)) {
        sKartTexture = gEncodedKartTexture[D_801651D0[screenId][playerId]][screenId][playerId].unk_00;
    } else {
        sKartTexture = gEncodedKartTexture[D_801651D0[screenId][playerId]][screenId - 1][playerId - 4].unk_00;
    }

    mtxf_translate_rotate(mtx, spDC, spD4);
    mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    convert_to_fixed_point_matrix(GetKartMatrix(playerId + (screenId * 8)), mtx);

    gSPMatrix(gDisplayListHead++, GetKartMatrix(playerId + (screenId * 8)),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListHead++, common_setting_render_character);
    gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                  gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId], spC2);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(gDisplayListHead++, sKartTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &gPlayerVtx[playerId][flipOffset], 8, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSP2Triangles(gDisplayListHead++, 4, 5, 6, 4, 4, 6, 7, 4);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}

void func_80025DE8(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    Mat4 mtx;
    Vec3f sp9C;
    Vec3s sp94;

    sp9C[0] = player->pos[0] + (sins(-player->rotation[1]) * -1.5);
    sp9C[1] = ((player->pos[1] - player->boundingBoxSize) + player->hopVerticalOffset) + 0.1;
    sp9C[2] = player->pos[2] + (coss(-player->rotation[1]) * -1.5);
    sp94[0] = -0x00B6;
    sp94[1] = player->unk_048[screenId];
    sp94[2] = player->unk_050[screenId];

    FrameInterpolation_RecordOpenChild("player_boost", playerId | screenId << 8);

    mtxf_translate_rotate(mtx, sp9C, sp94);
    mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], mtx);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008D10);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                  gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                  0x00000040);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(gDisplayListHead++, sKartTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &gPlayerVtx[playerId][flipOffset], 8, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSP2Triangles(gDisplayListHead++, 4, 5, 6, 4, 4, 6, 7, 4);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gMatrixEffectCount += 1;

    FrameInterpolation_RecordCloseChild();
}

void render_player_ice_reflection(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    Mat4 mtx;
    Vec3f sp9C;
    Vec3s sp94;

    sp94[0] = 0;
    sp94[1] = player->unk_048[screenId];
    sp94[2] = player->unk_050[screenId] + 0x8000; // invert Y
    sp9C[0] = player->pos[0];
    sp9C[1] = player->unk_074 + (4.0f * player->size);
    sp9C[2] = player->pos[2];
    if (!(player->unk_002 & (4 << (screenId * 4)))) {
        flipOffset = 8;
    } else {
        flipOffset = 0;
    }

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("PlayerReflection", playerId | screenId << 8);

    mtxf_translate_rotate(mtx, sp9C, sp94);
    mtxf_scale(mtx, gCharacterSize[player->characterId] * player->size);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxEffect[gMatrixEffectCount], mtx);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddEffectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, common_setting_render_character);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                  gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                  (s16) player->alpha / 2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    gDPLoadTextureBlock(gDisplayListHead++, sKartTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &gPlayerVtx[playerId][flipOffset], 8, 0);
    gSP2Triangles(gDisplayListHead++, 0, 1, 2, 0, 0, 2, 3, 0);
    gSP2Triangles(gDisplayListHead++, 4, 5, 6, 4, 4, 6, 7, 4);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gMatrixEffectCount += 1;

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}

void render_player(Player* player, s8 playerId, s8 screenId) {
    UNUSED s32 pad[2];
    s32 temp_t1;
    s32 var_v1;
    OSMesg* sp34;

    update_wheel_palette(player, playerId, screenId, D_801651D0[screenId][playerId]);
    if (!(player->unk_002 & (4 << (screenId * 4)))) {
        var_v1 = 0;
    } else {
        var_v1 = 8;
    }
    func_80023BF0(player, playerId, screenId, var_v1);
    temp_t1 = 8 << (screenId * 4);
    if ((temp_t1 == (player->unk_002 & temp_t1)) && (player->collision.surfaceDistance[2] <= 50.0f) &&
        (player->surfaceType != ICE)) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (playerId == screenId) {
                render_player_shadow(player, playerId, screenId);
            }
        } else {
            render_player_shadow(player, playerId, screenId);
        }
    }
    if ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB) {
        render_kart(player, playerId, screenId, var_v1);
    } else {
        render_ghost(player, playerId, screenId, var_v1);
    }
    osRecvMesg(&gDmaMesgQueue, (OSMesg*) &sp34, OS_MESG_BLOCK);
    if ((temp_t1 == (player->unk_002 & temp_t1)) && (player->surfaceType == ICE) && ((player->unk_0CA & 1) != 1) &&
        (player->collision.surfaceDistance[2] <= 30.0f)) {
        render_player_ice_reflection(player, playerId, screenId, var_v1);
    }
    if (player->boostPower >= 2.0f) {
        func_80025DE8(player, playerId, screenId, var_v1);
    }
    // Allows wheels to spin
    if (GameEngine_ResourceGetTexTypeByName(sKartTexture) !=
        1) { // only invalidate texture cache if it's a palette texture
        gSPInvalidateTexCache(gDisplayListHead++, sKartTexture);
    }
}

void func_80026A48(Player* player, s8 arg1) {
    f32 temp_f0;
    if (((player->effects & 0x4000) == 0x4000) && ((player->type & PLAYER_START_SEQUENCE) == 0)) {
        player->tyreSpeed += D_800DDE74[8];
        if (player->tyreSpeed >= 0x400) {
            player->tyreSpeed = 0;
        }
        return;
    }
    temp_f0 = ((player->speed * (1.0f + player->unk_104)) / 18.0f) * 216.0f;

    if ((temp_f0 <= 1.0f) || (gIsPlayerTripleBButtonCombo[arg1] == true)) {
        player->tyreSpeed = 0;
    } else {
        s32 temp = (s32) (temp_f0 / 12.0f);
        //! @warning This condition prevents an array overflow.
        // It's possible on hardware the tyres stop spinning despite that the player is still moving.
        // This prevents that overflow/bug
        // The ARRAY_COUNT is so if D_800DDE74 is changed, this fix automatically adjusts to the new size.
        if (temp >= ARRAY_COUNT(D_800DDE74)) {
            temp = ARRAY_COUNT(D_800DDE74) - 1;
        }
        player->tyreSpeed += D_800DDE74[temp];
    }
    if (player->tyreSpeed >= 0x400) {
        player->tyreSpeed = 0;
    }
}

// Properly define struct pointers, see buffers.h comment for more information.
#ifdef AVOID_UB
#define PLAYER_WHEEL_MACRO(a, screenId, playerId) &gPlayerPalettesList[a][screenId][playerId].wheel_palette
#else
#define PLAYER_WHEEL_MACRO(a, screenId, playerId) &gPlayerPalettesList[a][screenId][(PlayerId * 0x100) + 0xC0]
#endif

// 80 bytes between each wheel palette
u32 wheel_offset[] = { 1076, 1076, 996, 916, 916, 916, 836, 756, 756 };

void update_wheel_palette(Player* player, s8 playerId, s8 screenId, s8 arg3) {
    s16 frameId = gLastAnimFrameSelector[screenId][playerId];
    s16 groupId = gLastAnimGroupSelector[screenId][playerId];
    s16 tyreSpeed = player->tyreSpeed;

    u8 character = player->characterId;
    u32 wheel0 = 84*(8-groupId);
    u32 wheel1 = wheel_offset[groupId];

    if (((player->effects & 0x4000) == 0x4000) && ((player->type & PLAYER_START_SEQUENCE) == 0)) {
        if (((player->effects & 0x80) != 0x80) && ((player->effects & 0x40) != 0x40) &&
            ((player->effects & 0x20000) != 0x20000) && ((player->effects & 0x80000) != 0x80000) &&
            ((player->effects & 0x800000) != 0x800000) && ((player->unk_044 & 0x800) == 0)) {

            if (frameId <= 20) {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel0 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            } else {
                int32_t offset = ((frameId - 21) << 2) + (tyreSpeed >> 8) + 0x18;
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel1 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel0 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            } else {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel1 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            }
        }
    } else {
        if (((player->effects & 0x80) != 0x80) && ((player->effects & 0x40) != 0x40) &&
            ((player->effects & 0x80000) != 0x80000) && ((player->effects & 0x800000) != 0x800000) &&
            ((player->effects & 0x20000) != 0x20000) && ((player->unk_044 & 0x800) == 0)) {

            if (frameId <= 20) {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel0 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            } else {

                int32_t offset = ((frameId - 21) << 2) + (tyreSpeed >> 8) + 0x18;
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel1 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel0 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            } else {
                int32_t offset = (frameId << 2) + (tyreSpeed >> 8);
                load_wheel_palette_non_blocking(player, wheelPtr[character][wheel1 + offset],
                                                PLAYER_WHEEL_MACRO(arg3, screenId, playerId), 0x80);
            }
        }
    }
}

#undef PLAYER_WHEEL_MACRO

UNUSED void func_8002701C(void) {
}

UNUSED void func_80027024(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2) {
}
