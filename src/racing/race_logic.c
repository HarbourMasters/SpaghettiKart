#include <libultraship.h>
#include <stdio.h>
#include <macros.h>
#include <mk64.h>
#include <common_structs.h>
#include <defines.h>
#include <sounds.h>
#include "TrackBrowser.h"
#include "camera.h"
#include "waypoints.h"
#include "replays.h"
#include "main.h"
#include "code_800029B0.h"
#include "code_80057C60.h"
#include "update_objects.h"
#include "menu_items.h"
#include "code_80005FD0.h"
#include "spawn_players.h"
#include "audio/external.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "math_util_2.h"
#include "effects.h"
#include "math.h"
#include "menus.h"
#include "sounds.h"
#include "port/Game.h"
#include "port/audio/HMAS.h"
#include "engine/editor/Editor.h"

#pragma intrinsic(sqrtf)

extern s16 gPlayerBalloonCount[];
extern s16 D_8016348C;

extern s32 gLapCountByPlayerId[];
extern u16 D_802BA048;

extern s32 D_8018D2AC;
extern s32 D_802B91E0;

u16 D_802BA030;
u16 D_802BA032;

float D_802BA034;

s32 D_802BA038;
UNUSED s32 D_802BA03C;

s16 D_802BA040[4];
u16 D_802BA048;

void func_8028DF00(void) {
    struct Controller* controllers = &gControllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        D_802BA040[i] = controllers->button;
        controllers++;
    }
}

void func_8028DF38(void) {
    struct Controller* controllers = &gControllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        controllers->buttonPressed = (controllers->button & (D_802BA040[i] ^ controllers->button));
        controllers->buttonDepressed = (D_802BA040[i] & (D_802BA040[i] ^ controllers->button));
        controllers->button = D_802BA040[i];
        controllers++;
    }
}

void func_8028E028(void) {

    switch (gPlayerCountSelection1) {
        case 2:
            *(gNmiUnknown4 + gPlayerWinningIndex) += 1;
            break;
        case 3:
            *(gNmiUnknown5 + gPlayerWinningIndex) += 1;
            break;
        case 4:
            *(gNmiUnknown6 + gPlayerWinningIndex) += 1;
            break;
    }
    func_800CA118((u8) gPlayerWinningIndex);
    gRaceState = RACE_FINISHED;
    D_802BA038 = 10;
}

void update_player_battle_status(void) {
    Player* player;
    s32 playerIndex;
    s16 playersAlive[4];
    s16 playersDead[4];
    s16 aliveCounter = 0;
    s16 deadCounter = 0;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        player = &gPlayers[playerIndex];
        if (!(player->type & PLAYER_EXISTS)) {
            continue;
        }
        if (player->type & PLAYER_CINEMATIC_MODE) {
            continue;
        }
        // If player has no balloons left
        if (gPlayerBalloonCount[playerIndex] < 0) {
            player->type |= PLAYER_CINEMATIC_MODE;
            playersDead[deadCounter] = (s16) (player - gPlayerOne);
            deadCounter++;
            func_800CA118((u8) playerIndex); // play sad character sound?
        } else {
            playersAlive[aliveCounter] = (s16) (player - gPlayerOne);
            aliveCounter++;
        }
    }
    if (aliveCounter == 1) {
        gPlayerWinningIndex = (s32) playersAlive[0];
        func_8028E028();
    } else if (aliveCounter == 0) {
        gPlayerWinningIndex = (s32) playersDead[0];
        func_8028E028();
    }
}

void func_8028E298(void) {
    f32 temp_v0;
    s32 i;
    u16 temp_a2;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((gPlayers[i].type & PLAYER_CINEMATIC_MODE)) {
            continue;
        }
        temp_a2 = gPathIndexByPlayerId[i];

        temp_v0 = ((2 - gPlayers[i].lapCount) * gPathCountByPathIndex[temp_a2]);
        temp_v0 += gPathCountByPathIndex[temp_a2] * (1.0f - gLapCompletionPercentByPlayerId[i]);
        temp_v0 /= 15.0f;

        gTimePlayerLastTouchedFinishLine[i] = gCourseTimer + temp_v0;
    }
    D_8016348C = 1;
    update_player_rankings();
}

void set_next_course(void) {

    if (D_80150120) {
        if (GetCupCursorPosition() == GetCupSize() - 1) { // CUP_COURSE_FOUR) {
            gGotoMode = ENDING;
        } else {
            D_800DC544++;
            SetCupCursorPosition(GetCupCursorPosition() + 1);
            gCourseIndexInCup++;
            gGotoMode = RACING;
        }
    } else {
        D_800DC544++;
        gCourseIndexInCup++;
        SetCupCursorPosition(GetCupCursorPosition() + 1);
        gGotoMode = RACING;
    }
}

