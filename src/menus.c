#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include <mk64.h>
#include <stubs.h>

#include "menus.h"
#include "TrackBrowser.h"
#include "editor/Editor.h"
#include "main.h"
#include "code_800029B0.h"
#include "actors.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "menu_items.h"
#include "code_800AF9B0.h"
#include "save.h"
#include "replays.h"
#include "save_data.h"
#include <sounds.h>
#include "spawn_players.h"
#include "port/Game.h"

/** BSS **/
s32 gIntroModelZEye;
f32 gIntroModelScale; // XYZ scale on checkerboard flag, Z scale on intro logo
f32 gIntroModelRotX;
f32 gIntroModelRotY;
f32 gIntroModelRotZ;
f32 gIntroModelPosX;
f32 gIntroModelPosY;
f32 gIntroModelPosZ;
s32 gMenuFadeType;
s8 gCharacterGridSelections[4];   // Map from each player to current grid position (1-4 top, 5-8 bottom)
bool gCharacterGridIsSelected[4]; // Sets true if a character is selected for each player
s8 gSubMenuSelection;             // Map Select states, Options and Ghost Data text selection
s8 gMainMenuSelection;
s8 gPlayerSelectMenuSelection; // grid screen state?
s8 gDebugMenuSelection;
s8 gControllerPakMenuSelection;
s8 gScreenModeListIndex; // 0-4 index, selects a screen mode in sScreenModePlayerTable
u8 gSoundMode;
s8 gPlayerCount;
s8 gVersusResultCursorSelection;     // 4 options indexed (10-13), gets set when selecting an option
s8 gTimeTrialsResultCursorSelection; // 5 options indexed (5-9), gets set when selecting an option (excluding Save
                                     // Ghost)
s8 gBattleResultCursorSelection;     // 4 options indexed (10-13), gets set when selecting an option
s8 gTimeTrialDataCourseIndex;
s8 gCourseRecordsMenuSelection;    // Used for selecting an option in track record data
s8 gCourseRecordsSubMenuSelection; // Used for erase records and ghosts (Quit - Erase)
s8 gDebugGotoScene;
bool gGhostPlayerInit;
bool gTrackMapInit;
s32 gMenuTimingCounter;
s32 gMenuDelayTimer;
s8 gDemoUseController; // Sets true alongside gDemoMode, controller related
s8 gCupSelection;
s8 sTempCupSelection; // Same as gCupSelection but it's only set in map select, not referenced
s8 gCourseIndexInCup;
s8 unref_D_8018EE0C; // Set to 0 but never referenced

/** Data **/
s32 gMenuSelection = HARBOUR_MASTERS_MENU;
s32 gFadeModeSelection = FADE_MODE_NONE;
s8 gCharacterSelections[4] = { MARIO, LUIGI, YOSHI, TOAD };

// The current row selected in the mode column for each player indexed
// 0-1 1p / 0-2 2p´/ 0-1 3p / 0-1 4p
s8 gGameModeMenuColumn[NUM_ROWS_GAME_MODE_MENU] = { 0, 0, 0, 0 };

// For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
// be whether 'Begin' or 'Data' is selected. Not used for Battle.
// indexed as [column][row]
s8 gGameModeSubMenuColumn[NUM_COLUMN_GAME_MODE_SUB_MENU][NUM_ROWS_GAME_MODE_SUB_MENU] = { 
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }
};

s8 gNextDemoId = 0;
s8 gControllerPakSelectedTableRow = 0; // 0-4 index, value of the current visible row select

// Numbers starting from the second 0 to number 6 get altered
// as you move up or down the page table of content (min value is 0, max value is 16
s8 gControllerPakVisibleTableRows[12] = { 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0 };
s8 gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE; // 1 is down, 2 is up
s8 unref_D_800E86D4[12] = { 0 };
s8 unref_D_800E86E0[4] = { 0, 0, 0, 1 };

u32 sVIGammaOffDitherOn = (OS_VI_GAMMA_OFF | OS_VI_DITHER_FILTER_ON);

/** Rodata **/

// Sets the actual screen mode based on values set in sScreenModePlayerCount
const s8 sScreenModePlayerTable[] = { SCREEN_MODE_1P, SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL,
                                      SCREEN_MODE_2P_SPLITSCREEN_VERTICAL, SCREEN_MODE_3P_4P_SPLITSCREEN,
                                      SCREEN_MODE_3P_4P_SPLITSCREEN };

// Sets how many players can load on each screen mode set in sScreenModePlayerTable
const s8 sScreenModePlayerCount[] = { 1, 2, 2, 3, 4 };

// Set indexed slots numbers for one-two-three-four mode selection
const s8 gPlayerModeSelection[] = { 1, 2, 1, 1 };

