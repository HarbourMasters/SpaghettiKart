#ifndef DEFINES_H
#define DEFINES_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END   }
#else
#define EXTERN_C extern
#define EXTERN_C_START
#define EXTERN_C_END
#endif

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

#define ALL_BUTTONS                                                                                                   \
    (A_BUTTON | B_BUTTON | L_TRIG | R_TRIG | Z_TRIG | START_BUTTON | U_JPAD | L_JPAD | R_JPAD | D_JPAD | U_CBUTTONS | \
     L_CBUTTONS | R_CBUTTONS | D_CBUTTONS)

/**
 * Replay controller buttons
 * Used for time trial replays (including staff and player ghosts)
 * Each entry is converted to a u32 value
 * This allows access to the button struct member
 */
#define REPLAY_A_BUTTON (1 << 31) // 0x80000000
#define REPLAY_B_BUTTON (1 << 30) // 0x40000000
#define REPLAY_Z_TRIG (1 << 29)   // 0x20000000
#define REPLAY_R_TRIG (1 << 28)   // 0x10000000

#define REPLAY_FRAME_COUNTER 0xFF0000
#define REPLAY_CLEAR_FRAME_COUNTER (0xFFFFFFFF & ~REPLAY_FRAME_COUNTER)
#define REPLAY_STICK_Y 0xFF00
#define REPLAY_STICK_X 0xFF
#define REPLAY_FRAME_INCREMENT 0x10000

/**
 * @brief Jump to demo mode from the debug menu using L and A
 */
#define DEMO_MODE_ACTIVE 1
#define DEMO_MODE_INACTIVE 0

#ifdef VERSION_EU
#define TRACK_TIMER_ITER 0.020041665999999999    // 1 / 50
#define TRACK_TIMER_ITER_f 0.020041665999999999f // 1 / 50
#else
#define TRACK_TIMER_ITER 0.01666666    // 1 / 60
#define TRACK_TIMER_ITER_f 0.01666666f // 1 / 60
#endif

#define V_BlANK_TIMER_ITER 0.01666666

/**
 * Racing terms:
 * Staging means aligning a racecar to the starting line.
 * Start sequence means waiting for the light to turn green.
 * Used in the Player struct's 'type' member: player->type
 */
#define PLAYER_INACTIVE 0                 // 0x0000
#define PLAYER_UNKNOWN_0x10 (1 << 4)      // 0x0010 // unused?
#define PLAYER_UNKNOWN_0x40 (1 << 6)      // 0x0040
#define PLAYER_UNKNOWN_0x80 (1 << 7)      // 0x0080 // UNUSED
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

// Camera index into cameras array
enum CameraId {
    CAMERA_ONE = 0,
    CAMERA_TWO,
    CAMERA_THREE,
    CAMERA_FOUR,
    CAMERA_FREECAM
};

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
enum { TRACK_ONE, TRACK_TWO, TRACK_THREE, TRACK_FOUR };

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
 */
#define SOUND_STEREO 0
#define SOUND_HEADPHONES 1
#define SOUND_SURROUND 2
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
#define DEGREES_CONVERSION_FACTOR 182

// player->oobProps
/* Deals with the lower out of bounds (OOB) plane on levels. Represented by fluids (water / lava)
  or nothing for Rainbow Road and Skyscraper. */
#define UNDER_OOB_OR_FLUID_LEVEL 0x1 // Set while mostly under the plane. Does not necessarily trigger Lakitu on Koopa Troopa Beach.
#define PASS_OOB_OR_FLUID_LEVEL 0x2 // Set when passing through the lower plane in either direction
// The next two are also activated when passing through the lower plane.
#define UNDER_FLUID_LEVEL 0x4 // Stays active until Lakitu places back on track
#define UNDER_OOB_LEVEL 0x8 // Active while under a non-fluid OOB plane. Is momentarily active when passing through fluids.


/* UNK_002 has something to do with player animations. Each player has a 32-bit
flag broken into 8 groups of 4 bits. Those 4 bits affect how each of the 8 players
appear to the specified player */
#define CHANGING_ANIMATION 0x1 // Seems to be set when the kart animation has to change.
#define UNK_002_UNKNOWN_0x2 0x2 
#define UNK_002_UNKNOWN_0x4 0x4 /* Unclear, but has to do with viewing the side of player. At least tends to change if target
player spins. Something  with avoding rollover of aniamation frame data? */
#define SIDE_OF_KART 0x8 // Seems to be whether you are in a rectangle shooting out from both sides of target player

