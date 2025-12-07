#include <defines.h>
#include <mk64.h>
#include <stubs.h>
#include "networking/networking.h"

#include "spawn_players.h"
#include "code_800029B0.h"
#include "kart_attributes.h"
#include "memory.h"
#include "waypoints.h"
#include "buffers.h"
#include "kart_dma.h"
#include "camera.h"
#include "math_util.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "collision.h"
#include "render_courses.h"
#include "replays.h"
#include "code_80005FD0.h"
#include "render_player.h"
#include "podium_ceremony_actors.h"
#include "main.h"
#include "menus.h"
#include "render_player.h"
#include "menu_items.h"
#include "effects.h"
#include "decode.h"
#include "port/Game.h"

f32 D_80165210[8];
f32 D_80165230[8];
UNUSED f32 D_80165250[8];
s16 D_80165270[8];
f32 gPlayerCurrentSpeed[8];
f32 gPlayerWaterLevel[8];
s32 D_801652C0[8];
s32 D_801652E0[8];
s16 D_80165300[8];
// Shadows values from gPathIndexByPlayerId, but is an array
u16 gCopyPathIndexByPlayerId[8];
// Shadows values from gNearestPathPointByPlayerId, but is an array
s16 gCopyNearestWaypointByPlayerId[8];
s16 D_80165330[8];
s16 D_80165340;
UNUSED s32 D_80165348[29];
Player* D_801653C0[8];

bool gPlayerIsThrottleActive[8];
s32 gPlayerAButtonComboActiveThisFrame[8];
s32 gFrameSinceLastACombo[8];
s32 gCountASwitch[8];
bool gIsPlayerTripleAButtonCombo[8];
s32 gTimerBoostTripleACombo[8];

bool gPlayerIsBrakeActive[8];
s32 gPlayerBButtonComboActiveThisFrame[8];
s32 gFrameSinceLastBCombo[8];
s32 gCountBChangement[8];
bool gIsPlayerTripleBButtonCombo[8];
s32 gTimerBoostTripleBCombo[8];

s16 chooseCPUPlayers[7];

s16 D_8016556E;
s16 D_80165570;
s16 D_80165572;
s16 D_80165574;
s16 D_80165576;
s16 D_80165578;
s16 D_8016557A;
s16 D_8016557C;
s16 D_8016557E;
s16 D_80165580;
s16 D_80165582;