// Limit for each index column in one-two-three-four mode selection
const s8 sGameModePlayerColumnDefault[][3] = {
    { 2, 1, 0 }, // 1p (GP options, TT options, ...)
    { 2, 2, 0 }, // 2p (GP options, VS options, Battle)
    { 2, 0, 0 }, // 3p (VS options, Battle, ...)
    { 2, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Limit for each index column in one-two-three-four mode selection
// for extra mode (mirror mode), hence the extra value (3 instead of 2)
const s8 sGameModePlayerColumnExtra[][3] = {
    { 3, 1, 0 }, // 1p (GP options, TT options, ...)
    { 3, 3, 0 }, // 2p (GP options, VS options, Battle)
    { 3, 0, 0 }, // 3p (VS options, Battle, ...)
    { 3, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Modes to select in one-two-three-four mode selection
const s32 gGameModePlayerSelection[][3] = {
    { GRAND_PRIX, TIME_TRIALS, 0x00000000 }, // 1p game modes
    { GRAND_PRIX, VERSUS, BATTLE },          // 2p game modes
    { VERSUS, BATTLE, 0x00000000 },          // 3p game modes
    { VERSUS, BATTLE, 0x00000000 },          // 4p game modes
};

// Map from character grid position id to character id
// Note: changing order doesn't affect graphics, only the selection
const s8 sCharacterGridOrder[] = {
    MARIO, LUIGI, PEACH, TOAD, YOSHI, DK, WARIO, BOWSER,
};

const s16 gCupCourseOrder[5][4] = {
    // mushroom cup
    { TRACK_LUIGI_RACEWAY, TRACK_MOO_MOO_FARM, TRACK_KOOPA_BEACH, TRACK_KALIMARI_DESERT },
    // flower cup
    { TRACK_TOADS_TURNPIKE, TRACK_FRAPPE_SNOWLAND, TRACK_CHOCO_MOUNTAIN, TRACK_MARIO_RACEWAY },
    // star cup
    { TRACK_WARIO_STADIUM, TRACK_SHERBET_LAND, TRACK_ROYAL_RACEWAY, TRACK_BOWSER_CASTLE },
    // special cup
    { TRACK_DK_JUNGLE, TRACK_YOSHI_VALLEY, TRACK_BANSHEE_BOARDWALK, TRACK_RAINBOW_ROAD },
    // battle mode
    { TRACK_BIG_DONUT, TRACK_BLOCK_FORT, TRACK_DOUBLE_DECK, TRACK_SKYSCRAPER },
};

const s8 unref_800F2BDC[4] = { 1, 0, 0, 0 };

// Uses player count to set gScreenModeListIndex, the latter variable then selects a mode
// from sScreenModePlayerTable, note the 2 is not set since that's for vertical 2p screen
const s8 sScreenModeIdxFromPlayerMode[4] = { 0, 1, 3, 4 };

const union GameModePack sSoundMenuPack = { { SOUND_STEREO, SOUND_HEADPHONES, SOUND_UNUSED, SOUND_MONO } };

/**************************/

/**
 * Includes opening logo and splash screens
 */
void update_menus(void) {
    u16 controllerIdx;

    if (gFadeModeSelection == FADE_MODE_NONE) {
        for (controllerIdx = 0; controllerIdx < 4; controllerIdx++) {
            // Debug, quick jump through menus using the start button.
            if ((is_screen_being_faded() == 0) && (gEnableDebugMode) &&
                ((gControllers[controllerIdx].buttonPressed & START_BUTTON) != 0)) {
                // this is certainly a way to write these...
                switch (gMenuSelection) {
                    case COURSE_SELECT_MENU:
                        func_800CA330(0x19);
                        // deliberate (?) fallthru
                    case MAIN_MENU:
                    case CHARACTER_SELECT_MENU:
                        play_sound2(SOUND_MENU_OK_CLICKED);
                        break;
                }

                switch (gMenuSelection) {
                    case CONTROLLER_PAK_MENU:
                    case START_MENU:
                        break;
                    default:
                        func_8009E1C0();
                }
            }
            osViSetSpecialFeatures(sVIGammaOffDitherOn);
            switch (gMenuSelection) {
                case OPTIONS_MENU:
                    options_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case DATA_MENU:
                    data_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_DATA_MENU:
                    course_data_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case HARBOUR_MASTERS_MENU:
                    logo_intro_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case LOGO_INTRO_MENU:
                    logo_intro_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case CONTROLLER_PAK_MENU:
                    if (controllerIdx == PLAYER_ONE) {
                        controller_pak_menu_act(&gControllers[controllerIdx], controllerIdx);
                    }
                    break;
                case START_MENU_FROM_QUIT:
                case START_MENU:
                    splash_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case MAIN_MENU_FROM_QUIT:
                case MAIN_MENU:
                    main_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case PLAYER_SELECT_MENU_FROM_QUIT:
                case CHARACTER_SELECT_MENU:
                    player_select_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_SELECT_MENU_FROM_QUIT:
                case COURSE_SELECT_MENU:
                    course_select_menu_act(&gControllers[controllerIdx], controllerIdx);
                    break;
            }
        }
    }
}

/**
 * Navigation of the options menu
 */
void options_menu_act(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick; // sp3E
    MenuItem* sp38;
    s32 res;
    struct_8018EE10_entry* sp30;
    bool tempVar; // cursorWasMoved or communicateStoredAction
    UNUSED u32 pad;

    btnAndStick = (controller->buttonPressed | controller->stickPressed);

    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!is_screen_being_faded()) {
        sp38 = find_menu_items_dupe(0xF0);
        sp30 = (struct_8018EE10_entry*) gSomeDLBuffer;
        switch (gSubMenuSelection) {
            case SUB_MENU_OPTION_RETURN_GAME_SELECT:
            case SUB_MENU_OPTION_SOUND_MODE:
            case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
            case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                tempVar = false;
                if ((btnAndStick & D_JPAD) && (gSubMenuSelection < SUB_MENU_OPTION_MAX)) {
                    gSubMenuSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = 1;
                    tempVar = true;
                }
                if ((btnAndStick & U_JPAD) && (gSubMenuSelection > SUB_MENU_OPTION_MIN)) {
                    gSubMenuSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    tempVar = true;
                    sp38->subState = -1;
                }
                if (tempVar && gSoundMode != sp38->state) {
                    gSaveData.main.saveInfo.soundMode = gSoundMode;
                    write_save_data_grand_prix_points_and_sound_mode();
                    update_save_data_backup();
                    sp38->state = gSoundMode;
                }
                if (btnAndStick & B_BUTTON) {
                    func_8009E280();
                    play_sound2(SOUND_MENU_GO_BACK);
                    if (gSoundMode != sp38->state) {
                        gSaveData.main.saveInfo.soundMode = gSoundMode;
                        write_save_data_grand_prix_points_and_sound_mode();
                        update_save_data_backup();
                        sp38->state = gSoundMode;
                    }
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (gSubMenuSelection) {
                        case SUB_MENU_OPTION_SOUND_MODE:
                            if (gSoundMode < 3) {
                                gSoundMode += 1;
                            } else {
                                gSoundMode = SOUND_STEREO;
                            }
                            if (gSoundMode == SOUND_UNUSED) {
                                gSoundMode = SOUND_MONO;
                            }
                            set_sound_mode();
                            switch (gSoundMode) {
                                case SOUND_STEREO:
                                    play_sound2(SOUND_MENU_STEREO);
                                    return;
                                case SOUND_HEADPHONES:
                                    play_sound2(SOUND_MENU_HEADPHONES);
                                    return;
                                case SOUND_MONO:
                                    play_sound2(SOUND_MENU_MONO);
                                    return;
                            }
                            break;
                        case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
                            switch (controller_pak_2_status()) {
                                case PFS_INVALID_DATA:
                                    gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_NO_ERROR:
                                    func_800B6798();
                                    tempVar = controller_pak_1_status();
                                    switch (tempVar) {
                                        case PFS_INVALID_DATA:
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT;
                                            sp38->state = 0;
                                            play_sound2(SOUND_MENU_SELECT);
                                            break;
                                        case PFS_NO_ERROR:
                                            func_800B6708();
                                            break;
                                        case PFS_NO_PAK_INSERTED:
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_FILE_OVERFLOW:
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_PAK_BAD_READ:
                                        case PFS_PAK_CORRUPTED: // unreachable, bad reads always returns previous case
                                        default:
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                    }
                                    if (tempVar == PFS_INVALID_DATA && !sp30[PLAYER_ONE].ghostDataSaved &&
                                        !sp30[PLAYER_TWO].ghostDataSaved) {
                                        gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                        play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                        return;
                                    }
                                    if (tempVar == PFS_NO_ERROR) {
                                        if (sp30[PLAYER_ONE].ghostDataSaved) {
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                                            play_sound2(SOUND_MENU_SELECT);
                                        } else if (sp30[PLAYER_TWO].ghostDataSaved) {
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
                                            play_sound2(SOUND_MENU_SELECT);
                                        } else {
                                            gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                        }
                                    }
                                    // else return?
                                    return;
                                case PFS_NO_PAK_INSERTED:
                                    gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_PAK_BAD_READ:
                                default:
                                    gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                            }
                        case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                            gSubMenuSelection = SUB_MENU_ERASE_QUIT;
                            play_sound2(SOUND_MENU_SELECT);
                            return;
                        }
                        case SUB_MENU_OPTION_RETURN_GAME_SELECT: {
                            func_8009E280();
                            play_sound2(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                }
                // maybe else return?;
                break;
            }
            case SUB_MENU_ERASE_QUIT:
            case SUB_MENU_ERASE_ERASE: {
                if ((btnAndStick & D_JPAD) && (gSubMenuSelection < SUB_MENU_ERASE_MAX)) {
                    gSubMenuSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = 1;
                }
                if ((btnAndStick & U_JPAD) && (gSubMenuSelection > SUB_MENU_ERASE_MIN)) {
                    gSubMenuSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    gSubMenuSelection = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (gSubMenuSelection) {
                        case SUB_MENU_ERASE_QUIT:
                            gSubMenuSelection = SUB_MENU_OPTION_ERASE_ALL_DATA;
                            play_sound2(SOUND_MENU_GO_BACK);
                            break;
                        case SUB_MENU_ERASE_ERASE:
                            gSubMenuSelection = SUB_MENU_SAVE_DATA_ERASED;
                            func_800B46D0();
                            D_800DC5AC = 0;
                            play_sound2(SOUND_MENU_EXPLOSION);
                            break;
                    }
                }
                break; // or return?
            }
            case SUB_MENU_SAVE_DATA_ERASED: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    gSubMenuSelection = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
            case SUB_MENU_COPY_PAK_FROM_GHOST2_1P: {
                if ((btnAndStick & D_JPAD) && (gSubMenuSelection < SUB_MENU_COPY_PAK_FROM_GHOST_MAX) &&
                    (sp30[PLAYER_TWO].ghostDataSaved)) {
                    gSubMenuSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = 1;
                }
                if ((btnAndStick & U_JPAD) && (gSubMenuSelection > SUB_MENU_COPY_PAK_FROM_GHOST_MIN) &&
                    sp30[PLAYER_ONE].ghostDataSaved) {
                    gSubMenuSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    gSubMenuSelection = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    sp38->param2 = gSubMenuSelection - SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    if (sp30[sp38->param2].trackIndex == D_8018EE10[PLAYER_TWO].trackIndex &&
                        D_8018EE10[PLAYER_TWO].ghostDataSaved) {
                        gSubMenuSelection = SUB_MENU_COPY_PAK_TO_GHOST2_2P;
                    } else {
                        gSubMenuSelection = SUB_MENU_COPY_PAK_TO_GHOST1_2P;
                    }
                    play_sound2(SOUND_MENU_SELECT);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
            case SUB_MENU_COPY_PAK_TO_GHOST2_2P: {
                // bit of a fake match, but if it works it works?
                if ((sp30[sp38->param2].trackIndex !=
                     ((0, (D_8018EE10 + (gSubMenuSelection - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->trackIndex))) ||
                    ((D_8018EE10 + (gSubMenuSelection - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->ghostDataSaved == 0)) {
                    if ((btnAndStick & D_JPAD) && (gSubMenuSelection < SUB_MENU_COPY_PAK_TO_GHOST_MAX)) {
                        gSubMenuSelection += 1;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->paramf < 4.2) {
                            sp38->paramf += 4.0;
                        }
                        sp38->subState = 1;
                    }
                    if ((btnAndStick & U_JPAD) && (gSubMenuSelection > SUB_MENU_COPY_PAK_TO_GHOST_MIN)) {
                        gSubMenuSelection -= 1;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->paramf < 4.2) {
                            sp38->paramf += 4.0;
                        }
                        sp38->subState = -1;
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    gSubMenuSelection = sp38->param2 + SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    sp38->param1 = gSubMenuSelection - SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    if (D_8018EE10[(sp38->param1)].ghostDataSaved) {
                        gSubMenuSelection = SUB_MENU_COPY_PAK_PROMPT_QUIT;
                    } else {
                        gSubMenuSelection = SUB_MENU_COPY_PAK_START;
                        sp38->state = 0;
                    }
                    play_sound2(SOUND_MENU_SELECT);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA:
            case SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P:
            case SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P:
            case SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P:
            case SUB_MENU_COPY_PAK_COMPLETED:
            case SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P:
            case SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    gSubMenuSelection = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    play_sound2(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_PROMPT_QUIT:
            case SUB_MENU_COPY_PAK_PROMPT_COPY: {
                if ((btnAndStick & R_JPAD) && gSubMenuSelection < SUB_MENU_COPY_PAK_PROMPT_MAX) {
                    gSubMenuSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = 1;
                }
                if ((btnAndStick & L_JPAD) && gSubMenuSelection > SUB_MENU_COPY_PAK_PROMPT_MIN) {
                    gSubMenuSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->paramf < 4.2) {
                        sp38->paramf += 4.0;
                    }
                    sp38->subState = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    gSubMenuSelection = sp38->param1 + SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    if (gSubMenuSelection == SUB_MENU_COPY_PAK_PROMPT_QUIT) {
                        gSubMenuSelection = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        gSubMenuSelection = SUB_MENU_COPY_PAK_START;
                        play_sound2(SOUND_MENU_SELECT);
                        sp38->state = 0;
                    }
                }
                // return?
                break;
            }
            case SUB_MENU_COPY_PAK_START: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->state += 1;
                }
                if (sp38->state >= 3) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_COPYING;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_COPYING: {
                res = controller_pak_2_status();
                if (res == PFS_NO_ERROR) {
                    res = func_800B65F4(sp38->param2, sp38->param1);
                }
                if (res != 0) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                res = osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                    (u8*) gExtCode, &gControllerPak1FileNote);
                if (res == PFS_NO_ERROR) {
                    res = func_800B6178(sp38->param1);
                }
                if (res != 0) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                gSubMenuSelection = SUB_MENU_COPY_PAK_COMPLETED;
                D_8018EE10[sp38->param1].trackIndex = (sp30 + sp38->param2)->trackIndex;
                func_800B6088(sp38->param1);
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->state += 1;
                }
                if (sp38->state >= 3) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE: {
                if (func_800B6A68()) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                } else if (sp30[0].ghostDataSaved) {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                } else {
                    gSubMenuSelection = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
                }
                break;
            }
            default:
                break;
        }
    }
}

/**
 * Navigation of the data menu
 */
void data_menu_act(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->buttonPressed | controller->stickPressed);

    // Make pressing Start have the same effect as pressing A
    if ((gEnableDebugMode == 0) && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (is_screen_being_faded() == 0) {
        if (gSubMenuSelection == SUB_MENU_DATA) {
            // If DPad/Stick down pressed, move selection down if not already in bottom row
            if ((btnAndStick & D_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex % 4) != 3) {
                    ++gTimeTrialDataCourseIndex;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick up pressed, move selection up if not already in top row
            if ((btnAndStick & U_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex % 4) != 0) {
                    --gTimeTrialDataCourseIndex;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick right pressed, move selection right if not already in right-most column
            if ((btnAndStick & R_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 3) {
                    gTimeTrialDataCourseIndex += 4;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick left pressed, move selection left if not already in left-most column
            if ((btnAndStick & L_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 0) {
                    gTimeTrialDataCourseIndex -= 4;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If B pressed, go to main menu
            if ((btnAndStick & B_BUTTON) != 0) {
                func_8009E258();
                play_sound2(SOUND_MENU_GO_BACK);
                return;
            }
            // If A pressed, go to selected track's records
            if ((btnAndStick & A_BUTTON) != 0) {
                gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                func_8009E1C0();
                play_sound2(SOUND_MENU_OK_CLICKED);
            }
        }
        // If gSubMenuSelection is not SUB_MENU_DATA and A pressed, go to main menu
        // This condition is not reachable but this failsafe was added nonetheless
        else if ((btnAndStick & A_BUTTON) != 0) {
            func_8009E258();
            play_sound2(SOUND_MENU_OK_CLICKED);
        }
    }
}

/**
 * Navigation of the track records data menu
 */
void course_data_menu_act(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick; // sp2E
    MenuItem* sp28;
    CourseTimeTrialRecords* sp24;
    s32 res;

    btnAndStick = (controller->buttonPressed | controller->stickPressed);

    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!is_screen_being_faded()) {
        switch (gSubMenuSelection) {
            case SUB_MENU_DATA_OPTIONS: {
                if ((btnAndStick & L_JPAD) && (gTimeTrialDataCourseIndex > 0)) {
                    gTimeTrialDataCourseIndex -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                if ((btnAndStick & R_JPAD) && (gTimeTrialDataCourseIndex < 15)) {
                    gTimeTrialDataCourseIndex += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                sp28 = find_menu_items_dupe(0xE8);
                sp24 = &gSaveData.allCourseTimeTrialRecords.cupRecords[gTimeTrialDataCourseIndex / 4]
                            .courseRecords[gTimeTrialDataCourseIndex % 4];
                if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                    func_800B639C(gTimeTrialDataCourseIndex) < 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS && sp24->unknownBytes[0] == 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if ((btnAndStick & U_JPAD) && (gCourseRecordsMenuSelection > COURSE_RECORDS_MENU_MIN)) {
                    gCourseRecordsMenuSelection -= 1;
                    if (gCourseRecordsMenuSelection == 1 && sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection -= 1;
                    }
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->paramf < 4.2) {
                        sp28->paramf += 4.0;
                    }
                    sp28->subState = -1;
                }

                if ((btnAndStick & D_JPAD) && (gCourseRecordsMenuSelection < COURSE_RECORDS_MENU_MAX)) {
                    gCourseRecordsMenuSelection += 1;
                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS &&
                        sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection += 1;
                    }

                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                        func_800B639C(gTimeTrialDataCourseIndex) < 0) {
                        if (sp24->unknownBytes[0] == 0) {
                            gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                        } else {
                            gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_ERASE_RECORDS;
                        }
                    } else {
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp28->paramf < 4.2) {
                            sp28->paramf += 4.0;
                        }
                        sp28->subState = 1;
                    }
                }

                if (btnAndStick & B_BUTTON) {
                    func_8009E208();
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (sp28->paramf < 4.2) {
                        sp28->paramf += 4.0;
                    }
                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_RETURN_MENU) {
                        func_8009E208();
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        gSubMenuSelection = SUB_MENU_DATA_ERASE_CONFIRM;
                        gCourseRecordsSubMenuSelection = COURSE_RECORDS_SUB_MENU_QUIT;
                        play_sound2(SOUND_MENU_SELECT);
                    }
                }
                break;
            }
            case SUB_MENU_DATA_ERASE_CONFIRM: {
                sp28 = find_menu_items_dupe(0xE9);
                if ((btnAndStick & U_JPAD) && (gCourseRecordsSubMenuSelection > COURSE_RECORDS_SUB_MENU_MIN)) {
                    gCourseRecordsSubMenuSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->paramf < 4.2) {
                        sp28->paramf += 4.0;
                    }
                    sp28->subState = -1;
                }

                if ((btnAndStick & D_JPAD) && (gCourseRecordsSubMenuSelection < COURSE_RECORDS_SUB_MENU_MAX)) {
                    gCourseRecordsSubMenuSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->paramf < 4.2) {
                        sp28->paramf += 4.0;
                    }
                    sp28->subState = 1;
                }

                if (btnAndStick & B_BUTTON) {
                    gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (gCourseRecordsSubMenuSelection != COURSE_RECORDS_SUB_MENU_QUIT) {
                        res = 0;
                        switch (gCourseRecordsMenuSelection) {
                            case COURSE_RECORDS_MENU_ERASE_RECORDS: {
                                func_800B4728(gTimeTrialDataCourseIndex);
                                func_800B559C(gTimeTrialDataCourseIndex);
                                play_sound2(SOUND_MENU_EXPLOSION);
                                res = -1;
                                break;
                            }
                            case COURSE_RECORDS_MENU_ERASE_GHOST: {
                                res = func_800B639C(gTimeTrialDataCourseIndex);
                                if (res >= 0) {
                                    if (func_800B69BC(res) != 0) {
                                        gSubMenuSelection = SUB_MENU_DATA_CANT_ERASE;
                                        play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    } else {
                                        play_sound2(SOUND_MENU_EXPLOSION);
                                        gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
                                    }
                                }
                                break;
                            }
                        }

                        if (!(res + 1)) {
                            gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
                        }
                    } else {
                        play_sound2(SOUND_MENU_GO_BACK);
                        gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
                    }
                }
                break;
            }
            case SUB_MENU_DATA_CANT_ERASE: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
                }
                break;
            }
        }
    }
}

/**
 * On input skip logo screen
 **/
void logo_intro_menu_act(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->buttonPressed | controller->stickPressed);

    // If any button is pressed then fade audio out
    if ((is_screen_being_faded() == 0) && (btnAndStick)) {
        //! @todo Label audio funcs
        func_800CA388(0x3C);

        func_8009E1E4();
    }
}

/**
 * Navigation of the controller pak table data
 */
void controller_pak_menu_act(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick;
    OSPfsState* osPfsState;
    s32 selectedTableRow;
    UNUSED s8 pad;

    btnAndStick = (controller->buttonPressed | controller->stickPressed);
    if (is_screen_being_faded() == 0) {
        switch (gControllerPakMenuSelection) {
            case CONTROLLER_PAK_MENU_SELECT_RECORD:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_END;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_END:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    play_sound2(SOUND_MENU_SELECT);
                    func_8009E1C0();
                    gControllerPak1State = BAD;
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_TABLE_GAME_DATA:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    selectedTableRow = gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] - 1;
                    if (pfsError[selectedTableRow] == 0) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                        play_sound2(SOUND_MENU_SELECT);
                        return;
                    }
                } else if ((btnAndStick & B_BUTTON) != 0) {
                    if (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                        play_sound2(SOUND_MENU_GO_BACK);
                        return;
                    }
                } else if ((btnAndStick & U_JPAD) != 0) {
                    if (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        --gControllerPakSelectedTableRow;
                        if (gControllerPakSelectedTableRow < 0) {
                            gControllerPakSelectedTableRow = 0;
                            if (gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] != 1) {
                                gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_UP;
                                play_sound2(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }
                        } else {
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    }
                } else if (((btnAndStick & D_JPAD) != 0) &&
                           (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE)) {
                    ++gControllerPakSelectedTableRow;
                    if (gControllerPakSelectedTableRow >= CONTROLLER_PAK_MENU_TABLE_GAME_DATA) {
                        gControllerPakSelectedTableRow = CONTROLLER_PAK_MENU_QUIT;
                        if (gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] != 16) {
                            gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_DOWN;
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    } else {
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        return;
                    }
                }
                break;
            case CONTROLLER_PAK_MENU_QUIT:
                if ((btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_GO_TO_ERASING;
                    play_sound2(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & B_BUTTON) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_GO_TO_ERASING:
                gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASING;
                return;
            case CONTROLLER_PAK_MENU_ERASING:
                selectedTableRow = gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] - 1;
                osPfsState = &pfsState[selectedTableRow];

                switch (osPfsDeleteFile(&gControllerPak1FileHandle, osPfsState->company_code, osPfsState->game_code,
                                        (u8*) &osPfsState->game_name, (u8*) &osPfsState->ext_name)) {
                    default:
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED;
                        return;
                    case 0:
                        pfsError[selectedTableRow] = -1;
                        gControllerPak1NumPagesFree += (((osPfsState->file_size + 0xFF) >> 8) & 0xFF);
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                        return;
                    case PFS_ERR_NOPACK:
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK;
                        return;
                    case PFS_ERR_NEW_PACK:
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED;
                        return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                }
                break;
        }
    }
}

/**
 * Navigation of the main splash start screen menu
 * Also handles debug menu options
 */
void splash_menu_act(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick;
    u16 i;
    s32 isDebug = true;
    btnAndStick = controller->buttonPressed | controller->stickPressed;

    if (is_screen_being_faded() == 0) {
        if (controllerIdx == PLAYER_ONE) {
            gMenuDelayTimer += 1;
        }
        switch (gDebugMenuSelection) {
            case DEBUG_MENU_DISABLED: {
                isDebug = false;
                if ((gMenuDelayTimer >= 46) && (btnAndStick & (A_BUTTON | START_BUTTON))) {
                    func_8009E1C0();
                    func_800CA330(0x19);
                    play_sound2(SOUND_INTRO_ENTER_MENU);
                } else {
                    break;
                }
                break;
            }
            case DEBUG_MENU_DEBUG_MODE: {
                if (btnAndStick & (R_JPAD | L_JPAD)) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (gEnableDebugMode) {
                        gEnableDebugMode = CVarGetInteger("gEnableDebugMode", 0);
                    } else {
                        gEnableDebugMode = true;
                    }
                }
                if (btnAndStick & D_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gDebugMenuSelection = DEBUG_MENU_COURSE;
                }
                break;
            }
            case DEBUG_MENU_COURSE: {
                if (btnAndStick & R_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    TrackBrowser_NextTrack();
                    gCurrentCourseId = TrackBrowser_GetTrackIndex();
                    // if (gCurrentCourseId < (NUM_TRACKS - 2)) {
                    //     gCurrentCourseId += 1;
                    // } else {
                    //     gCurrentCourseId = 0;
                    // }
                }
                if (btnAndStick & L_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    TrackBrowser_PreviousTrack();
                    gCurrentCourseId = TrackBrowser_GetTrackIndex();
                    // if (gCurrentCourseId > 0) {
                    //     gCurrentCourseId -= 1;
                    // } else {
                    //     gCurrentCourseId = (NUM_TRACKS - 2);
                    // }
                }
                if (btnAndStick & U_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gDebugMenuSelection = DEBUG_MENU_DEBUG_MODE;
                }
                if (btnAndStick & D_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gDebugMenuSelection = DEBUG_MENU_CC;
                }
                break;
            }
            case DEBUG_MENU_CC: {
                if ((btnAndStick & R_JPAD) && (gCCSelection < 3)) {
                    gCCSelection += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (gCCSelection > 0)) {
                    gCCSelection -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_COURSE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_SCREEN_MODE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_SCREEN_MODE: {
                if ((btnAndStick & R_JPAD) && (gScreenModeListIndex < 4)) {
                    gScreenModeListIndex += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gScreenModeSelection = sScreenModePlayerTable[gScreenModeListIndex];
                }
                if ((btnAndStick & L_JPAD) && (gScreenModeListIndex > 0)) {
                    gScreenModeListIndex -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gScreenModeSelection = sScreenModePlayerTable[gScreenModeListIndex];
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_CC;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_PLAYER;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_PLAYER: {
                if ((btnAndStick & R_JPAD) && (gCharacterSelections[0] < 7)) {
                    gCharacterSelections[0] += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (gCharacterSelections[0] > 0)) {
                    gCharacterSelections[0] -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_SCREEN_MODE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_SOUND_MODE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_SOUND_MODE: {
                if ((btnAndStick & R_JPAD) && (gSoundMode < 3)) {
                    gSoundMode += 1;
                    if (gSoundMode == SOUND_UNUSED) {
                        gSoundMode = SOUND_MONO;
                    }
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    set_sound_mode();
                    gSaveData.main.saveInfo.soundMode = gSoundMode;
                    write_save_data_grand_prix_points_and_sound_mode();
                    update_save_data_backup();
                }
                if ((btnAndStick & L_JPAD) && (gSoundMode > 0)) {
                    gSoundMode -= 1;
                    if (gSoundMode == SOUND_UNUSED) {
                        gSoundMode = SOUND_HEADPHONES;
                    }
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    set_sound_mode();
                    gSaveData.main.saveInfo.soundMode = gSoundMode;
                    write_save_data_grand_prix_points_and_sound_mode();
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_PLAYER;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_LAUNCH_EDITOR;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_LAUNCH_EDITOR: {
                if (btnAndStick & (A_BUTTON | START_BUTTON)) {
                    Editor_Launch("mk:test_track");
                    play_sound2(SOUND_INTRO_ENTER_MENU);
                }

                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_SOUND_MODE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_GIVE_ALL_GOLD_CUP;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP: {
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_LAUNCH_EDITOR;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & B_BUTTON) {
                    for (i = 0; i < 16; i++) {
                        func_800B5404(0, i);
                    }
                    play_sound2(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & CONT_L) {
                    reset_save_data_grand_prix_points_and_sound_mode();
                    for (i = 0; i < 16; i++) {
                        func_800B5404(i / 4, i);
                    }
                    play_sound2(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & L_JPAD) {
                    reset_save_data_grand_prix_points_and_sound_mode();
                    for (i = 0; i < 16; i++) {
                        if (i % 4 == 2) {
                            func_800B5404(0, i);
                        } else {
                            func_800B5404(i / 4, i);
                        }
                    }
                    play_sound2(SOUND_MENU_SELECT);
                } else {
                    break;
                }
                break;
            }
            default:
                break;
        }

        gPlayerCountSelection1 = gPlayerCount = sScreenModePlayerCount[gScreenModeListIndex];

        if (isDebug) {
            if (btnAndStick & (A_BUTTON | START_BUTTON)) {
                func_8009E1C0();
                func_800CA330(0x19);
                gDebugMenuSelection = DEBUG_MENU_OPTION_SELECTED;

                if (controller->button & CONT_L) {
                    gDemoMode = DEMO_MODE_ACTIVE;
                } else {
                    gDemoMode = DEMO_MODE_INACTIVE;
                }

                if (controller->button & Z_TRIG) {
                    if (btnAndStick & A_BUTTON) {
                        gDebugGotoScene = DEBUG_GOTO_ENDING;
                    } else {
                        gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA;
                    }
                }
                play_sound2(SOUND_MENU_OK_CLICKED);
            } else if ((btnAndStick & B_BUTTON) && (controller->button & Z_TRIG)) {
                func_8009E1C0();
                func_800CA330(0x19);
                gDebugMenuSelection = DEBUG_MENU_OPTION_SELECTED;
                gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT;
                play_sound2(SOUND_MENU_OK_CLICKED);
            } else if (btnAndStick & CONT_R) {
                gDebugMenuSelection = DEBUG_MENU_DISABLED;
                play_sound2(SOUND_MENU_SELECT);
            }
        }
    }
}

void setup_game_mode_selected(void) {
    // For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
    // be whether 'Begin' or 'Data' is selected. Not used for Battle.
    s8 subMenuMode = gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
    // Determine which game mode was selected based on the number of players and the row selected on the main menu
    switch (gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]) {
        case GRAND_PRIX:
            gCCSelection = subMenuMode;
            gPlaceItemBoxes = 1;
            set_mirror_mode((subMenuMode == CC_EXTRA) ? 1 : 0);
            break;
        case VERSUS:
            gCCSelection = subMenuMode;
            gPlaceItemBoxes = 1;
            set_mirror_mode((subMenuMode == CC_EXTRA) ? 1 : 0);
            break;
        case BATTLE:
            gPlaceItemBoxes = 1;
            set_mirror_mode(0);
            break;
        case TIME_TRIALS:
            gCCSelection = CC_100;
            set_mirror_mode(0);
            gPlaceItemBoxes = 0;

            if ((subMenuMode && subMenuMode) && subMenuMode) {}

            break;
    }
}

/**
 * Navigation of the main game mode select screen
 */
#ifdef NON_MATCHING
// https://decomp.me/scratch/93qj8
// nonmatching: regalloc; controllerIdx is not AND-ed back into $a1, reg chaos follows
void main_menu_act(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick;
    s32 subMode;
    bool cursorMoved;
    s32 newMode;

    btnAndStick = controller->buttonPressed | controller->stickPressed;
    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!is_screen_being_faded()) {
        switch (gMainMenuSelection) {
            case MAIN_MENU_NONE: {
                newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                break;
            }
            case MAIN_MENU_PLAYER_SELECT: {
                if ((btnAndStick & R_JPAD) && gPlayerCount < 4) {
                    gPlayerCount += 1;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && gPlayerCount >= 2) {
                    gPlayerCount -= 1;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                // L800B2B38
                gPlayerCountSelection1 = gPlayerCount;
                switch (gPlayerCountSelection1) {
                    case 1:
                        gScreenModeSelection = SCREEN_MODE_1P;
                        break;
                    case 2:
                        gScreenModeSelection = SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL;
                        break;
                    case 3:
                    case 4:
                        gScreenModeSelection = SCREEN_MODE_3P_4P_SPLITSCREEN;
                        break;
                }
                // L800B2B94
                if (btnAndStick & B_BUTTON) {
                    func_8009E0F0(0x14);
                    func_800CA330(0x19);
                    gMenuFadeType = MENU_FADE_TYPE_BACK;
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B2C00
                    gMainMenuSelection = MAIN_MENU_MODE_SELECT;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_SELECT);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & CONT_L) {
                    // L800B2C58
                    gMainMenuSelection = MAIN_MENU_OPTION;
                    func_8009E280();
                    play_sound2(SOUND_MENU_OPTION);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & CONT_R) {
                    gMainMenuSelection = MAIN_MENU_DATA;
                    func_8009E258();
                    play_sound2(SOUND_MENU_DATA);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else {
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                }
                break;
            }
            case MAIN_MENU_MODE_SELECT: {
                if (btnAndStick & D_JPAD) {
                    if (gGameModeMenuColumn[gPlayerCount - 1] < gPlayerModeSelection[gPlayerCount - 1]) {
                        gGameModeMenuColumn[gPlayerCount - 1] += 1;
                        reset_cycle_flash_menu();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B2D94
                if (btnAndStick & U_JPAD) {
                    if (gGameModeMenuColumn[gPlayerCount - 1] > 0) {
                        gGameModeMenuColumn[gPlayerCount - 1] -= 1;
                        reset_cycle_flash_menu();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B2DE0
                if (btnAndStick & B_BUTTON) {
                    gMainMenuSelection = MAIN_MENU_PLAYER_SELECT;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B2E3C
                    switch (gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]) {
                        case 0:
                            gMainMenuSelection = MAIN_MENU_MODE_SUB_SELECT;
                            play_sound2(SOUND_MENU_GP);
                            break;
                        case 2:
                            gMainMenuSelection = MAIN_MENU_MODE_SUB_SELECT;
                            play_sound2(SOUND_MENU_VERSUS);
                            break;
                        case 1:
                            gMainMenuSelection = MAIN_MENU_MODE_SUB_SELECT;
                            play_sound2(SOUND_MENU_TIME_TRIALS);
                            break;
                        case 3:
                            gMainMenuSelection = MAIN_MENU_OK_SELECT;
                            play_sound2(SOUND_MENU_BATTLE);
                            break;
                        default:
                            gMainMenuSelection = MAIN_MENU_OK_SELECT;
                            break;
                    }
                    // L800B2F04
                    reset_cycle_flash_menu();
                    gMenuTimingCounter = 0;
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else {
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                }
                break;
            }
            case MAIN_MENU_MODE_SUB_SELECT:
            case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: {
                if (controllerIdx == PLAYER_ONE) {
                    gMenuTimingCounter++;
                    if ((gMenuTimingCounter == 100 || gMenuTimingCounter % 300 == 0)) {
                        // L800B2FAC
                        if (gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]] == 0 ||
                            gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]] == 2) {
                            play_sound2(SOUND_MENU_SELECT_LEVEL);
                        }
                    }
                }
                // L800B3000
                subMode = gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                if ((btnAndStick & U_JPAD) && (subMode > 0)) {
                    gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]] -= 1;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                // L800B3068
                if (btnAndStick & D_JPAD) {
                    cursorMoved = false;
                    if (has_unlocked_extra_mode()) {
                        if (subMode <
                            sGameModePlayerColumnExtra[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]) {
                            cursorMoved = true;
                        }
                    } else {
                        if (subMode <
                            sGameModePlayerColumnDefault[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]) {
                            cursorMoved = true;
                        }
                    }
                    // L800B3110
                    if (cursorMoved) {
                        gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]++;
                        reset_cycle_flash_menu();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B3150
                subMode = gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                if (btnAndStick & B_BUTTON) {
                    gMainMenuSelection = MAIN_MENU_MODE_SELECT;
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B31DC
                    reset_cycle_flash_menu();
                    if (gPlayerCount == 1 && gGameModeMenuColumn[gPlayerCount - 1] == 1 && subMode == 1) { // DATA
                        func_8009E258();
                        play_sound2(SOUND_MENU_DATA);
                    } else { // BEGIN
                        gMainMenuSelection = MAIN_MENU_OK_SELECT;
                        play_sound2(SOUND_MENU_SELECT);
                        gMenuTimingCounter = 0;
                    }
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else {
                    // L800B3294
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                }
                break;
            }
            case MAIN_MENU_OK_SELECT:
            case MAIN_MENU_OK_SELECT_GO_BACK: {
                if ((controllerIdx == PLAYER_ONE) && (++gMenuTimingCounter == 60 || gMenuTimingCounter % 300 == 0)) {
                    play_sound2(SOUND_MENU_OK);
                }
                // L800B330C
                if (btnAndStick & B_BUTTON) {
                    switch (gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]]) {
                        case 0:
                        case 1:
                        case 2:
                            gMainMenuSelection = MAIN_MENU_MODE_SUB_SELECT;
                            break;
                        case 3:
                        default:
                            gMainMenuSelection = MAIN_MENU_MODE_SELECT;
                            break;
                    }
                    // L800B3384
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_GO_BACK);
                    gMenuTimingCounter = 0;
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B33D8
                    func_8009E1C0();
                    play_sound2(SOUND_MENU_OK_CLICKED);
                    setup_game_mode_selected();
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                } else {
                    newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                }
                break;
            }
            case MAIN_MENU_OPTION:
            case MAIN_MENU_DATA: {
                newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                break;
            }
            default: {
                newMode = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                break;
            }
        }
        gModeSelection = newMode;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/menus/main_menu_act.s")
#endif

/**
 * Check if there is no currently selected and/or
 * hovered character at grid position `gridId`
 */
bool is_character_spot_free(s32 gridId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(gCharacterGridSelections); i++) {
        if (gridId == gCharacterGridSelections[i]) {
            return false;
        }
    }
    return true;
}

// Grid positions are from right to left, then top to bottom
// https://decomp.me/scratch/6R4jX
#if 1
/**
 * Navigation of the player select screen
 * Grid positions are from right to left, then top to bottom
 */
void player_select_menu_act(struct Controller* controller, u16 controllerIdx) {
    s8* bar;
    s8 selected;
    s8 i;
    s8 savedSelection;
    u16 btnAndStick;

    btnAndStick = (controller->buttonPressed) | (controller->stickPressed);
    if (!gEnableDebugMode && btnAndStick & CONT_START) {
        btnAndStick |= A_BUTTON;
    }

    if (!is_screen_being_faded()) {
        switch (gPlayerSelectMenuSelection) {
            case PLAYER_SELECT_MENU_MAIN: {
                savedSelection = gCharacterGridSelections[controllerIdx];
                if (savedSelection == 0) {
                    if (btnAndStick & B_BUTTON) {
                        func_8009E208();
                        play_sound2(0x49008002);
                    }
                    return;
                }
                // L800B3630
                if (btnAndStick & B_BUTTON) {
                    if (gCharacterGridIsSelected[controllerIdx]) {
                        gCharacterGridIsSelected[controllerIdx] = false;
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        func_8009E208();
                        play_sound2(0x49008002);
                    }
                }
                // L800B3684
                if ((btnAndStick & A_BUTTON) && (gCharacterGridIsSelected[controllerIdx] == 0)) {
                    gCharacterGridIsSelected[controllerIdx] = true;
                    i = sCharacterGridOrder[gCharacterGridSelections[controllerIdx] - 1];
                    func_800C90F4(controllerIdx, 0x2900800e + (i << 4));
                }
                // L800B36F4
                selected = false;
                for (i = 0; i < ARRAY_COUNT(gCharacterGridSelections); i++) {
                    if ((gCharacterGridSelections[i] != 0) && (gCharacterGridIsSelected[i] == 0)) {
                        selected = true;
                        break;
                    }
                }
                // L800B3738

                if (!selected) {
                    gPlayerSelectMenuSelection = PLAYER_SELECT_MENU_OK;
                    reset_cycle_flash_menu();
                    gMenuTimingCounter = 0;
                }

                // L800B3768
                if (gCharacterGridIsSelected[controllerIdx] == 0) {
                    if ((btnAndStick & CONT_RIGHT) && (btnAndStick & CONT_DOWN)) {
                        if (savedSelection == 1 || savedSelection == 2 || savedSelection == 3) {
                            // L800B37B0
                            savedSelection += 5;
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B37E4
                    if ((btnAndStick & CONT_LEFT) && (btnAndStick & CONT_DOWN)) {
                        if (savedSelection == 2 || savedSelection == 3 || savedSelection == 4) {
                            savedSelection += 3;
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B3844
                    if ((btnAndStick & CONT_RIGHT) && (btnAndStick & CONT_UP)) {
                        if (savedSelection == 5 || savedSelection == 6 || savedSelection == 7) {
                            savedSelection -= 3;
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B38A0
                    if ((btnAndStick & CONT_LEFT) && (btnAndStick & CONT_UP)) {
                        if (savedSelection == 6 || savedSelection == 7 || savedSelection == 8) {
                            savedSelection -= 5;
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B38FC
                    if (btnAndStick & CONT_RIGHT) {
                        if (savedSelection == 4 || savedSelection == 8)
                            return;
                        savedSelection += 1;
                        do {
                            // L800B391C
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000); // play_sound2(0x49008000);
                                break;
                            }
                            savedSelection += 1;
                            if ((savedSelection == 5) || (savedSelection == 9))
                                return;
                        } while (savedSelection < 10);
                        return;
                    }
                    // L800B3978
                    if (btnAndStick & CONT_LEFT) {
                        if (savedSelection == 1 || savedSelection == 5)
                            return;
                        savedSelection -= 1;
                        do {
                            if (is_character_spot_free(savedSelection)) {
                                gCharacterGridSelections[controllerIdx] = savedSelection;
                                play_sound2(0x49008000);
                                break;
                            }
                            savedSelection -= 1;
                            if ((savedSelection == 0) || (savedSelection == 4))
                                return;
                        } while (savedSelection >= 0);
                        return;
                    }
                    // L800B39F4
                    if ((btnAndStick & CONT_UP) && (savedSelection >= 5)) {
                        savedSelection = savedSelection - 4;
                    }
                    if ((btnAndStick & CONT_DOWN) && (savedSelection < 5)) {
                        savedSelection = savedSelection + 4;
                    }
                    // L800B3A30
                    if (is_character_spot_free(savedSelection)) {
                        gCharacterGridSelections[controllerIdx] = savedSelection;
                        play_sound2(0x49008000);
                    }
                }
                break;
            }
            case 2:
            case 3:
                if (controllerIdx == 0) {
                    gMenuTimingCounter++;
                    if ((gMenuTimingCounter == 60) || ((gMenuTimingCounter % 300) == 0)) {
                        // L800B3A94
                        play_sound2(0x4900900F);
                    }
                }
                // L800B3AA4
                if (btnAndStick & B_BUTTON) {
                    gPlayerSelectMenuSelection = PLAYER_SELECT_MENU_MAIN;
                    gCharacterGridIsSelected[controllerIdx] = false;
                    play_sound2(SOUND_MENU_GO_BACK);
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    func_8009E1C0();
                    play_sound2(0x49008016);
                    func_8000F124();
                }
                break;
            default:
                break;
        }
        // L800B3B24
        if (gCharacterGridSelections[controllerIdx] != 0) {
            gCharacterSelections[controllerIdx] = sCharacterGridOrder[gCharacterGridSelections[controllerIdx] - 1];
        }
    }
    // L800B3B44
}
#else
GLOBAL_ASM("asm/non_matchings/menus/player_select_menu_act.s")
#endif

u32 WorldNextCup(void);
u32 WorldPreviousCup(void);
u32 GetCupIndex(void);

/**
 * Navigation of the map select track menu screen
 */
void course_select_menu_act(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick = (controller->buttonPressed | controller->stickPressed);

    if ((!gEnableDebugMode) && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (is_screen_being_faded() == 0) {
        switch (gSubMenuSelection) {
            case SUB_MENU_MAP_SELECT_CUP:
                if ((btnAndStick & R_JPAD) != 0) {
                    sTempCupSelection = WorldNextCup();
                    //++gCupSelection;
                    // reset_cycle_flash_menu();
                    // play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (((btnAndStick & L_JPAD) != 0)) {
                    sTempCupSelection = WorldPreviousCup();
                    //--gCupSelection;
                    // reset_cycle_flash_menu();
                    // play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                D_800DC540 = GetCupIndex();
                gCurrentCourseId = gCupCourseOrder[gCupSelection][gCourseIndexInCup];
                TrackBrowser_SetTrackFromCup();
                if ((btnAndStick & B_BUTTON) != 0) {
                    func_8009E208();
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if ((btnAndStick & A_BUTTON) != 0) {
                    if (gModeSelection != GRAND_PRIX) {
                        gSubMenuSelection = SUB_MENU_MAP_SELECT_COURSE;
                        play_sound2(SOUND_MENU_SELECT);
                    } else {
                        gSubMenuSelection = SUB_MENU_MAP_SELECT_OK;
                        play_sound2(SOUND_MENU_SELECT);
                        SetCupCursorPosition(TRACK_ONE);
                        TrackBrowser_SetTrackFromCup();
                        gCurrentCourseId = gCupCourseOrder[gCupSelection][TRACK_ONE];
                        gMenuTimingCounter = 0;
                    }
                    reset_cycle_flash_menu();
                }
                break;
            case SUB_MENU_MAP_SELECT_COURSE:
            case SUB_MENU_MAP_SELECT_BATTLE_COURSE:
                if (((btnAndStick & D_JPAD) != 0) && (GetCupCursorPosition() < (GetCupSize() - 1))) {
                    ++gCourseIndexInCup;
                    SetCupCursorPosition(GetCupCursorPosition() + 1);
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (((btnAndStick & U_JPAD) != 0) && (GetCupCursorPosition() > TRACK_ONE)) {
                    --gCourseIndexInCup;
                    SetCupCursorPosition(GetCupCursorPosition() - 1);
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                gCurrentCourseId = gCupCourseOrder[gCupSelection][gCourseIndexInCup];
                TrackBrowser_SetTrackFromCup();
                if ((btnAndStick & B_BUTTON) != 0) {
                    if (gSubMenuSelection == SUB_MENU_MAP_SELECT_COURSE) {
                        gSubMenuSelection = SUB_MENU_MAP_SELECT_CUP;
                    } else {
                        func_8009E208();
                    }
                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    gSubMenuSelection = SUB_MENU_MAP_SELECT_OK;
                    play_sound2(SOUND_MENU_SELECT);
                    reset_cycle_flash_menu();
                    gMenuTimingCounter = 0;
                }
                break;
            case SUB_MENU_MAP_SELECT_OK:
                if ((controllerIdx == PLAYER_ONE) &&
                    ((++gMenuTimingCounter == 0x3C) || ((gMenuTimingCounter % 300) == 0))) {
                    play_sound2(SOUND_MENU_OK);
                }

                if ((btnAndStick & B_BUTTON) != 0) {
                    switch (gModeSelection) {
                        case GRAND_PRIX:
                            gSubMenuSelection = SUB_MENU_MAP_SELECT_CUP;
                            break;
                        case BATTLE:
                            gSubMenuSelection = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
                            break;
                        default:
                            gSubMenuSelection = SUB_MENU_MAP_SELECT_COURSE;
                            break;
                    }

                    reset_cycle_flash_menu();
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    func_8009E1C0();
                    func_800CA330(0x19);
                    play_sound2(SOUND_MENU_OK_CLICKED);
                }
                break;
        }
    }
}

/**
 * Loads menu states so they are preserved between menu changes
 */
void load_menu_states(s32 menuSelection) {
    s32 i;

    gDebugMenuSelection = CVarGetInteger("gEnableDebugMode", 0) + 1;
    gMenuTimingCounter = 0;
    gMenuDelayTimer = 0;
    gDemoUseController = 0;
    D_8015F890 = 0;
    D_8015F892 = 0;
    gDebugGotoScene = DEBUG_GOTO_RACING;
    gGhostPlayerInit = 0;
    D_8016556E = 0;
    bPlayerGhostDisabled = 1;
    D_80162DD8 = 1;
    D_80162E00 = 0;
    D_80162DC8 = 1;
    D_80162DCC = 0;

    switch (menuSelection) {
        case OPTIONS_MENU:
            gSubMenuSelection = SUB_MENU_OPTION_RETURN_GAME_SELECT;
            break;
        case DATA_MENU:
            gSubMenuSelection = SUB_MENU_DATA;
            break;
        case COURSE_DATA_MENU:
            gSubMenuSelection = SUB_MENU_DATA_OPTIONS;
            break;
        case HARBOUR_MASTERS_MENU:
            func_800CA008(0, 0);
            break;
        case LOGO_INTRO_MENU:
            func_800CA008(0, 0);
            break;
        case CONTROLLER_PAK_MENU: {
            gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
            func_800CA008(0, 0);
            break;
        }
        case 0:
        case START_MENU: {
            set_mirror_mode(0);
            gEnableDebugMode = CVarGetInteger("gEnableDebugMode", 0);
            CM_SetCup(GetMushroomCup());
            gCupSelection = MUSHROOM_CUP;
            gCourseIndexInCup = 0;
            gTimeTrialDataCourseIndex = 0;
            if (gPlayerCount <= 0) {
                gPlayerCount = 1;
            }
            if (gPlayerCount >= 5) {
                gPlayerCount = 4;
            }
            gScreenModeListIndex = sScreenModeIdxFromPlayerMode[gPlayerCount - 1];
            func_800CA008(0, 0);
            play_sequence(MUSIC_SEQ_TITLE_SCREEN);
            gTrackMapInit = 0;
            break;
        }
        case 1:
        case MAIN_MENU: {
            gEnableDebugMode = CVarGetInteger("gEnableDebugMode", 0);
            set_mirror_mode(0);
            gTrackMapInit = 0;
            func_800B5F30();
            func_8000F0E0();

            if (gGamestate != 0) {
                func_800CA008(0, 0);
                func_800CB2C4();
                gGamestate = 0;
                gGamestateNext = 0;
                play_sequence(MUSIC_SEQ_MAIN_MENU);
            }

            switch (gMenuFadeType) {
                case MENU_FADE_TYPE_MAIN: {
                    gMainMenuSelection = MAIN_MENU_PLAYER_SELECT;
                    play_sequence(MUSIC_SEQ_MAIN_MENU);
                    gPlayerCount = 1;
                    if (gScreenModeSelection >= NUM_SCREEN_MODES || gScreenModeSelection < 0) {
                        gScreenModeSelection = SCREEN_MODE_1P;
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    gMainMenuSelection = MAIN_MENU_OK_SELECT_GO_BACK;
                    break;
                }
                case MENU_FADE_TYPE_DATA: {
                    // why...
                    switch (gMainMenuSelection) {
                        default:
                            gMainMenuSelection = MAIN_MENU_MODE_SUB_SELECT_GO_BACK;
                            break;
                        case MAIN_MENU_OPTION:
                        case MAIN_MENU_DATA:
                            gMainMenuSelection = MAIN_MENU_PLAYER_SELECT;
                            break;
                    }
                    break;
                }
                case MENU_FADE_TYPE_OPTION: {
                    gMainMenuSelection = MAIN_MENU_PLAYER_SELECT;
                    break;
                }
            }
            break;
        }
        case 2:
        case CHARACTER_SELECT_MENU: {
            switch (gMenuFadeType) {
                case MENU_FADE_TYPE_MAIN: {
                    gPlayerSelectMenuSelection = PLAYER_SELECT_MENU_MAIN;
                    if (gGamestate == 0) {
                        for (i = 0; i < ARRAY_COUNT(gCharacterGridSelections); i++) {
                            if (i < gPlayerCount) {
                                gCharacterGridSelections[i] = i + 1;
                            } else {
                                gCharacterGridSelections[i] = 0;
                            }
                            gCharacterGridIsSelected[i] = false;
                            gCharacterSelections[i] = i;
                        }
                        play_sound2(SOUND_MENU_SELECT_PLAYER);
                    } else {
                        func_800CA008(0, 0);
                        func_800CB2C4();
                        gGamestate = 0;
                        gGamestateNext = 0;
                        play_sequence(MUSIC_SEQ_MAIN_MENU);
                        for (i = 0; i < ARRAY_COUNT(gCharacterGridIsSelected); i++) {
                            gCharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    gPlayerSelectMenuSelection = PLAYER_SELECT_MENU_OK_GO_BACK;
                    for (i = 0; i < ARRAY_COUNT(gCharacterGridIsSelected); i++) {
                        if (gPlayerCount > i) {
                            gCharacterGridIsSelected[i] = true;
                        } else {
                            gCharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 3:
        case COURSE_SELECT_MENU: {
            if (gModeSelection == BATTLE) {
                CM_SetCup(GetBattleCup());
                // gCupSelection = BATTLE_CUP;
                D_800DC540 = 4;
                CM_SetCupIndex(BATTLE_CUP);
                gSubMenuSelection = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
            } else {
                if (GetCup() == GetBattleCup()) {
                    CM_SetCup(GetMushroomCup());
                    CM_SetCupIndex(MUSHROOM_CUP);
                    // gCupSelection = MUSHROOM_CUP;
                }
                gSubMenuSelection = SUB_MENU_MAP_SELECT_CUP;
            }
            if (gGamestate != 0) {
                func_800CA008(0, 0);
                func_800CB2C4();
                gGamestate = 0;
                gGamestateNext = 0;
                play_sequence(MUSIC_SEQ_MAIN_MENU);
            }
            play_sound2(SOUND_MENU_SELECT_MAP);
            sTempCupSelection = 0;
            if (gModeSelection == GRAND_PRIX) {
                gCourseIndexInCup = 0;
            }

            for (i = 0; i < ARRAY_COUNT(gGPPointsByCharacterId); i++) {
                gGPPointsByCharacterId[i] = 0;
            }
            break;
        }
    }
    reset_cycle_flash_menu();
}

/**
 * Resets when moving cursor option or after a fade
 */
void reset_cycle_flash_menu(void) {
    gCycleFlashMenu = 0x20;
}

/**
 * Changes sound mode pack
 */
void set_sound_mode(void) {
    UNUSED u32 pad;
    union GameModePack pack;

    pack = sSoundMenuPack;
    if ((gSoundMode == SOUND_STEREO) || (gSoundMode == SOUND_HEADPHONES) || (gSoundMode == SOUND_MONO)) {
        func_800C3448(pack.modes[gSoundMode] | 0xE0000000);
    }
}

/**
 * Checks is a fade render mode is active so menus can't be
 * interacted while a fade transition is active
 */
bool is_screen_being_faded(void) {
    if ((gTransitionType[4] == 2) || (gTransitionType[4] == 3) || (gTransitionType[4] == 4) ||
        (gTransitionType[4] == 7)) {
        return true;
    }
    return false;
}

/**
 * Unused debug function, prints the character id for the player and both controller pak ghosts
 */
UNUSED void debug_print_ghost_kart_character_id(s32 arg0, s32 arg1) {
    struct_8018EE10_entry* pak1 = D_8018EE10;
    struct_8018EE10_entry* pak2 = (struct_8018EE10_entry*) gSomeDLBuffer;

    rmonPrintf("ghost_kart=%d,", D_80162DE0);
    rmonPrintf("pak1_ghost_kart=%d,", (pak1 + arg0)->characterId);
    rmonPrintf("pak2_ghost_kart=%d\n", (pak2 + arg1)->characterId);
}
