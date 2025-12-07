#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <mk64.h>

#include "player_controller.h"
#include "code_800029B0.h"
#include "kart_attributes.h"
#include "racing/memory.h"
#include "math_util.h"
#include "render_player.h"
#include "effects.h"
#include "collision.h"
#include "waypoints.h"
#include "audio/external.h"
#include "code_8003DC40.h"
#include "main.h"
#include "camera.h"
#include "spawn_players.h"
#include "code_80057C60.h"
#include "code_80005FD0.h"
#include "sounds.h"
#include "port/Game.h"
#include "src/enhancements/moon_jump.h"
#include "engine/Matrix.h"

extern s32 D_8018D168;

s16 cpu_forMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER, 0 };

s16 cpu_forPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER, 0 };

s16 cpu_forBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, 0 };

s16* cpu_forPlayer[] = { cpu_forMario, cpu_forLuigi, cpu_forYoshi, cpu_forToad,
                         cpu_forDk,    cpu_forWario, cpu_forPeach, cpu_forBowser };

s16 cpu_forMarioAndLuigi[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndYoshi[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndToad[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndDk[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndWario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forMarioAndPeach[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forMarioAndBowser[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forLuigiAndMario[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndYoshi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndToad[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndDk[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndWario[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forLuigiAndPeach[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forLuigiAndBowser[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forYoshiAndMario[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndLuigi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndToad[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndDk[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndWario[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forYoshiAndPeach[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forYoshiAndBowser[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 cpu_forToadAndMario[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndLuigi[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndYoshi[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndDk[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndWario[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 cpu_forToadAndPeach[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 cpu_forToadAndBowser[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 cpu_forDkAndMario[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndLuigi[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndToad[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndYoshi[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 cpu_forDkAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 cpu_forDkAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 cpu_forWarioAndMario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndLuigi[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndYoshi[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 cpu_forWarioAndToad[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 cpu_forWarioAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 cpu_forWarioAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 cpu_forPeachAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 cpu_forPeachAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 cpu_forPeachAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 cpu_forPeachAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16 cpu_forBowserAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 cpu_forBowserAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 cpu_forBowserAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 cpu_forBowserAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16* gListCPUforMario[] = { cpu_forMarioAndLuigi, cpu_forMarioAndLuigi, cpu_forMarioAndYoshi, cpu_forMarioAndToad,
                            cpu_forMarioAndDk,    cpu_forMarioAndWario, cpu_forMarioAndPeach, cpu_forMarioAndBowser };

s16* gListCPUforLuigi[] = { cpu_forLuigiAndMario, cpu_forLuigiAndMario, cpu_forLuigiAndYoshi, cpu_forLuigiAndToad,
                            cpu_forLuigiAndDk,    cpu_forLuigiAndWario, cpu_forLuigiAndPeach, cpu_forLuigiAndBowser };

s16* gListCPUforYoshi[] = { cpu_forYoshiAndMario, cpu_forYoshiAndLuigi, cpu_forYoshiAndLuigi, cpu_forYoshiAndToad,
                            cpu_forYoshiAndDk,    cpu_forYoshiAndWario, cpu_forYoshiAndPeach, cpu_forYoshiAndBowser };

s16* gListCPUforToad[] = { cpu_forToadAndMario, cpu_forToadAndLuigi, cpu_forToadAndYoshi, cpu_forToadAndYoshi,
                           cpu_forToadAndDk,    cpu_forToadAndWario, cpu_forToadAndPeach, cpu_forToadAndBowser };

s16* gListCPUforDk[] = { cpu_forDkAndMario, cpu_forDkAndLuigi, cpu_forDkAndToad,  cpu_forDkAndYoshi,
                         cpu_forDkAndYoshi, cpu_forDkAndWario, cpu_forDkAndPeach, cpu_forDkAndBowser };

s16* gListCPUforWario[] = { cpu_forWarioAndMario, cpu_forWarioAndLuigi, cpu_forWarioAndYoshi, cpu_forWarioAndToad,
                            cpu_forWarioAndDk,    cpu_forWarioAndDk,    cpu_forWarioAndPeach, cpu_forWarioAndBowser };

s16* gListCPUforPeach[] = { cpu_forPeachAndMario, cpu_forPeachAndLuigi, cpu_forPeachAndYoshi, cpu_forPeachAndToad,
                            cpu_forPeachAndDk,    cpu_forPeachAndWario, cpu_forPeachAndDk,    cpu_forPeachAndBowser };

s16* gListCPUforBowser[] = {
    cpu_forBowserAndMario, cpu_forBowserAndLuigi, cpu_forBowserAndYoshi, cpu_forBowserAndToad,
    cpu_forBowserAndDk,    cpu_forBowserAndWario, cpu_forBowserAndPeach, cpu_forBowserAndPeach
};

s16** cpu_forTwoPlayer[] = { gListCPUforMario, gListCPUforLuigi, gListCPUforYoshi, gListCPUforToad,
                             gListCPUforDk,    gListCPUforWario, gListCPUforPeach, gListCPUforBowser };

// func_80027D00
s32 get_player_index_for_player(Player* player) {
    s32 index;

    if (player == gPlayerOne) {
        index = 0;
    }
    if (player == gPlayerTwo) {
        index = 1;
    }
    if (player == gPlayerThree) {
        index = 2;
    }
    if (player == gPlayerFour) {
        index = 3;
    }
    if (player == gPlayerFive) {
        index = 4;
    }
    if (player == gPlayerSix) {
        index = 5;
    }
    if (player == gPlayerSeven) {
        index = 6;
    }
    if (player == gPlayerEight) {
        index = 7;
    }
    return index;
}

void func_80027DA8(Player* player, s8 playerId) {
    if (D_8015F890 != 1) {
        if ((player->type & 0x10) != 0x10) {
            if (((D_8018D168 == 1) && ((player->type & 0x4000) == 0x4000)) && ((player->type & 0x100) != 0x100)) {
                func_800C94A4(playerId);
                player->type |= 0x10;
            } else if ((player->type & 0x2000) == 0) {
                func_800C9A88(playerId);
                player->type |= 0x10;
            }
        }
    } else if ((player->type & 0x10) != 0x10) {
        if ((D_8018D168 == 1) && (player == gPlayerOne)) {
            func_800C94A4(playerId);
            player->type |= 0x10;
        } else if ((player->type & 0x2000) == 0) {
            func_800C9A88(playerId);
            player->type |= 0x10;
        }
    }
}

void func_80027EDC(Player* player, s8 playerId) {
    UNUSED s32 pad;
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {

        CM_WhatDoesThisDo(player, playerId);

        // switch (gCurrentCourseId) {
        //     // case COURSE_MARIO_RACEWAY:
        //     //     if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        //     //         ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
        //     //         if (D_80165300[playerId] != 1) {
        //     //             func_800CA288(playerId, 0x55);
        //     //         }
        //     //         D_80165300[playerId] = 1;
        //     //     } else {
        //     //         if (D_80165300[playerId] != 0) {
        //     //             func_800CA2B8(playerId);
        //     //             D_80165300[playerId] = 0;
        //     //         }
        //     //     }
        //         break;
        //     case COURSE_CHOCO_MOUNTAIN:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0xA0) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0xB4)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_BOWSER_CASTLE:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x41);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_BANSHEE_BOARDWALK:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x41);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_LUIGI_RACEWAY:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_TOADS_TURNPIKE:
        //         if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x1e);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_SHERBET_LAND:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x11C) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x209)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA288(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA2B8(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_DK_JUNGLE:
        //         if ((((s16) gNearestPathPointByPlayerId[playerId] >= 0) &&
        //              ((s16) gNearestPathPointByPlayerId[playerId] < 0x65)) ||
        //             (((s16) gNearestPathPointByPlayerId[playerId] >= 0x14A) &&
        //              ((s16) gNearestPathPointByPlayerId[playerId] < 0x21F))) {
        //             if (D_80165300[playerId] != 2) {
        //                 func_800C8F80(playerId, 0x0170802D);
        //             }
        //             D_80165300[playerId] = 2;
        //         } else {
        //             if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x288) &&
        //                 ((s16) gNearestPathPointByPlayerId[playerId] < 0x305)) {
        //                 if (D_80165300[playerId] != 1) {
        //                     func_800CA288(playerId, 0x55);
        //                 }
        //                 D_80165300[playerId] = 1;
        //             } else {
        //                 if (D_80165300[playerId] != 0) {
        //                     if (D_80165300[playerId] == 1) {
        //                         func_800CA2B8(playerId);
        //                     }
        //                     if (D_80165300[playerId] == 2) {
        //                         func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x70, 0x80, 0x2D));
        //                     }
        //                     D_80165300[playerId] = 0;
        //                 }
        //             }
        //         }
        //         break;
        //     default:
        //         break;
        // }
    } else {

        CM_WhatDoesThisDoAI(player, playerId);

        // switch (gCurrentCourseId) {
        //     case COURSE_MARIO_RACEWAY:
        //         // if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        //         //     ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
        //         //     if (D_80165300[playerId] != 1) {
        //         //         func_800CA2E4(playerId, 0x55);
        //         //     }
        //         //     D_80165300[playerId] = 1;
        //         // } else {
        //         //     if (D_80165300[playerId] != 0) {
        //         //         func_800CA30C(playerId);
        //         //         D_80165300[playerId] = 0;
        //         //     }
        //         // }
        //         break;
        //     case COURSE_CHOCO_MOUNTAIN:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0xA0) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0xB4)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_BOWSER_CASTLE:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x41);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_BANSHEE_BOARDWALK:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x41);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_LUIGI_RACEWAY:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_TOADS_TURNPIKE:
        //         if ((player->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x1E);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_SHERBET_LAND:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x11C) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x209)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     case COURSE_DK_JUNGLE:
        //         if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x288) &&
        //             ((s16) gNearestPathPointByPlayerId[playerId] < 0x305)) {
        //             if (D_80165300[playerId] != 1) {
        //                 func_800CA2E4(playerId, 0x55);
        //             }
        //             D_80165300[playerId] = 1;
        //         } else {
        //             if (D_80165300[playerId] != 0) {
        //                 func_800CA30C(playerId);
        //                 D_80165300[playerId] = 0;
        //             }
        //         }
        //         break;
        //     default:
        //         break;
        // }
    }
}

u16 check_player_camera_collision_rubberbanding(Player* player, Camera* camera, f32 arg2, f32 arg3) {
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

    if (CVarGetInteger("gDisableRubberbanding", 0) != 0) {
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

void func_80028864(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    u16 sp1E;

    if (!(player->type & PLAYER_START_SEQUENCE)) {
        switch (gActiveScreenMode) {
            case SCREEN_MODE_1P:
                sp1E = check_player_camera_collision_rubberbanding(player, camera1, (f32) D_8016557C, 0.0f);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                sp1E = check_player_camera_collision_rubberbanding(player, camera1, (f32) D_8016557C, 0.0f);
                if (sp1E == 1) {
                    break;
                }
                sp1E = check_player_camera_collision_rubberbanding(player, camera2, (f32) D_8016557C, 0.0f);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                sp1E = check_player_camera_collision_rubberbanding(player, camera1, (f32) D_8016557C, 0.0f);
                if (sp1E == 1) {
                    break;
                }
                sp1E = check_player_camera_collision_rubberbanding(player, camera2, (f32) D_8016557C, 0.0f);
                if (sp1E == 1) {
                    break;
                }
                sp1E = check_player_camera_collision_rubberbanding(player, camera3, (f32) D_8016557C, 0.0f);
                if (sp1E == 1) {
                    break;
                }
                sp1E = check_player_camera_collision_rubberbanding(player, camera4, (f32) D_8016557C, 0.0f);
                break;
        }
        if ((sp1E == 1) || ((player->type & PLAYER_INVISIBLE_OR_BOMB) == PLAYER_INVISIBLE_OR_BOMB) ||
            (gModeSelection == BATTLE) || ((player->unk_0CA & 2) != 0) || (player->unk_0CA & 8) ||
            //! @todo make a proper match
            ((*(D_801633F8 + (playerId))) == ((s16) 1U))) {
            player->effects &= ~0x1000;
            if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
                ((player->effects & 0x400) == 0x400) || ((player->effects & 0x4000) == 0x4000) ||
                ((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
                ((player->effects & 0x01000000) == 0x01000000) ||
                ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) ||
                ((player->effects & 0x20000) == 0x20000) || (player->unk_044 & 0x800)) {
                func_8002E594(player, camera, screenId, playerId);
            } else {
                func_8002D268(player, camera, screenId, playerId);
            }
        } else {
            control_cpu_movement(player, camera, screenId, playerId);
        }
    } else if ((player->type & PLAYER_STAGING) == PLAYER_STAGING) {
        func_8002D028(player, playerId);
        func_8002F730(player, camera, screenId, playerId);
    } else if (player->type & 0x80) {
        func_8002D268(player, camera, screenId, playerId);
    } else {
        if ((player->type & PLAYER_HUMAN) != PLAYER_HUMAN) {
            player->currentSpeed = 50.0f;
        }
        player->effects &= ~8;
    }
}

void func_80028C44(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((player->type & PLAYER_START_SEQUENCE) == 0) {
        player->effects &= ~0x1000;
        if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
            ((player->effects & 0x400) == 0x400) || ((player->effects & 0x4000) == 0x4000) ||
            ((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
            ((player->effects & 0x1000000) == 0x1000000) ||
            ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) ||
            ((player->effects & 0x20000) == 0x20000) || ((player->unk_044 & 0x800) != 0)) {
            func_8002E594(player, camera, screenId, playerId);
        } else {
            func_8002D268(player, camera, screenId, playerId);
        }
    } else {
        player->effects &= -9;
    }
}

void func_80028D3C(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((((player->type & PLAYER_START_SEQUENCE) == 0) && (gRaceState != RACE_FINISHED)) ||
        (player->unk_0CA & 2) != 0 || (player->unk_0CA & 8) != 0 || (player->effects & 0x4F010CC0) != 0) {
        player->effects &= ~0x1000;

        if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
            ((player->effects & 0x400) == 0x400) || ((player->effects & 0x4000) == 0x4000) ||
            ((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
            ((player->effects & 0x1000000) == 0x1000000) ||
            ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) ||
            ((player->effects & 0x20000) == 0x20000) || ((player->unk_044 & 0x800) != 0)) {
            func_8002E594(player, camera, screenId, playerId);
        } else {
            func_8002D268(player, camera, screenId, playerId);
        }
    } else {
        player->effects = player->effects & ~8;
    }
}

void func_80028E70(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        switch (gGamestate) {
            case ENDING:
                if (!(player->type & PLAYER_START_SEQUENCE)) {
                    func_80038C6C(player, camera, screenId, playerId);
                } else {
                    player->effects &= ~8;
                }
                break;
            default:
                func_80027DA8(player, playerId);
                switch (gModeSelection) {
                    case TIME_TRIALS:
                    case VERSUS:
                        func_80028C44(player, camera, playerId, screenId);
                        break;
                    case BATTLE:
                        func_80028D3C(player, camera, playerId, screenId);
                        break;
                    default:
                        func_80028864(player, camera, playerId, screenId);
                        break;
                }
                break;
        }
    }
}

void func_80028F70(void) {
    // ClearEffectsMatrixPool();
    gMatrixEffectCount = 0;

    switch(gActiveScreenMode) {
        default:
            for (size_t i = 0; i < NUM_PLAYERS; i++) {
                func_80028E70(&gPlayers[i], camera1, i, 0);
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            for (size_t i = 0; i < 4; i++) {
                func_80028E70(&gPlayers[i], camera1, i, 0);
            }
            break;
    }
}

void func_80029200(Player* player, s8 screenId) {
    if ((s32) player->slopeAccel < -0x71B) {
        player->animGroupSelector[screenId] = 0;
    }
    if (((s32) player->slopeAccel < -0x4F9) && ((s32) player->slopeAccel >= -0x71B)) {
        player->animGroupSelector[screenId] = 1;
    }
    if ((player->slopeAccel < -0x221) && (player->slopeAccel >= -0x4F9)) {
        player->animGroupSelector[screenId] = 2;
    }
    if ((player->slopeAccel < -0x16B) && (player->slopeAccel >= -0x221)) {
        player->animGroupSelector[screenId] = 3;
    }
    if ((player->slopeAccel < 0x16C) && (player->slopeAccel >= -0x16B)) {
        player->animGroupSelector[screenId] = 4;
    }
    if ((player->slopeAccel >= 0x16C) && (player->slopeAccel < 0x222)) {
        player->animGroupSelector[screenId] = 5;
    }
    if ((player->slopeAccel >= 0x222) && (player->slopeAccel < 0x4FA)) {
        player->animGroupSelector[screenId] = 6;
    }
    if ((player->slopeAccel >= 0x4FA) && (player->slopeAccel < 0x71C)) {
        player->animGroupSelector[screenId] = 7;
    }
    if (player->slopeAccel >= 0x71C) {
        player->animGroupSelector[screenId] = 8;
    }
}

void func_8002934C(Player* player, Camera* camera, s8 screenId, s8 playerId) {
    UNUSED s32 pad[2];
    f32 temp_f0;
    f32 temp_f2;
    UNUSED s32 pad2[3];
    f32 var_f0;
    s16 temp_a0;
    s32 temp_a0_2;
    s32 var_a1;
    s32 var_t0;
    u16 var_a0;

    player->unk_048[screenId] = atan2s(player->pos[0] - camera->pos[0], player->pos[2] - camera->pos[2]);
    player->animFrameSelector[screenId] =
        (u16) ((((player->unk_048[screenId]) + player->rotation[1] + player->unk_0C0))) / 128;

    temp_f2 = (gCharacterSize[player->characterId] * 18.0f) * player->size;
    temp_f0 = player->unk_230 - player->unk_23C;
    if ((player->effects & 8) != 8) {
        if ((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) {
            player->unk_0CC[screenId] = (s16) ((s32) (((f64) func_802B7C40(temp_f0 / temp_f2)) * 1.6));
        } else {
            player->unk_0CC[screenId] = func_802B7C40(temp_f0 / temp_f2) * 2;
        }
    }
    if ((player->effects & HIT_EFFECT) == HIT_EFFECT) {
        player->unk_0CC[screenId] = (s16) ((s32) player->unk_D9C);
    }
    if ((player->effects & 8) != 8) {
        temp_f0 = player->unk_1F8 - player->unk_1FC;
        player->unk_0D4[screenId] = (((func_802B7C40(temp_f0 / temp_f2)) * 0.9));
    } else {
        if (((player->animFrameSelector[screenId]) >= 0) && ((player->animFrameSelector[screenId]) < 0x101)) {
            var_f0 = player->oldPos[1] - player->pos[1];
        } else {
            var_f0 = player->pos[1] - player->oldPos[1];
        }
        player->unk_0D4[screenId] = (s16) ((s32) (((f64) func_802B7C40(var_f0 / temp_f2)) * 0.5));
    }
    if ((player->effects & HIT_EFFECT) == HIT_EFFECT) {
        player->unk_0D4[screenId] = (s16) ((s32) player->unk_D9C);
    }
    func_80029200(player, screenId);
    temp_a0 = ((player->unk_048[screenId] + player->rotation[1]) + player->unk_0C0);
    temp_a0 = (s16) player->unk_0D4[screenId] * sins((u16) temp_a0) + player->unk_0CC[screenId] * coss((u16) temp_a0);
    move_s16_towards(&player->unk_050[screenId], temp_a0, 0.5f);
    var_a0 = player->animFrameSelector[screenId];
    player->unk_002 = player->unk_002 & (~(4 << (screenId * 4)));
    if (var_a0 >= 0x101) {
        var_a0 = 0x201 - var_a0;
        player->unk_002 |= (4 << (screenId * 4));
    }
    if (((player->effects & 0x80) != 0x80) && ((player->effects & 0x40) != 0x40) &&
        ((player->effects & 0x80000) != 0x80000) && ((player->effects & 0x800000) != 0x800000) &&
        ((player->effects & 0x20000) != 0x20000) && (!(player->unk_044 & 0x800))) {
        if (var_a0 < 0x51) {
            var_a1 = 0x208;
            var_t0 = 0;
        } else {
            var_a1 = 0x666;
            var_t0 = 0xF;
        }
    } else {
        var_a1 = 0x666;
        var_t0 = 0;
    }
    if (((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
        (player->unk_044 & 0x800)) {
        player->unk_050[screenId] = 0;
    }
    if (((player->effects & 8) == 8) && ((player->unk_0CA & 2) == 2)) {
        player->unk_050[screenId] = 0;
    }
    var_a0 = (player->unk_048[screenId] + player->rotation[1] + player->unk_0C0);
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
        ((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
        ((player->effects & 0x20000) == 0x20000) || (player->unk_044 & 0x800)) {
        if (var_a0 >= 0x7FF9) {
            var_a0 = -var_a0;
            var_a0 /= var_a1;
            if (var_a0 == 0) {
                var_a0 = 1;
            }
        } else {
            var_a0 /= var_a1;
        }
    } else {
        if (var_a0 >= 0x7FF9) {
            var_a0 = (-var_a0);
        }
        var_a0 /= var_a1;
    }
    player->animFrameSelector[screenId] = var_a0 + var_t0;
    if ((player->animFrameSelector[screenId]) >= 0x23) {
        player->animFrameSelector[screenId] = 0x22;
    }
    if ((player->effects & 0x80) || (player->effects & 0x40) || (player->effects & 0x80000) ||
        (player->effects & 0x800000) || (player->effects & 0x20000) || (player->unk_044 & 0x800)) {

        if ((player->animFrameSelector[screenId]) >= 0x14) {
            player->animFrameSelector[screenId] = 0;
        }
    }
    if ((player->animGroupSelector[screenId]) >= 9) {
        player->animGroupSelector[screenId] = 4;
    }
    if (((player->effects & 0x80000) == 0x80000) || ((player->effects & 0x800000) == 0x800000) ||
        (player->unk_044 & 0x800)) {

        player->animGroupSelector[screenId] = 4;
    }
    if (((player->effects & 0x400) == 0x400) || ((player->effects & 0x01000000) == 0x01000000) ||
        ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) || (player->effects & 0x10000) ||
        (player->effects & 0x80) || (player->effects & 0x40)) {

        player->unk_002 |= 1 << (screenId * 4);
        D_80165190[screenId][playerId] = 1;

        if ((player->effects & 0x80) || (player->effects & 0x40)) {
            if ((player->animFrameSelector[screenId] == gLastAnimFrameSelector[screenId][playerId]) &&
                (player->animGroupSelector[screenId] == gLastAnimGroupSelector[screenId][playerId])) {
                player->unk_002 &= ~(1 << (screenId * 4));
                D_80165190[screenId][playerId] = 1;
            }
        } else if (((player->unk_0A8) >> 8) == D_80165150[screenId][playerId] >> 8) {
            player->unk_002 &= ~(1 << (screenId * 4));
        }
    } else {
        player->unk_002 |= 1 << (screenId * 4);
        if (((player->animFrameSelector[screenId] == gLastAnimFrameSelector[screenId][playerId]) &&
             (player->animGroupSelector[screenId] == gLastAnimGroupSelector[screenId][playerId])) &&
            ((D_80165190[screenId][playerId]) == 0)) {
            player->unk_002 &= ~(1 << (screenId * 4));
        }
    }
    temp_a0_2 = gLastAnimFrameSelector[screenId][playerId] - player->animFrameSelector[screenId];
    if ((temp_a0_2 >= 0x14) || (temp_a0_2 < (-0x13))) {
        player->unk_002 |= 1 << (screenId * 4);
    }
}

void func_80029B4C(Player* player, UNUSED f32 arg1, f32 arg2, UNUSED f32 arg3) {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    Vec3f sp8C;
    Vec3f sp80;
    Mat3 sp5C;
    UNUSED s32 pad;
    f32 temp_f0_2;
    f32 temp_f2_3;
    s16 temp_v0;
    f32 var_f12;

    if ((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) {
        var_f12 = 18.0f * ((((gCharacterSize[player->characterId] / 2)) * ((player->size) * 1.5)));
    } else {
        var_f12 = 18.0f * (gCharacterSize[player->characterId] / 2);
    }

    calculate_orientation_matrix(sp5C, 0.0f, 1.0f, 0.0f, (player->rotation[1] + player->unk_0C0));
    sp8C[0] = var_f12 - 3.6;
    sp8C[1] = -player->boundingBoxSize;
    sp8C[2] = var_f12 - 2.0f;
    mtxf_translate_vec3f_mat3(sp8C, sp5C);
    sp80[0] = player->tyres[FRONT_LEFT].pos[0];
    sp80[1] = player->tyres[FRONT_LEFT].pos[1];
    sp80[2] = player->tyres[FRONT_LEFT].pos[2];
    player->tyres[FRONT_LEFT].pos[0] = player->pos[0] + sp8C[0];
    player->tyres[FRONT_LEFT].pos[1] = player->pos[1] + sp8C[1];
    player->tyres[FRONT_LEFT].pos[2] = player->pos[2] + sp8C[2];
    player_terrain_collision(player, &player->tyres[FRONT_LEFT], sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 3.6;
    sp8C[1] = -player->boundingBoxSize;
    sp8C[2] = var_f12 - 2.0f;
    mtxf_translate_vec3f_mat3(sp8C, sp5C);
    sp80[0] = player->tyres[FRONT_RIGHT].pos[0];
    sp80[1] = player->tyres[FRONT_RIGHT].pos[1];
    sp80[2] = player->tyres[FRONT_RIGHT].pos[2];
    player->tyres[FRONT_RIGHT].pos[0] = player->pos[0] + sp8C[0];
    player->tyres[FRONT_RIGHT].pos[1] = player->pos[1] + sp8C[1];
    player->tyres[FRONT_RIGHT].pos[2] = player->pos[2] + sp8C[2];
    player_terrain_collision(player, &player->tyres[FRONT_RIGHT], sp80[0], sp80[1], sp80[2]);
    sp8C[0] = var_f12 - 2.6;
    sp8C[1] = -player->boundingBoxSize;
    sp8C[2] = (-var_f12) + 4.0f;
    mtxf_translate_vec3f_mat3(sp8C, sp5C);
    sp80[0] = player->tyres[BACK_LEFT].pos[0];
    sp80[1] = player->tyres[BACK_LEFT].pos[1];
    sp80[2] = player->tyres[BACK_LEFT].pos[2];
    player->tyres[BACK_LEFT].pos[0] = player->pos[0] + sp8C[0];
    player->tyres[BACK_LEFT].pos[1] = player->pos[1] + sp8C[1];
    player->tyres[BACK_LEFT].pos[2] = player->pos[2] + sp8C[2];
    player_terrain_collision(player, &player->tyres[BACK_LEFT], sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 2.6;
    sp8C[1] = -player->boundingBoxSize;
    sp8C[2] = (-var_f12) + 4.0f;
    mtxf_translate_vec3f_mat3(sp8C, sp5C);
    sp80[0] = player->tyres[BACK_RIGHT].pos[0];
    sp80[1] = player->tyres[BACK_RIGHT].pos[1];
    sp80[2] = player->tyres[BACK_RIGHT].pos[2];
    player->tyres[BACK_RIGHT].pos[0] = player->pos[0] + sp8C[0];
    player->tyres[BACK_RIGHT].pos[1] = player->pos[1] + sp8C[1];
    player->tyres[BACK_RIGHT].pos[2] = player->pos[2] + sp8C[2];
    player_terrain_collision(player, &player->tyres[BACK_RIGHT], sp80[0], sp80[1], sp80[2]);
    if (!(player->effects & 8)) {
        a = (player->tyres[BACK_LEFT].baseHeight + player->tyres[FRONT_LEFT].baseHeight) / 2;
        move_f32_towards(&player->unk_230, a, 0.5f);

        b = (player->tyres[BACK_RIGHT].baseHeight + player->tyres[FRONT_RIGHT].baseHeight) / 2;
        move_f32_towards(&player->unk_23C, b, 0.5f);

        c = (player->tyres[FRONT_RIGHT].baseHeight + player->tyres[FRONT_LEFT].baseHeight) / 2;
        move_f32_towards(&player->unk_1FC, c, 0.5f);

        d = (player->tyres[BACK_RIGHT].baseHeight + player->tyres[BACK_LEFT].baseHeight) / 2;
        move_f32_towards(&player->unk_1F8, d, 0.5f);
    }
    temp_f2_3 = ((gCharacterSize[player->characterId] * 18.0f) + 1.0f) * player->size;
    temp_f0_2 = player->unk_23C - player->unk_230;
    player->unk_206 = -func_802B7C40(temp_f0_2 / temp_f2_3);
    if (((player->unk_0CA & 2) == 2) || (player->effects & 8)) {
        player->unk_206 = 0;
    }
    if ((player->effects & 8) != 8) {
        temp_f0_2 = player->unk_1F8 - player->unk_1FC;
        move_s16_towards(&player->slopeAccel, func_802B7C40(temp_f0_2 / temp_f2_3), 0.5f);
    } else {
        temp_f0_2 = player->oldPos[1] - arg2;
        temp_v0 = func_802B7C40(temp_f0_2 / temp_f2_3);
        if (temp_f0_2 >= 0.0f) {
            temp_v0 /= 4;
        } else {
            temp_v0 *= 10;
        }
        move_s16_towards(&player->slopeAccel, temp_v0, 0.5f);
    }
    if (((player->effects & 8) == 8) && ((player->unk_0CA & 2) == 2)) {
        player->slopeAccel = (s16) ((s32) player->unk_D9C);
    }
    player->surfaceType = get_surface_type(player->collision.meshIndexZX) & 0xFF;
    if (player->surfaceType == BOOST_RAMP_ASPHALT) {
        if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) != BOOST_RAMP_ASPHALT_EFFECT) &&
            ((player->effects & 8) != 8)) {
            player->soundEffects |= BOOST_RAMP_ASPHALT_SOUND_EFFECT;
        }
    }
    if (player->surfaceType == BOOST_RAMP_WOOD) {
        if (((player->effects & BOOST_RAMP_WOOD_EFFECT) != BOOST_RAMP_WOOD_EFFECT) && ((player->effects & 8) != 8)) {
            player->soundEffects |= BOOST_RAMP_WOOD_SOUND_EFFECT;
        }
    }
}

void func_8002A194(Player* player, f32 x, f32 y, f32 z) {
  UNUSED s32 pad[2];
  f32 temp_f12;
  f32 var_f20;
  s32 temp_v0;
  s16 temp_v1;
  s16 var_a1;
  UNUSED s32 pad2;
  f32 temp_f0;

  temp_v1 = -player->rotation[1] - player->unk_0C0;
  if ((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) {
      var_f20 = (((gCharacterSize[player->characterId] * 18) / 2) * (player->size * 1.5)) - 1;
  } else {
      var_f20 = (((gCharacterSize[player->characterId] * 18) / 2) * player->size) - 1;
  }

  player->tyres[FRONT_LEFT].pos[2] = (coss(temp_v1 + 0x2000) * var_f20) + z;
  temp_f12 = (sins(temp_v1 + 0x2000) * var_f20) + x;
  player->tyres[FRONT_LEFT].pos[0] = temp_f12;
  player->tyres[FRONT_LEFT].baseHeight =
      calculate_surface_height(temp_f12, y, player->tyres[FRONT_LEFT].pos[2], player->collision.meshIndexZX);

  player->tyres[FRONT_RIGHT].pos[2] = (coss(temp_v1 - 0x2000) * var_f20) + z;
  temp_f12 = (sins(temp_v1 - 0x2000) * var_f20) + x;
  player->tyres[FRONT_RIGHT].pos[0] = temp_f12;
  player->tyres[FRONT_RIGHT].baseHeight =
      calculate_surface_height(temp_f12, y, player->tyres[FRONT_RIGHT].pos[2], player->collision.meshIndexZX);

  player->tyres[BACK_LEFT].pos[2] = (coss(temp_v1 + 0x6000) * var_f20) + z;
  temp_f12 = (sins(temp_v1 + 0x6000) * var_f20) + x;
  player->tyres[BACK_LEFT].pos[0] = temp_f12;
  player->tyres[BACK_LEFT].baseHeight =
      calculate_surface_height(temp_f12, y, player->tyres[BACK_LEFT].pos[2], player->collision.meshIndexZX);

  player->tyres[BACK_RIGHT].pos[2] = (coss(temp_v1 - 0x6000) * var_f20) + z;
  player->tyres[BACK_RIGHT].pos[0] = (sins(temp_v1 - 0x6000) * var_f20) + x;
  player->tyres[BACK_RIGHT].baseHeight = calculate_surface_height(
      player->tyres[BACK_LEFT].pos[0], y, player->tyres[BACK_LEFT].pos[2], player->collision.meshIndexZX);

  if ((player->effects & 8) != 8) {
      player->unk_230 = (player->tyres[BACK_LEFT].baseHeight + player->tyres[FRONT_LEFT].baseHeight) / 2;
      player->unk_23C = (player->tyres[BACK_RIGHT].baseHeight + player->tyres[FRONT_RIGHT].baseHeight) / 2;
      player->unk_1FC = (player->tyres[FRONT_RIGHT].baseHeight + player->tyres[FRONT_LEFT].baseHeight) / 2;
      player->unk_1F8 = (player->tyres[BACK_RIGHT].baseHeight + player->tyres[BACK_LEFT].baseHeight) / 2;
  }
  player->surfaceType = (u8) get_surface_type(player->collision.meshIndexZX);
  player->tyres[BACK_RIGHT].surfaceType = player->surfaceType;
  player->tyres[BACK_LEFT].surfaceType = player->surfaceType;
  player->tyres[FRONT_RIGHT].surfaceType = player->surfaceType;
  player->tyres[FRONT_LEFT].surfaceType = player->surfaceType;
  var_f20 = (gCharacterSize[player->characterId] * 18) + 1;
  temp_f0 = (player->unk_23C - player->unk_230);
  player->unk_206 = -func_802B7C40(temp_f0 / var_f20);
  if ((player->effects & 8) != 8) {
      temp_f0 = (player->unk_1F8 - player->unk_1FC);
      move_s16_towards(&player->slopeAccel, func_802B7C40(temp_f0 / var_f20), 0.5f);
  } else {
      temp_f0 = player->oldPos[1] - y;
      temp_v0 = func_802B7C40(temp_f0 / var_f20);
      if (temp_f0 >= 0.0f) {
          var_a1 = temp_v0 * 2;
      } else {
          var_a1 = temp_v0 * 0xA;
      }
      move_s16_towards(&player->slopeAccel, var_a1, 0.5f);
  }
  if (func_802ABD7C(player->collision.meshIndexZX) != 0) {
      player->tyres[BACK_RIGHT].unk_14 |= 1;
  } else {
      player->tyres[BACK_RIGHT].unk_14 &= ~1;
  }
  if (player->surfaceType == BOOST_RAMP_ASPHALT) {
      if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) != BOOST_RAMP_ASPHALT_EFFECT) &&
          ((player->effects & 8) != 8)) {
          player->soundEffects |= BOOST_RAMP_ASPHALT_SOUND_EFFECT;
      }
  }
  if (player->surfaceType == BOOST_RAMP_WOOD) {
      if (((player->effects & BOOST_RAMP_WOOD_EFFECT) != BOOST_RAMP_WOOD_EFFECT) && ((player->effects & 8) != 8)) {
          player->soundEffects |= BOOST_RAMP_WOOD_SOUND_EFFECT;
      }
  }
}

// Near identical to adjust_pos_orthogonally in memory.c
void func_8002A5F4(Vec3f arg0, f32 arg1, Vec3f arg2, f32 arg3, f32 arg4) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 tmp1;
    f32 tmp2;
    f32 tmp3;

    temp_f0 = arg2[0];
    temp_f2 = arg2[1];
    temp_f12 = arg2[2];
    temp_f14 = -arg0[0];
    temp_f16 = -arg0[1];
    temp_f18 = -arg0[2];
    temp_f20 = (temp_f14 * temp_f0) + (temp_f16 * temp_f2) + (temp_f18 * temp_f12);
    tmp1 = temp_f0 - (temp_f20 * temp_f14);
    tmp2 = temp_f2 - (temp_f20 * temp_f16);
    tmp3 = temp_f12 - (temp_f20 * temp_f18);
    if (arg1 < -arg4) {
        arg2[0] = tmp1 - (temp_f20 * temp_f14 * arg3);
        arg2[1] = tmp2 - (temp_f20 * temp_f16 * arg3);
        arg2[2] = tmp3 - (temp_f20 * temp_f18 * arg3);
    } else {
        arg2[0] = tmp1;
        arg2[1] = tmp2;
        arg2[2] = tmp3;
    }
}

void func_8002A704(Player* player, s8 arg1) {
    player->effects |= BOOST_EFFECT;
    player->soundEffects &= ~0x02000000;
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
        func_800C90F4(0U, (player->characterId * 0x10) + 0x29008001);
        func_800C9060(arg1, 0x1900A40BU);
    }
    player->boostTimer = 0x0050;
}

void func_8002A79C(Player* player, s8 arg1) {
    if (((player->effects & 0x100) != 0x100) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT) &&
        (player->driftState >= 2)) {
        player->effects |= 0x100;
        player->unk_23A = 0;
        player->driftState = 0;
        player->unk_228 = 0;
        if (D_8015F890 != 1) {
            if ((player->type & PLAYER_HUMAN) && !(player->type & PLAYER_INVISIBLE_OR_BOMB)) {
                func_800C9250(arg1);
            }
        } else if (player == gPlayerOne) {
            func_800C9250(arg1);
        }
    } else if ((player->effects & 0x100) == 0x100) {
        player->unk_23A += 1;
        if (player->unk_23A >= 0x1F) {
            player->unk_23A = 0;
            player->effects &= ~0x100;
            player->driftState = 0;
            player->unk_228 = 0;
        }
    }
}

void func_8002A8A4(Player* player, s8 arg1) {
    if (((s16) player->unk_0C0 / 182) > 0) {
        if (((s32) player->unk_07C >> 0x10) < -9) {
            if (player->unk_228 < 0x65) {
                player->unk_228++;
            }
            if ((player->unk_228 == 0x0064) && (player->type & PLAYER_HUMAN)) {
                func_800C9060(arg1, 0x1900851EU);
            }
        } else {
            if ((player->unk_228 >= 0x12) && (player->unk_228 < 0x64)) {
                if (player->driftState < 3) {
                    player->driftState++;
                }
            }
            if ((player->unk_228 >= 0xA) && (player->unk_228 < 0x64)) {
                player->unk_228 = 0x000A;
            } else {
                player->unk_228 = 0;
                player->driftState = 0;
            }
        }
    } else if (((s32) player->unk_07C >> 0x10) >= 0xA) {
        if (player->unk_228 < 0x65) {
            player->unk_228++;
        }
        if ((player->unk_228 == 0x0064) && (player->type & PLAYER_HUMAN)) {
            func_800C9060(arg1, 0x1900851EU);
        }
    } else {
        if ((player->unk_228 >= 0x12) && (player->unk_228 < 0x64)) {
            if (player->driftState < 3) {
                player->driftState++;
            }
        }
        if ((player->unk_228 >= 0xA) && (player->unk_228 < 0x64)) {
            player->unk_228 = 0x000A;
        } else {
            player->unk_228 = 0;
            player->driftState = 0;
        }
    }
}

void kart_hop(Player* player) {
    player->kartHopJerk = gKartHopJerkTable[player->characterId];
    player->kartHopAcceleration = 0.0f;
    player->kartHopVelocity = gKartHopInitialVelocityTable[player->characterId];
    player->effects |= 2;
    player->unk_DAC = 3.0f;
    player->kartGravity = 500.0f;
    func_80036C5C(player);
}

/**
  * Function: func_8002AAC0

  * Parameters:
  *     Player *player - A pointer to a Player

  * First kartHopJerk is subtracted from kartHopAcceleration
  * Then kartHopAcceleration is added to kartHopVelocity.

  * If kartHopVelocity is less than or equal to zero, all three
  * values (kartHop{Jerk,Acceleration,Velocity}) are set to 0.0f,
  * thereby ending the rising portion of the hop.

  * kartHopVelocity is restricted to values in [-INF, 15.0f]

  * kartHopAcceleration is restricted to values in [-9.0f, 9.0f]
**/
void func_8002AAC0(Player* player) {
    player->kartHopAcceleration -= player->kartHopJerk;
    if (player->kartHopAcceleration >= 9.0f) {
        player->kartHopAcceleration = 9.0f;
    }

    if (player->kartHopAcceleration <= -9.0f) {
        player->kartHopAcceleration = -9.0f;
    }

    player->kartHopVelocity += player->kartHopAcceleration;
    if (player->kartHopVelocity >= 15.0f) {
        player->kartHopVelocity = 15.0f;
    }

    if (player->kartHopVelocity <= 0.0f) {
        player->kartHopJerk = 0.0f;
        player->kartHopAcceleration = 0.0f;
        player->kartHopVelocity = 0.0f;
    }
}

void func_8002AB70(Player* player) {
    UNUSED s32 pad[2];
    if (((player->effects & 8) != 8) && (player->kartPropulsionStrength > 0.0f)) {
        if (((player->slopeAccel / 182) < -1) && ((player->slopeAccel / 182) >= -0x14) &&
            (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            move_f32_towards(&player->kartGravity, 500.0f, 1.0f);
            move_f32_towards(&player->unk_DAC, 3.0f, 0.05f);
        } else {
            move_f32_towards(&player->kartGravity, gKartGravityTable[player->characterId], 0.1f);
            move_f32_towards(&player->unk_DAC, 1.0f, 0.07f);
        }
    } else {
        if (player->collision.surfaceDistance[2] >= 50.0f) {
            player->unk_DAC = 2.0f;
        }
        move_f32_towards(&player->kartGravity, gKartGravityTable[player->characterId], 0.02f);
        if ((player->effects & 2) == 2) {
            move_f32_towards(&player->unk_DAC, 1.0f, 0.07f);
        } else {
            move_f32_towards(&player->unk_DAC, 1.0f, 0.07f);
        }
    }
    if ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) {
        move_f32_towards(&player->unk_DAC, 20.0f, 1.0f);
        player->kartGravity = 3500.0f;
    }
    if ((player->effects & BOOST_RAMP_WOOD_EFFECT) == BOOST_RAMP_WOOD_EFFECT) {
        move_f32_towards(&player->unk_DAC, 25.0f, 1.0f);
        player->kartGravity = 1800.0f;
    }
    if ((player->effects & 0x400) == 0x400) {
        player->kartGravity = 1100.0f;
    }
    if (player->effects & 0x80000) {
        player->kartGravity = 1500.0f;
    }
    if ((player->unk_044 & 0x800) != 0) {
        player->kartGravity = 1900.0f;
    }
    if ((player->effects & 0x800000) == 0x800000) {
        player->kartGravity = 300.0f;
    }
    if ((player->effects & 0x01000000) == 0x01000000) {
        player->kartGravity = 550.0f;
    }
    if ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) {
        player->kartGravity = 800.0f;
    }
}

UNUSED void func_8002AE20(void) {
}

UNUSED void func_8002AE28(void) {
}

UNUSED void func_8002AE30(void) {
}

void func_8002AE38(Player* player, s8 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    UNUSED s32 pad[4];
    s16 temp_v0_3;
    f32 sp28;
    f32 temp_f16;
    s16 temp_a0;
    s32 var_v1;

    sp28 = (sins(-player->rotation[1]) * player->speed) + arg2;
    temp_f16 = (coss(-player->rotation[1]) * player->speed) + arg3;
    if (((player->effects & 0x800) != 0x800) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT) &&
        !(player->unk_044 & 0x4000) &&
        ((((player->speed / 18.0f) * 216.0f) <= 8.0f) ||
         (((player->unk_07C >> 0x10) < 5) && ((player->unk_07C >> 0x10) > -5)))) {
        if ((player->effects & 0x20) == 0x20) {
            player->unk_0C0 = (f32) (player->unk_0C0 - (player->unk_0C0 / 10));
        } else {
            temp_v0_3 = player->unk_0C0;
            player->unk_0C0 = player->unk_078 * 9;
            temp_a0 = player->unk_0C0 - temp_v0_3;
            player->unk_0C0 = (f32) (temp_v0_3 + (temp_a0 / 15));
        }
    } else {
        temp_v0_3 = player->unk_0C0;
        if (D_801652C0[arg1] & 8) {
            var_v1 = 2;
        } else {
            var_v1 = 0;
        }
        if ((player->currentSpeed >= 200.0f) && (var_v1 == 2) &&
            (((player->unk_0C0 / 182) >= 0x10) || ((player->unk_0C0 / 182) < -0xF))) {
            player->unk_0C0 = atan2s(arg2 - arg4, arg3 - arg5) - atan2s(arg2 - sp28, arg3 - temp_f16);
        } else {
            player->unk_0C0 = (atan2s(arg2 - arg4, arg3 - arg5) - atan2s(arg2 - sp28, arg3 - temp_f16)) * 2;
        }
        if (((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT) &&
            ((((player->unk_07C >> 0x10) > 0) && (player->unk_0C0 < 0)) ||
             (((player->unk_07C >> 0x10) < 0) && (player->unk_0C0 > 0)))) {
            if (player->unk_0C0 > 0) {
                player->unk_0C0 = player->unk_078 * 0x14;
            }
            if (player->unk_0C0 < 0) {
                player->unk_0C0 = player->unk_078 * 0x14;
            }
            temp_a0 = player->unk_0C0 - temp_v0_3;
            player->unk_0C0 = (f32) (temp_v0_3 + (temp_a0 / 12));
        } else {
            if (player->unk_0C0 >= 0x1C71) {
                player->unk_0C0 = 0x1C70;
            }
            if (player->unk_0C0 < -0x1C70) {
                player->unk_0C0 = -0x1C70;
            }
            temp_a0 = player->unk_0C0 - temp_v0_3;
            player->unk_0C0 = (f32) (temp_v0_3 + (temp_a0 / 12));
        }
    }
}

void func_8002B218(Player* player) {
    u16 someIndex;
    u16 sp38[10] = { 0x0003, 0x0016, 0x0026, 0x003c, 0x0050, 0x0069, 0x0090, 0x009d, 0x00a9, 0x00cc };
    u16 sp24[10] = { 0x000c, 0x0021, 0x002f, 0x0045, 0x005f, 0x007a, 0x0098, 0x00a5, 0x00b3, 0x00d5 };

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (player->unk_006 == sp38[someIndex]) {
            player->effects |= DRIFTING_EFFECT;
            kart_hop(player);
            player->driftDuration = 0;
            break;
        }

        if (player->unk_006 == sp24[someIndex]) {
            player->effects &= ~DRIFTING_EFFECT;
            break;
        }
    }
}

void apply_sound_effect(Player* player, s8 playerId, UNUSED s8 screenId) {
    if ((player->soundEffects & 2) == 2) {
        apply_hit_by_item_sound_effect(player, playerId);
    }
    if ((player->soundEffects & 4) == 4) {
        func_8008C528(player, playerId);
    }
    if ((player->soundEffects & 1) == 1) {
        func_8008CDC0(player, playerId);
    }
    if ((player->soundEffects & BOOST_SOUND_EFFECT) == BOOST_SOUND_EFFECT) {
        apply_boost_sound_effect(player, playerId);
    }
    if ((player->soundEffects & 0x02000000) == 0x02000000) {
        func_8002A704(player, playerId);
    }
    if ((player->soundEffects & 0x1000) == 0x1000) {
        func_8008D570(player, playerId);
    }
    if ((player->soundEffects & 0x20000) == 0x20000) {
        func_8008D7B0(player, playerId);
    }
    if ((player->soundEffects & HIT_SOUND_EFFECT) == HIT_SOUND_EFFECT) {
        apply_hit_sound_effect(player, playerId);
    }
    if ((player->soundEffects & HIT_ROTATING_SOUND_EFFECT) == HIT_ROTATING_SOUND_EFFECT) {
        apply_hit_rotating_sound_effect(player, playerId);
    }
    if ((player->soundEffects & 0x200000) == 0x200000) {
        func_8008C73C(player, playerId);
    }
    if ((player->soundEffects & REVERSE_SOUND_EFFECT) == REVERSE_SOUND_EFFECT) {
        apply_reverse_sound_effect(player, playerId);
    }
    if ((player->soundEffects & HIT_BY_ITEM_SOUND_EFFECT) == HIT_BY_ITEM_SOUND_EFFECT) {
        apply_hit_by_item_sound_effect(player, playerId);
    }
    if ((player->soundEffects & BOOST_RAMP_ASPHALT_SOUND_EFFECT) == BOOST_RAMP_ASPHALT_SOUND_EFFECT) {
        apply_boost_ramp_asphalt_sound_effect(player, playerId);
    }
    if ((player->soundEffects & BOOST_RAMP_WOOD_SOUND_EFFECT) == BOOST_RAMP_WOOD_SOUND_EFFECT) {
        apply_boost_ramp_wood_sound_effect(player, playerId);
    }
    if ((player->soundEffects & STAR_SOUND_EFFECT) == STAR_SOUND_EFFECT) {
        apply_star_sound_effect(player, playerId);
    }
    if ((player->soundEffects & BOO_SOUND_EFFECT) == BOO_SOUND_EFFECT) {
        apply_boo_sound_effect(player, playerId);
    }
    if (player->soundEffects & 0x80) {
        func_8008D0FC(player, playerId);
    }
    if (player->soundEffects & 0x80000) {
        apply_reverse_sound_effect(player, playerId);
    }
}

void func_8002B5C0(Player* player, UNUSED s8 playerId, UNUSED s8 screenId) {
    if (((player->unk_0CA & 8) != 0) || ((player->unk_0CA & 2) != 0)) {
        player->soundEffects &= 0xFE1D0478;
    }
    if ((player->effects & 0x400) == 0x400) {
        player->soundEffects &= 0xFF5D457E;
    }
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
        player->soundEffects &= 0xFF5F457E;
    }
    if ((player->effects & 0x800) == 0x800) {
        player->soundEffects &= 0xFF5D457E;
    }
    if ((player->unk_044 & 0x4000) != 0) {
        player->soundEffects &= 0xFF5D457E;
    }
    if ((player->effects & 0x80000) == 0x80000) {
        player->soundEffects &= 0xFE1D4478;
    }
    if ((player->effects & 0x800000) == 0x800000) {
        player->soundEffects &= 0xFE1D0478;
    }
    if ((player->effects & HIT_EFFECT) == HIT_EFFECT) {
        player->soundEffects &= 0xFE1D0578;
    }
    if ((player->effects & 0x01000000) == 0x01000000) {
        player->soundEffects &= 0xFE1D4478;
    }
    if ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) {
        player->soundEffects &= 0xFE1D4478;
    }
    if ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) {
        player->soundEffects &= 0xFE1D0478;
    }
    if ((player->effects & BOOST_RAMP_WOOD_EFFECT) == BOOST_RAMP_WOOD_EFFECT) {
        player->soundEffects &= 0xFE1D0478;
    }
    if ((player->effects & 0x10000) == 0x10000) {
        player->soundEffects &= 0xFE1D0478;
    }
    if ((player->effects & STAR_EFFECT) == STAR_EFFECT) {
        player->soundEffects &= 0xFE9D8478;
    }
    if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
        player->soundEffects &= 0xFE9D8678;
    }
    if ((player->effects & 0x4000) == 0x4000) {
        player->soundEffects &= 0xFF5D45FF;
    }
    if ((player->effects & 0x20000) == 0x20000) {
        player->soundEffects &= 0xFE1D0478;
    }
}

void func_8002B830(Player* player, s8 playerId, s8 screenId) {
    if (player->soundEffects != 0) {
        func_8002B5C0(player, playerId, screenId);
    }
    if (player->soundEffects != 0) {
        apply_sound_effect(player, playerId, screenId);
    }
    if ((player->unk_044 & 0x400) != 0) {
        func_800911B4(player, playerId);
    }
}

UNUSED void func_8002B8A4(Player* player_one, Player* player_two) {
    s32 var_v1;

    // clang-format off
    /*
    if's are being done bracket-less on purpose,
    Technically only the `player_one == gPlayerEight` NEEDS to be
    bracket-less that looks weird
    */
    if (player_one == gPlayerOne) {   var_v1 = 0;
}
    if (player_one == gPlayerTwo) {   var_v1 = 1;
}
    if (player_one == gPlayerThree) { var_v1 = 2;
}
    if (player_one == gPlayerFour) {  var_v1 = 3;
}
    if (player_one == gPlayerFive) {  var_v1 = 4;
}
    if (player_one == gPlayerSix) {   var_v1 = 5;
}
    if (player_one == gPlayerSeven) { var_v1 = 6;
}
    if (player_one == gPlayerEight) { var_v1 = 7;
}
    D_801653C0[var_v1] = player_two;
    if (player_two == gPlayerOne) {   var_v1 = 0;
}
    if (player_two == gPlayerTwo) {   var_v1 = 1;
}
    if (player_two == gPlayerThree) { var_v1 = 2;
}
    if (player_two == gPlayerFour) {  var_v1 = 3;
}
    if (player_two == gPlayerFive) {  var_v1 = 4;
}
    if (player_two == gPlayerSix) {   var_v1 = 5;
}
    if (player_two == gPlayerSeven) { var_v1 = 6;
}
    if (player_two == gPlayerEight) { var_v1 = 7;
}
    D_801653C0[var_v1] = player_one;
    // clang-format on
}

void func_8002B9CC(Player* player, s8 arg1, UNUSED s32 arg2) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f14;
    s16 temp;
    s16 temp2;

    if ((player->unk_046 & 2) == 2) {
        temp_f0 = D_8018CE10[arg1].unk_04[0];
        temp_f2 = 0;
        temp_f14 = D_8018CE10[arg1].unk_04[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 6.5) {
            player->kartPropulsionStrength /= 4;
            player->currentSpeed /= 4;
            if (!(player->effects & 0x80) && !(player->effects & 0x40)) {
                func_8008C73C(player, arg1);
            }
        }
    } else {
        temp_f0 = gPlayerLastVelocity[arg1][0] - player->velocity[0];
        temp_f2 = gPlayerLastVelocity[arg1][1] - player->velocity[1];
        temp_f14 = gPlayerLastVelocity[arg1][2] - player->velocity[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 4.2) {
            player->kartPropulsionStrength /= 4;
            player->currentSpeed /= 4;
            if (!(player->effects & 0x80) && !(player->effects & 0x40)) {
                func_8008C73C(player, arg1);
            }
        }
        temp = (-(s16) get_angle_between_two_vectors(player->pos, &player->oldPos[0]));
        temp2 = (player->rotation[1] - player->unk_0C0);
        temp = temp - temp2;
        player->unk_234 = temp / 182;
    }
}

void func_8002BB9C(Player* player, f32* arg1, f32* arg2, UNUSED s8 arg3, UNUSED s8 arg4, UNUSED Vec3f arg5) {
    Mat3 sp64;
    Vec3f sp58;
    Vec3f sp4C;
    s16 var_v0;
    s16 t0;
    UNUSED s32 pad;
    s16 sp30[10] = { 0x0000, 0x00b6, 0x016c, 0x0222, 0x02d8, 0x038e, 0x0444, 0x04fa, 0x05b0, 0x0666 };

    if (((u16) player->unk_256) <= 0) {
        return;
    }

    if (((u16) player->unk_256) > 2) {
        return;
    }

    if (!(player->unk_046 & 0x20)) {
        return;
    }

    calculate_orientation_matrix(sp64, 0, 1, 0, (s16) 0);

    sp58[0] = *arg1;
    sp58[1] = 0;
    sp58[2] = *arg2;

    mtxf_translate_vec3f_mat3(sp58, sp64);

    sp4C[0] = player->oldPos[0];
    sp4C[1] = 0;
    sp4C[2] = player->oldPos[2];

    mtxf_translate_vec3f_mat3(sp4C, sp64);

    var_v0 = -(s16) get_angle_between_two_vectors(sp58, sp4C);
    t0 = player->rotation[1];
    var_v0 = 0x10000 + (t0 - var_v0);
    var_v0 /= 182;

    if (var_v0 < 0x97 && (var_v0 > -0x97)) {
        return;
    }

    var_v0 = (player->unk_07C >> 0x10) / 6;

    if (var_v0 < 0) {
        var_v0 *= -1;
    }

    if (var_v0 >= 8) {
        var_v0 = 8;
    }

    if ((player->unk_07C >> 0x10) < 0) {
        player->rotation[1] -= sp30[var_v0];
    } else {
        player->rotation[1] += sp30[var_v0];
    }
}

void func_8002BD58(Player* player) {
    s32 sp2C[7] = { 0x002f0000, 0x00300000, 0x00310000, 0x00320000, 0x00320000, 0x00320000, 0x00320000 };
    s32 spC[8] = { 0x00280000, 0x002c0000, 0x00300000, 0x00320000, 0x00320000, 0x00320000, 0x00320000, 0x00320000 };
    s16 temp_t5;

    if (player->unk_234 >= 0) {
        if ((player->unk_234 >= 5) && (player->unk_234 < 0x1E)) {
            player->unk_07C = sp2C[player->unk_234 / 6];
        }
        if ((player->unk_234 >= 0x1E) && (player->unk_234 < 0x50)) {
            player->unk_07C = spC[(s32) (player->unk_234 - 0x1E) / 12];
        }
        if (((player->unk_234 < 0x50) || (player->unk_234 >= 0x5B)) && (player->unk_234 >= 0x5B) &&
            (player->unk_234 < 0xA1)) {
            player->unk_07C = spC[1];
        }
    } else {
        temp_t5 = -player->unk_234;
        if ((player->unk_234 < -4) && (player->unk_234 >= -0x1E)) {
            player->unk_07C = sp2C[temp_t5 / 6] * -1;
        }
        if ((player->unk_234 < -0x1E) && (player->unk_234 >= -0x50)) {
            player->unk_07C = spC[(s32) (temp_t5 - 0x1E) / 12] * -1;
        }
        if (((player->unk_234 >= -0x50) || (player->unk_234 < -0x5A)) && (player->unk_234 < -0x5A) &&
            (player->unk_234 >= -0xA0)) {
            player->unk_07C = spC[1] * -1;
        }
    }
}

void func_8002BF4C(Player* player, s8 arg1) {
    UNUSED s32 pad[3];
    UNUSED s32 uselessAssignment;
    s32 i;
    s32 var_a2;
    Player* playerBorrow;
    Player* players = gPlayerOne;

    var_a2 = 0;

    if (((player->speed / 18.0f) * 216.0f) < 50.0f) {
        player->unk_0E2 = 0;
        player->effects &= 0xFFDFFFFF;
        return;
    }
    if ((player->effects & 0x200000) == 0x200000) {
        player->unk_0E2 -= 1;
        if (player->unk_0E2 <= 0) {
            player->effects &= 0xFFDFFFFF;
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            playerBorrow = &players[i];
            if (((player != playerBorrow) && ((playerBorrow->type & 0x100) == 0) && (playerBorrow->type & 0x8000)) &&
                ((var_a2 = func_8001FD78(player, playerBorrow->pos[0], playerBorrow->pos[1], playerBorrow->pos[2]),
                  var_a2 == 1))) {
                player->unk_0E2 += 1;
                if (player->unk_0E2 >= 0x3D) {
                    player->effects |= 0x200000;
                    if ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB) {
                        uselessAssignment = player->type & PLAYER_INVISIBLE_OR_BOMB;
                        func_800C90F4(arg1, (player->characterId * 0x10) + 0x29008001);
                    }
                    if ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB) {
                        uselessAssignment = var_a2;
                        func_800C9060(arg1, 0x19008011);
                    }
                }
                break;
            }
        }

        if (var_a2 == 0) {
            player->unk_0E2 = 0;
        }
    }
}

void func_8002C11C(Player* player) {
    if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        player->driftDuration += 1;
        if (player->driftDuration >= 0x65) {
            player->driftDuration = 0x64;
        }
    } else {
        player->driftDuration -= 1;
        if (player->driftDuration < 0) {
            player->driftDuration = 0;
        }
    }
}

void func_8002C17C(Player* player, s8 playerId) {
    if (IsYoshiValley()) {
        if ((player->collision.surfaceDistance[2] >= 600.0f) && (D_80165330[playerId] == 0)) {
            D_80165330[playerId] = 1;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (D_80165330[playerId] == 0) {
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (!(player->effects & 8)) {
            if (func_802ABDF4(player->collision.meshIndexZX) == 0) {
                D_80165330[playerId] = 0;
            }
        }
    } else if (IsFrappeSnowland()) {
        if ((player->surfaceType == SNOW_OFFROAD) && (D_80165330[playerId] == 0)) {
            D_80165330[playerId] = 1;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (player->surfaceType != SNOW_OFFROAD) {
            D_80165330[playerId] = 0;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        }
    } else if (IsRoyalRaceway()) {
        if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) != 0) && (D_80165330[playerId] == 0)) {
            D_80165330[playerId] = 1;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == 0) && !(player->effects & 8)) {
            D_80165330[playerId] = 0;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        }
    } else if (IsRainbowRoad()) {
        if ((player->collision.surfaceDistance[2] >= 600.0f) && (D_80165330[playerId] == 0)) {
            D_80165330[playerId] = 1;
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (D_80165330[playerId] == 0) {
            gCopyNearestWaypointByPlayerId[playerId] = gNearestPathPointByPlayerId[playerId];
            gCopyPathIndexByPlayerId[playerId] = gPathIndexByPlayerId[playerId];
        } else if (!((player->effects & 8) || (player->unk_0CA & 1))) {
            D_80165330[playerId] = 0;
        }
    } else {
        D_80165330[playerId] = 0;
        if (1) {}
    }
}

void update_player_environment_and_hazard_state(Player* player, s8 playerIndex) {
    gPlayerWaterLevel[playerIndex] = get_water_level(player);
    if (player->pos[1] <= gPlayerWaterLevel[playerIndex]) { // Player is in water, at least partially
        player->waterInteractionFlags |= WATER_IS_PARTIALLY_SUBMERGED;
    } else {
        player->waterInteractionFlags &= ~WATER_IS_PARTIALLY_SUBMERGED;
    }
    if (player->boundingBoxSize < (gPlayerWaterLevel[playerIndex] - player->pos[1])) {
        // Player is fully submerged in water
        player->waterInteractionFlags |= WATER_IS_FULLY_SUBMERGED;
        player->waterInteractionFlags &= ~WATER_IS_PARTIALLY_SUBMERGED;
    } else {
        // Player is not fully submerged in water, clear the flag
        player->waterInteractionFlags &= ~WATER_IS_FULLY_SUBMERGED;
    }
    if (player->boundingBoxSize < (gPlayerWaterLevel[playerIndex] - player->pos[1])) {
        // We confirm again that the player if fully submerged
        if ((player->waterInteractionFlags & WATER_IN_DEEP_LIQUID_STATE) != WATER_IN_DEEP_LIQUID_STATE) {
            // Set flag - it will be cleared shortly after
            player->waterInteractionFlags |= WATER_JUST_ENTERED_DEEP_LIQUID;
            // This flag persists longer
            player->waterInteractionFlags |= WATER_IN_DEEP_LIQUID_STATE;
            if ((!IsKoopaTroopaBeach()) && (!IsSkyscraper()) &&
                (!IsRainbowRoad()) && ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN)) {
                if ((IsBowsersCastle()) || (IsBigDonut())) {
                    func_800C9060((u8) playerIndex, 0x1900801CU);
                } else {
                    func_800C9060((u8) playerIndex, 0x19008008U);
                }
            }
        }
    }
    if ((IsKoopaTroopaBeach()) || (IsSkyscraper()) ||
        (IsRainbowRoad())) {
          // Different handling for these tracks, we just use the first two (0x0001 and 0x0002)
          player->waterInteractionFlags &= ~(WATER_IN_DEEP_LIQUID_STATE | WATER_JUST_ENTERED_DEEP_LIQUID);
    }
    if ((player->boundingBoxSize < (gPlayerWaterLevel[playerIndex] - player->pos[1])) &&
        (player->collision.surfaceDistance[2] >= 600.0f)) {
        // We are fully submerged in water and far away from the surface (out of bounds?)
        player->unk_0CA |= 1;
    }
    if (player->collision.surfaceDistance[2] >= 600.0f) {
        player->unk_0CA |= 0x0100;
    } else if ((player->effects & 8) != 8) {
        player->unk_0CA &= ~0x0100;
    }
    if ((player->type & PLAYER_CPU) && ((func_802ABDF4(player->collision.meshIndexZX) != 0) || (player->unk_0CA & 1))) {
        if (!(player->unk_0CA & 2) && !(player->unk_0CA & 8) && !(player->effects & 0x1000)) {
            func_80090778(player);
            func_80090868(player);
        }
    }
    if ((player->type & PLAYER_CPU) && (player->surfaceType == OUT_OF_BOUNDS) && !(player->effects & 8)) {
        func_80090778(player);
        func_80090868(player);
    }
    func_8002C17C(player, playerIndex);
}

void func_8002C7E4(Player* player, s8 arg1, s8 arg2) {
    if ((player->unk_046 & 1) != 1) {
        if ((player->effects & 0x8000) == 0x8000) {
            if ((player->effects & BOOST_EFFECT) != BOOST_EFFECT) {
                func_8002B9CC(player, arg1, arg2);
            }
            player->unk_044 &= ~0x0001;
            player->unk_046 |= 1;
            player->unk_046 |= 8;
            if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                func_8001CA24(player, 2.8f);
            }
            if ((player->unk_046 & 2) == 2) {
                if ((player->unk_046 & 4) != 4) {
                    player->unk_046 |= 4;
                    player->unk_046 |= 0x40;
                    if (player->effects & BOOST_EFFECT) {
                        remove_boost_effect(player);
                    }
                }
            }
        }
    }
    if ((player->effects & 0x8000) == 0x8000) {
        player->effects &= ~0x8000;
        player->unk_10C = 1;
        player->unk_044 &= ~0x0001;
        return;
    }
    player->unk_046 &= ~0x0001;
    player->effects &= ~0x8000;
    if (player->unk_10C > 0) {
        player->unk_10C += 1;
    }
    if (player->unk_10C >= 0xA) {
        player->unk_10C = 0;
    }
}

void func_8002C954(Player* player, s8 playerId, Vec3f arg2) {
    f32 temp_f0;
    f32 var_f14;
    f32 xdist;
    f32 ydist;
    f32 zdist;

    temp_f0 = player->pos[1] - player->unk_074;

    if (((((player->effects & 0x10000) != 0x10000) &&
          ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT)) ||
         ((((temp_f0 >= 20.0f) || (temp_f0 < (-1.0f))) && ((player->effects & 0x10000) == 0)) &&
          (player->effects & 8)) ||
         ((player->collision.unk34 == 0) && ((player->effects & 0x10000) == 0))) &&
        (((player->unk_0CA & 2) == 0) || (!(player->unk_0CA & 8)))) {
        func_8008F494(player, playerId);
    }
    if ((player->unk_046 & 0x20) != 0x20) {
        if ((player->collision.surfaceDistance[0] < (-1.0f)) || (player->collision.surfaceDistance[1] < (-1.0f))) {
            player->unk_256 = 1;
        }
        player->unk_046 |= 0x20;
    }
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) && (((player->speed / 18.0f) * 216.0f) > 30.0f)) {
        func_8001CA24(player, 3.0f);
    }
    player->unk_046 |= 0x10;
    player->unk_256++;
    if (player->unk_256 >= 0xA) {
        player->unk_256 = 0;
    }
    if ((player->slopeAccel >= 0) && (((player->speed / 18.0f) * 216.0f) > 5.0f)) {
        player_decelerate_alternative(player, 18.0f);
    }
    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        xdist = gPlayerLastVelocity[playerId][0] - arg2[0];
        var_f14 = gPlayerLastVelocity[playerId][1] - arg2[1];
        ydist = var_f14; // okay
        zdist = gPlayerLastVelocity[playerId][2] - arg2[2];
        var_f14 = sqrtf((xdist * xdist) + (ydist * ydist) + (zdist * zdist)) / 3;
        if (var_f14 >= 1.0) {
            var_f14 = 1.0f;
        }
        if ((var_f14 <= 0.6) && (((player->speed / 18.0f) * 216.0f) >= 40.0f) &&
            (!(player->type & PLAYER_INVISIBLE_OR_BOMB))) {
            func_800CAEC4(playerId, 0.6F);
        } else if (!(player->type & PLAYER_INVISIBLE_OR_BOMB)) {
            if ((var_f14 <= 0.6) && (((player->speed / 18.0f) * 216.0f) < 40.0f) &&
                (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                func_800CAEC4(playerId, 0.3F);
            } else {
                func_800CAEC4(playerId, var_f14);
            }
        }
    }
    if (player->effects & BOOST_EFFECT) {
        remove_boost_effect(player);
        player->kartPropulsionStrength /= 2;
    }
}

void apply_effect(Player* player, s8 arg1, s8 arg2) {
    if (((player->unk_0CA & 2) == 2) || ((player->unk_0CA & 8) == 8)) {
        func_80090970(player, arg1, arg2);
    }
    if ((player->effects & 0x800) == 0x800) {
        func_8008CEB0(player, arg1);
    }
    if (player->unk_044 & 0x4000) {
        func_8008D170(player, arg1);
    }
    if ((player->effects & BOOST_EFFECT) == BOOST_EFFECT) {
        apply_boost_effect(player);
    }
    if ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) {
        apply_boost_ramp_asphalt_effect(player);
    }
    if ((player->effects & BOOST_RAMP_WOOD_EFFECT) == BOOST_RAMP_WOOD_EFFECT) {
        apply_boost_ramp_wood_effect(player);
    }
    if ((s32) (player->effects & HIT_EFFECT) == HIT_EFFECT) {
        apply_hit_effect(player, arg1);
    }
    if ((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) {
        apply_lightning_effect(player, arg1);
    }
    if ((player->effects & 0x10000) == 0x10000) {
        func_8008F3F4(player, arg1);
    }
    if ((player->effects & STAR_EFFECT) == STAR_EFFECT) {
        apply_star_effect(player, arg1);
    }
    if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
        apply_boo_effect(player, arg1);
    }
    if (((player->effects & 0x20000000) == 0x20000000) && (player->unk_228 >= 0x64)) {
        player_decelerate_alternative(player, 4.0f);
    }
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) {
        func_8008C9EC(player, arg1);
    }
    if ((player->effects & 0x400) == 0x400) {
        func_8008C62C(player, arg1);
    }
    if ((player->effects & 0x01000000) == 0x01000000) {
        func_8008E4A4(player, arg1);
    }
    if ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT) {
        apply_hit_by_item_effect(player, arg1);
    }
    if ((player->effects & 0x4000) == 0x4000) {
        func_8008F1B8(player, arg1);
    }
    if ((player->effects & 0x80000) == 0x80000) {
        func_8008D698(player, arg1);
    }
    if ((player->effects & 0x800000) == 0x800000) {
        func_8008D8B4(player, arg1);
        player_decelerate_alternative(player, 10.0f);
    }
    if (gRaceState != RACE_FINISHED) {
        if (player->soundEffects & 0x04000000) {
            func_8008FC64(player, arg1);
        }
        if (player->soundEffects & 0x08000000) {
            func_8008FCDC(player, arg1);
        }
    }
    if (player->unk_044 & 0x800) {
        func_80091298(player, arg1);
    }
}

void func_8002D028(Player* player, s8 arg1) {
    Vec3f sp4C;
    f32 temp_f18;
    s16 temp_t1;
    s16 temp;
    s16 temp2;
    f32 thing0;
    UNUSED s32 pad;

    sp4C[0] = D_80165210[D_80165270[arg1]];
    sp4C[1] = 0;
    sp4C[2] = D_80165230[D_80165270[arg1]];

    temp = -(s16) get_angle_between_two_vectors(player->pos, sp4C);
    temp2 = player->rotation[1];
    temp = (temp - temp2);

    thing0 = 8;

    if (temp > ((s16) (thing0 * 182))) {
        temp = (thing0 * 182);
    }
    if (temp < ((s16) (-thing0 * 182))) {
        temp = (-thing0 * 182);
    }

    temp_t1 = (D_80165020[arg1] + ((s16) ((temp * 0x35) / (thing0 * 182)))) / 2;
    apply_cpu_turn(player, (s16) temp_t1);
    D_80165020[arg1] = (s16) temp_t1;

    temp_f18 = sqrtf((sp4C[0] - player->pos[0]) * (sp4C[0] - player->pos[0]) +
                     (sp4C[2] - player->pos[2]) * (sp4C[2] - player->pos[2]));
    if (temp_f18 <= 8.0f) {
        adjust_angle(&player->rotation[1], -0x8000, 0x016C);
        if ((player->rotation[1] < (-0x7F41)) || (player->rotation[1] > 0x7F41)) {
            player->type &= ~0x0200;
        }
        player->kartPropulsionStrength = 0;
        player->speed = 0;
        player->unk_104 = 0;
        player->tyreSpeed = 0;
        player->unk_07C = 0;
        player->velocity[0] = 0;
        player->velocity[1] = 0;
        player->velocity[2] = 0;
        player->unk_0C0 = 0;
        player->unk_078 = 0;
    } else {
        player->kartPropulsionStrength = 1200;
    }
}

void func_8002D268(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f sp184 = { 0.0, 0.0, 1.0 };
    Vec3f sp178 = { 0.0, 0.0, 0.0 };
    Vec3f sp16C = { 0.0, 0.0, 0.0 };
    Vec3f sp160 = { 0.0, 0.0, 0.0 };
    f32 sp104[] = { 0.825, 0.8, 0.725, 0.625, 0.425, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
                    0.3,   0.3, 0.3,   0.3,   0.3,   0.3, 0.3, 0.3, 0.3, 0.3, 0.3 };
    f32 temp;
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 temp2;
    s32 temp_v0_3;
    s32 temp3;
    f32 temp_f2_2;
    UNUSED s32 pad[8];
    f32 gravityX;
    f32 gravityY;
    f32 gravityZ;
    f32 surfaceDistance;
    UNUSED s32 pad2;
    Vec3f newVelocity;
    Vec3f sp8C;
    UNUSED s32 pad3[3];
    s32 sp7C = 0;
    UNUSED s32 pad4[6];

    func_80027EDC(player, playerId);
    func_8002C11C(player);
    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        func_8002A79C(player, playerId);
    }
    func_8002B830(player, playerId, screenId);
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
        func_8002BF4C(player, playerId);
    }
    apply_effect(player, playerId, screenId);
    if (((player->effects & 0x20000000) == 0x20000000) && (player->unk_228 >= 0x64)) {
        sp7C = 2;
    }
    func_80037BB4(player, sp160);
    func_8002AB70(player);
    func_8002FCA8(player, playerId);
    if (player->unk_044 & 1) {
        player->unk_064[0] *= -1.0f;
        player->unk_064[2] *= -1.0f;
    }
    if ((player->tyres[BACK_LEFT].surfaceType == ASPHALT) && (player->tyres[BACK_RIGHT].surfaceType == ASPHALT)) {
        gravityX = (-1 * (player->unk_064[0] + sp16C[0])) +
                ((-player->collision.orientationVector[0] * player->kartGravity) * 0.925);
        gravityY = (-player->collision.orientationVector[1] * player->kartGravity);
        gravityZ = (-1 * (player->unk_064[2] + sp16C[2])) +
                ((-player->collision.orientationVector[2] * player->kartGravity) * 0.925);
    } else {
        temp3 = (((player->speed / 18.0f) * 216.0f) / 10.0f);
        if (temp3 >= 10) {
            temp3 = 10;
        }
        gravityX = -1 * (player->unk_064[0] + sp16C[0]) +
                ((-player->collision.orientationVector[0] * player->kartGravity) * sp104[temp3]);
        gravityY = (-player->collision.orientationVector[1] * player->kartGravity);
        gravityZ = -1 * (player->unk_064[2] + sp16C[2]) +
                ((-player->collision.orientationVector[2] * player->kartGravity) * sp104[temp3]);
    }
    if (((player->effects & 8) != 8) && ((player->effects & 0x20) == 0x20)) {
        gravityX = 0 * (player->unk_064[0] + sp16C[0]);
        gravityY = -1 * player->kartGravity / 4;
        gravityZ = 0 * (player->unk_064[2] + sp16C[2]);
    }
    if ((player->effects & 8) == 8) {
        gravityX = 0 * (player->unk_064[0] + sp16C[0]);
        gravityY = -1 * player->kartGravity;
        gravityZ = 0 * (player->unk_064[2] + sp16C[2]);
    }
    temp_f2_2 = ((player->oldPos[2] - player->pos[2]) * coss(player->rotation[1] + player->unk_0C0)) +
                (-(player->oldPos[0] - player->pos[0]) * sins(player->rotation[1] + player->unk_0C0));
    if (temp_f2_2 > 0.1) {
        player->unk_044 |= 8;
    } else {
        player->unk_044 &= 0xFFF7;
    }
    if (((player->kartPropulsionStrength <= 0.0f) && ((temp_v0_3 = player->effects, (temp_v0_3 & 1) == 1))) &&
        ((temp_v0_3 & 0x20) != 0x20)) {
        sp178[2] = temp_f2_2 * 4500.0f;
    } else {
        sp178[2] = 0.0f;
    }
    sp178[1] = 0.0f;
    mtxf_translate_vec3f_mat3(sp178, player->orientationMatrix);
    gravityX += sp178[0];
    gravityZ += sp178[2];
    func_8002C7E4(player, playerId, screenId);
    sp184[2] = func_80030150(player, playerId);
    mtxf_translate_vec3f_mat3(sp184, player->orientationMatrix);
    newVelocity[0] = player->velocity[0];
    newVelocity[1] = player->velocity[1];
    newVelocity[2] = player->velocity[2];
    if (((player->unk_10C < 3) && (((s32) player->unk_256) < 3)) ||
        ((player->effects & BOOST_EFFECT) == BOOST_EFFECT)) {

        if (((player->unk_07C >> 16) >= 0x28) || ((player->unk_07C >> 16) < (-0x27))) {

            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) - (newVelocity[0] * (0.12 * ((f64) player->kartFriction)))) /
                        6000.0) /
                        (((((f64) player->unk_20C) * 0.6) + 1.0) + sp7C));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) - (newVelocity[2] * (0.12 * ((f64) player->kartFriction)))) /
                        6000.0) /
                        (((((f64) player->unk_20C) * 0.6) + 1.0) + sp7C));
        } else {
            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) - (newVelocity[0] * (0.12 * ((f64) player->kartFriction)))) /
                        6000.0) /
                        (sp7C + 1));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) - (newVelocity[2] * (0.12 * ((f64) player->kartFriction)))) /
                        6000.0) /
                        (sp7C + 1));
        }
    } else {
        newVelocity[0] +=
            (((((f64) ((sp184[0] + gravityX) + sp160[0])) - (newVelocity[0] * (0.12 * ((f64) player->kartFriction)))) / 6000.0) /
            30.0);
        newVelocity[2] +=
            (((((f64) ((sp184[2] + gravityZ) + sp160[2])) - (newVelocity[2] * (0.12 * ((f64) player->kartFriction)))) / 6000.0) /
            30.0);
    }
    newVelocity[1] +=
        (((((f64) ((sp184[1] + gravityY) + sp160[1])) - (newVelocity[1] * (0.12 * ((f64) player->kartFriction)))) / 6000.0) /
        ((f64) player->unk_DAC));
    if (((((player->unk_0CA & 2) == 2) || ((player->unk_0CA & 8) == 8)) ||
        ((player->effects & HIT_EFFECT) == HIT_EFFECT)) ||
        (player->unk_0CA & 1)) {
        newVelocity[0] = 0.0f;
        newVelocity[1] = 0.0f;
        newVelocity[2] = 0.0f;
    }
    if ((player->unk_044 & 0x10) == 0x10) {
        player->unk_044 &= 0xFFEF;
    }

    posX = player->pos[0];
    posY = player->pos[1];
    posZ = player->pos[2];

    player->oldPos[0] = player->pos[0];
    player->oldPos[2] = player->pos[2];
    player->oldPos[1] = player->pos[1];
    nextX = posX + player->velocity[0] + D_8018CE10[playerId].unk_04[0];
    nextY = posY + player->velocity[1];
    nextZ = posZ + player->velocity[2] + D_8018CE10[playerId].unk_04[2];

    if (((((player->unk_0CA & 2) != 2) && ((player->unk_0CA & 8) != 8)) &&
        ((player->effects & HIT_EFFECT) != HIT_EFFECT)) &&
        (!(player->unk_0CA & 1))) {
        func_8002AAC0(player);
        nextY += player->kartHopVelocity;
        nextY -= 0.02;
    }
    actor_terrain_collision(&player->collision, player->boundingBoxSize, nextX, nextY, nextZ, player->oldPos[0],
                            player->oldPos[1], player->oldPos[2]);
    player->unk_058 = 0.0f;
    player->unk_060 = 0.0f;
    player->unk_05C = 1.0f;
    if ((player->unk_044 & 1) != 1) {
        calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                    player->rotation[1]);
    } else {
        calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                    player->rotation[1] + 0x8000);
    }
    player->effects |= 8;
    player->unk_0C2 += 1;
    surfaceDistance = player->collision.surfaceDistance[2];
    if (surfaceDistance <= 0.0f) {
        player->effects = player->effects & (~2);
        player->effects = player->effects & (~8);
        if (player->unk_0C2 >= 35) {
            if (player->unk_0C2 >= 0x32) {
                player->unk_0C2 = 0x32;
            }
            player->unk_DB4.unkC = 3.0f;
            player->unk_DB4.unk18 = 0;
            player->unk_0B6 |= 0x100;
            if ((((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT)) &&
                ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {

                func_800C9060(playerId, 0x1900A60AU);
            } else if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                        ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                func_800CADD0((u8) playerId, ((f32) player->unk_0C2) / 35.0f);
            }
            if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                player->unk_044 |= 0x100;
            }
        }
        if (((player->unk_0C2 < 0x23) && (player->unk_0C2 >= 0x1C)) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            player->unk_DB4.unkC = 2.8f;
            player->unk_DB4.unk18 = 0;
            if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                func_800CADD0((u8) playerId, ((f32) player->unk_0C2) / 35.0f);
            }
            if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                player->unk_044 |= 0x100;
            }
        }
        if (((player->unk_0C2 < 0x1C) && (player->unk_0C2 >= 4)) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            player->unk_DB4.unk18 = 0;
            player->unk_DB4.unkC = 1.5f;
            if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                if (((player->unk_0C2 < 0xB) && (player->unk_0C2 >= 4)) && (IsBowsersCastle())) {
                    func_800CADD0((u8) playerId, player->unk_0C2 / 14.0f);
                } else {
                    func_800CADD0((u8) playerId, player->unk_0C2 / 25.0f);
                }
            }
        }
        player->unk_0C2 = 0;
        player->kartHopVelocity = player->unk_0C2;
    }
    surfaceDistance = player->collision.surfaceDistance[2];
    if (surfaceDistance <= 0.0f) {
        func_8003F46C(player, sp8C, newVelocity, sp178, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = player->collision.surfaceDistance[0];
    if (surfaceDistance < 0.0f) {
        func_8003F734(player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        func_8002C954(player, playerId, newVelocity);
    }
    surfaceDistance = player->collision.surfaceDistance[1];
    if (surfaceDistance < 0.0f) {
        func_8003FBAC(player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        func_8002C954(player, playerId, newVelocity);
    }
    surfaceDistance = player->collision.surfaceDistance[0];
    if (surfaceDistance >= 0.0f) {
        surfaceDistance = player->collision.surfaceDistance[1];
        if (surfaceDistance >= 0.0f) {
            player->unk_046 &= 0xFFDF;
            if (player->unk_256 != 0) {
                player->unk_256++;
                if (player->unk_256 >= 10) {
                    player->unk_256 = 0;
                }
            }
        }
    }
    if (((!(player->effects & 8)) && (func_802ABDB8(player->collision.meshIndexZX) != 0)) &&
        ((player->effects & 0x10000) != 0x10000)) {
        if ((!(player->unk_0CA & 2)) || (!(player->unk_0CA & 8))) {
            func_8008F494(player, playerId);
        }
    } else if (((!(player->effects & 8)) && (func_802ABDB8(player->collision.meshIndexZX) == 0)) &&
                (player->effects & 0x10000)) {
        func_8008F5A4(player, playerId);
    }
    player->unk_074 = calculate_surface_height(nextX, nextY, nextZ, player->collision.meshIndexZX);
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        (((gActiveScreenMode == SCREEN_MODE_1P) || (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
        (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        func_80029B4C(player, nextX, nextY, nextZ);
    } else {
        func_8002A194(player, nextX, nextY, nextZ);
    }
    func_8002AE38(player, playerId, posX, posZ, nextX, nextZ);

    temp2 = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->previousSpeed = player->speed;
    player->speed = sqrtf(temp2);

    if ((player->kartPropulsionStrength <= 0.0f) && (player->speed <= 0.08) && (D_8018CE10[playerId].unk_04[0] == 0.0f) &&
        (D_8018CE10[playerId].unk_04[2] == 0.0f)) {
        newVelocity[0] = newVelocity[0] + (-1 * newVelocity[0]);
        newVelocity[2] = newVelocity[2] + (-1 * newVelocity[2]);
    } else {
        player->pos[0] = nextX;
        player->pos[2] = nextZ;
    }
    player->pos[1] = nextY;
    if (CVarGetInteger("gNoWallColision", 0)) {
        player->pos[1] = nextY < CVarGetFloat("gMinHeight", 0.0f) ? CVarGetFloat("gMinHeight", 0.0f) : nextY;
    }
    if ((player->type & PLAYER_HUMAN) && (!(player->type & PLAYER_CPU))) {
        func_8002BB9C(player, &nextX, &nextZ, screenId, playerId, newVelocity);
    }
    player->unk_064[0] = sp178[0];
    player->unk_064[2] = sp178[2];
    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];
    gPlayerLastVelocity[playerId][0] = newVelocity[0];
    gPlayerLastVelocity[playerId][1] = newVelocity[1];
    gPlayerLastVelocity[playerId][2] = newVelocity[2];
    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        if (gKartTopSpeedTable[player->characterId] < player->speed) {
            temp = gKartTopSpeedTable[player->characterId] / player->speed;
            player->velocity[0] *= temp;
            player->velocity[1] *= temp;
            player->velocity[2] *= temp;
            player->speed = gKartTopSpeedTable[player->characterId];
        }
    }
    if ((player->unk_044 & 1) == 1) {
        if (player->speed > 1) {
            temp = 1 / player->speed;
            player->velocity[0] *= temp;
            player->velocity[1] *= temp;
            player->velocity[2] *= temp;
            player->speed = 1;
        }
    }
    if (player->collision.surfaceDistance[2] >= 500.0f) {
        player->unk_078 = (s16) (((s16) player->unk_078) / 2);
    }
    update_player_environment_and_hazard_state(player, playerId);
}

void set_player_height(Player* player) {
    s32 player_index;

    player_index = get_player_index_for_player(player);
    player->kartHopJerk = 0.0f;
    player->kartHopAcceleration = 0.0f;
    player->kartHopVelocity = 0.0f;
    player->pos[1] = spawn_actor_on_surface(player->pos[0], gPlayerPathY[player_index] + 10.0f, player->pos[2]) +
                     player->boundingBoxSize;
    if (((player->pos[1] - gPlayerPathY[player_index]) > 1200.0f) ||
        ((player->pos[1] - gPlayerPathY[player_index]) < -1200.0f)) {
        player->pos[1] = player->oldPos[1];
    }
    player->velocity[1] = 0.0f;
}

void func_8002E594(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f spEC = { 0.0f, 0.0f, 1.0f };
    Vec3f spE0 = { 0.0f, 0.0f, 0.0f };
    Vec3f spD4 = { 0.0f, 0.0f, 0.0f };
    f32 spD0;
    f32 spCC;
    f32 spC8;
    f32 temp_f0_6;
    UNUSED s32 pad;
    f32 posX;
    f32 posY;
    f32 posZ;
    UNUSED s32 pad2[12];
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    UNUSED s32 pad3[4];
    f32 temp;
    Vec3f sp54;
    Vec3f sp48;
    s16 sp46;
    func_8002B830(player, playerId, screenId);
    if ((((((((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) ||
            ((player->effects & 0x4000) == 0x4000)) ||
           ((player->effects & 0x80000) == 0x80000)) ||
          ((player->effects & 0x800000) == 0x800000)) ||
         ((player->effects & 0x20000) == 0x20000)) ||
        (player->unk_044 & 0x800)) {
        sp46 = 1;
    } else {
        sp46 = 0;
    }
    apply_effect(player, playerId, screenId);
    func_8002AB70(player);
    func_8002FCA8(player, playerId);
    if ((((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) ||
        ((player->effects & 0x20000) == 0x20000)) {
        sp80 = -1 * (player->unk_064[0]) + (((-player->collision.orientationVector[0]) * player->kartGravity) * 0.1);
        sp7C = (-player->collision.orientationVector[1]) * player->kartGravity;
        sp78 = -1 * (player->unk_064[2]) + (((-player->collision.orientationVector[2]) * player->kartGravity) * 0.1);
    } else {
        sp80 = -1 * player->unk_064[0];
        sp7C = -1 * player->kartGravity;
        sp78 = -1 * player->unk_064[2];
    }
    func_8002C7E4(player, playerId, screenId);
    if (sp46 == 1) {
        calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                     (s16) ((s32) player->rotation[1]));
        calculate_orientation_matrix(player->unk_150, player->unk_058, player->unk_05C, player->unk_060,
                                     (s16) ((s32) player->unk_0AE));
    } else {
        calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                     (s16) ((s32) player->rotation[1]));
    }
    spEC[2] = func_80030150(player, playerId);
    if (sp46 == 1) {
        mtxf_translate_vec3f_mat3(spEC, player->unk_150);
    } else {
        mtxf_translate_vec3f_mat3(spEC, player->orientationMatrix);
    }
    sp54[0] = player->velocity[0];
    sp54[1] = player->velocity[1];
    sp54[2] = player->velocity[2];
    if ((player->unk_10C < 3) && ((player->unk_256) < 3) && ((player->effects & 0x400) != 0x400) &&
        ((player->effects & 0x01000000) != 0x01000000) &&
        ((player->effects & HIT_BY_ITEM_EFFECT) != HIT_BY_ITEM_EFFECT)) {
        sp54[0] += (((((spEC[0] + sp80) + spD4[0])) - (sp54[0] * (0.12 * player->kartFriction))) / 6000) /
                   ((player->unk_20C * 5.0f) + 1.0f);
        sp54[2] += (((((spEC[2] + sp78) + spD4[2])) - (sp54[2] * (0.12 * player->kartFriction))) / 6000) /
                   ((player->unk_20C * 5.0f) + 1.0f);
    } else {
        sp54[0] +=
            ((((f64) (spEC[0] + sp80 + spD4[0]) - (sp54[0] * (0.2 * (f64) player->kartFriction))) / 6000) * 0.08);
        sp54[2] +=
            ((((f64) (spEC[2] + sp78 + spD4[2]) - (sp54[2] * (0.2 * (f64) player->kartFriction))) / 6000) * 0.08);
    }
    sp54[1] += (((((spEC[1] + sp7C) + spD4[1])) - (sp54[1] * (0.12 * player->kartFriction))) / 6000) / player->unk_DAC;

    if (((player->unk_0CA & 2) == 2) || ((player->unk_0CA & 8) == 8)) {
        sp54[0] = 0.0f;
        sp54[1] = 0.0f;
        sp54[2] = 0.0f;
    }
    posX = player->pos[0];
    posY = player->pos[1];
    posZ = player->pos[2];

    player->oldPos[0] = player->pos[0];
    player->oldPos[1] = player->pos[1];
    player->oldPos[2] = player->pos[2];

    spD0 = posX + player->velocity[0] + D_8018CE10[playerId].unk_04[0];
    spCC = posY + player->velocity[1];
    spC8 = posZ + player->velocity[2] + D_8018CE10[playerId].unk_04[2];
    func_8002AAC0(player);
    spCC += player->kartHopVelocity;
    actor_terrain_collision(&player->collision, player->boundingBoxSize, spD0, spCC, spC8, player->oldPos[0],
                            player->oldPos[1], player->oldPos[2]);
    player->effects |= 8;
    player->unk_0C2 += 1;
    player->unk_058 = 0.0f;
    player->unk_060 = 0.0f;
    player->unk_05C = 1.0f;
    sp74 = player->collision.surfaceDistance[2];
    if (sp74 <= 0.0f) {
        player->effects &= ~2;
        player->effects &= ~8;
        if ((((player->effects & 0x400) != 0x400) && ((player->effects & 0x01000000) != 0x01000000)) &&
            ((player->effects & HIT_BY_ITEM_EFFECT) != HIT_BY_ITEM_EFFECT)) {
            if (player->unk_0C2 >= 0x1C) {
                if (player->unk_0C2 >= 0x32) {
                    player->unk_0C2 = 0x0032;
                }
                player->unk_DB4.unk18 = 0;
                player->unk_0B6 |= 0x100;
                player->unk_DB4.unkC = 3.0f;
                if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                    ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                    func_800CADD0((u8) playerId, ((f32) player->unk_0C2) / 50.0f);
                }
                if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                    player->unk_044 |= 0x100;
                }
            }
            if (((player->unk_0C2 < 0x1C) && (player->unk_0C2 >= 0xA)) &&
                (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
                player->unk_DB4.unkC = 2.0f;
                player->unk_DB4.unk18 = 0;
                if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                    ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                    func_800CADD0((u8) playerId, ((f32) player->unk_0C2) / 50.0f);
                }
                if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                    player->unk_044 |= 0x100;
                }
            }
            player->unk_0C2 = 0;
        } else {
            if (player->unk_0C2 >= 0xA) {
                if (player->unk_0C2 >= 0x32) {
                    player->unk_0C2 = 0x0032;
                }
                if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                    ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                    func_800CADD0((u8) playerId, ((f32) player->unk_0C2) / 20.0f);
                }
                if (player->unk_0C2 >= 0x28) {
                    player->unk_0C2 = 0x0014;
                }
                if ((player->effects & 0x400) == 0x400) {
                    player->unk_0C2 /= 6.5;
                    player->kartHopJerk = 0.06f;
                    player->kartHopAcceleration = 0.0f;
                } else {
                    player->unk_0C2 /= 7.5;
                    player->kartHopJerk = 0.06f;
                    player->kartHopAcceleration = 0.0f;
                    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
                        player->unk_044 |= 0x100;
                    }
                }
            } else {
                player->unk_0C2 = 0;
            }
        }
        player->kartHopVelocity = (f32) player->unk_0C2;
    }
    sp74 = player->collision.surfaceDistance[2];
    if (sp74 <= 0.0f) {
        func_8003F46C(player, sp48, sp54, spE0, &sp74, &spD0, &spCC, &spC8);
    }
    sp74 = player->collision.surfaceDistance[0];
    if (sp74 < 0.0f) {
        func_8003F734(player, sp48, sp54, &sp74, &spD0, &spCC, &spC8);
        func_8002C954(player, playerId, sp54);
        player_decelerate_alternative(player, 6.0f);
    }
    sp74 = player->collision.surfaceDistance[1];
    if (sp74 < 0.0f) {
        func_8003FBAC(player, sp48, sp54, &sp74, &spD0, &spCC, &spC8);
        func_8002C954(player, playerId, sp54);
        player_decelerate_alternative(player, 6.0f);
    }
    sp74 = player->collision.surfaceDistance[0];
    if (sp74 >= 0.0f) {
        sp74 = player->collision.surfaceDistance[1];
        if (sp74 >= 0.0f) {
            player->unk_046 &= 0xFFDF;
            if (player->unk_256 != 0) {
                player->unk_256++;
                if ((player->unk_256) >= 0xA) {
                    player->unk_256 = 0;
                }
            }
        }
    }
    if (((func_802ABDB8(player->collision.meshIndexZX) != 0) && ((player->effects & 0x10000) != 0x10000)) &&
        (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
        if ((!(player->unk_0CA & 2)) || (!(player->unk_0CA & 8))) {
            func_8008F494(player, playerId);
        }
    } else if (((!(player->effects & 8)) && (func_802ABDB8(player->collision.meshIndexZX) == 0)) &&
               (player->effects & 0x10000)) {
        func_8008F5A4(player, playerId);
    }
    player->unk_074 = calculate_surface_height(spD0, spCC, spC8, player->collision.meshIndexZX);
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        (((gActiveScreenMode == SCREEN_MODE_1P) || (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
         (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        func_80029B4C(player, spD0, spCC, spC8);
    } else {
        func_8002A194(player, spD0, spCC, spC8);
    }
    func_8002AE38(player, playerId, posX, posZ, spD0, spC8);
    temp = (sp54[0] * sp54[0]) + (sp54[2] * sp54[2]);
    player->previousSpeed = player->speed;
    player->speed = sqrtf(temp);
    if ((((player->effects & 0x400) != 0x400) && (player->kartPropulsionStrength <= 0) && (player->speed < 0.13)) ||
        (((player->effects & 0x400) != 0x400) && (player->kartPropulsionStrength <= 0) && (player->speed < 0.20) &&
         ((player->effects & 1) == 1))) {
        sp54[0] = sp54[0] + (-1 * sp54[0]);
        sp54[2] = sp54[2] + (-1 * sp54[2]);
    } else {
        player->pos[0] = spD0;
        player->pos[2] = spC8;
    }
    player->pos[1] = spCC;
    player->unk_064[0] = spE0[0];
    player->unk_064[2] = spE0[2];
    player->velocity[0] = sp54[0];
    player->velocity[1] = sp54[1];
    player->velocity[2] = sp54[2];
    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        if (gKartTopSpeedTable[player->characterId] < player->speed) {
            temp_f0_6 = gKartTopSpeedTable[player->characterId] / player->speed;
            player->velocity[0] *= temp_f0_6;
            player->velocity[1] *= temp_f0_6;
            player->velocity[2] *= temp_f0_6;
            player->speed = gKartTopSpeedTable[player->characterId];
        }
    }
    update_player_environment_and_hazard_state(player, playerId);
}

void control_cpu_movement(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    UNUSED Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    f32 nextX;
    UNUSED s32 pad;
    f32 nextZ;
    UNUSED s32 pad2[15];
    f32 sp84;
    UNUSED s32 pad3;
    f32 sp7C;
    UNUSED s32 pad4[2];
    Vec3f newVelocity;
    UNUSED f32 pad5[7];
    f32 toSqrt;
    f32 topSpeedMultiplier;
    f32 nextY;
    player->effects |= 0x1000;
    player->unk_044 |= 0x10;
    nextY = gPlayerPathY[playerId];
    player->driftDuration = 0;
    player->effects &= ~DRIFTING_EFFECT;
    func_8002B830(player, playerId, screenId);
    apply_effect(player, playerId, screenId);
    sp84 = 0 * player->unk_064[0] + spDC[0];
    sp7C = 0 * player->unk_064[2] + spDC[2];
    player->unk_10C = 0;
    player->unk_256 = 0;
    player->effects &= ~0x8000;
    spF4[2] = func_80030150(player, playerId);
    mtxf_translate_vec3f_mat3(spF4, player->orientationMatrix);
    newVelocity[0] = player->velocity[0];
    newVelocity[1] = 0;
    newVelocity[2] = player->velocity[2];
    newVelocity[0] += (((spF4[0] + sp84) + spD0[0]) - (newVelocity[0] * (0.12 * player->kartFriction))) / 6000.0;
    newVelocity[2] += (((spF4[2] + sp7C) + spD0[2]) - (newVelocity[2] * (0.12 * player->kartFriction))) / 6000.0;
    player->oldPos[0] = player->pos[0];
    player->oldPos[1] = nextY;
    player->oldPos[2] = player->pos[2];
    nextX = player->pos[0] + player->velocity[0];
    nextZ = player->pos[2] + player->velocity[2];
    player->unk_0C0 = 0;
    player->kartHopJerk = 0;
    player->kartHopAcceleration = 0;
    player->kartHopVelocity = 0;
    calculate_orientation_matrix(player->orientationMatrix, player->unk_058, player->unk_05C, player->unk_060,
                                 player->rotation[1]);
    player->unk_0C2 = 0;
    player->effects &= ~2;
    player->effects &= ~8;
    player->slopeAccel = 0;
    player->unk_206 = 0;
    toSqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->previousSpeed = player->speed;
    player->speed = sqrtf(toSqrt);
    player->pos[0] = nextX;
    player->pos[2] = nextZ;
    player->pos[1] = nextY;
    player->unk_064[0] = 0;
    player->unk_064[2] = 0;
    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];
    gPlayerLastVelocity[playerId][0] = newVelocity[0];
    gPlayerLastVelocity[playerId][1] = newVelocity[1];
    gPlayerLastVelocity[playerId][2] = newVelocity[2];
    if (gKartTopSpeedTable[player->characterId] < player->speed) {
        topSpeedMultiplier = gKartTopSpeedTable[player->characterId] / player->speed;
        player->velocity[0] *= topSpeedMultiplier;
        player->velocity[1] *= topSpeedMultiplier;
        player->velocity[2] *= topSpeedMultiplier;
        player->speed = gKartTopSpeedTable[player->characterId];
    }
}

void func_8002F730(Player* player, UNUSED Camera* camera, UNUSED s8 screenId, s8 playerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    UNUSED Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    f32 spCC;
    f32 spC8;
    f32 spC4;

    f32 spC0;
    f32 sp44;
    f32 spB8;
    f32 temp_f0_2;
    UNUSED s32 pad[11];
    f32 sp84;
    UNUSED s32 pad2;
    f32 sp7C;
    f32 sp78;
    f32 sqrt;
    Vec3f sp68;
    Vec3f sp5C;
    UNUSED s32 pad3[3];

    func_80037BB4(player, spD0);
    sp84 = player->unk_064[0] * 0;
    sp7C = player->unk_064[2] * 0;
    spF4[2] = func_80030150(player, playerId);

    mtxf_translate_vec3f_mat3(spF4, player->orientationMatrix);

    sp68[0] = player->velocity[0];
    sp68[1] = player->velocity[1];
    sp68[2] = player->velocity[2];

    sp68[0] += (((f64) (spF4[0] + sp84) - (sp68[0] * 780.0)) / 6500.0);
    sp68[2] += (((f64) (spF4[2] + sp7C) - (sp68[2] * 780.0)) / 6500.0);
    sp68[1] += (((f64) (spF4[1] + -1100.0f) - (sp68[1] * 780.0)) / 6500.0);

    spC0 = player->pos[0];
    sp44 = player->pos[1];
    spB8 = player->pos[2];

    player->oldPos[0] = player->pos[0];
    player->oldPos[1] = player->pos[1];
    player->oldPos[2] = player->pos[2];

    spCC = player->velocity[0] + spC0;
    spC8 = player->velocity[1] + sp44;
    spC4 = player->velocity[2] + spB8;

    func_8002AAC0(player);

    spC8 += player->kartHopVelocity;
    actor_terrain_collision(&player->collision, player->boundingBoxSize, spCC, spC8, spC4, player->oldPos[0],
                            player->oldPos[1], player->oldPos[2]);
    player->unk_058 = 0.0f;
    player->unk_05C = 1.0f;
    player->unk_060 = 0.0f;
    calculate_orientation_matrix(player->orientationMatrix, 0.0f, 1.0f, 0.0f, (s16) (s32) player->rotation[1]);
    player->effects &= ~8;
    sp78 = player->collision.surfaceDistance[2];
    if (sp78 <= 0.0f) {
        if (1) {};
        func_8003F46C(player, sp5C, sp68, spE8, &sp78, &spCC, &spC8, &spC4);
    }
    player->unk_074 = calculate_surface_height(spCC, spC8, spC4, player->collision.meshIndexZX);
    if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        ((gActiveScreenMode == SCREEN_MODE_1P) || (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL) ||
         (gActiveScreenMode == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        func_80029B4C(player, spCC, spC8, spC4);
    } else {
        func_8002A194(player, spCC, spC8, spC4);
    }
    func_8002AE38(player, playerId, spC0, spB8, spCC, spC4);
    sqrt = (sp68[0] * sp68[0]) + (sp68[1] * sp68[1]) + (sp68[2] * sp68[2]);
    player->previousSpeed = player->speed;
    player->speed = sqrtf(sqrt);
    if (((player->kartPropulsionStrength <= 0.0f) && ((f64) player->speed < 0.13)) ||
        ((player->kartPropulsionStrength <= 0.0f) && ((f64) player->speed < 0.2) && ((player->effects & 1) == 1))) {
        sp68[0] = sp68[0] + (sp68[0] * -1.0f);
        sp68[2] = sp68[2] + (sp68[2] * -1.0f);
    } else {
        player->pos[0] = spCC;
        player->pos[2] = spC4;
    }
    player->pos[1] = spC8 - 0.018;

    player->unk_064[0] = spE8[0];
    player->unk_064[2] = spE8[2];

    player->velocity[0] = sp68[0];
    player->velocity[1] = sp68[1];
    player->velocity[2] = sp68[2];
    gPlayerLastVelocity[playerId][0] = sp68[0];
    gPlayerLastVelocity[playerId][1] = sp68[1];
    gPlayerLastVelocity[playerId][2] = sp68[2];

    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        if (gKartTopSpeedTable[player->characterId] < player->speed) {
            temp_f0_2 = gKartTopSpeedTable[player->characterId] / player->speed;
            player->velocity[0] *= temp_f0_2;
            player->velocity[1] *= temp_f0_2;
            player->velocity[2] *= temp_f0_2;
            player->speed = gKartTopSpeedTable[player->characterId];
        }
    }
}

void func_8002FCA8(Player* player, s8 arg1) {
    f32 var_f0;
    f32 var_f12;
    s32 temp_lo;
    s32 var_v1;

    var_f0 = 0.0f;
    if ((player->effects & STAR_EFFECT) != STAR_EFFECT) {
        if ((s32) player->tyres[BACK_RIGHT].surfaceType < 0xF) {
            var_f0 += D_800E2A90[player->characterId][player->tyres[BACK_RIGHT].surfaceType];
        }
        if ((s32) player->tyres[BACK_LEFT].surfaceType < 0xF) {
            var_f0 += D_800E2A90[player->characterId][player->tyres[BACK_LEFT].surfaceType];
        }
        if ((s32) player->tyres[FRONT_RIGHT].surfaceType < 0xF) {
            var_f0 += D_800E2AB0[player->characterId][player->tyres[FRONT_RIGHT].surfaceType];
        }
        if ((s32) player->tyres[FRONT_LEFT].surfaceType < 0xF) {
            var_f0 += D_800E2AB0[player->characterId][player->tyres[FRONT_LEFT].surfaceType];
        }
    }
    if (D_801652C0[arg1] & 4) {
        var_v1 = 2;
    } else {
        var_v1 = 0;
    }
    if ((player->currentSpeed >= 200.0f) && (var_v1 == 2)) {
        temp_lo = (s16) player->unk_0C0 / 182;
        if ((temp_lo > 0xF) || (temp_lo < -0xF)) {
            var_f0 += 1.0;
        }
    }
    if (((player->effects & 0x200000) == 0x200000) && ((player->type & PLAYER_HUMAN) != PLAYER_HUMAN)) {
        var_f0 = -3.0f;
    }

    if (player->unk_088 >= 0.0f) {
        var_f12 = player->unk_088 * var_f0;
    } else {
        var_f12 = -player->unk_088 * var_f0;
    }
    player->unk_208 = player->unk_088 - var_f12;
}

void func_8002FE84(Player* player, f32 arg1) {
    f32 temp_f0_3;
    f32 var_f0;
    s16 temp_lo;
    s32 test;

    if ((player->effects & 0x4000) == 0x4000) {
        player->unk_098 = ((player->currentSpeed * player->currentSpeed) / 25.0f) * 1.1;
        return;
    }

    // Huh?
    if (((player->effects & 0xFFFFFFFF) & 8) == 8) {
        player->unk_098 = ((player->currentSpeed * player->currentSpeed) / 25.0f) * 1.1;
        return;
    }

    var_f0 = 0.0f;
    player->unk_098 = arg1;
    temp_lo = player->slopeAccel / 182;
    if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
        var_f0 += (temp_lo * 0.0125) / 1.2;
    } else {
        var_f0 += (temp_lo * 0.025) / 1.2;
    }
    player->unk_098 = arg1 * (1.0f - var_f0);
    if (player->tyres[BACK_RIGHT].surfaceType == GRASS) {
        var_f0 += D_800E2E90[player->characterId][player->tyres[BACK_RIGHT].surfaceType] * 0.7;
    }
    if (player->tyres[BACK_LEFT].surfaceType == GRASS) {
        // The unecessary "* 1.0" here is to force the compiler to save this 0.7 as a separate RO value from the 0.7
        // just above this comment
        var_f0 += D_800E2E90[player->characterId][player->tyres[BACK_LEFT].surfaceType] * (0.7 * 1.0);
    }
    test = player->unk_0C0 / 182;
    if (test < 0) {
        var_f0 += -test * 0.004;
    } else {
        var_f0 += test * 0.004;
    }
    player->unk_098 = arg1 * (1.0 + (var_f0 * 0.7));
    if ((player->effects & 0x20) == 0x20) {
        temp_f0_3 = player->currentSpeed + 180.0f;
        player->unk_098 = (temp_f0_3 * temp_f0_3) / 25.0f;
    }
}

f32 func_80030150(Player* player, s8 arg1) {
    f32 var_f0;
    s16 temp_lo;
    f32 var_f2;
    s32 var_v0;

    var_f0 = 0.0f;
    var_f2 = (player->speed / 18.0f) * 216.0f;
    if (var_f2 >= 8.0f) {
        if ((player->effects & STAR_EFFECT) != STAR_EFFECT) {
            if ((s32) player->tyres[BACK_RIGHT].surfaceType >= 0xF) {
                // ???????
                if (1) {}
            } else {
                var_f0 += D_800E2E90[player->characterId][player->tyres[BACK_RIGHT].surfaceType];
            }

            if ((s32) player->tyres[BACK_LEFT].surfaceType < 0xF) {
                var_f0 += D_800E2E90[player->characterId][player->tyres[BACK_LEFT].surfaceType];
            }

            if ((s32) player->tyres[FRONT_RIGHT].surfaceType < 0xF) {
                var_f0 += D_800E2EB0[player->characterId][player->tyres[FRONT_RIGHT].surfaceType];
            }

            if ((s32) player->tyres[FRONT_LEFT].surfaceType < 0xF) {
                var_f0 += D_800E2E90[player->characterId][player->tyres[FRONT_LEFT].surfaceType];
            }
        }
        if (((player->effects & 8) != 8) && ((player->unk_0CA & 2) != 2)) {
            temp_lo = player->slopeAccel / 182;
            if (var_f2 >= 20.0f) {
                if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
                    var_f0 -= ((temp_lo * 0.0126) / 3.0);
                } else {
                    var_f0 -= ((temp_lo * 0.026) / 3.0);
                }
            } else {
                var_f0 += -0.2;
                if ((player->effects & LIGHTNING_EFFECT) == LIGHTNING_EFFECT) {
                    var_f0 += -0.55;
                }
            }
            if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) || (player->driftDuration > 0)) {
                var_v0 = (s16) player->unk_0C0 / 182;
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.004;
                } else {
                    var_f0 += var_v0 * 0.004;
                }
            } else {
                var_v0 = (s16) player->unk_0C0 / 182;
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * (0.01 + gKartTurnSpeedReductionTable0[player->characterId]);
                } else {
                    var_f0 += var_v0 * (0.01 + gKartTurnSpeedReductionTable0[player->characterId]);
                }
            }
            if (((player->effects & 0x20000000) == 0x20000000) && (player->unk_228 < 0xA)) {
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.008;
                } else {
                    var_f0 += var_v0 * 0.008;
                }
            }
            if ((player->effects & STAR_EFFECT) == STAR_EFFECT) {
                var_f0 += -0.25;
            }
        }
        if ((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) == WATER_IS_FULLY_SUBMERGED) {
            var_f0 += 0.3;
        } else {
            if ((player->waterInteractionFlags & WATER_IS_PARTIALLY_SUBMERGED) == WATER_IS_PARTIALLY_SUBMERGED) {
                var_f0 += 0.15;
            }
            if (((gPlayerWaterLevel[arg1] - player->tyres[BACK_LEFT].baseHeight) >= 3.5) ||
                ((gPlayerWaterLevel[arg1] - player->tyres[BACK_RIGHT].baseHeight) >= 3.5)) {
                var_f0 += 0.05;
            }
        }
        if ((player->effects & 8) != 0) {
            move_f32_towards(&player->unk_0A0, player->kartPropulsionStrength * 0.04, 0.15f);
        } else {
            move_f32_towards(&player->unk_0A0, 0.0f, 0.1f);
        }
    } else {
        player->unk_0A0 = 0.0f;
        player->unk_0E8 = 0.0f;
        if (((s16) player->slopeAccel / 182) < 0) {
            var_f0 += -0.85;
            if (player->effects & LIGHTNING_EFFECT) {
                var_f0 += -0.55;
            }
        }
    }
    if ((player->type & PLAYER_HUMAN) != PLAYER_HUMAN) {
        if ((player->effects & 0x200000) == 0x200000) {
            move_f32_towards(&player->unk_0E8, 380.0f, 0.5f);
        } else {
            move_f32_towards(&player->unk_0E8, 0.0f, 0.1f);
        }
    }
    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        if (((player->effects & 0x100) == 0x100) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT)) {
            move_f32_towards(&player->unk_0E8, 580.0f, 0.2f);
        } else {
            move_f32_towards(&player->unk_0E8, 0.0f, 0.01f);
        }
        if ((player->effects & 0x200000) == 0x200000) {
            move_f32_towards(&player->unk_0E4, 580.0f, 0.01f);
        } else {
            move_f32_towards(&player->unk_0E4, 0.0f, 0.01f);
        }
    }
    move_f32_towards(&player->unk_104, var_f0, gKartTurnSpeedReductionTable1[player->characterId] + 0.05);
    var_f2 = (player->kartPropulsionStrength + player->unk_0E8 + player->boostPower + player->unk_0E4) - player->unk_0A0;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (((player->unk_0CA & 2) == 2) || ((player->unk_0CA & 8) == 8) ||
        ((player->type & PLAYER_START_SEQUENCE) == PLAYER_START_SEQUENCE)) {
        return (1.0f - player->unk_104) * var_f2;
    }
    if (((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40) ||
        ((player->effects & 0x01000000) == 0x01000000) ||
        ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT)) {
        return (1.0f - player->unk_104) * var_f2;
    }
    // Have to `and` the 0x2000 here to force the compiler to reload the immediate
    if (((player->effects & BOOST_EFFECT) == (BOOST_EFFECT & 0xFFFFFFFF)) ||
        ((player->effects & BOOST_RAMP_ASPHALT_EFFECT) == BOOST_RAMP_ASPHALT_EFFECT) ||
        ((player->effects & BOOST_RAMP_WOOD_EFFECT) == BOOST_RAMP_WOOD_EFFECT)) {
        func_8002FE84(player, player->boostPower + player->kartPropulsionStrength);
        return player->boostPower + player->kartPropulsionStrength;
    }
    func_8002FE84(player, var_f2);
    return (1.0f - player->unk_104) * var_f2;
}