// arg4 is height? Or something like that?
void spawn_player(Player* player, s8 playerIndex, f32 startingRow, f32 startingColumn, f32 arg4, f32 arg5,
                  u16 characterId, s16 playerType) {
    f32 ret;
    s8 idx;

    player->type = PLAYER_INACTIVE;
    player->kartPropulsionStrength = 0;
    player->characterId = characterId;
    player->unk_0B6 = 0;
    player->kartFriction = gKartFrictionTable[player->characterId];
    player->boundingBoxSize = gKartBoundingBoxSizeTable[player->characterId];
    player->kartGravity = gKartGravityTable[player->characterId];

    switch (gModeSelection) {
        case GRAND_PRIX:
        case VERSUS:
            player->unk_084 = D_800E2400[gCCSelection][player->characterId];
            player->unk_088 = D_800E24B4[gCCSelection][player->characterId];
            player->unk_210 = D_800E2568[gCCSelection][player->characterId];
            player->topSpeed = gTopSpeedTable[gCCSelection][player->characterId];
            break;

        // Uses 100CC values
        case TIME_TRIALS:
            player->unk_084 = D_800E2400[CC_100][player->characterId];
            player->unk_088 = D_800E24B4[CC_100][player->characterId];
            player->unk_210 = D_800E2568[CC_100][player->characterId];
            player->topSpeed = gTopSpeedTable[CC_100][player->characterId];
            break;

        case BATTLE:
            player->unk_084 = D_800E2400[CC_BATTLE][player->characterId];
            player->unk_088 = D_800E24B4[CC_BATTLE][player->characterId];
            player->unk_210 = D_800E2568[CC_BATTLE][player->characterId];
            player->topSpeed = gTopSpeedTable[CC_BATTLE][player->characterId];
            break;
    }
    if (CVarGetInteger("gEnableCustomCC", 0) == 1) {
#define calc_a(x, y, x2, y2) (y2 - y) / (x2 - x)
#define calc_b(x, y, b) y - (b * x)
        f32 a;
        f32 b;

#define calc(table, field)                                                                      \
    a = calc_a(50, table[CC_50][player->characterId], 150, table[CC_150][player->characterId]); \
    b = calc_b(50, table[CC_50][player->characterId], a);                                       \
    player->field = a * CVarGetFloat("gCustomCC", 150.0f) + b;

        calc(gTopSpeedTable, topSpeed);
        calc(D_800E2400, unk_084);
        calc(D_800E24B4, unk_088);
        calc(D_800E2568, unk_210);
#undef calc_a
#undef calc_b
#undef calc
    }

    player->pos[0] = startingRow;
    ret = spawn_actor_on_surface(startingRow, arg4 + 50.0f, startingColumn) + player->boundingBoxSize;
    player->pos[2] = startingColumn;
    player->pos[1] = ret;
    player->oldPos[0] = startingRow;
    player->oldPos[1] = ret;

    gPlayerPathY[playerIndex] = ret;

    player->rotation[0] = 0;
    player->oldPos[2] = startingColumn;
    player->unk_05C = 1.0f;
    player->unk_058 = 0.0f;
    player->unk_060 = 0.0f;
    player->velocity[0] = 0.0f;
    player->velocity[1] = 0.0f;
    player->velocity[2] = 0.0f;
    player->rotation[1] = arg5;
    player->rotation[2] = 0;
    player->unk_0FA = 0;
    player->unk_002 = 0;

    player->effects = 0;
    player->unk_0C0 = 0;
    player->unk_07C = 0;
    player->hopFrameCounter = 0;
    player->unk_006 = 0;
    player->lapCount = -1;
    player->kartPropulsionStrength = 0.0f;
    player->unk_090 = 0.0f;
    player->speed = 0.0f;
    player->unk_074 = 0.0f;
    player->type = playerType;
    player->unk_0CA = 0;
    player->waterInteractionFlags = WATER_NO_INTERACTION;
    player->unk_10C = 0;
    player->unk_0E2 = 0;
    player->unk_0E8 = 0.0f;
    player->unk_0A0 = 0.0f;
    player->unk_104 = 0.0f;
    player->currentSpeed = 0.0f;
    player->unk_20C = 0.0f;
    player->unk_DAC = 0.0f;
    player->unk_044 = 0;
    player->unk_046 = 0;
    player->soundEffects = 0;
    player->alpha = ALPHA_MAX;

    player->unk_206 = 0;
    player->slopeAccel = 0;
    player->unk_D98 = 0;
    player->unk_D9A = 0;
    player->unk_DA4 = 0;
    player->unk_DA6 = 0;
    player->unk_DB4.unk0 = 0;
    player->unk_DB4.unk2 = 0;
    player->unk_DB4.unk18 = 0;
    player->unk_DB4.unk1A = 0;
    player->unk_DB4.unk1C = 0;
    player->unk_DB4.unk1E = 0;
    player->unk_DB4.unk20 = 0;

    player->unk_042 = 0;
    player->unk_078 = 0;
    player->unk_0A8 = 0;
    player->unk_0AA = 0;
    player->unk_0AC = 0;
    player->unk_0AE = 0;
    player->unk_0B0 = 0;
    player->unk_0B2 = 0;
    player->unk_0B4 = 0;
    player->unk_0C0 = 0;
    player->unk_0C2 = 0;
    player->unk_0C8 = 0;
    player->unk_0CA = 0;
    player->boostTimer = 0;
    player->waterInteractionFlags = WATER_NO_INTERACTION;
    player->unk_0E0 = 0;
    player->unk_0E2 = 0;
    player->unk_10C = 0;
    player->unk_200 = 0;
    player->driftDuration = 0;
    player->nearestPathPointId = 0;
    player->unk_228 = 0;
    player->driftState = 0;
    player->unk_234 = 0;
    player->unk_236 = 0;
    player->unk_238 = 0;
    player->unk_23A = 0;
    player->tyreSpeed = 0;
    player->unk_256 = 0;

    player->size = 1.0f;
    player->unk_DAC = 1.0f;

    player->unk_064[0] = 0.0f;
    player->unk_064[1] = 0.0f;
    player->unk_064[2] = 0.0f;
    player->boostPower = 0.0f;
    player->unk_D9C = 0.0f;
    player->unk_DA0 = 0.0f;
    player->unk_DA8 = 0.0f;
    player->unk_DB0 = 0.0f;
    player->unk_DB4.unk4 = 0.0f;
    player->unk_DB4.unk8 = 0.0f;
    player->unk_DB4.unkC = 0.0f;
    player->unk_DB4.unk10 = 0.0f;
    player->unk_DB4.unk14 = 0.0f;
    player->unk_084 = 0.0f;
    player->unk_088 = 0.0f;
    player->kartPropulsionStrength = 0.0f;
    player->unk_090 = 0.0f;
    player->speed = 0.0f;
    player->unk_098 = 0.0f;
    player->currentSpeed = 0.0f;
    player->unk_0A0 = 0.0f;
    player->unk_0A4 = 0.0f;
    player->unk_0B8 = 0.0f;
    player->unk_0E4 = 0.0f;
    player->unk_0E8 = 0.0f;
    player->kartHopVelocity = 0.0f;
    player->kartHopJerk = 0.0f;
    player->kartHopAcceleration = 0.0f;
    player->unk_104 = 0.0f;
    player->hopVerticalOffset = 0.0f;
    player->unk_1F8 = 0.0f;
    player->unk_1FC = 0.0f;
    player->unk_208 = 0.0f;
    player->unk_20C = 0.0f;
    player->unk_210 = 0.0f;
    player->unk_218 = 0.0f;
    player->unk_21C = 0.0f;
    player->previousSpeed = 0.0f;
    player->unk_230 = 0.0f;
    player->unk_23C = 0.0f;

    idx = playerIndex;

    gLastAnimFrameSelector[0][idx] = 0;
    gLastAnimFrameSelector[1][idx] = 0;
    gLastAnimFrameSelector[2][idx] = 0;
    gLastAnimFrameSelector[3][idx] = 0;
    gLastAnimGroupSelector[0][idx] = 0;
    gLastAnimGroupSelector[1][idx] = 0;
    gLastAnimGroupSelector[2][idx] = 0;
    gLastAnimGroupSelector[3][idx] = 0;
    D_80165190[0][idx] = 0;
    D_80165190[1][idx] = 0;
    D_80165190[2][idx] = 0;
    D_80165190[3][idx] = 0;
    D_801651D0[0][idx] = 0;
    D_801651D0[1][idx] = 0;
    D_801651D0[2][idx] = 0;
    D_801651D0[3][idx] = 0;

    gFrameSinceLastACombo[idx] = 0;
    gCountASwitch[idx] = 0;
    gIsPlayerTripleAButtonCombo[idx] = false;
    gTimerBoostTripleACombo[idx] = 0;
    gFrameSinceLastBCombo[idx] = 0;
    gCountBChangement[idx] = 0;
    gIsPlayerTripleBButtonCombo[idx] = false;
    gTimerBoostTripleBCombo[playerIndex] = 0;
    D_8018D900[0] = 0;

    D_801652E0[playerIndex] = 0;
    D_801652C0[playerIndex] = 0;
    D_80165020[playerIndex] = 0;
    gPlayerLastVelocity[playerIndex][0] = 0.0f;
    gPlayerLastVelocity[playerIndex][1] = 0.0f;
    gPlayerLastVelocity[playerIndex][2] = 0.0f;
    gPlayerCurrentSpeed[playerIndex] = 0.0f;
    gPlayerWaterLevel[playerIndex] = 0.0f;
    gPlayerIsThrottleActive[playerIndex] = 0;
    gPlayerAButtonComboActiveThisFrame[playerIndex] = 0;
    gPlayerIsBrakeActive[playerIndex] = 0;
    gPlayerBButtonComboActiveThisFrame[playerIndex] = 0;
    D_80165340 = 0;

    player->tyres[FRONT_LEFT].surfaceType = 0;
    player->tyres[FRONT_RIGHT].surfaceType = 0;
    player->tyres[BACK_LEFT].surfaceType = 0;
    player->tyres[BACK_RIGHT].surfaceType = 0;

    player->tyres[FRONT_LEFT].surfaceFlags = 0;
    player->tyres[FRONT_RIGHT].surfaceFlags = 0;
    player->tyres[BACK_LEFT].surfaceFlags = 0;
    player->tyres[BACK_RIGHT].surfaceFlags = 0;

    player->tyres[FRONT_LEFT].collisionMeshIndex = 0;
    player->tyres[FRONT_RIGHT].collisionMeshIndex = 0;
    player->tyres[BACK_LEFT].collisionMeshIndex = 0;
    player->tyres[BACK_RIGHT].collisionMeshIndex = 0;

    player->tyres[FRONT_RIGHT].unk_14 = 0;
    player->tyres[FRONT_LEFT].unk_14 = 0;
    player->tyres[BACK_LEFT].unk_14 = 0;
    player->tyres[BACK_RIGHT].unk_14 = 0;

    player->collision.unk30 = 0;
    player->collision.unk32 = 0;
    player->collision.unk34 = 0;
    player->collision.meshIndexYX = 0;
    player->collision.meshIndexZY = 0;
    player->collision.meshIndexZX = 0;

    player->tyres[FRONT_LEFT].pos[0] = 0.0f;
    player->tyres[FRONT_LEFT].pos[1] = 0.0f;
    player->tyres[FRONT_LEFT].pos[2] = 0.0f;

    player->tyres[FRONT_RIGHT].pos[0] = 0.0f;
    player->tyres[FRONT_RIGHT].pos[1] = 0.0f;
    player->tyres[FRONT_RIGHT].pos[2] = 0.0f;

    player->tyres[BACK_LEFT].pos[0] = 0.0f;
    player->tyres[BACK_LEFT].pos[1] = 0.0f;
    player->tyres[BACK_LEFT].pos[2] = 0.0f;

    player->tyres[BACK_RIGHT].pos[0] = 0.0f;
    player->tyres[BACK_RIGHT].pos[1] = 0.0f;
    player->tyres[BACK_RIGHT].pos[2] = 0.0f;

    player->tyres[FRONT_LEFT].baseHeight = 0.0f;
    player->tyres[FRONT_RIGHT].baseHeight = 0.0f;
    player->tyres[BACK_LEFT].baseHeight = 0.0f;
    player->tyres[BACK_RIGHT].baseHeight = 0.0f;

    player->collision.surfaceDistance[0] = 0.0f;
    player->collision.surfaceDistance[1] = 0.0f;
    player->collision.surfaceDistance[2] = 0.0f;
    player->collision.unk48[0] = 0.0f;
    player->collision.unk48[1] = 0.0f;
    player->collision.unk48[2] = 0.0f;
    player->collision.unk54[0] = 0.0f;
    player->collision.unk54[1] = 0.0f;
    player->collision.unk54[2] = 0.0f;
    player->collision.orientationVector[0] = 0.0f;
    player->collision.orientationVector[1] = 0.0f;
    player->collision.orientationVector[2] = 0.0f;

    D_80165300[playerIndex] = 0;
    D_8018CE10[playerIndex].unk_04[0] = 0.0f;
    D_8018CE10[playerIndex].unk_04[2] = 0.0f;
    func_80295BF8(playerIndex);
    reset_player_particle_pool(player);
    clear_all_player_balloons(player, playerIndex);
    if (gModeSelection == BATTLE) {
        init_all_player_balloons(player, playerIndex);
    }
    calculate_orientation_matrix(player->unk_150, player->unk_058, player->unk_05C, player->unk_060,
                                 player->rotation[1]);
    calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                 player->rotation[1]);
}

