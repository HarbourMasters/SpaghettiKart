#ifndef MENUS_H
#define MENUS_H

#include <common_structs.h>
#include <defines.h>

// gSubMenuSelection is position on options screen?
enum MenuOptionsCursorPositions { MENU_OPTIONS_CSRPOS_SOUNDMODE = 0x16 };

// needs to be a union (or array...?) to go into rodata as a const
// terrible for endianness... Best guess as to what this is for..
union GameModePack {
    u8 modes[4];
    s32 word;
};

/**
 * @brief Options for gMenuFadeType
 */
enum MenuFadeTypes {
    MENU_FADE_TYPE_MAIN,
    MENU_FADE_TYPE_BACK,
    MENU_FADE_TYPE_DEMO,
    MENU_FADE_TYPE_DATA,
    MENU_FADE_TYPE_OPTION,
    MENU_FADE_TYPE_MAX
};

/**
 * @brief Options for gSubMenuSelection
 */
enum SubMenuSelectionType {
    SUB_MENU_NONE,
    SUB_MENU_DATA, // 0x01, value repurposed in map select
    SUB_MENU_MAP_SELECT_CUP = 0x01,
    SUB_MENU_MAP_SELECT_COURSE,
    SUB_MENU_MAP_SELECT_OK,
    SUB_MENU_MAP_SELECT_BATTLE_COURSE,
    SUB_MENU_DATA_OPTIONS = 0x0B,
    SUB_MENU_DATA_ERASE_CONFIRM,
    SUB_MENU_DATA_CANT_ERASE,
    SUB_MENU_OPTION_MIN = 0x15,
    SUB_MENU_OPTION_RETURN_GAME_SELECT = SUB_MENU_OPTION_MIN,
    SUB_MENU_OPTION_SOUND_MODE,
    SUB_MENU_OPTION_COPY_CONTROLLER_PAK,
    SUB_MENU_OPTION_ERASE_ALL_DATA,
    SUB_MENU_OPTION_MAX = SUB_MENU_OPTION_ERASE_ALL_DATA, // 0x18
    SUB_MENU_ERASE_MIN = 0x1E,
    SUB_MENU_ERASE_QUIT = SUB_MENU_ERASE_MIN,
    SUB_MENU_ERASE_ERASE,
    SUB_MENU_ERASE_MAX = SUB_MENU_ERASE_ERASE, // 0x1F
    SUB_MENU_SAVE_DATA_ERASED,                 // 0x20
    SUB_MENU_COPY_PAK_FROM_GHOST_MIN = 0x28,
    SUB_MENU_COPY_PAK_FROM_GHOST1_1P = SUB_MENU_COPY_PAK_FROM_GHOST_MIN,
    SUB_MENU_COPY_PAK_FROM_GHOST2_1P,
    SUB_MENU_COPY_PAK_FROM_GHOST_MAX = SUB_MENU_COPY_PAK_FROM_GHOST2_1P, // 0x29
    SUB_MENU_COPY_PAK_ERROR_2P_MIN,                                      // 0x2A
    SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA = SUB_MENU_COPY_PAK_ERROR_2P_MIN,
    SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA,
    SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P,
    SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P,
    SUB_MENU_COPY_PAK_ERROR_2P_MAX = SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P, // 0x2D
    SUB_MENU_COPY_PAK_TO_GHOST_MIN = 0x32,
    SUB_MENU_COPY_PAK_TO_GHOST1_2P = SUB_MENU_COPY_PAK_TO_GHOST_MIN,
    SUB_MENU_COPY_PAK_TO_GHOST2_2P,
    SUB_MENU_COPY_PAK_TO_GHOST_MAX = SUB_MENU_COPY_PAK_TO_GHOST2_2P, // 0x33
    SUB_MENU_COPY_PAK_ERROR_1P_MIN,
    SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P = SUB_MENU_COPY_PAK_ERROR_1P_MIN, // 0x34
    SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P,
    SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P,
    SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P,
    SUB_MENU_COPY_PAK_ERROR_1P_MAX,
    SUB_MENU_COPY_PAK_PROMPT_MIN = SUB_MENU_COPY_PAK_ERROR_1P_MAX, // 0x38
    SUB_MENU_COPY_PAK_PROMPT_QUIT = SUB_MENU_COPY_PAK_PROMPT_MIN,
    SUB_MENU_COPY_PAK_PROMPT_COPY,
    SUB_MENU_COPY_PAK_PROMPT_MAX = SUB_MENU_COPY_PAK_PROMPT_COPY, // 0x39
    SUB_MENU_COPY_PAK_ACTION_MIN,
    SUB_MENU_COPY_PAK_START = SUB_MENU_COPY_PAK_ACTION_MIN, // 0x3A
    SUB_MENU_COPY_PAK_COPYING,
    SUB_MENU_COPY_PAK_COMPLETED,
    SUB_MENU_COPY_PAK_ACTION_MAX = SUB_MENU_COPY_PAK_COMPLETED, // 0x3C
    SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN = 0x41,
    SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P = SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN,
    SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P,
    SUB_MENU_COPY_PAK_UNABLE_ERROR_MAX = SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P,
    SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT = 0x46,
    SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE
};