#define WHISTLE 0x20     // Whistle spinout save graphic
#define CRASH 0x40       // Crash! graphic (vertical tumble)
#define WHIRRR 0x80      // Whirrr! graphic (spinning out)
#define POOMP 0x100      // Poomp! graphic (landing from a height)
#define BOING 0x800      // Boing! graphic (hopping)
#define EXPLOSION 0x1000 // Big shock looking graphic when starting tumble

// player->lakituProps
#define LAKITU_RETRIEVAL 0x1 // While lakitu is grabbing you, but before the scene transition of being placed on the track
#define HELD_BY_LAKITU   0x2
#define LAKITU_FIZZLE    0x4 // Disintegration and reintegration effect when transitioning from retrieval to placement
#define LAKITU_SCENE     0x8 // the whole segment from when lakitu is called to when you regain control
#define FRIGID_EFFECT   0x10 // Cold colors on Sherbet Land after in frigid water
#define THAWING_EFFECT  0x20 // Regaining usual colors post frigid effect
#define FROZEN_EFFECT   0x80 // In the ice cube
#define WENT_OVER_OOB  0x100 // Player went over (or is on) an OOB area. Cancelled if touch back in bounds
#define LAKITU_LAVA   0x1000 // smoky effect when retrieved from lava
#define LAKITU_WATER  0x2000 // dripping effect when retreived from water

// player->kartProps
#define BACK_UP               0x1
#define RIGHT_TURN            0x2 // non-drifting (more than 5 degrees)
#define LEFT_TURN             0x4 // non-drifting (more than 5 degrees)
#define MOVE_BACKWARDS        0x8 // includes lakitu
#define LOSE_GP_RACE         0x10 // pointless, only unsets itself
#define THROTTLE             0x20 // Closely tied to just pressing A. Possible exception for AB-spins
#define EARLY_SPINOUT_RIGHT  0x40 // Spinning out while facing right (not actually used for anything)
#define EARLY_SPINOUT_LEFT   0x80 // Spinning out while facing left
#define POST_TUMBLE_GAS     0x100 // Causes particles after a vertical tumble, I think
#define BECOME_INVISIBLE    0x200
#define UNUSED_0x400        0x400 // locked behind 0x800 (func_80091440)
#define UNUSED_0x800        0x800 // locked behind 0x400 (func_8002B830 -> func_800911B4)
#define UNUSED_0x1000      0x1000 // 0x1000 locked behind 0x400 (func_8002B830 -> func_800911B4)
#define UNUSED_0x2000      0x2000 // 0x2000 locked behind 0x400 and 0x800 (func_8002B830 -> func_800911B4, apply_effect -> func_80091298,
                                  // func_80091440)
#define DRIVING_SPINOUT    0x4000
#define UNKNOWN_BATTLE_VAR 0x8000 // 0x8000 something battle related, unclear if ever set

/**
 * @brief triggers indicating that an effect should be applied to a kart
 */
#define HIT_BANANA_TRIGGER              0x1 // hits a banana
#define HIGH_TUMBLE_TRIGGER             0x2 // hit by a red shell, blue shell, or hit a mole
#define LOW_TUMBLE_TRIGGER              0x4 // hit by a green shell
#define DRIVING_SPINOUT_TRIGGER        0x80 // spinning out from erratic driving
#define THWOMP_SQUISH_TRIGGER         0x100 // stomped by thwomp
#define SHROOM_TRIGGER                0x200 // being boosted by trigger a mushroom
#define BOO_TRIGGER                   0x800 // being a boo
#define UNUSED_TRIGGER_0x1000        0x1000 // Unused
#define STAR_TRIGGER                 0x2000 // Starting a star
#define LIGHTNING_STRIKE_TRIGGER     0x4000 // Struck by lightning
#define BOOST_RAMP_WOOD_TRIGGER      0x8000 // being boosted by a ramp
#define UNUSED_TRIGGER_0x20000      0x20000 // Unused
#define DRAG_ITEM_EFFECT            0x40000 // holding a non-shell item behind you
#define HIT_PADDLE_BOAT_TRIGGER     0x80000 // hit paddle boat
#define UNUSED_TRIGGER_0x10000     0x100000 // Unused
#define SPINOUT_TRIGGER            0x200000 // hit crab or spiny spinout or losing versus race
#define VERTICAL_TUMBLE_TRIGGER    0x400000 // hitting a fake item / bomb / snowman / car / train
#define BOOST_RAMP_ASPHALT_TRIGGER 0x800000 // being boosted by a boost pad
#define HIT_BY_STAR_TRIGGER       0x1000000 // being hit by a star
#define START_BOOST_TRIGGER       0x2000000 // Start boost
#define LOSE_BATTLE_EFFECT        0x4000000 // When losing battle mode
#define BECOME_BOMB_EFFECT        0x8000000 // When becoming a bomb in battle mode
#define START_SPINOUT_TRIGGER    0x10000000 // Spinning out by holding gas at start of race