void func_80039AE4(void) {
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            if (gGamestate == ENDING) {
                D_80165578 = 0x898;
                D_8016557A = 0;
                D_8016557C = 0x384;
                D_8016557E = 0;
                D_80165574 = 0x384;
                D_80165576 = 0;
                D_80165570 = 0x35C;
                D_80165572 = 0;
                D_80165580 = 0x1F4;
                D_80165582 = 0;
            } else {
                D_80165578 = 0x4B0;
                D_8016557A = -0xA;
                D_8016557C = 0x384;
                D_8016557E = 0x32;
                D_80165574 = 0x1F4;
                D_80165576 = 0;
                D_80165570 = 0x15E;
                D_80165572 = 0;
                D_80165580 = 0xFA;
                D_80165582 = 0;
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (gModeSelection == BATTLE) {
                D_80165578 = 0x898;
                D_8016557A = 0;
                D_8016557C = 0x320;
                D_8016557E = 0;
                D_80165574 = 0x190;
                D_80165576 = 0;
                D_80165570 = 0xC8;
                D_80165572 = 0;
                D_80165580 = 0xC8;
                D_80165582 = 0;
            } else {
                D_80165578 = 0x4B0;
                D_8016557A = 0x32;
                D_8016557C = 0x320;
                D_8016557E = 0x32;
                D_80165574 = 0x190;
                D_80165576 = 0;
                D_80165570 = 0x96;
                D_80165572 = 0;
                D_80165580 = 0x96;
                D_80165582 = 0;
            }
            break;

        default:
            if (gModeSelection == BATTLE) {
                D_80165578 = 0x898;
                D_8016557A = 0;
                D_8016557C = 0x320;
                D_8016557E = 0;
                D_80165574 = 0x190;
                D_80165576 = 0;
                D_80165570 = 0xC8;
                D_80165572 = 0;
                D_80165580 = 0xC8;
                D_80165582 = 0;
            } else {
                D_80165578 = 0x3E8;
                D_8016557A = 0;
                D_8016557C = 0x258;
                D_8016557E = 0;
                D_80165574 = 0x15E;
                D_80165576 = 0;
                D_80165570 = 0x96;
                D_80165572 = 0;
                D_80165580 = 0x96;
                D_80165582 = 0;
            }
            break;
    }
}