void func_8028E438(void) {
    ScreenContext* temp_v0 = &gScreenContexts[gPlayerWinningIndex];
    s32 phi_v1_4;

    D_800DC5B0 = 1;

    switch (D_8015F894) {
        case 0:
            D_800DC5B8 = 0;
            D_8015F894 = 1;
            if (gPlayerCountSelection1 == 3) {
                func_800925CC();
            }
            break;
        case 1:
            if (temp_v0->screenWidth < SCREEN_WIDTH) {
                temp_v0->screenWidth += 2;
            }
            if (temp_v0->screenHeight < SCREEN_HEIGHT) {
                temp_v0->screenHeight += 2;
            }
            if (temp_v0->screenStartX < 160) {
                temp_v0->screenStartX += 1;

            } else if (temp_v0->screenStartX > 160) {
                temp_v0->screenStartX -= 1;
            }
            if (temp_v0->screenStartY < 120) {
                temp_v0->screenStartY += 1;
            } else if (temp_v0->screenStartY > 120) {
                temp_v0->screenStartY -= 1;
            }
            phi_v1_4 = 0;

            if (temp_v0->screenHeight >= SCREEN_HEIGHT) {
                phi_v1_4++;
                temp_v0->screenHeight = SCREEN_HEIGHT;
            }
            if (temp_v0->screenWidth >= SCREEN_WIDTH) {
                temp_v0->screenWidth = SCREEN_WIDTH;
                phi_v1_4++;
            }

            if (temp_v0->screenStartY == 120) {
                phi_v1_4++;
            }
            if (temp_v0->screenStartX == 160) {
                phi_v1_4++;
            }
            gScreenAspect = (f32) ((f32) temp_v0->screenWidth / (f32) temp_v0->screenHeight);
            if (phi_v1_4 == 4) {
                D_8015F894 = 2;
                gActiveScreenMode = SCREEN_MODE_1P;
                gScreenOneCtx->screenWidth = temp_v0->screenWidth;
                gScreenOneCtx->screenHeight = temp_v0->screenHeight;
                gScreenOneCtx->screenStartX = temp_v0->screenStartX;
                gScreenOneCtx->screenStartY = temp_v0->screenStartY;
                if (gModeSelection == BATTLE) {
                    func_80092604();
                } else if (gModeSelection == VERSUS) {
                    func_80092604();
                    func_80019DF4();
                } else {
                    func_80092564();
                    gRaceState = RACE_EXIT;
                }
            }
            break;
        case 2:
            break;
    }
}

void func_8028E678(void) {
    s32 phi_a0_10 = 0;

    D_800DC5B0 = 1;

    switch (D_8015F894) {
        case 0:
            // Unused switch?
            switch (gModeSelection) {
                case GRAND_PRIX:
                case VERSUS:
                    break;
                case TIME_TRIALS:
                    break;
            }
            D_800DC5B8 = 0;
            switch (gScreenModeSelection) {
                case SCREEN_MODE_1P:
                    D_8015F894 = 1;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    D_8015F894 = 5;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    D_8015F894 = 6;
                    break;
            }
            break;
        case 5:
            gScreenOneCtx->screenWidth -= 4;

            gScreenTwoCtx->screenWidth -= 4;

            gScreenOneCtx->screenStartX -= 2;

            gScreenTwoCtx->screenStartX += 2;

            if (gScreenOneCtx->screenWidth < 160) {
                gScreenOneCtx->screenWidth = 160;
                phi_a0_10++;
            }

            if (gScreenTwoCtx->screenWidth < 160) {
                gScreenTwoCtx->screenWidth = 160;
                phi_a0_10++;
            }

            if (gScreenOneCtx->screenStartX < 80) {
                gScreenOneCtx->screenStartX = 80;
                phi_a0_10++;
            }

            if (gScreenTwoCtx->screenStartX > SCREEN_HEIGHT) {
                gScreenTwoCtx->screenStartX = SCREEN_HEIGHT;
                phi_a0_10++;
            }

            gScreenAspect = (f32) ((f32) gScreenOneCtx->screenWidth / (f32) gScreenOneCtx->screenHeight);
            if (phi_a0_10 == 4) {
                D_8015F894 = 3;
                func_80092500();
                func_80019DE4();
                func_80041D24();
            }
            break;
        case 6:
            gScreenOneCtx->screenHeight -= 4;
            gScreenTwoCtx->screenHeight -= 4;
            gScreenOneCtx->screenStartY -= 2;
            gScreenTwoCtx->screenStartY += 2;

            if (gScreenOneCtx->screenHeight < 120) {
                gScreenOneCtx->screenHeight = 120;
                phi_a0_10++;
            }

            if (gScreenTwoCtx->screenHeight < 120) {
                gScreenTwoCtx->screenHeight = 120;
                phi_a0_10++;
            }

            if (gScreenOneCtx->screenStartY < 60) {
                gScreenOneCtx->screenStartY = 60;
                phi_a0_10++;
            }

            if (gScreenTwoCtx->screenStartY > 180) {
                gScreenTwoCtx->screenStartY = 180;
                phi_a0_10++;
            }

            gScreenAspect = (f32) ((f32) gScreenOneCtx->screenWidth / (f32) gScreenOneCtx->screenHeight);
            if (phi_a0_10 == 4) {
                D_8015F894 = 3;
                func_80092500();
                func_80019DE4();
            }
            break;
        case 1:
            gScreenOneCtx->screenHeight -= 2;
            gScreenOneCtx->screenWidth = (gScreenOneCtx->screenHeight * SCREEN_WIDTH) / SCREEN_HEIGHT;

            if (gScreenOneCtx->screenHeight < 120) {

                gScreenOneCtx->screenHeight = 120;
                gScreenOneCtx->screenWidth = (gScreenOneCtx->screenHeight * SCREEN_WIDTH) / SCREEN_HEIGHT;
                D_8015F894 = 2;

                gScreenTwoCtx->screenWidth = gScreenOneCtx->screenWidth;
                gScreenTwoCtx->screenHeight = gScreenOneCtx->screenHeight;
                gScreenTwoCtx->screenStartX = gScreenOneCtx->screenStartX;
                gScreenTwoCtx->screenStartY = gScreenOneCtx->screenStartY;

                gActiveScreenMode = SCREEN_MODE_2P_SPLITSCREEN_VERTICAL;
                gScreenAspect = 1.33333337;
                gPlayerCountSelection1 = 2;
                func_8003DB5C();
                func_8005994C();
            }
            break;
        case 2:
            gScreenOneCtx->screenStartX -= 4;

            gScreenOneCtx->screenStartY -= 2;

            if (gScreenOneCtx->screenStartX < 80) {
                gScreenOneCtx->screenStartX = 80;
                phi_a0_10++;
            }

            if (gScreenOneCtx->screenStartY < 60) {
                gScreenOneCtx->screenStartY = 60;
                phi_a0_10++;
            }
            gScreenTwoCtx->screenStartX += 4;
            gScreenTwoCtx->screenStartY += 2;

            if (gScreenTwoCtx->screenStartX > SCREEN_HEIGHT) {
                gScreenTwoCtx->screenStartX = SCREEN_HEIGHT;
                phi_a0_10++;
            }
            if (gScreenTwoCtx->screenStartY > 180) {
                gScreenTwoCtx->screenStartY = 180;
                phi_a0_10++;
            }
            if (phi_a0_10 == 4) {
                D_8015F894 = 7;
                D_802BA030 = 3;
            }
            break;
        case 7:
            D_802BA030--;
            if (D_802BA030 == 0) {
                D_8015F894 = 3;
                func_80092500();
                if (gModeSelection == GRAND_PRIX) {
                    func_80019DE4();
                } else {
                    func_80019E58();
                }
            }
            break;
        case 4:
            gIsInQuitToMenuTransition = 1;
            gQuitToMenuTransitionCounter = 5;
            gRaceState = RACE_EXIT;
            set_next_course();
            break;
    }
}

