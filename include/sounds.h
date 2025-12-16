#ifndef SOUNDS_H
#define SOUNDS_H

//! @todo format like sm64 sounds.h
// The sound cue bit fields can be split up into its basic pieces and put back together.

// Sound Magic Definition:
// First Byte (Upper Nibble): Sound Bank (not the same as audio bank!)
// First Byte (Lower Nibble): Bitflags for audio playback?
// Third/Second Byte: Priority
// Third/Second Byte (Upper Nibble): More bitflags
// Third/Second Byte (Lower Nibble): Sound Status (this is set to SOUND_STATUS_PLAYING when passed to the audio driver.)
// Fourth Byte: Sound ID
#define SOUND_ARG_LOAD(sound_bank, byte2, byte3, sound_id) \
    ((sound_bank << 24) | (byte2 << 16) | (byte3 << 8) | sound_id)

/* Intro */
#define SOUND_INTRO_LOGO SOUND_ARG_LOAD(0x49, 0x01, 0x80, 0x08) // SOUND_ARG_LOAD(0x49, 0x01, 0x80, 0x08)
// Welcome to Mario Kart. Used in intro and credits.
#define SOUND_INTRO_WELCOME SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x09)
#define SOUND_INTRO_ENTER_MENU SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1A)

/* Menus */
#define SOUND_MENU_OK_CLICKED SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x16)

#define SOUND_MENU_CURSOR_MOVE SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x00)
#define SOUND_MENU_GO_BACK SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x02)
// Used elsewhere, seems same as above sound.
#define SOUND_ACTION_GO_BACK_2 SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x05)

#define SOUND_ACTION_UNKNOWN_CONFIRMATION SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x06)

#define SOUND_MENU_SELECT SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x01)

// Mario Grand Prix
#define SOUND_MENU_GP SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0A)
// Versus
#define SOUND_MENU_VERSUS SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0C)
// Time Trials
#define SOUND_MENU_TIME_TRIALS SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0B)
// Battle
#define SOUND_MENU_BATTLE SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0D)
// Select a level (Select a CC)
#define SOUND_MENU_SELECT_LEVEL SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0E)
// OK?
#define SOUND_MENU_OK SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x0F)

// Select your player
#define SOUND_MENU_SELECT_PLAYER SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x12)

// Select map
#define SOUND_MENU_SELECT_MAP SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x13)

// Plays in option if no controller pak.
#define SOUND_MENU_FILE_NOT_FOUND SOUND_ARG_LOAD(0x49, 0x00, 0xFF, 0x07)

// Option
#define SOUND_MENU_OPTION SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x10)
// Data
#define SOUND_MENU_DATA SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x11)

// On delete course record?
#define SOUND_MENU_EXPLOSION SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1D)

#define SOUND_MENU_STEREO SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x24)
#define SOUND_MENU_HEADPHONES SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x25)
#define SOUND_MENU_MONO SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x29)

/* Staging */
// Red and orange staging lights that Lakitu holds.
#define SOUND_ACTION_COUNTDOWN_LIGHT SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x03)
#define SOUND_ACTION_GREEN_LIGHT SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x04)
// Activates when light turns green.
#define SOUND_ACTION_REV_ENGINE SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x18)
#define SOUND_ACTION_REV_ENGINE_2 SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x19)

/* Racing */

// Volume Mode and unknown
#define SOUND_ACTION_PING SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1C)

#define SOUND_ACTION_TYRE_SQUEAL SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x08)

// Falling rock and bomb sound.
#define SOUND_ACTION_EXPLOSION SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x05)

#define SOUND_ACTION_EXPLOSION_2 SOUND_ARG_LOAD(0x19, 0x00, 0xF0, 0x0C)

/* Items */
// There's likely more of these but they are in func arguments.
// Did not rename in-case unrelated.
#define SOUND_ITEM_STAR SOUND_ARG_LOAD(0x31, 0x02, 0x90, 0x08)
#define SOUND_ITEM_THUNDERBOLT SOUND_ARG_LOAD(0x51, 0x01, 0xC0, 0x0C)

/* Score Screen */
// Coin pickup sound (perfect fourth; B to E)
#define SOUND_ACTION_COUNT_SCORE SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x17)
#define SOUND_ACTION_NEXT_COURSE SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x15)
// Same sound as ENTER_MENU
#define SOUND_ACTION_CONTINUE_UNKNOWN SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1B)

/* Ceremony */
// Congratulation
#define SOUND_CEREMONY_CONGRATULATION SOUND_ARG_LOAD(0x49, 0x00, 0x90, 0x14)
#define SOUND_CEREMONY_BALLOON_POP SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1E)
#define SOUND_CEREMONY_FISH SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x1F)
#define SOUND_CEREMONY_FISH_2 SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x20)
#define SOUND_CEREMONY_SHOOT_TROPHY SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x21)
#define SOUND_CEREMONY_PODIUM SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x22)

// The sound of the trophy. Plays in the background
#define SOUND_CEREMONY_TROPHY SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x23)

/* Credits */
// Hey, you're very good. See you next time!
#define SOUND_CREDITS_FAREWELL SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x26)

enum MusicSeq {
    MUSIC_SEQ_UNKNOWN = -1,
    MUSIC_SEQ_SOUND_PLAYER,
    MUSIC_SEQ_TITLE_SCREEN,
    MUSIC_SEQ_MAIN_MENU,
    MUSIC_SEQ_WARIO_STADIUM,
    MUSIC_SEQ_MOO_MOO_FARM,
    MUSIC_SEQ_CHOCO_MOUNTAIN,
    MUSIC_SEQ_KOOPA_TROOPA_BEACH,
    MUSIC_SEQ_BANSHEE_BOARDWALK,
    MUSIC_SEQ_FRAPPE_SNOWLAND,
    MUSIC_SEQ_BOWSERS_CASTLE,
    MUSIC_SEQ_KALIMARI_DESERT,
    MUSIC_SEQ_START_GRID_GP_VS,
    MUSIC_SEQ_FINAL_LAP_FANFARE,
    MUSIC_SEQ_FINISH_1ST_PLACE,
    MUSIC_SEQ_FINISH_2ND_4TH_PLACE,
    MUSIC_SEQ_FINISH_5TH_8TH_PLACE,
    MUSIC_SEQ_WINNING_RESULTS,
    MUSIC_SEQ_STAR_JINGLE,
    MUSIC_SEQ_RAINBOW_ROAD,
    MUSIC_SEQ_DK_JUNGLE,
    MUSIC_SEQ_GAME_OVER,
    MUSIC_SEQ_TOADS_TURNPIKE,
    MUSIC_SEQ_START_GRID_TIME_ATTACK,
    MUSIC_SEQ_VS_BATTLE_RESULTS,
    MUSIC_SEQ_LOSING_RESULTS,
    MUSIC_SEQ_BATTLE_ARENAS,
    MUSIC_SEQ_AWARD_CEREMONY_BUILDUP,
    MUSIC_SEQ_AWARD_CEREMONY_1ST_3RD,
    MUSIC_SEQ_STAFF_ROLL,
    MUSIC_SEQ_AWARD_CEREMONY_4TH_8TH,
    MUSIC_SEQ_LUIGI_RACEWAY,
    MUSIC_SEQ_MARIO_RACEWAY,
    MUSIC_SEQ_ROYAL_RACEWAY,
    MUSIC_SEQ_YOSHI_VALLEY,
    MUSIC_SEQ_BLOCK_FORT,
    MUSIC_SEQ_DOUBLE_DECK,
    MUSIC_SEQ_MAX
};

#endif // SOUNDS_H