// typedef struct {
//     s32 unk00[8];
// } temp_80039DA4; // to be removed when data is compilable
// extern temp_80039DA4 D_800E4360;
// extern temp_80039DA4 D_800E4380;

void func_80039DA4(void) {
    s32 i;

    static const s32 sp2C[] = {
        7, 6, 5, 4, 3, 2, 1, 0,
    };

    static const s32 spC[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
    };

    if (((GetCupCursorPosition() == COURSE_ONE) && (D_8016556E == 0)) || (gDemoMode == 1) ||
        (gDebugMenuSelection == DEBUG_MENU_OPTION_SELECTED)) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            D_80165270[i] = sp2C[i];
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            D_80165270[i] = spC[gGPCurrentRaceRankByPlayerId[i]];
        }
    }
}

UNUSED f32 D_800E43A0 = 1.0f;
UNUSED s16 D_800E43A4 = 1;
UNUSED s16 D_800E43A8 = 0;

void spawn_players_gp_one_player(f32* arg0, f32* arg1, f32 arg2) {
    func_80039DA4();
    if (((GetCupCursorPosition() == COURSE_ONE) && (D_8016556E == 0)) || (gDemoMode == 1) ||
        (gDebugMenuSelection == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        do {
            rand = random_int(7);
        } while (rand == gCharacterSelections[0]);

        // Randomize gPlayerTwo
        chooseCPUPlayers[0] = rand;

        // Chooses arr[0] as a fallback to prevent duplicating characters.
        // If it doesn't find the if, it will grab the final index as a fallback.
        for (i = 1; i < 7; i++) {
            u16* arr = (u16*) cpu_forPlayer[gCharacterSelections[0]];
            if (rand == arr[i]) {
                chooseCPUPlayers[i] = arr[0];
            } else {
                chooseCPUPlayers[i] = arr[i];
            }
        }
    }

    D_8016556E = 0;
    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, chooseCPUPlayers[0],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerThree, 2, arg0[D_80165270[2]], arg1[D_80165270[2]], arg2, 32768.0f, chooseCPUPlayers[1],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFour, 3, arg0[D_80165270[3]], arg1[D_80165270[3]], arg2, 32768.0f, chooseCPUPlayers[2],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFive, 4, arg0[D_80165270[4]], arg1[D_80165270[4]], arg2, 32768.0f, chooseCPUPlayers[3],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSix, 5, arg0[D_80165270[5]], arg1[D_80165270[5]], arg2, 32768.0f, chooseCPUPlayers[4],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSeven, 6, arg0[D_80165270[6]], arg1[D_80165270[6]], arg2, 32768.0f, chooseCPUPlayers[5],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerEight, 7, arg0[D_80165270[7]], arg1[D_80165270[7]], arg2, 32768.0f, chooseCPUPlayers[6],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        D_80164A28 = 0;
    } else {
        if (gNetwork.enabled) {
            spawn_network_players(arg0, arg1, arg2);
        } else {
            spawn_player(gPlayerOne, 0, arg0[D_80165270[0]], arg1[D_80165270[0]] + 250.0f, arg2, 32768.0f,
                         gCharacterSelections[0],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
            spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[0], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerThree, 2, arg0[D_80165270[3]], arg1[D_80165270[2]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[1], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerFour, 3, arg0[D_80165270[2]], arg1[D_80165270[3]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[2], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerFive, 4, arg0[D_80165270[5]], arg1[D_80165270[4]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[3], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerSix, 5, arg0[D_80165270[4]], arg1[D_80165270[5]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[4], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerSeven, 6, arg0[D_80165270[7]], arg1[D_80165270[6]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[5], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
            spawn_player(gPlayerEight, 7, arg0[D_80165270[6]], arg1[D_80165270[7]] + 250.0f, arg2, 32768.0f,
                         chooseCPUPlayers[6], PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_CPU);
        }
        D_80164A28 = 1;
    }
    func_80039AE4();
}

void spawn_players_versus_one_player(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_START_SEQUENCE | PLAYER_CPU);
        spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_START_SEQUENCE | PLAYER_CPU);
    } else if (D_8015F890 != 1) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        if (bPlayerGhostDisabled == 0) {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE0,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_CPU);
        }
        if (bCourseGhostDisabled == 0) {
            spawn_player(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE4,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_CPU);
        }
    } else {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE8,
                     PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        if (D_80162DD8 == 0) {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE0,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_CPU);
        }
        if (bCourseGhostDisabled == 0) {
            spawn_player(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE4,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_CPU);
        }
    }
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_gp_two_player(f32* arg0, f32* arg1, f32 arg2) {
    func_80039DA4();
    if ((GetCupCursorPosition() == COURSE_ONE) || (gDemoMode == 1) ||
        (gDebugMenuSelection == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        // @todo: this is a do-while loop
    getRand:
        rand = random_int(7);
        if (gCharacterSelections[0] == rand) {
            goto getRand;
        }
        if (gCharacterSelections[1] == rand) {
            goto getRand;
        }

        chooseCPUPlayers[0] = rand;

        for (i = 1; i < 6; i++) {
            u16* arr = (u16*) cpu_forTwoPlayer[gCharacterSelections[0]][gCharacterSelections[1]];
            if (rand == arr[i]) {
                chooseCPUPlayers[i] = arr[0];
            } else {
                chooseCPUPlayers[i] = arr[i];
            }
        }
    }

    spawn_player(gPlayerThree, 2, arg0[D_80165270[2]], arg1[D_80165270[2]], arg2, 32768.0f, chooseCPUPlayers[0],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerFour, 3, arg0[D_80165270[3]], arg1[D_80165270[3]], arg2, 32768.0f, chooseCPUPlayers[1],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerFive, 4, arg0[D_80165270[4]], arg1[D_80165270[4]], arg2, 32768.0f, chooseCPUPlayers[2],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerSix, 5, arg0[D_80165270[5]], arg1[D_80165270[5]], arg2, 32768.0f, chooseCPUPlayers[3],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerSeven, 6, arg0[D_80165270[6]], arg1[D_80165270[6]], arg2, 32768.0f, chooseCPUPlayers[4],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerEight, 7, arg0[D_80165270[7]], arg1[D_80165270[7]], arg2, 32768.0f, chooseCPUPlayers[5],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);

    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_HUMAN_AND_CPU);
    } else {
        spawn_player(gPlayerOne, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    if (gDemoMode == 1) {
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    } else {
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }

    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_versus_two_player(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_CPU);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
    } else {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    if (gDemoMode == 1) {
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1], PLAYER_HUMAN_AND_CPU);
    } else {
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_2p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (IsBigDonut()) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    } else {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2],
                 PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[3],
                 PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_CPU);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003B318(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2], PLAYER_HUMAN_AND_CPU);
    }

    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_CPU);
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_3p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (IsBigDonut()) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 0.0f, gCharacterSelections[2],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    } else {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, -16384.0f, gCharacterSelections[2],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_CPU);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003B870(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[3],
                 PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2], PLAYER_HUMAN_AND_CPU);
        spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[3], PLAYER_HUMAN_AND_CPU);
    }
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_CPU);
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_4p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (IsBigDonut()) {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 0.0f, gCharacterSelections[2],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[3],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    } else {
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, -16384.0f, gCharacterSelections[2],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 16384.0f, gCharacterSelections[3],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_CPU);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_CPU);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003BE30(void) {
    spawn_player(gPlayerOne, 0, -2770.774f, -345.187f, -34.6f, 0.0f, gCharacterIdByGPOverallRank[0],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerTwo, 1, -3691.506f, -6.822f, -6.95f, 36400.0f, gCharacterIdByGPOverallRank[1],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerThree, 2, -3475.028f, -998.485f, -8.059f, 45500.0f, gCharacterIdByGPOverallRank[2],
                 PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    if (D_802874D8.unk1D >= 3) {
        spawn_player(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, 28210.0f, D_802874D8.unk1E,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    } else {
        spawn_player(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, 28210.0f, gCharacterIdByGPOverallRank[3],
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
    }
    spawn_player(gPlayerFive, 4, -2770.774f, -345.187f, -34.6f, 0.0f, 0, 0x7000);
    spawn_player(gPlayerSix, 5, -3691.506f, -6.822f, -6.95f, 36400.0f, 0, 0x7000);
    spawn_player(gPlayerSeven, 6, -3475.028f, -998.485f, -8.059f, 45500.0f, 0, 0x7000);
    spawn_player(gPlayerEight, 7, -3025.772f, 110.039f, -23.224f, 28210.0f, 0, 0x7000);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003C0F0(void) {
    if (gModeSelection == BATTLE) {
        func_8000EEDC();
    } else if (!IsPodiumCeremony()) {
        init_course_path_point();
    }

    // The tour delays player spawning until the end of the tour
    if ((CM_IsTourEnabled() == false) || (gIsEditorPaused == true)) {
        spawn_and_set_player_spawns();
    }
}

void spawn_and_set_player_spawns(void) {
    s16 sp5E;
    s16 sp5C;
    s16 sp5A = 0;
    s32 temp;

    if ((!IsPodiumCeremony()) && (gModeSelection != BATTLE)) {
        sp5E = (f32) gTrackPaths[0][0].x;
        sp5C = (f32) gTrackPaths[0][0].z;
        sp5A = (f32) gTrackPaths[0][0].y;
        if (IsToadsTurnpike()) {
            sp5E = 0;
        }
    }

    if ((gModeSelection != BATTLE) && (!IsPodiumCeremony())) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                switch (gModeSelection) {
                    case GRAND_PRIX:
                        D_80165210[0] = (D_80165210[2] = (D_80165210[4] = (D_80165210[6] = sp5E + 0x14)));
                        D_80165210[1] = (D_80165210[3] = (D_80165210[5] = (D_80165210[7] = sp5E - 0x14)));
                        D_80165230[0] = sp5C + 0x1E;
                        D_80165230[1] = sp5C + 0x32;
                        D_80165230[2] = sp5C + 0x46;
                        D_80165230[3] = sp5C + 0x5A;
                        D_80165230[4] = sp5C + 0x6E;
                        D_80165230[5] = sp5C + 0x82;
                        D_80165230[6] = sp5C + 0x96;
                        D_80165230[7] = sp5C + 0xAA;
                        spawn_players_gp_one_player(D_80165210, D_80165230, sp5A);
                        break;

                    case TIME_TRIALS:
                        D_80165210[0] = (D_80165210[2] = (D_80165210[4] = (D_80165210[6] = sp5E)));
                        D_80165210[1] = (D_80165210[3] = (D_80165210[5] = (D_80165210[7] = sp5E)));
                        D_80165230[0] = sp5C + 0x1E;
                        D_80165230[1] = sp5C + 0x1E;
                        D_80165230[2] = sp5C + 0x1E;
                        D_80165230[3] = sp5C + 0x1E;
                        D_80165230[4] = sp5C + 0x1E;
                        D_80165230[5] = sp5C + 0x1E;
                        D_80165230[6] = sp5C + 0x1E;
                        D_80165230[7] = sp5C + 0x1E;
                        spawn_players_versus_one_player(D_80165210, D_80165230, sp5A);
                        break;
                }
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                switch (gModeSelection) {
                    case GRAND_PRIX:
                        D_80165210[0] = (D_80165210[2] = (D_80165210[4] = (D_80165210[6] = sp5E + 0x14)));
                        D_80165210[1] = (D_80165210[3] = (D_80165210[5] = (D_80165210[7] = sp5E - 0x14)));
                        D_80165230[0] = sp5C + 0x1E;
                        D_80165230[1] = sp5C + 0x32;
                        D_80165230[2] = sp5C + 0x46;
                        D_80165230[3] = sp5C + 0x5A;
                        D_80165230[4] = sp5C + 0x6E;
                        D_80165230[5] = sp5C + 0x82;
                        D_80165230[6] = sp5C + 0x96;
                        D_80165230[7] = sp5C + 0xAA;
                        spawn_players_gp_two_player(D_80165210, D_80165230, sp5A);
                        break;

                    case VERSUS:
                        D_80165210[0] = (D_80165210[2] = (D_80165210[4] = (D_80165210[6] = sp5E + 0xA)));
                        D_80165210[1] = (D_80165210[3] = (D_80165210[5] = (D_80165210[7] = sp5E - 0xA)));
                        D_80165230[0] = sp5C + 0x1E;
                        D_80165230[1] = sp5C + 0x1E;
                        D_80165230[2] = sp5C + 0x1E;
                        D_80165230[3] = sp5C + 0x1E;
                        D_80165230[4] = sp5C + 0x1E;
                        D_80165230[5] = sp5C + 0x1E;
                        D_80165230[6] = sp5C + 0x1E;
                        D_80165230[7] = sp5C + 0x1E;
                        spawn_players_versus_two_player(D_80165210, D_80165230, sp5A);
                        break;
                }
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                switch (gModeSelection) {
                    case VERSUS:
                        D_80165210[0] = sp5E + 0x1E;
                        D_80165210[6] = sp5E - 0xA;
                        D_80165210[1] = sp5E + 0xA;
                        D_80165210[7] = sp5E - 0x1E;
                        D_80165210[4] = sp5E - 0xA;
                        D_80165210[2] = sp5E - 0xA;
                        D_80165210[5] = sp5E - 0x1E;
                        D_80165210[3] = sp5E - 0x1E;
                        D_80165230[0] = sp5C + 0x1E;
                        D_80165230[1] = sp5C + 0x1E;
                        D_80165230[2] = sp5C + 0x1E;
                        D_80165230[3] = sp5C + 0x1E;
                        D_80165230[4] = sp5C + 0x1E;
                        D_80165230[5] = sp5C + 0x1E;
                        D_80165230[6] = sp5C + 0x1E;
                        D_80165230[7] = sp5C + 0x1E;
                        if (gPlayerCountSelection1 == 4) {
                            func_8003B870(D_80165210, D_80165230, sp5A);
                        } else {
                            func_8003B318(D_80165210, D_80165230, sp5A);
                        }
                        break;
                }
                break;
        }
    } else if (IsBlockFort()) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                temp = 5;
                if (1) {};
                D_80165210[0] = 0;
                D_80165210[1] = 0;
                D_80165230[1] = -200.0f;
                D_80165230[0] = 200.0f;
                spawn_players_2p_battle(D_80165210, D_80165230, temp);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                temp = 5;
                D_80165210[2] = -200.0f;
                D_80165230[1] = -200.0f;
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165230[2] = 0.0f;
                D_80165230[3] = 0.0f;
                D_80165210[3] = 200.0f;
                D_80165230[0] = 200.0f;
                if (gPlayerCountSelection1 == 4) {
                    spawn_players_4p_battle(D_80165210, D_80165230, temp);
                } else {
                    spawn_players_3p_battle(D_80165210, D_80165230, temp);
                }
                break;
        }
    } else if (IsSkyscraper()) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                temp = 0x1E0;
                if (1) {};
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165230[1] = -400.0f;
                D_80165230[0] = 400.0f;
                spawn_players_2p_battle(D_80165210, D_80165230, temp);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                temp = 0x1E0;
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165210[2] = -400.0f;
                D_80165210[3] = 400.0f;
                D_80165230[0] = 400.0f;
                D_80165230[1] = -400.0f;
                D_80165230[2] = 0.0f;
                D_80165230[3] = 0.0f;
                if (gPlayerCountSelection1 == 4) {
                    spawn_players_4p_battle(D_80165210, D_80165230, temp);
                } else {
                    spawn_players_3p_battle(D_80165210, D_80165230, temp);
                }
                break;
        }
    } else if (IsDoubleDeck()) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                temp = 0x37;
                if (1) {};
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165230[1] = -160.0f;
                D_80165230[0] = 160.0f;
                spawn_players_2p_battle(D_80165210, D_80165230, temp);
                break;

            case 3:
                temp = 0x37;
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165210[2] = -160.0f;
                D_80165210[3] = 160.0f;
                D_80165230[0] = 160.0f;
                D_80165230[1] = -160.0f;
                D_80165230[2] = 0.0f;
                D_80165230[3] = 0.0f;
                if (gPlayerCountSelection1 == 4) {
                    spawn_players_4p_battle(D_80165210, D_80165230, temp);
                } else {
                    spawn_players_3p_battle(D_80165210, D_80165230, temp);
                }
                break;
        }
    } else if (IsBigDonut()) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                temp = 0xC8;
                if (1) {};
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165230[1] = -575.0f;
                D_80165230[0] = 575.0f;
                spawn_players_2p_battle(D_80165210, D_80165230, temp);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                temp = 0xC8;
                D_80165210[0] = 0.0f;
                D_80165210[1] = 0.0f;
                D_80165210[2] = -575.0f;
                D_80165210[3] = 575.0f;
                D_80165230[0] = 575.0f;
                D_80165230[1] = -575.0f;
                D_80165230[2] = 0.0f;
                D_80165230[3] = 0.0f;
                if (gPlayerCountSelection1 == 4) {
                    spawn_players_4p_battle(D_80165210, D_80165230, temp);
                } else {
                    spawn_players_3p_battle(D_80165210, D_80165230, temp);
                }
                break;
        }
    } else {
        D_80165210[0] = (D_80165210[2] = (D_80165210[4] = (D_80165210[6] = 20.0f)));
        D_80165210[1] = (D_80165210[3] = (D_80165210[5] = (D_80165210[7] = -20.0f)));
        D_80165230[0] = 30.0f;
        D_80165230[1] = 50.0f;
        D_80165230[2] = 70.0f;
        D_80165230[3] = 90.0f;
        D_80165230[4] = 110.0f;
        D_80165230[5] = 130.0f;
        D_80165230[6] = 150.0f;
        D_80165230[7] = 170.0f;
        spawn_player(gPlayerOne, 0, D_80165210[0], D_80165230[0], sp5A, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, D_80165210[1], D_80165230[1], sp5A, 32768.0f, 1,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerThree, 2, D_80165210[2], D_80165230[2], sp5A, 32768.0f, 2,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFour, 3, D_80165210[3], D_80165230[3], sp5A, 32768.0f, 3,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFive, 4, D_80165210[4], D_80165230[4], sp5A, 32768.0f, 4,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSix, 5, D_80165210[5], D_80165230[5], sp5A, 32768.0f, 5,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSeven, 6, D_80165210[6], D_80165230[6], sp5A, 32768.0f, 6,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerEight, 7, D_80165210[7], D_80165230[7], sp5A, 32768.0f, 7,
                     PLAYER_EXISTS | PLAYER_CPU | PLAYER_START_SEQUENCE);
        D_80164A28 = 0;
    }

    if (gModeSelection != BATTLE) {
        init_players();
    }
}

