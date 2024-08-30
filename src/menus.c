#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include <mk64.h>
#include <stubs.h>

#include "menus.h"
#include "main.h"
#include "code_800029B0.h"
#include "actors.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "code_80091750.h"
#include "code_800AF9B0.h"
#include "save.h"
#include "staff_ghosts.h"
#include "save_data.h"
#include <sounds.h>
#include "spawn_players.h"

/** BSS **/
s32 D_8018EDC0;
f32 D_8018EDC4;
f32 D_8018EDC8;
f32 D_8018EDCC;
f32 D_8018EDD0;
f32 D_8018EDD4;
f32 D_8018EDD8;
f32 D_8018EDDC;
s32 D_8018EDE0;
s8 gCharacterGridSelections[4]; // map from player id to current grid position
s8 D_8018EDE8[4];               // map player id to isCharSelected on CSS
s8 D_8018EDEC;
s8 gMainMenuSelectionDepth;
s8 D_8018EDEE; // grid screen state?
s8 gDebugMenuSelection;
s8 gControllerPakMenuSelection;
s8 D_8018EDF1;
u8 gSoundMode;
s8 gPlayerCount; // main menu row id? but it is also the number of players define
s8 D_8018EDF4;
s8 D_8018EDF5;
s8 D_8018EDF6;
s8 gTimeTrialDataCourseIndex;
s8 gCourseRecordsMenuSelection;
s8 D_8018EDF9;
s8 gDebugGotoScene;
s8 D_8018EDFB;
s8 D_8018EDFC;
s32 gMenuTimingCounter;
s32 gMenuDelayTimer;
s8 D_8018EE08;
s8 gCupSelection;
s8 D_8018EE0A;
s8 gCupCourseSelection;
s8 D_8018EE0C;
struct_8018EE10_entry D_8018EE10[2];

/** Data **/
s32 gMenuSelection = START_MENU;
s32 D_800E86A4 = 0;
s8 gCharacterSelections[4] = { MARIO, LUIGI, YOSHI, TOAD };

// gGameModeRowSelectionForNumPlayers is (D_800E86AC - 1)
// gGameModeSubMenuRowSelectionForNumPlayers (D_800E86AC + 1)