void update_player_kart_hop(Player* player) {
    f32 hopInitialUpwardVelocity;
    f32 hopDownwardAcceleration;

    if (((player->unk_0CA & 2) != 2) && ((player->unk_0CA & 8) != 8)) {
        if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (player->unk_DB4.unkC < 1.0f)) {
            switch (player->surfaceType) { /* irregular */
                case ASPHALT:
                    if (random_int(0x000AU) != 8) {
                        hopInitialUpwardVelocity = 0.35f;
                        hopDownwardAcceleration = 0.55f;
                    } else {
                        player->hopFrameCounter = 0;
                        player->hopVerticalOffset = 0.0f;
                        hopInitialUpwardVelocity = 0.0f;
                        hopDownwardAcceleration = 0.0f;
                    }
                    break;
                case TRAIN_TRACK:
                case ROPE_BRIDGE:
                    hopInitialUpwardVelocity = 0.94f;
                    hopDownwardAcceleration = 0.85f;
                    break;
                default:
                    if (1) {}
                    hopInitialUpwardVelocity = 0.46f;
                    hopDownwardAcceleration = 0.48f;
                    break;
            }
        } else if (random_int(0x000AU) != 8) {
            hopInitialUpwardVelocity = 0.3f;
            hopDownwardAcceleration = 0.54f;
        } else {
            player->hopFrameCounter = 0;
            player->hopVerticalOffset = 0.0f;
            hopInitialUpwardVelocity = 0.0f;
            hopDownwardAcceleration = 0.0f;
        }

        // Increase the hop frame counter
        player->hopFrameCounter += 1;

        // Vertical displacement under constant acceleration: Δy = v₀t + (1/2)at²
        player->hopVerticalOffset = (player->hopFrameCounter * hopInitialUpwardVelocity) - (0.5 * hopDownwardAcceleration * (player->hopFrameCounter * player->hopFrameCounter));

        if ((player->hopFrameCounter != 0) && (player->hopVerticalOffset < 0.0f)) {
            player->hopFrameCounter = 0;
        }
        if (player->hopVerticalOffset <= 0.0f) {
            player->hopVerticalOffset = 0.0f;
        }
    }
}