/**
 * @brief Options for gMainMenuSelection
 */
enum MainMenuSelectionType {
    MAIN_MENU_NONE,
    MAIN_MENU_OPTION,
    MAIN_MENU_DATA,
    MAIN_MENU_PLAYER_SELECT,
    MAIN_MENU_MODE_SELECT,            // Mario GP - T. Trials - VS - Battle
    MAIN_MENU_MODE_SUB_SELECT,        // CC Selection and T.Trials options (Begin, Data)
    MAIN_MENU_OK_SELECT,              // OK option after selecting a submode
    MAIN_MENU_OK_SELECT_GO_BACK,      // OK option after going back from player select
    MAIN_MENU_MODE_SUB_SELECT_GO_BACK // Used in T.Trials after going back to Data option from track data
};

/**
 * @brief Options for gPlayerSelectMenuSelection
 */
enum PlayerSelectMenuSelectionTypes {
    PLAYER_SELECT_MENU_NONE,
    PLAYER_SELECT_MENU_MAIN,
    PLAYER_SELECT_MENU_OK,        // OK option after selecting characters
    PLAYER_SELECT_MENU_OK_GO_BACK // OK option after going back from map select
};

/**
 * @brief Options for gDebugMenuSelection
 */
enum DebugMenuSelectionTypes {
    DEBUG_MENU_NONE,
    DEBUG_MENU_DISABLED,
    DEBUG_MENU_DEBUG_MODE,
    DEBUG_MENU_COURSE,
    DEBUG_MENU_CC,
    DEBUG_MENU_SCREEN_MODE,
    DEBUG_MENU_PLAYER,
    DEBUG_MENU_SOUND_MODE,
    DEBUG_MENU_LAUNCH_EDITOR,
    DEBUG_MENU_GIVE_ALL_GOLD_CUP,
    DEBUG_MENU_OPTION_SELECTED = 0x40
};

/**
 * @brief Options for gControllerPakMenuSelection
 */
enum ControllerPakMenuSelectionTypes {
    CONTROLLER_PAK_MENU_NONE,
    CONTROLLER_PAK_MENU_SELECT_RECORD,
    CONTROLLER_PAK_MENU_END,
    CONTROLLER_PAK_MENU_ERASE,
    CONTROLLER_PAK_MENU_QUIT,
    CONTROLLER_PAK_MENU_TABLE_GAME_DATA,
    CONTROLLER_PAK_MENU_GO_TO_ERASING,
    CONTROLLER_PAK_MENU_ERASING,
    CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED,
    CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK,
    CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED
};

/**
 * @brief Options for gCourseRecordsMenuSelection
 */
enum CourseRecordsMenuSelectionTypes {
    COURSE_RECORDS_MENU_MIN,
    COURSE_RECORDS_MENU_RETURN_MENU = COURSE_RECORDS_MENU_MIN,
    COURSE_RECORDS_MENU_ERASE_RECORDS,
    COURSE_RECORDS_MENU_ERASE_GHOST,
    COURSE_RECORDS_MENU_MAX = COURSE_RECORDS_MENU_ERASE_GHOST
};

/**
 * @brief Options for gCourseRecordsSubMenuSelection
 */
enum CourseRecordsSubMenuSelectionTypes {
    COURSE_RECORDS_SUB_MENU_MIN,
    COURSE_RECORDS_SUB_MENU_QUIT = COURSE_RECORDS_SUB_MENU_MIN,
    COURSE_RECORDS_SUB_MENU_ERASE,
    COURSE_RECORDS_SUB_MENU_MAX = COURSE_RECORDS_SUB_MENU_ERASE
};

/**
 * @brief Options for gDebugGotoScene
 */
