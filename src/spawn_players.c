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
#include "staff_ghosts.h"
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
f32 D_80165280[8];
f32 D_801652A0[8];
s32 D_801652C0[8];
s32 D_801652E0[8];
s16 D_80165300[8];
// Shadows values from gPathIndexByPlayerId, but is an array
u16 gCopyPathIndexByPlayerId[8];
// Shadows values from gNearestWaypointByPlayerId, but is an array
s16 gCopyNearestWaypointByPlayerId[8];
s16 D_80165330[8];
s16 D_80165340;
UNUSED s32 D_80165348[29];
Player* D_801653C0[8];

bool gPlayerIsThrottleActive[8];
s32 D_80165400[8];
s32 gFrameSinceLastACombo[8];
s32 gCountASwitch[8];
bool gIsPlayerTripleAButtonCombo[8];
s32 gTimerBoostTripleACombo[8];

bool gPlayerIsBrakeActive[8];
s32 D_801654C0[8];
s32 gFrameSinceLastBCombo[8];
s32 gCountBChangement[8];
bool gIsPlayerTripleBButtonCombo[8];
s32 gTimerBoostTripleBCombo[8];

s16 chooseKartAIPlayers[7];

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
    player->unk_08C = 0;
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

    D_80164510[playerIndex] = ret;

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
    player->unk_07A = 0;
    player->unk_006 = 0;
    player->lapCount = -1;
    player->unk_08C = 0.0f;
    player->unk_090 = 0.0f;
    player->unk_094 = 0.0f;
    player->unk_074 = 0.0f;
    player->type = playerType;
    player->unk_0CA = 0;
    player->unk_0DE = 0;
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
    player->unk_0C6 = 0xFF;

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
    player->unk_0DE = 0;
    player->unk_0E0 = 0;
    player->unk_0E2 = 0;
    player->unk_10C = 0;
    player->unk_200 = 0;
    player->unk_204 = 0;
    player->nearestWaypointId = 0;
    player->unk_228 = 0;
    player->unk_22A = 0;
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
    player->unk_08C = 0.0f;
    player->unk_090 = 0.0f;
    player->unk_094 = 0.0f;
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
    player->unk_108 = 0.0f;
    player->unk_1F8 = 0.0f;
    player->unk_1FC = 0.0f;
    player->unk_208 = 0.0f;
    player->unk_20C = 0.0f;
    player->unk_210 = 0.0f;
    player->unk_218 = 0.0f;
    player->unk_21C = 0.0f;
    player->unk_22C = 0.0f;
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
    D_80165070[playerIndex][0] = 0.0f;
    D_80165070[playerIndex][1] = 0.0f;
    D_80165070[playerIndex][2] = 0.0f;
    D_80165280[playerIndex] = 0.0f;
    D_801652A0[playerIndex] = 0.0f;
    gPlayerIsThrottleActive[playerIndex] = 0;
    D_80165400[playerIndex] = 0;
    gPlayerIsBrakeActive[playerIndex] = 0;
    D_801654C0[playerIndex] = 0;
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
    func_8005D6C0(player);
    func_8006B87C(player, playerIndex);
    if (gModeSelection == BATTLE) {
        func_8006B7E4(player, playerIndex);
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

    s32 sp2C[] = {
        7, 6, 5, 4, 3, 2, 1, 0,
    };

    s32 spC[] = {
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
        chooseKartAIPlayers[0] = rand;

        // Chooses arr[0] as a fallback to prevent duplicating characters.
        // If it doesn't find the if, it will grab the final index as a fallback.
        for (i = 1; i < 7; i++) {
            u16* arr = (u16*) gKartAIforPlayer[gCharacterSelections[0]];
            if (rand == arr[i]) {
                chooseKartAIPlayers[i] = arr[0];
            } else {
                chooseKartAIPlayers[i] = arr[i];
            }
        }
    }

    D_8016556E = 0;
    if (gDemoMode == 1) {
        spawn_player(gPlayerOneCopy, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, chooseKartAIPlayers[0],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerThree, 2, arg0[D_80165270[2]], arg1[D_80165270[2]], arg2, 32768.0f, chooseKartAIPlayers[1],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFour, 3, arg0[D_80165270[3]], arg1[D_80165270[3]], arg2, 32768.0f, chooseKartAIPlayers[2],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFive, 4, arg0[D_80165270[4]], arg1[D_80165270[4]], arg2, 32768.0f, chooseKartAIPlayers[3],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSix, 5, arg0[D_80165270[5]], arg1[D_80165270[5]], arg2, 32768.0f, chooseKartAIPlayers[4],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSeven, 6, arg0[D_80165270[6]], arg1[D_80165270[6]], arg2, 32768.0f, chooseKartAIPlayers[5],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerEight, 7, arg0[D_80165270[7]], arg1[D_80165270[7]], arg2, 32768.0f, chooseKartAIPlayers[6],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        D_80164A28 = 0;
    } else {
        if (gNetwork.enabled) {
            spawn_network_players(arg0, arg1, arg2);
        } else {
            spawn_player(gPlayerOneCopy, 0, arg0[D_80165270[0]], arg1[D_80165270[0]] + 250.0f, arg2, 32768.0f,
                         gCharacterSelections[0],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
            spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[0],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerThree, 2, arg0[D_80165270[3]], arg1[D_80165270[2]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[1],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerFour, 3, arg0[D_80165270[2]], arg1[D_80165270[3]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[2],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerFive, 4, arg0[D_80165270[5]], arg1[D_80165270[4]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[3],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerSix, 5, arg0[D_80165270[4]], arg1[D_80165270[5]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[4],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerSeven, 6, arg0[D_80165270[7]], arg1[D_80165270[6]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[5],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
            spawn_player(gPlayerEight, 7, arg0[D_80165270[6]], arg1[D_80165270[7]] + 250.0f, arg2, 32768.0f,
                         chooseKartAIPlayers[6],
                         PLAYER_EXISTS | PLAYER_STAGING | PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        }
        D_80164A28 = 1;
    }
    func_80039AE4();
}

void spawn_players_versus_one_player(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    } else if (D_8015F890 != 1) {
        spawn_player(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        if (D_80162DD4 == 0) {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE0,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        }
        if (D_80162DD6 == 0) {
            spawn_player(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE4,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        }
    } else {
        spawn_player(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE8,
                     PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        if (D_80162DD8 == 0) {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE0,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_KART_AI);
        }
        if (D_80162DD6 == 0) {
            spawn_player(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, D_80162DE4,
                         PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_START_SEQUENCE | PLAYER_INVISIBLE_OR_BOMB);
        } else {
            spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                         PLAYER_START_SEQUENCE | PLAYER_KART_AI);
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

        chooseKartAIPlayers[0] = rand;

        for (i = 1; i < 6; i++) {
            u16* arr = (u16*) gKartAIforTwoPlayer[gCharacterSelections[0]][gCharacterSelections[1]];
            if (rand == arr[i]) {
                chooseKartAIPlayers[i] = arr[0];
            } else {
                chooseKartAIPlayers[i] = arr[i];
            }
        }
    }

    spawn_player(gPlayerThree, 2, arg0[D_80165270[2]], arg1[D_80165270[2]], arg2, 32768.0f, chooseKartAIPlayers[0],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerFour, 3, arg0[D_80165270[3]], arg1[D_80165270[3]], arg2, 32768.0f, chooseKartAIPlayers[1],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerFive, 4, arg0[D_80165270[4]], arg1[D_80165270[4]], arg2, 32768.0f, chooseKartAIPlayers[2],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerSix, 5, arg0[D_80165270[5]], arg1[D_80165270[5]], arg2, 32768.0f, chooseKartAIPlayers[3],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerSeven, 6, arg0[D_80165270[6]], arg1[D_80165270[6]], arg2, 32768.0f, chooseKartAIPlayers[4],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerEight, 7, arg0[D_80165270[7]], arg1[D_80165270[7]], arg2, 32768.0f, chooseKartAIPlayers[5],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);

    if (gDemoMode == 1) {
        spawn_player(gPlayerOneCopy, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_HUMAN_AND_KART_AI);
    } else {
        spawn_player(gPlayerOneCopy, 0, arg0[D_80165270[0]], arg1[D_80165270[0]], arg2, 32768.0f,
                     gCharacterSelections[0], PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    if (gDemoMode == 1) {
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    } else {
        spawn_player(gPlayerTwo, 1, arg0[D_80165270[1]], arg1[D_80165270[1]], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }

    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_versus_two_player(f32* arg0, f32* arg1, f32 arg2) {
    spawn_player(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, gCharacterSelections[0],
                 PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    if (gDemoMode == 1) {
        spawn_player(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_KART_AI);
    } else {
        spawn_player(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    if (gDemoMode == 1) {
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_HUMAN_AND_KART_AI);
    } else {
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
    }
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_2p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (GetCourse() == GetBigDonut()) {
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
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
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
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2],
                     PLAYER_HUMAN_AND_KART_AI);
    }

    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_3p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (GetCourse() == GetBigDonut()) {
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
    spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
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
        spawn_player(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, gCharacterSelections[0],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, gCharacterSelections[1],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, gCharacterSelections[2],
                     PLAYER_HUMAN_AND_KART_AI);
        spawn_player(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, gCharacterSelections[3],
                     PLAYER_HUMAN_AND_KART_AI);
    }
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    D_80164A28 = 0;
    func_80039AE4();
}

void spawn_players_4p_battle(f32* arg0, f32* arg1, f32 arg2) {
    if (GetCourse() == GetBigDonut()) {
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
    spawn_player(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    spawn_player(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, PLAYER_START_SEQUENCE | PLAYER_KART_AI);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003BE30(void) {
    spawn_player(gPlayerOne, 0, -2770.774f, -345.187f, -34.6f, 0.0f, gCharacterIdByGPOverallRank[0],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerTwo, 1, -3691.506f, -6.822f, -6.95f, 36400.0f, gCharacterIdByGPOverallRank[1],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    spawn_player(gPlayerThree, 2, -3475.028f, -998.485f, -8.059f, 45500.0f, gCharacterIdByGPOverallRank[2],
                 PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    if (D_802874D8.unk1D >= 3) {
        spawn_player(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, 28210.0f, D_802874D8.unk1E,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    } else {
        spawn_player(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, 28210.0f, gCharacterIdByGPOverallRank[3],
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
    }
    spawn_player(gPlayerFive, 4, -2770.774f, -345.187f, -34.6f, 0.0f, 0, 0x7000);
    spawn_player(gPlayerSix, 5, -3691.506f, -6.822f, -6.95f, 36400.0f, 0, 0x7000);
    spawn_player(gPlayerSeven, 6, -3475.028f, -998.485f, -8.059f, 45500.0f, 0, 0x7000);
    spawn_player(gPlayerEight, 7, -3025.772f, 110.039f, -23.224f, 28210.0f, 0, 0x7000);
    D_80164A28 = 0;
    func_80039AE4();
}

void func_8003C0F0(void) {
    s16 sp5E;
    s16 sp5C;
    s16 sp5A;
    s32 temp;
    UNUSED s32 pad[4];

    if (gModeSelection == BATTLE) {
        func_8000EEDC();
    } else if (GetCourse() != GetPodiumCeremony()) {
        func_8000F2DC();
        sp5E = (f32) D_80164550[0][0].posX;
        sp5C = (f32) D_80164550[0][0].posZ;
        sp5A = (f32) D_80164550[0][0].posY;
        if (GetCourse() == GetToadsTurnpike()) {
            sp5E = 0;
        }
    }

    if ((gModeSelection != BATTLE) && (GetCourse() != GetPodiumCeremony())) {
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
    } else if (GetCourse() == GetBlockFort()) {
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
    } else if (GetCourse() == GetSkyscraper()) {
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
    } else if (GetCourse() == GetDoubleDeck()) {
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
    } else if (GetCourse() == GetBigDonut()) {
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
        spawn_player(gPlayerOneCopy, 0, D_80165210[0], D_80165230[0], sp5A, 32768.0f, gCharacterSelections[0],
                     PLAYER_EXISTS | PLAYER_START_SEQUENCE | PLAYER_HUMAN);
        spawn_player(gPlayerTwo, 1, D_80165210[1], D_80165230[1], sp5A, 32768.0f, 1,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerThree, 2, D_80165210[2], D_80165230[2], sp5A, 32768.0f, 2,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFour, 3, D_80165210[3], D_80165230[3], sp5A, 32768.0f, 3,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerFive, 4, D_80165210[4], D_80165230[4], sp5A, 32768.0f, 4,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSix, 5, D_80165210[5], D_80165230[5], sp5A, 32768.0f, 5,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerSeven, 6, D_80165210[6], D_80165230[6], sp5A, 32768.0f, 6,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        spawn_player(gPlayerEight, 7, D_80165210[7], D_80165230[7], sp5A, 32768.0f, 7,
                     PLAYER_EXISTS | PLAYER_KART_AI | PLAYER_START_SEQUENCE);
        D_80164A28 = 0;
    }

    if (gModeSelection != BATTLE) {
        func_8000F628();
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

void func_8003D080(void) {
    UNUSED s32 pad;
    Player* player = &gPlayers[0];

    func_8005D290();
    if (gGamestate == ENDING) {
        func_8003CD78();
    } else {
        func_8003C0F0();
    }
    if (!gDemoMode) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                switch (gModeSelection) {
                    case GRAND_PRIX:
                        if (GetCourse() == GetToadsTurnpike()) {
                            camera_init(0.0f, player->pos[1], D_80165230[7], player->rotation[1], 8, 0);
                        } else {
                            camera_init((D_80165210[7] + D_80165210[6]) / 2, player->pos[1], D_80165230[7],
                                        player->rotation[1], 8, 0);
                        }
                        break;

                    case TIME_TRIALS:
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 0);
                        break;

                    default:
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 10, 0);
                        break;
                }
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                switch (gModeSelection) {
                    case GRAND_PRIX:
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 0);
                        player++;
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 1);
                        break;

                    case BATTLE:
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 0);
                        player++;
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 1);
                        break;

                    default:
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 0);
                        player++;
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 1);
                        break;
                }
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (gModeSelection == BATTLE) {
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 0);
                    player++;
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 1);
                    player++;
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 2);
                    if (gPlayerCountSelection1 == 4) {
                        player++;
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 9, 3);
                    }
                } else {
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 0);
                    player++;
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 1);
                    player++;
                    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 2);
                    if (gPlayerCountSelection1 == 4) {
                        player++;
                        camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 1, 3);
                    }
                }
                break;
        }
    } else {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 0);
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 0);
                player++;
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 1);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 0);
                player++;
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 1);
                player++;
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 2);
                player++;
                camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 3);
                break;
        }
    }

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            func_8003CD98(gPlayerOneCopy, camera1, 0, 0); // sic
            func_8003CD98(gPlayerTwo, camera1, 1, 0);
            func_8003CD98(gPlayerThree, camera1, 2, 0);
            func_8003CD98(gPlayerFour, camera1, 3, 0);
            func_8003CD98(gPlayerFive, camera1, 4, 0);
            func_8003CD98(gPlayerSix, camera1, 5, 0);
            func_8003CD98(gPlayerSeven, camera1, 6, 0);
            func_8003CD98(gPlayerEight, camera1, 7, 0);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            func_8003CD98(gPlayerOneCopy, camera1, 0, 0);
            func_8003CD98(gPlayerTwo, camera1, 1, 0);
            func_8003CD98(gPlayerThree, camera1, 2, 0);
            func_8003CD98(gPlayerFour, camera1, 3, 0);
            func_8003CD98(gPlayerFive, camera1, 4, 0);
            func_8003CD98(gPlayerSix, camera1, 5, 0);
            func_8003CD98(gPlayerSeven, camera1, 6, 0);
            func_8003CD98(gPlayerEight, camera1, 7, 0);
            func_8003CD98(gPlayerOneCopy, camera2, 0, 1);
            func_8003CD98(gPlayerTwo, camera2, 1, 1);
            func_8003CD98(gPlayerThree, camera2, 2, 1);
            func_8003CD98(gPlayerFour, camera2, 3, 1);
            func_8003CD98(gPlayerFive, camera2, 4, 1);
            func_8003CD98(gPlayerSix, camera2, 5, 1);
            func_8003CD98(gPlayerSeven, camera2, 6, 1);
            func_8003CD98(gPlayerEight, camera2, 7, 1);
            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            func_8003CD98(gPlayerOneCopy, camera1, 0, 0);
            func_8003CD98(gPlayerTwo, camera1, 1, 0);
            func_8003CD98(gPlayerThree, camera1, 2, 0);
            func_8003CD98(gPlayerFour, camera1, 3, 0);
            func_8003CD98(gPlayerOneCopy, camera2, 0, 1);
            func_8003CD98(gPlayerTwo, camera2, 1, 1);
            func_8003CD98(gPlayerThree, camera2, 2, 1);
            func_8003CD98(gPlayerFour, camera2, 3, 1);
            func_8003CD98(gPlayerOneCopy, camera3, 0, 2);
            func_8003CD98(gPlayerTwo, camera3, 1, 2);
            func_8003CD98(gPlayerThree, camera3, 2, 2);
            func_8003CD98(gPlayerFour, camera3, 3, 2);
            func_8003CD98(gPlayerOneCopy, camera4, 0, 3);
            func_8003CD98(gPlayerTwo, camera4, 1, 3);
            func_8003CD98(gPlayerThree, camera4, 2, 3);
            func_8003CD98(gPlayerFour, camera4, 3, 3);
            break;
    }
}

void func_8003DB5C(void) {
    Player* player = gPlayerOne;
    s32 playerId;

    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 0);
    camera_init(player->pos[0], player->pos[1], player->pos[2], player->rotation[1], 3, 1);

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++, player++) {
        load_kart_palette(player, playerId, 1, 0);
        load_kart_palette(player, playerId, 1, 1);
    }
}