void detect_triple_a_combo_a_released(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }
    if (gIsPlayerTripleAButtonCombo[playerIndex] == false) {
        if (gPlayerIsThrottleActive[playerIndex] == true) {
            if ((gFrameSinceLastACombo[playerIndex] < 2) || (gFrameSinceLastACombo[playerIndex] >= 9)) {
                gCountASwitch[playerIndex] = 0;
            }
            gFrameSinceLastACombo[playerIndex] = 0;
            gPlayerAButtonComboActiveThisFrame[playerIndex] = 0;
        }
        gPlayerIsThrottleActive[playerIndex] = false;
        gFrameSinceLastACombo[playerIndex]++;
        if (gFrameSinceLastACombo[playerIndex] >= 9) {
            gFrameSinceLastACombo[playerIndex] = 9;
        }
        if ((gFrameSinceLastACombo[playerIndex] >= 2) && (gFrameSinceLastACombo[playerIndex] < 9)) {
            if (gPlayerAButtonComboActiveThisFrame[playerIndex] == 0) {
                gCountASwitch[playerIndex] += 1;
            }
            gPlayerAButtonComboActiveThisFrame[playerIndex] = 1;
        }
        if (gCountASwitch[playerIndex] == 5) {
            gIsPlayerTripleAButtonCombo[playerIndex] = true;
            gTimerBoostTripleACombo[playerIndex] = 120;
            gCountASwitch[playerIndex] = 0;
            gFrameSinceLastACombo[playerIndex] = 0;
        }
    } else {
        gTimerBoostTripleACombo[playerIndex]--;
        if (gTimerBoostTripleACombo[playerIndex] <= 0) {
            gIsPlayerTripleAButtonCombo[playerIndex] = false;
        }
    }
}