#define ALL_TRIGGERS (0xFFFFFFFF)
#define RACING_SPINOUT_TRIGGERS (SPINOUT_TRIGGER | DRIVING_SPINOUT_TRIGGER | HIT_BANANA_TRIGGER) // 0x200081
#define RAMP_BOOST_TRIGGERS (BOOST_RAMP_ASPHALT_TRIGGER | BOOST_RAMP_WOOD_TRIGGER) // 0x00808000
#define ANY_BOOST_TRIGGERS (RAMP_BOOST_TRIGGERS | SHROOM_TRIGGER) // 0x00808200
#define STATE_TRANSITION_TRIGGERS (STAR_TRIGGER | BOO_TRIGGER | UNUSED_TRIGGER_0x1000 | UNUSED_TRIGGER_0x20000)// 0x00023800
#define HIT_TRIGGERS (HIT_BY_STAR_TRIGGER | VERTICAL_TUMBLE_TRIGGER | \
    LIGHTNING_STRIKE_TRIGGER | LOW_TUMBLE_TRIGGER | HIGH_TUMBLE_TRIGGER | THWOMP_SQUISH_TRIGGER) // 0x01404106

/**
 * @brief effect of player's
 * for effects
 */
#define UNKNOWN_EFFECT_0x1 0x1               //
#define BOOST_RAMP_WOOD_EFFECT 0x4           // being boosted by a ramp
#define DRIFTING_EFFECT 0x10                 // drifting
#define UNKNOWN_EFFECT_0x40 0x40             //
#define UNKNOWN_EFFECT_0x80 0x80             //
#define UNKNOWN_EFFECT_0xC 0xC               //
#define UNKNOWN_EFFECT_0x100 0x100           //
#define UNKNOWN_EFFECT_0x1000 0x1000         //
#define STAR_EFFECT 0x200                    // being a star
#define BOOST_EFFECT 0x2000                  // being boosted by trigger a mushroom
#define UNKNOWN_EFFECT_0x10000 0x10000       //
#define BOOST_RAMP_ASPHALT_EFFECT 0x100000   // being boosted by a boost pad
#define UNKNOWN_EFFECT_0x200000 0x200000     //
#define REVERSE_EFFECT 0x400000              // Player is facing the wrong direction
#define UNKNOWN_EFFECT_0x1000000 0x1000000   //
#define HIT_BY_ITEM_EFFECT 0x2000000         // being hit by an item
#define HIT_EFFECT 0x4000000                 // hitting an object
#define UNKNOWN_EFFECT_0x10000000 0x10000000 //
#define LIGHTNING_EFFECT 0x40000000          // being hit by lightning
#define BOO_EFFECT 0x80000000                // being a boo

/**
 * @brief durations of effects
 */
#define STAR_EFFECT_DURATION 0xA
#define BOO_EFFECT_DURATION 0x7

/**
 * @brief alpha relates values
 */
#define ALPHA_MAX 0xFF
#define ALPHA_MIN 0x0
#define ALPHA_BOO_EFFECT 0x60

/**
 * @brief durations of effects
 */
#define STAR_EFFECT_DURATION 0xA
#define BOO_EFFECT_DURATION 0x7

/**
 * @brief alpha related values
 */
#define ALPHA_MAX 0xFF
#define ALPHA_MIN 0x0
#define ALPHA_BOO_EFFECT 0x60

#define ALPHA_CHANGE_LARGE 8
#define ALPHA_CHANGE_MEDIUM 4
#define ALPHA_CHANGE_SMALL 2

/**
 * @brief shell state
 *
 */
#define SPAWN_FIRST_SHELL 0
#define SPAWN_SECOND_SHELL 1
#define SPAWN_THIRD_SHELL 2
#define SHELL_COLLISION 3 // Activated when triple shells have spawned
#define ORBIT_PLAYER 4

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