void func_8003CD78(void) {
    func_8003BE30();
}

void func_8003CD98(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if (player->type & PLAYER_EXISTS) {
        if (screenId == 0) {
            func_8002D268(player, camera, screenId, playerId);
        }
        func_8002934C(player, camera, screenId, playerId);
        if ((screenId == 0) || (screenId == 1)) {
            load_kart_palette(player, playerId, screenId, 0);
            load_kart_palette(player, playerId, screenId, 1);
            load_kart_texture(player, playerId, screenId, screenId, 0);
#ifdef TARGET_N64
            mio0decode((u8*) &gEncodedKartTexture[0][screenId][playerId],
                       (u8*) &D_802BFB80.arraySize8[0][screenId][playerId]);
#endif
        } else {
            load_kart_palette(player, playerId, screenId, 0);
            load_kart_palette(player, playerId, screenId, 1);
            load_kart_texture(player, (s8) (playerId + 4), screenId, (s8) (screenId - 2), 0);
#ifdef TARGET_N64
            mio0decode((u8*) &gEncodedKartTexture[0][screenId - 2][playerId + 4],
                       (u8*) &D_802BFB80.arraySize8[0][screenId - 2][playerId + 4]);
#endif
        }

        gLastAnimFrameSelector[screenId][playerId] = player->animFrameSelector[screenId];
        gLastAnimGroupSelector[screenId][playerId] = player->animGroupSelector[screenId];
        D_80165150[screenId][playerId] = player->unk_0A8;
        D_801651D0[screenId][playerId] = 0;
        render_player(player, playerId, screenId);
    }
}