void detect_triple_a_combo_a_pressed(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }
    if (gIsPlayerTripleAButtonCombo[playerIndex] == false) {
        if (gPlayerIsThrottleActive[playerIndex] == false) {
            if ((gFrameSinceLastACombo[playerIndex] < 2) || (gFrameSinceLastACombo[playerIndex] >= 9)) {
                gCountASwitch[playerIndex] = 0;
            }
            gFrameSinceLastACombo[playerIndex] = 0;
            gPlayerAButtonComboActiveThisFrame[playerIndex] = 0;
        }
        gPlayerIsThrottleActive[playerIndex] = true;
        gFrameSinceLastACombo[playerIndex]++;
        if (gFrameSinceLastACombo[playerIndex] >= 9) {
            gFrameSinceLastACombo[playerIndex] = 9;
        }
        if ((gFrameSinceLastACombo[playerIndex] >= 2) && (gFrameSinceLastACombo[playerIndex] < 9)) {
            if (gPlayerAButtonComboActiveThisFrame[playerIndex] == 0) {
                gCountASwitch[playerIndex] += 1;
            }
            gPlayerAButtonComboActiveThisFrame[playerIndex] = 1;
        }
        if (gCountASwitch[playerIndex] == 5) {
            gIsPlayerTripleAButtonCombo[playerIndex] = true;
            gTimerBoostTripleACombo[playerIndex] = 120;
            gCountASwitch[playerIndex] = 0;
            gFrameSinceLastACombo[playerIndex] = 0;
        }
    } else {
        gTimerBoostTripleACombo[playerIndex]--;
        if (gTimerBoostTripleACombo[playerIndex] <= 0) {
            gIsPlayerTripleAButtonCombo[playerIndex] = false;
        }
    }
}