void func_8028EC38(s32 arg0) {
    gGotoMode = arg0;
    gRaceState = RACE_UNK;
    func_800CA330(25);
    func_800CA388(25);
    D_800DC5B4 = 1;
    D_800DC5B0 = 1;
    D_800DC5B8 = 0;
    D_802BA038 = 5;
    if(HMAS_IsPlaying(HMAS_MUSIC)) {
        HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_VOLUME, HMAS_LINEAR, 10, 0.0f);
    }
}

void func_8028EC98(s32 arg0) {

    // We want music in multiplayer, so this was removed
    //if (gScreenModeSelection == SCREEN_MODE_3P_4P_SPLITSCREEN) {
    //    return;
    //}

    func_800029B0();

    enum MusicSeq sequence = CM_GetProps()->Sequence;

    if (sequence != MUSIC_SEQ_UNKNOWN) {
        play_sequence(sequence);
    }
}

void start_race(void) {
    s32 i;

    D_8015011E = -1;
    if (!gDemoMode) {
        func_8028EC98(gCurrentCourseId);
    }

    if (gRaceState == RACE_STAGING) {
        gRaceState = RACE_IN_PROGRESS;
    }

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((gPlayers[i].type & PLAYER_EXISTS) == 0) {
            continue;
        }

        // Sets player to human.
        if (gPlayers[i].type & PLAYER_START_SEQUENCE) {
            gPlayers[i].type ^= PLAYER_START_SEQUENCE;
        }
    }
}

f32 func_8028EE8C(s32 arg0) {
    f32 temp_v0 = gPlayers[arg0].pos[2];
    f32 temp_v1 = gPlayers[arg0].oldPos[2];
    f32 temp_f14 = D_8015F8D0[2] - temp_v0;
    f32 temp_f16 = temp_v1 - D_8015F8D0[2];
    return gCourseTimer - ((TRACK_TIMER_ITER_f * temp_f14) / (temp_f14 + temp_f16));
}

void func_8028EEF0(s32 i) {
    gPlayers[i].type |= PLAYER_CINEMATIC_MODE;
}