void spawn_players_and_cameras(void) {
    UNUSED s32 pad;
    Player* player = &gPlayers[0];
    Camera* camera;

    // Load textures for balloons and kart shadows
    func_8005D290();
    // Spawn players
    if (gGamestate == ENDING) {
        func_8003CD78();
    } else {
        func_8003C0F0();
    }

    u32 mode; // set camera mode
    switch (gModeSelection) {
        case GRAND_PRIX:
            if (gActiveScreenMode == SCREEN_MODE_1P) {
                mode = 8;
            } else {
                mode = 1;
            }
            break;
        case TIME_TRIALS:
            mode = 1;
            break;
        case BATTLE:
            mode = 9;
            break;
        default:
            mode = 1;
            break;
    }

    if (gDemoMode) {
        mode = 3;
    }

    for (size_t i = 0; i < 4; i++) {
        D_80164A08[i] = 0;
        D_80164498[i] = 0;
    }

    if (gActiveScreenMode == SCREEN_MODE_1P) {
        spawn_single_player_camera(mode);
    } else {
        spawn_multiplayer_cameras(mode);
    }

    // Add freecam, tourcam, and lookbehind cameras
    Vec3f spawn = {player->pos[0], player->pos[1], player->pos[2]};


    camera = CM_AddFreeCamera(spawn, player->rotation[1], 1);
    D_8015F480[PLAYER_ONE].freeCamera = camera;

    if (CVarGetInteger("gFreecam", false) == true) {
        CM_SetFreeCamera(true);
        D_8015F480[PLAYER_ONE].camera = camera;
    }

    if ((CM_IsTourEnabled() == true) && (gModeSelection == GRAND_PRIX) && (gIsEditorPaused == false)) {
        camera = CM_AddTourCamera(spawn, player->rotation[1], 1);
        if (NULL != camera) {
            CM_AttachCamera(camera, PLAYER_ONE);
            D_8015F480[PLAYER_ONE].camera = camera;
            D_8015F480[PLAYER_ONE].pendingCamera = NULL;
            CM_CameraSetActive(0, false);
            CM_ActivateTourCamera(camera);
        }
    }
}