void player_accelerate_alternative(Player* player) {
    s32 player_index;

    player_index = get_player_index_for_player(player);
    if (gIsPlayerTripleAButtonCombo[player_index] == false) {
        if ((0.0 <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.1))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][0] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.1) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.2))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][1] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.2) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.3))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][2] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.3) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.4))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][3] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.4) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.5))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][4] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.5) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.6))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][5] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.6) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.7))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][6] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.7) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.8))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][7] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.8) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.9))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][8] + (0.05 * (player->slopeAccel / 182));
        }
        if (((player->topSpeed * 0.9) <= player->currentSpeed) && (player->currentSpeed <= (player->topSpeed * 1.0))) {
            player->currentSpeed +=
                gKartAccelerationTables[player->characterId][9] + (0.05 * (player->slopeAccel / 182));
        }
    } else {
        if ((0.0 <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.1))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][0] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.1) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.2))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][1] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.2) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.3))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][2] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.3) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.4))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][3] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.4) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.5))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][4] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.5) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.6))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][5] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.6) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.7))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][6] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.7) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.8))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][7] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.8) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.9))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][8] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
        if (((player->topSpeed * 0.9) <= player->currentSpeed) && (player->currentSpeed <= (player->topSpeed * 1.0))) {
            player->currentSpeed +=
                (gKartAccelerationTables[player->characterId][9] + (0.05 * (player->slopeAccel / 182))) *
                gKartTripleABoost[player->characterId];
        }
    }
    if (player->currentSpeed < 0.0f) {
        player->currentSpeed = 0.0f;
    }
    if (player->topSpeed <= player->currentSpeed) {
        player->currentSpeed = player->topSpeed;
    }
    if (!((player->effects & 8)) || ((player->effects & LIGHTNING_EFFECT))) {
        player->kartPropulsionStrength = (player->currentSpeed * player->currentSpeed) / 25.0f;
    }
    player->unk_044 |= 0x20;
    if ((player->soundEffects * 8) < 0) {
        func_8008F104(player, player_index);
        player->soundEffects &= 0xEFFFFFFF;
    }
}

