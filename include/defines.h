#ifndef DEFINES_H
#define DEFINES_H

/**
 * @brief Options for Controller Pak state
 */
#define OK 1
#define BAD 0

/**
 * @brief Enable debug mode
 *
 * Press start to skip through menus
 *
 * Toggle resource meters by holding R and tapping B. L must not be held.
 *
 * Reset to start screen by holding A, B, R, and L.
 *
 * View player direction and currentPathPoint in a single player race during staging tap L while holding A and B.
 * Turn off this UI by tapping R while holding A and B.
 * @bug This looks like it should work at any point in the race.
 *
 * Set player 1 to the final lap by tapping D-pad up.
 * Set player 1 and player 2 to the final lap by tapping D-pad right.
 * Set player all players to the final lap by tapping D-pad down.
 *
 * Immediately start the race if any controller presses Z.
 *
 * As the ceremony cutscene starts hold a C or D-pad to switch characters.
 * C UP        = LUIGI
 * C LEFT      = YOSHI
 * C RIGHT     = TOAD
 * C DOWN      = DK
 * D-pad UP    = WARIO
 * D-pad LEFT  = PEACH
 * D-pad RIGHT = BOWSER
 * D-pad DOWN  = MARIO
 *
 */
#if defined(GCC) || defined(DEBUG)
#define DEBUG_MODE 1
#define DEBUG_MENU 2
#else
#define DEBUG_MODE 1
#define DEBUG_MENU 2
#endif

#define HOLD_ALL_DPAD_AND_C_BUTTONS \
    (U_JPAD | L_JPAD | R_JPAD | D_JPAD | U_CBUTTONS | L_CBUTTONS | R_CBUTTONS | D_CBUTTONS)

/**
 * @brief Jump to demo mode from the debug menu using L and A
 */
#define DEMO_MODE_ACTIVE 1
#define DEMO_MODE_INACTIVE 0

#ifdef VERSION_EU
#define COURSE_TIMER_ITER 0.020041665999999999    // 1 / 50
#define COURSE_TIMER_ITER_f 0.020041665999999999f // 1 / 50
#else
#define COURSE_TIMER_ITER 0.01666666    // 1 / 60
#define COURSE_TIMER_ITER_f 0.01666666f // 1 / 60
#endif

#define V_BlANK_TIMER_ITER 0.01666666

/**
 * Racing terms:
 * Staging means aligning a racecar to the starting line.
 * Start sequence means waiting for the light to turn green.
 * Used in the Player struct's 'type' member: player->type
 */
#define PLAYER_INACTIVE 0                 // 0x0000
#define PLAYER_UNKNOWN_0x40 (1 << 6)      // 0x0040
#define PLAYER_INVISIBLE_OR_BOMB (1 << 8) // 0x0100
#define PLAYER_STAGING (1 << 9)           // 0x0200
#define PLAYER_UNKNOWN (1 << 10)          // 0x0400 // unused ?
#define PLAYER_CINEMATIC_MODE (1 << 11)   // 0x0800 // Also used to track eliminations in Battle mode.
#define PLAYER_CPU (1 << 12)              // 0x1000
#define PLAYER_START_SEQUENCE (1 << 13)   // 0x2000
#define PLAYER_HUMAN (1 << 14)            // 0x4000
#define PLAYER_EXISTS (1 << 15)           // 0x8000

// Compiles to -0x1000 in diff.py
#define PLAYER_HUMAN_AND_CPU PLAYER_EXISTS | PLAYER_HUMAN | PLAYER_CPU | PLAYER_START_SEQUENCE

#define ZERO_PLAYERS_SELECTED 0
#define ONE_PLAYERS_SELECTED 1
#define TWO_PLAYERS_SELECTED 2
#define THREE_PLAYERS_SELECTED 3
#define FOUR_PLAYERS_SELECTED 4
#define SELECTED_PLAYER_DEFINES_TOTAL 5

enum PlayerId {
    PLAYER_NONE = -1,
    PLAYER_ONE = 0,
    PLAYER_TWO = 1,
    PLAYER_THREE = 2,
    PLAYER_FOUR = 3,
    PLAYER_FIVE = 4,
    PLAYER_SIX = 5,
    PLAYER_SEVEN = 6,
    PLAYER_EIGHT = 7
};

#define NUM_PLAYERS 8

#define MARIO_SIZE 0.75f
#define LUIGI_SIZE 0.75f
#define YOSHI_SIZE 0.75f
#define TOAD_SIZE 0.75f
#define DK_SIZE 0.75f
#define WARIO_SIZE 0.75f
#define PEACH_SIZE 0.75f
#define BOWSER_SIZE 0.75f

// 2P Game has Grand Prix, VS, and Battle as available game types
#define MAX_NUM_MAIN_MENU_GAME_TYPES 3

/**
 * @brief Options for gModeSelection
 */
#define GRAND_PRIX 0
#define TIME_TRIALS 1
#define VERSUS 2
#define BATTLE 3