enum DebugGotoSceneTypes {
    DEBUG_GOTO_RACING,
    DEBUG_GOTO_ENDING,
    DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT,
    DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA
};

/**
 * @brief Options for gFadeModeSelection
 */
enum FadeModeSelectionTypes { FADE_MODE_NONE, FADE_MODE_MAIN, FADE_MODE_LOGO };

/**
 * @brief Options for gControllerPakScrollDirection
 */
enum ControllerPakScrollDirectionTypes {
    CONTROLLER_PAK_SCROLL_DIR_NONE,
    CONTROLLER_PAK_SCROLL_DIR_DOWN,
    CONTROLLER_PAK_SCROLL_DIR_UP
};

#define NUM_ROWS_GAME_MODE_MENU 4
#define NUM_COLUMN_GAME_MODE_SUB_MENU 4
#define NUM_ROWS_GAME_MODE_SUB_MENU 3

// This really, really shouldn't be in this header file, but I don't know where else to put it
void rmonPrintf(const char*, ...);

/* functions */
void update_menus(void);
void options_menu_act(struct Controller*, u16);
void data_menu_act(struct Controller*, u16);
void course_data_menu_act(struct Controller*, u16);
void logo_intro_menu_act(struct Controller*, u16);
void controller_pak_menu_act(struct Controller*, u16);
void splash_menu_act(struct Controller*, u16);
void setup_game_mode_selected(void);
void main_menu_act(struct Controller*, u16);
bool is_character_spot_free(s32);
void player_select_menu_act(struct Controller* controller, u16 arg1);
void course_select_menu_act(struct Controller*, u16);
void load_menu_states(s32);
void reset_cycle_flash_menu(void);
void set_sound_mode(void);
bool is_screen_being_faded(void);

/* data */
extern s32 gIntroModelZEye;
extern f32 gIntroModelScale;
extern f32 gIntroModelRotX;
extern f32 gIntroModelRotY;
extern f32 gIntroModelRotZ;
extern f32 gIntroModelPosX;
extern f32 gIntroModelPosY;
extern f32 gIntroModelPosZ;
extern s32 gMenuFadeType;

extern s8 gCharacterGridSelections[];
extern bool gCharacterGridIsSelected[];
extern s8 gSubMenuSelection;
extern s8 gMainMenuSelection;
extern s8 gPlayerSelectMenuSelection;
extern s8 gDebugMenuSelection;
extern s8 gControllerPakMenuSelection;
extern s8 gScreenModeListIndex;
extern u8 gSoundMode;
extern s8 gPlayerCount;
extern s8 gVersusResultCursorSelection;
extern s8 gTimeTrialsResultCursorSelection;
extern s8 gBattleResultCursorSelection;
extern s8 gTimeTrialDataCourseIndex;
extern s8 gCourseRecordsMenuSelection;
extern s8 gCourseRecordsSubMenuSelection;
extern s8 gDebugGotoScene;
extern bool gGhostPlayerInit;
extern bool gTrackMapInit;
extern s32 gMenuTimingCounter;
extern s32 gMenuDelayTimer;
extern s8 gDemoUseController;
extern s8 gCupSelection;
extern s8 sTempCupSelection;
extern s8 gCourseIndexInCup;
extern s8 unref_D_8018EE0C;

extern s32 gMenuSelection; // D_800E86A0
extern s32 gFadeModeSelection;
extern s8 gCharacterSelections[];

extern s8 gGameModeMenuColumn[];
extern s8 gGameModeSubMenuColumn[NUM_COLUMN_GAME_MODE_SUB_MENU][NUM_ROWS_GAME_MODE_SUB_MENU];
extern s8 gNextDemoId;
extern s8 gControllerPakSelectedTableRow;
extern s8 gControllerPakVisibleTableRows[];
extern s8 gControllerPakScrollDirection;
extern s8 unref_D_800E86D4[];
extern u32 sVIGammaOffDitherOn;

extern const s8 sScreenModePlayerTable[5];
extern const s8 sScreenModePlayerCount[5];
extern const s8 gPlayerModeSelection[];
extern const s8 sGameModePlayerColumnDefault[][3];
extern const s8 sGameModePlayerColumnExtra[][3];
extern const s32 gGameModePlayerSelection[][3];
extern const s8 sCharacterGridOrder[8];
extern const s16 gCupCourseOrder[NUM_CUPS][NUM_COURSES_PER_CUP];

extern const union GameModePack sSoundMenuPack;

// end of menus.c variables

#endif /* MENUS_H */