void func_8028EF28(void) {
    s16 currentPosition;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        Player* player = &gPlayers[i];

        if ((gPlayers[i].type & PLAYER_EXISTS) == 0) {
            continue;
        }

        if (gLapCountByPlayerId[i] < gPlayers[i].lapCount) {
            gPlayers[i].lapCount--;
        } else if (gLapCountByPlayerId[i] > gPlayers[i].lapCount) {
            gPlayers[i].lapCount++;

            if ((gPlayers[i].type & PLAYER_HUMAN) != 0) {
                if (gPlayers[i].lapCount == 3) {
                    func_8028EEF0(i);

                    currentPosition = gPlayers[i].currentRank;
                    gPlayers[i].type |= PLAYER_CPU;

                    if (currentPosition < 4) {
                        D_80150120 = 1;
                    }

                    func_800CA118((u8) i);
                    if ((D_802BA032 & PLAYER_EXISTS) == 0) {
                        D_802BA032 |= PLAYER_EXISTS;
                    }

                    if (gModeSelection == GRAND_PRIX && gPlayerCountSelection1 == 2 && D_802BA048 == 0) {
                        D_802BA048 = 1;
                    }
                    if ((gPlayers[i].type & PLAYER_INVISIBLE_OR_BOMB) == 0) {
                        gRaceState = RACE_CALCULATE_RANKS;
                    }
                    if (gModeSelection == TIME_TRIALS) {
                        func_80005AE8(player);
                    }

                    if (gModeSelection == VERSUS) {
                        D_802BA038 = 180;
                        if (currentPosition == 0) {
                            gPlayerWinningIndex = i;
                        }
                        switch (gPlayerCountSelection1) {
                            case 2:
                                if (currentPosition == 0) {
                                    *(gNmiUnknown1 + i) += 1;
                                }
                                if (*(gNmiUnknown1 + i) > 99) {
                                    *(gNmiUnknown1 + i) = 99;
                                }
                                gRaceState = RACE_FINISHED;
                                i = gPlayerPositionLUT[1];
                                gPlayers[i].soundEffects |= 0x200000;
                                gPlayers[i].type |= PLAYER_CPU;
                                func_800CA118((u8) i);
                                break;
                            case 3:
                                if (currentPosition < 3) {
                                    *(gNmiUnknown2 + i * 3 + currentPosition) += 1;
                                }
                                if (*(gNmiUnknown2 + i * 3 + currentPosition) > 99) {
                                    *(gNmiUnknown2 + i * 3 + currentPosition) = 99;
                                }
                                if (currentPosition == 1) {
                                    gRaceState = RACE_FINISHED;
                                    i = gPlayerPositionLUT[2];
                                    *(gNmiUnknown2 + i * 3 + 2) += 1;
                                    if (*(gNmiUnknown2 + i * 3 + 2) > 99) {
                                        *(gNmiUnknown2 + i * 3 + 2) = 99;
                                    }
                                    gPlayers[i].soundEffects |= 0x200000;
                                    gPlayers[i].type |= PLAYER_CPU;
                                    func_800CA118((u8) i);
                                }
                                break;
                            case 4:
                                if (currentPosition < 3) {
                                    *(gNmiUnknown3 + i * 3 + currentPosition) += 1;
                                }
                                if (*(gNmiUnknown3 + i * 3 + currentPosition) > 99) {
                                    *(gNmiUnknown3 + i * 3 + currentPosition) = 99;
                                }
                                if (currentPosition == 2) {
                                    gRaceState = RACE_FINISHED;
                                    i = gPlayerPositionLUT[3];
                                    gPlayers[i].soundEffects |= 0x200000;
                                    gPlayers[i].type |= PLAYER_CPU;
                                    func_800CA118((u8) i);
                                }
                                break;
                        }
                    }

                } else if (gPlayers[i].lapCount == 2) {
                    if ((gPlayers[i].type & 0x100) != 0) {
                        return;
                    }
                    if ((D_802BA032 & 0x4000) == 0) {
                        D_802BA032 |= 0x4000;
                        func_800CA49C((u8) i);
                    }
                }
            } else if (gPlayers[i].lapCount == 3) {
                func_8028EEF0(i);
                if (gModeSelection == TIME_TRIALS) {
                    func_80005AE8(player);
                }
            }
        }
    }
    if ((D_802BA048 != 0) && (D_802BA048 != 100)) {
        D_802BA048 = 100;
        set_places_end_course_with_time();
    }
}

void func_8028F3E8(void) {
}

// func_8028F3F0
void update_race_position_data(void) {
    s16 i;
    s16 position;

    for (i = 0; i < NUM_PLAYERS; i++) {
        if (((gPlayers[i].type & PLAYER_EXISTS) != 0) && ((gPlayers[i].type & PLAYER_CINEMATIC_MODE) == 0) &&
            ((gPlayers[i].type & PLAYER_INVISIBLE_OR_BOMB) == 0)) {
            position = gGPCurrentRaceRankByPlayerId[i];
            gPlayers[i].currentRank = position;
            gPlayerPositionLUT[position] = i;
        }
    }
}

void func_8028F474(void) {
    s32 i;

    switch (gRaceState) {
        case RACE_IN_PROGRESS:
        case RACE_CALCULATE_RANKS:
        case RACE_FINISHED:
        case RACE_EXIT:
            for (i = 0; i < NUM_PLAYERS; i++) {
                update_player(i);
            }
        case RACE_SETUP:
        case RACE_STAGING:
            update_vehicles();
            break;
    }
}