void player_decelerate_alternative(Player* player, f32 speed) {
    s32 player_index;
    player_index = get_player_index_for_player(player);

    player->currentSpeed -= speed;
    if (player->currentSpeed <= 0.0f) {
        player->currentSpeed = 0.0f;
    }
    if (player->speed < 0.2) {
        player->kartPropulsionStrength = 0.0f;
    }
    if (player->topSpeed <= player->currentSpeed) {
        player->currentSpeed = player->topSpeed;
    }
    if ((player->effects & 8) != 8) {
        player->kartPropulsionStrength = (player->currentSpeed * player->currentSpeed) / 25.0f;
    }
    player->unk_044 &= 0xFFDF;
    if ((player->soundEffects * 8) < 0) {
        func_8008F104(player, player_index);
        player->soundEffects &= 0xEFFFFFFF;
    }
}

void detect_triple_b_combo_b_released(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }

    if (gIsPlayerTripleBButtonCombo[playerIndex] == false) {
        if (gPlayerIsBrakeActive[playerIndex] == true) {
            if ((gFrameSinceLastBCombo[playerIndex] < 2) || (gFrameSinceLastBCombo[playerIndex] >= 9)) {
                gCountBChangement[playerIndex] = 0;
            }
            gFrameSinceLastBCombo[playerIndex] = 0;
            gPlayerBButtonComboActiveThisFrame[playerIndex] = 0;
        }
        gPlayerIsBrakeActive[playerIndex] = false;
        gFrameSinceLastBCombo[playerIndex]++;
        if (gFrameSinceLastBCombo[playerIndex] >= 9) {
            gFrameSinceLastBCombo[playerIndex] = 9;
        }
        if ((gFrameSinceLastBCombo[playerIndex] >= 2) && (gFrameSinceLastBCombo[playerIndex] < 9)) {
            if (gPlayerBButtonComboActiveThisFrame[playerIndex] == 0) {
                gCountBChangement[playerIndex]++;
            }
            gPlayerBButtonComboActiveThisFrame[playerIndex] = 1;
        }
        if (gCountBChangement[playerIndex] == 5) {
            gIsPlayerTripleBButtonCombo[playerIndex] = true;
            gTimerBoostTripleBCombo[playerIndex] = 120;
            gCountBChangement[playerIndex] = 0;
            gFrameSinceLastBCombo[playerIndex] = 0;
        }
    } else {
        gTimerBoostTripleBCombo[playerIndex]--;
        if (gTimerBoostTripleBCombo[playerIndex] <= 0) {
            gIsPlayerTripleBButtonCombo[playerIndex] = false;
        }
    }
}

void detect_triple_b_combo_b_pressed(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }

    if (gIsPlayerTripleBButtonCombo[playerIndex] == false) {
        if (gPlayerIsBrakeActive[playerIndex] == false) {
            if ((gFrameSinceLastBCombo[playerIndex] < 2) || (gFrameSinceLastBCombo[playerIndex] >= 9)) {
                gCountBChangement[playerIndex] = 0;
            }
            gFrameSinceLastBCombo[playerIndex] = 0;
            gPlayerBButtonComboActiveThisFrame[playerIndex] = 0;
        }
        gPlayerIsBrakeActive[playerIndex] = true;
        gFrameSinceLastBCombo[playerIndex]++;
        if (gFrameSinceLastBCombo[playerIndex] >= 9) {
            gFrameSinceLastBCombo[playerIndex] = 9;
        }
        if ((gFrameSinceLastBCombo[playerIndex] >= 2) && (gFrameSinceLastBCombo[playerIndex] < 9)) {
            if (gPlayerBButtonComboActiveThisFrame[playerIndex] == 0) {
                gCountBChangement[playerIndex]++;
            }
            gPlayerBButtonComboActiveThisFrame[playerIndex] = 1;
        }
        if (gCountBChangement[playerIndex] == 5) {
            gIsPlayerTripleBButtonCombo[playerIndex] = true;
            gTimerBoostTripleBCombo[playerIndex] = 120;
            gCountBChangement[playerIndex] = 0;
            gFrameSinceLastBCombo[playerIndex] = 0;
        }
    } else {
        gTimerBoostTripleBCombo[playerIndex]--;
        if (gTimerBoostTripleBCombo[playerIndex] <= 0) {
            gIsPlayerTripleBButtonCombo[playerIndex] = false;
        }
    }
}

void func_800323E4(Player* player) {
    s32 var_v1;
    f32 test;
    f32 var_f2;

    var_f2 = 0.0f;
    if (player == gPlayerOne) {
        var_v1 = 0;
    }
    if (player == gPlayerTwo) {
        var_v1 = 1;
    }
    if (player == gPlayerThree) {
        var_v1 = 2;
    }
    if (player == gPlayerFour) {
        var_v1 = 3;
    }
    if (player == gPlayerFive) {
        var_v1 = 4;
    }
    if (player == gPlayerSix) {
        var_v1 = 5;
    }
    if (player == gPlayerSeven) {
        var_v1 = 6;
    }
    if (player == gPlayerEight) {
        var_v1 = 7;
    }
    player->effects |= 1;
    // This check will never be true, why is it here?
    if ((player->effects & 0x20) == 0x20) {
        player_decelerate_alternative(player, 1.0f);
        player->unk_20C = var_f2;
    } else {
        if ((s32) player->tyres[BACK_RIGHT].surfaceType < 0xF) {
            var_f2 += D_800E3210[player->characterId][player->tyres[BACK_RIGHT].surfaceType];
        }
        if ((s32) player->tyres[BACK_LEFT].surfaceType < 0xF) {
            var_f2 += D_800E3210[player->characterId][player->tyres[BACK_LEFT].surfaceType];
        }
        test = player->previousSpeed - player->speed;
        if (test <= 0.0f) {
            player->unk_20C = 0.0f;
        } else {
            player->unk_20C += 0.02;
            if (player->unk_20C >= 2.0f) {
                player->unk_20C = 2.0f;
            }
        }
        if (gIsPlayerTripleBButtonCombo[var_v1] == true) {
            if (player->unk_20C >= 2.0f) {
                player_decelerate_alternative(player, (1.0f - var_f2) * 5.0f);
            } else {
                player_decelerate_alternative(player, (1.0f - var_f2) * 3.0f);
            }
        } else {
            if (((player->speed / 18.0f) * 216.0f) <= 20.0f) {
                player_decelerate_alternative(player, (1.0f - var_f2) * 4.0f);
            }
            if (player->unk_20C >= 2.0f) {
                player_decelerate_alternative(player, (1.0f - var_f2) * 2.5);
            } else {
                player_decelerate_alternative(player, (1.0f - var_f2) * 1.2);
            }
        }
    }
}

void player_accelerate_during_start_sequence(Player* player) {
    s32 temp_v0;
    s32 var_v0;
    s32 test;

    temp_v0 = get_player_index_for_player(player);
    if ((player->currentSpeed >= 0.0) && (player->currentSpeed < (player->topSpeed * 0.1))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][0] * 3.0;
    }
    if (((player->topSpeed * 0.1) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.2))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][1] * 3.0;
    }
    if (((player->topSpeed * 0.2) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.3))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][2] * 3.0;
    }
    if (((player->topSpeed * 0.3) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.4))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][3] * 3.0;
    }
    if (((player->topSpeed * 0.4) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.5))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][4] * 3.0;
    }
    if (((player->topSpeed * 0.5) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.6))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][5] * 3.0;
    }
    if (((player->topSpeed * 0.6) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.7))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][6] * 2.5;
    }
    if (((player->topSpeed * 0.7) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.8))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][7] * 2.5;
    }
    if (((player->topSpeed * 0.8) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.9))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][8] * 2.5;
    }
    if (((player->topSpeed * 0.9) <= player->currentSpeed) && (player->currentSpeed <= player->topSpeed * 1.0)) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][9] * 2.5;
    }
    if (D_801656F0 == 1) {
        test = gRaceFrameCounter - D_801652E0[temp_v0];
        if (gModeSelection == TIME_TRIALS) {
            var_v0 = 0x14;
        } else {
            var_v0 = 8;
        }
        if ((test < var_v0) && ((player->unk_044 & 0x20) != 0x20)) {
            player->soundEffects |= 0x02000000;
        } else if ((player->topSpeed * 0.9f) <= player->currentSpeed) {
            if ((player->soundEffects & 0x02000000) != 0x02000000) {
                player->soundEffects |= 0x10000000;
                player->soundEffects &= ~0x02000000;
            }
        }
    }
    player->unk_044 |= 0x20;
    player->unk_098 = (player->currentSpeed * player->currentSpeed) / 25.0f;
}

void player_decelerate_during_start_sequence(Player* player, f32 speedReduction) {
    player->currentSpeed -= speedReduction;
    if (player->currentSpeed <= 0.0f) {
        player->currentSpeed = 0.0f;
    }
    if (player->speed < 0.2) {
        player->kartPropulsionStrength = 0.0f;
    }
    if (player->topSpeed <= player->currentSpeed) {
        player->currentSpeed = player->topSpeed;
    }
    if ((f64) player->currentSpeed <= (player->topSpeed * 0.7)) {
        player->soundEffects &= ~0x10000000;
    }
    player->soundEffects &= ~0x02000000;
    player->unk_044 &= ~0x0020;
    player->unk_098 = (player->currentSpeed * player->currentSpeed) / 25.0f;
}

void player_accelerate(Player* player) {
    UNUSED s32 player_index;

    player_index = get_player_index_for_player(player);
    if ((0.0 <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.1))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][0] * 3.2;
    }
    if (((player->topSpeed * 0.1) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.2))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][1] * 3.2;
    }
    if (((player->topSpeed * 0.2) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.3))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][2] * 3.2;
    }
    if (((player->topSpeed * 0.3) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.4))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][3] * 3.2;
    }
    if (((player->topSpeed * 0.4) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.5))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][4] * 3.2;
    }
    if (((player->topSpeed * 0.5) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.6))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][5] * 3.2;
    }
    if (((player->topSpeed * 0.6) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.7))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][6] * 3.2;
    }
    if (((player->topSpeed * 0.7) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.8))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][7] * 2.8;
    }
    if (((player->topSpeed * 0.8) <= player->currentSpeed) && (player->currentSpeed < (player->topSpeed * 0.9))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][8] * 2.8;
    }
    if (((player->topSpeed * 0.9) <= player->currentSpeed) && (player->currentSpeed <= (player->topSpeed * 1.0))) {
        player->currentSpeed += gKartAccelerationTables[player->characterId][9] * 2.8;
    }
    if (player->currentSpeed < 0.0f) {
        player->currentSpeed = 0.0f;
    }
    player->unk_098 = (player->currentSpeed * player->currentSpeed) / 25.0f;
}

void player_decelerate(Player* player, f32 speedReduction) {
    player->currentSpeed -= speedReduction;
    if (player->currentSpeed <= 0.0f) {
        player->currentSpeed = 0.0f;
    }
    if (player->topSpeed <= player->currentSpeed) {
        player->currentSpeed = player->topSpeed;
    }
    player->unk_098 = (player->currentSpeed * player->currentSpeed) / 25.0f;
}

void player_accelerate_global(Player* player, s32 playerIndex) {
    if ((gPlayerCurrentSpeed[playerIndex] >= 0.0) && (gPlayerCurrentSpeed[playerIndex] < ((f64) player->topSpeed * 0.1))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][0] * 3.2;
    }
    if (((player->topSpeed * 0.1) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.2))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][1] * 3.2;
    }
    if (((player->topSpeed * 0.2) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.3))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][2] * 3.2;
    }
    if (((player->topSpeed * 0.3) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.4))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][3] * 3.2;
    }
    if (((player->topSpeed * 0.4) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.5))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][4] * 3.2;
    }
    if (((player->topSpeed * 0.5) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.6))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][5] * 3.2;
    }
    if (((player->topSpeed * 0.6) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.7))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][6] * 3.2;
    }
    if (((player->topSpeed * 0.7) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.8))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][7] * 2.8;
    }
    if (((player->topSpeed * 0.8) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] < (player->topSpeed * 0.9))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][8] * 2.8;
    }
    if (((player->topSpeed * 0.9) <= gPlayerCurrentSpeed[playerIndex]) && (gPlayerCurrentSpeed[playerIndex] <= (player->topSpeed * 1.0))) {
        gPlayerCurrentSpeed[playerIndex] += gKartAccelerationTables[player->characterId][9] * 2.8;
    }
    player->unk_044 |= 0x20;
    if (gPlayerCurrentSpeed[playerIndex] < 0.0f) {
        gPlayerCurrentSpeed[playerIndex] = 0.0f;
    }
    player->unk_098 = (gPlayerCurrentSpeed[playerIndex] * gPlayerCurrentSpeed[playerIndex]) / 25.0f;
}

void player_decelerate_global(Player* player, f32 speedReduction, s32 playerIndex) {
    player->unk_044 &= ~0x20;
    gPlayerCurrentSpeed[playerIndex] -= speedReduction;
    if (gPlayerCurrentSpeed[playerIndex] <= 0.0f) {
        gPlayerCurrentSpeed[playerIndex] = 0.0f;
    }
    if (player->topSpeed <= gPlayerCurrentSpeed[playerIndex]) {
        gPlayerCurrentSpeed[playerIndex] = player->topSpeed;
    }
    player->unk_098 = (gPlayerCurrentSpeed[playerIndex] * gPlayerCurrentSpeed[playerIndex]) / 25.0f;
}

void func_80033850(Player* player, f32 arg1) {
    player->unk_090 += arg1;
    if (player->unk_090 >= 0.0f) {
        player->unk_090 = 0.0f;
    }
}

void func_80033884(Player* player, s32* arg1, s32* arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 temp_v1;

    if ((*arg1 >= arg4) || (-arg4 >= *arg1)) {
        temp_v1 = player->unk_200;
        player->unk_200 -= 0x800;
        if (player->unk_200 >= 0xF0000000) {
            player->unk_200 = temp_v1;
        }
        if (arg5 >= (s32) player->unk_200) {
            player->unk_200 = arg5;
        }

        *arg2 = (arg3 < *arg2) ? *arg2 - player->unk_200 : *arg2 + player->unk_200;

        if (player->unk_090 < ((f32) arg6)) {
            player->unk_090 = (f32) -arg6;
        }
    }
}