/**
 * @brief Options for gCCSelection
 * CC stands for cubic-centimetres.
 * It measures engine displacement composed from
 * cylinder volume.
 * Generally, the main determiner of horsepower output.
 */

#define CC_50 0
#define CC_100 1
#define CC_150 2
#define CC_EXTRA 3
#define CC_BATTLE 4

/**
 * @brief Options for gCupSelection
 * There is a "cup" for battle mode, probably so that
 * the battle courses could be displayed in the same
 * way race courses are.
 */
enum { MUSHROOM_CUP, FLOWER_CUP, STAR_CUP, SPECIAL_CUP, BATTLE_CUP, NUM_CUPS };

#define NUM_COURSES_PER_CUP 4

/**
 * @brief Options for gCourseIndexInCup
 */
enum { COURSE_ONE, COURSE_TWO, COURSE_THREE, COURSE_FOUR };

/**
 * @brief Character IDs
 * Note that these are not the same as the values
 * found in gCharacterGridSelections as those are
 * ordered by table
 */
#define MARIO 0
#define LUIGI 1
#define YOSHI 2
#define TOAD 3
#define DK 4
#define WARIO 5
#define PEACH 6
#define BOWSER 7

/**
 * @brief Options for gMenuSelection
 */
#define OPTIONS_MENU 5
#define DATA_MENU 6
#define COURSE_DATA_MENU 7
#define LOGO_INTRO_MENU 8
#define CONTROLLER_PAK_MENU 9
#define START_MENU 10
#define MAIN_MENU 11
#define CHARACTER_SELECT_MENU 12
#define COURSE_SELECT_MENU 13
#define RACING_DUPLICATE 14
#define HARBOUR_MASTERS_MENU 15

/**
 * @brief Options for gGameState gGotoMode and gGotoMenu
 */
#define START_MENU_FROM_QUIT 0
#define MAIN_MENU_FROM_QUIT 1
#define PLAYER_SELECT_MENU_FROM_QUIT 2
#define COURSE_SELECT_MENU_FROM_QUIT 3
#define RACING 4
#define ENDING 5
#define CREDITS_SEQUENCE 9

/**
 * @brief Options for gRaceState
 */
#define RACE_INIT 0
#define RACE_SETUP 1
#define RACE_STAGING 2
#define RACE_IN_PROGRESS 3
#define RACE_CALCULATE_RANKS 4
#define RACE_FINISHED 5 // End of race and score screen
#define RACE_UNK 6
#define RACE_EXIT 7

/**
 * @brief Options for gScreenModeSelection and gActiveScreenMode
 */
#define SCREEN_MODE_1P 0
#define SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL 1
#define SCREEN_MODE_2P_SPLITSCREEN_VERTICAL 2
#define SCREEN_MODE_3P_4P_SPLITSCREEN 3
#define NUM_SCREEN_MODES 4

/**
 * @brief Indexes for accessing Time Trial records
 */
#define TIME_TRIAL_3LAP_RECORD_1 0
#define TIME_TRIAL_3LAP_RECORD_2 1
#define TIME_TRIAL_3LAP_RECORD_3 2
#define TIME_TRIAL_3LAP_RECORD_4 3
#define TIME_TRIAL_3LAP_RECORD_5 4
#define TIME_TRIAL_1LAP_RECORD 5

/**
 * @brief Text color options
 * The 2 cycling options cycle through Blue -> Green -> Red
 * Cycle 2 appears to be one step ahead of Cycle 1
 * Other values like 6 and 7 sort of work, they cause rapid
 * flashing between 2 colors, but also cause individual characters
 * to be cutoff on their right side
 */
enum COLOR_ID {
    TEXT_BLUE,
    TEXT_GREEN,
    TEXT_RED,
    TEXT_YELLOW,
    TEXT_BLUE_GREEN_RED_CYCLE_1,
    TEXT_BLUE_GREEN_RED_CYCLE_2
};

/**
 * @brief Sound mode options
 * Option 2 appears to be unused, as such its probably not
 * a valid option
 */
#define SOUND_STEREO 0
#define SOUND_HEADPHONES 1
#define SOUND_UNUSED 2
#define SOUND_MONO 3
#define NUM_SOUND_MODES 4

/**
 * @brief Title screen demo options
 * All demos use 100 CC
 */
#define DEMO_ONE 0   // Mario,                  Mario Raceway,   Grand Prix
#define DEMO_TWO 1   // Yoshi DK,               Choco Mountain,  Versus
#define DEMO_THREE 2 // Luigi,                  Kalamari Desert, Grand Prix
#define DEMO_FOUR 3  // Wario Yoshi Bowser,     Wario Stadium,   Versus
#define DEMO_FIVE 4  // Bowser,                 Bowser Castle,   Grand Prix
#define DEMO_SIX 5   // Mario Luigi Peach Toad, Sherbert Land,   Versus
#define NUM_DEMOS 6

/**
 * @brief Item IDs
 */