void func_8028F4E8(void) {
    if (gEnableDebugMode) {
        if (((gControllerFive->button & R_TRIG) != 0) && ((gControllerFive->button & L_TRIG) != 0) &&
            ((gControllerFive->button & A_BUTTON) != 0) && ((gControllerFive->button & B_BUTTON) != 0)) {

            func_800CA330(0x19);
            func_800CA388(0x19);
            gGotoMode = START_MENU_FROM_QUIT;
            gRaceState = RACE_UNK;
            D_800DC5B4 = 1;
            D_800DC5B0 = 1;
            D_800DC5B8 = 0;
            D_802BA038 = 5;
        }
    }
}

/**
 * On race launch, the screen starts small and quickly gets bigger
 * as an effect.
 */
void func_8028F588(void) {
    s16 screenWidth;

    if ((Editor_IsEnabled() == true) || (CM_IsTourEnabled() == true)) {
        gScreenOneCtx->screenWidth = SCREEN_WIDTH;
        gScreenOneCtx->screenHeight = SCREEN_HEIGHT;
        return;
    }

    switch (gActiveScreenMode) { /* irregular */
        case SCREEN_MODE_1P:
            screenWidth = (s16) (s32) (320.0f * D_802BA034);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            gScreenOneCtx->screenWidth = screenWidth;
            screenWidth = (s16) (s32) (240.0f * D_802BA034);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            gScreenOneCtx->screenHeight = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            screenWidth = (s16) (s32) (160.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            gScreenOneCtx->screenWidth = screenWidth;
            gScreenTwoCtx->screenWidth = screenWidth;
            screenWidth = (s16) (s32) (240.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x1E0) {
                screenWidth = 0x01DC;
            }
            gScreenOneCtx->screenHeight = screenWidth;
            gScreenTwoCtx->screenHeight = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            screenWidth = (s16) (s32) (320.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x280) {
                screenWidth = 0x027C;
            }
            gScreenOneCtx->screenWidth = screenWidth;
            gScreenTwoCtx->screenWidth = screenWidth;
            screenWidth = (s16) (s32) (120.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            gScreenOneCtx->screenHeight = screenWidth;
            gScreenTwoCtx->screenHeight = screenWidth;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            screenWidth = (s16) (s32) (160.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            gScreenOneCtx->screenWidth = screenWidth;
            gScreenTwoCtx->screenWidth = screenWidth;
            gScreenThreeCtx->screenWidth = screenWidth;
            gScreenFourCtx->screenWidth = screenWidth;
            screenWidth = (s16) (s32) (120.0f * D_802BA034);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            gScreenOneCtx->screenHeight = screenWidth;
            gScreenTwoCtx->screenHeight = screenWidth;
            gScreenThreeCtx->screenHeight = screenWidth;
            gScreenFourCtx->screenHeight = screenWidth;
            break;
    }
}

void func_8028F8BC(void) {
    D_802BA034 = (f32) (D_802BA034 - 0.017f);
    if (D_802BA034 < 0.0f) {
        D_802BA034 = 0.0f;
    }
    func_8028F588();
}

void func_8028F914(void) {
    D_802BA034 = (f32) (D_802BA034 + 0.028f);
    if (D_802BA034 > 1.0f) {
        D_802BA034 = 1.0f;
    }
    func_8028F588();
}

void func_8028F970(void) {
    s32 i;

    if (D_8015F890) {
        return;
    }

    //! @todo increasing players past four would require increase this loop iterator.
    for (i = 0; i < 4; i++) {

        Player* player = &gPlayers[i];
        struct Controller* controller = &gControllers[i];

        if (!(player->type & PLAYER_HUMAN)) {
            continue;
        }
        if (player->type & PLAYER_CPU) {
            continue;
        }

        if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            if ((controller->buttonPressed & L_TRIG) && !(controller->button & R_TRIG)) {
                controller->buttonPressed &= 0xFFDF;

                D_800DC5A8++;
                if (D_800DC5A8 >= 3) {
                    D_800DC5A8 = 0;
                }
                play_sound2(SOUND_ACTION_PING);
                func_800029B0();
            }
        }
        if ((controller->buttonPressed & START_BUTTON) && (!(controller->button & R_TRIG)) &&
            (!(controller->button & L_TRIG))) {
            func_8028DF00();
            gIsGamePaused = (controller - gControllerOne) + 1;
            controller->buttonPressed = 0;
            func_800C9F90(1);
            gPauseTriggered = 1;
            if (gModeSelection == TIME_TRIALS) {
                if (gPlayerOne->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerOne);
                }
                if (gPlayerTwo->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerTwo);
                }
                if (gPlayerThree->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerThree);
                }
            }
            return;
        }
    }

    if (gEnableDebugMode) {
        if (gModeSelection != BATTLE) { // Skip laps
            if (gControllerOne->buttonPressed & U_JPAD) {
                gLapCountByPlayerId[0] = 2;
                play_sound2(SOUND_MENU_OPTION);
                printf("[race_logic.c] [func_8028F970] Skipped laps for player 1\n");
            }
            if (gControllerOne->buttonPressed & R_JPAD) {
                gLapCountByPlayerId[0] = 2;
                gLapCountByPlayerId[1] = 2;
                play_sound2(SOUND_MENU_OPTION);
                printf("[race_logic.c] [func_8028F970] Skipped laps for players 1 & 2\n");
            }
            if (gControllerOne->buttonPressed & D_JPAD) {
                for (size_t i = 0; i < NUM_PLAYERS; i++) {
                    gLapCountByPlayerId[i] = 2;
                }
                play_sound2(SOUND_MENU_OPTION);
                printf("[race_logic.c] [func_8028F970] Skipped laps for all players\n");
            }
        }
    }
}