// the current row selected in column i on main menu
s8 D_800E86AC[4] = { 0, 0, 0, 0 };
// For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
// be whether 'Begin' or 'Data' is selected. Not used for Battle.
// indexed as [column][row]
s8 D_800E86B0[4][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

s8 gNextDemoId = 0;
s8 gControllerPakSelectedTableRow = 0;
s8 D_800E86C4[12] = { 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0 };
s8 D_800E86D0[16] = { 0 }; // this doesn't make a lot of sense...
s8 unref_800E86E0[4] = { 0, 0, 0, 1 };

u32 sVIGammaOffDitherOn = OS_VI_GAMMA_OFF | OS_VI_DITHER_FILTER_ON;

/*** Const/rodata Data ***/
// used to set gScreenModeSelection; might be smaller; could be function static data
const s8 D_800F2B50[] = { 0, 1, 2, 3, 3 };

// set to gPlayerCount, then that sets gPlayerCountSelection1
const s8 D_800F2B58[] = { 1, 2, 2, 3, 4 };

// Limit for each column in one-two-three-four players selection
const s8 D_800F2B60[5][3] = {
    { 1, 2, 1 }, { 1, 2, 1 }, { 0, 2, 2 }, { 0, 2, 0 }, { 0, 2, 0 },
    // {0, 3, 1},
    // {0, 3, 3},
    // {0, 3, 0},
    // {0, 3, 0},
};

// is this another union GameModePack? Figure out when decomping.
const s32 gGameModeFromNumPlayersAndRowSelection[5][3] = {
    { 0x03010003, 0x03000300, 0x00030000 },  // Despite this matching, there is no way this line belongs in this array
    { GRAND_PRIX, TIME_TRIALS, 0x00000000 }, // first column
    { GRAND_PRIX, VERSUS, BATTLE },          // second
    { VERSUS, BATTLE, 0x00000000 },          // third
    { VERSUS, BATTLE, 0x00000000 },          // four
};

// map from character grid position id to character id
const s8 D_800F2BAC[] = {
    MARIO, LUIGI, PEACH, TOAD, YOSHI, DK, WARIO, BOWSER,
};

const s16 gCupCourseOrder[5][4] = {
    // mushroom cup
    { COURSE_LUIGI_RACEWAY, COURSE_MOO_MOO_FARM, COURSE_KOOPA_BEACH, COURSE_KALAMARI_DESERT },
    // flower cup
    { COURSE_TOADS_TURNPIKE, COURSE_FRAPPE_SNOWLAND, COURSE_CHOCO_MOUNTAIN, COURSE_MARIO_RACEWAY },
    // star cup
    { COURSE_WARIO_STADIUM, COURSE_SHERBET_LAND, COURSE_ROYAL_RACEWAY, COURSE_BOWSER_CASTLE },
    // special cup
    { COURSE_DK_JUNGLE, COURSE_YOSHI_VALLEY, COURSE_BANSHEE_BOARDWALK, COURSE_RAINBOW_ROAD },
    // battle mode
    { COURSE_BIG_DONUT, COURSE_BLOCK_FORT, COURSE_DOUBLE_DECK, COURSE_SKYSCRAPER },
};

const s8 D_800F2BDC[8] = { 1, 0, 0, 0, 0, 1, 3, 4 };

const union GameModePack D_800F2BE4 = { { 0, 1, 2, 3 } };

/**************************/

/**
 * Includes opening logo and splash screens
 */
void update_menus(void) {
    u16 controllerIdx;

    if (D_800E86A4 == 0) {
        for (controllerIdx = 0; controllerIdx < 4; controllerIdx++) {

            // Debug, quick jump through menus using the start button.
            if ((func_800B4520() == 0) && (gEnableDebugMode) &&
                ((gControllers[controllerIdx].buttonPressed & START_BUTTON) != 0)) {
                // this is certainly a way to write these...
                switch (gMenuSelection) {
                    case COURSE_SELECT_MENU:
                        func_800CA330(0x19);
                        // deliberate (?) fallthru
                    case MAIN_MENU:
                    case PLAYER_SELECT_MENU:
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
                case PLAYER_SELECT_MENU:
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

// navigation of the options menu
void options_menu_act(struct Controller* controller, u16 arg1) {
    u16 btnAndStick; // sp3E
    struct_8018D9E0_entry* sp38;
    s32 res;
    struct_8018EE10_entry* sp30;
    bool sp2C; // cursorWasMoved or communicateStoredAction
    UNUSED u32 pad;

    btnAndStick = (controller->buttonPressed | controller->stickPressed);

    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!func_800B4520()) {
        sp38 = find_8018D9E0_entry_dupe(0xF0);
        sp30 = (struct_8018EE10_entry*) D_8018D9C0;
        switch (D_8018EDEC) {
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18: {
                sp2C = false;
                if ((btnAndStick & D_JPAD) && (D_8018EDEC < 0x18)) {
                    D_8018EDEC += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = 1;
                    sp2C = true;
                }
                if ((btnAndStick & U_JPAD) && (D_8018EDEC >= 0x16)) {
                    D_8018EDEC -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp2C = true;
                    sp38->unk8 = -1;
                }
                if (sp2C && gSoundMode != sp38->cursor) {
                    gSaveData.main.saveInfo.soundMode = gSoundMode;
                    write_save_data_grand_prix_points_and_sound_mode();
                    update_save_data_backup();
                    sp38->cursor = gSoundMode;
                }
                if (btnAndStick & B_BUTTON) {
                    func_8009E280();
                    play_sound2(SOUND_MENU_GO_BACK);
                    if (gSoundMode != sp38->cursor) {
                        gSaveData.main.saveInfo.soundMode = gSoundMode;
                        write_save_data_grand_prix_points_and_sound_mode();
                        update_save_data_backup();
                        sp38->cursor = gSoundMode;
                    }
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (D_8018EDEC) {
                        case 0x16:
                            if (gSoundMode < 3) {
                                gSoundMode += 1;
                            } else {
                                gSoundMode = SOUND_STEREO;
                            }
                            if (gSoundMode == SOUND_UNUSED) {
                                gSoundMode = SOUND_MONO;
                            }
                            func_800B44BC();
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
                        case 0x17:
                            switch (controller_pak_2_status()) {
                                case PFS_INVALID_DATA:
                                    D_8018EDEC = 0x2B;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_NO_ERROR:
                                    func_800B6798();
                                    sp2C = controller_pak_1_status();
                                    switch (sp2C) {
                                        case PFS_INVALID_DATA:
                                            D_8018EDEC = 0x46;
                                            sp38->cursor = 0;
                                            play_sound2(SOUND_MENU_SELECT);
                                            break;
                                        case PFS_NO_ERROR:
                                            func_800B6708();
                                            break;
                                        case PFS_NO_PAK_INSERTED:
                                            D_8018EDEC = 0x34;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_FILE_OVERFLOW:
                                            D_8018EDEC = 0x37;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_PAK_BAD_READ:
                                        case 3:
                                        default:
                                            D_8018EDEC = 0x35;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                    }
                                    if (sp2C == PFS_INVALID_DATA && !sp30[0].ghostDataSaved &&
                                        !sp30[1].ghostDataSaved) {
                                        D_8018EDEC = 0x2A;
                                        play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                        return;
                                    }
                                    if (sp2C == PFS_NO_ERROR) {
                                        if (sp30[0].ghostDataSaved) {
                                            D_8018EDEC = 0x28;
                                            play_sound2(SOUND_MENU_SELECT);
                                        } else if (sp30[1].ghostDataSaved) {
                                            D_8018EDEC = 0x29;
                                            play_sound2(SOUND_MENU_SELECT);
                                        } else {
                                            D_8018EDEC = 0x2A;
                                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                        }
                                    }
                                    // else return?
                                    return;
                                case PFS_NO_PAK_INSERTED:
                                    D_8018EDEC = 0x2C;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_PAK_BAD_READ:
                                default:
                                    D_8018EDEC = 0x2D;
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                            }
                        case 0x18: {
                            D_8018EDEC = 0x1E;
                            play_sound2(SOUND_MENU_SELECT);
                            return;
                        }
                        case 0x15: {
                            func_8009E280();
                            play_sound2(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                }
                // maybe else return?;
                break;
            }
            case 0x1E:
            case 0x1F: {
                if ((btnAndStick & D_JPAD) && (D_8018EDEC < 0x1F)) {
                    D_8018EDEC += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = 1;
                }
                if ((btnAndStick & U_JPAD) && (D_8018EDEC >= 0x1F)) {
                    D_8018EDEC -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    D_8018EDEC = 0x18;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (D_8018EDEC) {
                        case 0x1E:
                            D_8018EDEC = 0x18;
                            play_sound2(SOUND_MENU_GO_BACK);
                            break;
                        case 0x1F:
                            D_8018EDEC = 0x20;
                            func_800B46D0();
                            D_800DC5AC = 0;
                            play_sound2(SOUND_MENU_EXPLOSION);
                            break;
                    }
                }
                break; // or return?
            }
            case 0x20: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    D_8018EDEC = 0x18;
                    play_sound2(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case 0x28:
            case 0x29: {
                if ((btnAndStick & D_JPAD) && (D_8018EDEC < 0x29) && (sp30[1].ghostDataSaved)) {
                    D_8018EDEC += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = 1;
                }
                if ((btnAndStick & U_JPAD) && (D_8018EDEC >= 0x29) && sp30[0].ghostDataSaved) {
                    D_8018EDEC -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    D_8018EDEC = 0x17;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    sp38->unk20 = D_8018EDEC - 0x28;
                    if (sp30[sp38->unk20].courseIndex == D_8018EE10[1].courseIndex && D_8018EE10[1].ghostDataSaved) {
                        D_8018EDEC = 0x33;
                    } else {
                        D_8018EDEC = 0x32;
                    }
                    play_sound2(SOUND_MENU_SELECT);
                }
                break;
            }
            case 0x32:
            case 0x33: {
                // bit of a fake match, but if it works it works?
                if ((sp30[sp38->unk20].courseIndex != ((0, (D_8018EE10 + (D_8018EDEC - 0x32))->courseIndex))) ||
                    ((D_8018EE10 + (D_8018EDEC - 0x32))->ghostDataSaved == 0)) {
                    if ((btnAndStick & D_JPAD) && (D_8018EDEC < 0x33)) {
                        D_8018EDEC += 1;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->unk24 < 4.2) {
                            sp38->unk24 += 4.0;
                        }
                        sp38->unk8 = 1;
                    }
                    if ((btnAndStick & U_JPAD) && (D_8018EDEC >= 0x33)) {
                        D_8018EDEC -= 1;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->unk24 < 4.2) {
                            sp38->unk24 += 4.0;
                        }
                        sp38->unk8 = -1;
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    D_8018EDEC = sp38->unk20 + 0x28;
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    sp38->unk1C = D_8018EDEC - 0x32;
                    if (D_8018EE10[(sp38->unk1C)].ghostDataSaved) {
                        D_8018EDEC = 0x38;
                    } else {
                        D_8018EDEC = 0x3A;
                        sp38->cursor = 0;
                    }
                    play_sound2(SOUND_MENU_SELECT);
                }
                break;
            }
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x34:
            case 0x35:
            case 0x37:
            case 0x3C:
            case 0x41:
            case 0x42: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    D_8018EDEC = 0x17;
                    play_sound2(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case 0x38:
            case 0x39: {
                if ((btnAndStick & R_JPAD) && D_8018EDEC < 0x39) {
                    D_8018EDEC += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = 1;
                }
                if ((btnAndStick & L_JPAD) && D_8018EDEC >= 0x39) {
                    D_8018EDEC -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->unk24 < 4.2) {
                        sp38->unk24 += 4.0;
                    }
                    sp38->unk8 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    D_8018EDEC = sp38->unk1C + 0x32;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    if (D_8018EDEC == 0x38) {
                        D_8018EDEC = 0x17;
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        D_8018EDEC = 0x3A;
                        play_sound2(SOUND_MENU_SELECT);
                        sp38->cursor = 0;
                    }
                }
                // return?
                break;
            }
            case 0x3A: {
                if (arg1 == 0) {
                    sp38->cursor += 1;
                }
                if (sp38->cursor >= 3) {
                    D_8018EDEC = 0x3B;
                }
                break;
            }
            case 0x3B: {
                res = controller_pak_2_status();
                if (res == PFS_NO_ERROR) {
                    res = func_800B65F4(sp38->unk20, sp38->unk1C);
                }
                if (res != 0) {
                    D_8018EDEC = 0x42;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                res = osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                    (u8*) gExtCode, &gControllerPak1FileNote);
                if (res == PFS_NO_ERROR) {
                    res = func_800B6178(sp38->unk1C);
                }
                if (res != 0) {
                    D_8018EDEC = 0x41;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                D_8018EDEC = 0x3C;
                D_8018EE10[sp38->unk1C].courseIndex = (sp30 + sp38->unk20)->courseIndex;
                func_800B6088(sp38->unk1C);
                break;
            }
            case 0x46: {
                if (arg1 == 0) {
                    sp38->cursor += 1;
                }
                if (sp38->cursor >= 3) {
                    D_8018EDEC = 0x47;
                }
                break;
            }
            case 0x47: {
                if (func_800B6A68()) {
                    D_8018EDEC = 0x36;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                } else if (sp30[0].ghostDataSaved) {
                    D_8018EDEC = 0x28;
                } else {
                    D_8018EDEC = 0x29;
                }
                break;
            }
            default:
                break;
        }
    }
}

// Handle navigating the data menu interface
void data_menu_act(struct Controller* controller, UNUSED u16 arg1) {
    u16 buttonAndStickPress = (controller->buttonPressed | controller->stickPressed);

    // Make pressing Start have the same effect as pressing A
    if ((gEnableDebugMode == 0) && ((buttonAndStickPress & 0x1000) != 0)) {
        buttonAndStickPress |= 0x8000;
    }

    if (func_800B4520() == 0) {
        if (D_8018EDEC == 1) {
            // If DPad/Stick down pressed, move selection down if not already in bottom row
            if ((buttonAndStickPress & 0x400) != 0) {
                if ((gTimeTrialDataCourseIndex % 4) != 3) {
                    ++gTimeTrialDataCourseIndex;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick up pressed, move selection up if not already in top row
            if ((buttonAndStickPress & 0x800) != 0) {
                if ((gTimeTrialDataCourseIndex & 3) != 0) {
                    --gTimeTrialDataCourseIndex;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick right pressed, move selection right if not already in right-most column
            if ((buttonAndStickPress & 0x100) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 3) {
                    gTimeTrialDataCourseIndex += 4;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick left pressed, move selection left if not already in left-most column
            if ((buttonAndStickPress & 0x200) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 0) {
                    gTimeTrialDataCourseIndex -= 4;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If B pressed, go to main menu
            if ((buttonAndStickPress & 0x4000) != 0) {
                func_8009E258();
                play_sound2(SOUND_MENU_GO_BACK);
                return;
            }
            // If A pressed, go to selected course's records
            if ((buttonAndStickPress & 0x8000) != 0) {
                gCourseRecordsMenuSelection = 0;
                func_8009E1C0();
                play_sound2(SOUND_MENU_OK_CLICKED);
            }
        }
        // If D_8018EDEC != 1 and A pressed, go to main menu
        // (Will D_8018EDEC ever not equal 1 when entering the data menu?)
        else if ((buttonAndStickPress & 0x8000) != 0) {
            func_8009E258();
            play_sound2(SOUND_MENU_OK_CLICKED);
        }
    }
}

void course_data_menu_act(struct Controller* controller, UNUSED u16 arg1) {
    u16 btnAndStick; // sp2E
    struct_8018D9E0_entry* sp28;
    CourseTimeTrialRecords* sp24;
    s32 res;

    btnAndStick = (controller->buttonPressed | controller->stickPressed);

    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!func_800B4520()) {
        switch (D_8018EDEC) {
            case 0x0B: {
                if ((btnAndStick & L_JPAD) && (gTimeTrialDataCourseIndex > 0)) {
                    gTimeTrialDataCourseIndex -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                if ((btnAndStick & R_JPAD) && (gTimeTrialDataCourseIndex < 15)) {
                    gTimeTrialDataCourseIndex += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                sp28 = find_8018D9E0_entry_dupe(0xE8);
                sp24 = &gSaveData.allCourseTimeTrialRecords.cupRecords[gTimeTrialDataCourseIndex / 4]
                            .courseRecords[gTimeTrialDataCourseIndex % 4];
                if (gCourseRecordsMenuSelection == 2 && func_800B639C(gTimeTrialDataCourseIndex) < 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if (gCourseRecordsMenuSelection == 1 && sp24->unknownBytes[0] == 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if ((btnAndStick & U_JPAD) && (gCourseRecordsMenuSelection > 0)) {
                    gCourseRecordsMenuSelection -= 1;
                    if (gCourseRecordsMenuSelection == 1 && sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection -= 1;
                    }
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->unk24 < 4.2) {
                        sp28->unk24 += 4.0;
                    }
                    sp28->unk8 = -1;
                }

                if ((btnAndStick & D_JPAD) && (gCourseRecordsMenuSelection < 2)) {
                    gCourseRecordsMenuSelection += 1;
                    if (gCourseRecordsMenuSelection == 1 && sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection += 1;
                    }

                    if (gCourseRecordsMenuSelection == 2 && func_800B639C(gTimeTrialDataCourseIndex) < 0) {
                        if (sp24->unknownBytes[0] == 0) {
                            gCourseRecordsMenuSelection = 0;
                        } else {
                            gCourseRecordsMenuSelection = 1;
                        }
                    } else {
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (sp28->unk24 < 4.2) {
                            sp28->unk24 += 4.0;
                        }
                        sp28->unk8 = 1;
                    }
                }

                if (btnAndStick & B_BUTTON) {
                    func_8009E208();
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (sp28->unk24 < 4.2) {
                        sp28->unk24 += 4.0;
                    }
                    if (gCourseRecordsMenuSelection == 0) {
                        func_8009E208();
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        D_8018EDEC = 0x0C;
                        D_8018EDF9 = 0;
                        play_sound2(SOUND_MENU_SELECT);
                    }
                }
                break;
            }
            case 0x0C: {
                sp28 = find_8018D9E0_entry_dupe(0xE9);
                if ((btnAndStick & U_JPAD) && (D_8018EDF9 > 0)) {
                    D_8018EDF9 -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->unk24 < 4.2) {
                        sp28->unk24 += 4.0;
                    }
                    sp28->unk8 = -1;
                }

                if ((btnAndStick & D_JPAD) && (D_8018EDF9 <= 0)) {
                    D_8018EDF9 += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->unk24 < 4.2) {
                        sp28->unk24 += 4.0;
                    }
                    sp28->unk8 = 1;
                }

                if (btnAndStick & B_BUTTON) {
                    D_8018EDEC = 0xB;
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (D_8018EDF9 != 0) {
                        res = 0;
                        switch (gCourseRecordsMenuSelection) {
                            case 1: {
                                func_800B4728(gTimeTrialDataCourseIndex);
                                func_800B559C(gTimeTrialDataCourseIndex);
                                play_sound2(SOUND_MENU_EXPLOSION);
                                res = -1;
                                break;
                            }
                            case 2: {
                                res = func_800B639C(gTimeTrialDataCourseIndex);
                                if (res >= 0) {
                                    if (func_800B69BC(res) != 0) {
                                        D_8018EDEC = 0x0D;
                                        play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    } else {
                                        play_sound2(SOUND_MENU_EXPLOSION);
                                        D_8018EDEC = 0x0B;
                                    }
                                }
                                break;
                            }
                        }

                        if (!(res + 1)) {
                            D_8018EDEC = 0xB;
                        }
                    } else {
                        play_sound2(SOUND_MENU_GO_BACK);
                        D_8018EDEC = 0xB;
                    }
                }
                break;
            }
            case 0x0D: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    D_8018EDEC = 0xB;
                }
                break;
            }
        }
    }
}

/**
 * On input skip logo screen
 **/
void logo_intro_menu_act(struct Controller* arg0, UNUSED u16 arg1) {
    u16 anyInput = arg0->buttonPressed | arg0->stickPressed;

// Note: Choosing a course in the middle of a cup
// will contain no definition for player staging/lineup.
// #define SKIP_TO_RACE
#ifdef SKIP_TO_RACE
    gGamestateNext = 4; // Enter race state
    gCCSelection = CC_100;
    gCupSelection = 1;
    gCupCourseSelection = 0;
    gCurrentCourseId = 0;
    gScreenModeSelection = SCREEN_MODE_1P;
    gCharacterSelections[0] = 0;
    gModeSelection = GRAND_PRIX;
    gPlayerCount = 1;
#endif

    if ((func_800B4520() == 0) && (anyInput)) {
        // Audio related
        func_800CA388(0x3C);

        func_8009E1E4();
    }
}

void controller_pak_menu_act(struct Controller* controller, UNUSED u16 arg1) {
    u16 buttonAndStickPress;
    OSPfsState* osPfsState;
    s32 selectedTableRow;
    UNUSED s8 pad;

    buttonAndStickPress = controller->buttonPressed | controller->stickPressed;
    if (func_800B4520() == 0) {
        switch (gControllerPakMenuSelection) {
            case CONTROLLER_PAK_MENU_SELECT_RECORD:
                if ((buttonAndStickPress & 0x9000) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_SELECT);
                    return;
                }
                if ((buttonAndStickPress & 0x300) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_END;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_END:
                if ((buttonAndStickPress & 0x9000) != 0) {
                    play_sound2(SOUND_MENU_SELECT);
                    func_8009E1C0();
                    gControllerPak1State = BAD;
                    return;
                }
                if ((buttonAndStickPress & 0x300) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_TABLE_GAME_DATA:
                if ((buttonAndStickPress & 0x9000) != 0) {
                    selectedTableRow = D_800E86C4[gControllerPakSelectedTableRow + 2] - 1;
                    if (pfsError[selectedTableRow] == 0) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                        play_sound2(SOUND_MENU_SELECT);
                        return;
                    }
                } else if ((buttonAndStickPress & 0x4000) != 0) {
                    if (D_800E86D0[0] == 0) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                        play_sound2(SOUND_MENU_GO_BACK);
                        return;
                    }
                } else if ((buttonAndStickPress & 0x800) != 0) {
                    if (D_800E86D0[0] == 0) {
                        --gControllerPakSelectedTableRow;
                        if (gControllerPakSelectedTableRow < 0) {
                            gControllerPakSelectedTableRow = 0;
                            if (D_800E86C4[gControllerPakSelectedTableRow + 2] != 1) {
                                D_800E86D0[0] = 2;
                                play_sound2(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }
                        } else {
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    }
                } else if (((buttonAndStickPress & 0x400) != 0) && (D_800E86D0[0] == 0)) {
                    ++gControllerPakSelectedTableRow;
                    if (gControllerPakSelectedTableRow >= CONTROLLER_PAK_MENU_TABLE_GAME_DATA) {
                        gControllerPakSelectedTableRow = CONTROLLER_PAK_MENU_QUIT;
                        if (D_800E86C4[gControllerPakSelectedTableRow + 2] != 0x10) {
                            D_800E86D0[0] = 1;
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
                if ((buttonAndStickPress & 0xD000) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((buttonAndStickPress & 0x300) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE:
                if ((buttonAndStickPress & 0x9000) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_GO_TO_ERASING;
                    play_sound2(SOUND_MENU_SELECT);
                    return;
                }
                if ((buttonAndStickPress & 0x4000) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((buttonAndStickPress & 0x300) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_GO_TO_ERASING:
                gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASING;
                return;
            case CONTROLLER_PAK_MENU_ERASING:
                selectedTableRow = D_800E86C4[gControllerPakSelectedTableRow + 2] - 1;
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
                    case PFS_ERR_NEW_PACK: /* switch 1 */
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED;
                        return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
                if ((buttonAndStickPress & 0x9000) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                }
                break;
        }
    }
}

void splash_menu_act(struct Controller* controller, u16 arg1) {
    u16 btnAndStick;
    u16 i;
    s32 sp28;

    sp28 = true;
    btnAndStick = controller->buttonPressed | controller->stickPressed;

    if (func_800B4520() == 0) {
        if (arg1 == 0) {
            gMenuDelayTimer += 1;
        }
        switch (gDebugMenuSelection) {
            case DEBUG_MENU_DISABLED: {
                sp28 = false;
                if ((gMenuDelayTimer >= 0x2E) && (btnAndStick & (A_BUTTON | START_BUTTON))) {
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
                        gEnableDebugMode = DEBUG_MODE;
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
                    if (gCurrentCourseId < (NUM_COURSES - 2)) {
                        gCurrentCourseId += 1;
                    } else {
                        gCurrentCourseId = 0;
                    }
                }
                if (btnAndStick & L_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (gCurrentCourseId > 0) {
                        gCurrentCourseId -= 1;
                    } else {
                        gCurrentCourseId = (NUM_COURSES - 2);
                    }
                }
                if (btnAndStick & U_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gDebugMenuSelection = DEBUG_MENU_DEBUG_MODE;
                }
                if (btnAndStick & D_JPAD) {
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gDebugMenuSelection = DEBUG_MENU_SCREEN_MODE;
                }
                break;
            }
            case DEBUG_MENU_SCREEN_MODE: {
                if ((btnAndStick & R_JPAD) && (D_8018EDF1 < 4)) {
                    D_8018EDF1 += 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gScreenModeSelection = D_800F2B50[D_8018EDF1];
                }
                if ((btnAndStick & L_JPAD) && (D_8018EDF1 > 0)) {
                    D_8018EDF1 -= 1;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    gScreenModeSelection = D_800F2B50[D_8018EDF1];
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_COURSE;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_PLAYER;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_PLAYER: {
                if ((btnAndStick & R_JPAD) && (*gCharacterSelections < 7)) {
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
                    func_800B44BC();
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
                    func_800B44BC();
                    gSaveData.main.saveInfo.soundMode = gSoundMode;
                    write_save_data_grand_prix_points_and_sound_mode();
                }
                if (btnAndStick & U_JPAD) {
                    gDebugMenuSelection = DEBUG_MENU_PLAYER;
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
                    gDebugMenuSelection = DEBUG_MENU_SOUND_MODE;
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

        gPlayerCountSelection1 = gPlayerCount = D_800F2B58[D_8018EDF1];

        if (sp28) {
            if (btnAndStick & (A_BUTTON | START_BUTTON)) {
                func_8009E1C0();
                func_800CA330(0x19);
                gDebugMenuSelection = DEBUG_MENU_EXITED;

                if (controller->button & CONT_L) {
                    gDemoMode = DEMO_MODE_ACTIVE;
                } else {
                    gDemoMode = DEMO_MODE_INACTIVE;
                }

                if (controller->button & Z_TRIG) {
                    if (btnAndStick & A_BUTTON) {
                        gDebugGotoScene = DEBUG_GOTO_ENDING;
                    } else {
                        gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_CC_EXTRA;
                    }
                }
                play_sound2(SOUND_MENU_OK_CLICKED);
            } else if ((btnAndStick & B_BUTTON) && (controller->button & Z_TRIG)) {
                func_8009E1C0();
                func_800CA330(0x19);
                gDebugMenuSelection = DEBUG_MENU_EXITED;
                gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_CC_50;
                play_sound2(SOUND_MENU_OK_CLICKED);
            } else if (btnAndStick & CONT_R) {
                gDebugMenuSelection = DEBUG_MENU_DISABLED;
                play_sound2(SOUND_MENU_SELECT);
            }
        }
    }
}

void func_800B28C8(void) {
    // For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
    // be whether 'Begin' or 'Data' is selected. Not used for Battle.
    s8 temp_v0 = D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]];
    // Determine which game mode was selected based on the number of players and the row selected on the main menu
    switch (gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]]) {
        case GRAND_PRIX:
            gCCSelection = temp_v0;
            gPlaceItemBoxes = 1;
            gIsMirrorMode = (temp_v0 == CC_EXTRA) ? 1 : 0;
            break;
        case VERSUS:
            gCCSelection = temp_v0;
            gPlaceItemBoxes = 1;
            gIsMirrorMode = (temp_v0 == CC_EXTRA) ? 1 : 0;
            break;
        case BATTLE:
            gPlaceItemBoxes = 1;
            gIsMirrorMode = 0;
            break;
        case TIME_TRIALS:
            gCCSelection = CC_100;
            gIsMirrorMode = 0;
            gPlaceItemBoxes = 0;

            if ((temp_v0 && temp_v0) && temp_v0) {}

            break;
    }
}

#ifdef NON_MATCHING
// nonmatching: regalloc; arg1 is not AND-ed back into $a1, reg chaos follows
void main_menu_act(struct Controller* controller, u16 arg1) {
    u16 btnAndStick; // sp2E
    s32 sp28;
    bool sp24;
    s32 newMode; // temp_v1_2?

    btnAndStick = controller->buttonPressed | controller->stickPressed;
    if (!gEnableDebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!func_800B4520()) {
        switch (gMainMenuSelectionDepth) {
            case BLANK_MAIN_MENU: {
                newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                break;
            }
            case PLAYER_NUM_SELECTION: {
                if ((btnAndStick & R_JPAD) && gPlayerCount < 4) {
                    gPlayerCount += 1;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && gPlayerCount >= 2) {
                    gPlayerCount -= 1;
                    func_800B44AC();
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
                    D_8018EDE0 = 1;
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B2C00
                    gMainMenuSelectionDepth = GAME_MODE_SELECTION;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_SELECT);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & CONT_L) {
                    // L800B2C58
                    gMainMenuSelectionDepth = OPTIONS_SELECTION;
                    func_8009E280();
                    play_sound2(SOUND_MENU_OPTION);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & CONT_R) {
                    gMainMenuSelectionDepth = DATA_SELECTION;
                    func_8009E258();
                    play_sound2(SOUND_MENU_DATA);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else {
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                }
                break;
            }
            case GAME_MODE_SELECTION: {
                if (btnAndStick & D_JPAD) {
                    if (D_800E86AC[gPlayerCount - 1] < D_800F2B58[gPlayerCount + 7]) {
                        D_800E86AC[gPlayerCount - 1] += 1;
                        func_800B44AC();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B2D94
                if (btnAndStick & U_JPAD) {
                    if (D_800E86AC[gPlayerCount - 1] > 0) {
                        D_800E86AC[gPlayerCount - 1] -= 1;
                        func_800B44AC();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B2DE0
                if (btnAndStick & B_BUTTON) {
                    gMainMenuSelectionDepth = PLAYER_NUM_SELECTION;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B2E3C
                    switch (gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]]) {
                        case 0:
                            gMainMenuSelectionDepth = GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION;
                            play_sound2(SOUND_MENU_GP);
                            break;
                        case 2:
                            gMainMenuSelectionDepth = GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION;
                            play_sound2(SOUND_MENU_VERSUS);
                            break;
                        case 1:
                            gMainMenuSelectionDepth = GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION;
                            play_sound2(SOUND_MENU_TIME_TRIALS);
                            break;
                        case 3:
                            gMainMenuSelectionDepth = CONFIRM_OK_SELECTION;
                            play_sound2(SOUND_MENU_BATTLE);
                            break;
                        default:
                            gMainMenuSelectionDepth = CONFIRM_OK_SELECTION;
                            break;
                    }
                    // L800B2F04
                    func_800B44AC();
                    gMenuTimingCounter = 0;
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else {
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                }
                break;
            }
            case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION:
            case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT: {
                if (arg1 == 0) {
                    gMenuTimingCounter++;
                    if ((gMenuTimingCounter == 100 || gMenuTimingCounter % 300 == 0)) {
                        // L800B2FAC
                        if (gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]] == 0 ||
                            gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]] == 2) {
                            play_sound2(SOUND_MENU_SELECT_LEVEL);
                        }
                    }
                }
                // L800B3000
                sp28 = D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]];
                if ((btnAndStick & U_JPAD) && (sp28 > 0)) {
                    D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]] -= 1;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                // L800B3068
                if (btnAndStick & D_JPAD) {
                    sp24 = false;
                    if (func_800B555C()) {
                        if (sp28 < D_800F2B60[gPlayerCount + 4][D_800E86AC[gPlayerCount - 1] + 1]) {
                            sp24 = true;
                        }
                    } else {
                        // L800B30D4
                        if (sp28 < D_800F2B60[gPlayerCount][D_800E86AC[gPlayerCount - 1] + 1]) {
                            sp24 = true;
                        }
                    }
                    // L800B3110
                    if (sp24) {
                        D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]]++;
                        func_800B44AC();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                // L800B3150
                sp28 = D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]];
                if (btnAndStick & B_BUTTON) {
                    gMainMenuSelectionDepth = GAME_MODE_SELECTION;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_GO_BACK);
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B31DC
                    func_800B44AC();
                    if (gPlayerCount == 1 && D_800E86AC[gPlayerCount - 1] == 1 && sp28 == 1) {
                        func_8009E258();
                        play_sound2(SOUND_MENU_DATA);
                    } else {
                        gMainMenuSelectionDepth = CONFIRM_OK_SELECTION;
                        play_sound2(SOUND_MENU_SELECT);
                        gMenuTimingCounter = 0;
                    }
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else {
                    // L800B3294
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                }
                break;
            }
            case CONFIRM_OK_SELECTION:
            case CONFIRM_OK_SELECTION_FROM_BACK_OUT: {
                if ((arg1 == 0) && (++gMenuTimingCounter == 60 || gMenuTimingCounter % 300 == 0)) {
                    play_sound2(SOUND_MENU_OK);
                }
                // L800B330C
                if (btnAndStick & B_BUTTON) {
                    switch (gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]]) {
                        case 0:
                        case 1:
                        case 2:
                            gMainMenuSelectionDepth = GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION;
                            break;
                        case 3:
                        default:
                            gMainMenuSelectionDepth = GAME_MODE_SELECTION;
                            break;
                    }
                    // L800B3384
                    func_800B44AC();
                    play_sound2(SOUND_MENU_GO_BACK);
                    gMenuTimingCounter = 0;
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else if (btnAndStick & A_BUTTON) {
                    // L800B33D8
                    func_8009E1C0();
                    play_sound2(SOUND_MENU_OK_CLICKED);
                    func_800B28C8();
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                } else {
                    newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                }
                break;
            }
            case OPTIONS_SELECTION:
            case DATA_SELECTION: {
                newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                break;
            }
            default: {
                newMode = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                break;
            }
        }
        gModeSelection = newMode;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/menus/main_menu_act.s")
#endif

// check if there is no currently selected and/or hovered character at grid position `gridId`
bool is_character_spot_free(s32 gridId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(gCharacterGridSelections); i++) {
        if (gridId == gCharacterGridSelections[i]) {
            return false;
        }
    }
    return true;
}

#ifdef NON_MATCHING
// grid positions are from right to left, then top to bottom
// nonmatching: the gCharacterGridSelections pointer is not promoted to $s0
void player_select_menu_act(struct Controller* controller, u16 arg1) {
    s8* bar;
    s8 selected;
    s8 i;
    s8 saved_selection;
    u16 btnAndStick;

    btnAndStick = (controller->buttonPressed) | (controller->stickPressed);
    if (!gEnableDebugMode && btnAndStick & CONT_START) {
        btnAndStick |= A_BUTTON;
    }

    if (!func_800B4520()) {
        switch (D_8018EDEE) {
            case 1: {
                saved_selection = gCharacterGridSelections[arg1];
                if (saved_selection == 0) {
                    if (btnAndStick & B_BUTTON) {
                        func_8009E208();
                        play_sound2(0x49008002);
                    }
                    return;
                }
                // L800B3630
                if (btnAndStick & B_BUTTON) {
                    if (D_8018EDE8[arg1]) {
                        D_8018EDE8[arg1] = false;
                        play_sound2(SOUND_MENU_GO_BACK);
                    } else {
                        func_8009E208();
                        play_sound2(0x49008002);
                    }
                }
                // L800B3684
                if ((btnAndStick & A_BUTTON) && (D_8018EDE8[arg1] == 0)) {
                    D_8018EDE8[arg1] = true;
                    i = D_800F2BAC[gCharacterGridSelections[arg1] - 1];
                    func_800C90F4(arg1, 0x2900800e + (i << 4));
                }
                // L800B36F4
                selected = false;
                for (i = 0; i < 4; i++) { // for (i = 0; i < ARRAY_COUNT(gCharacterGridSelections); i++) {
                    if ((gCharacterGridSelections[i] != 0) &&
                        (D_8018EDE8[i] == 0)) { //(gCharacterGridSelections[i] && D_8018EDE8[i]) {
                        selected = true;
                        break;
                    }
                }
                // L800B3738

                if (!selected) {
                    D_8018EDEE = 2;
                    func_800B44AC();
                    gMenuTimingCounter = 0;
                }

                // L800B3768
                if (D_8018EDE8[arg1] == 0) {
                    if ((btnAndStick & CONT_RIGHT) && (btnAndStick & CONT_DOWN)) {
                        if (saved_selection == 1 || saved_selection == 2 || saved_selection == 3) {
                            // L800B37B0
                            saved_selection += 5;
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B37E4
                    if ((btnAndStick & CONT_LEFT) && (btnAndStick & CONT_DOWN)) {
                        if (saved_selection == 2 || saved_selection == 3 || saved_selection == 4) {
                            saved_selection += 3;
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B3844
                    if ((btnAndStick & CONT_RIGHT) && (btnAndStick & CONT_UP)) {
                        if (saved_selection == 5 || saved_selection == 6 || saved_selection == 7) {
                            saved_selection -= 3;
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B38A0
                    if ((btnAndStick & CONT_LEFT) && (btnAndStick & CONT_UP)) {
                        if (saved_selection == 6 || saved_selection == 7 || saved_selection == 8) {
                            saved_selection -= 5;
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000);
                            }
                        }
                        return;
                    }
                    // L800B38FC
                    if (btnAndStick & CONT_RIGHT) {
                        if (saved_selection == 4 || saved_selection == 8)
                            return;
                        saved_selection += 1;
                        do {
                            // L800B391C
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000); // play_sound2(0x49008000);
                                break;
                            }
                            saved_selection += 1;
                            if ((saved_selection == 5) || (saved_selection == 9))
                                return;
                        } while (saved_selection < 10);
                        return;
                    }
                    // L800B3978
                    if (btnAndStick & CONT_LEFT) {
                        if (saved_selection == 1 || saved_selection == 5)
                            return;
                        saved_selection -= 1;
                        do {
                            if (is_character_spot_free(saved_selection)) {
                                gCharacterGridSelections[arg1] = saved_selection;
                                play_sound2(0x49008000);
                                break;
                            }
                            saved_selection -= 1;
                            if ((saved_selection == 0) || (saved_selection == 4))
                                return;
                        } while (saved_selection >= 0);
                        return;
                    }
                    // L800B39F4
                    if ((btnAndStick & CONT_UP) && (saved_selection >= 5)) {
                        saved_selection = saved_selection - 4;
                    }
                    if ((btnAndStick & CONT_DOWN) && (saved_selection < 5)) {
                        saved_selection = saved_selection + 4;
                    }
                    // L800B3A30
                    if (is_character_spot_free(saved_selection)) {
                        gCharacterGridSelections[arg1] = saved_selection;
                        play_sound2(0x49008000);
                    }
                }
                break;
            }
            case 2:
            case 3:
                if (arg1 == 0) {
                    gMenuTimingCounter++;
                    if ((gMenuTimingCounter == 60) || ((gMenuTimingCounter % 300) == 0)) {
                        // L800B3A94
                        play_sound2(0x4900900F);
                    }
                }
                // L800B3AA4
                if (btnAndStick & B_BUTTON) {
                    D_8018EDEE = 1;
                    D_8018EDE8[arg1] = false;
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
        if (gCharacterGridSelections[arg1] != 0) {
            gCharacterSelections[arg1] = D_800F2BAC[gCharacterGridSelections[arg1] - 1];
        }
    }
    // L800B3B44
}
#else
GLOBAL_ASM("asm/non_matchings/menus/player_select_menu_act.s")
#endif

// Handle navigating the course menu interface
void course_select_menu_act(struct Controller* arg0, u16 arg1) {
    u16 buttonAndStickPress = (arg0->buttonPressed | arg0->stickPressed);

    if ((!gEnableDebugMode) && ((buttonAndStickPress & START_BUTTON) != 0)) {
        buttonAndStickPress |= A_BUTTON;
    }

    if (func_800B4520() == 0) {
        switch (D_8018EDEC) {
            case 1:
                if ((buttonAndStickPress & R_JPAD) != 0) {
                    if (gCupSelection < SPECIAL_CUP) {
                        D_8018EE0A = gCupSelection;
                        ++gCupSelection;
                        func_800B44AC();
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (((buttonAndStickPress & L_JPAD) != 0) && (gCupSelection > MUSHROOM_CUP)) {
                    D_8018EE0A = gCupSelection;
                    --gCupSelection;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                D_800DC540 = gCupSelection;
                gCurrentCourseId = gCupCourseOrder[gCupSelection][gCupCourseSelection];
                if ((buttonAndStickPress & B_BUTTON) != 0) {
                    func_8009E208();
                    play_sound2(SOUND_MENU_GO_BACK);
                } else if ((buttonAndStickPress & A_BUTTON) != 0) {
                    if (gModeSelection != GRAND_PRIX) {
                        D_8018EDEC = 2;
                        play_sound2(SOUND_MENU_SELECT);
                    } else {
                        D_8018EDEC = 3;
                        play_sound2(SOUND_MENU_SELECT);
                        gCurrentCourseId = gCupCourseOrder[gCupSelection][CUP_COURSE_ONE];
                        gMenuTimingCounter = 0;
                    }
                    func_800B44AC();
                }
                break;
            case 2:
            case 4:
                if (((buttonAndStickPress & D_JPAD) != 0) && (gCupCourseSelection < CUP_COURSE_FOUR)) {
                    ++gCupCourseSelection;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }
                if (((buttonAndStickPress & U_JPAD) != 0) && (gCupCourseSelection > CUP_COURSE_ONE)) {
                    --gCupCourseSelection;
                    func_800B44AC();
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                }

                gCurrentCourseId = gCupCourseOrder[gCupSelection][gCupCourseSelection];
                if ((buttonAndStickPress & B_BUTTON) != 0) {
                    if (D_8018EDEC == 2) {
                        D_8018EDEC = 1;
                    } else {
                        func_8009E208();
                    }
                    func_800B44AC();
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((buttonAndStickPress & A_BUTTON) != 0) {
                    D_8018EDEC = 3;
                    play_sound2(SOUND_MENU_SELECT);
                    func_800B44AC();
                    gMenuTimingCounter = 0;
                }
                break;
            case 3:
                if ((arg1 == 0) && ((++gMenuTimingCounter == 0x3C) || ((gMenuTimingCounter % 300) == 0))) {
                    play_sound2(SOUND_MENU_OK);
                }

                if ((buttonAndStickPress & B_BUTTON) != 0) {
                    switch (gModeSelection) {
                        case GRAND_PRIX:
                            D_8018EDEC = 1;
                            break;
                        case BATTLE:
                            D_8018EDEC = 4;
                            break;
                        default:
                            D_8018EDEC = 2;
                            break;
                    }

                    func_800B44AC();
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((buttonAndStickPress & A_BUTTON) != 0) {
                    func_8009E1C0();
                    func_800CA330(0x19);
                    play_sound2(SOUND_MENU_OK_CLICKED);
                }
                break;
        }
    }
}

void func_800B3F74(s32 menuSelection) {
    s32 i;

    gDebugMenuSelection = DEBUG_MENU;
    gMenuTimingCounter = 0;
    gMenuDelayTimer = 0;
    D_8018EE08 = 0;
    D_8015F890 = 0;
    D_8015F892 = 0;
    gDebugGotoScene = DEBUG_GOTO_RACING;
    D_8018EDFB = 0;
    D_8016556E = 0;
    D_80162DD4 = 1;
    D_80162DD8 = 1;
    D_80162E00 = 0;
    D_80162DC8 = 1;
    D_80162DCC = 0;

    switch (menuSelection) {
        case 5:
            D_8018EDEC = 21;
            break;
        case 6:
            D_8018EDEC = 1;
            break;
        case 7:
            D_8018EDEC = 11;
            break;
        case 8:
            func_800CA008(0, 0);
            break;
        case 9: {
            gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
            func_800CA008(0, 0);
            break;
        }
        case 0:
        case 10: {
            gIsMirrorMode = 0;
            gEnableDebugMode = DEBUG_MODE;
            gCupSelection = MUSHROOM_CUP;
            gCupCourseSelection = 0;
            gTimeTrialDataCourseIndex = 0;
            if (gPlayerCount <= 0) {
                gPlayerCount = 1;
            }
            if (gPlayerCount >= 5) {
                gPlayerCount = 4;
            }
            D_8018EDF1 = D_800F2BDC[gPlayerCount + 3];
            func_800CA008(0, 0);
            func_800C8EAC(1);
            D_8018EDFC = 0;
            break;
        }
        case 1:
        case 11: {
            gEnableDebugMode = DEBUG_MODE;
            gIsMirrorMode = 0;
            D_8018EDFC = 0;
            func_800B5F30();
            func_8000F0E0();

            if (gGamestate != 0) {
                func_800CA008(0, 0);
                func_800CB2C4();
                gGamestate = 0;
                gGamestateNext = 0;
                func_800C8EAC(2);
            }

            switch (D_8018EDE0) {
                case 0: {
                    gMainMenuSelectionDepth = PLAYER_NUM_SELECTION;
                    func_800C8EAC(2);
                    gPlayerCount = 1;
                    if (gScreenModeSelection >= NUM_SCREEN_MODES || gScreenModeSelection < 0) {
                        gScreenModeSelection = SCREEN_MODE_1P;
                    }
                    break;
                }
                case 1: {
                    gMainMenuSelectionDepth = CONFIRM_OK_SELECTION_FROM_BACK_OUT;
                    break;
                }
                case 3: {
                    // why...
                    switch (gMainMenuSelectionDepth) {
                        default:
                            gMainMenuSelectionDepth = TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT;
                            break;
                        case OPTIONS_SELECTION:
                        case DATA_SELECTION:
                            gMainMenuSelectionDepth = PLAYER_NUM_SELECTION;
                            break;
                    }
                    break;
                }
                case 4: {
                    gMainMenuSelectionDepth = PLAYER_NUM_SELECTION;
                    break;
                }
            }
            break;
        }
        case 2:
        case 12: {
            switch (D_8018EDE0) {
                case 0: {
                    D_8018EDEE = 1;
                    if (gGamestate == 0) {
                        for (i = 0; i < 4; i++) {
                            if (i < gPlayerCount) {
                                gCharacterGridSelections[i] = i + 1;
                            } else {
                                gCharacterGridSelections[i] = 0;
                            }
                            D_8018EDE8[i] = false;
                            gCharacterSelections[i] = i;
                        }
                        play_sound2(SOUND_MENU_SELECT_PLAYER);
                    } else {
                        func_800CA008(0, 0);
                        func_800CB2C4();
                        gGamestate = 0;
                        gGamestateNext = 0;
                        func_800C8EAC(2);
                        for (i = 0; i < ARRAY_COUNT(D_8018EDE8); i++) {
                            D_8018EDE8[i] = false;
                        }
                    }
                    break;
                }
                case 1: {
                    D_8018EDEE = 3;
                    for (i = 0; i < ARRAY_COUNT(D_8018EDE8); i++) {
                        if (gPlayerCount > i) {
                            D_8018EDE8[i] = true;
                        } else {
                            D_8018EDE8[i] = false;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 3:
        case 13: {
            if (gModeSelection == BATTLE) {
                gCupSelection = BATTLE_CUP;
                D_800DC540 = 4;
                D_8018EDEC = 4;
            } else {
                if (gCupSelection == BATTLE_CUP) {
                    gCupSelection = MUSHROOM_CUP;
                }
                D_8018EDEC = 1;
            }
            if (gGamestate != 0) {
                func_800CA008(0, 0);
                func_800CB2C4();
                gGamestate = 0;
                gGamestateNext = 0;
                func_800C8EAC(2);
            }
            play_sound2(SOUND_MENU_SELECT_MAP);
            D_8018EE0A = 0;
            if (gModeSelection == GRAND_PRIX) {
                gCupCourseSelection = 0;
            }

            for (i = 0; i < ARRAY_COUNT(gGPPointsByCharacterId); i++) {
                gGPPointsByCharacterId[i] = 0;
            }
            break;
        }
    }
    func_800B44AC();
}

void func_800B44AC(void) {
    gCycleFlashMenu = 0x20;
}

void func_800B44BC(void) {
    UNUSED u32 pad;
    union GameModePack pack;

    pack = D_800F2BE4;
    if ((gSoundMode == SOUND_STEREO) || (gSoundMode == SOUND_HEADPHONES) || (gSoundMode == SOUND_MONO)) {
        func_800C3448(pack.modes[gSoundMode] | 0xE0000000);
    }
}

// Likely checks that the user is actually in the menus and not racing.
bool func_800B4520(void) {

    if ((D_8018E7AC[4] == 2) || (D_8018E7AC[4] == 3) || (D_8018E7AC[4] == 4) || (D_8018E7AC[4] == 7)) {
        return true;
    }
    return false;
}

UNUSED void func_800B4560(s32 arg0, s32 arg1) {
    struct_8018EE10_entry* pak1 = D_8018EE10;
    struct_8018EE10_entry* pak2 = (struct_8018EE10_entry*) D_8018D9C0;

    rmonPrintf("ghost_kart=%d,", D_80162DE0);
    rmonPrintf("pak1_ghost_kart=%d,", (pak1 + arg0)->characterId);
    rmonPrintf("pak2_ghost_kart=%d\n", (pak2 + arg1)->characterId);
}