enum ITEMS {
    /* 0x00 */ ITEM_NONE = 0,
    /* 0x01 */ ITEM_BANANA,
    /* 0x02 */ ITEM_BANANA_BUNCH,
    /* 0x03 */ ITEM_GREEN_SHELL,
    /* 0x04 */ ITEM_TRIPLE_GREEN_SHELL,
    /* 0x05 */ ITEM_RED_SHELL,
    /* 0x06 */ ITEM_TRIPLE_RED_SHELL,
    /* 0x07 */ ITEM_BLUE_SPINY_SHELL,
    /* 0x08 */ ITEM_THUNDERBOLT,
    /* 0x09 */ ITEM_FAKE_ITEM_BOX,
    /* 0x0A */ ITEM_STAR,
    /* 0x0B */ ITEM_BOO,
    /* 0x0C */ ITEM_MUSHROOM,
    /* 0x0D */ ITEM_DOUBLE_MUSHROOM,
    /* 0x0E */ ITEM_TRIPLE_MUSHROOM,
    /* 0x0F */ ITEM_SUPER_MUSHROOM,
    /* 0x0G */ ITEM_MAX,
};

enum CPU_BEHAVIOURS {
    BEHAVIOUR_NONE = 0,
    BEHAVIOUR_1,
    BEHAVIOUR_HOP,
    BEHAVIOUR_DRIVE_CENTER,
    BEHAVIOUR_DRIVE_LEFT,
    BEHAVIOUR_DRIVE_OUTER,
    BEHAVIOUR_NORMAL_SPEED,
    BEHAVIOUR_FAST_SPEED,
    BEHAVIOUR_SLOW_SPEED,
    BEHAVIOUR_9,
    BEHAVIOUR_10,
    BEHAVIOUR_MAX_SPEED
};

enum DIRECTION { NORTH, EAST, SOUTH, WEST };

enum PLACE { FIRST_PLACE, SECOND_PLACE, THIRD_PLACE, FOURTH_PLACE };

/**
 * @brief Balloon status
 */
#define BALLOON_STATUS_GONE 0
#define BALLOON_STATUS_PRESENT 1
#define BALLOON_STATUS_DEPARTING 2

/**
 * Number of podiums in podium ceremony
 */
#define NUM_PODIUMS 3

/**
 * @brief Max representable time, 100 minutes measured in centiseconds
 */
#define MAX_TIME 0x927C0

/**
 * @brief sound effect of player's
 * for soundEffect
 */
#define HIT_SOUND_EFFECT 0x100                   // hitting an object
#define BOOST_SOUND_EFFECT 0x200                 // being boosted by trigger a mushroom
#define BOO_SOUND_EFFECT 0x800                   // being a boo
#define STAR_SOUND_EFFECT 0x2000                 // being a star
#define HIT_ROTATING_SOUND_EFFECT 0x4000         // hitting a rotating object
#define BOOST_RAMP_WOOD_SOUND_EFFECT 0x8000      // being boosted by a ramp
#define HOLD_BANANA_SOUND_EFFECT 0x40000         // holding a banana
#define REVERSE_SOUND_EFFECT 0x400000            // being in the wrong direction
#define BOOST_RAMP_ASPHALT_SOUND_EFFECT 0x800000 // being boosted by a boost pad
#define HIT_BY_ITEM_SOUND_EFFECT 0x1000000       // being hit by an item

/**
 * @brief effect of player's
 * for effects
 */
#define BOOST_RAMP_WOOD_EFFECT 0x4         // being boosted by a ramp
#define STAR_EFFECT 0x200                  // being a star
#define BOOST_EFFECT 0x2000                // being boosted by trigger a mushroom
#define BOOST_RAMP_ASPHALT_EFFECT 0x100000 // being boosted by a boost pad
#define REVERSE_EFFECT 0x400000            // being in reverse of the course
#define HIT_BY_ITEM_EFFECT 0x2000000       // being hit by an item
#define HIT_EFFECT 0x4000000               // hitting an object
#define LIGHTNING_EFFECT 0x40000000        // being hit by lightning
#define BOO_EFFECT 0x80000000              // being a boo

/**
 * @brief shell state
 *
 */
#define SPAWN_FIRST_SHELL 0
#define SPAWN_SECOND_SHELL 1
#define SPAWN_THIRD_SHELL 2

#define GPACK_RGB888(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define COLOR_LIGHT GPACK_RGB888(0x1C, 0x00, 0x00)
#define COLOR_LAVA GPACK_RGB888(0x34, 0x00, 0x00)
#define COLOR_BLACK GPACK_RGB888(0, 0, 0)

/**
 *
 * Collision mesh flags
 *
 */

#define GRID_SIZE 32

#define FACING_Y_AXIS 0x4000
#define FACING_X_AXIS 0x8000
#define FACING_Z_AXIS 0x2000

#endif // DEFINES_H

/**
 * 
 * Laps
 * 
 */
#define MIN_LAPS 0
#define MAX_LAPS 3