void func_8028FBD4(void) {
    gGotoMode = START_MENU_FROM_QUIT;
    gRaceState = RACE_UNK;
    func_800CA330(25);
    func_800CA388(25);
    D_800DC5B4 = 1;
    D_800DC5B0 = 1;
    D_800DC5B8 = 0;
    D_802BA038 = 5;
}

#ifdef VERSION_EU
#define D_802BA038_SIZE 1600
#else
#define D_802BA038_SIZE 1920
#endif

void func_8028FC34(void) {
    if (D_802BA038 < 0) {
        D_802BA038 = D_802BA038_SIZE;
        return;
    }
    D_802BA038--;
    if (gControllerFive->buttonPressed != 0) {
        func_8028FBD4();
        gMenuSelection = START_MENU;
        return;
    }
    if (D_802BA038 == 0) {
        func_8028FBD4();
        gMenuSelection = LOGO_INTRO_MENU;
    }
}

void func_8028FCBC(void) {
    Player* ply = &gPlayers[0];
    s32 i;
    u32 phi_v0_4;

    if (gDemoUseController) {
        func_8028FC34();
    }
    switch (gRaceState) {
        case RACE_INIT:
            if (!gDemoMode) {
                if (gModeSelection == GRAND_PRIX) {
                    play_sequence2(MUSIC_SEQ_START_GRID_GP_VS);
                    play_sound2(SOUND_ACTION_REV_ENGINE);
                    play_sound2(SOUND_ACTION_REV_ENGINE_2);
                } else {
                    play_sequence2(MUSIC_SEQ_START_GRID_TIME_ATTACK);
                }
            }
            func_80002DAC();
            gRaceState = RACE_SETUP;
            D_80150118 = 3.0f;
            // creditsRenderMode = 0; This is now set in GameCamera class
            D_802BA032 = 0;
            D_8015011E = 0;
            gCourseTimer = 0.0f;
            gVBlankTimer = 0.0f;
            D_800DC5B0 = 1;
            D_800DC5B4 = 1;
            D_802BA034 = 0.008f;
            D_8015F894 = 0;
            if (gScreenModeSelection != SCREEN_MODE_1P) {
                func_8005C64C(&D_8018D2AC);
            }
            for (i = 0; i < NUM_PLAYERS; i++) {
                if ((ply->type & PLAYER_EXISTS) == 0) {
                    continue;
                }
                ply->type |= PLAYER_START_SEQUENCE;
                ply++;
            }
            D_800DC5B8 = 1;

            break;
        case RACE_SETUP:
            func_8028F914();
            if (D_802BA034 == 1.0f) {
                if (gActiveScreenMode != SCREEN_MODE_1P) {
                    if (IsLuigiRaceway()) {
                        func_802A7940();
                    } else if (IsWarioStadium()) {
                        func_802A7728();
                    }
                }
                gRaceState = RACE_STAGING;
                D_800DC5B0 = 0;
                D_800DC5B8 = 1;
                CM_SpawnStarterLakitu(); // func_80078F64();
                if ((gModeSelection == TIME_TRIALS) && (bCourseGhostDisabled == 0)) {
                    phi_v0_4 = 0x1;
                    //! @warning this used to be < gCurrentCourseId
                    // Hopefully this is equivallent.
                    for (i = 0; i < TrackBrowser_GetTrackIndex(); i++) {
                        phi_v0_4 <<= 1;
                    }
                    if ((D_8015F890 == 0) && (!(D_800DC5AC & phi_v0_4))) {
                        func_80092630();
                        D_800DC5AC |= phi_v0_4;
                    }
                }
                if (gPlayerCountSelection1 == 3) {
                    func_800925A0();
                }
            }
            func_8028F4E8();
            break;
        case RACE_STAGING:
            if (gDemoMode) {
                start_race();
            }
            if ((gEnableDebugMode) && (gControllerFive->buttonPressed & Z_TRIG)) {
                start_race();
            }
            func_8028F4E8();
            break;
        case RACE_IN_PROGRESS:
            if (gModeSelection == BATTLE) {
                update_player_battle_status();
            } else {
                update_race_position_data();
                func_8028EF28();
            }
            func_8028F4E8();
            func_8028F970();
            break;
        case RACE_CALCULATE_RANKS:
            switch (gModeSelection) {
                case GRAND_PRIX:
                    func_8028F4E8();
                    update_race_position_data();
                    func_8028EF28();
                    func_8028F970();

                    switch (gScreenModeSelection) {
                        case SCREEN_MODE_1P:
                            D_802BA038 = 690;
                            gRaceState = RACE_FINISHED;
                            func_8028E298();
                            break;
                        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                            if (((gPlayerOne->type & PLAYER_CINEMATIC_MODE) != 0) &&
                                ((gPlayerTwo->type & PLAYER_CINEMATIC_MODE) != 0)) {

                                if (gPlayerOne->currentRank < gPlayerTwo->currentRank) {
                                    gPlayerWinningIndex = 1;
                                } else {
                                    gPlayerWinningIndex = 0;
                                }

                                func_8028E298();
                                D_802BA038 = 600;
                                gRaceState = RACE_FINISHED;
                            }
                            break;
                    }
                    break;
                case VERSUS:
                    func_8028F4E8();
                    update_race_position_data();
                    func_8028EF28();
                    func_8028F970();
                    break;
                case TIME_TRIALS:
                    D_802BA038 = 360;
                    if (D_8015F890 != 0) {
                        gRaceState = RACE_EXIT;
                    } else {
                        gRaceState = RACE_FINISHED;
                    }
                    break;
            }
            break;
        case RACE_FINISHED:
            if (D_802BA038 != 0) {
                D_802BA038--;
            } else {
                switch (gModeSelection) {
                    case GRAND_PRIX:
                        if (D_80150120 != 0) {
                            func_8028E678();
                        } else if (gScreenModeSelection == SCREEN_MODE_1P) {
                            func_80092564();
                            gRaceState = RACE_EXIT;
                        } else {
                            func_8028E438();
                        }
                        break;
                    case TIME_TRIALS:
                        func_8028E678();
                        break;
                    case VERSUS:
                    case BATTLE:
                        func_8028E438();
                        break;
                }
            }
            func_8028F4E8();
            break;
        case RACE_UNK:
            func_8028F8BC();
            if (D_802BA034 <= 0) {
                gIsInQuitToMenuTransition = 1;
                gQuitToMenuTransitionCounter = 5;
                gTourComplete = false;
            }
            break;
        case RACE_EXIT:
            break;
    }
}