void spawn_single_player_camera(u32 mode) {
    Vec3f spawn = {gPlayerOne->pos[0], gPlayerOne->pos[1], gPlayerOne->pos[2]};

    // Technically there should be a default case of mode 10 here. Except it never gets used.
    if (gModeSelection == GRAND_PRIX && !gDemoMode) {
        if (IsToadsTurnpike()) {
            spawn[0] = 0.0f;
            spawn[2] = D_80165230[7];
        } else {
            spawn[0] = (D_80165210[7] + D_80165210[6]) / 2;
            spawn[2] = D_80165230[7];

        }
    }

    Camera* camera = CM_AddCamera(spawn, gPlayerOne->rotation[1], mode);
    if (camera) {
        CM_AttachCamera(camera, PLAYER_ONE);
        D_8015F480[PLAYER_ONE].camera = camera;
        D_8015F480[PLAYER_ONE].raceCamera = camera;
    }

    camera = CM_AddLookBehindCamera(spawn, gPlayerOne->rotation[1], mode);
    if (camera) {
        CM_AttachCamera(camera, PLAYER_ONE);
        D_8015F480[PLAYER_ONE].lookBehindCamera = camera;
    }
}

void spawn_multiplayer_cameras(u32 mode) {
    Camera* camera;
    for (size_t i = 0; i <= gPlayerCountSelection1; i++) {
        Vec3f spawn = {gPlayers[i].pos[0], gPlayers[i].pos[1], gPlayers[i].pos[2]};
        camera = CM_AddCamera(spawn, gPlayers[i].rotation[1], mode);
        if (camera) {
            CM_AttachCamera(camera, i);
            D_8015F480[i].camera = camera;
            D_8015F480[i].raceCamera = camera;
        }

    }

    for (size_t i = 0; i <= gPlayerCountSelection1; i++) {
        Vec3f spawn = {gPlayers[i].pos[0], gPlayers[i].pos[1], gPlayers[i].pos[2]};
        camera = CM_AddLookBehindCamera(spawn, gPlayers[i].rotation[1], mode);
        if (camera) {
            CM_AttachCamera(camera, i);
            D_8015F480[i].lookBehindCamera = camera;
        }
    }

}