UNUSED void func_80033940(Player* player, s32* arg1, s32 arg2, s32 arg3, f32 arg4) {
    u32 temp_v1;

    temp_v1 = player->unk_200;
    player->unk_200 -= 0x800;
    if (!(player->unk_200 < 0xF0000000)) {
        player->unk_200 = temp_v1;
    }
    if (arg3 >= (s32) player->unk_200) {
        player->unk_200 = arg3;
    }

    *arg1 = (arg2 < *arg1) ? *arg1 - player->unk_200 : *arg1 + player->unk_200;

    if (player->unk_090 < arg4) {
        player->unk_090 = (f32) -arg4;
    }
}

void func_800339C4(Player* player, s32* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 temp_v0;

    temp_v0 = player->unk_200;
    player->unk_200 -= 0x800;
    if (player->unk_200 >= 0xF0000000) {
        player->unk_200 = temp_v0;
    }
    if (arg3 >= (s32) player->unk_200) {
        player->unk_200 = arg3;
    }

    *arg1 = (arg2 < *arg1) ? *arg1 - player->unk_200 : *arg1 + player->unk_200;

    func_80033850(player, arg4);
}

void func_80033A40(Player* player, s32* arg1, s32* arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6) {
    s32 temp_v1;

    if ((*arg1 >= arg4) || (-arg4 >= *arg1)) {
        temp_v1 = player->unk_200;
        player->unk_200 -= 0x800;
        if (player->unk_200 >= 0xF0000000) {
            player->unk_200 = temp_v1;
        }
        if (arg5 >= (s32) player->unk_200) {
            player->unk_200 = arg5;
        }

        *arg2 = (arg3 < *arg2) ? *arg2 - player->unk_200 : *arg2 + player->unk_200;

        func_80033850(player, arg6);
    }
}

void func_80033AE0(Player* player, struct Controller* controller, s8 arg2) {
    s32 sp2E4;
    s32 temp_v0_3;
    UNUSED s32 pad[2];
    UNUSED s16 pad2;
    s16 var_s1_2;
    s32 sp2D0;
    s32 sp2CC;
    s32 sp2C8;
    f32 var_f2_2;
    f32 var_f12 = 0.0f;
    f32 var_f2 = 0;
    UNUSED s32 pad3;
    s32 var_a0;
    f32 sp44[156] = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6,
                      0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8 };

    if (((((player->effects & 2) != 2) && ((((player->unk_0C0 / 182) <= 6) && ((player->unk_0C0 / 182) >= (-6))) ||
                                           ((controller->button & R_TRIG) != R_TRIG))) ||
         (((player->speed / 18.0f) * 216.0f) <= 20.0f)) ||
        ((player->effects & 0x8000) == 0x8000)) {
        func_80036CB4(player);
    }
    if ((player->unk_0C0 / 182) < (-5)) {
        player->unk_044 |= 4;
        player->unk_044 &= 0xFFFD;
        D_801652C0[arg2]++;
    } else if ((player->unk_0C0 / 182) >= 6) {
        player->unk_044 |= 2;
        player->unk_044 &= 0xFFFB;
        D_801652C0[arg2]++;
    } else {
        player->unk_044 &= 0xFFF9;
        D_801652C0[arg2] = 0;
    }
    if (((player->effects & 2) == 2) || ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT)) {
        player->unk_044 &= 0xFFF9;
    }
    sp2E4 = player->unk_07C;
    temp_v0_3 = func_80038534(controller);
    if (((player->unk_044 & 1) == 1) || ((player->unk_044 & 8) == 8)) {
        temp_v0_3 = -temp_v0_3;
    }
    player->unk_07C = (temp_v0_3 << 16) & 0xFFFF0000;
    sp2D0 = sp2E4 - player->unk_07C;
    sp2D0 = sp2D0 >> 16;
    player->unk_0FA = (s16) sp2D0;
    if (((sp2D0 >= 0x5A) || (sp2D0 < (-0x59))) && (!(player->unk_044 & 0x4000))) {
        if ((((((!(player->effects & DRIFTING_EFFECT)) && (gCCSelection == CC_150)) && (gModeSelection != BATTLE)) &&
              (!(player->effects & 8))) &&
             (((player->speed / 18.0f) * 216.0f) >= 40.0f)) &&
            (player->driftDuration == 0)) {
            player->soundEffects |= 0x80;
        }
    }
    if (((s32) player->tyres[BACK_RIGHT].surfaceType) < 0xF) {
        var_f2 += D_800E3610[player->characterId][player->tyres[BACK_RIGHT].surfaceType];
    }
    if (((s32) player->tyres[BACK_LEFT].surfaceType) < 0xF) {
        var_f2 += D_800E3610[player->characterId][player->tyres[BACK_LEFT].surfaceType];
    }
    if ((player->effects & 0x20) == 0x20) {
        sp2C8 = 10;
        sp2CC = 10;
    } else {
        if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
            var_a0 = 3;
        } else {
            var_a0 = 0;
        }
        if (((player->speed / 18.0f) * 216.0f) >= 15.0f) {
            if ((player->unk_044 & 2) == 2) {
                if ((sp2D0 < 36) && (sp2D0 >= 0)) {
                    sp2C8 =
                        (gKartTable800E3650[player->characterId] + 1.0f) * (((f32) (var_a0 + 0xF)) * (1.0f + var_f2));
                    sp2CC =
                        (gKartTable800E3650[player->characterId] + 1.0f) * (((f32) (var_a0 + 0xF)) * (1.0f + var_f2));
                } else {
                    sp2C8 = (s32) (((f32) (var_a0 + 5)) * (1.0f + var_f2));
                    sp2CC = (s32) (((f32) (var_a0 + 9)) * (1.0f + var_f2));
                }
            } else if ((player->unk_044 & 4) == 4) {
                if ((sp2D0 >= (-0x23)) && (sp2D0 <= 0)) {
                    sp2C8 =
                        (gKartTable800E3650[player->characterId] + 1.0f) * (((f32) (var_a0 + 0xF)) * (1.0f + var_f2));
                    sp2CC =
                        (gKartTable800E3650[player->characterId] + 1.0f) * (((f32) (var_a0 + 0xF)) * (1.0f + var_f2));
                } else {
                    sp2C8 = (s32) (((f32) (var_a0 + 5)) * (1.0f + var_f2));
                    sp2CC = (s32) (((f32) (var_a0 + 9)) * (1.0f + var_f2));
                }
            } else {
                sp2C8 = (s32) (((f32) (var_a0 + 3)) * (1.0f + var_f2));
                sp2CC = (s32) (((f32) (var_a0 + 6)) * (1.0f + var_f2));
            }
        } else {
            sp2C8 = 8;
            sp2CC = 8;
        }
    }
    if ((player->waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) == WATER_IS_FULLY_SUBMERGED) {
        sp2C8 *= 1.5;
        sp2CC *= 1.5;
    } else {
        if ((player->waterInteractionFlags & WATER_IS_PARTIALLY_SUBMERGED) == WATER_IS_PARTIALLY_SUBMERGED) {
            sp2C8 *= 1.2;
            sp2CC *= 1.2;
        }
        if ((((f64) (gPlayerWaterLevel[arg2] - player->tyres[BACK_LEFT].baseHeight)) >= 3.5) ||
            (((f64) (gPlayerWaterLevel[arg2] - player->tyres[BACK_RIGHT].baseHeight)) >= 3.5)) {
            sp2C8 *= 1.05;
            sp2CC *= 1.05;
        }
    }
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000005A, 0x78000 / sp2C8, 0x000001C2);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000059, 0x76000 / sp2C8, 0x000001B8);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000058, 0x74000 / sp2C8, 0x000001AE);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000057, 0x72000 / sp2C8, 0x000001A4);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000056, 0x70000 / sp2C8, 0x0000019A);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000055, 0x58000 / sp2C8, 0x00000190);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000054, 0x56000 / sp2C8, 0x0000018B);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000053, 0x50000 / sp2C8, 0x00000186);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000052, 0x4F000 / sp2C8, 0x00000186);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000051, 0x4E000 / sp2C8, 0x0000017C);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000050, 0x4D000 / sp2C8, 0x00000172);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004F, 0x4C000 / sp2C8, 0x00000168);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004E, 0x4C000 / sp2C8, 0x00000168);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004D, 0x4B000 / sp2C8, 0x0000015E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004C, 0x4A000 / sp2C8, 0x00000154);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004B, 0x49000 / sp2C8, 0x0000014A);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000004A, 0x49000 / sp2C8, 0x0000014A);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000049, 0x49000 / sp2C8, 0x0000014A);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000048, 0x48000 / sp2C8, 0x00000140);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000047, 0x47000 / sp2C8, 0x0000013B);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000046, 0x47000 / sp2C8, 0x0000013B);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000045, 0x46000 / sp2C8, 0x00000131);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000044, 0x46000 / sp2C8, 0x00000131);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000043, 0x45000 / sp2C8, 0x00000118);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000042, 0x46000 / sp2C8, 0x0000010E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000041, 0x45000 / sp2C8, 0x0000010E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000040, 0x44000 / sp2C8, 0x00000104);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003F, 0x43000 / sp2C8, 0x000000FA);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003E, 0x43000 / sp2C8, 0x000000FA);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003D, 0x43000 / sp2C8, 0x000000FA);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003C, 0x3D000 / sp2C8, 0x000000F5);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003B, 0x3C000 / sp2C8, 0x000000F5);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000003A, 0x3B000 / sp2C8, 0x000000F5);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000039, 0x3A000 / sp2C8, 0x000000F5);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000038, 0x38000 / sp2C8, 0x000000F5);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000037, 0x38000 / sp2C8, 0x000000E6);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000036, 0x38000 / sp2C8, 0x000000E6);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000035, 0x38000 / sp2C8, 0x000000E6);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000034, 0x38000 / sp2C8, 0x000000E6);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000033, 0x38000 / sp2C8, 0x000000E6);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000032, 0x32000 / sp2C8, 0x000000DC);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000031, 0x32000 / sp2C8, 0x000000DC);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000030, 0x32000 / sp2C8, 0x000000DC);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002F, 0x32000 / sp2C8, 0x000000DC);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002E, 0x32000 / sp2C8, 0x000000DC);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002D, 0x30000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002C, 0x2E000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002B, 0x2E000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000002A, 0x2E000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000029, 0x2E000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000028, 0x2E000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000027, 0x2C000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000026, 0x28000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000025, 0x28000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000024, 0x24000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000023, 0x24000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000022, 0x22000 / sp2C8, 0x0000006E);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000020, 0x20000 / sp2C8, 0x00000064);
    func_80033884(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001F, 0x20000 / sp2C8, 0x00000064);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001E, 0x1F000 / sp2CC, 0.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001D, 0x1E000 / sp2CC, 0.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001C, 0x1D000 / sp2CC, 0.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001B, 0x1C000 / sp2CC, 0.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000001A, 0x1B000 / sp2CC, 0.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000019, 0x1A000 / sp2CC, 1.0f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000018, 0x19000 / sp2CC, 1.0f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000017, 0x18000 / sp2CC, 1.0f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000016, 0x17000 / sp2CC, 1.0f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000015, 0x16000 / sp2CC, 1.0f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000014, 0x15000 / sp2CC, 1.05f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000013, 0x14000 / sp2CC, 1.05f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000012, 0x13000 / sp2CC, 1.05f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000011, 0x12000 / sp2CC, 1.05f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x00000010, 0x11000 / sp2CC, 1.05f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000F, 0x10000 / sp2CC, 1.2f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000E, 0xF000 / sp2CC, 1.2f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000D, 0xE000 / sp2CC, 1.2f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000C, 0xD000 / sp2CC, 1.2f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000B, 0xC000 / sp2CC, 1.2f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0x0000000A, 0xE000 / sp2CC, 1.6f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 9, 0xD000 / sp2CC, 1.6f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 8, 0xC000 / sp2CC, 1.6f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 7, 0xB000 / sp2CC, 1.6f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 6, 0xA000 / sp2CC, 1.6f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 5, 0x9000 / sp2CC, 1.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 4, 0x8000 / sp2CC, 1.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 3, 0x7000 / sp2CC, 1.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 2, 0x6000 / sp2CC, 1.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 1, 0x5000 / sp2CC, 1.9f);
    func_80033A40(player, &sp2D0, &sp2E4, player->unk_07C, 0, 0 / sp2CC, 1.9f);
    if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        var_f2_2 = (f32) (((s32) (sp2E4 >> 16)) / 8);
    } else if (((player->speed / 18.0f) * 216.0f) <= 25.0f) {
        var_f2_2 = (f32) ((sp2E4 >> 16) / 12);
    } else {
        var_f2_2 = ((f32) (sp2E4 >> 0x10)) / (8.0f + (player->currentSpeed / 50.0f));
    }
    if (var_f2_2 < 0.0f) {
        var_f2_2 = -var_f2_2;
    }
    if ((player->effects & 0x20) == 0x20) {
        var_f2_2 = var_f2_2 * (sp44[((s16) ((player->speed / 18.0f) * 216.0f)) + 10] * 1.5f);
    } else if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        var_f2_2 = var_f2_2 * sp44[(s16) ((player->speed / 18.0f) * 216.0f)];
    } else {
        var_f2_2 = var_f2_2 * (sp44[(s16) ((player->speed / 18.0f) * 216.0f)] * 1.5f);
    }
    player->unk_07C = sp2E4;
    if (player->unk_10C != 0) {
        func_8002BD58(player);
    }
    player->effects &= 0xDFFFFFFF;
    if (((s32) player->tyres[BACK_RIGHT].surfaceType) > 0xE) {
        var_f12 = var_f12;
    } else {
        var_f12 += D_800E3410[player->characterId][player->tyres[BACK_RIGHT].surfaceType];
    }
    if (((s32) player->tyres[BACK_LEFT].surfaceType) < 0xF) {
        var_f12 += D_800E3410[player->characterId][player->tyres[BACK_LEFT].surfaceType];
    }
    if (((player->effects & 2) != 2) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT)) {
        if ((player->effects & 0x20) == 0x20) {
            player->unk_078 = (s16) ((s32) (((f32) ((((s32) player->unk_07C) >> 0x10) * 5)) * var_f2_2));
        } else {
            if ((player->effects & 0x1) != 0x1) {
                if (((player->unk_07C >> 16) >= 45) || ((player->unk_07C >> 16) <= (-45))) {
                    player->unk_078 = ((player->unk_07C >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      (0.15 + gKartHandlingTable[player->characterId]);
                } else {
                    player->unk_078 = ((player->unk_07C >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      gKartHandlingTable[player->characterId];
                }
            } else {
                if ((((player->speed / 18.0f) * 216.0f) >= 0.0f) && (((player->speed / 18.0f) * 216.0f) < 8.0f)) {
                    player->unk_078 = (player->unk_07C >> 16) * (var_f2_2 + (var_f2_2 * var_f12));
                }
                if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (((player->speed / 18.0f) * 216.0f) < 65.0f)) {
                    player->unk_078 = (player->unk_07C >> 16) * ((var_f2_2 + 1.5) + (var_f2_2 * var_f12));
                }
                if (((player->speed / 18.0f) * 216.0f) >= 65.0f) {
                    player->unk_078 = (player->unk_07C >> 16) * ((var_f2_2 + 1.6) + (var_f2_2 * var_f12));
                }
            }
            player->unk_228 = 0;
            if (player->driftState < 2) {
                player->driftState = 0;
            }
        }
    } else if (((player->effects & 8) != 8) && ((player->effects & 2) != 2)) {
        if ((((s16) player->unk_0C0) / 182) > 0) {
            var_s1_2 = (((s32) (((player->unk_07C >> 0x10) * 0xD) + 0x2B1)) / 106) + 0x28;
            if ((player->unk_07C >> 0x10) < (-0x27)) {
                player->effects = player->effects | 0x20000000;
                if ((player->unk_07C >> 0x10) < (-0x31)) {
                    player->effects |= 0x20000000;
                }
            }
            func_8002A8A4(player, arg2);
        } else {
            var_s1_2 = (((s32) (((player->unk_07C >> 0x10) * 0xD) + 0x2B1)) / 106) - 0x35;
            if ((player->unk_07C >> 0x10) >= 0x28) {
                player->effects = player->effects | 0x20000000;
                if ((player->unk_07C >> 0x10) < (-0x31)) {
                    player->effects |= 0x20000000;
                }
            }
            func_8002A8A4(player, arg2);
        }
        if ((((player->speed / 18.0f) * 216.0f) >= 0.0f) && (((player->speed / 18.0f) * 216.0f) < 8.0f)) {
            player->unk_078 = (s16) ((s32) (var_s1_2 * ((var_f2_2 + 2.0f) + (var_f2_2 * var_f12))));
        }
        if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (((player->speed / 18.0f) * 216.0f) < 65.0f)) {
            player->unk_078 = var_s1_2 * ((var_f2_2 + 3) + (var_f2_2 * var_f12));
        }
        if (((player->speed / 18.0f) * 216.0f) >= 65.0f) {
            player->unk_078 = var_s1_2 * ((((f64) var_f2_2) + 3.5) + (var_f2_2 * var_f12));
        }
        if ((player->effects & 0x20000000) == 0x20000000) {
            player->unk_078 *= 0.9;
        } else {
            player->unk_078 *= 0.65;
        }
    } else {
        var_s1_2 = (s16) (((s32) player->unk_07C) >> 16);
        if (temp_v0_3 == 0) {
            var_s1_2 = 0;
        }
        if (((player->speed / 18.0f) * 216.0f) <= 5.0f) {
            player->unk_078 = (s16) ((s32) (((f32) var_s1_2) * (var_f2_2 + 6.0f)));
        } else {
            player->unk_078 = ((s16) var_s1_2) * (var_f2_2 + 1.5f);
        }
    }
    if (gModeSelection == BATTLE) {
        player->unk_078 *= 1.7;
    }
}

void apply_cpu_turn(Player* player, s16 arg1) {
    s32 sp304 = 0;
    UNUSED f32 pad[6];
    f32 var_f0;
    s16 var_v0;
    f32 sp44[168] = {
        0.0f, 0.1f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f,
        0.7f, 0.7f, 0.6f, 0.5f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
        0.4f, 0.4f, 0.5f, 0.5f, 0.5f, 0.5f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f,
        0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
    };
    f32 sp24[8] = {
        3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f,
    };

    if (!((player->effects & 0x80) || (player->effects & 0x40) || (player->effects & 0x400) ||
          (player->effects & 0x10000) || (player->effects & 0x20000) || (player->effects & 0x01000000) ||
          (player->effects & HIT_BY_ITEM_EFFECT) || (player->effects & HIT_EFFECT))) {
        if (!(((player->speed / 18.0f) * 216.0f) >= 110.0f)) {
            player->effects &= ~0x20000000;
            player->unk_228 = 0;
            if (!(player->effects & 0x80) && !(player->effects & 0x40)) {
                sp304 = (s32) player->unk_07C >> 0x10;
                move_s32_towards(&sp304, (s32) arg1, 0.35f);
                sp304 <<= 0x10;
                if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
                    var_f0 = (sp304 >> 0x10) / 5;
                } else {
                    var_f0 = (f32) (sp304 >> 0x10) / (8.0f + (player->currentSpeed / 50.0f));
                }
                if (var_f0 < 0.0f) {
                    var_f0 = -var_f0;
                }
                if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
                    var_f0 = sp44[(s16) ((player->speed / 18.0f) * 216.0f)] * var_f0;
                } else {
                    var_f0 = sp44[(s16) ((player->speed / 18.0f) * 216.0f)] * sp24[player->characterId] * var_f0;
                }
                player->unk_07C = sp304;
                if (((player->effects & 2) != 2) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT)) {
                    if ((player->effects & 1) != 1) {
                        player->unk_078 = (player->unk_07C >> 0x10) * var_f0;
                    } else {
                        player->unk_078 = (player->unk_07C >> 0x10) * (var_f0 + 1.5);
                    }
                } else if ((player->effects & 8) != 8) {
                    if (((s16) player->unk_0C0 / 182) > 0) {
                        var_v0 = player->unk_07C >> 0x10;
                    } else {
                        var_v0 = player->unk_07C >> 0x10;
                    }
                    player->unk_078 = var_v0 * (var_f0 + 3.0);
                    player->unk_078 *= 0.8;
                } else {
                    var_v0 = (s16) ((s32) player->unk_07C >> 0x10);
                    if (arg1 == 0) {
                        var_v0 = 0;
                    }
                    player->unk_078 = var_v0 * var_f0;
                }
                if ((((player->effects & 2) != 2) && (player->unk_0C0 < 0x3D) && (player->unk_0C0 > -0x3D)) ||
                    (((player->speed / 18.0f) * 216.0f) <= 20.0f) || ((player->effects & 0x8000) == 0x8000)) {
                    func_80036CB4(player);
                }
            }
        }
    }
}

void func_80036C5C(Player* player) {
    if (((player->speed / 18.0f) * 216.0f) > 20.0f) {
        player->driftDuration = 0;
        player->effects |= DRIFTING_EFFECT;
        player->unk_0B6 |= 0x800;
    }
}

void func_80036CB4(Player* player) {
    s32 test;

    if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN)) {
        if ((player->unk_0C0 / 182) > 0) {
            test = ((((player->unk_07C >> 0x10) * 0xD) + 0x2B1) / 106) + 0x28;
            player->unk_07C = test << 0x10;
        }
        if ((player->unk_0C0 / 182) < 0) {
            test = ((((player->unk_07C >> 0x10) * 0xD) + 0x2B1) / 106) - 0x35;
            player->unk_07C = test << 0x10;
        }
        player->effects &= ~DRIFTING_EFFECT;
    }
    if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->type & PLAYER_HUMAN) != PLAYER_HUMAN)) {
        player->effects &= ~DRIFTING_EFFECT;
    }
}