UNUSED void func_80290314(void) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = START_MENU_FROM_QUIT;
    gTourComplete = false;
}

void func_80290338(void) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = MAIN_MENU_FROM_QUIT;
    gTourComplete = false;
}

void func_80290360(void) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = PLAYER_SELECT_MENU_FROM_QUIT;
    gTourComplete = false;
}

void func_80290388(void) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = COURSE_SELECT_MENU_FROM_QUIT;
    gTourComplete = false;
}

void func_802903B0(void) {
    gIsInQuitToMenuTransition = 1;
    gQuitToMenuTransitionCounter = 5;
    gGotoMode = RACING;
    gTourComplete = false;
    // Stop when retrying
    if(HMAS_IsPlaying(HMAS_MUSIC)) {
        HMAS_Stop(HMAS_MUSIC);
    }
}

void func_802903D8(Player* playerOne, Player* playerTwo) {
    f32 sp70 = (playerOne->boundingBoxSize + playerTwo->boundingBoxSize) - 5.0f;
    f32 temp_f0;
    f32 sp74;
    Vec3f sp60;
    Vec3f sp54;
    f32 temp_f0_2;
    f32 temp_f16;
    f32 temp_f2;

    f32 gFloatArray802B8790[] = { 1.2, 1.0, 0.9, 0.7, 2.0, 1.8, 0.9, 2.3 };

    f32 sp24 = gFloatArray802B8790[playerOne->characterId];
    f32 sp20 = gFloatArray802B8790[playerTwo->characterId];

    sp60[0] = playerOne->pos[0] - playerTwo->pos[0];
    sp60[1] = (playerOne->pos[1] - playerOne->boundingBoxSize) - (playerTwo->pos[1] - playerTwo->boundingBoxSize);
    sp60[2] = playerOne->pos[2] - playerTwo->pos[2];

    sp54[0] = playerTwo->velocity[0] - playerOne->velocity[0];
    sp54[1] = playerTwo->velocity[1] - playerOne->velocity[1];
    sp54[2] = playerTwo->velocity[2] - playerOne->velocity[2];

    temp_f0 = sqrtf((sp60[0] * sp60[0]) + (sp60[1] * sp60[1]) + (sp60[2] * sp60[2]));

    if (temp_f0 < 0.1f) {
        return;
    }

    sp74 = temp_f0 - sp70;
    if (sp74 > 0) {
        return;
    }

    if (playerOne->type & PLAYER_UNKNOWN_0x40) {
        if (playerTwo->type & PLAYER_UNKNOWN_0x40) {
            func_8008FC1C(playerOne);
            func_8008FC1C(playerTwo);
            func_800C9060((playerTwo - gPlayerOne), 0x19008001U);
            return;
        } else {
            playerTwo->soundEffects |= REVERSE_SOUND_EFFECT;
            func_8008FC1C(playerOne);
            func_800C9060((playerTwo - gPlayerOne), 0x19008001U);
        }
    } else if (playerTwo->type & PLAYER_UNKNOWN_0x40) {
        playerOne->soundEffects |= REVERSE_SOUND_EFFECT;
        func_8008FC1C(playerTwo);
        func_800C9060(playerOne - gPlayerOne, 0x19008001U);
        return;
    }
    if (playerOne->effects & 0x200) {
        if (!(playerTwo->effects & 0x200)) {
            playerTwo->soundEffects |= HIT_BY_ITEM_SOUND_EFFECT;
        }
    } else if (playerTwo->effects & 0x200) {
        playerOne->soundEffects |= HIT_BY_ITEM_SOUND_EFFECT;
    } else {
        playerOne->effects |= 0x8000;
        playerTwo->effects |= 0x8000;
    }
    temp_f0_2 = sqrtf((sp54[0] * sp54[0]) + (sp54[1] * sp54[1]) + (sp54[2] * sp54[2]));
    sp60[0] /= temp_f0;
    sp60[1] /= temp_f0;
    sp60[2] /= temp_f0;
    if (temp_f0_2 < 0.2f) {
        temp_f0 = (playerOne->boundingBoxSize + playerTwo->boundingBoxSize) * 0.55f;
        playerOne->pos[0] = playerTwo->pos[0] + (sp60[0] * temp_f0);
        playerOne->pos[1] = playerTwo->pos[1] + (sp60[1] * temp_f0);
        playerOne->pos[2] = playerTwo->pos[2] + (sp60[2] * temp_f0);
        playerTwo->pos[0] -= temp_f0 * sp60[0];
        playerTwo->pos[1] -= temp_f0 * sp60[1];
        playerTwo->pos[2] -= temp_f0 * sp60[2];
        return;
    } else {
        temp_f16 = ((sp60[0] * sp54[0]) + (sp60[1] * sp54[1]) + (sp60[2] * sp54[2])) / temp_f0_2;
    }
    temp_f0_2 = temp_f0_2 * temp_f16 * 0.85;
    if ((playerOne->effects & 0x200) != 0x200) {
        temp_f2 = (temp_f0_2 * sp20) / sp24;
        playerOne->velocity[0] += sp60[0] * temp_f2;
        playerOne->velocity[1] += sp60[1] * temp_f2;
        playerOne->velocity[2] += sp60[2] * temp_f2;
        playerOne->pos[0] -= sp60[0] * sp74 * 0.5f;
        playerOne->pos[1] -= sp60[1] * sp74 * 0.5f;
        playerOne->pos[2] -= sp60[2] * sp74 * 0.5f;
    }
    if ((playerTwo->effects & 0x200) != 0x200) {
        temp_f2 = (temp_f0_2 * sp24) / sp20;
        playerTwo->velocity[0] -= sp60[0] * temp_f2;
        playerTwo->velocity[1] -= sp60[1] * temp_f2;
        playerTwo->velocity[2] -= sp60[2] * temp_f2;
        playerTwo->pos[0] += sp60[0] * sp74 * 0.5f;
        playerTwo->pos[1] += sp60[1] * sp74 * 0.5f;
        playerTwo->pos[2] += sp60[2] * sp74 * 0.5f;
    }
    if (playerOne->type & PLAYER_HUMAN) {
        func_800C9060((playerOne - gPlayerOne), 0x19008001U);
        return;
    }
    if (playerTwo->type & PLAYER_HUMAN) {
        func_800C9060((playerTwo - gPlayerOne), 0x19008001U);
    }
}