/**
 * Loads 8 players per screen in 1p/2p mode
 * Loads 4 players per screen in 3p/4p mode
 */
void load_kart_textures(void) {
    size_t screens = 0;
    switch(gActiveScreenMode) {
        case SCREEN_MODE_1P:
            screens = 1;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            screens = 2;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            screens = 4;
            break;
    }

    static const size_t playerCounts[4] = { 8, 8, 4, 4 };
    for (size_t i = 0; i < screens; i++) {
        for (size_t ply = 0; ply < playerCounts[gPlayerCountSelection1]; ply++) {
            func_8003CD98(&gPlayers[ply], D_8015F480[i].camera, ply, i);
        }
    }
}

void func_8003DB5C(void) {
    Player* player = gPlayerOne;
    Camera* camera;
    s32 playerId;

    Vec3f spawn = {player->pos[0], player->pos[1], player->pos[2]};
    camera = CM_AddCamera(spawn, player->rotation[1], 3);
    if (camera) {
        CM_AttachCamera(camera, PLAYER_ONE);
    }
    camera = CM_AddCamera(spawn, player->rotation[1], 3);
    if (camera) {
        CM_AttachCamera(camera, PLAYER_ONE);
    }

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++, player++) {
        load_kart_palette(player, playerId, 1, 0);
        load_kart_palette(player, playerId, 1, 1);
    }
}