void func_80036DB4(Player* player, Vec3f arg1, Vec3f arg2) {
    s16 thing;
    UNUSED s16 pad;
    f32 sp20;
    f32 var_f18;
    s32 temp_t6;

    if (((player->effects & 0x1000) == 0x1000) || ((player->effects & 0x20) == 0x20)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
            var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->unk_20C * 10.0f);
            sp20 = player->unk_084 * 3.0f;
        } else if (!(player->effects & 0x800) && !(player->unk_044 & 0x4000)) {
            thing = player->unk_0FA;
            if (thing > 0) {
                thing *= -1;
            }
            temp_t6 = player->unk_07C >> 0x10;
            if ((temp_t6 < 0x15) && (temp_t6 >= -0x14)) {
                if (thing < 0x14) {
                    var_f18 = (player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-player->currentSpeed * 0.02) + (-player->unk_20C * 50.0f);
                } else {
                    var_f18 = (player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              ((temp_t6 * 0.01) + (-player->currentSpeed * 0.05)) + (-player->unk_20C * 50.0f);
                }
            } else {
                var_f18 = (player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                          ((temp_t6 * 0.1) + (-player->currentSpeed * 0.15)) + (-player->unk_20C * 50.0f);
            }
            sp20 = player->unk_084;
        } else {
            var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) * 1.5) +
                      (((player->unk_07C >> 0x10) * 0.1) + (-player->currentSpeed * 0.05)) + (-player->unk_20C * 50.0f);
            sp20 = player->unk_084;
        }
        if ((player->effects & STAR_EFFECT) == STAR_EFFECT) {
            if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
                var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->unk_20C * 10.0f);
                sp20 = player->unk_084 * 3.0f;
            } else {
                var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = player->unk_084;
            }
        }
        arg1[0] = (player->unk_090 + var_f18) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * sp20;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_800371F4(Player* player, Vec3f arg1, Vec3f arg2) {
    s16 var_v0;
    f32 sp20;
    f32 var_f18;
    s32 temp_t6;

    if (((player->effects & 0x1000) == 0x1000) || ((player->effects & 0x20) == 0x20)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
            var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->unk_20C * 50.0f);
            sp20 = player->unk_084 * 3.0f;
        } else if (!(player->effects & 0x800) && !(player->unk_044 & 0x4000)) {
            var_v0 = player->unk_0FA;
            if (var_v0 > 0) {
                var_v0 *= -1;
            }
            temp_t6 = (s32) player->unk_07C >> 0x10;
            if ((temp_t6 < 0x15) && (temp_t6 >= -0x14)) {
                if (var_v0 < 0x14) {
                    var_f18 = (player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-player->currentSpeed * 0.02) + (-player->unk_20C * 50.0f);
                } else {
                    var_f18 = ((player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) -
                               ((temp_t6 * 0.01) + (player->currentSpeed * 0.05))) +
                              (-player->unk_20C * 50.0f);
                }
            } else {
                var_f18 = ((player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) -
                           ((temp_t6 * 0.1) + (player->currentSpeed * 0.15))) +
                          (-player->unk_20C * 50.0f);
            }
            sp20 = player->unk_084;
        } else {
            var_f18 = ((player->unk_208 + ((f64) (-(player->speed / 18.0f) * 216.0f) * 1.5)) -
                       (((player->unk_07C >> 0x10) * 0.1) + (player->currentSpeed * 0.05))) +
                      (-player->unk_20C * 50.0f);
            sp20 = player->unk_084;
        }
        if ((player->effects & STAR_EFFECT) == STAR_EFFECT) {
            if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
                var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->unk_20C * 50.0f);
                sp20 = player->unk_084 * 3.0f;
            } else {
                var_f18 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = player->unk_084;
            }
        }
        arg1[0] = -(player->unk_090 + var_f18) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * sp20;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_80037614(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((player->effects & 0x1000) == 0x1000) || ((player->effects & 0x20) == 0x20)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = (var_f2 + 28.0f) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * player->speed;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_8003777C(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((player->effects & 0x1000) == 0x1000) || ((player->effects & 0x20) == 0x20)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if (((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) && ((player->effects & 2) != 2)) {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = -(var_f2 + 28.0f) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * player->speed;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_800378E8(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((player->effects & 0x20) == 0x20) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
            var_f2 = player->unk_208 + (-(player->speed / 18.0f) * 216.0f * 5.0f) + (-player->unk_20C * 10.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 40.0f) + (-player->unk_20C * 50.0f);
            var_f12 = player->unk_084;
        }
        arg1[0] = (player->unk_090 + var_f2) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * var_f12;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_80037A4C(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((player->effects & 0x20) == 0x20) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    } else {
        if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
            var_f2 = player->unk_208 + (-(player->speed / 18.0f) * 216.0f * 5.0f) + (-player->unk_20C * 50.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = player->unk_208 + ((-(player->speed / 18.0f) * 216.0f) / 40.0f) + (-player->unk_20C * 50.0f);
            var_f12 = player->unk_084;
        }
        arg1[0] = -(player->unk_090 + var_f2) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * var_f12;
        mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void func_80037BB4(Player* player, Vec3f arg1) {
    UNUSED s32 pad[3];
    Vec3f sp20;

    if (player->unk_078 == 0) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
    } else {
        if (player->unk_078 < 0) {
            if (((player->effects & 0x20000000) != 0x20000000) || (player->unk_228 >= 0x64)) {
                player->rotation[1] += player->unk_078;
            }
            if (!(player->type & PLAYER_CPU)) {
                if (gModeSelection == BATTLE) {
                    func_800378E8(player, sp20, arg1);
                } else {
                    func_80036DB4(player, sp20, arg1);
                }
            } else {
                func_80037614(player, sp20, arg1);
            }
        } else {
            if (((player->effects & 0x20000000) != 0x20000000) || (player->unk_228 >= 0x64)) {
                player->rotation[1] += player->unk_078;
            }
            if (!(player->type & PLAYER_CPU)) {
                if (gModeSelection == BATTLE) {
                    func_80037A4C(player, sp20, arg1);
                } else {
                    func_800371F4(player, sp20, arg1);
                }
            } else {
                func_8003777C(player, sp20, arg1);
            }
        }
    }
}

void func_80037CFC(Player* player, struct Controller* controller, s8 arg2) {

    if (CVarGetInteger("gEnableMoonJump", 0)) {
        moon_jump(player, controller);
    }

    if (((player->effects & 0x80) != 0x80) && ((player->effects & 0x40) != 0x40) &&
        ((player->effects & 0x400) != 0x400) && ((player->effects & 0x4000) != 0x4000) &&
        ((player->effects & 0x01000000) != 0x01000000) &&
        ((player->effects & HIT_BY_ITEM_EFFECT) != HIT_BY_ITEM_EFFECT) && ((player->effects & 0x10000) != 0x10000) &&
        ((player->effects & 0x20000) != 0x20000)) {
        if (((player->effects & HIT_EFFECT) != HIT_EFFECT) && ((player->effects & 8) != 8) &&
            ((player->effects & 2) != 2) && ((player->effects & DRIFTING_EFFECT) != DRIFTING_EFFECT) &&
            (controller->buttonPressed & R_TRIG)) {
            kart_hop(player);
            if (((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
                ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB)) {
                func_800C9060(arg2, 0x19008000);
            }
        }
        if ((player->effects & 8) != 8) {
            func_80033AE0(player, controller, arg2);
        } else if (((player->effects & 2) == 2) && (player->collision.surfaceDistance[2] <= 5.0f)) {
            func_80033AE0(player, controller, arg2);
        }
        player->effects &= ~1;
        if ((!(player->effects & BOOST_RAMP_ASPHALT_EFFECT)) && (!(player->effects & BOOST_RAMP_WOOD_EFFECT))) {
            if (((player->speed / 18.0f) * 216.0f) <= 12.0f) {
                if (controller->button & A_BUTTON) {
                    if (controller->button & B_BUTTON) {
                        player->effects |= 0x20;
                        if ((player->effects & 0x20) != 0x20) {
                            player->currentSpeed += 100.0f;
                        }
                    }
                }
            }
            if (((player->effects & 0x20) == 0x20) &&
                (((controller->button & B_BUTTON) == 0) || (!(controller->button & A_BUTTON)))) {
                player->effects &= ~0x20;
            }
        }
        if ((player->unk_044 & 1) != 1) {
            if (controller->button & A_BUTTON) {
                player_accelerate_alternative(player);
                detect_triple_a_combo_a_pressed(player);
            } else {
                if (gModeSelection == BATTLE) {
                    player_decelerate_alternative(player, 2.0f);
                } else {
                    player_decelerate_alternative(player, 1.0f);
                }
                detect_triple_a_combo_a_released(player);
            }
            if (controller->button & B_BUTTON) {
                func_800323E4(player);
                detect_triple_b_combo_b_pressed(player);
            } else {
                player->unk_20C = 0.0f;
                detect_triple_b_combo_b_released(player);
            }
        }
        if ((!(player->effects & BOOST_RAMP_ASPHALT_EFFECT)) && (!(player->effects & 4))) {
            if (((func_800388B0(controller) < (-0x31)) && (((player->speed / 18.0f) * 216.0f) <= 5.0f)) &&
                (controller->button & B_BUTTON)) {
                player->currentSpeed = 140.0f;
                player->unk_044 |= 1;
                player->kartPropulsionStrength = (player->currentSpeed * player->currentSpeed) / 25.0f;
                player->unk_20C = 0.0f;
            }
            if ((func_800388B0(controller) >= -0x1D) || (!(controller->button & B_BUTTON))) {
                if ((player->unk_044 & 1) == 1) {
                    player->unk_044 &= 0xFFFE;
                    player->currentSpeed = 0.0f;
                }
            }
        }
    } else {
        if ((player->effects & 0x4000) == 0x4000) {
            if (controller->button & A_BUTTON) {
                player_accelerate_alternative(player);
            } else {
                player_decelerate_alternative(player, 5.0f);
            }
        }
        if (((((player->effects & 0x80) == 0x80) || ((player->effects & 0x40) == 0x40)) ||
             ((player->effects & 0x01000000) == 0x01000000)) ||
            ((player->effects & HIT_BY_ITEM_EFFECT) == HIT_BY_ITEM_EFFECT)) {
            if (controller->button & A_BUTTON) {
                detect_triple_a_combo_a_pressed(player);
                player_accelerate_global(player, arg2);
                return;
            }
            detect_triple_a_combo_a_released(player);
            player_decelerate_global(player, 5.0f, arg2);
        }
    }
}

void handle_a_press_for_player_during_race(Player* player, struct Controller* controller, s8 arg2) {
    if (((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) && ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) &&
        ((player->type & PLAYER_CPU) != PLAYER_CPU)) {
        if ((player->type & PLAYER_START_SEQUENCE) != PLAYER_START_SEQUENCE) {
            if (((player->unk_0CA & 2) == 2) || ((player->unk_0CA & 8) == 8)) {
                if (controller->button & A_BUTTON) {
                    player_accelerate(player);
                } else {
                    player_decelerate(player, 5.0f);
                }
            } else {
                func_80037CFC(player, controller, arg2);
            }
            D_80164A89 = 1;
        } else if (D_8018D168 == 1) {
            if (D_801656F0 == 1) {
                if (D_801652E0[arg2] == 0) {
                    D_801652E0[arg2] = gRaceFrameCounter;
                }
            }
            if (controller->button & A_BUTTON) {
                player_accelerate_during_start_sequence(player);
            } else {
                player_decelerate_during_start_sequence(player, 5.0f);
            }
        }
    }
}

void handle_a_press_for_all_players_during_race(void) {
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_5;
    u16 temp_v0_6;

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            switch (gModeSelection) {
                case GRAND_PRIX:
                    handle_a_press_for_player_during_race(gPlayerOne, gControllerOne, 0);
                    return;
                case TIME_TRIALS:
                    if (D_8015F890 != 1) {
                        handle_a_press_for_player_during_race(gPlayerOne, gControllerOne, 0);
                        temp_v0_3 = gPlayerTwo->type;
                        if (((temp_v0_3 & 0x100) == 0x100) && ((temp_v0_3 & 0x800) != 0x800)) {
                            handle_a_press_for_player_during_race(gPlayerTwo, gControllerSix, 1);
                        }
                        temp_v0_4 = gPlayerThree->type;
                        if (((temp_v0_4 & 0x100) == 0x100) && ((temp_v0_4 & 0x800) != 0x800)) {
                            handle_a_press_for_player_during_race(gPlayerThree, gControllerSeven, 2);
                            return;
                        }
                    } else {
                        if ((gPlayerOne->type & 0x800) != 0x800) {
                            handle_a_press_for_player_during_race(gPlayerOne, gControllerEight, 0);
                        }
                        temp_v0_5 = gPlayerTwo->type;
                        if (((temp_v0_5 & 0x100) == 0x100) && ((temp_v0_5 & 0x800) != 0x800)) {
                            handle_a_press_for_player_during_race(gPlayerTwo, gControllerSix, 1);
                        }
                        temp_v0_6 = gPlayerThree->type;
                        if (((temp_v0_6 & 0x100) == 0x100) && ((temp_v0_6 & 0x800) != 0x800)) {
                            handle_a_press_for_player_during_race(gPlayerThree, gControllerSeven, 2);
                            return;
                        }
                        return;
                    }

                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            handle_a_press_for_player_during_race(gPlayerOne, gControllerOne, 0);
            handle_a_press_for_player_during_race(gPlayerTwo, gControllerTwo, 1);
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            handle_a_press_for_player_during_race(gPlayerOne, gControllerOne, 0);
            handle_a_press_for_player_during_race(gPlayerTwo, gControllerTwo, 1);
            handle_a_press_for_player_during_race(gPlayerThree, gControllerThree, 2);
            if (gPlayerCountSelection1 == 4) {
                handle_a_press_for_player_during_race(gPlayerFour, gControllerFour, 3);
            }
            break;
    }
}

s16 func_80038534(struct Controller* controller) {
    s16 temp_a2;
    s16 var_a3;
    s16 var_t0;
    s16 temp_v0;

    temp_v0 = controller->rawStickX;
    temp_a2 = controller->rawStickY;
    var_a3 = temp_v0;
    var_t0 = temp_a2;

    if (temp_v0 > 0xC) {
        var_t0 = (temp_a2 * 0x000C) / temp_v0;
        var_a3 = 0x000C;
    }
    if (var_a3 < -0xC) {
        var_t0 = (var_t0 * 0x000C) / -var_a3;
        var_a3 = -0x000C;
    }
    if (var_t0 > 0xC) {
        var_a3 = (var_a3 * 0x000C) / var_t0;
        var_t0 = 0x000C;
    }
    if (var_t0 < -0xC) {
        var_a3 = (var_a3 * 0x000C) / -var_t0;
        var_t0 = -0x000C;
    }
    if ((((controller->rawStickX > -0xD) && (controller->rawStickX < 0xD)) && (controller->rawStickY > -0xD)) &&
        (controller->rawStickY < 0xD)) {
        temp_v0 = 0;
        temp_a2 = 0;
    } else {
        temp_v0 -= var_a3;
        temp_a2 -= var_t0;
    }
    if (temp_v0 > 0x35) {
        temp_a2 = (temp_a2 * 0x0035) / temp_v0;
        temp_v0 = 0x0035;
    }
    if (temp_v0 < -0x35) {
        temp_a2 = (temp_a2 * 0x0035) / -temp_v0;
        temp_v0 = -0x0035;
    }
    if (temp_a2 > 0x35) {
        temp_v0 = (temp_v0 * 0x0035) / temp_a2;
        temp_a2 = 0x0035;
    }
    if (temp_a2 < -0x35) {
        temp_v0 = (temp_v0 * 0x0035) / -temp_a2;
    }
    return temp_v0;
}

s16 func_800388B0(struct Controller* controller) {
    s16 temp_a2;
    s16 var_a3;
    s16 var_t0;
    s16 temp_v0;

    temp_v0 = controller->rawStickX;
    temp_a2 = controller->rawStickY;
    var_a3 = temp_v0;
    var_t0 = temp_a2;

    if (temp_v0 > 0xC) {
        var_t0 = (temp_a2 * 0x000C) / temp_v0;
        var_a3 = 0x000C;
    }
    if (var_a3 < -0xC) {
        var_t0 = (var_t0 * 0x000C) / -var_a3;
        var_a3 = -0x000C;
    }
    if (var_t0 > 0xC) {
        var_a3 = (var_a3 * 0x000C) / var_t0;
        var_t0 = 0x000C;
    }
    if (var_t0 < -0xC) {
        var_a3 = (var_a3 * 0x000C) / -var_t0;
        var_t0 = -0x000C;
    }
    if ((((controller->rawStickX > -0xD) && (controller->rawStickX < 0xD)) && (controller->rawStickY > -0xD)) &&
        (controller->rawStickY < 0xD)) {
        temp_v0 = 0;
        temp_a2 = 0;
    } else {
        temp_v0 -= var_a3;
        temp_a2 -= var_t0;
    }
    if (temp_v0 > 0x35) {
        temp_a2 = (temp_a2 * 0x0035) / temp_v0;
        temp_v0 = 0x0035;
    }
    if (temp_v0 < -0x35) {
        temp_a2 = (temp_a2 * 0x0035) / -temp_v0;
        temp_v0 = -0x0035;
    }
    if (temp_a2 > 0x35) {
        temp_v0 = (temp_v0 * 0x0035) / temp_a2;
        temp_a2 = 0x0035;
    }
    if (temp_a2 < -0x35) {
        temp_v0 = (temp_v0 * 0x0035) / -temp_a2;
        temp_a2 = -0x0035;
    }
    return temp_a2;
}

void func_80038BE4(Player* player, s16 arg1) {
    player->currentSpeed += (f32) arg1;
    if (player->currentSpeed < 0.0f) {
        player->currentSpeed = 0.0f;
    }
    if (player->currentSpeed >= 250.0f) {
        player->currentSpeed = 250.0f;
    }
    player->unk_044 |= 0x20;
    player->kartPropulsionStrength = (player->currentSpeed * player->currentSpeed) / 25.0f;
}

void func_80038C6C(Player* player, UNUSED Camera* camera, s8 arg2, s8 playerId) {
    Vec3f sp114 = { 0.0, 0.0, 1.0 };
    Vec3f sp108 = { 0.0, 0.0, 0.0 };
    Vec3f spFC = { 0.0, 0.0, 0.0 };
    Vec3f spF0 = { 0.0, 0.0, 0.0 };
    f32 spEC;
    f32 spE8;
    f32 spE4;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 sqrt;

    f32 divOptimize;
    UNUSED s32 pad2[10];
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    UNUSED s32 pad3;
    Vec3f sp88;
    Vec3f sp7C;
    UNUSED s32 pad[10];

    player->unk_084 = -10.0f;
    player->unk_088 = 28.0f;
    player->topSpeed = 250.0f;
    func_8002B830(player, playerId, arg2);
    apply_effect(player, playerId, arg2);
    player->rotation[1] += player->unk_078;
    spF0[0] = 0;
    spF0[1] = 0;
    spF0[2] = 0;
    func_8002AB70(player);
    spA4 = 0 * (player->unk_064[0] + spFC[0]);
    spA0 = -1 * player->kartGravity;
    sp9C = 0 * (player->unk_064[2] + spFC[2]);
    sp108[2] = 0;
    sp108[1] = 0;
    sp108[0] = 0;
    mtxf_translate_vec3f_mat3(sp108, player->orientationMatrix);
    spA4 += sp108[0];
    sp9C += sp108[2];
    sp114[2] = player->kartPropulsionStrength;
    mtxf_translate_vec3f_mat3(sp114, player->orientationMatrix);

    sp88[0] = player->velocity[0];
    sp88[1] = player->velocity[1];
    sp88[2] = player->velocity[2];

    sp88[0] += ((((((sp114[0] + spA4) + spF0[0])) - (sp88[0] * (0.12 * (player->kartFriction)))) / 6000.0) / 1);
    sp88[2] += ((((((sp114[2] + sp9C) + spF0[2])) - (sp88[2] * (0.12 * (player->kartFriction)))) / 6000.0) / 1);
    sp88[1] += ((((((sp114[1] + spA0) + spF0[1])) - (sp88[1] * (0.12 * (player->kartFriction)))) / 6000.0) / 1);
    if ((player->unk_044 & 0x10) == 0x10) {
        player->unk_044 &= 0xFFEF;
    }

    posX = player->pos[0];
    posY = player->pos[1];
    posZ = player->pos[2];

    player->oldPos[0] = player->pos[0];
    player->oldPos[1] = player->pos[1];
    player->oldPos[2] = player->pos[2];

    spEC = posX + player->velocity[0];
    spE8 = posY + player->velocity[1];
    spE4 = posZ + player->velocity[2];
    func_8002AAC0(player);
    spE8 += player->kartHopVelocity;
    spE8 -= 0.02;
    actor_terrain_collision(&player->collision, player->boundingBoxSize, spEC, spE8, spE4, player->oldPos[0],
                            player->oldPos[1], player->oldPos[2]);
    player->unk_058 = 0;
    player->unk_060 = 0;
    player->unk_05C = 1.0f;
    calculate_orientation_matrix(player->orientationMatrix, 0, 1.0f, 0, player->rotation[1]);
    player->effects |= 8;
    player->unk_0C2 += 1;
    sp98 = player->collision.surfaceDistance[2];
    if (sp98 <= 0) {
        player->unk_0C2 = 0;
        player->effects &= ~2;
        player->effects &= ~8;
        player->kartHopVelocity = player->unk_0C2;
    }
    sp98 = player->collision.surfaceDistance[2];
    if (sp98 <= 0) {
        func_8003F46C(player, sp7C, sp88, sp108, &sp98, &spEC, &spE8, &spE4);
    }
    sp98 = player->collision.surfaceDistance[0];
    if (sp98 < 0) {
        func_8003F734(player, sp7C, sp88, &sp98, &spEC, &spE8, &spE4);
        func_8002C954(player, playerId, sp88);
    }
    sp98 = player->collision.surfaceDistance[1];
    if (sp98 < 0) {
        func_8003FBAC(player, sp7C, sp88, &sp98, &spEC, &spE8, &spE4);
        func_8002C954(player, playerId, sp88);
    }
    sp98 = player->collision.surfaceDistance[0];
    if (sp98 >= 0) {
        sp98 = player->collision.surfaceDistance[1];
        if (sp98 >= 0) {
            player->unk_046 &= 0xFFDF;
        }
    }
    player->unk_074 = calculate_surface_height(spEC, spE8, spE4, player->collision.meshIndexZX);
    func_80029B4C(player, spEC, spE8, spE4);
    func_8002AE38(player, playerId, posX, posZ, spEC, spE4);
    sqrt = (sp88[0] * sp88[0]) + (sp88[2] * sp88[2]);
    player->previousSpeed = player->speed;
    player->speed = sqrtf(sqrt);

    player->pos[0] = spEC;
    player->pos[2] = spE4;
    player->pos[1] = spE8;

    player->unk_064[0] = sp108[0];
    player->unk_064[2] = sp108[2];

    player->velocity[0] = sp88[0];
    player->velocity[1] = sp88[1];
    player->velocity[2] = sp88[2];

    gPlayerLastVelocity[playerId][0] = sp88[0];
    gPlayerLastVelocity[playerId][1] = sp88[1];
    gPlayerLastVelocity[playerId][2] = sp88[2];

    if ((player->type & PLAYER_HUMAN) == PLAYER_HUMAN) {
        if (gKartTopSpeedTable[player->characterId] < player->speed) {
            divOptimize = gKartTopSpeedTable[player->characterId] / player->speed;
            player->velocity[0] *= divOptimize;
            player->velocity[1] *= divOptimize;
            player->velocity[2] *= divOptimize;
            player->speed = gKartTopSpeedTable[player->characterId];
        }
    }
    if ((player->unk_044 & 1) == 1) {
        if (player->speed > 1.0f) {
            player->velocity[0] *= 1.0f / player->speed;
            player->velocity[1] *= 1.0f / player->speed;
            player->velocity[2] *= 1.0f / player->speed;
            player->speed = 1.0f;
        }
    }
    if (player->collision.surfaceDistance[2] >= 500.0f) {
        player->unk_078 /= 2;
    }
    update_player_environment_and_hazard_state(player, playerId);
}