void func_802909F0(void) {
    Player* ply;
    Player* ply2;
    s32 i;
    s32 k;

    for (i = 0; i < 7; i++) {
        ply = &gPlayers[i];

        if ((ply->type & PLAYER_EXISTS) && (!(ply->effects & 0x80000000)) &&
            (!(ply->type & PLAYER_INVISIBLE_OR_BOMB)) && (!(ply->effects & 0x4000000))) {

            for (k = i + 1; k < NUM_PLAYERS; k++) {
                ply2 = &gPlayers[k];

                if ((ply2->type & PLAYER_EXISTS) && (!(ply2->effects & 0x80000000)) &&
                    (!(ply2->type & PLAYER_INVISIBLE_OR_BOMB)) && (!(ply2->effects & 0x4000000))) {

                    func_802903D8(ply, ply2);
                }
            }
        }
    }
}

void func_80290B14(void) {

    func_80059C50();

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            func_8001EE98(gPlayerOne, camera1, 0);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            func_8001EE98(gPlayerOne, camera1, 0);
            func_8001EE98(gPlayerTwo, camera2, 1);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            func_8001EE98(gPlayerOne, camera1, 0);
            func_8001EE98(gPlayerTwo, camera2, 1);
            func_8001EE98(gPlayerThree, camera3, 2);
            func_8001EE98(gPlayerFour, camera4, 3);
            break;
    }
}
