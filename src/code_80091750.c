#include <libultraship.h>
#include <macros.h>
#include <string.h>
#include <mk64.h>
#include <stubs.h>
#include <defines.h>
#include <segments.h>
#include <libultra/types.h>
#include <sounds.h>
#include <assets/texture_data_2.h>
#include "code_800029B0.h"
#include "code_80091750.h"
#include "code_80005FD0.h"
#include "code_8006E9C0.h"
#include "menus.h"
#include "save.h"
#include "code_80057C60.h"
#include "credits.h"
#include "data_segment2.h"
#include "code_800AF9B0.h"
#include "code_80281780.h"
#include "memory.h"
#include "audio/external.h"
#include "render_objects.h"
#include "staff_ghosts.h"
#include <assets/common_data.h>
#include "textures.h"
#include "math_util.h"
#include "save_data.h"
#include "podium_ceremony_actors.h"
#include "skybox_and_splitscreen.h"
#include <assets/startup_logo.h>
#include "buffers.h"
#include "racing/race_logic.h"
#include "ending/code_80281C40.h"
#include "spawn_players.h"
#include "render_player.h"
#include "decode.h"
//! @todo Move gGfxPool out of main.h
// Unfortunately that's not a small effort due to weird import structure in this project
#include "main.h"

#include "stdio.h"
#include "port/Engine.h"

u16* D_8018D9B0;
u8* D_8018D9B4;
u8* D_8018D9B8;
u8* D_8018D9BC;
void* D_8018D9C0;
/**
 * List of bytes indexed by character ID
 * Indicates number of Grand Prix points that character
 * has scored
 */
s8 gGPPointsByCharacterId[8];
s8 gCharacterIdByGPOverallRank[8];
s8 D_8018D9D8;
s8 D_8018D9D9;
struct_8018D9E0_entry D_8018D9E0[D_8018D9E0_SIZE];
struct_8018DEE0_entry D_8018DEE0[D_8018DEE0_SIZE];
struct_8018E060_entry D_8018E060[D_8018E060_SIZE + 1];
struct_8018E0E8_entry D_8018E0E8[D_8018E0E8_SIZE];
s32 gD_8018E118TotalSize;
struct_8018E118_entry D_8018E118[D_8018E118_SIZE];
s32 gNumD_8018E118Entries;
Gfx* D_8018E75C;
s32 gNumD_8018E768Entries;
struct_8018E768_entry D_8018E768[D_8018E768_SIZE];
s32 gCycleFlashMenu;
s8 D_8018E7AC[5];
u32 D_8018E7B8[5];
u32 D_8018E7D0[4];
s32 D_8018E7E0;
struct UnkStruct_8018E7E8 D_8018E7E8[D_8018E7E8_SIZE];
struct UnkStruct_8018E7E8 D_8018E810[D_8018E810_SIZE];
s8 D_8018E838[4]; // Import to C was required for matching.
s32 D_8018E83C;

s32 D_8018E840[4]; // This may all be one big array.
s32 D_8018E850[2]; // This is probably incorrect. Fix after decomping code.
s32 D_8018E858[2];
s8 gTextColor;
s32 D_8018E864_pad;
OSPfs gControllerPak1FileHandle;
OSPfs gControllerPak2FileHandle;
OSPfsState pfsState[16];
s32 pfsError[16]; // 0 = Ok, anything else = error.
s32 gControllerPak1NumFilesUsed;
s32 gControllerPak1MaxWriteableFiles;

s32 gControllerPak1NumPagesFree;
s32 gControllerPak1FileNote;
s32 gControllerPak2FileNote;
s32 code_80091750_bss_pad2;
ALIGNED8 SaveData gSaveData;

u8 D_8018ED90;
u8 D_8018ED91;
s32 s8018ED94;

f32 D_8018ED98; // Rotation
f32 D_8018ED9C; // Rotation
f32 D_8018EDA0; // Rotation

f32 D_8018EDA4;
f32 D_8018EDA8;
f32 D_8018EDAC;

Unk_D_800E70A0 D_800E70A0[] = {
    { 0x3d, 0x11, 0x00, 0x00 }, { 0x15, 0x3e, 0x00, 0x00 }, { 0x5c, 0x3e, 0x00, 0x00 },
    { 0xa3, 0x3e, 0x00, 0x00 }, { 0xea, 0x3e, 0x00, 0x00 }, { 0x10a, 0xc8, 0x00, 0x00 },
    { 0x15, 0xc8, 0x00, 0x00 }, { 0x55, 0xc8, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E70E8[] = {
    { 0x40, 0x41, 0x00, 0x00 },
    { 0x40, 0x53, 0x00, 0x00 },
    { 0x40, 0x65, 0x00, 0x00 },
    { 0x40, 0x77, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7108[][4] = {
    {
        { 0x18, 0x3f, 0x00, 0x00 },
        { 0x5d, 0x3f, 0x00, 0x00 },
        { 0xa2, 0x3f, 0x00, 0x00 },
        { 0xe7, 0x3f, 0x00, 0x00 },
    },
    {
        { 0x18, 0x91, 0x00, 0x00 },
        { 0x5d, 0x91, 0x00, 0x00 },
        { 0xa2, 0x91, 0x00, 0x00 },
        { 0xe7, 0x91, 0x00, 0x00 },
    },
};

Unk_D_800E70A0 D_800E7148[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0x5d, 0x3b, 0x00, 0x00 },
    { 0xa2, 0x3b, 0x00, 0x00 },
    { 0xe8, 0x3b, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7168[] = {
    { 0x17, 0x70, 0x00, 0x00 },
    { 0x57, 0x70, 0x00, 0x00 },
    { 0x17, 0x97, 0x00, 0x00 },
    { 0x57, 0x97, 0x00, 0x00 },
};

// In a perfect world this would be `Unk_D_800E70A0 D_800E7188[][4]`
Unk_D_800E70A0 D_800E7188[] = {
    { 0x80, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x80, 0x3f, 0x00, 0x00 }, { 0x80, 0x91, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x58, 0x00, 0x00 }, { 0xa6, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x3f, 0x00, 0x00 }, { 0xa6, 0x3f, 0x00, 0x00 }, { 0x5a, 0x91, 0x00, 0x00 }, { 0xa6, 0x91, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7208[][2] = {
    {
        { 0x9d, 0x70, 0x00, 0x00 },
        { 0x128, 0x81, 0x00, 0x00 },
    },
    {
        { 0x9d, 0x88, 0x00, 0x00 },
        { 0x128, 0x99, 0x00, 0x00 },
    },
    {
        { 0x9d, 0xa0, 0x00, 0x00 },
        { 0x128, 0xb1, 0x00, 0x00 },
    },
    {
        { 0x9d, 0xb8, 0x00, 0x00 },
        { 0x128, 0xc9, 0x00, 0x00 },
    },
};

Unk_D_800E70A0 D_800E7248[] = {
    { 0xff6a, 0x3b, 0x00, 0x00 },
    { 0x172, 0x3b, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7258[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0xc5, 0x3b, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7268[] = {
    { 0x28, 0x73, 0x00, 0x00 },
    { 0x28, 0x3c, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7278[] = {
    { 0x3e, 0x43, 0x00, 0x00 },     { 0xa1, 0x43, 0x00, 0x00 },
    { 0x3e, 0xc5, 0x00, 0x00 },     { 0xa1, 0xc5, 0x00, 0x00 },

    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E72F8 = { 0x140, 0x23, 0x00, 0x00 };

// In a perfect world this would be `Unk_D_800E70A0 D_800E7300[][4]`
Unk_D_800E70A0 D_800E7300[] = {
    { 0x50, 0x23, 0x00, 0x00 }, { 0xb0, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x32, 0x23, 0x00, 0x00 }, { 0x80, 0x23, 0x00, 0x00 }, { 0xce, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x18, 0x23, 0x00, 0x00 }, { 0x5d, 0x23, 0x00, 0x00 }, { 0xa2, 0x23, 0x00, 0x00 }, { 0xe7, 0x23, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7360[] = {
    { 0x61, 0xa7, 0x00, 0x00 },
    { 0x61, 0xb6, 0x00, 0x00 },
    { 0x61, 0xc5, 0x00, 0x00 },
    { 0x61, 0xd4, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7380[] = {
    { 0x30, 0x4b, 0x00, 0x00 },
    { 0x109, 0x4b, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7390[] = {
    { 0xad, 0x8d, 0x00, 0x00 }, { 0xad, 0x9a, 0x00, 0x00 }, { 0xad, 0xa7, 0x00, 0x00 },
    { 0xad, 0xb4, 0x00, 0x00 }, { 0xad, 0xc1, 0x00, 0x00 }, { 0xad, 0xce, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E73C0[] = {
    { 0xac, 0xa5, 0x00, 0x00 },
    { 0xac, 0xc3, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E73D0[] = {
    { 0xc0, 0xb3, 0x00, 0x00 },
    { 0xc0, 0xc2, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E73E0[] = {
    { 0x61, 0x94, 0x00, 0x00 }, { 0x61, 0xa1, 0x00, 0x00 }, { 0x61, 0xae, 0x00, 0x00 },
    { 0x61, 0xbb, 0x00, 0x00 }, { 0x61, 0xc8, 0x00, 0x00 }, { 0x61, 0xd5, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7410[] = {
    { 0x52, 0x90, 0x00, 0x00 },
    { 0x52, 0xa4, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7420[] = {
    { 0x76, 0x95, 0x00, 0x00 },
    { 0x76, 0xa4, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7430[] = {
    { 0x17, 0xa, 0x00, 0x00 }, { 0x5d, 0xa, 0x00, 0x00 }, { 0xa2, 0xa, 0x00, 0x00 },
    { 0xe8, 0xa, 0x00, 0x00 }, { 0x17, 0xa, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7458[] = {
    { 0x14a, 0x32, 0x00, 0x00 },  { 0xff60, 0xd4, 0x00, 0x00 }, { 0xa0, 0x10e, 0x00, 0x00 },
    { 0xff60, 0xbe, 0x00, 0x00 }, { 0x143, 0x5a, 0x00, 0x00 },
};

Unk_D_800E70A0 D_800E7480[] = {
    { 0xa0, 0x32, 0x00, 0x00 }, { 0x9b, 0xd4, 0x00, 0x00 }, { 0xa0, 0x50, 0x00, 0x00 },
    { 0x9b, 0xbe, 0x00, 0x00 }, { 0x80, 0x5a, 0x00, 0x00 },
};

RGBA16 D_800E74A8[] = {
    { 0x00, 0xf3, 0xf3, 0xff }, { 0xff, 0xa8, 0xc3, 0xff }, { 0xff, 0xfe, 0x7a, 0xff },
    { 0x7b, 0xfc, 0x7b, 0xff }, { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 D_800E74D0[] = {
    { 0x00, 0xf3, 0xf3, 0xff },
    { 0xff, 0xa8, 0xc3, 0xff },
    { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 D_800E74E8[] = {
    { 0xff, 0xaf, 0xaf, 0xff },
    { 0xaf, 0xff, 0xaf, 0xff },
    { 0xaf, 0xaf, 0xff, 0xff },
};

const s16 gGlyphDisplayWidth[] = {
    0x000c, 0x000d, 0x000b, 0x000b, 0x000a, 0x000b, 0x000b, 0x000d, 0x0007, 0x000a, 0x000c, 0x000a, 0x0012, 0x000d,
    0x000c, 0x000c, 0x000c, 0x000c, 0x000b, 0x000d, 0x000c, 0x000c, 0x0012, 0x000d, 0x000c, 0x000c, 0x000a, 0x000a,
    0x000a, 0x0006, 0x001e, 0x0006, 0x000a, 0x0008, 0x000b, 0x000c, 0x000c, 0x000d, 0x000a, 0x000b, 0x000a, 0x000a,
    0x0008, 0x001c, 0x000a, 0x0010, 0x000f, 0x0010, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000e,
    0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e,
    0x000e, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000e, 0x000f, 0x000e, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000b, 0x000f,
    0x000f, 0x000f, 0x000f, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x000f,
    0x000f, 0x0017, 0x000f, 0x0017, 0x0017, 0x0017, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
};

char* gCupNames[] = {
    "mushroom cup",
    "flower cup",
    "star cup",
    "special cup",
    "battle",
    // ????
    "mushroom cup",
    "flower cup",
    "star cup",
    "special cup",
};

// Displays at beginning of course
char* gCourseNames[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

char* gCourseNamesDup[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

char* gCourseNamesDup2[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

// Used in debug menu at splash screen
char* gDebugCourseNames[] = {
#include "assets/course_metadata/gCourseDebugNames.inc.c"
};

const s8 gPerCupIndexByCourseId[] = {
#include "assets/course_metadata/gPerCupIndexByCourseId.inc.c"
};

// @todo Increase this array for more than eight players
const s8 D_800EFD64[] = { 0, 1, 4, 3, 5, 6, 2, 7 };

// Maps course IDs (as defined in the COURSES enum) to the cup they belong to
s8 gCupSelectionByCourseId[] = {
#include "assets/course_metadata/gCupSelectionByCourseId.inc.c"
};

char* D_800E7678[] = {
    "none",
    "bronze",
    "silver",
    "gold",
};

char* gDebugCharacterNames[] = {
    "MARIO", "LUIGI", "YOSHI", "KINOPIO", "D.KONG", "WARIO", "PEACH", "KOOPA",
};

char* D_800E76A8[] = {
    "MARIO",    "LUIGI", "YOSHI", "TOAD", "D.K.", "WARIO", "PEACH", "BOWSER",
    "ーーーー", // NOT HYPHENS!!! These are EUC-JP characters (0xa1 0xbc)
};

char* D_800E76CC[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* D_800E76DC[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* gDebugScreenModeNames[] = {
    "1p", "2players UD", "2players LR", "3players", "4players",
};

char* gDebugSoundModeNames[] = {
    "stereo",
    "head phone",
    "xxx",
    "monaural",
};

char* gSoundModeNames[NUM_SOUND_MODES] = { "STEREO", "HEADPHONE", "", "MONO" };

char* D_800E7720[] = {
    "WINNER!",
    "LOSER!",
};

char* D_800E7728[] = {
    "BEST RECORDS",
    "BEST LAP",
};

// Might need a const?
char* D_800E7730 = "LAP TIME";

char* D_800E7734[] = {
    "LAP 1",
    "LAP 2",
    "LAP 3",
    "TOTAL",
};

char* D_800E7744[] = {
    // The s/n/r/t here are not ASCII, they are EUC-JP characters
    // 0xae 0xf3/0xae 0xee/0xae 0xf2/0xae 0xf4
    "1 ｓ", "2 ｎ", "3 ｒ", "4 ｔ", "5 ｔ", " ",
};

char* gTextPauseButton[] = {
    "CONTINUE GAME", "RETRY", "COURSE CHANGE", "DRIVER CHANGE", "QUIT", "REPLAY", "SAVE GHOST",
};

char* D_800E7778[] = {
    "VS MATCH RANKING",
    "BATTLE RANKING",
};

// This is plain data, it should not end up in rodata
char D_800E7780[] = "NOW-MEET THE COURSE GHOST!!!";

char* D_800E77A0[] = { "CONNECT A CONTROLLER TO SOCKET 1,", "THEN POWER ON AGAIN" };

char* D_800E77A8[] = {
    "BATTLE GAME",
    "POP OPPOSING PLAYER'S BALLOONS",
    "WHEN ALL 3 ARE GONE,THEY ARE OUT!",
};

// This is plain data, it should not end up in rodata
char D_800E77B4[] = "a BUTTON*SEE DATA  B BUTTON*EXIT";

// This is plain data, it should not end up in rodata
char D_800E77D8[] = "distance";

char* sCourseLengths[] = {
#include "assets/course_metadata/sCourseLengths.inc.c"
};

char* D_800E7834[] = {
    "return to menu",
    "erase records for this course",
    "erase ghost from this course",
};

char* D_800E7840[] = {
    "quit",
    "erase",
};

// Why oh why is this array flat? It should be D_800E7848[][3]
char* D_800E7848[] = {
    "THE BEST RECORDS AND BEST", "LAP FOR THIS COURSE WILL BE", "ERASED.  IS THIS OK?",

    "GHOST DATA FOR THIS",       "COURSE WILL BE ERASED.",      "IS THIS OK?",
};

char* D_800E7860[] = {
    "UNABLE TO ERASE ",
    "GHOST DATA",
};

char* D_800E7868[] = {
    "RETURN TO GAME SELECT",
    "SOUND MODE",
    "COPY N64 CONTROLLER PAK",
    "ERASE ALL DATA",
};

char* D_800E7878[] = {
    "ALL SAVED DATA WILL BE",
    "PERMANENTLY ERASED.",
    "ARE YOU REALLY SURE?",
};

char* D_800E7884[] = {
    "",
    "ALL SAVED DATA",
    "HAS BEEN NOW ERASED.",
};

// In a perfect world this would be `char *D_800E7890[][4]`
char* D_800E7890[] = {
    "CONTROLLER 1 DOES NOT HAVE ",
    "N64 CONTROLLER PAK",
    "",
    "",

    "UNABLE TO READ ",
    "N64 CONTROLLER PAK DATA ",
    "FROM CONTROLLER 1",
    "",

    "UNABLE TO CREATE GAME DATA ",
    "FROM CONTROLLER 1 ",
    "N64 CONTROLLER PAK",
    "",

    "UNABLE TO COPY GHOST ",
    "-- INSUFFICIENT FREE PAGES ",
    "IN CONTROLLER 1 ",
    "N64 CONTROLLER PAK",
};

// In a perfect world this would be `char *D_800E78D0[][3]`
char* D_800E78D0[] = {
    "NO GHOST DATA ",         "IN CONTROLLER 2 ",         "N64 CONTROLLER PAK",

    "NO MARIO KART 64 DATA ", "PRESENT IN CONTROLLER 2 ", "N64 CONTROLLER PAK",

    "CONTROLLER 2 ",          "DOES NOT HAVE ",           "N64 CONTROLLER PAK SET",

    "UNABLE TO READ DATA ",   "FROM CONTROLLER 2 ",       "N64 CONTROLLER PAK",
};

// In a perfect world this would be `char *D_800E7900[][4]`
char* D_800E7900[] = {
    "UNABLE TO COPY DATA ", "FROM CONTROLLER 1 ", "N64 CONTROLLER PAK",

    "UNABLE TO READ DATA ", "FROM CONTROLLER 2 ", "N64 CONTROLLER PAK",
};

char* D_800E7918[] = {
    "CONTROLLER 1",
    "CONTROLLER 2",
};

char* D_800E7920[] = {
    "WHICH FILE DO YOU WANT TO MAKE A COPY OF?",
    "TO WHICH FILE DO YOU WANT TO COPY?",
};

char* D_800E7928[] = {
    "CURRENT DATA WILL BE ERASED,",
    "IS THIS OK?",
};

char* D_800E7930[] = {
    "QUIT",
    "COPY",
};

char* D_800E7938[] = {
    "COPYING",
    "DATA COPY COMPLETED",
};

// In a perfect world this would be `char *D_800E7940[][4]`
char* D_800E7940[] = {
    "NO N64 CONTROLLER PAK DETECTED",
    "TO SAVE GHOST DATA, ",
    "INSERT N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",

    "UNABLE TO READ ",
    "N64 CONTROLLER PAK DATA",
    "",
    "",

    "",
    "",
    "",
    "",

    "INSUFFICIENT FREE PAGES AVAILABLE ",
    "IN N64 CONTROLLER PAK TO CREATE ",
    "GAME DATA, PLEASE FREE 121 PAGES.",
    "SEE INSTRUCTION BOOKLET FOR DETAILS.",
};

// Unused?
char* D_800E7980[] = {
    "TO SAVE GHOST DATA, ",
    "INSERT N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",
};

// In a perfect world this would be `char *D_800E798C[][7]`
char* D_800E798C[] = {
    "N64 CONTROLLER PAK ",
    "NOT DETECTED. ",
    "IF YOU WANT TO SAVE ",
    "THE GHOST DATA, ",
    "PLEASE INSERT ",
    "N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",

    "",
    "UNABLE TO SAVE ",
    "     THE GHOST",
    "",
    "",
    "",
    "",

    "",
    "UNABLE TO SAVE ",
    "     THE GHOST",
    "",
    "",
    "",
    "",

    "INSUFFICIENT ",
    "FREE PAGES AVAILABLE ",
    "",
    "-- GHOST DATA ",
    "COULD NOT BE SAVED",
    "",
    "",

    "",
    "CANNOT CREATE ",
    "     GAME DATA",
    "",
    "",
    "",
    "",

    "",
    "THIS GHOST IS ",
    "     ALREADY SAVED",
    "",
    "",
    "",
    "",
};

char* D_800E7A34[] = {
    "RACE DATA CANNOT ",
    "BE SAVED FOR GHOST",
};

char* D_800E7A3C[] = {
    "SELECT THE FILE ",
    "YOU WANT TO SAVE",
};

// Might need a const?
char* D_800E7A44 = "NO DATA";

char* D_800E7A48[] = {
    "CREATING ",
    "MARIO KART 64 ",
    "GAME DATA",
};

char* D_800E7A54[] = {
    "CANNOT CREATE GAME DATA",
    "",
    "",
};

char* D_800E7A60[] = {
    "THE PREVIOUS DATA ",
    "WILL BE ERASED, ",
    "IS THIS OK?",
};

char* D_800E7A6C[] = {
    "QUIT",
    "SAVE",
};

char* D_800E7A74[] = {
    "SAVING GHOST DATA",
    "",
    "PLEASE WAIT",
};

char* D_800E7A80[] = {
    "UNABLE TO SAVE ",
    "THE GHOST",
};

char* D_800E7A88[] = {
    "YOU ARE AWARDED THE",
    "GOLD CUP",
    "SILVER CUP",
    "BRONZE CUP",
};

// Might need a const?
char* D_800E7A98 = "MAYBE NEXT TIME!";

char* D_800E7A9C[] = {
    "CONGRATULATIONS!",
    "WHAT A PITY!",
};

char* D_800E7AA4[] = {
    "YOU PLACED", "    st", "    nd", "    rd", "    th", "    th", "    th", "    th", "    th",
};

const s8 gGPPointRewards[] = { 9, 6, 3, 1 };
const s8 D_800F0B1C[] = {
    0, 0, 1, 0, 1, 0, 1, 2, 0, 1, 2, 3,
};
const s8 D_800F0B28[] = {
    0, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 1, 2, 2, 1, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

const s8 D_800F0B50[] = { 0x1f, 0x0b, 0x15, 0x29 };
const s8 D_800F0B54[] = { 0x20, 0x0f, 0x18, 0x2c };

RGBA16 D_800E7AC8[] = {
    { 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff },
    { 0x00, 0x00, 0x50, 0xff },
    { 0xff, 0xff, 0xff, 0xff },
};

RGBA16 D_800E7AE8[] = {
    { 0x00, 0x00, 0x00, 0xff },
    { 0xff, 0xff, 0xff, 0xff },
};

MkTexture* D_800E7AF8[] = {
    D_02000000, D_02000028, D_02000050, D_02000078, D_020000A0, D_020000C8, D_020000F0, D_02000118, D_02000140,
    D_02000168, D_02000190, D_020001B8, D_020001E0, D_02000208, D_02000230, D_02000258, D_02000280, D_020002A8,
    D_020002D0, D_020002F8, D_02000320, D_02000348, D_02000370, D_02000398, D_020003C0, D_020003E8, D_02000410,
    D_02000438, D_02000460, D_02000488, D_020004B0, D_020004D8, D_02000500, D_02000528, D_02000550, D_02000578,
    D_020005A0, D_020005C8, D_020005F0, D_02000618, D_02000640, D_02000668, D_02000690, D_020006B8, D_020006E0,
    D_02000708, D_02000730, D_02000758, D_02000780, D_020007A8, D_020007D0, D_020007F8, D_02000820, D_02000848,
    D_02000870, D_02000898, D_020008C0, D_020008E8, D_02000910, D_02000938, D_02000960, D_02000988, D_020009B0,
    D_020009D8, D_02000A00, D_02000A28, D_02000A50, D_02000A78, D_02000AA0, D_02000AC8, D_02000AF0, D_02000B18,
    D_02000B40, D_02000B68, D_02000B90, D_02000BB8, D_02000BE0, D_02000C08, D_02000C30, D_02000C58, D_02000C80,
    D_02000CA8, D_02000CD0, D_02000CF8, D_02000D20, D_02000D48, D_02000D70, D_02000D98, D_02000DC0, D_02000DE8,
    D_02000E10, D_02000E38, D_02000E60, D_02000E88, D_02000EB0, D_02000ED8, D_02000F00, D_02000F28, D_02000F50,
    D_02000F78, D_02000FA0, D_02000FC8, D_02000FF0, D_02001018, D_02001040, D_02001068, D_02001090, D_020010B8,
    D_020010E0, D_02001108, D_02001130, D_02001158, D_02001180, D_020011A8, D_020011D0, D_020011F8, D_02001220,
    D_02001248, D_02001270, D_02001298, D_020012C0, D_020012E8, D_02001310, D_02001338, D_02001360, D_02001388,
    D_020013B0, D_020013D8, D_02001400, D_02001428, D_02001450, D_02001478, D_020014A0,
};

MkTexture* D_800E7D0C[] = {
    D_020016BC, D_020016E4, D_0200170C, D_02001734, D_0200175C,
    D_02001784, D_020017AC, D_020017D4, D_020017FC, D_02001824,
};

MkAnimation* D_800E7D34[] = {
    D_0200198C, D_0200199C, D_020019AC, D_020019BC, D_020019CC, D_020019DC,
};

MkTexture* D_800E7D4C[] = {
    D_02004598,
    D_020045C0,
};

MkTexture* D_800E7D54[] = {
    D_02001A8C, D_02001A64, D_02001AB4, D_02001A14, D_02001B04, D_020019EC, D_02001ADC, D_02001A3C,
};

MkTexture* D_800E7D74[] = {
    D_02001B2C, D_02001B54, D_02001B7C, D_02001BA4, D_02001BCC, D_02001BF4, D_02001C1C,
    D_02001C44, D_02001C6C, D_02001C94, D_02001CBC, D_02001CE4, D_02001D0C, D_02001D34,
    D_02001D5C, D_02001D84, D_02001DAC, D_02001DD4, D_02001DFC, D_02001E24,
};

MkTexture* D_800E7DC4[] = {
    D_02004EA8, D_02004ED0, D_02004EF8, D_02004F20, D_02004F48, D_02004F70, D_02004F98,
    D_02004FC0, D_02004FE8, D_02005010, D_02005038, D_02005060, D_02005088, D_020050B0,
    D_020050D8, D_02005100, D_02005128, D_02005150, D_02005178, D_020051A0,
};

// Unused?
MkAnimation* D_800E7E14[] = {
    D_020020BC,
    D_020020CC,
    D_020020DC,
};

MkAnimation* D_800E7E20[] = {
    D_020020DC, D_020020EC, D_020020FC, D_0200210C, D_0200210C,
};

MkAnimation* D_800E7E34[] = {
    D_02001E64, D_02001E74, D_02001E84, D_02001E94, D_02001EA4, D_02001EB4, D_02001EC4,
    D_02001ED4, D_02001EE4, D_02001EF4, D_02001F04, D_02001F14, D_02001F24, D_02001F34,
    D_02001F44, D_02001F54, D_02001F64, D_02001F74, D_02001F84, D_02001F94,
};

MkTexture* gGlyphTextureLUT[] = {
    D_0200211C, D_02002144, D_0200216C, D_02002194, D_020021BC, D_020021E4, D_0200220C, D_02002234, D_0200225C,
    D_02002284, D_020022AC, D_020022D4, D_020022FC, D_02002324, D_0200234C, D_02002374, D_0200239C, D_020023C4,
    D_020023EC, D_02002414, D_0200243C, D_02002464, D_0200248C, D_020024B4, D_020024DC, D_02002504, D_0200252C,
    D_02002554, D_0200257C, D_020025A4, D_02004584, D_0200261C, D_02002694, D_020026BC, D_020026E4, D_0200270C,
    D_02002734, D_0200275C, D_02002784, D_020027AC, D_020027D4, D_020027FC, D_020025CC, D_02002644, D_020025F4,
    D_0200266C, D_02004534, D_02004584, D_02002824, D_0200284C, D_02002874, D_0200289C, D_020028C4, D_020028EC,
    D_02002F54, D_02002914, D_02002F7C, D_0200293C, D_02002FA4, D_02002964, D_02002FCC, D_0200298C, D_02002FF4,
    D_020029B4, D_0200301C, D_020029DC, D_02003044, D_02002A04, D_0200306C, D_02002A2C, D_02003094, D_02002A54,
    D_020030BC, D_02002A7C, D_020030E4, D_02002AA4, D_0200310C, D_020033B4, D_02002ACC, D_02003134, D_02002AF4,
    D_0200315C, D_02002B1C, D_02003184, D_02002B44, D_02002B6C, D_02002B94, D_02002BBC, D_02002BE4, D_02002C0C,
    D_020031AC,
};

MkTexture* D_800E7FF0[] = {
    D_02003274, D_02002C34, D_020031D4, D_0200329C, D_02002C5C, D_020031FC, D_020032C4, D_02002C84, D_02003224,
    D_020032EC, D_02002CAC, D_0200324C, D_02003314, D_02002CD4, D_02002CFC, D_02002D24, D_02002D4C, D_02002D74,
    D_0200333C, D_02002D9C, D_02003364, D_02002DC4, D_0200338C, D_02002DEC, D_02002E14, D_02002E3C, D_02002E64,
    D_02002E8C, D_02002EB4, D_02002EDC, D_02002F04, D_02002F2C, D_020033DC, D_02003404, D_0200342C, D_02003454,
    D_0200347C, D_020034A4, D_020034CC, D_020034F4, D_0200351C, D_02003544, D_0200356C, D_02003BD4,
};

MkTexture* D_800E80A0[] = {
    D_02003594, D_02003BFC, D_020035BC, D_02003C24, D_020035E4, D_02003C4C, D_0200360C, D_02003C74,
    D_02003634, D_02003C9C, D_0200365C, D_02003CC4, D_02003684, D_02003CEC, D_020036AC, D_02003D14,
    D_020036D4, D_02003D3C, D_020036FC, D_02003D64, D_02003724, D_02003D8C, D_02004034, D_0200374C,
    D_02003DB4, D_02003774, D_02003DDC, D_0200379C, D_02003E04,
};

MkTexture* D_800E8114[] = {
    D_020037C4, D_020037EC, D_02003814, D_0200383C, D_02003864, D_0200388C, D_02003E2C, D_02003EF4,
    D_020038B4, D_02003E54, D_02003F1C, D_020038DC, D_02003E7C, D_02003F44, D_02003904, D_02003EA4,
    D_02003F6C, D_0200392C, D_02003ECC, D_02003F94, D_02003954, D_0200397C, D_020039A4, D_020039CC,
};

MkTexture* D_800E8174[] = {
    D_020039F4,
    D_02003FBC,
};

MkTexture* D_800E817C[] = {
    D_02003A1C, D_02003FE4, D_02003A44, D_0200400C, D_02003A6C, D_02003A94, D_02003ABC, D_02003AE4, D_02003B0C,
    D_02003B34, D_02003B5C, D_02003B84, D_02003BAC, D_0200405C, D_02004084, D_020040AC, D_020040D4, D_020040FC,
    D_020043CC, D_02004444, D_0200437C, D_020043F4, D_02004124, D_0200414C, D_02004174, D_0200419C,
};

MkTexture* D_800E81E4[] = {
    D_020041C4, D_020041EC, D_02004214, D_0200423C, D_02004264, D_0200428C, D_020042B4, D_020042DC, D_02004354,
    D_020043A4, D_0200441C, D_0200446C, D_02004494, D_020044BC, D_02004304, D_0200432C, D_020044E4, D_0200450C,
};

MkTexture* D_800E822C[] = {
    D_02004534,
    D_0200455C,
};

// In a perfect world this would be `MkTexture *D_800E8234[][2]`
MkTexture* D_800E8234[] = {
    D_02004688, D_020047DC, D_020046D8, D_02004804, D_0200473C, D_0200482C,
};

MkTexture* D_800E824C[] = {
    D_0200478C,
    D_02004854,
};

MkTexture* D_800E8254[] = {
    D_02004660, D_02004688, D_020046D8, D_0200473C, D_0200478C, D_0200487C, D_020048A4, D_020048CC,
};

MkTexture* D_800E8274[] = {
    D_020048F4, D_0200491C, D_02004944, D_0200496C, D_020049BC, D_02004994, D_020049E4, D_02004A34,
};

MkTexture* D_800E8294[] = {
    D_020048F4, D_0200491C, D_02004944, D_0200496C, D_020049BC, D_02004994, D_020049E4, D_02004A34,
};

MkTexture* D_800E82B4[] = {
    D_02004A5C,
    D_02004A98,
    D_02004AD4,
    D_02004B10,
};

MkTexture* D_800E82C4[] = {
    D_02004B9C, D_02004BC4, D_02004BEC, D_02004C14, D_02004C3C, D_02004C64,
    D_02004C8C, D_02004CF0, D_02004D54, D_02004DB8, D_02004E1C, D_02004E80,
};

MkTexture* D_800E82F4[] = {
    D_02004BC4, D_02004BEC, D_02004C14, D_02004C3C, D_02004C64, D_02004C8C,
    D_02004CF0, D_02004D54, D_02004DB8, D_02004E1C, D_02004E80,
};

MkAnimation* D_800E8320[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* D_800E8340[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* D_800E8360[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* gCharacterCelebrateAnimation[] = {
    D_02006718, D_02006900, D_02006CD0, D_02006EB8, D_020070A0, D_02007290, D_02007478, D_02006AE8,
};

MkAnimation* D_800E83A0[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* gCharacterDeselectAnimation[] = {
    D_02006788, D_02006970, D_02006D40, D_02006F28, D_02007110, D_02007300, D_020074E8, D_02006B58,
};

MkAnimation* gCharacterSingleBlinkAnimation[] = {
    D_020067E0, D_020069C8, D_02006D98, D_02006F80, D_02007170, D_02007358, D_02007540, D_02006BB0,
};

MkAnimation* gCharacterDoubleBlinkAnimation[] = {
    D_02006838, D_02006A20, D_02006DF0, D_02006FD8, D_020071C8, D_020073B0, D_02007598, D_02006C08,
};

MkAnimation* gCharacterDefeatAnimation[] = {
    D_020068E0, D_02006AC8, D_02006E98, D_02007080, D_02007270, D_02007458, D_02007640, D_02006CB0,
};

s32 D_800E8440[] = {
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
};

s32 D_800E8460[] = {
    0x09, 0x09, 0x09, 0x09, 0x0a, 0x09, 0x09, 0x09,
};

s32 D_800E8480[] = {
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
};

s32 D_800E84A0[] = {
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
};

Gfx* D_800E84CC[] = {
    D_02007838, D_02007858, D_02007878, D_02007898, D_020078B8, D_020078D8, D_020078F8, D_02007918,
};

Gfx* D_800E84EC[] = {
    D_02007938, D_02007958, D_02007978, D_02007998, D_020079B8, D_020079D8, D_020079F8, D_02007A18,
};

Gfx* D_800E850C[] = {
    D_02007A38, D_02007A58, D_02007A78, D_02007A98, D_02007AB8, D_02007AD8, D_02007AF8, D_02007B18,
};

s8 D_800E852C = 1;

f32 D_800E8530 = 0.0f;

f32 D_800E8534 = 3.0f;

Unk_D_800E70A0 D_800E8538[] = { { 0x69, 0x9b, 0x00, 0x00 } };

// In a perfect world this would be `Unk_D_800E70A0 D_800E8540[][4]`
Unk_D_800E70A0 D_800E8540[] = {
    { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 },

    { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 }, { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 },

    { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 }, { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 },

    { 0x28, 0x28, 0x00, 0x00 }, { 0xb2, 0x28, 0x00, 0x00 }, { 0x28, 0xa0, 0x00, 0x00 }, { 0xb2, 0xa0, 0x00, 0x00 },
};

// In a perfect world this would be `Unk_D_800E70A0 D_800E85C0[][4]`
Unk_D_800E70A0 D_800E85C0[] = {
    { 0x69, 0x8c, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 },

    { 0x69, 0x55, 0x00, 0x00 }, { 0x69, 0xcd, 0x00, 0x00 }, { 0x78, 0x28, 0x00, 0x00 }, { 0x78, 0xa0, 0x00, 0x00 },
};

// In a perfect world this would be `Unk_D_800E70A0 D_800E8600[][4]`
Unk_D_800E70A0 D_800E8600[] = {
    { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 },

    { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 }, { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 },

    { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 }, { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 },

    { 0x28, 0x28, 0x00, 0x00 }, { 0xb2, 0x28, 0x00, 0x00 }, { 0x28, 0xa0, 0x00, 0x00 }, { 0xb2, 0xa0, 0x00, 0x00 },
};

f64 exponent_by_squaring(f64 base, s32 exponent) {
    s32 positive_exponent;
    f64 result;

    if (exponent < 0) {
        positive_exponent = -exponent;
    } else {
        positive_exponent = exponent;
    }

    result = 1.0;
    if (positive_exponent != 0) {
        do {
            if ((positive_exponent & 1) != 0) {
                result *= base;
            }
            positive_exponent >>= 1;
            base *= base;
        } while (positive_exponent != 0);
    }
    if (exponent >= 0) {
        return result;
    }
    return 1.0 / result;
}

f64 func_800917B0(f64 arg0, f64 arg1) {
    if ((arg1 <= 2147483647.0) && (-2147483647.0 <= arg1)) {
        if (arg1 == (s32) arg1) {
            return exponent_by_squaring(arg0, arg1);
        }
    }
    if (arg0 > 0.0) {
        return func_8009195C(func_8009186C(arg0) * arg1);
    }
    return 0.0;
}

f64 func_8009186C(f64 arg0) {
    s32 var_v0;
    s32 sp38;
    f64 var_f2;
    f64 temp_f0;
    f64 temp_f12;

    if (arg0 <= 0.0) {
        return 0.0;
    }
    func_80091AC0(arg0 / 1.414213562373095, &sp38);
    arg0 /= func_80091A6C(1.0, sp38);
    var_v0 = 1;
    arg0 = (arg0 - 1.0) / (arg0 + 1.0);
    temp_f12 = arg0 * arg0;
    var_f2 = arg0;
    do {
        var_v0 += 2;
        arg0 *= temp_f12;
        temp_f0 = var_f2;
        var_f2 += arg0 / (f64) var_v0;
    } while (temp_f0 != var_f2);
    return ((f64) sp38 * 0.6931471805599453) + (2 * var_f2);
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/dXpT5
// Some math reordering issues on the continued fraction line
f64 func_8009195C(f64 arg0) {
    s32 temp_f10;
    s32 six = 6;
    s32 ten = 10;
    s32 fourteen = 14;
    s32 eighteen = 18;
    f64 temp_f2;

    if (arg0 >= 0.0) {
        temp_f2 = 0.5;
    } else {
        temp_f2 = -0.5;
    }
    temp_f10 = temp_f2 + (arg0 / 0.6931471805599453);
    arg0 -= (temp_f10 * 0.6931471805599453);
    temp_f2 = arg0 * arg0;
    /**
     * This is the denominator part a tanh(x/2) continued fraction, where arg0 is x
     * The best reference to this I can find is:
     *https://math.stackexchange.com/questions/3241906/continued-fraction-02-6-10-14-22n-1-frace-1e1
     **/
    temp_f2 =
        2 + (temp_f2 / (six + (temp_f2 / (ten + (temp_f2 / (fourteen + (temp_f2 / (eighteen + (temp_f2 / 22)))))))));
    return func_80091A6C((temp_f2 + arg0) / (temp_f2 - arg0), temp_f10);
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_8009195C.s")
#endif

/**
 * This function appears to multiply some `value`
 * by 2 ^ `exponent`, even if that exponent is negative
 **/
f64 func_80091A6C(f64 value, s32 exponent) {
    f64 base;

    if (exponent >= 0) {
        base = 2.0;
    } else {
        exponent = -exponent;
        base = 0.5;
    }

    if (exponent != 0) {
        do {
            if ((exponent & 1) != 0) {
                value *= base;
            }
            exponent >>= 1;
            base *= base;
        } while (exponent != 0);
    }
    return value;
}

/**
 * Its unclear what exact purpose this function serves
 * The function multiplies/divides arg0 by 2 until its in
 * the range (0.5, 1.0].
 * arg2 appears to track the exponent in the power-of-2
 * that would undo the changes to arg0
 **/
f64 func_80091AC0(f64 arg0, s32* arg2) {
    const f64 const1 = 2.0;
    s32 exponent = 0;

    while ((arg0 < 0.5) || (arg0 >= 1.0)) {
        if (arg0 < 0.5) {
            arg0 *= const1;
            exponent -= 1;
        } else {
            arg0 /= const1;
            exponent += 1;
        }
    }
    *arg2 = exponent;
    return arg0;
}

void swap_values(s32* arg0, s32* arg1) {
    s32 temp;

    temp = *arg0;
    *arg0 = *arg1;
    *arg1 = temp;
}

extern s8 D_800E852C;

void func_80091B78(void) {
    s32 why = 0;
    s32 i;

    if (D_800E852C) {
        D_800E852C = why;
        D_8018EDF4 = 10;
        D_8018EDF5 = 5;
        D_8018EDF6 = 10;
        if (osEepromProbe(&gSIEventMesgQueue) != 0) {
            // save data disabled for now due to array overflow
            // load_save_data();
        }
        if (func_80091D74() != 0) {
            gMenuSelection = CONTROLLER_PAK_MENU;
        }
    }
    if (gMenuSelection == LOGO_INTRO_MENU) {
        gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
#ifdef TARGET_N64
        set_segment_base_addr(6, decompress_segments((u8*) STARTUP_LOGO_ROM_START, (u8*) STARTUP_LOGO_ROM_END));
#endif
    }
    gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
    // Hypothetically, this should be a ptr... But only hypothetically.
    D_8018D9B0 = get_next_available_memory_addr(0x000900B0);
    D_8018D9B4 = (u8*) get_next_available_memory_addr(0x0000CE00);
    D_8018D9B8 = (u8*) get_next_available_memory_addr(0x00012C00);
    D_8018D9C0 = (struct_8018EE10_entry*) get_next_available_memory_addr(0x00001000);
    func_800AF9B0();
    D_8018EE0C = 0;

    for (i = 0; i < 5; i++) {
        D_8018E7AC[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        D_8018E838[i] = 0;
    }

    D_800DC5EC->screenStartX = 160;
    D_800DC5EC->screenStartY = 120;
    D_800DC5EC->screenWidth = SCREEN_WIDTH;
    D_800DC5EC->screenHeight = SCREEN_HEIGHT;
    D_800E86A4 = 1;
    func_80094C60();

    for (i = 0; i < 4; i++) {
        func_800C97C4((u8) i);
    }

    for (i = 1; i < 8; i++) {
        func_800C9D0C((u8) i);
    }

    func_800B44BC();
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
}

s32 func_80091D74(void) {
    u8 sp67;
    UNUSED s32 pad[10];
    s32 i;

    if (!gControllerBits) {
        return 0;
    }
    if ((!gControllerStatuses[0].status) & CONT_CARD_ON) {
        return 0;
    }
    if (!(gControllerOne->button & START_BUTTON)) {
        return 0;
    }
    osPfsIsPlug(&gSIEventMesgQueue, &sp67);
    if (sp67 & 1) {
        if (osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, 0)) {
            return 0;
        }
    } else {
        return 0;
    }

    if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed, &gControllerPak1MaxWriteableFiles)) {
        return 0;
    }

    for (i = 0; i < 16; i++) {
        pfsError[i] = osPfsFileState(&gControllerPak1FileHandle, i, pfsState + i);
    }

    if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree)) {
        return 0;
    }
    gControllerPak1NumPagesFree >>= 8;
    return 1;
}

void func_80091EE4(void) {
    s32 temp_s0;
    s32 temp_s2;
    s32 tmp;

    gControllerPak1State = BAD;
    tmp = func_800B5F30();

    if ((D_8018EDFB != 0) && (tmp == 0)) {
        temp_s2 = (gCupSelection * 4) + gCupCourseSelection;
        func_800B6708();

        for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
            if ((D_8018EE10[temp_s0].ghostDataSaved != 0) && (temp_s2 == D_8018EE10[temp_s0].courseIndex)) {
                func_800B64EC(temp_s0);
                temp_s0 = 2;
                D_8018EDFB = 0;
            }
        }
    }
}

void func_80091FA4(void) {
    s32 i;

    //! @todo These sizes need to be sizeof() for shiftability if possible
    D_8018D9B4 = (u8*) get_next_available_memory_addr(0x00002800);
    D_8018D9B0 = (u16*) get_next_available_memory_addr(0x000124F8);
    D_8018D9B8 = (u8*) get_next_available_memory_addr(0x00001000);
    D_8018D9BC = get_next_available_memory_addr(4);

    for (i = 0; i < 5; i++) {
        D_8018E7AC[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        D_8018E838[i] = 0;
    }

    func_80099110();
    func_8009A344();
    func_8009E620();
    func_80092258();
    add_8018D9E0_entry(0x00000096, 0x00000064, 0x00000024, 1);
    add_8018D9E0_entry(0x00000097, 0x00000064, 0x000000DD, 1);
    add_8018D9E0_entry(0x00000098, 0, 0, 0);
    add_8018D9E0_entry(0x000000C7, 0, 0, 0);
    if (gModeSelection == TIME_TRIALS) {
        add_8018D9E0_entry(0x000000BE, 0, 0, 0);
        add_8018D9E0_entry(0x0000010E, 0, 0, 0);
    }
    if ((D_8015F890 != 0) && (gModeSelection == TIME_TRIALS)) {
        add_8018D9E0_entry(0x000000BD, 0, 0, 0);
    }
    if (!(gControllerBits & 1) && (D_8018EE08 != 0)) {
        add_8018D9E0_entry(4, 0, 0, 2);
    }
    func_800B5F30();
}

void func_80092148(void) {
    struct Controller* controller = gControllerFive;
    s32 isHeld;

    isHeld = controller->button ? 1 : 0;

    if (controller->buttonPressed) {
        D_8018D9D9 = 1;
    }
    if ((isHeld) && (D_8018D9D9)) {
        D_8018D9D8 = 1;
        return;
    }
    D_8018D9D8 = 0;
}

void func_800921B4(void) {
    D_8018D9D9 = 0;
}

// if selected and detect kind menu
void text_rainbow_effect(s32 test, s32 target, s32 alternative_color) {
    if (test == target) {
        if (func_800B4520() != 0) {
            set_text_color(gGlobalTimer % 3);
        } else {
            set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        }
    } else {
        set_text_color(alternative_color);
    }
}

void set_text_color_rainbow_if_selected(s32 test, s32 target, s32 alternative_color) {
    if (test == target) {
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    } else {
        set_text_color(alternative_color);
    }
}

void func_80092258(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_8018E850[i] = i;
        D_8018E858[i] = 0;
    }
}

//! @bug vtx overflow from idx + 36 in this func
void func_80092290(s32 arg0, s32* arg1, s32* arg2) {
    s32 temp_v1;
    s32 i;
    s32 idx;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_t0;
    s32 a, b, c, d;
    Vtx* vtx;

    Vtx* v1 = (Vtx*) LOAD_ASSET(D_02007BB8);
    // Vtx *v2 = (Vtx *) LOAD_ASSET(D_02007CD8);
    // Vtx *v3 = (Vtx *) LOAD_ASSET(D_02007DF8);

    Vtx* D_800E84C0[] = {
        &v1[0],
        &v1[18],
        &v1[36],
    };

    if ((arg0 < 4) || (arg0 >= 6)) {
        return;
    }

    idx = (((arg0 * 4) + ((gGlobalTimer % 2) * 2)) - 6);

    *arg2 += 16;
    if (*arg2 >= 256) {
        *arg2 = 0;
        *arg1 = (s32) (*arg1 + 1) % 3;
    }

    for (i = 0; i < 3; i++) {
        if (i == 0) {
            vtx = (Vtx*) &v1[0];
        } else if (i == 1) {
            vtx = (Vtx*) &v1[18];
        } else if (i == 2) {
            vtx = (Vtx*) &v1[36];
        }
        // vtx = (Vtx *) segmented_to_virtual_dupe_2(&v1[0]);

        temp_v1 = (*arg1 * 2) + 2;

        //! @bug vtx array overflow temp fix
        if ((vtx + temp_v1) >= 54) {
            return;
        }
        temp_t6 = (vtx + temp_v1)->v.cn[0] * (256 - *arg2);
        temp_t9 = (vtx + temp_v1)->v.cn[1] * (256 - *arg2);
        temp_t7 = (vtx + temp_v1)->v.cn[2] * (256 - *arg2);
        temp_t8_2 = (vtx + temp_v1)->v.cn[3] * (256 - *arg2);

        temp_v1 = (((*arg1 * 2) + 2) % 6) + 2;
        a = ((vtx + temp_v1)->v.cn[0] * *arg2);
        b = ((vtx + temp_v1)->v.cn[1] * *arg2);
        c = ((vtx + temp_v1)->v.cn[2] * *arg2);
        d = ((vtx + temp_v1)->v.cn[3] * *arg2);

        //! @bug vtx array overflow temp fix
        if ((vtx + idx) >= 54) {
            return;
        }

        (vtx + idx + 1)->v.cn[0] = (temp_t6 + a) / 256;
        (vtx + idx + 1)->v.cn[1] = (temp_t9 + b) / 256;
        (vtx + idx + 1)->v.cn[2] = (temp_t7 + c) / 256;
        (vtx + idx + 1)->v.cn[3] = (temp_t8_2 + d) / 256;
    }
}

void func_80092500(void) {

    switch (gModeSelection) {
        case GRAND_PRIX:
            add_8018D9E0_entry(0xAA, 0, 0, 0);
            break;
        case TIME_TRIALS:
            add_8018D9E0_entry(0xB9, 0, 0, 0);
            break;
    }
}

void func_80092564(void) {
    add_8018D9E0_entry(0xAC, 0, 0, 0);
    func_8005D18C();
    func_8001968C();
}

void func_800925A0(void) {
    add_8018D9E0_entry(0xAF, 0, 0, 0);
}

void func_800925CC(void) {
    struct_8018D9E0_entry* temp = find_8018D9E0_entry_dupe(0xAF);
    if (temp->cursor == 2) {
        temp->cursor = 3;
    }
}

void func_80092604(void) {
    add_8018D9E0_entry(0xB0, 0, 0, 0);
}

void func_80092630(void) {
    add_8018D9E0_entry(0xBC, 0, 0, 0);
}

void func_8009265C(void) {
    add_8018D9E0_entry(0x12B, 0, 0, 2);
}

void func_80092688(void) {
    switch (D_800DC5E4) {
        case 0:
            add_8018D9E0_entry(0x1CE, 0, 0, 2);
            return;
        default:
            add_8018D9E0_entry(0x190, 0, 0, 2);
            add_8018D9E0_entry(0x191, 0, 0, 2);
            return;
        case 2:
            add_8018D9E0_entry(0x192, 0, 0, 2);
            add_8018D9E0_entry(0x193, 0, 0, 2);
            return;
        case 3:
            add_8018D9E0_entry(0x194, 0, 0, 2);
            add_8018D9E0_entry(0x195, 0, 0, 2);
            return;
        case 4:
            add_8018D9E0_entry(0x196, 0, 0, 2);
            add_8018D9E0_entry(0x197, 0, 0, 2);
            return;
        case 5:
            add_8018D9E0_entry(0x198, 0, 0, 2);
            add_8018D9E0_entry(0x199, 0, 0, 2);
            add_8018D9E0_entry(0x19A, 0, 0, 2);
            add_8018D9E0_entry(0x19B, 0, 0, 2);
            return;
        case 6:
            add_8018D9E0_entry(0x198, 0, 0, 2);
            add_8018D9E0_entry(0x19C, 0, 0, 2);
            add_8018D9E0_entry(0x19D, 0, 0, 2);
            add_8018D9E0_entry(0x19E, 0, 0, 2);
            return;
        case 7:
            add_8018D9E0_entry(0x19F, 0, 0, 2);
            add_8018D9E0_entry(0x1A0, 0, 0, 2);
            add_8018D9E0_entry(0x1A1, 0, 0, 2);
            add_8018D9E0_entry(0x1A2, 0, 0, 2);
            return;
        case 8:
            add_8018D9E0_entry(0x1A3, 0, 0, 2);
            add_8018D9E0_entry(0x1A4, 0, 0, 2);
            return;
        case 9:
            add_8018D9E0_entry(0x1A5, 0, 0, 2);
            add_8018D9E0_entry(0x1A6, 0, 0, 2);
            add_8018D9E0_entry(0x1A7, 0, 0, 2);
            add_8018D9E0_entry(0x1A8, 0, 0, 2);
            add_8018D9E0_entry(0x1A9, 0, 0, 2);
            return;
        case 10:
            add_8018D9E0_entry(0x1A5, 0, 0, 2);
            add_8018D9E0_entry(0x1A6, 0, 0, 2);
            add_8018D9E0_entry(0x1AA, 0, 0, 2);
            add_8018D9E0_entry(0x1AB, 0, 0, 2);
            add_8018D9E0_entry(0x1AC, 0, 0, 2);
            return;
        case 11:
            add_8018D9E0_entry(0x1AD, 0, 0, 2);
            add_8018D9E0_entry(0x1AE, 0, 0, 2);
            add_8018D9E0_entry(0x1AF, 0, 0, 2);
            add_8018D9E0_entry(0x1B0, 0, 0, 2);
            return;
        case 12:
            add_8018D9E0_entry(0x1B1, 0, 0, 2);
            add_8018D9E0_entry(0x1B2, 0, 0, 2);
            return;
        case 13:
            add_8018D9E0_entry(0x1B3, 0, 0, 2);
            add_8018D9E0_entry(0x1B4, 0, 0, 2);
            add_8018D9E0_entry(0x1B5, 0, 0, 2);
            return;
        case 14:
            add_8018D9E0_entry(0x1B6, 0, 0, 2);
            add_8018D9E0_entry(0x1B7, 0, 0, 2);
            add_8018D9E0_entry(0x1B8, 0, 0, 2);
            add_8018D9E0_entry(0x1B9, 0, 0, 2);
            add_8018D9E0_entry(0x1BA, 0, 0, 2);
            add_8018D9E0_entry(0x1BB, 0, 0, 2);
            add_8018D9E0_entry(0x1BC, 0, 0, 2);
            return;
        case 15:
            add_8018D9E0_entry(0x1BD, 0, 0, 2);
            add_8018D9E0_entry(0x1BE, 0, 0, 2);
            add_8018D9E0_entry(0x1BF, 0, 0, 2);
            add_8018D9E0_entry(0x1C0, 0, 0, 2);
            return;
        case 16:
            add_8018D9E0_entry(0x1C1, 0, 0, 2);
            add_8018D9E0_entry(0x1C2, 0, 0, 2);
            add_8018D9E0_entry(0x1C3, 0, 0, 2);
            return;
        case 17:
            add_8018D9E0_entry(0x1C4, 0, 0, 2);
            add_8018D9E0_entry(0x1C5, 0, 0, 2);
            add_8018D9E0_entry(0x1C6, 0, 0, 2);
            add_8018D9E0_entry(0x1C7, 0, 0, 2);
            add_8018D9E0_entry(0x1C8, 0, 0, 2);
            add_8018D9E0_entry(0x1C9, 0, 0, 2);
            add_8018D9E0_entry(0x1CA, 0, 0, 2);
            add_8018D9E0_entry(0x1CB, 0, 0, 2);
            add_8018D9E0_entry(0x1CC, 0, 0, 2);
            return;
        case 18:
            add_8018D9E0_entry(0x1CD, 0, 0, 2);
            return;
    }
}

void func_80092C80(void) {
    D_8018ED91 = 1;
}

// Originally func_80092C90
// Some kind of lookup function, seems to return an index to be used
// to get a character's width in pixels
s32 char_to_glyph_index(char* character) {
    s32 var_v1;
    s8 temp_v0;

    temp_v0 = *character;
    var_v1 = 1;
    if ((temp_v0 >= 'a') && (temp_v0 <= 'z')) {
        var_v1 = temp_v0 - 0x61;
    } else if ((temp_v0 >= 'A') && (temp_v0 <= 'Z')) {
        var_v1 = temp_v0 - 0x41;
    } else if ((temp_v0 >= '0') && (temp_v0 <= '9')) {
        var_v1 = temp_v0 - 0x10;
    } else if (temp_v0 == ' ') {
        var_v1 = -1;
    } else if (temp_v0 < 0) {
        // Handling EUC-JUP characters
        switch (temp_v0) { /* irregular */
            case -92:      // 0xA4
                var_v1 = func_80092E1C(character + 1);
                break;
            case -91: // 0xA5
                var_v1 = func_80092DF8(character + 1);
                break;
            case -95: // 0xA1
            case -93: // 0xA3
            case -85: // 0xAB
                var_v1 = func_80092EE4(character);
                break;
        }
    } else {
        switch (temp_v0) {
            case '!':
                var_v1 = 0x0000001A;
                break;
            case '-':
                var_v1 = 0x0000001B;
                break;
            case '?':
                var_v1 = 0x0000001C;
                break;
            case '\'':
                var_v1 = 0x0000001D;
                break;
            case '$':
                var_v1 = 0x0000001E;
                break;
            case '.':
                var_v1 = 0x0000001F;
                break;
            case '\"':
                var_v1 = 0x0000002A;
                break;
            case '*':
                var_v1 = 0x0000002B;
                break;
            case '+':
                var_v1 = 0x0000002C;
                break;
            // Displayed as "cc"
            case '(':
                var_v1 = 0x0000002D;
                break;
            case ',':
                var_v1 = 0x0000002E;
                break;
            default:
                var_v1 = -2;
                break;
        }
    }
    return var_v1;
}

s32 func_80092DF8(char* arg) {
    return func_80092E1C(arg) + 0x50;
}

s32 func_80092E1C(char* character) {
    s32 ret;
    UNUSED s32 test;
    u8 temp_t6;
    u8 temp;

    temp_t6 = (*character + 0x80);
    // Huh?
    temp = temp_t6;
    if (temp) {}
    if ((temp > 0x20) && (temp < 0x2B)) {
        if (temp % 2) {
            ret = ((temp - 0x21) / 2) + 0x7B;
        } else {
            ret = ((temp - 0x21) / 2) + 0x30;
        }
    } else if ((temp > 0x2A) && (temp < 0x6E)) {
        ret = temp + 0xA;
    } else {
        switch (temp) { /* irregular */
            case 0x6F:
                ret = 0x00000078;
                break;
            case 0x72:
            case 0x73:
                ret = temp + 7;
                break;
            default:
                ret = -2;
                break;
        }
    }
    return ret;
}

s32 func_80092EE4(char* character) {
    u8 temp_t6;
    s32 var_v1;

    temp_t6 = (character[1] + 0x80);
    var_v1 = 2;
    switch (character[0]) {    /* switch 3; irregular */
        case -95:              /* switch 3 */
            switch (temp_t6) { /* switch 1 */
                case 0x22:     /* switch 1 */
                case 0x24:     /* switch 1 */
                    var_v1 = 0x000000EA;
                    break;
                case 0x23: /* switch 1 */
                    var_v1 = 0x000000E9;
                    break;
                case 0x25: /* switch 1 */
                    var_v1 = 0x000000D0;
                    break;
                case 0x2A: /* switch 1 */
                    var_v1 = 0x000000E8;
                    break;
                case 0x30: /* switch 1 */
                    var_v1 = 0x000000EB;
                    break;
                case 0x47: /* switch 1 */
                    var_v1 = 0x000000D1;
                    break;
                case 0x49: /* switch 1 */
                    var_v1 = 0x000000D2;
                    break;
                case 0x5C: /* switch 1 */
                    var_v1 = 0x000000D3;
                    break;
                case 0x3C: /* switch 1 */
                case 0x3D: /* switch 1 */
                case 0x5D: /* switch 1 */
                    var_v1 = 0x000000D4;
                    break;
                default: /* switch 1 */
                    break;
            }
            break;
        case -93: /* switch 3 */
            if ((temp_t6 >= 0x30) && (temp_t6 < 0x3A)) {
                var_v1 = temp_t6 + 0xA5;
            } else {
                switch (temp_t6) { /* switch 2 */
                    case 0x44:     /* switch 2 */
                        var_v1 = 0x000000DF;
                        break;
                    case 0x43: /* switch 2 */
                    case 0x63: /* switch 2 */
                        var_v1 = 0x000000E0;
                        break;
                    case 0x4E: /* switch 2 */
                    case 0x6E: /* switch 2 */
                        var_v1 = 0x000000E1;
                        break;
                    case 0x50: /* switch 2 */
                    case 0x70: /* switch 2 */
                        var_v1 = 0x000000E2;
                        break;
                    case 0x52: /* switch 2 */
                    case 0x72: /* switch 2 */
                        var_v1 = 0x000000E3;
                        break;
                    case 0x73: /* switch 2 */
                        var_v1 = 0x000000E4;
                        break;
                    case 0x54: /* switch 2 */
                    case 0x74: /* switch 2 */
                        var_v1 = 0x000000E5;
                        break;
                    case 0x53: /* switch 2 */
                        var_v1 = 0x000000E6;
                        break;
                    case 0x56: /* switch 2 */
                    case 0x76: /* switch 2 */
                        var_v1 = 0x000000E7;
                        break;
                    default: /* switch 2 */
                        break;
                }
            }
            break;
        case -85: /* switch 3 */
            if (temp_t6 == 0x2E) {
                var_v1 = 0x000000E0;
            }
            break;
        default: /* switch 3 */
            var_v1 = 2;
    }
    return var_v1;
}

// Originally func_80093034
s32 get_string_width(char* buffer) {
    s32 glyphIndex;
    s32 stringWidth = 0;

    if (*buffer != 0) {
        do {
            glyphIndex = char_to_glyph_index(buffer);
            if (glyphIndex >= 0) {
                stringWidth += gGlyphDisplayWidth[glyphIndex];
            } else if (glyphIndex == -1) {
                stringWidth += 7;
            }
            if (glyphIndex >= 0x30) {
                buffer += 2;
            } else {
                buffer += 1;
            }
        } while (*buffer != 0);
    }
    return stringWidth;
}

void set_text_color(s32 arg0) {
    gTextColor = arg0;
}

UNUSED void func_800930E4(s32 arg0, s32 arg1, char* arg2) {
    set_text_color(TEXT_BLUE);
    func_80093324(arg0, arg1, arg2, 0, 1.0, 1.0);
}

// "tracking" is a uniform spacing between all characters in a given word
void print_text0(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode) {
    s32 stringWidth = 0;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                func_80099184((MkTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]));
                gDisplayListHead =
                    print_letter(gDisplayListHead,
                                 (MkTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]),
                                 column + (stringWidth * scaleX), row, mode, scaleX, scaleY);
                stringWidth += gGlyphDisplayWidth[glyphIndex] + tracking;
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                stringWidth += tracking + 7;
            } else {
                gSPDisplayList(gDisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }
    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void func_80093324(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text0(column, row, text, tracking, scaleX, scaleY, 1);
}

void func_80093358(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text0(column, row, text, tracking, scaleX, scaleY, 2);
}

// "tracking" is a uniform spacing between all characters in a given word
void print_text1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    while (*temp_string != 0) {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * scaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (arg6) {
        case 1:
            // ???
            do {
            } while (0);
        case 3:
            column -= stringWidth;
            break;
        case 2:
        case 4:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (arg6 < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    while (*text != 0) {
        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            func_80099184(segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]));
            gDisplayListHead = print_letter(gDisplayListHead, segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]),
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(gDisplayListHead++, D_020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void func_800936B8(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text1(column, row, text, tracking, scaleX, scaleY, 1);
}

void draw_text(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text1(column, row, text, tracking, scaleX, scaleY, 2);
}

void func_80093720(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text1(column, row, text, tracking, scaleX, scaleY, 3);
}

void func_80093754(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text1(column, row, text, tracking, scaleX, scaleY, 4);
}

void print_text2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    MkTexture* glyphTexture;
    s32 characterWidth;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                glyphTexture = (MkTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]);
                func_80099184(glyphTexture);
                gDisplayListHead =
                    print_letter(gDisplayListHead, glyphTexture, column - (gGlyphDisplayWidth[glyphIndex] / 2), row,
                                 arg6, scaleX, scaleY);
                if ((glyphIndex >= 0xD5) && (glyphIndex < 0xE0)) {
                    characterWidth = 0x20;
                } else {
                    characterWidth = 0xC;
                }
                column = column + (s32) ((characterWidth + tracking) * scaleX);
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                column = column + (s32) ((tracking + 7) * scaleX);
            } else {
                gSPDisplayList(gDisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }

    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void func_800939C8(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text2(column, row, text, tracking, scaleX, scaleY, 1);
}

void text_draw(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text2(column, row, text, tracking, scaleX, scaleY, 2);
}

void func_80093A30(s32 arg0) {
    func_8009E2A8(D_800F0B1C[arg0]);
}

void func_80093A5C(u32 arg0) {
    if (D_8015F788 == 0) {
        func_8009C918();
    }
    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            func_800940EC((s32) D_800F0B1C[arg0]);
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            if (D_8015F788 == 0) {
                func_80093C1C((s32) D_800F0B1C[arg0]);
            } else {
                func_800940EC((s32) D_800F0B1C[arg0]);
            }
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            if (D_8015F788 == 3) {
                func_800940EC((s32) D_800F0B1C[arg0]);
            } else {
                func_80093C1C((s32) D_800F0B1C[arg0]);
            }
            break;
    }
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
}

UNUSED void func_80093B70(u32 arg0) {
    if ((arg0 == 0) || (arg0 == 2) || (arg0 == 3) || (arg0 == 8)) {
        func_8009C918();
    }
    switch (arg0) {
        case 0:
            func_800940EC(0);
            break;
        case 2:
        case 3:
        case 8:
            func_80093C1C(0);
            break;
        case 1:
        case 4:
        case 9:
            func_800940EC(1);
            break;
        case 10:
            func_80093C1C(2);
            break;
        case 11:
            func_800940EC(3);
            break;
        default:
            break;
    }
}

void func_80093C1C(s32 arg0) {
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    func_8009CA6C(arg0);
    gSPDisplayList(gDisplayListHead++, D_02007F48);
}

UNUSED void func_80093C88(void) {
    return;
}

UNUSED void func_80093C90(void) {
    return;
}

void func_80093C98(s32 arg0) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gGfxPool->mtxEffect[gMatrixEffectCount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    func_800A8250();
    if (arg0 == 0) {
        func_800A54EC();
        func_8009CA6C(4);
        D_80165754 = gMatrixEffectCount;
        gMatrixEffectCount = 0;
    }
}

void func_80093E20(void) {
    func_80093C98(0);
}

void func_80093E40(void) {
    func_80093C98(1);
}

void func_80093E60(void) {
    s32 i;

    D_8018D9B4 = get_next_available_memory_addr(0x00002800);
    D_8018D9B0 = (u16*) get_next_available_memory_addr(0x000124F8);
    D_8018D9B8 = get_next_available_memory_addr(0x00001000);
    D_8018D9BC = get_next_available_memory_addr(4U);

    for (i = 0; i < 5; i++) {
        D_8018E7AC[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        D_8018E838[i] = 0;
    }

    func_80099110();
    func_8009A344();
    func_8009E620();
    func_80092258();
    D_8018ED91 = 0;
}

void func_80093F10(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gGfxPool->mtxEffect[gMatrixEffectCount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    func_80092290(4, D_8018E850, D_8018E858);
    func_80092290(5, (s32*) &D_8018E850[1], (s32*) &D_8018E858[1]);
    func_8009C918();
    func_80099A70();
    func_80099E54();
    func_800A8230();
    func_80099AEC();
    func_80099EC4();
    func_8009CA2C();
    gSPDisplayList(gDisplayListHead++, D_02007F48);
    gMatrixEffectCount = 0;
}

void func_800940EC(s32 arg0) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&gGfxPool->mtxEffect[gMatrixEffectCount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    func_80092290(4, D_8018E850, D_8018E858);
    func_80092290(5, (s32*) &D_8018E850[1], (s32*) &D_8018E858[1]);
    func_80092148();
    func_80099A70();
    func_80099E54();
    func_800A8230();
    func_80099AEC();
    func_80099EC4();
    func_8009CA6C(arg0);
    gSPDisplayList(gDisplayListHead++, D_02007F48);
    func_80057CE4();
}

void func_800942D0(void) {
    Mtx* test;
    f32 var_f26;
    s32 var_s2;
    s32 thing;
    test = &gGfxPool->mtxObject[0];
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxLookAt[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    guRotate(test, D_8018EDC8, 1.0f, 0.0f, 0.0f);
    guRotate(test + 1, D_8018EDCC, 0.0f, 1.0f, 0.0f);
    guScale(test + 2, 1.0f, 1.0f, D_8018EDC4);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0x00);
    gSPDisplayList(gDisplayListHead++, D_02007F60);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    if (D_800E8530 > 0) {
        var_f26 = D_800E8534;
        if (var_f26 > 10.0f) {
            var_f26 = 10.0f;
        }
        for (var_s2 = 0, thing = 0xC0; var_s2 < 0xC; var_s2++, thing -= 0x10) {
            guRotate(test, 0.0f, 1.0f, 0.0f, 0.0f);
            guRotate(test + 1, (var_s2 + 1) * D_800E8530 * var_f26, 0.0f, 1.0f, 0.0f);
            guScale(test + 2, 1.0f, 1.0f, 2.0f);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, thing);
            gSPDisplayList(gDisplayListHead++, startup_texture_dl4);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}

void func_80094660(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    move_segment_table_to_dmem();
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    guPerspective(&arg0->mtxScreen, &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    guLookAt(&arg0->mtxLookAt[0], 0.0f, 0.0f, (f32) D_8018EDC0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    func_800942D0();
    gDPPipeSync(gDisplayListHead++);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
}

void func_800947B4(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    move_segment_table_to_dmem();
    guPerspective(&arg0->mtxPersp[0], &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    guLookAt(&arg0->mtxLookAt[1], 0.0f, 0.0f, (f32) D_8018EDC0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    guRotate(&arg0->mtxObject[0], D_8018EDC8, 1.0f, 0, 0);
    guRotate(&arg0->mtxObject[1], D_8018EDCC, 0, 1.0f, 0);
    guRotate(&arg0->mtxObject[2], D_8018EDD0, 0, 0, 1.0f);
    guScale(&arg0->mtxObject[3], D_8018EDC4, D_8018EDC4, D_8018EDC4);
    guTranslate(&arg0->mtxObject[4], D_8018EDD4, D_8018EDD8, D_8018EDDC);
    gSPMatrix(gDisplayListHead++, &arg0->mtxPersp[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &arg0->mtxLookAt[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[0], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[2], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[3], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[4], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_02007FC8);
    func_800B0004();
    gSPDisplayList(gDisplayListHead++, D_02007650);
}

void func_80094A64(struct GfxPool* pool) {
    gMatrixHudCount = 0;
    gMatrixEffectCount = 0;
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&pool->mtxScreen, 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, &pool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007650);
    func_80094C60();
    func_80092290(4, D_8018E850, D_8018E858);
    func_80092290(5, (s32*) &D_8018E850[1], (s32*) &D_8018E858[1]);
    func_80099A70();
    func_8009C918();
    switch (gMenuSelection) {
        case START_MENU:
            func_80095574();
            func_80093E40();
            break;
        case OPTIONS_MENU:
        case DATA_MENU:
        case COURSE_DATA_MENU:
        case LOGO_INTRO_MENU:
        case CONTROLLER_PAK_MENU:
        case MAIN_MENU:
        case PLAYER_SELECT_MENU:
        case COURSE_SELECT_MENU:
            func_800A8230();
            func_80099AEC();
            break;
    }
    func_8009CA2C();
    gCycleFlashMenu += 1;
    gDPPipeSync(gDisplayListHead++);
    gSPDisplayList(gDisplayListHead++, D_020076B0);
}

void func_80094C60(void) {
    if (D_800E86A4 != 0) {
        func_80099110();
        func_8009A344();
        func_8009E620();
        func_8009B938();
        func_80092258();
        func_800B5F30();
        func_800B6014();
        func_800B3F74(gMenuSelection);
        switch (gMenuSelection) {
            case OPTIONS_MENU:
                add_8018D9E0_entry(0x00000023, 0, 0, 2);
                add_8018D9E0_entry(0x000000F1, 0, 0, 4);
                add_8018D9E0_entry(0x000000F0, 0, 0, 2);
                break;
            case DATA_MENU:
                add_8018D9E0_entry(0x00000023, 0, 0, 2);
                add_8018D9E0_entry(0x0000008C, 0, 0, 6);
                add_8018D9E0_entry(0x0000007C, 0, 0, 6);
                add_8018D9E0_entry(0x0000007D, 0, 0, 6);
                add_8018D9E0_entry(0x0000007E, 0, 0, 6);
                add_8018D9E0_entry(0x0000007F, 0, 0, 6);
                add_8018D9E0_entry(0x00000080, 0, 0, 6);
                add_8018D9E0_entry(0x00000081, 0, 0, 6);
                add_8018D9E0_entry(0x00000082, 0, 0, 6);
                add_8018D9E0_entry(0x00000083, 0, 0, 6);
                add_8018D9E0_entry(0x00000084, 0, 0, 6);
                add_8018D9E0_entry(0x00000085, 0, 0, 6);
                add_8018D9E0_entry(0x00000086, 0, 0, 6);
                add_8018D9E0_entry(0x00000087, 0, 0, 6);
                add_8018D9E0_entry(0x00000088, 0, 0, 6);
                add_8018D9E0_entry(0x00000089, 0, 0, 6);
                add_8018D9E0_entry(0x0000008A, 0, 0, 6);
                add_8018D9E0_entry(0x0000008B, 0, 0, 6);
                add_8018D9E0_entry(0x0000008D, 0, 0, 8);
                break;
            case COURSE_DATA_MENU:
                add_8018D9E0_entry(0x000000E6, 0, 0, 8);
                add_8018D9E0_entry(0x000000E7, 0, 0, 8);
                add_8018D9E0_entry(0x000000E8, 0, 0, 8);
                add_8018D9E0_entry(0x000000E9, 0, 0, 8);
                add_8018D9E0_entry(0x000000EA, 0, 0, 8);
                break;
            case LOGO_INTRO_MENU:
                add_8018D9E0_entry(0x000000FA, 0, 0, 0);
                break;
            case CONTROLLER_PAK_MENU:
                add_8018D9E0_entry(0x000000DA, 0, 0, 0);
                add_8018D9E0_entry(0x000000D2, 0, 0, 4);
                add_8018D9E0_entry(0x000000D4, 0, 0, 6);
                add_8018D9E0_entry(0x000000D3, 0, 0, 8);
                add_8018D9E0_entry(0x000000D5, 0, 0, 0x0A);
                add_8018D9E0_entry(0x000000D6, 0, 0, 0x0A);
                add_8018D9E0_entry(0x000000D7, 0, 0, 0x0A);
                add_8018D9E0_entry(0x000000D8, 0, 0, 0x0A);
                add_8018D9E0_entry(0x000000D9, 0, 0, 0x0A);
                break;
            case START_MENU:
                add_8018D9E0_entry(2, 0, 0, 4);
                add_8018D9E0_entry(1, 0, 0, 0);
                add_8018D9E0_entry(0x000000FB, 0, 0, 0);
                if (gControllerBits & 1) {
                    add_8018D9E0_entry(3, 0, 0, 2);
                } else {
                    add_8018D9E0_entry(4, 0, 0, 2);
                }
                add_8018D9E0_entry(5, 0, 0, 6);
                gDemoMode = 0;
                D_8018EE08 = 0;
                break;
            case MAIN_MENU:
                add_8018D9E0_entry(0x00000023, 0, 0, 2);
                add_8018D9E0_entry(0x0000000A, 0x0000015E, 0x00000011, 6);
                add_8018D9E0_entry(0x0000000E, 0x0000015E, 0x0000003E, 6);
                add_8018D9E0_entry(0x0000000D, 0x0000015E, 0x0000003E, 6);
                add_8018D9E0_entry(0x0000000C, 0x0000015E, 0x0000003E, 6);
                add_8018D9E0_entry(0x0000000B, 0x0000015E, 0x0000003E, 6);
                add_8018D9E0_entry(0x0000000F, 0x0000015E, 0x000000C8, 6);
                add_8018D9E0_entry(0x00000011, 0x0000015E, 0x000000C8, 6);
                add_8018D9E0_entry(0x00000010, 0x0000015E, 0x000000C8, 6);
                if (func_800B555C() != 0) {
                    add_8018D9E0_entry(0x00000015, 0, 0, 6);
                }
                add_8018D9E0_entry(0x00000014, 0, 0, 6);
                add_8018D9E0_entry(0x00000013, 0, 0, 6);
                add_8018D9E0_entry(0x00000012, 0, 0, 6);
                add_8018D9E0_entry(0x00000019, 0, 0, 6);
                add_8018D9E0_entry(0x00000018, 0, 0, 6);
                add_8018D9E0_entry(0x0000001B, 0, 0, 0x0C);
                break;
            case PLAYER_SELECT_MENU:
                add_8018D9E0_entry(0x00000024, 0, 0, 2);
                add_8018D9E0_entry(0x0000002A, 0, 0, 6);
                add_8018D9E0_entry(0x00000033, 0, 0, 6);
                add_8018D9E0_entry(0x0000002B, 0, 0, 8);
                add_8018D9E0_entry(0x0000002C, 0, 0, 8);
                add_8018D9E0_entry(0x0000002D, 0, 0, 6);
                add_8018D9E0_entry(0x0000002E, 0, 0, 6);
                add_8018D9E0_entry(0x0000002F, 0, 0, 6);
                add_8018D9E0_entry(0x00000030, 0, 0, 6);
                add_8018D9E0_entry(0x00000031, 0, 0, 6);
                add_8018D9E0_entry(0x00000032, 0, 0, 8);
                add_8018D9E0_entry(0x00000034, 0, 0, 0x0C);
                add_8018D9E0_entry(0x00000035, 0, 0, 0x0C);
                add_8018D9E0_entry(0x00000036, 0, 0, 0x0C);
                add_8018D9E0_entry(0x00000037, 0, 0, 0x0C);
                break;
            case COURSE_SELECT_MENU:
                add_8018D9E0_entry(0x00000025, 0, 0, 2);
                add_8018D9E0_entry(0x00000052, 0, 0, 6);
                if (gModeSelection != BATTLE) {
                    add_8018D9E0_entry(0x00000053, 0, 0, 4);
                    add_8018D9E0_entry(0x00000054, 0, 0, 4);
                    add_8018D9E0_entry(0x00000055, 0, 0, 4);
                    add_8018D9E0_entry(0x00000056, 0, 0, 4);
                    add_8018D9E0_entry(0x00000058, 0, 0, 6);
                    add_8018D9E0_entry(0x00000059, 0, 0, 6);
                    add_8018D9E0_entry(0x0000005A, 0, 0, 6);
                    add_8018D9E0_entry(0x0000005B, 0, 0, 6);
                } else {
                    add_8018D9E0_entry(0x0000005C, 0, 0, 6);
                    add_8018D9E0_entry(0x0000006E, 0, 0, 6);
                }
                add_8018D9E0_entry(0x00000064, 0, 0, 6);
                add_8018D9E0_entry(0x0000005F, 0, 0, 6);
                add_8018D9E0_entry(0x00000060, 0, 0, 6);
                add_8018D9E0_entry(0x00000061, 0, 0, 6);
                add_8018D9E0_entry(0x00000062, 0, 0, 6);
                add_8018D9E0_entry(0x0000005D, 0, 0, 6);
                add_8018D9E0_entry(0x0000005E, 0, 0, 8);
                if (gModeSelection == TIME_TRIALS) {
                    add_8018D9E0_entry(0x00000065, 0, 0, 8);
                    add_8018D9E0_entry(0x00000066, 0, 0, 8);
                    add_8018D9E0_entry(0x00000069, 0, 0, 8);
                }
                if (gModeSelection == GRAND_PRIX) {
                    add_8018D9E0_entry(0x00000068, 0, 0, 8);
                    add_8018D9E0_entry(0x00000067, 0, 0, 5);
                }
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            default:
                break;
        }
        if (D_800E86A4 != 2) {
            func_8009DF4C(0x00000014);
        } else {
            func_8009DF6C(0x00000014);
        }
        D_800E86A4 = 0;
    }
}

void func_80095574(void) {
    s32 var_v0;

    if ((D_8018EE0C < 3) || (D_8018E7AC[4] != 0)) {
        func_800A8230();
    }
    if (gDebugMenuSelection >= 2) {
        load_debug_font();
        debug_print_str2(0x00000050, 0x00000064, "debug_mode");
        switch (gDebugMenuSelection) {
            case DEBUG_MENU_DEBUG_MODE:
                debug_print_str2(0x00000046, 0x00000064, "*");
                break;
            case DEBUG_MENU_COURSE:
                debug_print_str2(0x00000046, 0x0000006E, "*");
                break;
            case DEBUG_MENU_SCREEN_MODE:
                debug_print_str2(0x00000046, 0x00000078, "*");
                break;
            case DEBUG_MENU_PLAYER:
                debug_print_str2(0x00000046, 0x00000082, "*");
                break;
            case DEBUG_MENU_SOUND_MODE:
                debug_print_str2(0x00000046, 0x0000008C, "*");
                break;
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP:
                debug_print_str2(0x00000046, 0x00000096, "*");
                break;
        }
        if (gEnableDebugMode) {
            debug_print_str2(0x000000AA, 0x00000064, "on");
        } else {
            debug_print_str2(0x000000AA, 0x00000064, "off");
        }
        if ((gCurrentCourseId >= (NUM_COURSES - 1)) || (gCurrentCourseId < 0)) {
            gCurrentCourseId = 0;
        }
        print_str_num(0x00000050, 0x0000006E, "map_number", gCurrentCourseId);
        if (gCurrentCourseId < 0xA) {
            var_v0 = 0;
        } else {
            var_v0 = 8;
        }
        debug_print_str2(var_v0 + 0xB9, 0x0000006E, gDebugCourseNames[gCurrentCourseId]);
        debug_print_str2(0x00000050, 0x00000078, "screen_mode");
        debug_print_str2(0x000000AA, 0x00000078, gDebugScreenModeNames[D_8018EDF1]);
        debug_print_str2(0x00000050, 0x00000082, "player");
        debug_print_str2(0x000000AA, 0x00000082, gDebugCharacterNames[gCharacterSelections[0]]);
        debug_print_str2(0x00000050, 0x0000008C, "sound mode");
        debug_print_str2(0x000000AA, 0x0000008C, gDebugSoundModeNames[gSoundMode]);
        if (gDebugMenuSelection == DEBUG_MENU_GIVE_ALL_GOLD_CUP) {
            debug_print_str2(0x00000050, 0x00000096, "push b to get all goldcup");
        }
        func_80057778();
    }
    if (gDebugMenuSelection == DEBUG_MENU_DISABLED) {
        gMenuTimingCounter += 1;
    } else {
        gMenuTimingCounter = 3;
    }
    if (gMenuTimingCounter == DEBUG_MENU_DEBUG_MODE) {
        play_sound2(SOUND_INTRO_WELCOME);
    }
    if (gMenuTimingCounter >= 0x12D) {
        func_8009E230();
        func_800CA0A0();
    }
    gSPDisplayList(gDisplayListHead++, D_020076E0);
}

// While this matches, its a little screwy
// This function seemingly needs to return a Gfx*, but doing that explicity doesn't match
// Instead we depend on the fact that the result of draw_box_fill is left
// in v0 which means it is returned, sort of.
// Its also weird that the displayListHead argument goes entirely unused. What's up with that?
Gfx* draw_flash_select_case(UNUSED Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 speed) {
    s32 greyscale;

    greyscale = ((gCycleFlashMenu % speed) << 9) / speed;
    if (greyscale > 0x100) {
        greyscale = 0x200 - greyscale;
    }

    if (greyscale > 0xFF) { // set max greyscale to 0xFF
        greyscale = 0xFF;
    }

#if AVOID_UB
    return gDisplayListHead =
               draw_box_fill(gDisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#else
    gDisplayListHead = draw_box_fill(gDisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#endif
}

Gfx* draw_flash_select_case_slow(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return draw_flash_select_case(displayListHead, ulx, uly, lrx, lry, 64);
}

Gfx* draw_flash_select_case_fast(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return draw_flash_select_case(displayListHead, ulx, uly, lrx, lry, 4);
}

Gfx* func_800959F8(Gfx* displayListHead, Vtx* arg1) {
    s32 index;
    Vtx* a_D_02007BB8 = (Vtx*) LOAD_ASSET(D_02007BB8);
    // Vtx *a_D_02007CD8 = (Vtx *) LOAD_ASSET(D_02007CD8);
    // Vtx *a_D_02007DF8 = (Vtx *) LOAD_ASSET(D_02007DF8);

    if ((s32) gTextColor < TEXT_BLUE_GREEN_RED_CYCLE_1) {
        index = gTextColor;
    } else {
        index = ((gTextColor * 2) + ((s32) gGlobalTimer % 2)) - 4;
    }
    if (arg1 == &a_D_02007BB8[0]) {
        gSPDisplayList(displayListHead++, D_800E84CC[index]);
    } else if (arg1 == &a_D_02007BB8[18]) {
        gSPDisplayList(displayListHead++, D_800E84EC[index]);
    } else if (arg1 == &a_D_02007BB8[36]) {
        gSPDisplayList(displayListHead++, D_800E850C[index]);
    }

    return displayListHead;
}

typedef struct {
    u16 i[4][4];
    u16 f[4][4];
} Mtx_u;

typedef union {
    Mtx_u u;
    Mtx_t m;
    long long int force_structure_alignment;
} Mtx2;

typedef union {
    s16 s[2];
    s32 w;
} TheWhyUnion;

// Why... Why... Why... This function is so bad it's not going in the header.
void func_80095AE0(Mtx2* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    TheWhyUnion sp14;
    TheWhyUnion sp10;
    TheWhyUnion spC;
    TheWhyUnion sp8;
    s32 i;

#ifdef AVOID_UB
    size_t row;
    size_t col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            arg0->m[row][col] = 0;
        }
    }
#else
    // clang-format off
    // should be inline
    for(i = 0; i < 16; i++) { arg0->m[0][i] = 0; }
    // clang-format on
#endif

    sp14.w = arg3 * 65536.0f;
    sp10.w = arg4 * 65536.0f;
    spC.w = arg1 * 65536.0f;
    sp8.w = arg2 * 65536.0f;
    arg0->u.i[0][0] = sp14.s[0];
    arg0->u.i[1][1] = sp10.s[0];
    arg0->u.i[2][2] = 1;
    arg0->u.i[3][0] = spC.s[0];
    arg0->u.i[3][1] = sp8.s[0];
    arg0->u.i[3][3] = 1;
    arg0->u.f[0][0] = sp14.s[1];
    arg0->u.f[1][1] = sp10.s[1];
    arg0->u.f[3][0] = spC.s[1];
    arg0->u.f[3][1] = sp8.s[1];
}

Gfx* func_80095BD0(Gfx* displayListHead, u8* arg1, f32 arg2, f32 arg3, u32 arg4, u32 arg5, f32 arg6, f32 arg7) {
    Vtx* var_a1;
    Mtx* sp28;

    Vtx* a_D_02007BB8 = (Vtx*) LOAD_ASSET(D_02007BB8);
    // Vtx *a_D_02007CD8 = (Vtx *) LOAD_ASSET(D_02007CD8);
    // Vtx *a_D_02007DF8 = (Vtx *) LOAD_ASSET(D_02007DF8);

    // A match is a match, but why are goto's required here?
    if (gMatrixEffectCount >= 0x2F7) {
        goto func_80095BD0_label1;
    }
    sp28 = &gGfxPool->mtxEffect[gMatrixEffectCount];
    if (gMatrixEffectCount < 0) {
        rmonPrintf("effectcount < 0 !!!!!!(kawano)\n");
    }
    goto func_80095BD0_label2;
func_80095BD0_label1:
    rmonPrintf("MAX effectcount(760) over!!!!(kawano)\n");
    return displayListHead;
func_80095BD0_label2:
    func_80095AE0(sp28, arg2, arg3, arg6, arg7);
    gSPMatrix(displayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gMatrixEffectCount += 1;
    gDPLoadTextureTile_4b(displayListHead++, arg1, G_IM_FMT_I, arg4, 0, 0, 0, arg4, arg5, 0, G_TX_NOMIRROR | G_TX_WRAP,
                          G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    switch (arg4) {
        default:
            var_a1 = &D_02007BB8[18];
            break;
        case 16:
            var_a1 = &D_02007BB8[18];
            break;
        case 26:
            var_a1 = D_02007BB8;
            break;
        case 30:
            var_a1 = &D_02007BB8[36];
            break;
    }

    return func_800959F8(displayListHead, var_a1);
}

// Player select menu character border
Gfx* func_80095E10(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while (var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < arg7; var_s3 += temp_lo) {

        if (arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < arg6; var_a1_2 += var_t0) {

            if (arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}

Gfx* func_800963F0(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB, u8* argC, u32 argD, u32 argE) {
    u32 var_a1_2 = arg6;
    u32 var_s3 = arg7;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    u32 a;
    u32 b;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while (var_t0 < argD) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((temp_lo / 2) > argE) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (argA < 0) {
        arg6 -= argA;
        argA = 0;
    } else if ((argA + (arg8 - arg6) * arg4) > SCREEN_WIDTH) {
        arg8 -= argA + (arg8 - arg6) * arg4 - SCREEN_WIDTH;
    }

    if (argB < 0) {
        arg7 -= argB;
        argB = 0;
    } else if ((argB + (arg9 - arg7) * arg5) > SCREEN_HEIGHT) {
        arg9 -= argB + (arg9 - arg7) * arg5 - SCREEN_HEIGHT;
    }

    if (arg8 < arg6) {
        return displayListHead;
    }
    if (arg9 < arg7) {
        return displayListHead;
    }
    arg2 /= arg4;
    arg3 /= arg5;

    sp7C = argA;
    for (var_s3 = arg7; var_s3 < arg9; var_s3 += temp_lo) {

        if (arg9 < temp_lo + var_s3) {
            var_s4 = arg9 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }
        b = var_s4 * arg5;
        for (var_a1_2 = arg6; var_a1_2 < arg8; var_a1_2 += var_t0) {

            if (arg8 < var_t0 + var_a1_2) {
                var_s2 = arg8 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            a = var_s2 * arg4;

            gDPLoadTextureTile(displayListHead++, argC, arg1, G_IM_SIZ_16b, argD, argE, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, argA * 4, argB * 4, (argA + a) * 4, (argB + b) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            argA += var_t0 * arg4;
        }

        argA = sp7C;
        argB += temp_lo * arg5;
    }
    return displayListHead;
}

#define D_0B002A00 gTextureTitleChocoMountain

#ifdef NON_MATCHING
// https://decomp.me/scratch/xV83r
// Possibly a missed variable rename or just weird diffs.

// I don't know what this actually meant to be. Its plausible that its meant to be a reference to
// `gTextureTitleChocoMountain` That would be weird though because this function doesn't draw that picture at all. So
// its plausible that its instead using it as some form semi-random data for the static pattern?

// This function is responsible for drawing a near unnoticeable static pattern
// over the course images when loading the cup selection screen
// Try locking the word at `8018DC80` to see something like 0x20 just before confirming character selection to make it
// last longer

Gfx* func_80096CD8(Gfx* displayListHead, s32 arg1, s32 arg2, u32 width, u32 arg4) {
    u32 var_s1_3;
    u32 var_fp;
    u32 var_v0;
    u32 var_a1;
    s32 var_ra = 1;
    s32 spCC;
    s32 masks = 0;
    s32 maskt = 0;
    s32 rand;

    while (var_ra < width) {
        var_ra *= 2;
    }

    spCC = 0x400 / var_ra;

    while ((spCC / 2) > arg4) {
        spCC /= 2;
    }

    rand = var_ra;
    while (rand > 1) {
        rand /= 2;
        masks += 1;
    }
    rand = spCC;
    while (rand > 1) {
        rand /= 2;
        maskt += 1;
    }

    if (arg1 < 0) {
        width -= arg1;
        arg1 = 0;
    } else if ((arg1 + width) > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - arg1;
    }
    if (arg2 < 0) {
        arg4 -= arg2;
        arg2 = 0;
    } else if ((arg2 + arg4) > SCREEN_HEIGHT) {
        arg4 = SCREEN_HEIGHT - arg2;
    }

    if (width == 0) {
        return displayListHead;
    }
    if (arg4 == 0) {
        return displayListHead;
    }

    rand = random_int(100);
    displayListHead = draw_box(displayListHead, arg1, arg2, arg1 + width, arg2 + arg4, 0, 0, 0, rand);
    rand += 150;
    gDPPipeSync(displayListHead++);
    gDPSetRenderMode(displayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(displayListHead++, 0, 0, rand, rand, rand, rand);
    gDPSetCombineMode(displayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    for (var_fp = arg2; var_fp < (arg2 + arg4); var_fp += spCC) {
        if ((var_fp + spCC) > (arg2 + arg4)) {
            var_v0 = (arg2 + arg4) - var_fp;
            if (var_v0 == 0) {
                break;
            }
        } else {
            var_v0 = spCC;
        }
        for (var_s1_3 = arg1; var_s1_3 < (arg1 + width); var_s1_3 += var_ra) {
            if ((var_s1_3 + var_ra) > (arg1 + width)) {
                var_a1 = (arg1 + width) - var_s1_3;
                if (var_a1 == 0) {
                    break;
                }
            } else {
                var_a1 = var_ra;
            }
            gDPLoadTextureTile(displayListHead++, D_0B002A00 + (random_int(128) * 2), G_IM_FMT_IA, G_IM_SIZ_16b, width,
                               arg4, var_s1_3, var_fp, var_s1_3 + var_a1, var_fp + var_v0, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, masks, maskt, G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(displayListHead++, var_s1_3 * 4, var_fp * 4, (var_s1_3 + var_a1) * 4,
                                (var_fp + var_v0) * 4, 0, (var_s1_3 * 32) & 0xFFFF, (var_fp * 32) & 0xFFFF, 1024, 1024);
        }
    }

    return displayListHead;
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80096CD8.s")
#endif

#ifdef NON_MATCHING
Gfx* func_80097274(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u16* argA, u32 argB, u32 argC, s32 argD) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_2CYCLE);
    gDPSetTextureLOD(displayListHead++, G_TL_TILE);
    gDPSetPrimColor(displayListHead++, 0, 0, 0, 0, 0, gGlobalTimer % 256);
    gDPSetCombineLERP(displayListHead++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0, 0,
                      0, 0, COMBINED, 0, 0, 0, COMBINED);

    while (var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < arg7; var_s3 += temp_lo) {

        if (arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < arg6; var_a1_2 += var_t0) {

            if (arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }

            gDPLoadMultiTile(displayListHead++, arg1, 0, G_TX_RENDERTILE, arg2, G_IM_SIZ_16b, argB, argC, var_a1_2,
                             var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_WRAP, G_TX_WRAP, sp68, sp64,
                             G_TX_NOLOD, G_TX_NOLOD);

            gDPLoadMultiTile(displayListHead++, D_0B002A00 + random_int(128) * 2, 256, G_TX_RENDERTILE + 1, arg2,
                             G_IM_SIZ_16b, argB, argC, var_a1_2, var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0,
                             G_TX_WRAP, G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80097274.s")
#endif

#undef D_0B002A00

Gfx* func_80097A14(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                   u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = func_80095E10(displayListHead, arg1, 0x00001000, 0x00000400, arg2, arg3, arg4, arg5, arg6, arg7,
                                    arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* func_80097AE4(Gfx* displayListHead, s8 fmt, s32 arg2, s32 arg3, u8* arg4, s32 width) {
    s32 i;
    s32 temp;
    s32 arg2Copy;
    s32 dsdx;

    if (width >= 32) {
        return displayListHead;
    }

    arg2Copy = arg2;

    for (i = 0; i < 64; i += 32) {
        temp = 0;
        dsdx = 0x8000 / (32 - width);
        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp, i, temp + 32, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg2 + width) << 2, arg3 << 2, (arg2 + 32) << 2, (arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        arg2 += 32;

        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp + 32, i, temp + 64, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg2 << 2, arg3 << 2, ((arg2 - width) + 32) << 2, (arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        arg2 = arg2Copy;
        arg3 += 32;
    }
    return displayListHead;
}

// Render player characters at player select menu
Gfx* func_80097E58(Gfx* displayListHead, s8 fmt, u32 arg2, u32 arg3, u32 arg4, u32 arg5, s32 arg6, s32 arg7,
                   u8* someTexture, u32 arg9, u32 argA, s32 width) {
    u32 ult;
    u32 temp;
    s32 arg6Copy;
    s32 temp_v1;
    s32 var_s2;
    s32 lrs;
    s32 spDC;
    s32 temp2 = 32;

    if (width >= 32) {
        return displayListHead;
    }

    arg6Copy = arg6;

    lrs = arg9 / 2;
    spDC = arg9 - lrs;
    for (ult = arg3; ult < arg5; ult += 32) {
        temp = 0;
        if ((ult + temp2) > arg5) {
            var_s2 = arg5 - ult;
            if (!var_s2) {
                break;
            }
        } else {
            var_s2 = temp2;
        }
        temp_v1 = ((32 * lrs) << 10) / (lrs * (32 - width));

        // Renders the left side
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg6 + lrs * (width) / 32) << 2, arg7 << 2, (((arg6) + lrs) << 2),
                            (((arg7) + var_s2) << 2), 0, 0, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 += lrs;
        temp_v1 = ((32 * spDC) << 10) / (spDC * (32 - width));
        // Renders the right side
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp + arg9,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg6 << 2, arg7 << 2, ((arg6 + spDC * (32 - (width)) / 32) << 2),
                            (((arg7) + var_s2) << 2), 0, (1 + lrs << 5) & 0xFFFF, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 = arg6Copy;
        arg7 += temp2;
    }
    return displayListHead;
}

Gfx* func_80098558(Gfx* displayListHead, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, UNUSED s32 arg7,
                   s32 arg8) {
    u32 var_a3;
    u32 var_v0;
    s32 arg5Copy;

    arg5Copy = arg5;
    for (var_v0 = arg2; var_v0 < arg4; var_v0 += 0x20) {
        for (var_a3 = arg1; var_a3 < arg3; var_a3 += 0x20) {
            gDPLoadTextureTile(displayListHead++, D_8018D9B0, G_IM_FMT_RGBA, G_IM_SIZ_16b, arg8, 0, var_a3, var_v0,
                               var_a3 + 0x20, var_v0 + 0x20, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5,
                               5, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg5 << 2, arg6 << 2, (arg5 + 0x20) << 2, (arg6 + 0x20) << 2, 0, 0,
                                0, 1024, 1024);
            arg5 += 0x20;
        }
        arg5 = arg5Copy;
        arg6 += 0x20;
    }
    return displayListHead;
}

Gfx* func_800987D0(Gfx* displayListHead, u32 arg1, u32 arg2, u32 width, u32 height, s32 column, s32 row,
                   UNUSED u8* arg7, u32 textureWidth, UNUSED s32 textureHeight) {
    s32 var_a2;
    s32 var_v0_2;
    s32 columnCopy;
    s32 temp_f4_2;
    UNUSED s32 stackPadding0;
    s32 temp_f6;
    f32 temp_f0;
    f32 temp_f18;
    f32 temp_f24;

    if (D_8018E7B8[0] == 0) {
        D_8018E7B8[0] = 1;
    }
    temp_f24 = sins(((D_8018E7D0[0] * 0x4E20) / D_8018E7B8[0]) % 20000U);
    temp_f0 = coss(((D_8018E7D0[0] * 0x4E20) / D_8018E7B8[0]) % 20000U);
    temp_f18 = (((f32) D_8018E7D0[0] * 0.5) / D_8018E7B8[0]) + 1.0;
    columnCopy = column;
    for (var_v0_2 = arg2; (u32) var_v0_2 < height; var_v0_2 += 0x20) {
        for (var_a2 = arg1; (u32) var_a2 < width; var_a2 += 0x20) {
            gDPLoadTextureTile(displayListHead++, D_8018D9B0, G_IM_FMT_RGBA, G_IM_SIZ_16b, textureWidth, 0, var_a2,
                               var_v0_2, var_a2 + 0x20, var_v0_2 + 0x20, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
            temp_f6 = (temp_f18 * ((temp_f0 * (column - 0xA0)) + (temp_f24 * (row - 0x78)))) + 160.0f;
            temp_f4_2 = (temp_f18 * ((-temp_f24 * (column - 0xA0)) + (temp_f0 * (row - 0x78)))) + 120.0f;
            gSPTextureRectangle(displayListHead++, temp_f6 * 4, temp_f4_2 * 4, ((temp_f6 + 0x20) ^ 0) * 4,
                                ((temp_f4_2 + 0x20) ^ 0) * 4, 0, 0, 0, 1024, 1024);
            column += 0x20;
        }
        column = columnCopy;
        row += 0x20;
    }
    return displayListHead;
}

// draw a box filled with a solid color
Gfx* draw_box_fill(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 red, s32 green, s32 blue, s32 alpha) {
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    if (lrx < ulx) {
        swap_values(&ulx, &lrx);
    }
    if (lry < uly) {
        swap_values(&uly, &lry);
    }
    if ((ulx >= 0x140) || (uly >= 0xF0)) {
        return displayListHead;
    }
    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if ((lrx < 0) || (lry < 0)) {
        return displayListHead;
    }
    if (lrx >= SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH - 1;
    }
    if (lry >= SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT - 1;
    }
    gSPDisplayList(displayListHead++, D_02008030);
    gDPSetFillColor(displayListHead++, (GPACK_RGBA5551(red, green, (u32) blue, alpha) << 0x10 |
                                        GPACK_RGBA5551(red, green, (u32) blue, alpha)));
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gSPDisplayList(displayListHead++, D_02008058);
    return displayListHead;
}

// draw a box filled with a solid color
Gfx* draw_box_fill_wide(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 red, s32 green, s32 blue,
                        s32 alpha) {
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    if (lrx < ulx) {
        swap_values(&ulx, &lrx);
    }
    if (lry < uly) {
        swap_values(&uly, &lry);
    }
    if ((ulx >= 0x140) || (uly >= 0xF0)) {
        return displayListHead;
    }
    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if ((lrx < 0) || (lry < 0)) {
        return displayListHead;
    }
    if (lrx >= SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH - 1;
    }
    if (lry >= SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT - 1;
    }
    gSPDisplayList(displayListHead++, D_02008030);
    gDPSetFillColor(displayListHead++, (GPACK_RGBA5551(red, green, (u32) blue, alpha) << 0x10 |
                                        GPACK_RGBA5551(red, green, (u32) blue, alpha)));
    gDPFillWideRectangle(displayListHead++, OTRGetDimensionFromLeftEdge(ulx), uly, OTRGetDimensionFromRightEdge(lrx),
                         lry);
    // gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gSPDisplayList(displayListHead++, D_02008058);
    return displayListHead;
}

// draw a box with a solid outline
Gfx* draw_box(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, u32 red, u32 green, u32 blue, u32 alpha) {
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    if (lrx < ulx) {
        swap_values(&ulx, &lrx);
    }
    if (lry < uly) {
        swap_values(&uly, &lry);
    }
    if ((ulx >= 0x140) || (uly >= 0xF0)) {
        return displayListHead;
    }
    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if ((lrx < 0) || (lry < 0)) {
        return displayListHead;
    }
    if (lrx >= 0x141) {
        lrx = 0x140;
    }
    if (lry >= 0xF1) {
        lry = 0xF0;
    }
    gSPDisplayList(displayListHead++, D_02008008);
    gDPSetPrimColor(displayListHead++, 0, 0, red, green, blue, alpha);
    // gDPFillWideRectangle(displayListHead++, OTRGetRectDimensionFromLeftEdge(0), uly,
    // OTRGetRectDimensionFromRightEdge(SCREEN_WIDTH), lry);
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gDPPipeSync(displayListHead++);
    return displayListHead;
}

//! @todo Need to call this somewhere on func_800A2D1C
Gfx* draw_box_fill_wide_wrap(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return draw_box_fill_wide(displayListHead, ulx, uly, lrx, lry, 0, 0, 0, 0xFF);
}

Gfx* func_80098FC8(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return draw_box_fill(displayListHead, ulx, uly, lrx, lry, 0, 0, 0, 0xFF);
}

void dma_copy_base_729a30(u64* arg0, size_t nbytes, void* vaddr) {
#ifdef TARGET_N64
    OSIoMesg sp30;
    OSMesg sp2C;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&sp30, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(arg0)],
                 vaddr, nbytes, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &sp2C, OS_MESG_BLOCK);
#endif
}

void dma_copy_base_7fa3c0(u64* arg0, size_t nbytes, void* vaddr) {
#ifdef TARGET_N64
    OSIoMesg sp30;
    OSMesg sp2C;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&sp30, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0bSegmentRomStart[SEGMENT_OFFSET(arg0)],
                 vaddr, nbytes, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &sp2C, OS_MESG_BLOCK);
#endif
}

void func_80099110(void) {
    gD_8018E118TotalSize = 0;
    gNumD_8018E118Entries = 0;
}

/**
 * Differs from memory.c with `+ 0x8` instead of `| 0x8`
 *
 * @param addr
 * @return void*
 */
void* segmented_to_virtual_dupe(const void* addr) {
#ifdef TARGET_N64
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((gSegmentTable[segment] + offset) + 0x80000000);
#else
    return addr;
#endif
}

void* segmented_to_virtual_dupe_2(const void* addr) {
#ifdef TARGET_N64
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((gSegmentTable[segment] + offset) + 0x80000000);
#else
    return addr;
#endif
}

#include <assets/player_selection.h>

#ifdef NON_MATCHING
// https://decomp.me/scratch/NAZ12
// Register allocation nonsense
void func_80099184(MkTexture* arg0) {
    u16 var_a1_2;
    s32 var_v0;
    s32 var_a1;
    MkTexture* var_s1;
    struct_8018E118_entry* thing = &D_8018E118[0];
    var_s1 = segmented_to_virtual_dupe(arg0);
    while (var_s1->textureData != NULL) {
        var_a1 = 0;
        for (var_v0 = 0; var_v0 < gNumD_8018E118Entries; var_v0++) {
            if (var_s1->textureData == (thing + var_v0)->textureData) {
                var_a1 = 1;
                break;
            }
        }

        if (var_a1 == 0) {
            if (var_s1->type == 3) {
                if (var_s1->size != 0) {
                    var_a1_2 = var_s1->size;
                } else {
                    var_a1_2 = 0x1000;
                }
                if (var_a1_2 % 8) {
                    var_a1_2 = ((var_a1_2 / 8) * 8) + 8;
                }
                // dma_copy_base_729a30(var_s1->textureData, var_a1_2, D_8018D9B4);
                // mio0decode(D_8018D9B4, &D_8018D9B0[gD_8018E118TotalSize]);
                // size_t texSize = ResourceGetTexSizeByName(var_s1->textureData);
                // memcpy(&D_8018D9B0[gD_8018E118TotalSize], var_s1->textureData, texSize);
                strcpy(&D_8018D9B0[gD_8018E118TotalSize], var_s1->textureData);
            } else {
                // dma_copy_base_729a30(var_s1->textureData, (var_s1->height * var_s1->width) * 2,
                // &D_8018D9B0[gD_8018E118TotalSize]); memcpy(&D_8018D9B0[gD_8018E118TotalSize], var_s1->textureData,
                // var_s1->width * var_s1->height*2);
                strcpy(&D_8018D9B0[gD_8018E118TotalSize], var_s1->textureData);
            }
            thing[gNumD_8018E118Entries].textureData = var_s1->textureData;
            thing[gNumD_8018E118Entries].offset = gD_8018E118TotalSize;
            gD_8018E118TotalSize += var_s1->height * var_s1->width;
            gD_8018E118TotalSize = ((gD_8018E118TotalSize / 8) * 8) + 8;
            gNumD_8018E118Entries += 1;
        }
        var_s1++;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80099184.s")
#endif

#ifdef NON_MATCHING
// https://decomp.me/scratch/O2tkD
// Register allocation nonsense
void func_80099394(MkTexture* arg0) {
    UNUSED u16 var_a1_2;
    s32 var_v0;
    s32 var_a1;
    UNUSED s32 temp_s3;
    MkTexture* var_s1;
    struct_8018E118_entry* thing;

    var_s1 = segmented_to_virtual_dupe(arg0);
    while (var_s1->textureData != NULL) {
        var_a1 = 0;
        for (var_v0 = 0; var_v0 < gNumD_8018E118Entries; var_v0++) {
            // wtf is going on here?
            if (D_8018E118[var_v0 ^ 0].textureData == (*var_s1).textureData) {
                var_a1 = 1;
                break;
            }
        }
        if (var_a1 == 0) {
            if (var_s1->type == 5) {
#ifdef TARGET_N64
                dma_copy_base_729a30(var_s1->textureData, (u32) ((s32) (var_s1->height * var_s1->width) / 2),
                                     &D_8018D9B0[gD_8018E118TotalSize]);
#else
                u8* tex = LOAD_ASSET(var_s1->textureData);
                size_t texSize = ResourceGetTexSizeByName(var_s1->textureData);
                memcpy(&D_8018D9B0[gD_8018E118TotalSize], tex, texSize);
#endif
            }

            thing = &D_8018E118[gNumD_8018E118Entries];
            thing->textureData = var_s1->textureData;
            thing = &D_8018E118[gNumD_8018E118Entries];
            thing->offset = gD_8018E118TotalSize;

            gD_8018E118TotalSize += (var_s1->height * var_s1->width);
            gD_8018E118TotalSize = ((gD_8018E118TotalSize / 8) * 8) + 8;
            gNumD_8018E118Entries += 1;
        }
        var_s1++;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80099394.s")
#endif

#ifdef NON_MATCHING
// Register allocation nonsense
// https://decomp.me/scratch/Wv2MX
void func_8009952C(MkTexture* arg0) {
    UNUSED u16 var_a1_2;
    s32 var_v0;
    s32 var_a1;
    UNUSED s32 temp_s3;
    MkTexture* var_s1;
    struct_8018E118_entry* thing;

    var_s1 = segmented_to_virtual_dupe(arg0);
    while (var_s1->textureData != NULL) {
        var_a1 = 0;
        for (var_v0 = 0; var_v0 < gNumD_8018E118Entries; var_v0++) {
            // wtf is going on here?
            if (D_8018E118[var_v0 ^ 0].textureData == (*var_s1).textureData) {
                var_a1 = 1;
                break;
            }
        }
        if (var_a1 == 0) {
#ifdef TARGET_N64
            dma_copy_base_729a30(var_s1->textureData, 0x00008000U, D_8018D9B4);
            mio0decode(D_8018D9B4, (u8*) &D_8018D9B0[gD_8018E118TotalSize]);
#else
            memcpy(&D_8018D9B0[gD_8018E118TotalSize], var_s1->textureData, var_s1->width * var_s1->height * 2);
#endif

            thing = &D_8018E118[gNumD_8018E118Entries];
            thing->textureData = var_s1->textureData;
            thing = &D_8018E118[gNumD_8018E118Entries];
            thing->offset = gD_8018E118TotalSize;

            gD_8018E118TotalSize += (var_s1->height * var_s1->width);
            gD_8018E118TotalSize = ((gD_8018E118TotalSize / 8) * 8) + 8;
            gNumD_8018E118Entries += 1;
        }
        var_s1++;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_8009952C.s")
#endif

void func_8009969C(MkTexture* arg0) {
    func_800996BC(arg0, 1);
}

#ifdef NON_MATCHING
// Register allocation nonsense
// https://decomp.me/scratch/hwAAp
void func_800996BC(MkTexture* arg0, s32 arg1) {
    u16 var_a1_2;
    s32 var_v0;
    s32 var_a1;
    u8 var_v0_2;
    MkTexture* texture;
    struct_8018E118_entry* thing = &D_8018E118[0];

    texture = segmented_to_virtual_dupe(arg0);

    size_t siz = texture->width * texture->height;
    // for (size_t i = 0; i < siz; i++) {
    //     printf(" 0x%llX", texture->textureData[i]);
    // }

    // Because it's loading an mkTexture. The second element in the array is usually all zeros.
    // Despite this loop, it's usually only ran once.
    while (texture->textureData != NULL) {
        var_a1 = 0;
        for (var_v0 = 0; var_v0 < gNumD_8018E118Entries; var_v0++) {
            if (texture->textureData == (thing + var_v0)->textureData) {
                var_a1 = 1;
                break;
            }
        }
        if ((var_a1 == 0) || (arg1 > 0)) {
            if (texture->size != 0) {
                var_a1_2 = texture->size;
            } else {
                var_a1_2 = 0x1000;
            }
            if ((var_a1_2 % 8)) {
                var_a1_2 = (((var_a1_2 / 8) * 8) + 8);
            }
            switch (arg1) { /* irregular */
                case -1:
                case 1:
                    // dma_copy_base_729a30(texture->textureData, var_a1_2, D_8018D9B4);
                    break;
                case 0:
                case 2:
                    // dma_copy_base_7fa3c0(texture->textureData, var_a1_2, D_8018D9B4);
                    break;
            }
            switch (arg1) { /* switch 1; irregular */
                case -1:    /* switch 1 */
                case 1: {
                    // mio0decode(D_8018D9B4, (u8*)&D_8018D9B0[gD_8018E118TotalSize]);
                    // printf("w: %d, h: %d", texture->width, texture->height);
                    u8* tex = (u8*) LOAD_ASSET(texture->textureData);
                    memcpy(&D_8018D9B0[gD_8018E118TotalSize], tex, texture->width * texture->height * 2);
                    break;
                }
                case 0: /* switch 1 */
                case 2: /* switch 1 */
                    if (texture->type == 1) {
                        var_v0_2 = 0x000000BE;
                    } else {
                        var_v0_2 = 1;
                    }
                    if (1) {}
                    // D_8018D9B0[gD_8018E118TotalSize] = &gTextureBackgroundBlueSky;
                    // tkmk00decode(D_8018D9B4, texture->textureData, (u8*)&D_8018D9B0[gD_8018E118TotalSize], var_v0_2);
                    u8* tex2 = (u8*) LOAD_ASSET(texture->textureData);
                    memcpy(&D_8018D9B0[gD_8018E118TotalSize], tex2, texture->width * texture->height * 2);
                    break;
            }

            thing[gNumD_8018E118Entries].textureData = texture->textureData;
            thing[gNumD_8018E118Entries].offset = gD_8018E118TotalSize;
            gD_8018E118TotalSize += texture->height * texture->width;
            gD_8018E118TotalSize = ((gD_8018E118TotalSize / 8) * 8) + 8;
            gNumD_8018E118Entries += 1;
        }
        texture++;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800996BC.s")
#endif

void func_80099958(MkTexture* arg0, s32 arg1, s32 arg2) {
    u16 var_a1;
    UNUSED u8* thing;
    MkTexture* temp_v0;

    temp_v0 = segmented_to_virtual_dupe(arg0);
    while (temp_v0->textureData != NULL) {
        if (temp_v0->size != 0) {
            var_a1 = temp_v0->size;
        } else {
            var_a1 = 0x1400;
        }
        if (var_a1 % 8) {
            // Round up to the next multiple of eight
            var_a1 = ((var_a1 / 8) * 8) + 8;
        }
        // dma_copy_base_729a30(temp_v0->textureData, var_a1, D_8018D9B4);
        // mio0decode(D_8018D9B4, D_802BFB80.arraySize4[arg2][arg1 / 2][(arg1 % 2) + 2].pixel_index_array);
        u8* tex = LOAD_ASSET(temp_v0->textureData);
        memcpy(D_802BFB80.arraySize4[arg2][arg1 / 2][(arg1 % 2) + 2].pixel_index_array, temp_v0->textureData,
               temp_v0->width * temp_v0->height * 2);
        temp_v0++;
    }
}

// Possibly a debug print function?
void func_80099A70(void) {
    s32 i;
    D_8018E060[0].texture = NULL;
    for (i = 0; i < D_8018E060_SIZE; i++) {}
}

void func_80099A94(MkTexture* arg0, s32 arg1) {
    struct_8018E060_entry* var_v1;

    var_v1 = &D_8018E060[0];
    while (var_v1->texture != NULL) {
        var_v1++;
    }
    var_v1->texture = segmented_to_virtual_dupe(arg0);
    var_v1->unk_4 = arg1;
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/rxEoi
// Something's up with the handling of `_textures_0aSegmentRomStart`, I don't know how to fix it
void func_80099AEC(void) {
    s8 var_s4;
    s32 size;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 huh;
    OSIoMesg sp68;
    OSMesg sp64;
    UNUSED u8* test;
    s32 sp60;
    MkTexture* temp_s2;
    struct_8018E060_entry* var_s1;

    if (gGamestate == RACING) {
        sp60 = 0x00000500;
    } else {
        sp60 = 0x00001000;
    }

    var_s4 = 0;
    var_s1 = &D_8018E060[0];
    temp_s2 = var_s1->texture;

    if (temp_s2 == NULL)
        return;

    huh = temp_s2->size;
    if (huh != 0) {
        size = huh;
    } else {
        size = 0x1400;
    }
    if (size % 8) {
        size = ((size / 8) * 8) + 8;
    }
    osInvalDCache(D_8018D9B4, size);
#ifdef TARGET_N64
    osPiStartDma(&sp68, 0, 0, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)],
                 D_8018D9B4, size, &gDmaMesgQueue);
#endif
    osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    while (1) {
        if ((var_s1 + 1)->texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->texture;
            huh = (var_s1 + 1)->texture->size;
            if (huh != 0) {
                size = huh;
            } else {
                size = 0x1400;
            }
            if (size % 8) {
                size = ((size / 8) * 8) + 8;
            }
            osInvalDCache(D_8018D9B4 + sp60 * 4, size);
#ifdef TARGET_N64
            osPiStartDma(&sp68, 0, 0, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)],
                         D_8018D9B4 + sp60 * 4, size, &gDmaMesgQueue);
#endif
        }
#ifdef TARGET_N64
        mio0decode(D_8018D9B4, (u8*) &D_8018D9B0[D_8018E118[var_s1->unk_4].offset]);
#else
        memcpy(&D_8018D9B0[D_8018E118[var_s1->unk_4].offset], var_s1->texture->textureData,
               var_s1->texture->width * var_s1->texture->height * 2);
#endif
        var_s1->texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
        if ((var_s1 + 1)->texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->texture;
            huh = (var_s1 + 1)->texture->size;
            if (huh != 0) {
                size = huh;
            } else {
                size = 0x1400;
            }
            if (size % 8) {
                size = ((size / 8) * 8) + 8;
            }
            osInvalDCache(D_8018D9B4, size);
#ifdef TARGET_N64
            osPiStartDma(&sp68, 0, 0, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)],
                         D_8018D9B4, size, &gDmaMesgQueue);
#endif
        }
#ifdef TARGET_N64
        mio0decode(D_8018D9B4 + sp60 * 4, (u8*) &D_8018D9B0[D_8018E118[var_s1->unk_4].offset]);
#else
        memcpy(&D_8018D9B0[D_8018E118[var_s1->unk_4].offset], var_s1->texture->textureData,
               var_s1->texture->width * var_s1->texture->height * 2);
#endif
        var_s1->texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80099AEC.s")
#endif

void func_80099E54(void) {
    D_8018E0E8[0].mk64Texture = NULL;
}

void func_80099E60(MkTexture* arg0, s32 arg1, s32 arg2) {
    struct_8018E0E8_entry* var_v1;

    var_v1 = D_8018E0E8;
    while (var_v1->mk64Texture != NULL) {
        var_v1++;
    }
    var_v1->mk64Texture = segmented_to_virtual_dupe(arg0);
    var_v1->unk4 = arg1;
    var_v1->unk6 = arg2;
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/rUXbD
// Some fakematch nonsense, may or may not be necessary
// Issue is with instruction ordering near the first `osPiStartDma` call
void func_80099EC4(void) {
    s8 var_s4;
    s32 var_s0;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    OSIoMesg sp68;
    OSMesg sp64;
    s32 huh;
    u8* test;
    MkTexture* temp_s2;
    struct_8018E0E8_entry* var_s1;

    var_s4 = 0;
    var_s1 = D_8018E0E8;
    temp_s2 = var_s1->mk64Texture;

    if (temp_s2 == NULL)
        return;

    huh = temp_s2->size;
    if (huh != 0) {
        var_s0 = huh;
    } else {
        var_s0 = 0x1400;
    }
    if (var_s0 % 8) {
        var_s0 = ((var_s0 / 8) * 8) + 8;
    }
    osInvalDCache(D_8018D9B4, var_s0);
#ifdef TARGET_N64
    test = &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)];
    osPiStartDma(&sp68, 0, 0, (uintptr_t) test, D_8018D9B4, var_s0, &gDmaMesgQueue);
#endif
    if ((var_s0 && var_s0) && var_s0) {}
    // osPiStartDma(&sp68, 0, 0, &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)], D_8018D9B4, var_s0,
    // &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    while (1) {
        if ((var_s1 + 1)->mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->mk64Texture;
            huh = (var_s1 + 1)->mk64Texture->size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(D_8018D9B4 + 0x1400, var_s0);
#ifdef TARGET_N64
            osPiStartDma(&sp68, 0, 0, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)],
                         D_8018D9B4 + 0x1400, var_s0, &gDmaMesgQueue);
#endif
        }
#ifdef TARGET_N64
        mio0decode(D_8018D9B4,
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array);
#else
        memcpy(D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array,
               temp_s2->textureData, temp_s2->width * temp_s2->height * 2);
#endif
        var_s1->mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
        if ((var_s1 + 1)->mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->mk64Texture;
            huh = (var_s1 + 1)->mk64Texture->size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(D_8018D9B4, var_s0);
#ifdef TARGET_N64
            osPiStartDma(&sp68, 0, 0, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(temp_s2->textureData)],
                         D_8018D9B4, var_s0, &gDmaMesgQueue);
#endif
        }
#ifdef TARGET_N64
        mio0decode(D_8018D9B4 + 0x1400,
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array);
#else
        memcpy(D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array,
               temp_s2->textureData, temp_s2->width * temp_s2->height * 2);
#endif
        var_s1->mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_80099EC4.s")
#endif

void func_8009A238(MkTexture* arg0, s32 arg1) {
    s32 var_a3;
    s32 temp_v1;
    u64* sp24;
    UNUSED struct_8018E118_entry* temp_v0;

    temp_v1 = D_8018E118[arg1].offset;
    sp24 = arg0->textureData;
    var_a3 = arg0->size;
    if (var_a3 % 8) {
        var_a3 = ((var_a3 / 8) * 8) + 8;
    }
    // dma_copy_base_7fa3c0(sp24, var_a3, D_8018D9B4);
    // tkmk00decode(D_8018D9B4, D_8018D9B8, (u8*) &D_8018D9B0[temp_v1], 1);
    // u8 *tex = (u8 *) LOAD_ASSET(arg0->textureData);
    size_t texSize = ResourceGetTexSizeByName(arg0->textureData);
    memcpy(&D_8018D9B0[temp_v1], arg0->textureData, texSize);
    D_8018E118[arg1].textureData = arg0->textureData;
}

void func_8009A2F0(struct_8018E0E8_entry* arg0) {
    MkTexture* var_a0;
    struct_8018E0E8_entry* temp_v0;

    temp_v0 = segmented_to_virtual_dupe_2(arg0);
    var_a0 = temp_v0->mk64Texture;
    while (var_a0 != NULL) {
        if (var_a0 == NULL) {
            break;
        }
        func_800996BC(var_a0, 0);
        if (1) {}
        temp_v0++;
        var_a0 = temp_v0->mk64Texture;
    }
}

void func_8009A344(void) {
    s32 index;
    for (index = 0; index < D_8018DEE0_SIZE; index++) {
        D_8018DEE0[index].visible = 0;
    }
}

s32 func_8009A374(MkAnimation* anim) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = segmented_to_virtual_dupe_2(anim);
    i = 0;
    while (D_8018DEE0[i].visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &D_8018DEE0[i];
    entry->textureSequence = anim;
    entry->sequenceIndex = -1;
    entry->frameCountDown = 0;
    entry->visible = 0x80000000;
    entry->D_8018E118_index = gNumD_8018E118Entries;

    if (anim[0].mk64Texture) {
        func_8009969C(anim[0].mk64Texture);
    }
    if (anim[1].mk64Texture) {
        func_8009969C(anim[1].mk64Texture);
    } else {
        func_8009969C(anim[0].mk64Texture);
    }

    entry->unk14 = 0;
    return i;
}

s32 func_8009A478(MkAnimation* anim, s32 arg1) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = segmented_to_virtual_dupe_2(anim);
    i = 0;
    while (D_8018DEE0[i].visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &D_8018DEE0[i];
    entry->textureSequence = anim;
    entry->sequenceIndex = -1;
    entry->frameCountDown = 0;
    entry->visible = 0x80000000;
    entry->D_8018E118_index = gNumD_8018E118Entries;
    if (anim[0].mk64Texture) {
        func_80099958(anim[0].mk64Texture, arg1, 0);
    }
    if (anim[1].mk64Texture) {
        func_80099958(anim[1].mk64Texture, arg1, 1);
    } else {
        func_80099958(anim[0].mk64Texture, arg1, 1);
    }
    entry->unk14 = 0;
    return i;
}

void func_8009A594(s32 arg0, s32 arg1, MkAnimation* arg2) {
    MkAnimation* temp_v0;
    MkTexture* temp_a0;

    temp_v0 = segmented_to_virtual_dupe_2(arg2);
    D_8018DEE0[arg0].textureSequence = temp_v0;
    D_8018DEE0[arg0].sequenceIndex = arg1;
    // All hail the fake match gods who, in their infinite grace, have blessed us
    // with this enigma of a match on the first iteration of permutation
    D_8018DEE0[arg0].frameCountDown = (temp_v0 + arg1)->frame_length;
    temp_a0 = segmented_to_virtual_dupe(temp_v0[arg1].mk64Texture);
    if (D_8018DEE0[arg0].unk14 != 0) {
        func_80099A94(temp_a0, D_8018DEE0[arg0].D_8018E118_index);
        D_8018DEE0[arg0].unk14 = 0;
    } else {
        func_80099A94(temp_a0, D_8018DEE0[arg0].D_8018E118_index + 1);
        D_8018DEE0[arg0].unk14 = 1;
    }
}

void func_8009A640(s32 arg0, s32 arg1, s32 arg2, MkAnimation* arg3) {
    MkAnimation* temp_v0;
    MkTexture* temp_a0;

    temp_v0 = segmented_to_virtual_dupe_2(arg3);
    D_8018DEE0[arg0].textureSequence = temp_v0;
    D_8018DEE0[arg0].sequenceIndex = arg1;
    D_8018DEE0[arg0].frameCountDown = (temp_v0 + arg1)->frame_length;
    temp_a0 = segmented_to_virtual_dupe(temp_v0[arg1].mk64Texture);
    D_8018DEE0[arg0].unk14 ^= 1;
    func_80099E60(temp_a0, arg2, D_8018DEE0[arg0].unk14);
}

UNUSED void func_8009A6D4(void) {
    s32 index;
    for (index = 0; index < D_8018DEE0_SIZE; index++) {
        if ((D_8018DEE0[index].visible & 0x80000000) != 0) {
            func_8009A878(&D_8018DEE0[index]);
            gDisplayListHead = func_8009C434(gDisplayListHead, &D_8018DEE0[index], 0, 0, 0);
        }
    }
    func_80099AEC();
}

void func_8009A76C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct_8018DEE0_entry* temp = &D_8018DEE0[arg0];
    if (temp->visible & 0x80000000) {
        func_8009A878(temp);
        gDisplayListHead = func_8009C434(gDisplayListHead, temp, arg1, arg2, arg3);
    }
}

void func_8009A7EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct_8018DEE0_entry* temp = &D_8018DEE0[arg0];
    if (temp->visible & 0x80000000) {
        func_8009A944(temp, arg3);
        gDisplayListHead = func_8009C708(gDisplayListHead, temp, arg1, arg2, arg3, arg4);
    }
}

MkTexture* func_8009A878(struct_8018DEE0_entry* arg0) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MkTexture* temp_a0;

    temp_v1 = arg0->textureSequence;
    if (arg0->sequenceIndex < 0) {
        arg0->sequenceIndex = 0;
        arg0->frameCountDown = 0;
    }
    arg0->frameCountDown--;
    if (arg0->frameCountDown <= 0) {
        arg0->sequenceIndex++;
        // Again, hail the fake match gods
        var_v0 = ((test = temp_v1) + arg0->sequenceIndex);
        if (var_v0->mk64Texture == NULL) {
            arg0->sequenceIndex = 0;
        }
        var_v0 = (test + arg0->sequenceIndex);
        arg0->frameCountDown = var_v0->frame_length;
        temp_a0 = segmented_to_virtual_dupe(var_v0->mk64Texture);
        if (arg0->unk14 != 0) {
            func_80099A94(temp_a0, arg0->D_8018E118_index);
            arg0->unk14 = 0;
        } else {
            func_80099A94(temp_a0, arg0->D_8018E118_index + 1);
            arg0->unk14 = 1;
        }
    }
    return arg0->textureSequence[arg0->sequenceIndex].mk64Texture;
}

MkTexture* func_8009A944(struct_8018DEE0_entry* arg0, s32 arg1) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MkTexture* temp_a0;

    temp_v1 = arg0->textureSequence;
    if (arg0->sequenceIndex < 0) {
        arg0->sequenceIndex = 0;
        arg0->frameCountDown = 0;
    }
    arg0->frameCountDown--;
    if (arg0->frameCountDown <= 0) {
        arg0->sequenceIndex++;
        var_v0 = ((test = temp_v1) + arg0->sequenceIndex);
        if (var_v0->mk64Texture == NULL) {
            arg0->sequenceIndex = 0;
        }
        var_v0 = (test + arg0->sequenceIndex);
        arg0->frameCountDown = var_v0->frame_length;
        temp_a0 = segmented_to_virtual_dupe(var_v0->mk64Texture);
        arg0->unk14 ^= 1;
        func_80099E60(temp_a0, arg1, arg0->unk14);
    }
    return arg0->textureSequence[arg0->sequenceIndex].mk64Texture;
}

void func_8009A9FC(s32 arg0, s32 arg1, u32 arg2, s32 arg3) {
    s32 red;
    s32 green;
    s32 blue;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    s32 alpha;
    s32 temp_t9;
    u16 temp_a0;
    s32 var_t1;
    u16* color0;
    u16* color1;

    color0 = &D_8018D9B0[D_8018E118[arg0].offset];
    color1 = &D_8018D9B0[D_8018E118[arg1].offset];
    for (var_t1 = 0; (u32) var_t1 < arg2; var_t1++) {
        temp_a0 = BSWAP16(*color0++);
        red = (temp_a0 & 0xF800) >> 0xB;
        green = (temp_a0 & 0x7C0) >> 6;
        blue = (temp_a0 & 0x3E) >> 1;
        alpha = temp_a0 & 0x1;
        if (alpha) {}
        temp_t9 = ((red * 0x4D) + (green * 0x96) + (blue * 0x1D)) >> 8;
        newred = (((((temp_t9 - red) * arg3) >> 8) + red) << 0xB);
        newgreen = (((((((temp_t9 * 7) / 8) - green) * arg3) >> 8) + green) << 6);
        newblue = (((((((temp_t9 * 6) / 8) - blue) * arg3) >> 8) + blue) << 1);
        *color1++ = BSWAP16(newblue + newgreen + newred + alpha);
    }
}

void func_8009AB7C(s32 arg0) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &D_8018D9B0[D_8018E118[arg0].offset];
    for (var_v1 = 0; var_v1 < 0x4B000; var_v1++) {
        u16 color_pixel = BSWAP16(*color);
        red = ((color_pixel & 0xF800) >> 0xB) * 0x4D;
        green = ((color_pixel & 0x7C0) >> 6) * 0x96;
        blue = ((color_pixel & 0x3E) >> 1) * 0x1D;
        alpha = color_pixel & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 >>= 8;
        newred = temp_t9 << 0xB;
        newgreen = temp_t9 << 6;
        newblue = temp_t9 << 1;
        *color++ = BSWAP16(newblue + newgreen + newred + alpha);
    }
}

void func_8009AD78(s32 arg0, s32 arg1) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    UNUSED s32 newred;
    UNUSED s32 newgreen;
    UNUSED s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    s32 size;
    UNUSED u16 temp_a0;
    u16* color;

    color = &D_8018D9B0[D_8018E118[arg0].offset];
    size = D_8018E118[arg0 + 1].offset - D_8018E118[arg0].offset;
    for (var_v1 = 0; var_v1 != size; var_v1++) {
        u16 color_pixel = BSWAP16(*color);
        red = ((color_pixel & 0xF800) >> 0xB) * 0x4D;
        green = ((color_pixel & 0x7C0) >> 6) * 0x96;
        blue = ((color_pixel & 0x3E) >> 1) * 0x1D;
        alpha = color_pixel & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 = temp_t9 >> 8;
        temp_t9 += ((0x20 - temp_t9) * arg1) >> 8;
        *color++ = BSWAP16((temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + alpha);
    }
}

void func_8009B0A4(s32 arg0, u32 arg1) {
    u32 var_s0;
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    u32 temp_t9;
    s32 size;
    u16* color;
    f32 sp48[0x20];

    for (var_s0 = 0; var_s0 < 0x20; var_s0++) {
        sp48[var_s0] = func_800917B0(var_s0 * 0.03125, (arg1 * 1.5 * 0.00390625) + 0.25);
    }
    color = &D_8018D9B0[D_8018E118[arg0].offset];
    size = D_8018E118[arg0 + 1].offset - D_8018E118[arg0].offset;
    for (var_s0 = 0; var_s0 < (u32) size; var_s0++) {
        u16 color_pixel = BSWAP16(*color);
        red = ((color_pixel & 0xF800) >> 0xB) * 0x55;
        green = ((color_pixel & 0x7C0) >> 6) * 0x4B;
        blue = ((color_pixel & 0x3E) >> 1) * 0x5F;
        alpha = color_pixel & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 >>= 8;
        temp_t9 = sp48[temp_t9] * 32.0f;
        if (temp_t9 >= 0x20) {
            temp_t9 = 0x1F;
        }
        *color++ = BSWAP16((temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + alpha);
    }
}

void func_8009B538(s32 arg0, s32 screen_size, s32 arg2, s32 arg3, s32 arg4) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &D_8018D9B0[D_8018E118[arg0].offset];
    for (var_v1 = 0; var_v1 < screen_size; var_v1++) {
        u16 color_pixel = BSWAP16(*color);
        red = ((color_pixel & 0xF800) >> 0xB) * 0x4D;
        green = ((color_pixel & 0x7C0) >> 6) * 0x96;
        blue = ((color_pixel & 0x3E) >> 1) * 0x1D;
        alpha = (color_pixel & 0x1);
        temp_t9 = red + green + blue;
        temp_t9 = temp_t9 >> 8;
        newred = ((temp_t9 * arg2) >> 8) << 0xB;
        newgreen = ((temp_t9 * arg3) >> 8) << 6;
        newblue = ((temp_t9 * arg4) >> 8) << 1;
        *color++ = BSWAP16(newred + newgreen + newblue + alpha);
    }
}

u16* func_8009B8C4(u64* arg0) {
    UNUSED s32 pad[2];
    s32 offset;
    s32 found;
    s32 someIndex;

    found = 0;
    for (someIndex = 0; someIndex < gNumD_8018E118Entries; someIndex++) {
        if (arg0 == D_8018E118[someIndex].textureData) {
            found = 1;
            offset = D_8018E118[someIndex].offset;
            break;
        }
    }

    if (found != 0) {
        return &D_8018D9B0[offset];
    }
    return NULL;
}

// D_8018D9C0 is a little weird. In code_800AF9B0 its treated as a
// struct_8018EE10_entry pointer. But here its being treated as a
// Gfx pointer. It seems to be multi use.
void func_8009B938(void) {
    D_8018E75C = (Gfx*) D_8018D9C0;
    gNumD_8018E768Entries = 0;
}

void func_8009B954(MkTexture* arg0) {
    D_8018E768[gNumD_8018E768Entries].textures = segmented_to_virtual_dupe(arg0);
    D_8018E768[gNumD_8018E768Entries].displayList = D_8018E75C;
}

void func_8009B998(void) {
    gSPEndDisplayList(D_8018E75C++);
    gNumD_8018E768Entries += 1;
}

// I don't get how this compiles, given that there's a code path
// that has no explicit return value.
// Based on the target assembly, in the event that it never finds
// the desired entry, it'll treat the return value of segmented_to_virtual_dupe
// as the return of this function. Which seems like a bug to me
Gfx* func_8009B9D0(Gfx* displayListHead, MkTexture* textures) {
    Gfx* displayList;
    UNUSED s32 pad;
    bool found;
    s32 index;

    found = false;
    for (index = 0; index < D_8018E768_SIZE; index++) {
        if (D_8018E768[index].textures == segmented_to_virtual_dupe(textures)) {
            displayList = D_8018E768[index].displayList;
            found = true;
            break;
        }
    }
    if (found) {
        gSPDisplayList(displayListHead++, displayList);
        return displayListHead;
    }
}

Gfx* func_8009BA74(Gfx* arg0, MkTexture* arg1, s32 column, s32 row) {
    MkTexture* temp_v0;
    u8* temp_v0_3;
    s8 var_s4;

    temp_v0 = segmented_to_virtual_dupe(arg1);
    while (temp_v0->textureData != NULL) {
        var_s4 = 0;
        switch (temp_v0->type) {
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 2:
                gSPDisplayList(arg0++, D_02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_s4 = 3;
                break;
            case 4:
                gSPDisplayList(arg0++, D_02007788);
                break;
            default:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_v0_3 = (u8*) func_8009B8C4(temp_v0->textureData);
        if (temp_v0_3 != 0) {
            if (D_8018E7AC[4] != 4) {
                arg0 =
                    func_80095E10(arg0, var_s4, 0x00000400, 0x00000400, 0, 0, temp_v0->width, temp_v0->height,
                                  temp_v0->dX + column, temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            } else {
                arg0 = func_800987D0(arg0, 0U, 0U, temp_v0->width, temp_v0->height, temp_v0->dX + column,
                                     temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            }
        }
        temp_v0++;
    }
    return arg0;
}

Gfx* func_8009BC9C(Gfx* arg0, MkTexture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    MkTexture* var_s0;
    u8* temp_v0_3;

    var_s0 = segmented_to_virtual_dupe(arg1);
    while (var_s0->textureData != NULL) {
        switch (var_s0->type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_v0_3 = (u8*) func_8009B8C4(var_s0->textureData);
        if (temp_v0_3 != 0) {
            switch (arg4) { /* switch 1; irregular */
                case 1:     /* switch 1 */
                    arg0 = func_80097AE4(arg0, 0, var_s0->dX + arg2, var_s0->dY + arg3, temp_v0_3, arg5);
                    break;
                case 2: /* switch 1 */
                    arg0 = func_80097E58(arg0, 0, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                         var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height, arg5);
                    break;
                case 3: /* switch 1 */
                    arg0 = func_80097A14(arg0, 0, 0, 0, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                         var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height);
                    break;
                case 4: /* switch 1 */
                    arg0 = func_80097274(arg0, 0, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height,
                                         arg5);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* print_letter(Gfx* arg0, MkTexture* glyphTexture, f32 arg2, f32 arg3, s32 mode, f32 scaleX, f32 scaleY) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 thing0;
    f32 thing1;
    MkTexture* var_s0;

    var_s0 = segmented_to_virtual_dupe(glyphTexture);
    while (var_s0->textureData != NULL) {
        var_v0 = 0;

        thing0 = var_s0->dX + arg2;
        if (thing0 > 320.0f) {
            var_v0 = 1;
        }
        thing0 += var_s0->width * scaleX;
        if (thing0 < 0.0f) {
            var_v0 += 1;
        }
        thing1 = var_s0->dY + arg3;
        if (thing1 < 0.0f) {
            var_v0 += 1;
        }
        thing1 -= var_s0->height * scaleY;
        if (thing1 > 240.0f) {
            var_v0 += 1;
        }

        if (var_v0 != 0) {
            var_s0++;
        } else {
            temp_v0_2 = (u8*) func_8009B8C4(var_s0->textureData);
            if (temp_v0_2 != 0) {
                switch (mode) { /* irregular */
                    case 1:
                        gSPDisplayList(arg0++, D_020077F8);
                        arg0 = func_80095BD0(arg0, temp_v0_2, var_s0->dX + arg2, var_s0->dY + arg3, var_s0->width,
                                             var_s0->height, scaleX, scaleY);
                        break;
                    case 2:
                        gSPDisplayList(arg0++, D_02007818);
                        arg0 = func_80095BD0(arg0, temp_v0_2, var_s0->dX + arg2, var_s0->dY + arg3, var_s0->width,
                                             var_s0->height, scaleX, scaleY);
                        break;
                }
            }
            var_s0++;
        }
    }
    return arg0;
}

Gfx* func_8009C204(Gfx* arg0, MkTexture* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 var_s2;
    u8* temp_t0;
    MkTexture* var_s1;

    var_s1 = segmented_to_virtual_dupe(arg1);
    while (var_s1->textureData != NULL) {
        var_s2 = 0;
        switch (var_s1->type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_s2 = 3;
                break;
            default:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_t0 = (u8*) func_8009B8C4(var_s1->textureData);
        switch (arg4) { /* switch 1; irregular */
            case 2:     /* switch 1 */
                arg0 =
                    func_800963F0(arg0, var_s2, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s1->width, var_s1->height,
                                  var_s1->dX + arg2, var_s1->dY + arg3, temp_t0, var_s1->width, var_s1->height);
                break;
            case 3: /* switch 1 */
                arg0 = func_800963F0(arg0, var_s2, 0x00000400, 0x00000400, 0.457f, 0.5f, 0, 0, var_s1->width,
                                     var_s1->height, var_s1->dX + arg2, var_s1->dY + arg3, temp_t0, var_s1->width,
                                     var_s1->height);
                break;
        }
        var_s1++;
    }
    return arg0;
}

Gfx* func_8009C434(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 var_t0;
    s32 var_t1;
    Gfx* temp;
    MkTexture* var_s0;

    var_s0 = segmented_to_virtual_dupe(arg1->textureSequence[arg1->sequenceIndex].mk64Texture);
    temp = D_02007728;
    while (var_s0->textureData != NULL) {
        var_t1 = 0;
        switch (var_s0->type) { /* irregular */
            default:
                gSPDisplayList(arg0++, temp);
                break;
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, temp);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_t1 = 3;
                break;
        }
        if (arg1->unk14 != 0) {
            var_t0 = D_8018E118[arg1->D_8018E118_index + 1].offset;
        } else {
            var_t0 = D_8018E118[arg1->D_8018E118_index].offset;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        if (arg4 >= 0) {
            arg0 =
                func_80097E58(arg0, var_t1, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                              (u8*) &D_8018D9B0[var_t0], var_s0->width, var_s0->height, (u32) arg4);
        } else {
            switch (arg4) {
                case -1:
                    arg0 = func_80095E10(arg0, var_t1, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, (u8*) &D_8018D9B0[var_t0], var_s0->width,
                                         var_s0->height);
                    break;
                case -2:
                    arg0 = func_800963F0(arg0, var_t1, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s0->width,
                                         var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                                         (u8*) &D_8018D9B0[var_t0], var_s0->width, var_s0->height);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* func_8009C708(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 var_t0;
    UNUSED s32 thing;
    Gfx* temp;
    MkTexture* var_s1;

    var_s1 = segmented_to_virtual_dupe(arg1->textureSequence[arg1->sequenceIndex].mk64Texture);
    temp = D_02007728;
    while (var_s1->textureData != NULL) {
        var_t0 = 0;
        switch (var_s1->type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, temp);
                break;
            case 2:
                gSPDisplayList(arg0++, D_02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_t0 = 3;
                break;
            default:
                gSPDisplayList(arg0++, temp);
                break;
        }
        if (arg5 >= 0) {
            arg0 =
                func_80097E58(arg0, var_t0, 0, 0U, var_s1->width, var_s1->height, var_s1->dX + arg2, var_s1->dY + arg3,
                              D_802BFB80.arraySize4[arg1->unk14][arg4 / 2][(arg4 % 2) + 2].pixel_index_array,
                              var_s1->width, var_s1->height, (u32) arg5);
        }
        var_s1++;
    }
    return arg0;
}

void func_8009C918(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        D_8018E7E8[someIndex].x = D_8015F480[someIndex].screenStartX;
        D_8018E7E8[someIndex].y = D_8015F480[someIndex].screenStartY;
        D_8018E810[someIndex].x = D_8015F480[someIndex].screenWidth;
        D_8018E810[someIndex].y = D_8015F480[someIndex].screenHeight;
    }

    D_8018E7E8[4].x = 0x00A0;
    D_8018E7E8[4].y = 0x0078;
    D_8018E810[4].x = 0x0140;
    D_8018E810[4].y = 0x00F0;
}

void func_8009CA2C(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        func_8009CA6C(var_s0);
    }
}

void func_8009CA6C(s32 arg0) {
    s32 var_a1;

    if ((arg0 == 4) || ((find_8018D9E0_entry(0x000000AA) == NULL) && (find_8018D9E0_entry(0x000000AB) == NULL) &&
                        (find_8018D9E0_entry(0x000000B9) == NULL) && (find_8018D9E0_entry(0x000000BA) == NULL) &&
                        (find_8018D9E0_entry(0x000000AC) == NULL) && (find_8018D9E0_entry(0x000000B0) == NULL))) {
        var_a1 = 0;
        gSPDisplayList(gDisplayListHead++, D_0D0076F8);
        if ((arg0 != 4) && (gIsGamePaused != 0)) {
            var_a1 = 1;
        }
        switch (D_8018E7AC[arg0]) {
            case 1:
                func_8009CDDC(arg0, var_a1);
                return;
            case 2:
                func_8009D958(arg0, var_a1);
                return;
            case 3:
                func_8009DB8C();
                return;
            case 4:
                func_8009DAA8();
                return;
            case 5:
                func_8009D998(arg0);
                return;
            case 7:
                func_8009D978(arg0, var_a1);
                return;
            case 8:
                func_8009CDFC(arg0, var_a1);
                break;
            case 0:
            default:
                break;
        }
    }
}

void func_8009CBE4(s32 arg0, s32 arg1, s32 arg2) {
    RGBA16* color;
    s16 x, y, w, h;
    UNUSED s32 pad[3];
    struct UnkStruct_800DC5EC* unk;
    struct UnkStruct_8018E7E8 *size, *start;

    if ((gModeSelection == GRAND_PRIX) || (gModeSelection == TIME_TRIALS)) {
        start = &(D_8018E7E8[arg0]);
        size = &(D_8018E810[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else if (arg0 >= 4) {
        start = &(D_8018E7E8[arg0]);
        size = &(D_8018E810[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else {
        unk = &D_8015F480[arg0];
        x = unk->screenStartX;
        y = unk->screenStartY;
        w = unk->screenWidth;
        h = unk->screenHeight;
    }
    color = &D_800E7AE8[arg2];
    gDisplayListHead = draw_box(gDisplayListHead, x - (w / 2), y - (h / 2), (w / 2) + x, (h / 2) + y, color->red,
                                color->green, color->blue, 0xFF - (D_8018E7D0[arg0] * 0xFF / D_8018E7B8[arg0]));

    if ((arg1 == 0) && (D_8018E7D0[arg0] += 1, (D_8018E7D0[arg0] >= D_8018E7B8[arg0]))) {
        if (gGamestate == 4) {
            D_8018E7AC[arg0] = 6;
            return;
        }
        D_8018E7AC[arg0] = 0;
        D_8018EE0C = 0;
    }
}

void func_8009CDDC(s32 arg0, s32 arg1) {
    func_8009CBE4(arg0, arg1, 0);
}

void func_8009CDFC(s32 arg0, s32 arg1) {
    func_8009CBE4(arg0, arg1, 1);
}

void func_8009CE1C(void) {
    if ((gSoundMode != 3) && (gPlayerCountSelection1 >= 2)) {
        func_800C3448(0xE0000002);
    }
}

void func_8009CE64(s32 arg0) {
    s32 thing;
    s32 var_a1;
    UNUSED s32 stackPadding0;
    struct_8018D9E0_entry* temp_v0;

    var_a1 = 0;
    if (gGamestate == 5) {
        if (2 != gCCSelection) {
            thing = gCCSelection;
            if (thing != 3) {
                goto func_8009CE64_label1;
            }
            goto func_8009CE64_label2;
        }
    func_8009CE64_label2:
        if ((D_802874D8.unk1D < 3) && (gCupSelection == 3)) {
            var_a1 = 1;
        }
    func_8009CE64_label1:
        if (var_a1) {
            gGotoMenu = 9;
            gCreditsCourseId = 8;
        } else {
            gGotoMenu = 1;
            gMenuSelection = 0x0000000B;
        }
    } else if (gGamestate == 4) {
        if (D_8018E7AC[arg0] == 2) {
            if (arg0 != 4) {
                D_8018E7AC[arg0] = 5;
            } else {
                var_a1 = 0;
                temp_v0 = find_8018D9E0_entry(0x000000B0);
                if (temp_v0 != NULL) {
                    switch (temp_v0->cursor) { /* switch 8; irregular */
                        case 10:               /* switch 8 */
                            func_802903B0();
                            break;
                        case 11: /* switch 8 */
                            func_80290388();
                            break;
                        case 12: /* switch 8 */
                            func_80290360();
                            break;
                        default: /* switch 8 */
                        case 13: /* switch 8 */
                            func_80290338();
                            break;
                    }
                } else {
                    var_a1 = 0;
                    temp_v0 = find_8018D9E0_entry(0x000000AC);
                    if (temp_v0 != NULL) {
                        switch (temp_v0->cursor) { /* switch 7; irregular */
                            case 11:               /* switch 7 */
                                func_802903B0();
                                D_8016556E = 1;
                                break;
                            default: /* switch 7 */
                            case 12: /* switch 7 */
                                func_80290338();
                                break;
                        }
                    } else {
                        var_a1 = 0;
                        temp_v0 = find_8018D9E0_entry(0x000000C7);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->cursor) { /* switch 1 */
                                case 12:               /* switch 1 */
                                    func_802903B0();
                                    var_a1 = 1;
                                    break;
                                case 13: /* switch 1 */
                                case 22: /* switch 1 */
                                case 42: /* switch 1 */
                                    func_80290388();
                                    var_a1 = 1;
                                    break;
                                case 14: /* switch 1 */
                                case 23: /* switch 1 */
                                case 43: /* switch 1 */
                                    func_80290360();
                                    var_a1 = 1;
                                    break;
                                case 15: /* switch 1 */
                                case 24: /* switch 1 */
                                case 32: /* switch 1 */
                                case 44: /* switch 1 */
                                    func_80290338();
                                    var_a1 = 1;
                                    break;
                                default: /* switch 1 */
                                    D_8018E7AC[arg0] = 5;
                                    break;
                            }
                            if (var_a1 != 0) {
                                gIsGamePaused = 0;
                            }
                        }
                        temp_v0 = find_8018D9E0_entry(0x000000BD);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->cursor) { /* switch 2 */
                                case 11:               /* switch 2 */
                                    D_8015F892 = 1;
                                    D_8015F890 = 0;
                                    func_802903B0();
                                    break;
                                case 12: /* switch 2 */
                                    D_8015F892 = 0;
                                    D_8015F890 = 0;
                                    func_80290388();
                                    break;
                                case 13: /* switch 2 */
                                    D_8015F892 = 0;
                                    D_8015F890 = 0;
                                    func_80290360();
                                    break;
                                case 14: /* switch 2 */
                                    D_8015F892 = 0;
                                    D_8015F890 = 0;
                                    func_80290338();
                                    break;
                                case 15: /* switch 2 */
                                    D_8015F892 = 0;
                                    D_8015F890 = 1;
                                    func_802903B0();
                                    break;
                                default: /* switch 2 */
                                    break;
                            }
                        } else {
                            D_8018E7AC[arg0] = 5;
                        }
                    }
                }
            }
        }
    } else {
        D_8018E7AC[arg0] = 0;
        if (gDebugMenuSelection != 0x40) {
            switch (D_8018EDE0) { /* switch 3 */
                case 0:           /* switch 3 */
                    if (gMenuSelection == 8) {
                        gMenuSelection = 0x0000000A;
                        D_800E86A4 = 2;
                    } else {
                        gMenuSelection++;
                    }
                    break;
                case 1: /* switch 3 */
                    gMenuSelection -= 1;
                    break;
                case 2: /* switch 3 */
                    gDemoMode = 1;
                    D_8018EE08 = 1;
                    gGamestateNext = 4;
                    gCCSelection = (s32) 1;
                    switch (gNextDemoId) { /* switch 4 */
                        case 0:            /* switch 4 */
                            gCurrentCourseId = 0;
                            gScreenModeSelection = 0;
                            gPlayerCountSelection1 = 1;
                            gPlayerCount = 1;
                            gCharacterSelections[0] = 0;
                            gModeSelection = 0;
                            break;
                        case 1: /* switch 4 */
                            gCurrentCourseId = (s16) 1;
                            gScreenModeSelection = (s32) 1;
                            gPlayerCountSelection1 = 2;
                            gPlayerCount = (s8) 2;
                            gCharacterSelections[0] = (s8) 2;
                            gCharacterSelections[1] = (s8) 4;
                            gModeSelection = 2;
                            break;
                        case 2: /* switch 4 */
                            gCurrentCourseId = 0x000B;
                            gScreenModeSelection = 0;
                            gPlayerCountSelection1 = (s32) 1;
                            gPlayerCount = 1;
                            gCharacterSelections[0] = 1;
                            gModeSelection = 0;
                            break;
                        case 3: /* switch 4 */
                            gCurrentCourseId = 0x000E;
                            gScreenModeSelection = 3;
                            gPlayerCountSelection1 = 3;
                            gPlayerCount = (s8) 3;
                            gCharacterSelections[0] = 5;
                            gCharacterSelections[1] = 2;
                            gCharacterSelections[2] = 7;
                            gModeSelection = (s32) 2;
                            break;
                        case 4: /* switch 4 */
                            gCurrentCourseId = 2;
                            gScreenModeSelection = 0;
                            gPlayerCountSelection1 = (s32) 1;
                            gPlayerCount = 1;
                            gCharacterSelections[0] = 7;
                            gModeSelection = 0;
                            break;
                        case 5: /* switch 4 */
                            gCurrentCourseId = 0x000C;
                            gScreenModeSelection = 3;
                            gPlayerCountSelection1 = 4;
                            gPlayerCount = 4;
                            gCharacterSelections[0] = 0;
                            gCharacterSelections[1] = 1;
                            gCharacterSelections[2] = 6;
                            gCharacterSelections[3] = 3;
                            gModeSelection = 2;
                            break;
                        default:
                            break;
                    }
                    gNextDemoId += 1;
                    if (gNextDemoId >= 6) {
                        gNextDemoId = 0;
                    }
                    gCupSelection = gCupSelectionByCourseId[gCurrentCourseId];
                    D_800DC540 = (s32) gCupSelection;
                    gCupCourseSelection = (s8) gPerCupIndexByCourseId[gCurrentCourseId];
                    break;
                case 3: /* switch 3 */
                    switch (gMenuSelection) {
                        case 11:
                            gMenuSelection = 6;
                            break;
                        case 6:
                            gMenuSelection = 11;
                            break;
                    }
                    break;
                case 4: /* switch 3 */
                    switch (gMenuSelection) {
                        case 11:
                            gMenuSelection = 5;
                            break;
                        case 5:
                            gMenuSelection = 11;
                            break;
                    }
                    break;
            }
            if (D_800E86A4 == 0) {
                D_800E86A4 = 1;
            }
            if (gMenuSelection >= 0xE) {
                gGamestateNext = 4;
                if (gModeSelection == 1) {
                    D_8018EDFB = (s8) 1;
                }
                func_8009CE1C();
            }
            D_8018EE0C = 0;
        } else {
            switch (gDebugGotoScene) { /* switch 5; irregular */
                case 1:                /* switch 5 */
                    gGamestateNext = (s32) 5;
                    break;
                case 2: /* switch 5 */
                case 3: /* switch 5 */
                    gGamestateNext = 9;
                    gCreditsCourseId = 8;
                    break;
                default: /* switch 5 */
                    gGamestateNext = 4;
                    if (gModeSelection == (s32) 1) {
                        D_8018EDFB = 1;
                    }
                    break;
            }
            func_8000F124();
            if (gScreenModeSelection == 3) {
                switch (gModeSelection) {
                    case 0:
                    case 1:
                        gModeSelection = 2;
                        break;
                }
            }
            switch (gCurrentCourseId) {
                case COURSE_BLOCK_FORT:
                case COURSE_SKYSCRAPER:
                case COURSE_DOUBLE_DECK:
                case COURSE_BIG_DONUT:
                    gModeSelection = 3;
                    if (gPlayerCountSelection1 == 1) {
                        gPlayerCount = 2;
                        gScreenModeSelection = 1;
                        gPlayerCountSelection1 = gPlayerCount;
                    }
                    break;
                default:
                    if (gModeSelection == 3) {
                        gModeSelection = 0;
                    }
                    if ((gModeSelection == 2) && (gPlayerCountSelection1 == 1)) {
                        gModeSelection = 0;
                    }
            }
            gCupSelection = gCupSelectionByCourseId[gCurrentCourseId];
            D_800DC540 = gCupSelection;
            gCupCourseSelection = gPerCupIndexByCourseId[gCurrentCourseId];
            switch (gDebugGotoScene) { /* switch 6; irregular */
                case 1:                /* switch 6 */
                    break;
                case 2: /* switch 6 */
                    gCCSelection = 0;
                    break;
                case 3: /* switch 6 */
                    gCCSelection = 3;
                    break;
                default: /* switch 6 */
                    if (gCCSelection == 3) {
                        gIsMirrorMode = 1;
                    } else {
                        gIsMirrorMode = 0;
                    }
                    break;
            }
        }
    }
}

void func_8009D77C(s32 arg0, s32 arg1, s32 arg2) {
    s16 var_ra;
    s16 var_t3;
    s16 var_t4;
    s32 temp_t8;
    s32 temp_v1;
    s32 var_t2;
    s32 someMath0;
    s32 someMath1;
    RGBA16* temp_v0_2;
    s32 sp44;
    UNUSED s32 stackPadding0;

    if ((gModeSelection == 0) || (gModeSelection == 1)) {
        var_t3 = D_8018E7E8[arg0].x;
        var_t4 = D_8018E7E8[arg0].y;
        var_ra = D_8018E810[arg0].x;
        sp44 = D_8018E810[arg0].y;
    } else if (arg0 >= 4) {
        var_t3 = D_8018E7E8[arg0].x;
        var_t4 = D_8018E7E8[arg0].y;
        var_ra = D_8018E810[arg0].x;
        sp44 = D_8018E810[arg0].y;
    } else {
        var_t3 = D_8015F480[arg0].screenStartX;
        var_t4 = D_8015F480[arg0].screenStartY;
        var_ra = D_8015F480[arg0].screenWidth;
        sp44 = D_8015F480[arg0].screenHeight;
    }
    var_t2 = (D_8018E7D0[arg0] * 0xFF) / D_8018E7B8[arg0];
    if (var_t2 >= 0x100) {
        var_t2 = 0x000000FF;
    }
    temp_v1 = var_ra / 2;
    temp_t8 = sp44 / 2;
    temp_v0_2 = &D_800E7AE8[arg2];
    // Why does it have to written like this to match?
    someMath0 = temp_v1;
    someMath0 += var_t3;
    someMath1 = temp_t8;
    someMath1 += var_t4;
    gDisplayListHead = draw_box(gDisplayListHead, var_t3 - temp_v1, var_t4 - temp_t8, someMath0, someMath1,
                                temp_v0_2->red, temp_v0_2->green, temp_v0_2->blue, var_t2);
    if (arg1 == 0) {
        D_8018E7D0[arg0]++;
        if ((D_8018E7B8[arg0] + 1) < D_8018E7D0[arg0]) {
            func_8009CE64(arg0);
        }
    }
}

void func_8009D958(s32 arg0, s32 arg1) {
    func_8009D77C(arg0, arg1, 0);
}

void func_8009D978(s32 arg0, s32 arg1) {
    func_8009D77C(arg0, arg1, 1);
}

void func_8009D998(s32 arg0) {
    s16 var_t0;
    s16 var_t1;
    s16 var_t2;
    s16 var_t3;
    s32 temp_v0;
    s32 temp_v1;
    s32 someMath0;
    s32 someMath1;

    if ((gModeSelection == 0) || (gModeSelection == 1)) {
        var_t0 = D_8018E7E8[arg0].x;
        var_t1 = D_8018E7E8[arg0].y;
        var_t2 = D_8018E810[arg0].x;
        var_t3 = D_8018E810[arg0].y;
    } else if (arg0 >= 4) {
        var_t0 = D_8018E7E8[arg0].x;
        var_t1 = D_8018E7E8[arg0].y;
        var_t2 = D_8018E810[arg0].x;
        var_t3 = D_8018E810[arg0].y;
    } else {
        var_t0 = D_8015F480[arg0].screenStartX;
        var_t1 = D_8015F480[arg0].screenStartY;
        var_t2 = D_8015F480[arg0].screenWidth;
        var_t3 = D_8015F480[arg0].screenHeight;
    }
    temp_v0 = var_t2 / 2;
    temp_v1 = var_t3 / 2;
    // Why does it have to written like this to match?
    someMath0 = temp_v0;
    someMath0 += var_t0;
    someMath1 = temp_v1;
    someMath1 += var_t1;
    gDisplayListHead =
        draw_box(gDisplayListHead, var_t0 - temp_v0, var_t1 - temp_v1, someMath0, someMath1, 0, 0, 0, 0x000000FF);
}

void func_8009DAA8(void) {
    u32 var_t0;

    D_8018E7D0[4]++;
    if (D_8018E7D0[4] >= (D_8018E7B8[4] + 1)) {
        func_8009CE64(4);
    }
    gDPPipeSync(gDisplayListHead++);
    var_t0 = (D_8018E7D0[4] * 255) / D_8018E7B8[4];
    if ((s32) var_t0 >= 0x100) {
        var_t0 = 0x000000FF;
    }
    gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, (s32) var_t0);
}

void func_8009DB8C(void) {
    s32 temp_t4;
    s32 temp_t5;
    s32 var_s0;
    u32 var_s3;
    s32 var_v1;

    D_8018E7D0[4]++;
    // why?
    var_v1 = D_8018E7D0[4];
    if ((u32) var_v1 >= D_8018E7B8[4]) {
        if ((u32) var_v1 == D_8018E7B8[4]) {
            for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
                D_8018D9B8[var_s0] = 1;
            }
        } else {
            func_8009CE64(4);
        }
    } else {
        var_s0 = 0;
        var_s3 = 0;
        while (var_s3 < (0x4B0U / D_8018E7B8[4])) {
            if ((D_8018D9B8[var_s0] == 0) && (random_int((0x4B0U - D_8018E7D0[4]) / D_8018E7B8[4]) == 0)) {
                var_s3 += 1;
                D_8018D9B8[var_s0] = 1;
            }
            var_s0 += 1;
            if (var_s0 >= 0x4B0) {
                var_s0 = 0;
            }
        }
    }
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
        if (D_8018D9B8[var_s0] != 0) {
            temp_t4 = (var_s0 % 40) * 8;
            temp_t5 = (var_s0 / 40) * 8;
            gDPFillRectangle(gDisplayListHead++, temp_t4, temp_t5, temp_t4 + 8, temp_t5 + 8);
        }
    }
    gDPPipeSync(gDisplayListHead++);
    var_v1 = (D_8018E7D0[4] * 255) / D_8018E7B8[4];
    if (var_v1 >= 0x100) {
        var_v1 = 0x000000FF;
    }
    gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, var_v1);
}

void func_8009DEF8(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((D_8018E7AC[4] != 1) && (D_8018E7AC[4] != 6)) {
        D_8018E7AC[4] = arg1;
        D_8018E7B8[4] = arg0;
        if (D_8018E7B8[4] >= 0x100U) {
            D_8018E7B8[4] = 0xFFU;
        }
        D_8018E7D0[4] = 0;
    }
}

void func_8009DF4C(s32 arg0) {
    func_8009DEF8(arg0, 1);
}

void func_8009DF6C(s32 arg0) {
    func_8009DEF8(arg0, 8);
}

void func_8009DF8C(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((D_8018E7AC[4] != 2) && (D_8018E7AC[4] != 5)) {
        D_8018E7AC[4] = arg1;
        D_8018E7B8[4] = arg0;
        if (D_8018E7B8[4] >= 0x100U) {
            D_8018E7B8[4] = 0xFFU;
        }
        D_8018E7D0[4] = 0;
    }
}

void func_8009DFE0(s32 arg0) {
    func_8009DF8C(arg0, 2);
}

void func_8009E000(s32 arg0) {
    func_8009DF8C(arg0, 7);
}

void func_8009E020(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = D_8018E7AC[arg0];
    if ((temp != 1) && (temp != 6)) {
        D_8018E7AC[arg0] = 1;
        D_8018E7B8[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            D_8018E7B8[arg0] = 0xFF;
        }
        D_8018E7D0[arg0] = 0;
    }
}

void func_8009E088(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = D_8018E7AC[arg0];
    if ((temp != 2) && (temp != 5)) {
        D_8018E7AC[arg0] = 2;
        D_8018E7B8[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            D_8018E7B8[arg0] = 0xFF;
        }
        D_8018E7D0[arg0] = 0;
    }
}

void func_8009E0F0(s32 arg0) {
    s32 var_v0;

    if (D_8018E7AC[4] != 3) {
        D_8018E7AC[4] = 3;
        D_8018E7B8[4] = arg0;
        if (D_8018E7B8[4] >= 0x100U) {
            D_8018E7B8[4] = 0x000000FF;
        }
        D_8018E7D0[4] = 0;
        for (var_v0 = 0; var_v0 < 0x4B0; var_v0++) {
            D_8018D9B8[var_v0] = 0;
        }
    }
}

void func_8009E17C(u32 arg0) {
    if (D_8018E7AC[4] != 4) {
        D_8018E7AC[4] = 4;
        D_8018E7B8[4] = arg0;
        if (D_8018E7B8[4] >= 0x100U) {
            D_8018E7B8[4] = 0x000000FFU;
        }
        D_8018E7D0[4] = 0;
    }
}

void func_8009E1C0(void) {
    func_8009DFE0(10);
    D_8018EDE0 = 0;
}

void func_8009E1E4(void) {
    func_8009E000(10);
    D_8018EDE0 = 0;
}

void func_8009E208(void) {
    func_8009DFE0(10);
    D_8018EDE0 = 1;
}

void func_8009E230(void) {
    func_8009DFE0(10);
    D_8018EDE0 = 2;
}

void func_8009E258(void) {
    func_8009DFE0(10);
    D_8018EDE0 = 3;
}

void func_8009E280(void) {
    func_8009DFE0(10);
    D_8018EDE0 = 4;
}

void func_8009E2A8(s32 arg0) {
    switch (D_8018E838[arg0]) {
        case 0:
            break;
        case 1:
            func_8009E2F0(arg0);
            break;
        default:
            D_8018E838[arg0] = 0;
            break;
    }
}

void func_8009E2F0(s32 arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 someIndex;
    s32 temp_t7;
    f32 temp_t7_2;
    RGBA16* temp_v0;
    struct UnkStruct_8018E7E8* temp_t0;
    struct UnkStruct_8018E7E8* temp_t1;

    temp_t7 = D_800F0B28[D_8018E840[arg0]];
    if (temp_t7 != 0) {
        temp_t1 = &D_8018E7E8[arg0];
        temp_t0 = &D_8018E810[arg0];
        temp_v0 = &D_800E7AC8[temp_t7];
        if ((u32) D_8018E840[arg0] < 0x1BU) {
            gDisplayListHead = draw_box(gDisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->green, temp_v0->blue, temp_v0->alpha);
        } else {
            temp_t7_2 = ((u32) (38 - D_8018E840[arg0])) / 11.0;
            gDisplayListHead = draw_box(gDisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->green, temp_v0->blue, (u32) (temp_v0->alpha * temp_t7_2));
        }
    }
    D_8018E840[arg0]++;
    if ((u32) D_8018E840[arg0] >= 0x26U) {
        for (someIndex = 0; someIndex < 4; someIndex++) {
            D_8018E838[someIndex] = 0;
        }
    }
}

void func_8009E5BC(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        func_8009E5FC(i);
    }
}

void func_8009E5FC(s32 arg0) {
    D_8018E838[arg0] = 1;
    D_8018E840[arg0] = 0;
}

void func_8009E620(void) {
    s32 index;
    for (index = 0; index < D_8018D9E0_SIZE; index++) {
        D_8018D9E0[index].type = 0;
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/1BHpa
// Stack differences, can't figure out how to fix them
void add_8018D9E0_entry(s32 type, s32 column, s32 row, s8 priority) {
    struct_8018D9E0_entry* var_ra;
    s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    s32 temp_v0_6;
    s32 var_v0;
    s32 var_v1_3;
    s32 temp_a1;
    UNUSED MkTexture* mk64Texture;
    MkAnimation* var_a0;
    s32 one = 1;

    var_v0 = 0;
    var_ra = D_8018D9E0;
    // ????????
    // Credit to Vetri for the idea to mess around with this loop
    // to fix the issue near the 0xD4 case
    while (1) {
        var_v0++;
        if (var_ra->type == 0)
            break;

        if (var_v0 > 0x20) {
            while (1) {}
        }
        var_ra++;
    }
    var_ra->type = type;
    var_ra->cursor = 0;
    var_ra->unk8 = 0;
    var_ra->column = column;
    var_ra->row = row;
    var_ra->priority = (u8) priority;
    var_ra->visible = one;
    var_ra->unk1C = 0;
    var_ra->unk20 = 0;
    switch (type) {
        case 0xFA:
            s8018ED94 = 0;
            D_800E8530 = 0.0f;
            D_800E8534 = 3.0f;
            D_8018EDC0 = 0x000009C4;
            D_8018EDC8 = 0;
            D_8018EDCC = -270.0f;
            D_8018EDD0 = 0;
            D_8018EDD4 = 0;
            D_8018EDD8 = 0;
            D_8018EDDC = 0;
            D_8018EDC4 = 3;
            var_ra->unk1C = -1;
            var_ra->unk20 = one;
            break;
        case 0xFB:
            D_8018EDC0 = 0x00000708;
            D_8018EDC8 = -51.0f;
            D_8018EDCC = -12.0f;
            D_8018EDD0 = -18.0f;
            D_8018EDD4 = -270.0f;
            D_8018EDD8 = 750.0f;
            D_8018EDDC = 0;
            D_8018EDC4 = 1.0f;
            var_ra->unk1C = -1;
            var_ra->unk20 = one;
            break;
        case 0xD2:
            func_800996BC(D_020014C8, 0);
            func_8009B954(D_020014C8);
            D_8018E75C = func_8009BA74(D_8018E75C, D_020014C8, var_ra->column, var_ra->row);
            func_8009B998();
            break;
        case 0xD3:
            func_800996BC(D_02001540, 0);
            func_8009B954(D_02001540);
            D_8018E75C = func_8009BA74(D_8018E75C, D_02001540, var_ra->column, var_ra->row);
            func_8009B998();
            break;
        case 0xD4:
            func_800996BC(D_0200157C, 0);
            func_80099184(D_02001874);
            var_ra->row = 0x00000069;
            for (var_v0 = 0; var_v0 < 133; var_v0++) {
                func_80099184(segmented_to_virtual_dupe(D_800E7AF8[var_v0]));
            }
            break;
        case 0xD5:
            func_80099184(D_020015A4);
            func_8009B954(D_020015A4);
            D_8018E75C = func_8009BA74(D_8018E75C, D_020015A4, var_ra->column, var_ra->row);
            gDPLoadTextureBlock(D_8018E75C++, func_8009B8C4(gTexture7ED50C), G_IM_FMT_IA, G_IM_SIZ_16b, 256, 5, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(D_8018E75C++, 0x80, 0x2C0, 0x480, 0x2D4, G_TX_RENDERTILE, 0, 0x80, 0x0400, 0xFC00);
            func_8009B998();
            func_80099184(D_020015CC);
            func_8009B954(D_020015CC);
            D_8018E75C = func_8009BA74(D_8018E75C, D_020015CC, var_ra->column, var_ra->row);
            func_8009B998();
            func_80099184(D_02001630);
            func_8009B954(D_02001630);
            D_8018E75C = func_8009BA74(D_8018E75C, D_02001630, var_ra->column, var_ra->row);
            func_8009B998();
            func_80099184(D_02001658);
            func_8009B954(D_02001658);
            D_8018E75C = func_8009BA74(D_8018E75C, D_02001658, var_ra->column, var_ra->row);
            func_8009B998();
            break;
        case 0xD6:
            var_ra->D_8018DEE0_index = func_8009A374(segmented_to_virtual_dupe_2(D_800E7D34[0]));
            break;
        case 0xD7:
            for (var_v0 = 0; var_v0 < 10; var_v0++) {
                func_80099184(segmented_to_virtual_dupe(D_800E7D0C[var_v0]));
            }
            break;
        case 0xD8:
        case 0xD9:
            func_80099184(D_0200184C);
            break;
        case 0x1:
            func_800996BC(D_800E7D4C[func_800B555C()], 0);
            break;
        case 0x2:
            func_8006EE44();
            gD_8018E118TotalSize += 0x10000;
            func_80099184(D_020045E8);
            break;
        case 0x3:
            func_80099184(D_02004610);
            break;
        case 0x23:
        case 0x24:
        case 0x25:
            func_800996BC(D_800E7D4C[func_800B555C()], 0);
            func_800996BC(D_02004B74, 0);
            func_8009B0A4(0, 0x00000019);
            func_8009B538(0, SCREEN_WIDTH * SCREEN_HEIGHT, D_800E74E8[type - 0x23].red, D_800E74E8[type - 0x23].green,
                          D_800E74E8[type - 0x23].blue);
            break;
        case 0xF:
            var_ra->unk1C = 0x00000020;
            /* fallthrough */
        case 0x0A:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
            func_800996BC(segmented_to_virtual_dupe(D_800E8274[type - 0x12]), 0);
            break;
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
            func_800996BC(segmented_to_virtual_dupe(D_800E8234[((type - 0xB) * 2) + 0]), 0);
            func_80099184(segmented_to_virtual_dupe(D_800E8234[((type - 0xB) * 2) + 1]));
            break;
        case 0x2A:
            func_800996BC(D_02004B4C, 0);
            break;
        case 0x33:
            func_800996BC(D_02004B74, 0);
            var_ra->unk1C = 0x00000020;
            break;
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
            func_80099184(segmented_to_virtual_dupe(D_800E82B4[type - 0x34]));
            break;
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
            var_ra->D_8018DEE0_index = func_8009A374(segmented_to_virtual_dupe_2(D_800E8320[type - 0x2B]));
            func_800996BC(segmented_to_virtual_dupe(D_800E7D54[type - 0x2B]), 0);
            break;
        case 0xA0:
        case 0xA1:
            var_ra->D_8018DEE0_index = func_8009A374(segmented_to_virtual_dupe_2(D_800E8320[type - 0xA0]));
            break;
        case 0x5D:
            var_ra->unk1C = 0x00000020;
            /* fallthrough */
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
            func_800996BC(segmented_to_virtual_dupe(D_800E82C4[type - 0x52]), 0);
            break;
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
            var_ra->D_8018DEE0_index =
                func_8009A374(segmented_to_virtual_dupe_2(D_800E7E34[gCupCourseOrder[0][var_ra->type - 0x5F]]));
            break;
        case 0x5E:
            var_ra->unk20 = random_int(4U) + 2;
            break;
        case 0x65:
        case 0x66:
            var_ra->column = D_800E7248[type - 0x65].column;
            var_ra->row = D_800E7248[type - 0x65].row;
            break;
        case 0x67:
            var_ra->unk1C = (s32) gCupSelection;
            var_ra->unk20 = func_800B54C0(gCupSelection, gCCSelection);
            var_ra->D_8018DEE0_index =
                func_8009A374(segmented_to_virtual_dupe_2(D_800E7E20[((gCCSelection / 2) * 4) - var_ra->unk20]));
            var_ra->column = D_800E7268[0].column;
            var_ra->row = D_800E7268[0].row;
            break;
        case 0x68:
            func_800996BC(segmented_to_virtual_dupe(D_800E8294[gCCSelection]), 0);
            var_ra->column = 0x00000037;
            var_ra->row = 0x000000C3;
            break;
        case 0x69:
            func_800996BC(segmented_to_virtual_dupe(D_02004A0C), 0);
            if (controller_pak_1_status() == 0) {
                func_800B6708();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
            func_800996BC(segmented_to_virtual_dupe(D_800E82F4[type - 0x78]), 0);
            break;
        case 0x8C:
            func_800996BC(segmented_to_virtual_dupe(D_02004A34), 0);
            if (controller_pak_1_status() == 0) {
                func_800B6708();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case 0x8D:
            func_80099184(segmented_to_virtual_dupe(D_02001FA4));
            break;
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
            temp_v0_6 = var_ra->type - 0x7C;
            func_800996BC(segmented_to_virtual_dupe(D_800E7D74[gCupCourseOrder[temp_v0_6 / 4][temp_v0_6 % 4]]), -1);
            temp_v0_6 = var_ra->type - 0x7C;
            func_800996BC(segmented_to_virtual_dupe(D_800E7DC4[gCupCourseOrder[temp_v0_6 / 4][temp_v0_6 % 4]]), 0);
            func_800996BC(segmented_to_virtual_dupe(D_02004A0C), 0);
            break;
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
            temp_a1 = D_800EFD64[gCharacterSelections[type - 0xB1]];
            var_v1_3 = 0;
            stackPadding0 = type - 0xB1;
            switch (gModeSelection) {
                case 2:
                    if (gGPCurrentRaceRankByPlayerId[type - 0xB1] != 0) {
                        var_v1_3 = 1;
                    }
                    break;
                case 3:
                    if ((type - 0xB1) != gPlayerWinningIndex) {
                        var_v1_3 = 1;
                    }
                    break;
                default:
                    break;
            }
            if (var_v1_3 != 0) {
                var_a0 = gCharacterDefeatAnimation[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            var_ra->D_8018DEE0_index = func_8009A478(segmented_to_virtual_dupe_2(var_a0), stackPadding0);
            func_800996BC(segmented_to_virtual_dupe(D_800E7D54[temp_a1]), 0);
            func_80099184(segmented_to_virtual_dupe(D_800E82B4[type - 0xB1]));
            break;
        case 0xBB:
            var_ra->unk1C = func_800B5020(playerHUD[0].someTimer, gCharacterSelections[0]);
            var_ra->unk20 = func_800B5218();
            if (D_80162DD4 != 1) {
                if (func_800051C4() >= 0x3C01) {
                    D_80162DD4 = 1;
                }
            }
            if ((var_ra->unk1C == 0) || (var_ra->unk20 != 0)) {
                func_800B559C((gCupSelection * 4) + gCupCourseSelection);
            }
            break;
        case 0xE6:
            var_ra->D_8018DEE0_index = func_8009A374(segmented_to_virtual_dupe_2(
                D_800E7E34[gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]]));
            var_ra->unk1C = gTimeTrialDataCourseIndex;
            func_800996BC(segmented_to_virtual_dupe(D_02004A0C), 0);
            func_8006EF60();
            if (controller_pak_1_status() == 0) {
                func_800B6708();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case 0xF0:
            var_ra->cursor = (s32) gSoundMode;
            break;
        case 0xF1:
            func_800996BC(segmented_to_virtual_dupe(D_02004638), 0);
            break;
        case 0xBE:
            D_8018ED90 = 0;
            break;
        case 0x130:
            temp_a1 = D_800EFD64[D_802874D8.unk1E];
            if (D_802874D8.unk1D >= 3) {
                var_v0 = 1;
            } else {
                var_v0 = 0;
            }
            if (var_v0 != 0) {
                var_a0 = gCharacterDefeatAnimation[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            var_ra->D_8018DEE0_index = func_8009A478(segmented_to_virtual_dupe_2(var_a0), 0);
            func_800996BC(segmented_to_virtual_dupe(D_800E7D54[temp_a1]), 0);
            break;
        case 0x190:
        case 0x191:
        case 0x192:
        case 0x193:
        case 0x194:
        case 0x195:
        case 0x196:
        case 0x197:
        case 0x198:
        case 0x199:
        case 0x19A:
        case 0x19B:
        case 0x19C:
        case 0x19D:
        case 0x19E:
        case 0x19F:
        case 0x1A0:
        case 0x1A1:
        case 0x1A2:
        case 0x1A3:
        case 0x1A4:
        case 0x1A5:
        case 0x1A6:
        case 0x1A7:
        case 0x1A8:
        case 0x1A9:
        case 0x1AA:
        case 0x1AB:
        case 0x1AC:
        case 0x1AD:
        case 0x1AE:
        case 0x1AF:
        case 0x1B0:
        case 0x1B1:
        case 0x1B2:
        case 0x1B3:
        case 0x1B4:
        case 0x1B5:
        case 0x1B6:
        case 0x1B7:
        case 0x1B8:
        case 0x1B9:
        case 0x1BA:
        case 0x1BB:
        case 0x1BC:
        case 0x1BD:
        case 0x1BE:
        case 0x1BF:
        case 0x1C0:
        case 0x1C1:
        case 0x1C2:
        case 0x1C3:
        case 0x1C4:
        case 0x1C5:
        case 0x1C6:
        case 0x1C7:
        case 0x1C8:
        case 0x1C9:
        case 0x1CA:
        case 0x1CB:
        case 0x1CC:
        case 0x1CD:
        case 0x1CE:
        default:
            break;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/add_8018D9E0_entry.s")
#endif

#ifdef NON_MATCHING
// https://decomp.me/scratch/MatRp
// Biggest diff left is in the case 0x12 though 0x19 handling. Not really sure what's going on there
// There's also a diff in the handling of D_800E77A0 in case 0x4. Not sure what's going on there either
void func_8009F5E0(struct_8018D9E0_entry* arg0) {
    s32 var_a1;
    s32 var_v1;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    MkTexture* sp9C;
    UNUSED s32 stackPadding2;
    s32 temp_a0;
    s32 temp_t2;
    s32 temp_t5;
    s32 temp_t9;
    s32 temp_v1;
    char sp80[3];
    s32 var_t0;
    UNUSED s32 var_a2;
    f32 why = 0.75f;
    s32 one = 1;
    f32 floatone = 1;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    f32 var_f0;

    if ((s8) arg0->visible != 0) {
        gDPPipeSync(gDisplayListHead++);
        switch (arg0->type) { /* switch 6; irregular */
            case 0xFA:        /* switch 6 */
                func_80094660(gGfxPool, arg0->unk1C);
                break;
            case 0xFB: /* switch 6 */
                func_800947B4(gGfxPool, arg0->unk1C);
                break;
            case 0xD2: /* switch 6 */
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020014C8);
                break;
            case 0xD3: /* switch 6 */
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001540);
                break;
            case 0xD4: /* switch 6 */
                func_800A09E0(arg0);
                func_800A0AD0(arg0);
                func_800A0B80(arg0);
                break;
            case 0xD5: /* switch 6 */
                       // Sets the text color of the text on the Controller Pak menu
                       // Also sets the color of the shading at the top and bottom of the menu
                       // Does not set color of the text in the table itself
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020015A4);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x00, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020015CC);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x32, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001630);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x32, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001658);
                break;
            case 0xD6: /* switch 6 */
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, -1);
                break;
            case 0xD7: /* switch 6 */
                func_800A0DFC();
                break;
            case 0xD8: /* switch 6 */
            case 0xD9: /* switch 6 */
                func_800A0EB8(arg0, arg0->type - 0xD8);
                break;
            case 0x1: /* switch 6 */
                gDisplayListHead =
                    func_8009BA74(gDisplayListHead, D_800E7D4C[func_800B555C()], arg0->column, arg0->row);
                break;
            case 0x2: /* switch 6 */
                func_8004C8D4((arg0->column + 0xA0), (arg0->row + 0x47));
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_020045E8, arg0->column, arg0->row);
                break;
            case 0x3: /* switch 6 */
                if (((gGlobalTimer / 8) % 3) != 0) {
                    gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004610, arg0->column, arg0->row);
                }
                break;
            case 0x5: /* switch 6 */
                var_t0 = (s32) ((f32) (get_string_width(gCourseNamesDup[0]) + 5) * 0.9f) / 2;
                gDisplayListHead = draw_box(gDisplayListHead, 0xA0 - var_t0, 0x0000007B, var_t0 + 0xA0, 0x000000A4, 0,
                                            0, 0, 0x00000096);
                set_text_color(1);
                draw_text(0x0000009B, 0x0000008C, gCourseNamesDup[0], 0, 0.9f, 0.9f);
                temp_v1 = func_800B4EB4(0, 7) & 0xFFFFF;
                if (temp_v1 < 0x1EAA) {
                    set_text_color((s32) gGlobalTimer % 2);
                } else if (temp_v1 < 0x2329) {
                    set_text_color((s32) gGlobalTimer % 3);
                } else {
                    set_text_color(3);
                }
                get_time_record_minutes(temp_v1, sp80);
                func_800939C8(0x00000077, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                func_80093324(0x0000008B, 0x000000A0, "'", 0, 1.0f, 1.0f);
                get_time_record_seconds(temp_v1, sp80);
                func_800939C8(0x00000094, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                func_80093324(0x000000A7, 0x000000A0, "\"", 0, 1.0f, 1.0f);
                get_time_record_centiseconds(temp_v1, sp80);
                func_800939C8(0x000000B4, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                break;
            case 0x4: /* switch 6 */
                var_t0 = get_string_width(D_800E77A0[0]);
                temp_v1 = get_string_width(D_800E77A0[1]);
                if (var_t0 < temp_v1) {
                    var_t0 = temp_v1;
                }
                temp_t2 = (s32) (var_t0 * why) / 2;
                temp_t5 = (s32) (((why * 2) + 0.5) * 16.0) / 2;
                gDisplayListHead = draw_box(gDisplayListHead, 0xA0 - temp_t2, 0xB6 - temp_t5, temp_t2 + 0xA0,
                                            temp_t5 + 0xB6, 0, 0, 0, 0x00000096);
                set_text_color(4);
                for (temp_t9 = 0; temp_t9 < 2; temp_t9++) {
                    draw_text(0xA0 * one - floatone * why, (s32) (0xB4 * one + ((f32) (temp_t9 * 0x12) * why)),
                              D_800E77A0[temp_t9], 0, why, why);
                }
                break;
            case 0x23: /* switch 6 */
            case 0x24: /* switch 6 */
            case 0x25: /* switch 6 */
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_800E7D4C[func_800B555C()], arg0->column, arg0->row, 3, 0);
                break;
            case 0xA: /* switch 6 */
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004660, arg0->column, arg0->row);
                break;
            case 0xB: /* switch 6 */
            case 0xC: /* switch 6 */
            case 0xD: /* switch 6 */
            case 0xE: /* switch 6 */
                var_a1 = arg0->type - 0xB;
                func_800A8270(var_a1, arg0);
                func_800A0FA4(arg0, var_a1);
                break;
            case 0xF: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead = func_8009BC9C(gDisplayListHead, D_0200487C, arg0->column, arg0->row, 2, arg0->unk1C);
                break;
            case 0x10: /* switch 6 */
            case 0x11: /* switch 6 */
                var_a1 = arg0->type - 0xF;
                if (arg0->unk1C < 0x20) {
                    temp_t9 = (arg0->unk1C * 0x3A) / 64;
                    if (var_a1 == gMainMenuSelectionDepth) {
                        gDisplayListHead =
                            draw_flash_select_case_fast(gDisplayListHead, arg0->column + temp_t9, arg0->row,
                                                        (arg0->column - temp_t9) + 0x39, arg0->row + 0x12);
                    } else {
                        gDisplayListHead =
                            draw_box_fill(gDisplayListHead, arg0->column + temp_t9, arg0->row,
                                          (arg0->column - temp_t9) + 0x39, arg0->row + 0x12, 1, 1, 1, 0x000000FF);
                    }
                }
                var_v1 = arg0->type - 0xA;
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_800E8254[var_v1], arg0->column, arg0->row, 2, arg0->unk1C);
                break;
            case 0x12: /* switch 6 */
            case 0x13: /* switch 6 */
            case 0x14: /* switch 6 */
            case 0x15: /* switch 6 */
            case 0x16: /* switch 6 */
            case 0x17: /* switch 6 */
            case 0x18: /* switch 6 */
            case 0x19: /* switch 6 */
                var_v1 = D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]];
                var_a1 = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
                switch (arg0->type) { /* switch 5 */
                    case 0x12:        /* switch 5 */
                    case 0x13:        /* switch 5 */
                    case 0x14:        /* switch 5 */
                    case 0x15:        /* switch 5 */
                        if ((var_a1 != 0) && (var_a1 != 2)) {
                            var_v1 = -1;
                        }
                        var_a1 = 18;
                        sp9C = segmented_to_virtual_dupe(D_800E824C[arg0->type]);
                        break;
                    case 0x16: /* switch 5 */
                    case 0x17: /* switch 5 */
                        if (var_a1 != 2) {
                            var_v1 = -1;
                        } else {
                            var_a1 = 22;
                            sp9C = segmented_to_virtual_dupe(D_800E824C[arg0->type]);
                        }
                        break;
                    case 0x18: /* switch 5 */
                    case 0x19: /* switch 5 */
                        if (var_a1 != 1) {
                            var_v1 = -1;
                        } else {
                            var_a1 = 24;
                            sp9C = segmented_to_virtual_dupe(D_800E824C[arg0->type]);
                        }
                        break;
                }
                if (var_v1 != -1) {
                    if ((gMainMenuSelectionDepth >= 5) && (var_v1 == (arg0->type - var_a1))) {
                        if (gMainMenuSelectionDepth >= 6) {
                            gDisplayListHead =
                                draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                              arg0->row + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                        } else {
                            gDisplayListHead =
                                draw_flash_select_case_slow(gDisplayListHead, arg0->column ^ 0, arg0->row ^ 0,
                                                            arg0->column + 0x3F, arg0->row + 0x11);
                        }
                    } else {
                        gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                         arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                    }
                    gDisplayListHead = func_8009BA74(gDisplayListHead, sp9C, arg0->column, arg0->row);
                }
                break;
            case 0x1B: /* switch 6 */
                func_800A10CC(arg0);
                break;
            case 0x2A: /* switch 6 */
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004B4C, arg0->column, arg0->row);
                break;
            case 0x34: /* switch 6 */
            case 0x35: /* switch 6 */
            case 0x36: /* switch 6 */
            case 0x37: /* switch 6 */
                temp_a0 = arg0->type - 0x34;
                if (gCharacterGridSelections[temp_a0]) {
                    if (D_8018EDE8[temp_a0] == 0) {
                        temp_t2 = 0x000000FF;
                    } else {
                        temp_t2 = gGlobalTimer % 16;
                        if (temp_t2 >= 8) {
                            temp_t2 = (-temp_t2 * 8) + 0x80;
                        } else {
                            temp_t2 *= 8;
                        }
                        temp_t2 += 0xBF;
                    }
                    func_800A11D0(arg0, temp_a0, temp_t2);
                }
                break;
            case 0x33: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead = func_8009BC9C(gDisplayListHead, D_02004B74, arg0->column, arg0->row, 2, arg0->unk1C);
                break;
            case 0x2B: /* switch 6 */
            case 0x2C: /* switch 6 */
            case 0x2D: /* switch 6 */
            case 0x2E: /* switch 6 */
            case 0x2F: /* switch 6 */
            case 0x30: /* switch 6 */
            case 0x31: /* switch 6 */
            case 0x32: /* switch 6 */
                func_800A12BC(arg0, segmented_to_virtual_dupe(D_800E7D54[arg0->type - 0x2B]));
                /* fallthrough */
            case 0xA0: /* switch 6 */
            case 0xA1: /* switch 6 */
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, arg0->unk1C);
                break;
            case 0x58: /* switch 6 */
            case 0x59: /* switch 6 */
            case 0x5A: /* switch 6 */
            case 0x5B: /* switch 6 */
            case 0x5C: /* switch 6 */
                func_800A8A98(arg0);
                gDisplayListHead =
                    func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E82C4[arg0->type - 0x52]),
                                  arg0->column, arg0->row);
                func_800A8CA4(arg0);
                break;
            case 0x52: /* switch 6 */
                gDisplayListHead =
                    func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E82C4[arg0->type - 0x52]),
                                  arg0->column, arg0->row);
                break;
            case 0x5F: /* switch 6 */
            case 0x60: /* switch 6 */
            case 0x61: /* switch 6 */
            case 0x62: /* switch 6 */
                func_800A1500(arg0);
                break;
            case 0x53: /* switch 6 */
            case 0x54: /* switch 6 */
            case 0x55: /* switch 6 */
            case 0x56: /* switch 6 */
                var_a1 = arg0->type - 0x53;
                func_800A890C(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case 0x5D: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead = func_8009BC9C(gDisplayListHead, D_02004E80, arg0->column, arg0->row, 2, arg0->unk1C);
                break;
            case 0x65: /* switch 6 */
            case 0x66: /* switch 6 */
                if (arg0->type == 0x00000065) {
                    var_f0 = 0.6f;
                } else {
                    var_f0 = 0.8f;
                }
                func_800A86E8(arg0);
                set_text_color(3);
                func_80093324(arg0->column + 8, arg0->row + 0x10, D_800E7728[arg0->type - 0x65], 0, var_f0, 0.8f);
                func_800A874C(arg0);
                break;
            case 0x6E: /* switch 6 */
                func_800A8E14(arg0);
                break;
            case 0x67: /* switch 6 */
                func_800A8EC0(arg0);
                break;
            case 0x68: /* switch 6 */
                gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                 arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                gDisplayListHead = func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E8294[gCCSelection]),
                                                 arg0->column, arg0->row);
                break;
            case 0x69: /* switch 6 */
                func_800A8F48(arg0);
                break;
            case 0x78: /* switch 6 */
            case 0x79: /* switch 6 */
            case 0x7A: /* switch 6 */
            case 0x7B: /* switch 6 */
                var_a1 = arg0->type - 0x78;
                func_800A90D4(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case 0x8C: /* switch 6 */
                if ((gMainMenuSelectionDepth >= 5) && (var_a1 == (arg0->type - var_v1))) {
                    if (gMainMenuSelectionDepth >= 6) {
                        gDisplayListHead =
                            draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                          arg0->row + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        gDisplayListHead = draw_flash_select_case_slow(
                            gDisplayListHead, arg0->column ^ 0, arg0->row ^ 0, arg0->column + 0x3F, arg0->row + 0x11);
                    }
                } else {
                    gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                     arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                }
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004A34, arg0->column, arg0->row);
                set_text_color(3);
                func_800936B8(0x00000125, 0x0000001C, D_800E77B4, 0, 0.55f, 0.55f);
                break;
            case 0x8D: /* switch 6 */
                func_800A1780(arg0);
                break;
            case 0x7C: /* switch 6 */
            case 0x7D: /* switch 6 */
            case 0x7E: /* switch 6 */
            case 0x7F: /* switch 6 */
            case 0x80: /* switch 6 */
            case 0x81: /* switch 6 */
            case 0x82: /* switch 6 */
            case 0x83: /* switch 6 */
            case 0x84: /* switch 6 */
            case 0x85: /* switch 6 */
            case 0x86: /* switch 6 */
            case 0x87: /* switch 6 */
            case 0x88: /* switch 6 */
            case 0x89: /* switch 6 */
            case 0x8A: /* switch 6 */
            case 0x8B: /* switch 6 */
                func_800A15EC(arg0);
                break;
            case 0x96: /* switch 6 */
                set_text_color(4);
                func_800936B8(arg0->column, arg0->row, gCupNames[D_800DC540], arg0->unk1C, arg0->unk24, 1.0f);
                break;
            case 0x97: /* switch 6 */
                set_text_color(5);
                func_80093324(arg0->column, arg0->row, gCourseNames[gCurrentCourseId], arg0->unk1C, arg0->unk24, 1.0f);
                break;
            case 0x98: /* switch 6 */
                func_800A2D1C(arg0);
                break;
            case 0x5E: /* switch 6 */
                gDisplayListHead = func_80096CD8(gDisplayListHead, 0x00000019, 0x00000072, 0x0000007CU, 0x0000004AU);
                break;
            case 0xAA: /* switch 6 */
                func_800A2EB8(arg0);
                break;
            case 0xAB: /* switch 6 */
                func_800A34A8(arg0);
                break;
            case 0xAC: /* switch 6 */
                func_800A6154(arg0);
                break;
            case 0xAF: /* switch 6 */
                func_800A6034(arg0);
                break;
            case 0xB0: /* switch 6 */
                func_800A638C(arg0);
                break;
            case 0xB1: /* switch 6 */
            case 0xB2: /* switch 6 */
            case 0xB3: /* switch 6 */
            case 0xB4: /* switch 6 */
                if (arg0->cursor != 0) {
                    var_v1 = arg0->type - 0xB1;
                    gDisplayListHead =
                        func_8009BA74(gDisplayListHead,
                                      segmented_to_virtual_dupe(D_800E7D54[D_800EFD64[gCharacterSelections[var_v1]]]),
                                      arg0->column, arg0->row);
                    func_8009A7EC(arg0->D_8018DEE0_index, arg0->column, arg0->row, var_v1, arg0->unk1C);
                    func_800A11D0(arg0, var_v1, 0x000000FF);
                }
                break;
            case 0xB9: /* switch 6 */
                func_800A3C84(arg0);
                break;
            case 0xBA: /* switch 6 */
                func_800A3E60(arg0);
                break;
            case 0xBC: /* switch 6 */
                func_800A4A24(arg0);
                break;
            case 0xC7: /* switch 6 */
                render_pause_menu(arg0);
                break;
            case 0xBD: /* switch 6 */
                func_800A5738(arg0);
                break;
            case 0xE6: /* switch 6 */
                func_800A1924(arg0);
                break;
            case 0xE7: /* switch 6 */
                func_800A1A20(arg0);
                break;
            case 0xE8: /* switch 6 */
                func_800A1BE0(arg0);
                break;
            case 0xE9: /* switch 6 */
                func_800A1DE0(arg0);
                break;
            case 0xEA: /* switch 6 */
                func_800A1F30(arg0);
                break;
            case 0xF0: /* switch 6 */
                func_800A1FB0(arg0);
                break;
            case 0xF1: /* switch 6 */
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004638, arg0->column, arg0->row);
                break;
            case 0x10E: /* switch 6 */
                func_800A70E8(arg0);
                break;
            case 0x12B: /* switch 6 */
                func_800A7258(arg0);
                break;
            case 0x12C: /* switch 6 */
                func_800A72FC(arg0);
                break;
            case 0x12D: /* switch 6 */
                func_800A7448(arg0);
                break;
            case 0x12E: /* switch 6 */
                func_800A75A0(arg0);
                break;
            case 0x12F: /* switch 6 */
                func_800A761C(arg0);
                break;
            case 0x130: /* switch 6 */
                if (arg0->cursor != 0) {
                    gDisplayListHead = func_8009BA74(
                        gDisplayListHead, segmented_to_virtual_dupe(D_800E7D54[D_800EFD64[D_802874D8.unk1E]]),
                        arg0->column, arg0->row);
                    func_8009A7EC(arg0->D_8018DEE0_index, arg0->column, arg0->row, 0, arg0->unk1C);
                }
                break;
            case 0x190: /* switch 6 */
            case 0x191: /* switch 6 */
            case 0x192: /* switch 6 */
            case 0x193: /* switch 6 */
            case 0x194: /* switch 6 */
            case 0x195: /* switch 6 */
            case 0x196: /* switch 6 */
            case 0x197: /* switch 6 */
            case 0x198: /* switch 6 */
            case 0x199: /* switch 6 */
            case 0x19A: /* switch 6 */
            case 0x19B: /* switch 6 */
            case 0x19C: /* switch 6 */
            case 0x19D: /* switch 6 */
            case 0x19E: /* switch 6 */
            case 0x19F: /* switch 6 */
            case 0x1A0: /* switch 6 */
            case 0x1A1: /* switch 6 */
            case 0x1A2: /* switch 6 */
            case 0x1A3: /* switch 6 */
            case 0x1A4: /* switch 6 */
            case 0x1A5: /* switch 6 */
            case 0x1A6: /* switch 6 */
            case 0x1A7: /* switch 6 */
            case 0x1A8: /* switch 6 */
            case 0x1A9: /* switch 6 */
            case 0x1AA: /* switch 6 */
            case 0x1AB: /* switch 6 */
            case 0x1AC: /* switch 6 */
            case 0x1AD: /* switch 6 */
            case 0x1AE: /* switch 6 */
            case 0x1AF: /* switch 6 */
            case 0x1B0: /* switch 6 */
            case 0x1B1: /* switch 6 */
            case 0x1B2: /* switch 6 */
            case 0x1B3: /* switch 6 */
            case 0x1B4: /* switch 6 */
            case 0x1B5: /* switch 6 */
            case 0x1B6: /* switch 6 */
            case 0x1B7: /* switch 6 */
            case 0x1B8: /* switch 6 */
            case 0x1B9: /* switch 6 */
            case 0x1BA: /* switch 6 */
            case 0x1BB: /* switch 6 */
            case 0x1BC: /* switch 6 */
            case 0x1BD: /* switch 6 */
            case 0x1BE: /* switch 6 */
            case 0x1BF: /* switch 6 */
            case 0x1C0: /* switch 6 */
            case 0x1C1: /* switch 6 */
            case 0x1C2: /* switch 6 */
            case 0x1C3: /* switch 6 */
            case 0x1C4: /* switch 6 */
            case 0x1C5: /* switch 6 */
            case 0x1C6: /* switch 6 */
            case 0x1C7: /* switch 6 */
            case 0x1C8: /* switch 6 */
            case 0x1C9: /* switch 6 */
            case 0x1CA: /* switch 6 */
            case 0x1CB: /* switch 6 */
            case 0x1CC: /* switch 6 */
            case 0x1CD: /* switch 6 */
            case 0x1CE: /* switch 6 */
                func_800A7790(arg0);
                break;
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_8009F5E0.s")
#endif

void func_800A08D8(u8 arg0, s32 column, s32 row) {
    if (arg0 >= 0x10) {
        arg0 -= 0x10;
        if (arg0 < 0x85) {
            if (arg0 >= 0x32) {
                arg0 = 0x2B;
            }
            gDisplayListHead =
                func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E7AF8[arg0]), column, row);
        }
    }
}

// Walks through `someString` for `len` charcters, seemingly even going over null terminators.
// Returns a count of all non-terminator characters walked over.
s32 func_800A095C(char* someString, s32 len, s32 column, s32 row) {
    s32 tempColumn;
    s32 nonTerminatorCount;

    nonTerminatorCount = 0;
    tempColumn = column;
    for (; len != 0; len--, tempColumn += 8) {
        if (*someString != 0) {
            nonTerminatorCount++;
        }
        func_800A08D8(*someString++, tempColumn, row);
    }
    return nonTerminatorCount;
}

#ifdef NON_MATCHING
// Non-matching due to the constants 9 and 0xA being saved to the wrong registers
// Same functionality, but doesn't match byte for byte :/
void func_800A09E0(struct_8018D9E0_entry* arg0) {
    s32 table_row, x = 0x20, y;
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, 194);
    for (table_row = 0; table_row < 9; table_row++) {
        if ((D_800E86D0[0] != 0) || ((table_row != 0) && (table_row != 8))) {
            y = (table_row * 0xA) + arg0->row;
            gDisplayListHead = func_8009BA74(gDisplayListHead, D_0200157C, x, y);
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800A09E0.s")
#endif

void func_800A0AD0(UNUSED struct_8018D9E0_entry* arg0) {
    struct_8018D9E0_entry* temp_t1;
    // Find struct_8018D9E0_entry with a type/id of 0xDA
    temp_t1 = find_8018D9E0_entry_dupe(0xDA);
    if ((gControllerPakMenuSelection != CONTROLLER_PAK_MENU_SELECT_RECORD) &&
        (gControllerPakMenuSelection != CONTROLLER_PAK_MENU_END)) {
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, temp_t1->unk20, 0x00, 0xFF);
        gDisplayListHead =
            func_8009BA74(gDisplayListHead, D_02001874, 0x24, (gControllerPakSelectedTableRow * 0xA) + 0x7C);
    }
}

void func_800A0B80(struct_8018D9E0_entry* arg0) {
    UNUSED s32 temp_a2;
    s32 temp_s1;
    s32 temp_s2;
    s32 var_s1;
    s32 var_s5;
    s32 var_s0;
    OSPfsState* temp_s4;

    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x32, 0xFF);
    for (var_s5 = 0; var_s5 < 9; var_s5++) {
        if (D_800E86C4[var_s5] == 0) {
            continue;
        }

        temp_s1 = var_s5 * 0xA;
        var_s0 = D_800E86C4[var_s5];
        if (var_s0 < 0xA) {
            func_800A08D8(var_s0 + 0x10, 0x00000032, arg0->row + temp_s1 + 1);
        } else {
            var_s0 %= 10;
            func_800A08D8(var_s0 + 0x10, 0x00000035, arg0->row + temp_s1 + 1);
            func_800A08D8(0x11U, 0x0000002F, arg0->row + temp_s1 + 1);
        }
        temp_s2 = arg0->row + temp_s1 + 1;
        if (pfsError[D_800E86C4[var_s5] - 1] == 0) {
            temp_s4 = &pfsState[D_800E86C4[var_s5] - 1];
            var_s0 = func_800A095C(temp_s4->game_name, 0x00000010, 0x0000004F, temp_s2);
            if (temp_s4->ext_name[0] != 0) {
                func_800A08D8(0x3CU, (var_s0 * 8) + 0x4F, temp_s2);
                func_800A08D8(temp_s4->ext_name[0], (var_s0 * 8) + 0x57, temp_s2);
            }
            var_s1 = 0x10;
            var_s0 = (temp_s4->file_size + 0xFF) >> 8;
            // An actual do-while loop, a rare beast in these parts
            do {
                func_800A08D8(((var_s0 % 10) + 0x10), var_s1 + 0xFD, temp_s2);
                var_s0 /= 10;
                var_s1 -= 8;
            } while (var_s0 != 0);
        }
    }
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
}

void func_800A0DFC(void) {
    s32 temp_t6;
    s32 var_s0;
    s32 var_s1;

    var_s0 = gControllerPak1NumPagesFree;
    var_s1 = 0x00000110;
    do {
        temp_t6 = var_s0 % 10;
        var_s0 /= 10;
        gDisplayListHead =
            func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E7D0C[temp_t6]), var_s1, 0x000000B8);
        var_s1 -= 9;
    } while (var_s0 != 0);
}

void func_800A0EB8(UNUSED struct_8018D9E0_entry* arg0, s32 arg1) {
    s32 var_t1;
    s32 thing;
    Unk_D_800E70A0* temp_v0;
    struct_8018D9E0_entry* temp_t3;

    temp_t3 = find_8018D9E0_entry_dupe(0x000000DA);
    if (arg1 == 0) {
        if (gControllerPakMenuSelection == CONTROLLER_PAK_MENU_END) {
            var_t1 = 1;
        } else {
            var_t1 = 0;
        }
    } else {
        thing = gControllerPakMenuSelection; // ?
        if ((thing == CONTROLLER_PAK_MENU_ERASE) || (thing == CONTROLLER_PAK_MENU_QUIT)) {
            var_t1 = ((arg1 * 2) + gControllerPakMenuSelection) - 3;
        } else {
            return;
        }
    }
    temp_v0 = &D_800E7278[var_t1];
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, temp_t3->unk20, 0x00, 0xFF);
    gDisplayListHead = func_8009BA74(gDisplayListHead, D_0200184C, (s32) temp_v0->column, (s32) temp_v0->row);
}

void func_800A0FA4(struct_8018D9E0_entry* arg0, s32 arg1) {
    switch (arg0->cursor) {
        case 0:
        case 2:
        case 3:
            gDisplayListHead = func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E8234[(arg1 * 2) + 0]),
                                             arg0->column, arg0->row);
            gDisplayListHead = func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E8234[(arg1 * 2) + 1]),
                                             arg0->column, arg0->row);
            break;
        case 1:
        case 4:
            gDisplayListHead = func_8009BC9C(gDisplayListHead, segmented_to_virtual_dupe(D_800E8234[(arg1 * 2) + 0]),
                                             arg0->column, arg0->row, 2, arg0->unk1C);
            gDisplayListHead = func_8009BC9C(gDisplayListHead, segmented_to_virtual_dupe(D_800E8234[(arg1 * 2) + 1]),
                                             arg0->column, arg0->row, 2, arg0->unk1C);
            break;
    }
}

void func_800A10CC(struct_8018D9E0_entry* arg0) {
    s32 var_s1;
    s32 index;

    switch (arg0->cursor) {
        case 2:
        case 3:
        case 4:
        case 5:
            gDisplayListHead =
                draw_box_fill(gDisplayListHead, 0x0000001E, 0x00000032, 0x00000122, 0x0000006E, 0, 0, 0, 0x000000FF);
            index = arg0->cursor - 2;
            set_text_color(3);
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                // In a perfect world this would be `D_800E7940[index][var_s1]`
                func_80093324(0x00000023, 0x41 + (0xD * var_s1), D_800E7940[(index * 4) + var_s1], 0, 0.65f, 0.65f);
            }
            break;
        default:
            break;
    }
}

void func_800A11D0(struct_8018D9E0_entry* arg0, s32 arg1, s32 arg2) {
    RGBA16* temp_v1;

    temp_v1 = &D_800E74A8[arg1];
    gDPSetPrimColor(gDisplayListHead++, 0, 0, temp_v1->red, temp_v1->green, temp_v1->blue, temp_v1->alpha);
    gDPSetEnvColor(gDisplayListHead++, arg2, arg2, arg2, 0x00);
    gDisplayListHead =
        func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E82B4[arg1]), arg0->column, arg0->row);
}

void func_800A12BC(struct_8018D9E0_entry* arg0, MkTexture* arg1) {
    switch (arg0->cursor) {
        case 0:
        case 2:
        case 4:
            gDisplayListHead = func_8009BA74(gDisplayListHead, arg1, arg0->column, arg0->row);
            break;
        case 1:
        case 3:
            gDisplayListHead = func_8009BC9C(gDisplayListHead, arg1, arg0->column, arg0->row, 2, arg0->unk1C);
            break;
    }
}

void func_800A1350(struct_8018D9E0_entry* arg0) {
    s32 thing;
    if (func_800AAFCC(arg0->type - 0x2B) < 0) {
        switch (arg0->cursor) {
            case 0:
            case 2:
            case 4:
                gDisplayListHead = draw_box(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x40,
                                            arg0->row + 0x4C, 0, 0, 0, 0x00000064);
                break;
            case 1:
            case 3:
                thing = arg0->unk1C;
                gDisplayListHead = draw_box(gDisplayListHead, arg0->column + thing, arg0->row,
                                            (arg0->column - thing) + 0x40, arg0->row + 0x4C, 0, 0, 0, 0x00000064);
                break;
        }
    }
}

void func_800A143C(struct_8018D9E0_entry* arg0, s32 arg1) {
    switch (arg0->cursor) {
        case 0:
        case 2:
        case 3:
            gDisplayListHead = func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_800E82C4[arg1 + 1]),
                                             arg0->column, arg0->row);
            break;
        case 1:
        case 4:
            gDisplayListHead = func_8009BC9C(gDisplayListHead, segmented_to_virtual_dupe(D_800E82C4[arg1 + 1]),
                                             arg0->column, arg0->row, 2, arg0->unk1C);
            break;
    }
}

void func_800A1500(struct_8018D9E0_entry* arg0) {
    struct_8018D9E0_entry* temp_v0;
    Unk_D_800E70A0* temp_v0_2;
    s32 var_a1;

    var_a1 = 0;
    temp_v0 = find_8018D9E0_entry_dupe(0x00000064);
    switch (temp_v0->cursor) { /* irregular */
        case 0:
        case 1:
            break;
        case 2:
            if (((temp_v0->unk1C % 4) + 0x5F) != arg0->type) {
                var_a1 = 1;
            }
            break;
        case 3:
            var_a1 = 1;
            break;
    }
    switch (var_a1) { /* switch 1; irregular */
        case 0:       /* switch 1 */
            func_8009A76C(arg0->D_8018DEE0_index, 0x00000017, 0x00000070, -1);
            break;
        case 1: /* switch 1 */
            temp_v0_2 = &D_800E7168[arg0->type - 0x5F];
            func_8009A76C(arg0->D_8018DEE0_index, temp_v0_2->column, temp_v0_2->row, -2);
            break;
    }
}

void func_800A15EC(struct_8018D9E0_entry* arg0) {
    s16 courseId = gCupCourseOrder[(arg0->type - 0x7C) / 4][(arg0->type - 0x7C) % 4];
    gDisplayListHead =
        func_8009C204(gDisplayListHead, segmented_to_virtual_dupe(D_800E7D74[courseId]), arg0->column, arg0->row, 2);
    gDisplayListHead = draw_box(gDisplayListHead, arg0->column, arg0->row + 0x27, arg0->column + 0x40, arg0->row + 0x30,
                                0, 0, 0, 0xFF);
    gDisplayListHead = func_8009C204(gDisplayListHead, segmented_to_virtual_dupe(D_800E7DC4[courseId]), arg0->column,
                                     arg0->row + 0x27, 3);
    if (func_800B639C(arg0->type - 0x7C) >= 0) {
        // The "^ 0" is required to force the use of v1 instead of a 4th s* register
        gDisplayListHead = draw_flash_select_case_slow(gDisplayListHead, arg0->column + 0x20, arg0->row ^ 0,
                                                       arg0->column + 0x3F, arg0->row + 9);
        gDisplayListHead =
            func_8009C204(gDisplayListHead, segmented_to_virtual_dupe(&D_02004A0C), arg0->column + 0x20, arg0->row, 2);
    }
}

void func_800A1780(struct_8018D9E0_entry* arg0) {
    RGBA16* temp_a1;
    RGBA16* temp_v1;
    s32 temp_a2;
    u32 red;
    u32 green;
    u32 blue;
    u32 alpha;

    temp_v1 = &D_800E74D0[arg0->unk20];
    temp_a1 = &D_800E74D0[(arg0->unk20 + 1) % 3];
    temp_a2 = 256 - arg0->unk1C;
    red = ((temp_v1->red * temp_a2) + (temp_a1->red * arg0->unk1C)) / 256;
    green = ((temp_v1->green * temp_a2) + (temp_a1->green * arg0->unk1C)) / 256;
    blue = ((temp_v1->blue * temp_a2) + (temp_a1->blue * arg0->unk1C)) / 256;
    alpha = ((temp_v1->alpha * temp_a2) + (temp_a1->alpha * arg0->unk1C)) / 256;
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
    gDisplayListHead = func_8009BA74(gDisplayListHead, segmented_to_virtual_dupe(D_02001FA4), arg0->column, arg0->row);
}

void func_800A1924(struct_8018D9E0_entry* arg0) {
    func_8009A76C(arg0->D_8018DEE0_index, 0x17, 0x84, -1);
    if (func_800B639C(gTimeTrialDataCourseIndex) >= TIME_TRIAL_DATA_LUIGI_RACEWAY) {
        gDisplayListHead = draw_flash_select_case_slow(gDisplayListHead, 0x57, 0x84, 0x96, 0x95);
        gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004A0C, 0x57, 0x84);
    }
    func_8004EF9C(gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]);
    do {
        gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    } while (0);
}

void func_800A1A20(struct_8018D9E0_entry* arg0) {
    s16 courseId;
    s32 recordType;
    s32 rowOffset;

    courseId = gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4];
    arg0->column = 0x14;
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    draw_text(0x69, arg0->row + 0x19, gCourseNamesDup[courseId], 0, 0.75f, 0.75f);
    set_text_color(TEXT_RED);
    func_80093324(0x2D, arg0->row + 0x28, (char*) &D_800E77D8, 0, 0.75f, 0.75f);
    func_800936B8(0xA5, arg0->row + 0x28, sCourseLengths[courseId], 1, 0.75f, 0.75f);
    set_text_color(TEXT_YELLOW);
    func_80093324(0xA0, arg0->row + 0x86, D_800E7728[0], 0, 0.75f, 0.75f);
    // Print the 3 Lap Time Trial records
    for (recordType = TIME_TRIAL_3LAP_RECORD_1, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD;
         recordType++, rowOffset += 0xD) {
        set_text_color(TEXT_RED);
        func_800A474C(recordType, 0x96, arg0->row + rowOffset + 0x92);
    }
    set_text_color(TEXT_YELLOW);
    func_80093324(0xA0, arg0->row + 0xD5, D_800E7728[1], 0, 0.75f, 0.75f);
    func_800A474C(TIME_TRIAL_1LAP_RECORD, 0x96, arg0->row + 0xE1);
}

void func_800A1BE0(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED char* wut;
    Unk_D_800E70A0 sp78;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    UNUSED s32 thing;
    CourseTimeTrialRecords* temp_s6;

    temp_s6 = &gSaveData.allCourseTimeTrialRecords.cupRecords[gTimeTrialDataCourseIndex / 4]
                   .courseRecords[gTimeTrialDataCourseIndex % 4];
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        wut = D_800E7834[var_s0];
        var_s1 = 0;
        if (var_s0 == gCourseRecordsMenuSelection) {
            var_s2 = 5;
        } else {
            var_s2 = 1;
            switch (var_s0) { /* irregular */
                case 1:
                    if (temp_s6->unknownBytes[0] == 0) {
                        var_s1 = 1;
                    }
                    break;
                case 2:
                    if (func_800B639C((s32) gTimeTrialDataCourseIndex) < 0) {
                        var_s1 = 1;
                    }
                    break;
            }
        }
        if (var_s1 != 0) {
            set_text_color(0);
            gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
            func_80093358(0x00000025, 0x3F + (0xD * var_s0), D_800E7834[var_s0], 0, 0.6f, 0.6f);
        } else {
            set_text_color(var_s2);
            func_80093324(0x00000025, 0x3F + (0xD * var_s0), D_800E7834[var_s0], 0, 0.6f, 0.6f);
        }
    }
    sp78.column = 0x001F;
    sp78.row = (gCourseRecordsMenuSelection * 0xD) + 0x3A;
    func_800A66A8(arg0, (Unk_D_800E70A0*) &sp78);
}

void func_800A1DE0(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    Unk_D_800E70A0 sp58;
    s32 var_a0;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED char* wut;

    set_text_color(1);
    for (var_s1 = 0; var_s1 < 3; var_s1++) {
        // Removing `wut` introduces counter intuitive changes to how this loop is handled
        // Also, in a perfect world this would be `D_800E7848[gCourseRecordsMenuSelection - 1][var_s1]`
        wut = D_800E7848[(gCourseRecordsMenuSelection - 1) * 3 + var_s1];
        func_80093324(0x0000001B, 0x3C + (0xD * var_s1), D_800E7848[(gCourseRecordsMenuSelection - 1) * 3 + var_s1], 0,
                      0.65f, 0.65f);
    }

    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        wut = D_800E7840[var_s1];
        if (var_s1 == D_8018EDF9) {
            var_a0 = 5;
        } else {
            var_a0 = 1;
        }
        set_text_color(var_a0);
        func_80093324(0x00000043, 0x6E + (0xD * var_s1), D_800E7840[var_s1], 0, 0.65f, 0.65f);
    }

    sp58.column = 0x003B;
    sp58.row = (D_8018EDF9 * 0xD) + 0x66;
    func_800A66A8(arg0, &sp58);
}

void func_800A1F30(UNUSED struct_8018D9E0_entry* unused) {
    s32 row;
    s32 text;

    set_text_color(TEXT_RED);
    for (row = 0x49, text = 0; row < 0x69; row += 0x10, text++) {
        func_80093324(0x2A, row, D_800E7860[text], 0, 0.75f, 0.75f);
    }
}

#ifdef NON_MATCHING
// Register allocation stuff, minor stack diffs
void func_800A1FB0(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0 spE0;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    UNUSED s32 stackPadding5;
    s32 var_s5;
    s32 var_s4;
    UNUSED s32 stackPadding6;
    char spB8[3];
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    s32 var_s1;
    char spA8[3];
    s32 var_s2;
    UNUSED s32 temp_v0;
    UNUSED s32 stackPadding9;
    char sp98[3];
    struct_8018EE10_entry* var_v1;

    gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, 0x00000064);
    switch (D_8018EDEC) { /* switch 1 */
        case 0x15:        /* switch 1 */
        case 0x16:        /* switch 1 */
        case 0x17:        /* switch 1 */
        case 0x18:        /* switch 1 */
            for (stackPadding0 = 0; stackPadding0 < 4; stackPadding0++) {
                set_text_color_rainbow_if_selected(D_8018EDEC - 0x15, stackPadding0, 3);
                func_80093324(0x00000032, 0x55 + (0x23 * stackPadding0), D_800E7868[stackPadding0], 0, 0.9f, 1.0f);
                if (stackPadding0 == (D_8018EDEC - 0x15)) {
                    spE0.column = 0x0032;
                    spE0.row = 0x55 + (0x23 * stackPadding0);
                }
            }
            set_text_color(1);
            draw_text(0x000000E6, 0x00000078, gSoundModeNames[gSoundMode], 0, 1.0f, 1.0f);
            break;
        case 0x1E: /* switch 1 */
        case 0x1F: /* switch 1 */
            set_text_color(3);
            for (stackPadding0 = 0; stackPadding0 < 3; stackPadding0++) {
                func_80093324(0x00000028, 0x55 + (0x14 * stackPadding0), D_800E7878[stackPadding0], 0, 1.0f, 1.0f);
            }
            for (stackPadding0 = 0; stackPadding0 < 2; stackPadding0++) {
                set_text_color_rainbow_if_selected(D_8018EDEC - 0x1E, stackPadding0, 1);
                func_80093324(0x00000084, 0x96 + (0x19 * stackPadding0), D_800E7840[stackPadding0], 0, 1.0f, 1.0f);
                if (stackPadding0 == (D_8018EDEC - 0x1E)) {
                    spE0.column = 0x0084;
                    spE0.row = 0x96 + (0x19 * stackPadding0);
                }
            }
            break;
        case 0x20: /* switch 1 */
            set_text_color(3);
            for (stackPadding0 = 0; stackPadding0 < 3; stackPadding0++) {
                func_80093324(0x00000032, 0x55 + (0x14 * stackPadding0), D_800E7884[stackPadding0], 0, 1.0f, 1.0f);
            }
            break;
        case 0x2A: /* switch 1 */
        case 0x2B: /* switch 1 */
        case 0x2C: /* switch 1 */
        case 0x2D: /* switch 1 */
            set_text_color(2);
            var_s1 = D_8018EDEC - 0x2A;
            for (stackPadding0 = 0; stackPadding0 < 3; stackPadding0++) {
                func_80093324(0x00000032, 0x55 + (0x14 * stackPadding0), D_800E78D0[(var_s1 * 3) + stackPadding0], 0,
                              0.9f, 0.9f);
            }
            break;
        case 0x34: /* switch 1 */
        case 0x35: /* switch 1 */
        case 0x36: /* switch 1 */
        case 0x37: /* switch 1 */
            set_text_color(2);
            var_s1 = D_8018EDEC - 0x34;
            for (stackPadding0 = 0; stackPadding0 < 4; stackPadding0++) {
                func_80093324(0x00000023, 0x55 + (0x14 * stackPadding0), D_800E7890[(var_s1 * 4) + stackPadding0], 0,
                              0.8f, 0.8f);
            }
            break;
        case 0x41: /* switch 1 */
        case 0x42: /* switch 1 */
            set_text_color(2);
            var_s1 = D_8018EDEC - 0x41;
            for (stackPadding0 = 0; stackPadding0 < 3; stackPadding0++) {
                func_80093324(0x00000041, 0x55 + (0x14 * stackPadding0), D_800E7900[(var_s1 * 3) + stackPadding0], 0,
                              0.9f, 0.9f);
            }
            break;
        case 0x46: /* switch 1 */
        case 0x47: /* switch 1 */
            set_text_color(3);
            for (stackPadding0 = 0; stackPadding0 < 3; stackPadding0++) {
                func_80093324(0x00000050, 0x55 + (0x14 * stackPadding0), D_800E7A48[stackPadding0], 0, 1.0f, 1.0f);
            }
            break;
        case 0x28: /* switch 1 */
        case 0x29: /* switch 1 */
        case 0x32: /* switch 1 */
        case 0x33: /* switch 1 */
            switch (D_8018EDEC) {
                case 0x28:
                case 0x29:
                    var_s5 = 0x28;
                    var_s4 = 0;
                    break;
                case 0x32:
                case 0x33:
                    var_s5 = 0x32;
                    var_s4 = 1;
                default:
                    // var_s5 = spC4;
                    // var_s4 = spC0;
                    break;
            }
            set_text_color(var_s4 + 1);
            draw_text(0x000000A0, 0x00000055, D_800E7920[var_s4], 0, 0.6f, 0.6f);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                set_text_color(3);
                draw_text(0x5C + (0x82 * var_s1), 0x0000007D, D_800E7918[var_s1], 0, 0.75f, 0.75f);
                for (var_s2 = 0; var_s2 < 2; var_s2++) {
                    if (var_s1 != var_s4) {
                        text_rainbow_effect(D_8018EDEC - var_s5, var_s2, TEXT_GREEN);
                        if (var_s2 == (D_8018EDEC - var_s5)) {
                            spE0.column = 0x20 + (0x89 * var_s1);
                            spE0.row = 0x96 + (0x1E * var_s2);
                        }
                    } else if ((var_s4 != 0) && (var_s2 == arg0->unk20)) {
                        set_text_color((s32) gGlobalTimer % 3);
                    } else {
                        set_text_color(1);
                    }
                    convert_number_to_ascii(var_s2 + 1, &spB8[0]);
                    func_80093324(0x20 + (0x89 * var_s1), 0x96 + (0x1E * var_s2), &spB8[1], 0, 0.6f, 0.6f);
                    if (var_s1 == 0) {
                        var_v1 = &D_8018EE10[var_s2];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) D_8018D9C0)[var_s2];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        func_80093324(0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        func_80093324(
                            0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2),
                            gCourseNamesDup2[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
        case 0x38: /* switch 1 */
        case 0x39: /* switch 1 */
            set_text_color(2);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                draw_text(0x000000A0, 0x4D + (0x14 * var_s1), D_800E7928[var_s1], 0, 0.8f, 0.8f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                set_text_color(3);
                draw_text(0x5C + (0x82 * var_s1), 0x0000007D, D_800E7918[var_s1], 0, 0.75f, 0.75f);
                for (var_s2 = 0; var_s2 != 2; var_s2++) {
                    if (var_s1 == 0) {
                        if (var_s2 == arg0->unk1C) {
                            set_text_color((s32) gGlobalTimer % 3);
                        } else {
                            set_text_color(1);
                        }
                    } else if (var_s2 == arg0->unk20) {
                        set_text_color((s32) gGlobalTimer % 3);
                    } else {
                        set_text_color(1);
                    }
                    convert_number_to_ascii(var_s2 + 1, &spA8[0]);
                    func_80093324(0x20 + (0x89 * var_s1), 0x96 + (0x1E * var_s2), &spA8[1], 0, 0.6f, 0.6f);
                    if (var_s1 == 0) {
                        var_v1 = &D_8018EE10[var_s2];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) D_8018D9C0)[var_s2];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        func_80093324(0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        func_80093324(
                            0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2),
                            gCourseNamesDup2[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            // Register allocation diffs here
            for (var_s2 = 0; var_s2 < 2; var_s2++) {
                if (var_s2 == (D_8018EDEC - 0x38)) {
                    spE0.column = 0x6E + (0x32 * var_s2);
                    spE0.row = 0x00D2;
                }
                text_rainbow_effect((D_8018EDEC - 0x38), var_s2, TEXT_YELLOW);
                func_80093324(0x6E + (0x32 * var_s2), 0x000000D2, D_800E7930[var_s2], 0, 0.75f, 0.75f);
            }
            break;
        case 0x3A: /* switch 1 */
        case 0x3B: /* switch 1 */
        case 0x3C: /* switch 1 */
            var_s5 = (D_8018EDEC - 0x3A) / 2;
            set_text_color(2);
            draw_text(0x000000A0, 0x00000055, D_800E7938[var_s5], 0, 1.0f, 1.0f);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                set_text_color(3);
                draw_text(0x5C + (0x82 * var_s1), 0x0000007D, D_800E7918[var_s1], 0, 0.75f, 0.75f);
                for (var_s2 = 0; var_s2 < 2; var_s2++) {
                    if (var_s1 == 0) {
                        if (var_s2 == arg0->unk1C) {
                            if (var_s5 == 0) {
                                set_text_color(2);
                            } else {
                                set_text_color(gGlobalTimer % 3);
                            }
                        } else {
                            set_text_color(1);
                        }
                    } else if (var_s2 == arg0->unk20) {
                        set_text_color(2);
                    } else {
                        set_text_color(1);
                    }
                    convert_number_to_ascii(var_s2 + 1, &sp98[0]);
                    func_80093324(0x20 + (0x89 * var_s1), 0x96 + (0x1E * var_s2), &sp98[1], 0, 0.6f, 0.6f);
                    if (var_s1 == 0) {
                        var_v1 = &D_8018EE10[var_s2];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) D_8018D9C0)[var_s2];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        func_80093324(0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        func_80093324(
                            0x2A + (var_s1 * 0x89), 0x96 + (0x1E * var_s2),
                            gCourseNamesDup2[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
    }
    switch (D_8018EDEC) { /* switch 2 */
        case 0x28:        /* switch 2 */
        case 0x29:        /* switch 2 */
        case 0x32:        /* switch 2 */
        case 0x33:        /* switch 2 */
        case 0x38:        /* switch 2 */
        case 0x39:        /* switch 2 */
            spE0.column -= 5;
            spE0.row -= 6;
            break;
        default: /* switch 2 */
            spE0.column -= 0xA;
            spE0.row -= 8;
            break;
    }
    func_800A66A8(arg0, (Unk_D_800E70A0*) &spE0);
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800A1FB0.s")
#endif

void func_800A2D1C(struct_8018D9E0_entry* arg0) {
    switch (D_80164A28) {
        case 1:
            gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0, 0x13F, 0x28);
            gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0xC7, 0x13F, 0xEF);
            arg0->unk1C = 0x28;
            break;
        case 2:
            arg0->unk1C -= 2;
            if (arg0->unk1C > 0) {
                gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0, 0x13F, arg0->unk1C);
                gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0xEF - arg0->unk1C, 0x13F, 0xEF);
            } else {
                arg0->type = 0;
            }
            break;
        default:
            if ((gModeSelection != GRAND_PRIX) || (gPlayerCountSelection1 != 1) || (D_8018EE08 != 0)) {
                arg0->type = 0;
            } else {
                arg0->unk1C -= 2;
                if (arg0->unk1C > 0) {
                    gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0, 0x13F, arg0->unk1C);
                    gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0xEF - arg0->unk1C, 0x13F, 0xEF);
                } else {
                    arg0->type = 0;
                }
            }
            break;
    }
}

void func_800A2EB8(struct_8018D9E0_entry* arg0) {
    s8 sp70[8];
    UNUSED s32 stackPadding0;
    char sp68[3];
    s32 temp_s0;
    s32 var_a0;
    s32 var_s2;

    for (var_s2 = 0; var_s2 < NUM_PLAYERS; var_s2++) {
        sp70[var_s2] = gPlayers[gGPCurrentRacePlayerIdByRank[var_s2]].characterId;
    }
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    func_80093324(arg0->column + 0x1E, arg0->row + 0x19, "results", 0, 1.0f, 1.0f);
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    func_80093324(arg0->column + 0x2C, arg0->row + 0x28, "round", 0, 0.7f, 0.7f);
    convert_number_to_ascii(gCupCourseSelection + 1, sp68);
    func_80093324(arg0->column + 0x57, arg0->row + 0x28, &sp68[1], 0, 0.7f, 0.7f);
    for (var_s2 = 0; var_s2 < 4; var_s2++) {
        if (gGPCurrentRacePlayerIdByRank[var_s2] < gPlayerCount) {
            var_a0 = (s32) gGlobalTimer % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        set_text_color(var_a0);
        func_800A32B4(arg0->column + 7, arg0->row + (0x10 * var_s2) + 0x38, (s32) sp70[var_s2], var_s2);
    }
    for (var_s2 = 4; var_s2 < 8; var_s2++) {
        if (gGPCurrentRacePlayerIdByRank[var_s2] < gPlayerCount) {
            var_a0 = (s32) gGlobalTimer % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        set_text_color(var_a0);
        func_800A32B4(0xBE - arg0->column, arg0->row + (0x10 * var_s2) + 0x5A, sp70[var_s2], var_s2);
    }
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    temp_s0 = (s32) (((f32) (get_string_width(gCupNames[gCupSelection]) + 8) * 0.6f) / 2);
    draw_text((-(s32) (((f32) (get_string_width(D_800E76CC[gCCSelection]) + 8) * 0.6f) / 2) - arg0->column) + 0xF5,
              arg0->row + 0xE1, gCupNames[D_800DC540], 0, 0.6f, 0.6f);
    draw_text((temp_s0 - arg0->column) + 0xF5, arg0->row + 0xE1,
              D_800E76CC[D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]]], 0, 0.6f, 0.6f);
}

void func_800A32B4(s32 arg0, s32 arg1, s32 characterId, s32 rank) {
    UNUSED s32 stackPadding0;
    f32 sp50;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    char sp3C[4];

    sp50 = gTimePlayerLastTouchedFinishLine[gGPCurrentRacePlayerIdByRank[rank]];
    convert_number_to_ascii(rank + 1, sp3C);
    sp3C[2] = '.';
    sp3C[3] = '\0';
    func_800939C8(arg0 - 1, arg1, &sp3C[1], -4, 0.7f, 0.7f);
    func_80093324(arg0 + 0xA, arg1, D_800E76A8[characterId], 0, 0.65f, 0.7f);
    convert_number_to_ascii((s32) (sp50 / 60.0f), sp3C);
    func_800939C8(arg0 + 0x42, arg1, sp3C, 0, 0.7f, 0.7f);
    convert_number_to_ascii((s32) sp50 % 60, sp3C);
    func_80093324(arg0 + 0x4E, arg1, "'", 0, 0.7f, 0.7f);
    func_800939C8(arg0 + 0x56, arg1, sp3C, 0, 0.7f, 0.7f);
    convert_number_to_ascii((s32) ((f64) sp50 * 100.0) % 100, sp3C);
    func_80093324(arg0 + 0x62, arg1, "\"", 0, 0.7f, 0.7f);
    func_800939C8(arg0 + 0x6A, arg1, sp3C, 0, 0.7f, 0.7f);
}

void func_800A34A8(struct_8018D9E0_entry* arg0) {
    s8 sp80[8];
    UNUSED s32 stackPadding0;
    char sp78[3];
    UNUSED s32 stackPadding1;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;
    UNUSED s32 stackPadding2;
    s32 temp_s0_3;
    s32 rank;
    s32 test;

    if (arg0->cursor != 0) {
        if (arg0->cursor < 9) {
            for (rank = 0; rank < 8; rank++) {
                sp80[rank] = gPlayers[gGPCurrentRacePlayerIdByRank[rank]].characterId;
            }
        } else {
            func_800A3A10(sp80);
            func_800A3A10(gCharacterIdByGPOverallRank);
        }
        set_text_color(4);
        func_80093324(arg0->column + 0x19, 0x19 - arg0->row, "driver's points", 0, 0.8f, 0.8f);
        set_text_color(5);
        func_80093324(arg0->column + 0x36, 0x28 - arg0->row, "round", 0, 0.7f, 0.7f);
        convert_number_to_ascii(gCupCourseSelection + 1, sp78);
        func_80093324(arg0->column + 0x61, (0x28 & 0xFFFFFFFF) - arg0->row, &sp78[1], 0, 0.7f, 0.7f);
        for (rank = 0; rank < 4; rank++) {
            test = arg0->cursor;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((rank * 5) < arg0->unk1C) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->cursor < 9) {
                    var_v0 = gGPCurrentRacePlayerIdByRank[rank];
                    var_v1 = 0;
                } else {
                    var_v1 = 0x0000000D;
                    var_v0 = D_80164478[sp80[rank]];
                }
                if (var_v0 < gPlayerCount) {
                    var_a0 = (s32) gGlobalTimer % 3;
                } else {
                    var_a0 = 3;
                }
                set_text_color(var_a0);
                func_800A3ADC(arg0, arg0->column + var_v1 + 0x1C, ((rank * 0x10) - arg0->row) + 0x38, sp80[rank], rank,
                              sp80);
            }
        }
        for (rank = 4; rank < NUM_PLAYERS; rank++) {
            test = arg0->cursor;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((rank * 5) < arg0->unk1C) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->cursor < 9) {
                    var_v0 = gGPCurrentRacePlayerIdByRank[rank];
                } else {
                    var_v0 = D_80164478[sp80[rank]];
                }
                if (var_v0 < gPlayerCount) {
                    var_a0 = (s32) gGlobalTimer % 3;
                } else {
                    var_a0 = 3;
                }
                set_text_color(var_a0);
                func_800A3ADC(arg0, 0xBE - arg0->column, arg0->row + (rank * 0x10) + 0x5A, sp80[rank], rank, sp80);
            }
        }
        set_text_color(5);
        temp_s0_3 = ((get_string_width(gCupNames[gCupSelection]) + 8) * 0.6f) / 2;
        draw_text((-(s32) (((get_string_width(D_800E76CC[gCCSelection]) + 8) * 0.6f) / 2) - arg0->column) + 0xE6,
                  arg0->row + 0xE1, gCupNames[D_800DC540], 0, 0.6f, 0.6f);
        draw_text((temp_s0_3 - arg0->column) + 0xE6, arg0->row + 0xE1,
                  D_800E76CC[D_800E86B0[gPlayerCount - 1][D_800E86AC[gPlayerCount - 1]]], 0, 0.6f, 0.6f);
    }
}

void func_800A3A10(s8* arg0) {
    s32 temp_a3;
    s32 temp_t1;
    s32 var_a1;
    s32 var_v0;
    UNUSED s32 thing1;
    UNUSED s8* new_var;

    for (var_v0 = 0; var_v0 < 8; var_v0++) {
        arg0[var_v0] = var_v0;
        for (var_a1 = var_v0; var_a1 > 0; var_a1--) {
            new_var = &arg0[var_a1];
            temp_a3 = arg0[var_a1 - 1];
            thing1 = gGPPointsByCharacterId[temp_a3];
            temp_t1 = arg0[var_a1];
            if (gGPPointsByCharacterId[temp_a3] < gGPPointsByCharacterId[temp_t1]) {
                arg0[var_a1] = temp_a3;
                arg0[var_a1 - 1] = temp_t1;
            } else if (gGPPointsByCharacterId[temp_t1] == gGPPointsByCharacterId[temp_a3]) {
                if ((D_80164478[temp_t1] < gPlayerCount) && (D_80164478[temp_t1] < D_80164478[temp_a3])) {
                    arg0[var_a1] = temp_a3;
                    arg0[var_a1 - 1] = temp_t1;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}

void func_800A3ADC(struct_8018D9E0_entry* arg0, s32 arg1, s32 arg2, s32 characterId, s32 arg4, s8* arg5) {
    UNUSED s32 stackPadding0;
    s32 wut;
    char sp34[4];
    s32 phi_v1;

    if (arg0->cursor < 9) {
        convert_number_to_ascii(arg4 + 1, sp34);
    } else {
        for (phi_v1 = arg4; phi_v1 > 0; phi_v1--) {
            wut = phi_v1 - 1;
            if (gGPPointsByCharacterId[arg5[phi_v1]] != gGPPointsByCharacterId[arg5[wut]]) {
                break;
            }
        }
        convert_number_to_ascii(phi_v1 + 1, sp34);
    }
    sp34[2] = '.';
    sp34[3] = '\0';
    func_800939C8(arg1, arg2, &sp34[1], -4, 0.7f, 0.7f);
    func_80093324(arg1 + 0xA, arg2, D_800E76A8[characterId], 0, 0.7f, 0.7f);
    convert_number_to_ascii(gGPPointsByCharacterId[characterId], sp34);
    func_800939C8(arg1 + 0x47, arg2, sp34, 0, 0.7f, 0.7f);
    if ((arg4 < 4) && (arg0->cursor < 9)) {
        convert_number_to_ascii(D_8018D9BC[arg4], sp34);
        sp34[0] = '+';
        func_80093324(arg1 + 0x5A, arg2, sp34, 0, 0.7f, 0.7f);
    }
}

void func_800A3C84(struct_8018D9E0_entry* arg0) {
    s32 recordType;
    s32 rowOffset;

    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    draw_text(arg0->column + 0x43, arg0->row + 0x19,
              gCourseNamesDup[gCupCourseOrder[gCupSelection][gCupCourseSelection]], 0, 0.6f, 0.6f);
    set_text_color(TEXT_YELLOW);
    draw_text(arg0->column + 0x46, arg0->row + 0x28, D_800E7730, 0, 0.75f, 0.75f);
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_3LAP_RECORD_5; recordType += 1, rowOffset += 0xF) {
        func_800A4550(recordType, arg0->column + 0x17, arg0->row + rowOffset + 0x37);
    }
    set_text_color(TEXT_YELLOW);
    func_80093324(0xB4 - arg0->column, arg0->row + 0x86, D_800E7728[0], 0, 0.75f, 0.75f);
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD; recordType += 1, rowOffset += 0xD) {
        set_text_color(TEXT_RED);
        func_800A474C(recordType, 0xAA - arg0->column, arg0->row + rowOffset + 0x92);
    }
    set_text_color(TEXT_YELLOW);
    func_80093324(0xB4 - arg0->column, arg0->row + 0xD5, D_800E7728[1], 0, 0.75f, 0.75f);
    func_800A474C(TIME_TRIAL_1LAP_RECORD, 0xAA - arg0->column, arg0->row + 0xE1);
}

void func_800A3E60(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    Unk_D_800E70A0 sp84;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    s32 var_v0;
    s32 var_v1;
    s32 var_s1;
    Unk_D_800E70A0* var_v0_5;
    char sp60[3];

    var_v0 = arg0->cursor;
    if (var_v0 == 0) {
        return;
    }
    if (var_v0 == 0x0000001F) {
        return;
    }

    set_text_color(4);
    draw_text(arg0->column + 0x55, 0x19 - arg0->row,
              gCourseNamesDup[gCupCourseOrder[gCupSelection][gCupCourseSelection]], 0, 0.6f, 0.6f);
    set_text_color(3);
    draw_text(arg0->column + 0x55, 0x28 - arg0->row, D_800E7730, 0, 0.75f, 0.75f);
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        func_800A4550(var_s1, arg0->column + 0x26, ((0xF * var_s1) - arg0->row) + 0x37);
    }
    switch (arg0->cursor) { /* switch 1 */
        case 1:             /* switch 1 */
        case 5:             /* switch 1 */
        case 6:             /* switch 1 */
        case 7:             /* switch 1 */
        case 8:             /* switch 1 */
        case 9:             /* switch 1 */
        case 10:            /* switch 1 */
        case 30:            /* switch 1 */
            for (var_s1 = 0; var_s1 < 6; var_s1++) {
                var_v1 = 0;
                text_rainbow_effect(arg0->cursor - 5, var_s1, 1);
                switch (var_s1) { /* switch 3; irregular */
                    case 4:       /* switch 3 */
                        if (D_80162DF8 == 1) {
                            var_v1 = 1;
                        }
                        break;
                    case 5: /* switch 3 */
                        if (D_80162DD4 != 0) {
                            var_v1 = 2;
                        }
                        break;
                }
                if (var_v1 != 0) {
                    set_text_color(0);
                    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
                    func_80093358(0xB2 - arg0->column, arg0->row + (0xD * var_s1) + 0x93, gTextPauseButton[var_s1 + 1],
                                  0, 0.75f, 0.75f);
                } else {
                    func_80093324(0xB2 - arg0->column, arg0->row + (0xD * var_s1) + 0x93, gTextPauseButton[var_s1 + 1],
                                  0, 0.75f, 0.75f);
                }
            }
            break;
        case 11: /* switch 1 */
        case 12: /* switch 1 */
        case 13: /* switch 1 */
        case 14: /* switch 1 */
        case 15: /* switch 1 */
        case 16: /* switch 1 */
            set_text_color(3);
            var_v1 = arg0->cursor - 11;
            for (var_s1 = 0; var_s1 < 7; var_s1++) {
                func_80093324(0x000000A2, 0x8C + (0xD * var_s1), D_800E798C[(var_v1 * 7) + var_s1], 0, 0.6f, 0.6f);
            }
            break;
        case 17: /* switch 1 */
        case 18: /* switch 1 */
            set_text_color(1);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                func_80093324(0x000000A5, arg0->row + (0xD * var_s1) + 0x8C, D_800E7A3C[var_s1], 0, 0.7f, 0.7f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                text_rainbow_effect(arg0->cursor - 0x11, var_s1, 1);
                convert_number_to_ascii(var_s1 + 1, sp60);
                func_80093324(0xB1 - arg0->column, 0xAA + (0x1E * var_s1), &sp60[1], 0, 0.6f, 0.6f);
                if (D_8018EE10[var_s1].ghostDataSaved == 0) {
                    func_80093324(0xBB - arg0->column, 0xAA + (0x1E * var_s1), D_800E7A44, 0, 0.45f, 0.45f);
                } else {
                    func_80093324(0xBB - arg0->column, 0xAA + (0x1E * var_s1),
                                  gCourseNamesDup2[gCupCourseOrder[D_8018EE10[var_s1].courseIndex / 4]
                                                                  [D_8018EE10[var_s1].courseIndex % 4]],
                                  0, 0.45f, 0.45f);
                }
            }
            break;
        case 19: /* switch 1 */
            set_text_color(3);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                func_80093324(0x000000AA, (0xD * var_s1) + 0x93, D_800E7A48[var_s1], 0, 0.8f, 0.8f);
            }
            break;
        case 20: /* switch 1 */
        case 21: /* switch 1 */
            if (var_s1 && var_s1) {}
            set_text_color(3);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                func_80093324(0x000000A3, arg0->row + (0xD * var_s1) + 0x8C, D_800E7A60[var_s1], 0, 0.67f, 0.67f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                text_rainbow_effect(arg0->cursor - 0x14, var_s1, 1);
                func_80093324(0xC8 - arg0->column, 0xB9 + (0xF * var_s1), D_800E7A6C[var_s1], 0, 0.75f, 0.75f);
            }
            break;
        case 25: /* switch 1 */
            set_text_color(3);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                func_80093324(0x000000A3, (0xD * var_s1) + 0x93, D_800E7A74[var_s1], 0, 0.67f, 0.67f);
            }
            break;
        case 26: /* switch 1 */
            set_text_color(3);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                func_80093324(0x000000AA, (0xD * var_s1) + 0x93, D_800E7A80[var_s1], 0, 0.75f, 0.75f);
            }
            break;
    }
    switch (arg0->cursor) { /* switch 2 */
        case 5:             /* switch 2 */
        case 6:             /* switch 2 */
        case 7:             /* switch 2 */
        case 8:             /* switch 2 */
        case 9:             /* switch 2 */
        case 10:            /* switch 2 */
            var_v0_5 = &D_800E7390[arg0->cursor - 5];
            break;
        case 17: /* switch 2 */
        case 18: /* switch 2 */
            var_v0_5 = &D_800E73C0[arg0->cursor - 17];
            break;
        case 20: /* switch 2 */
        case 21: /* switch 2 */
            var_v0_5 = &D_800E73D0[arg0->cursor - 20];
            break;
        case 30: /* switch 2 */
            var_v0_5 = &D_800E7390[arg0->unk1C - 5];
            break;
        default:
            return;
    }
    sp84.column = var_v0_5->column - arg0->column;
    sp84.row = var_v0_5->row + arg0->row;
    func_800A66A8(arg0, &sp84);
}

void func_800A4550(s32 lapNumber, s32 column, s32 row) {
    UNUSED s32 stackPadding0;
    s32 sp40;
    UNUSED s32 stackPadding1;
    s32 textColor;
    char sp34[3];
    struct_8018D9E0_entry* temp_v0_2;

    if (lapNumber < 3) {
        sp40 = playerHUD[PLAYER_ONE].lapDurations[lapNumber];
        set_text_color(2);
    } else {
        sp40 = playerHUD[PLAYER_ONE].someTimer;
        set_text_color(1);
    }
    func_800936B8(column + 0x21, row, D_800E7734[lapNumber], 0, 0.7f, 0.7f);
    temp_v0_2 = find_8018D9E0_entry_dupe(0x000000BB);
    if (lapNumber < 3) {
        if (temp_v0_2->unk20 & (1 << lapNumber)) {
            textColor = (s32) gGlobalTimer % 3;
        } else {
            textColor = TEXT_YELLOW;
        }
    } else {
        if (temp_v0_2->unk1C >= 0) {
            textColor = (s32) gGlobalTimer % 3;
        } else {
            textColor = TEXT_YELLOW;
        }
    }
    set_text_color(textColor);
    get_time_record_minutes(sp40, sp34);
    func_800939C8(column + 0x2C, row, sp34, 0, 0.7f, 0.7f);
    func_80093324(column + 0x37, row, "'", 0, 0.7f, 0.7f);
    get_time_record_seconds(sp40, sp34);
    func_800939C8(column + 0x40, row, sp34, 0, 0.7f, 0.7f);
    func_80093324(column + 0x4B, row, "\"", 0, 0.7f, 0.7f);
    get_time_record_centiseconds(sp40, sp34);
    func_800939C8(column + 0x55, row, sp34, 0, 0.7f, 0.7f);
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/mWCHC
// The majority of th diff is from the handling of `textColor`. There is something super fucky there.
// The permuter found a decent improvement with making `textColor` a `volatile` variable but that
// seems wrong for several reasons

void func_800A474C(s32 recordType, s32 column, s32 row) {
    UNUSED s32 stackPadding0;
    u32 timeRecord;
    s32 textColor;
    s32 temp_t0;
    s32 characterId;
    char sp38[3];
    struct_8018D9E0_entry* temp_v0;
    s32 sp30;
    if (gGamestate == 4) {
        sp30 = 0;
    } else {
        sp30 = 1;
    }
    if (recordType < 5) {
        if (sp30 == 0) {
            timeRecord = func_800B4E24(recordType);
        } else {
            timeRecord = func_800B4EB4(recordType, gTimeTrialDataCourseIndex);
        }
        set_text_color(TEXT_GREEN);
    } else {
        if (sp30 == 0) {
            timeRecord = func_800B4F2C();
        } else {
            timeRecord = func_800B4FB0(gTimeTrialDataCourseIndex);
        }
    }
    func_800939C8(column + 0x14, row, D_800E7744[recordType], 2, 0.65f, 0.65f);
    if (sp30 == 0) {
        temp_v0 = find_8018D9E0_entry_dupe(0x000000BB);
        if (recordType < 5) {
            if (recordType == temp_v0->unk1C) {
                textColor = gGlobalTimer % 3;
            } else {
                textColor = TEXT_YELLOW;
            }
        } else {
            // huh?
            textColor = TEXT_YELLOW;
            if (temp_v0->unk20 != 0) {
                textColor = gGlobalTimer;
                textColor %= 3;
            } else {
            }
        }
    } else {
        textColor = TEXT_YELLOW;
    }
    set_text_color(textColor);
    temp_t0 = timeRecord & 0xFFFFF;
    get_time_record_minutes(temp_t0, sp38);
    func_800939C8(column + 0x27, row, sp38, 0, 0.65f, 0.65f);
    func_80093324(column + 0x32, row, "'", 0, 0.65f, 0.65f);
    if (!textColor) {}
    get_time_record_seconds(temp_t0, sp38);
    func_800939C8(column + 0x3B, row, sp38, 0, 0.65f, 0.65f);
    func_80093324(column + 0x46, row, "\"", 0, 0.65f, 0.65f);
    get_time_record_centiseconds(temp_t0, sp38);
    func_800939C8(column + 0x50, row, sp38, 0, 0.65f, 0.65f);
    if (temp_t0 < 0x927C0) {
        characterId = timeRecord >> 0x14;
    } else {
        characterId = 8;
    }
    draw_text(column + 0x78, row, D_800E76A8[characterId], 0, 0.65f, 0.65f);
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800A474C.s")
#endif

void func_800A4A24(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    f32 someMultiplier = 0.85f;
    s32 thing = 24.0f * someMultiplier;

    temp_t0 = 0x140 - arg0->column;
    temp_t1 = arg0->row;
    temp_t2 = (s32) ((get_string_width(D_800E7780) + 8) * someMultiplier) / 2;
    gDisplayListHead = draw_box(gDisplayListHead, temp_t0 - temp_t2, (temp_t1 - thing) + 4, temp_t2 + temp_t0,
                                temp_t1 + 4, 0, 0, 0, 0x00000064);
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    draw_text(arg0->column - 3, arg0->row, D_800E7780, 0, 0.85f, 0.85f);
}

void render_pause_menu(struct_8018D9E0_entry* arg0) {
    if (gIsGamePaused != 0) {
        switch (gModeSelection) {
            case TIME_TRIALS:
                render_pause_menu_time_trials(arg0);
                break;
            case VERSUS:
                render_pause_menu_versus(arg0);
                break;
            case GRAND_PRIX:
                render_pause_grand_prix(arg0);
                break;
            case BATTLE:
                render_pause_battle(arg0);
                break;
        }
    }
}

void render_pause_menu_time_trials(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    char sp68[3];
    s32 temp_a0;
    s32 var_s0;
    s32 zero = 0; // ?

    gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x0000008C);
    set_text_color(TEXT_YELLOW);
    draw_text(0x000000A0, 0x00000050, gCourseNamesDup[gCupCourseOrder[gCupSelection][gCupCourseSelection]], 0, 1.0f,
              1.0f);
    set_text_color(TEXT_RED);
    draw_text(0x0000009D, 0x00000060, D_800E7728[0], 0, 0.8f, 0.8f);
    temp_a0 = func_800B4E24(TIME_TRIAL_3LAP_RECORD_1);
    temp_a0 &= 0xFFFFF;
    get_time_record_minutes(temp_a0, sp68);
    func_800939C8(0x0000007F, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    func_80093324(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
    get_time_record_seconds(temp_a0, sp68);
    func_800939C8(0x00000098, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    func_80093324(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
    get_time_record_centiseconds(temp_a0, sp68);
    func_800939C8(0x000000B3, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    draw_text(0x0000009D, 0x0000007C, D_800E7728[1], 0, 0.8f, 0.8f);
    temp_a0 = func_800B4F2C();
    temp_a0 &= 0xFFFFF;
    get_time_record_minutes(temp_a0, sp68);
    func_800939C8(0x0000007F, 0x00000089, sp68, 0, 0.8f, 0.8f);
    func_80093324(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
    get_time_record_seconds(temp_a0, sp68);
    func_800939C8(0x00000098, 0x00000089, sp68, 0, 0.8f, 0.8f);
    func_80093324(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
    get_time_record_centiseconds(temp_a0, sp68);
    func_800939C8(0x000000B3, 0x00000089, sp68, 0, 0.8f, 0.8f);
    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        text_rainbow_effect(arg0->cursor - 11, var_s0, TEXT_GREEN);
        func_80093324(D_800E8538[zero].column, D_800E8538[zero].row + (13 * var_s0), gTextPauseButton[var_s0], 0, 0.75f,
                      0.75f);
    }
}

void render_pause_menu_versus(struct_8018D9E0_entry* arg0) {
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_s0;
    s32 var_s1;
    Unk_D_800E70A0* temp_s3;
    struct UnkStruct_800DC5EC* temp_v0;

    temp_v0 = &D_8015F480[gIsGamePaused - 1];
    temp_v1 = temp_v0->screenStartX;
    temp_t0 = temp_v0->screenStartY;
    temp_t3 = temp_v0->screenWidth / 2;
    temp_t4 = temp_v0->screenHeight / 2;
    gDisplayListHead = draw_box(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8540[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        if (var_s0 > 0) {
            var_s1 = var_s0 + 1;
        } else {
            var_s1 = var_s0;
        }
        text_rainbow_effect(arg0->cursor - 0x15, var_s0, TEXT_YELLOW);
        func_80093324(temp_s3->column - 2, temp_s3->row + (13 * var_s0), gTextPauseButton[var_s1], 0, 0.75f, 0.75f);
    }
}

void render_pause_grand_prix(struct_8018D9E0_entry* arg0) {
    s32 temp_t0;
    s32 temp_v1;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_s0;
    Unk_D_800E70A0* temp_s3;
    struct UnkStruct_800DC5EC* temp_v0;
    f32 one = 1.0f;

    temp_v0 = &D_8015F480[gIsGamePaused - 1];
    temp_v1 = temp_v0->screenStartX;
    temp_t0 = temp_v0->screenStartY;
    temp_t3 = temp_v0->screenWidth / 2;
    temp_t4 = temp_v0->screenHeight / 2;
    gDisplayListHead = draw_box(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 140);
    temp_s3 = &D_800E85C0[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
    temp_s0 = ((get_string_width(gCupNames[gCupSelection]) * one) + 10.0f) / 2;
    temp_s1 = ((get_string_width(D_800E76CC[gCCSelection]) * one) + 10.0f) / 2;
    set_text_color(TEXT_YELLOW);
    draw_text(160 - temp_s1, temp_s3->row - 50, gCupNames[gCupSelection], 0, 1.0f, 1.0f);
    set_text_color(TEXT_YELLOW);
    draw_text(160 + temp_s0, temp_s3->row - 50, D_800E76CC[gCCSelection], 0, 1.0f, 1.0f);
    set_text_color(TEXT_YELLOW);
    draw_text(160, temp_s3->row - 30, gCourseNamesDup[gCupCourseOrder[gCupSelection][gCupCourseSelection]], 0, 1.0f,
              1.0f);
    for (var_s0 = 0; var_s0 < 2; var_s0++) {
        text_rainbow_effect(arg0->cursor - 31, var_s0, TEXT_YELLOW);
        func_80093324(temp_s3->column, temp_s3->row + (var_s0 * 13), gTextPauseButton[var_s0 * 4], 0, 0.75f, 0.75f);
    }
}

void render_pause_battle(struct_8018D9E0_entry* arg0) {
    struct UnkStruct_800DC5EC* temp_v0;
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_a1;
    s32 var_s1;
    Unk_D_800E70A0* temp_s3;

    temp_v0 = &D_8015F480[gIsGamePaused - 1];
    temp_v1 = temp_v0->screenStartX;
    temp_t0 = temp_v0->screenStartY;
    temp_t3 = temp_v0->screenWidth / 2;
    temp_t4 = temp_v0->screenHeight / 2;
    gDisplayListHead = draw_box(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8600[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
    for (var_a1 = 0; var_a1 < 4; var_a1++) {
        if (var_a1 > 0) {
            var_s1 = var_a1 + 1;
        } else {
            var_s1 = var_a1;
        }
        text_rainbow_effect(arg0->cursor - 0x29, var_a1, TEXT_YELLOW);
        func_80093324(temp_s3->column - 2, temp_s3->row + 13 * var_a1, gTextPauseButton[var_s1], 0, 0.75f, 0.75f);
    }
}

void func_800A54EC(void) {
    Unk_D_800E70A0 sp50;
    Unk_D_800E70A0* var_v1;
    struct_8018D9E0_entry* sp48;
    s32 whyTheSequel;
    s32 why;
    UNUSED Unk_D_800E70A0* huh;

    if (gIsGamePaused == 0) {
        return;
    }

    why = gModeSelection;
    sp48 = find_8018D9E0_entry(0x000000C7);
    if (why) {} // ?????
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gGfxPool->mtxEffect[gMatrixEffectCount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    switch (why) { /* irregular */
        default:
            break;
        case 1:
            var_v1 = &D_800E8538[0];
            break;
        case 2:
            var_v1 = &D_800E8540[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
            break;
        case 0:
            var_v1 = &D_800E85C0[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
            break;
        case 3:
            var_v1 = &D_800E8600[(gScreenModeSelection * 4) + (gIsGamePaused - 1)];
            break;
    }
    whyTheSequel = D_800F0B50[why];
    sp50.column = var_v1->column - 8;
    sp50.row = (var_v1->row + ((sp48->cursor - whyTheSequel) * 0xD)) - 8;
    func_800A66A8(sp48, &sp50);
}

void func_800A5738(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0 sp98;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    f32 why;
    char sp84[3];
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    s32 temp_a0;
    UNUSED s32 var_v1;
    s32 var_s1;
    s32 var_s2;
    s32 temp_v0;
    s32 zero = 0;
    Unk_D_800E70A0* var_v0_9;
    char sp5C[3];

    if (arg0->cursor == 0) {
        if ((arg0->unk1C >= 0x1E) && ((gGlobalTimer / 16) % 2)) {
            why = get_string_width(gTextPauseButton[5]) * 0.8f;
            gDisplayListHead =
                draw_box(gDisplayListHead, 0x000000C0, 0x00000021, (s32) (why) + 0xC6, 0x00000032, 0, 0, 0, 0x00000096);
            set_text_color(1);
            func_80093324(0x000000BF, 0x00000030, gTextPauseButton[5], 0, 0.8f, 0.8f);
        }
    } else {
        if (arg0->cursor == 1) {
            var_s1 = arg0->unk1C;
            var_s2 = (s32) (arg0->unk1C * 0xFF) / 140;
        } else {
            var_s1 = 0x0000008C;
            var_s2 = 0x000000FF;
        }
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, var_s1);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_s2);
        set_text_color(3);
        func_80093754(0x000000A0, 0x00000050, gCourseNamesDup[gCupCourseOrder[gCupSelection][gCupCourseSelection]], 0,
                      1.0f, 1.0f);
        switch (arg0->cursor) { /* switch 1 */
            case 1:             /* switch 1 */
            case 11:            /* switch 1 */
            case 12:            /* switch 1 */
            case 13:            /* switch 1 */
            case 14:            /* switch 1 */
            case 15:            /* switch 1 */
            case 16:            /* switch 1 */
                set_text_color(2);
                func_80093754(0x0000009D, 0x00000060, D_800E7728[0], 0, 0.8f, 0.8f);
                temp_a0 = func_800B4E24(0);
                temp_a0 &= 0xFFFFF;
                get_time_record_minutes(temp_a0, sp84);
                text_draw(0x0000007F, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                func_80093358(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
                get_time_record_seconds(temp_a0, sp84);
                text_draw(0x00000098, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                func_80093358(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
                get_time_record_centiseconds(temp_a0, sp84);
                text_draw(0x000000B3, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                func_80093754(0x0000009D, 0x0000007C, D_800E7728[1], 0, 0.8f, 0.8f);
                temp_a0 = func_800B4F2C();
                temp_a0 &= 0xFFFFF;
                get_time_record_minutes(temp_a0, sp84);
                text_draw(0x0000007F, 0x00000089, sp84, 0, 0.8f, 0.8f);
                func_80093358(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
                get_time_record_seconds(temp_a0, sp84);
                text_draw(0x00000098, 0x00000089, sp84, 0, 0.8f, 0.8f);
                func_80093358(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
                get_time_record_centiseconds(temp_a0, sp84);
                text_draw(0x000000B3, 0x00000089, sp84, 0, 0.8f, 0.8f);
                for (var_s1 = 0; var_s1 < 6; var_s1++) {
                    text_rainbow_effect(arg0->cursor - 0xB, var_s1, TEXT_GREEN);
                    func_80093358(D_800E8538[zero].column, D_800E8538[zero].row + (0xD * var_s1),
                                  gTextPauseButton[var_s1 + 1], 0, 0.75f, 0.75f);
                }
                break;
            case 21: /* switch 1 */
            case 22: /* switch 1 */
            case 23: /* switch 1 */
            case 24: /* switch 1 */
            case 25: /* switch 1 */
            case 26: /* switch 1 */
                set_text_color(3);
                temp_v0 = arg0->cursor - 0x15;
                for (var_s1 = 0; var_s1 < 7; var_s1++) {
                    func_80093324(0x0000004D, 0x6E + (0xD * var_s1), D_800E798C[(temp_v0 * 7) + var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 30: /* switch 1 */
            case 31: /* switch 1 */
                set_text_color(1);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    func_80093324(0x0000005A, arg0->row + (0xD * var_s1) + 0x6E, D_800E7A3C[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    text_rainbow_effect(arg0->cursor - 0x1E, var_s1, TEXT_GREEN);
                    convert_number_to_ascii(var_s1 + 1, sp5C);
                    func_80093324(0x5A - arg0->column, (0x96 + (0x14 * var_s1)), &sp5C[1], 0, 0.75f, 0.75f);
                    if (D_8018EE10[var_s1].ghostDataSaved == 0) {
                        func_80093324(0x69 - arg0->column, (0x96 + (0x14 * var_s1)), D_800E7A44, 0, 0.75f, 0.75f);
                    } else {
                        func_80093324(0x69 - arg0->column, (0x96 + (0x14 * var_s1)),
                                      gCourseNamesDup2[gCupCourseOrder[D_8018EE10[var_s1].courseIndex / 4]
                                                                      [D_8018EE10[var_s1].courseIndex % 4]],
                                      0, 0.75f, 0.75f);
                    }
                }
                break;
            case 32: /* switch 1 */
                set_text_color(3);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    func_80093324(0x00000064, (0xD * var_s1) + 0x6E, D_800E7A48[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 35: /* switch 1 */
            case 36: /* switch 1 */
                set_text_color(3);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    func_80093324(0x00000055, arg0->row + (0xD * var_s1) + 0x6E, D_800E7A60[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    text_rainbow_effect(arg0->cursor - 0x23, var_s1, TEXT_GREEN);
                    func_80093324(0x7D - arg0->column, 0x9B + (0xF * var_s1), D_800E7A6C[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 40: /* switch 1 */
                set_text_color(3);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    func_80093324(0x00000055, (0xD * var_s1) + 0x6E, D_800E7A74[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 41: /* switch 1 */
                set_text_color(3);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    func_80093324(0x0000005D, (0xD * var_s1) + 0x6E, D_800E7A80[var_s1], 0, 0.8f, 0.8f);
                }
                break;
        }
        switch (arg0->cursor) { /* switch 2 */
            case 11:            /* switch 2 */
            case 12:            /* switch 2 */
            case 13:            /* switch 2 */
            case 14:            /* switch 2 */
            case 15:            /* switch 2 */
            case 16:            /* switch 2 */
                var_v0_9 = &D_800E73E0[arg0->cursor - 11];
                break;
            case 30: /* switch 2 */
            case 31: /* switch 2 */
                var_v0_9 = &D_800E7410[arg0->cursor - 30];
                break;
            case 35:      /* switch 2 */
            case 36:      /* switch 2 */
                if (0) {} // wtf?
                var_v0_9 = &D_800E7420[arg0->cursor - 35];
                break;
            default:
                return;
        }
        sp98.column = var_v0_9->column;
        sp98.row = var_v0_9->row;
        func_800A66A8(arg0, &sp98);
    }
}

void func_800A6034(struct_8018D9E0_entry* arg0) {
    char* text;

    if (D_801657E8 != true) {
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, arg0->unk1C);
        text = gCupNames[D_800DC540];
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        func_80093754(arg0->column + 0x41, arg0->row + 0xA0, text, 0, 0.85f, 1.0f);
        text = gCourseNames[gCurrentCourseId];
        set_text_color((s32) gCurrentCourseId % 4);
        func_80093754(arg0->column + 0x41, arg0->row + 0xC3, text, 0, 0.65f, 0.85f);
    }
}

void func_800A6154(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    Unk_D_800E70A0 sp6C;
    UNUSED s32 stackPadding3;
    s32 var_s0;
    s32 var_s1;

    if (arg0->cursor == 0) {
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->unk1C);
        set_text_color(3);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->unk1C * 0xFF) / 100);
        for (var_s1 = 0, var_s0 = 0x96; var_s0 < 0xBE; var_s1++, var_s0 += 0x14) {
            func_80093358(0x0000008C, var_s0, gTextPauseButton[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    } else {
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        for (var_s1 = 0, var_s0 = 0x96; var_s1 < 2; var_s1++, var_s0 += 0x14) {
            text_rainbow_effect(arg0->cursor - 0xB, var_s1, TEXT_YELLOW);
            func_80093324(0x0000008C, var_s0, gTextPauseButton[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    }
    if (arg0->cursor >= 0xB) {
        sp6C.column = 0x0084;
        sp6C.row = (arg0->cursor * 0x14) - 0x4E;
        func_800A66A8(arg0, &sp6C);
    }
    if (arg0->unk20 > 0) {
        gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0, 0x0000013F, arg0->unk20);
        gDisplayListHead = func_80098FC8(gDisplayListHead, 0, 0xEF - arg0->unk20, 0x0000013F, 0x000000EF);
    }
}

void func_800A638C(struct_8018D9E0_entry* arg0) {
    UNUSED s32 temp_a0;
    s32 var_a1;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED s8** var_s2;

    if (arg0->cursor == 0) {
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->unk1C);
        set_text_color(5);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->unk1C * 0xFF) / 100);
        func_80093754(0x000000A0, arg0->row + 0x1E, D_800E7778[gModeSelection / 3], 0, 1.0f, 1.0f);
    } else {
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        set_text_color(5);
        draw_text(0x000000A0, arg0->row + 0x1E, D_800E7778[gModeSelection / 3], 0, 1.0f, 1.0f);
    }
    switch (arg0->cursor) { /* irregular */
        default:
            var_a1 = 0x000000FF;
            break;
        case 0:
        case 1:
            var_a1 = 0;
            break;
        case 2:
            var_a1 = arg0->unk1C;
            break;
    }
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_a1);
    switch (gPlayerCount) {
        case 2:
            func_800A69C8(arg0);
            break;
        case 3:
            func_800A6BEC(arg0);
            break;
        case 4:
            func_800A6CC0(arg0);
            break;
        default:
            break;
    }
    if (arg0->cursor >= 10) {
        for (var_s1 = 0; var_s1 < 4; var_s1++) {
            text_rainbow_effect(arg0->cursor - 0xA, var_s1, TEXT_GREEN);
            func_80093324(0x00000069, 0xAE + (0xF * var_s1), gTextPauseButton[var_s1 + 1], 0, 0.8f, 0.8f);
        }
        func_800A66A8(arg0, &D_800E7360[arg0->cursor - 10]);
    }
}

#ifdef NON_MATCHING
void guMtxCatL(Mtx*, Mtx*, Mtx*);
// https://decomp.me/scratch/GUqCE
// All the math stuff at the top is messed up
void func_800A66A8(struct_8018D9E0_entry* arg0, Unk_D_800E70A0* arg1) {
    Mtx* temp_s0;
    Mtx* temp_s1;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f14;

    temp_s1 = &gGfxPool->mtxEffect[gMatrixEffectCount];
    if (arg0->unk24 > 1.5) {
        arg0->unk24 *= 0.95;
    } else {
        arg0->unk24 = 1.5f;
    }
    temp_f2 = arg0->unk24 * 3.0f * arg0->unk8;
    temp_f12 = arg0->unk24 * 4.0f;
    temp_f14 = arg0->unk24 * 2.0f;
    D_8018EDA4 = temp_f2;
    D_8018EDA8 = temp_f12;
    D_8018EDAC = temp_f14;
    D_8018ED98 += D_8018EDA4;
    D_8018ED9C += D_8018EDA8;
    D_8018EDA0 += D_8018EDAC;
    guScale(temp_s1, 1.2f, 1.2f, 1.2f);
    temp_s0 = temp_s1 + 1;
    guRotate(temp_s0, D_8018ED9C, 0.0f, 1.0f, 0.0f);
    guMtxCatL(temp_s1, temp_s0, temp_s1);
    guRotate(temp_s0, D_8018EDA0, 0.0f, 0.0f, 1.0f);
    guMtxCatL(temp_s1, temp_s0, temp_s1);
    guRotate(temp_s0, D_8018ED98, 1.0f, 0.0f, 0.0f);
    guMtxCatL(temp_s1, temp_s0, temp_s1);
    guTranslate(temp_s0, arg1->column, arg1->row, 0.0f);
    guMtxCatL(temp_s1, temp_s0, temp_s1);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxEffect[gMatrixEffectCount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPNoOp(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPDisplayList(gDisplayListHead++, D_0D003090);
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800A66A8.s")
#endif

void func_800A69C8(UNUSED struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* thing;
    UNUSED s32 stackPadding1;
    s32 var_s0;
    char sp74[5];
    s32 var_v1;
    char* temp_s3;
    u8* var_s4;

    for (var_s0 = 0; var_s0 < gPlayerCount; var_s0++) {
        var_v1 = 0;
        thing = &D_800E7300[var_s0];
        switch (gModeSelection) { /* irregular */
            case VERSUS:
                if (gGPCurrentRaceRankByPlayerId[var_s0] != 0) {
                    var_v1 = 1;
                }
                var_s4 = &gNmiUnknown1[var_s0];
                break;
            case BATTLE:
                if (var_s0 != gPlayerWinningIndex) {
                    var_v1 = 1;
                }
                var_s4 = &gNmiUnknown4[var_s0];
                break;
        }
        temp_s3 = D_800E7720[var_v1];
        if (var_v1 != 0) {
            set_text_color(TEXT_BLUE);
        } else {
            set_text_color((s32) gGlobalTimer % 3);
        }
        func_800A79F4(var_s4[0], sp74);
        text_draw(thing->column + 0x10, thing->row + 0x75, sp74, 0, 1.0f, 1.0f);
        func_80093754(D_800E7380[var_s0].column, D_800E7380[var_s0].row, temp_s3, 0, 0.65f, 1.0f);
    }
    set_text_color(TEXT_BLUE);
    // Not a hyphen, that is an EUC-JP character
    text_draw(0x0000009E, D_800E7300[0].row + 0x6D, "ー", 0, 1.0f, 1.0f);
}

void func_800A6BEC(UNUSED struct_8018D9E0_entry* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < gPlayerCount; var_s0++) {
        switch (gModeSelection) { /* irregular */
            case VERSUS:
                func_800A6E94(3, var_s0, gNmiUnknown2);
                break;
            case BATTLE:
                func_800A6D94(3, var_s0, gNmiUnknown5);
                break;
        }
    }
}

void func_800A6CC0(UNUSED struct_8018D9E0_entry* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < gPlayerCount; var_s0++) {
        switch (gModeSelection) { /* irregular */
            case VERSUS:
                func_800A6E94(4, var_s0, gNmiUnknown3);
                break;
            case BATTLE:
                func_800A6D94(4, var_s0, gNmiUnknown6);
                break;
        }
    }
}

void func_800A6D94(s32 arg0, s32 arg1, u8* arg2) {
    UNUSED s32 stackPadding0;
    Unk_D_800E70A0* stackPadding1;
    char sp30[5];
    s32 var_v0;
    f32 otherThing;
    s32 thing;

    var_v0 = 0;
    if (arg1 != gPlayerWinningIndex) {
        var_v0 = 1;
    }
    thing = arg2[arg1];
    if (var_v0 != 0) {
        set_text_color(0);
    } else {
        set_text_color(gGlobalTimer % 3);
    }
    stackPadding1 = &D_800E7300[((arg0 - 2) * 4) + arg1];
    func_800A79F4(thing, sp30);
    otherThing = 0.75f;
    text_draw((stackPadding1->column + 0x20) - ((32.0f * otherThing) / 2), stackPadding1->row + 0x75, sp30, 0, 0.75f,
              0.75f);
}

// The ｓ/ｎ/ｒ/ー are not ASCII characters, they're EUC-JP characters
void func_800A6E94(s32 arg0, s32 arg1, u8* arg2) {
    UNUSED s32 stackPadding0;
    u8* temp_v0;
    Unk_D_800E70A0* temp_s0;
    char sp40[3];
    s32 rank;
    // Everything about this variable is bizarre
    s32 sp38 = -1;

    temp_s0 = &D_800E7300[((arg0 - 2) * 4) + arg1];
    rank = gGPCurrentRaceRankByPlayerId[arg1];
    if (rank == ++sp38) {
        set_text_color(gGlobalTimer % 3);
    } else {
        set_text_color(TEXT_YELLOW);
    }
    text_draw(temp_s0->column + 4, temp_s0->row + 0x5A, "1 ｓ ー", 0, 0.8f, 0.8f);
    temp_v0 = arg2 + (arg1 * 3);
    convert_number_to_ascii(temp_v0[0], sp40);
    text_draw(temp_s0->column + 0x2D, temp_s0->row + 0x5A, sp40, 0, 0.8f, 0.8f);
    if (rank == ++sp38) {
        set_text_color(gGlobalTimer % 3);
    } else {
        set_text_color(TEXT_BLUE);
    }
    text_draw(temp_s0->column + 4, temp_s0->row + 0x69, "2 ｎ ー", 0, 0.8f, 0.8f);
    convert_number_to_ascii(temp_v0[1], sp40);
    text_draw(temp_s0->column + 0x2D, temp_s0->row + 0x69, sp40, 0, 0.8f, 0.8f);
    if (++sp38 == rank) {
        set_text_color(gGlobalTimer % 3);
    } else {
        set_text_color(TEXT_RED);
    }
    text_draw(temp_s0->column + 4, temp_s0->row + 0x78, "3 ｒ ー", 0, 0.8f, 0.8f);
    convert_number_to_ascii(temp_v0[2], sp40);
    text_draw(temp_s0->column + 0x2D, temp_s0->row + 0x78, sp40, 0, 0.8f, 0.8f);
}

void func_800A70E8(struct_8018D9E0_entry* arg0) {
    s32 var_s0;
    s32 temp_f6;
    s32 alpha;
    s32 loopIndex;
    s32 stringIndex;

    if (arg0->cursor == 1) {
        var_s0 = get_string_width(D_800E7A34[0]) * 0.45f;
        temp_f6 = get_string_width(D_800E7A34[1]) * 0.45f;
        if (var_s0 < temp_f6) {
            var_s0 = temp_f6;
        }
        gDisplayListHead =
            draw_box(gDisplayListHead, 0x000000C0, 0x00000022, var_s0 + 0xC6, 0x00000039, 0, 0, 0, 0x00000096);
        alpha = 0x180 - ((arg0->unk1C % 32) * 8);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, alpha);
        set_text_color(TEXT_RED);
        for (loopIndex = 0x2C, stringIndex = 0; loopIndex < 0x40; loopIndex += 0xA, stringIndex++) {
            func_80093358(0x000000C0, loopIndex, D_800E7A34[stringIndex], 0, 0.45f, 0.45f);
        }
    }
}

// Shading layer of the grand prix podium result screen
void func_800A7258(struct_8018D9E0_entry* arg0) {
    if (arg0->cursor == 0) {
        // If shading layer is fading in
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, arg0->unk1C);
    } else {
        // All other stages of the podium scene
        gDisplayListHead = draw_box(gDisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, 0x64);
    }
}

// Podium scene, top line
void func_800A72FC(struct_8018D9E0_entry* arg0) {
    UNUSED s32 pad;
    s32 cupNameLength = (((f32) get_string_width(gCupNames[gCupSelection]) * 1) + 10) / 2;
    s32 ccNameLength = (((f32) get_string_width(D_800E76CC[gCCSelection]) * 1) + 10) / 2;

    set_text_color(TEXT_YELLOW);
    draw_text(arg0->column - ccNameLength, arg0->row, gCupNames[gCupSelection], 0, 1, 1);
    set_text_color(TEXT_YELLOW);
    draw_text(arg0->column + cupNameLength, arg0->row, D_800E76DC[gCCSelection], 0, 1, 1);
}

void func_800A7448(struct_8018D9E0_entry* arg0) {
    UNUSED s32 pad;
    s32 sp40;
    s32 sp3C;
    s32 thing = D_802874D8.unk1D;
    if (thing >= 3) {
        set_text_color(TEXT_YELLOW);
        draw_text(arg0->column, arg0->row, D_800E7A98, 0, 0.75f, 0.75f);
    } else {
        sp40 = (s32) (((f32) (get_string_width(D_800E7A88[0]) + 5) * 0.75f) / 2);
        sp3C = (s32) (((f32) (get_string_width(D_800E7A88[thing + 1]) + 5) * 0.75f) / 2);
        set_text_color(TEXT_YELLOW);
        draw_text(arg0->column - sp3C, arg0->row, D_800E7A88[0], 0, 0.75f, 0.75f);
        set_text_color(TEXT_YELLOW);
        draw_text(arg0->column + sp40, arg0->row, D_800E7A88[thing + 1], 0, 0.75f, 0.75f);
    }
}

void func_800A75A0(struct_8018D9E0_entry* arg0) {
    UNUSED s32 pad;
    s32 topThree;

    if (D_802874D8.unk1D < 3) {
        topThree = 0;
    } else {
        topThree = 1;
    }

    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    draw_text(arg0->column, arg0->row, D_800E7A9C[topThree], 0, 1.3f, 1.3f);
}

void func_800A761C(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    s32 sp48;
    s32 sp44;
    char sp3C[5];
    s32 temp_a0;

    temp_a0 = D_802874D8.unk1D + 1;
    func_800A79F4(temp_a0, sp3C);
    sp48 = ((get_string_width(D_800E7AA4[0]) + 5) * 1.2f) / 2;
    sp44 = ((get_string_width(D_800E7AA4[temp_a0]) + 5) * 1.2f) / 2;
    set_text_color(5);
    draw_text(arg0->column - sp44, arg0->row, D_800E7AA4[0], 0, 1.2f, 1.2f);
    set_text_color((s32) gGlobalTimer % 3);
    draw_text(arg0->column + sp48, arg0->row, D_800E7AA4[temp_a0], 0, 1.2f, 1.2f);
    convert_number_to_ascii(temp_a0, sp3C);
    func_800939C8((arg0->column + sp48) - 0x18, arg0->row, &sp3C[1], 0, 2.0f, 2.0f);
}

void func_800A7790(struct_8018D9E0_entry* arg0) {
    f32 someScaling;
    s32 creditIndex;
    s8 slideDirection;
    UNUSED s32 pad;
    creditIndex = arg0->type - 0x190;
    set_text_color(D_802850C0[creditIndex].textColor);
    slideDirection = D_802850C0[creditIndex].slideDirection;
    if ((slideDirection == SLIDE_RIGHT) || (slideDirection != SLIDE_LEFT)) {
        someScaling = D_802850C0[creditIndex].textScaling;
        func_800936B8(arg0->column, arg0->row, D_802854B0[creditIndex], arg0->unk1C * someScaling,
                      arg0->unk24 * someScaling, someScaling);
    } else {
        someScaling = D_802850C0[creditIndex].textScaling;
        func_80093324(arg0->column, arg0->row, D_802854B0[creditIndex], arg0->unk1C * someScaling,
                      arg0->unk24 * someScaling, someScaling);
    }
}

// Originally func_800A7894
// Presumes that "number" is a 2 digit number. Convert it to a string
void convert_number_to_ascii(s32 number, char* buffer) {
    buffer[0] = (number / 0xA) + 0x30;
    buffer[1] = (number % 0xA) + 0x30;
    // Terminator
    buffer[2] = 0;
}

// Originally func_800A78E0
// MK 64 doesn't show more then 2 digits for any given time
// (minute, second, centisecond)
// So, if timeRecord is greater than 10 minutes, just use '-'s
void write_dashes(char* buffer) {
    buffer[0] = 0x2D;
    buffer[1] = 0x2D;
    buffer[2] = '\0';
}

// Originally func_800A78F4
// timeRecord is assumed to be measured in centiseconds
void get_time_record_minutes(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        write_dashes(buffer);
        return;
    }
    convert_number_to_ascii(timeRecord / 0x1770, buffer);
}

// Originally func_800A7944
// timeRecord is assumed to be measured in centiseconds
void get_time_record_seconds(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        write_dashes(buffer);
        return;
    }
    convert_number_to_ascii((timeRecord / 0x64) % 0x3C, buffer);
}

// Originally func_800A79A4
// timeRecord is assumed to be measured in centiseconds
void get_time_record_centiseconds(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        write_dashes(buffer);
        return;
    }
    convert_number_to_ascii(timeRecord % 0x64, buffer);
}

// Converts a 2-digit number to EUC-JP by the looks of it
void func_800A79F4(s32 arg0, char* arg1) {
    arg1[0] = 0xA3;
    arg1[1] = (arg0 / 0xA) - 0x50;
    arg1[2] = 0xA3;
    arg1[3] = (arg0 % 0xA) - 0x50;
    arg1[4] = '\0';
}

void func_800A7A4C(s32 arg0) {
    s32 var_s0;
    s32 var_v1;
    s32 var_v1_2;
    s32 type;
    struct_8018D9E0_entry* var_s1;
    s32 one = 1;

    for (var_v1_2 = 0; var_v1_2 < D_8018D9E0_SIZE; var_v1_2++) {
        var_v1 = 0;
        var_s1 = &D_8018D9E0[var_v1_2];
        type = var_s1->type;
        if ((type == 4) || (type == 5) || (type == 0x000000C7)) {
            if (arg0 != 0) {
                var_v1 = 1;
            }
        } else if (arg0 == 0) {
            var_v1 = 1;
        }

        if (var_v1 == 0) {
            continue;
        }

        switch (type) { /* switch 8; irregular */
            case 0xFA:  /* switch 8 */
                if (s8018ED94 < 0x50) {
                    D_800E8534 = 3.0f;
                } else if (s8018ED94 < 0x5A) {
                    if (D_800E8530 < 1.0) {
                        D_800E8530 += 0.1;
                    }
                    D_800E8534 += 0.1;
                } else if (s8018ED94 < 0xA0) {
                    D_800E8534 += 0.1;
                } else if (s8018ED94 < 0x190) {
                    D_800E8534 += 0.3;
                }
                D_8018EDCC -= D_800E8534;
                s8018ED94 += 1;
                if (D_8018EDCC < -360.0f) {
                    D_8018EDCC += 360.0f;
                }
                var_s1->unk1C++;
                if (var_s1->unk1C == 0x000000B4) {
                    func_8009E000(0x00000028);
                    func_800CA388(0x64U);
                    D_8018EDE0 = 0;
                }
                if ((var_s1->unk20 != 0) && (var_s1->unk1C >= 3)) {
                    var_s1->unk20 = 0;
                    play_sound2(SOUND_INTRO_LOGO);
                }
                break;
            case 0xDA: /* switch 8 */
                func_800A954C(var_s1);
                break;
            case 0xD6: /* switch 8 */
                func_800A9710(var_s1);
                break;
            case 0xD4: /* switch 8 */
                func_800A97BC(var_s1);
                break;
            case 0x5:                     /* switch 8 */
                switch (var_s1->cursor) { /* switch 9; irregular */
                    case 0:               /* switch 9 */
                        if (gControllerFive->button & R_TRIG) {
                            var_s1->cursor = (s32) 1U;
                            play_sound2(SOUND_ACTION_PING);
                        } else {
                            var_s1->visible = 0;
                        }
                        break;
                    case 1:  /* switch 9 */
                    default: /* switch 9 */
                        var_s1->visible = one;
                        break;
                }
                break;
            case 0xA: /* switch 8 */
                func_800AA280(var_s1);
                break;
            case 0x10:                             /* switch 8 */
            case 0x11:                             /* switch 8 */
                switch (gMainMenuSelectionDepth) { /* switch 5 */
                    case OPTIONS_SELECTION:        /* switch 5 */
                    case DATA_SELECTION:           /* switch 5 */
                    case PLAYER_NUM_SELECTION:     /* switch 5 */
                        func_800A9B9C(var_s1);
                        break;
                    case GAME_MODE_SELECTION:                           /* switch 5 */
                    case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION: /* switch 5 */
                    case CONFIRM_OK_SELECTION:                          /* switch 5 */
                    case CONFIRM_OK_SELECTION_FROM_BACK_OUT:            /* switch 5 */
                    case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT:      /* switch 5 */
                        func_800A9C40(var_s1);
                        break;
                }
                break;
            case 0xF: /* switch 8 */
                func_800AA280(var_s1);
                func_800A9A98(var_s1);
                break;
            case 0xB:                              /* switch 8 */
            case 0xC:                              /* switch 8 */
            case 0xD:                              /* switch 8 */
            case 0xE:                              /* switch 8 */
                switch (gMainMenuSelectionDepth) { /* switch 6 */
                    case OPTIONS_SELECTION:        /* switch 6 */
                    case DATA_SELECTION:           /* switch 6 */
                    case PLAYER_NUM_SELECTION:     /* switch 6 */
                        func_800A9B9C(var_s1);
                        break;
                    case GAME_MODE_SELECTION:                           /* switch 6 */
                    case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION: /* switch 6 */
                    case CONFIRM_OK_SELECTION:                          /* switch 6 */
                    case CONFIRM_OK_SELECTION_FROM_BACK_OUT:            /* switch 6 */
                    case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT:      /* switch 6 */
                        func_800A9C40(var_s1);
                        break;
                }
                func_800A9D5C(var_s1);
                break;
            case 0x12: /* switch 8 */
            case 0x13: /* switch 8 */
            case 0x14: /* switch 8 */
            case 0x15: /* switch 8 */
            case 0x16: /* switch 8 */
            case 0x17: /* switch 8 */
            case 0x18: /* switch 8 */
            case 0x19: /* switch 8 */
                func_800A9E58(var_s1);
                break;
            case 0x1B: /* switch 8 */
                func_800AA2EC(var_s1);
                break;
            case 0x34: /* switch 8 */
            case 0x35: /* switch 8 */
            case 0x36: /* switch 8 */
            case 0x37: /* switch 8 */
                func_800AADD4(var_s1);
                break;
            case 0x2B: /* switch 8 */
            case 0x2C: /* switch 8 */
            case 0x2D: /* switch 8 */
            case 0x2E: /* switch 8 */
            case 0x2F: /* switch 8 */
            case 0x30: /* switch 8 */
            case 0x31: /* switch 8 */
            case 0x32: /* switch 8 */
                func_800AAC18(var_s1);
                switch (var_s1->type) { /* switch 7 */
                    case 43:            /* switch 7 */
                    case 44:            /* switch 7 */
                    case 45:            /* switch 7 */
                    case 46:            /* switch 7 */
                    case 47:            /* switch 7 */
                    case 48:            /* switch 7 */
                    case 49:            /* switch 7 */
                    case 50:            /* switch 7 */
                        func_800AA69C(var_s1);
                        break;
                }
                switch (D_8018EDEE) { /* switch 10; irregular */
                    case 1:           /* switch 10 */
                        func_800AAB90(var_s1);
                        break;
                    case 2: /* switch 10 */
                    case 3: /* switch 10 */
                        func_800AAA9C(var_s1);
                        break;
                }
                func_800AAE18(var_s1);
                break;
            case 0x33: /* switch 8 */
            case 0x5D: /* switch 8 */
                func_800A9A98(var_s1);
                break;
            case 0x53: /* switch 8 */
            case 0x54: /* switch 8 */
            case 0x55: /* switch 8 */
            case 0x56: /* switch 8 */
                func_800AB164(var_s1);
                switch (D_8018EDEC) { /* switch 11; irregular */
                    case 1:           /* switch 11 */
                        func_800AB020(var_s1);
                        break;
                    case 2: /* switch 11 */
                    case 3: /* switch 11 */
                        func_800AB098(var_s1);
                        break;
                }
                break;
            case 0x58: /* switch 8 */
            case 0x59: /* switch 8 */
            case 0x5A: /* switch 8 */
            case 0x5B: /* switch 8 */
                func_800AB260(var_s1);
                break;
            case 0x64: /* switch 8 */
                func_800AB314(var_s1);
                break;
            case 0x5F: /* switch 8 */
            case 0x60: /* switch 8 */
            case 0x61: /* switch 8 */
            case 0x62: /* switch 8 */
                func_800AB290(var_s1);
                break;
            case 0x65: /* switch 8 */
            case 0x66: /* switch 8 */
                func_800AB904(var_s1);
                break;
            case 0x67: /* switch 8 */
                func_800AB9B0(var_s1);
                break;
            case 0x78: /* switch 8 */
            case 0x79: /* switch 8 */
            case 0x7A: /* switch 8 */
            case 0x7B: /* switch 8 */
            case 0x8C: /* switch 8 */
                func_800ABAE8(var_s1);
                break;
            case 0x8D: /* switch 8 */
                func_800ABB24(var_s1);
                break;
            case 0x7C: /* switch 8 */
            case 0x7D: /* switch 8 */
            case 0x7E: /* switch 8 */
            case 0x7F: /* switch 8 */
            case 0x80: /* switch 8 */
            case 0x81: /* switch 8 */
            case 0x82: /* switch 8 */
            case 0x83: /* switch 8 */
            case 0x84: /* switch 8 */
            case 0x85: /* switch 8 */
            case 0x86: /* switch 8 */
            case 0x87: /* switch 8 */
            case 0x88: /* switch 8 */
            case 0x89: /* switch 8 */
            case 0x8A: /* switch 8 */
            case 0x8B: /* switch 8 */
                func_800ABBCC(var_s1);
                break;
            case 0x96: /* switch 8 */
                func_800ABC38(var_s1);
                break;
            case 0x97: /* switch 8 */
                func_800ABEAC(var_s1);
                break;
            case 0x5E: /* switch 8 */
                func_800AC300(var_s1);
                break;
            case 0xAA: /* switch 8 */
                func_800AC324(var_s1);
                break;
            case 0xAB: /* switch 8 */
                func_800AC458(var_s1);
                break;
            case 0xAC: /* switch 8 */
                func_800ACA14(var_s1);
                break;
            case 0xAF: /* switch 8 */
                func_800AC978(var_s1);
                break;
            case 0xB0: /* switch 8 */
                func_800ACC50(var_s1);
                break;
            case 0xB1: /* switch 8 */
            case 0xB2: /* switch 8 */
            case 0xB3: /* switch 8 */
            case 0xB4: /* switch 8 */
                func_800ACF40(var_s1);
                break;
            case 0xB9: /* switch 8 */
                func_800AD1A4(var_s1);
                break;
            case 0xBA: /* switch 8 */
                func_800AD2E8(var_s1);
                break;
            case 0xBC: /* switch 8 */
                func_800AEC54(var_s1);
                break;
            case 0xC7: /* switch 8 */
                func_800ADF48(var_s1);
                break;
            case 0xBD: /* switch 8 */
                func_800AE218(var_s1);
                break;
            case 0xE6: /* switch 8 */
                func_800AEDBC(var_s1);
                break;
            case 0xE8: /* switch 8 */
                func_800AEE90(var_s1);
                break;
            case 0xE9: /* switch 8 */
                func_800AEEBC(var_s1);
                break;
            case 0xEA: /* switch 8 */
                func_800AEEE8(var_s1);
                break;
            case 0xBE: /* switch 8 */
                func_800AEF14(var_s1);
                break;
            case 0x10E: /* switch 8 */
                func_800AEF74(var_s1);
                break;
            case 0x12B: /* switch 8 */
                func_800AF004(var_s1);
                break;
            case 0x12C: /* switch 8 */
            case 0x12D: /* switch 8 */
            case 0x12E: /* switch 8 */
            case 0x12F: /* switch 8 */
                func_800AF1AC(var_s1);
                break;
            case 0x130: /* switch 8 */
                func_800AF270(var_s1);
                break;
            case 0x190: /* switch 8 */
            case 0x191: /* switch 8 */
            case 0x192: /* switch 8 */
            case 0x193: /* switch 8 */
            case 0x194: /* switch 8 */
            case 0x195: /* switch 8 */
            case 0x196: /* switch 8 */
            case 0x197: /* switch 8 */
            case 0x198: /* switch 8 */
            case 0x199: /* switch 8 */
            case 0x19A: /* switch 8 */
            case 0x19B: /* switch 8 */
            case 0x19C: /* switch 8 */
            case 0x19D: /* switch 8 */
            case 0x19E: /* switch 8 */
            case 0x19F: /* switch 8 */
            case 0x1A0: /* switch 8 */
            case 0x1A1: /* switch 8 */
            case 0x1A2: /* switch 8 */
            case 0x1A3: /* switch 8 */
            case 0x1A4: /* switch 8 */
            case 0x1A5: /* switch 8 */
            case 0x1A6: /* switch 8 */
            case 0x1A7: /* switch 8 */
            case 0x1A8: /* switch 8 */
            case 0x1A9: /* switch 8 */
            case 0x1AA: /* switch 8 */
            case 0x1AB: /* switch 8 */
            case 0x1AC: /* switch 8 */
            case 0x1AD: /* switch 8 */
            case 0x1AE: /* switch 8 */
            case 0x1AF: /* switch 8 */
            case 0x1B0: /* switch 8 */
            case 0x1B1: /* switch 8 */
            case 0x1B2: /* switch 8 */
            case 0x1B3: /* switch 8 */
            case 0x1B4: /* switch 8 */
            case 0x1B5: /* switch 8 */
            case 0x1B6: /* switch 8 */
            case 0x1B7: /* switch 8 */
            case 0x1B8: /* switch 8 */
            case 0x1B9: /* switch 8 */
            case 0x1BA: /* switch 8 */
            case 0x1BB: /* switch 8 */
            case 0x1BC: /* switch 8 */
            case 0x1BD: /* switch 8 */
            case 0x1BE: /* switch 8 */
            case 0x1BF: /* switch 8 */
            case 0x1C0: /* switch 8 */
            case 0x1C1: /* switch 8 */
            case 0x1C2: /* switch 8 */
            case 0x1C3: /* switch 8 */
            case 0x1C4: /* switch 8 */
            case 0x1C5: /* switch 8 */
            case 0x1C6: /* switch 8 */
            case 0x1C7: /* switch 8 */
            case 0x1C8: /* switch 8 */
            case 0x1C9: /* switch 8 */
            case 0x1CA: /* switch 8 */
            case 0x1CB: /* switch 8 */
            case 0x1CC: /* switch 8 */
            case 0x1CD: /* switch 8 */
            case 0x1CE: /* switch 8 */
                func_800AF480(var_s1);
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                break;
        }
    }

    for (var_s0 = 0; var_s0 < 0x10; var_s0++) {
        for (var_v1_2 = 0; var_v1_2 < D_8018D9E0_SIZE; var_v1_2++) {
            var_v1 = 0;
            var_s1 = &D_8018D9E0[var_v1_2];
            if (var_s1 && var_s1) {} // ?
            type = var_s1->type;
            if ((type == 4) || (type == 5) || (type == 0x000000C7)) {
                if (arg0 != 0) {
                    var_v1 = 1;
                }
            } else if (arg0 == 0) {
                var_v1 = 1;
            }
            if ((var_v1 != 0) && (var_s0 == (s8) var_s1->priority)) {
                func_8009F5E0(var_s1);
            }
        }
    }
}

void func_800A8230(void) {
    func_800A7A4C(0);
}

void func_800A8250(void) {
    func_800A7A4C(1);
}

void func_800A8270(s32 arg0, struct_8018D9E0_entry* arg1) {
    s32 temp_t1;
    s32 temp_t6;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;

    if (arg1->unk1C < 0x20) {
        temp_t6 = (arg1->unk1C << 6) / 64;
        temp_t1 = arg1->column;
        var_s0 = arg1->row;
        var_s3 = temp_t1 + temp_t6;
        var_s4 = (temp_t1 - temp_t6) + 0x3F;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if ((arg0 + 1) == gPlayerCount) {
            if ((gMainMenuSelectionDepth == OPTIONS_SELECTION) || (gMainMenuSelectionDepth == DATA_SELECTION) ||
                (gMainMenuSelectionDepth == PLAYER_NUM_SELECTION)) {
                gDisplayListHead = draw_flash_select_case_slow(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
            } else {
                gDisplayListHead = draw_box_fill(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35, 0x000000FF,
                                                 0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            gDisplayListHead = func_80098FC8(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
        }
        for (var_s0 += 0x41, var_s2 = 0; var_s2 <= D_800F2B60[0][arg0]; var_s2++, var_s0 += 0x12) {
            if ((var_s2 == D_800E86AC[arg0]) && ((arg0 + 1) == gPlayerCount) && (gMainMenuSelectionDepth >= 4)) {
                if (gMainMenuSelectionDepth == GAME_MODE_SELECTION) {
                    gDisplayListHead =
                        draw_flash_select_case_slow(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11);
                } else {
                    gDisplayListHead = draw_box_fill(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11,
                                                     0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                }
            } else {
                gDisplayListHead =
                    draw_box_fill(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11, 1, 1, 1, 0x000000FF);
            }
        }
    }
}

void func_800A8564(struct_8018D9E0_entry* arg0) {
    s32 sp34;
    s32 var_a1;
    MkTexture* var_a0;
    s32 temp_a2;
    s32 temp_t0;

    var_a1 = 0;
    switch (arg0->type) { /* irregular */
        case 0xF:
            var_a0 = D_0200487C;
            if ((gMainMenuSelectionDepth == CONFIRM_OK_SELECTION) ||
                (gMainMenuSelectionDepth == CONFIRM_OK_SELECTION_FROM_BACK_OUT)) {
                var_a1 = 1;
            }
            break;
        case 0x33:
            var_a0 = D_02004B74;
            if ((D_8018EDEE == 2) || (D_8018EDEE == 3)) {
                var_a1 = 1;
            }
            break;
        case 0x5D:
            var_a0 = D_02004E80;
            if (D_8018EDEC == 3) {
                var_a1 = 1;
            }
            break;
        default:
            return;
    }
    if (arg0->unk1C < 0x20) {
        sp34 = (arg0->unk1C << 5) / 64;
        var_a0 = segmented_to_virtual_dupe(var_a0);
        temp_t0 = arg0->column + var_a0->dX;
        temp_a2 = arg0->row + var_a0->dY;
        if (var_a1 != 0) {
            gDisplayListHead = draw_flash_select_case_slow(gDisplayListHead, temp_t0 + sp34, temp_a2,
                                                           (temp_t0 - sp34) + 0x1E, temp_a2 + 0x12);
        } else {
            gDisplayListHead = draw_box_fill(gDisplayListHead, temp_t0 + sp34, temp_a2, (temp_t0 - sp34) + 0x1E,
                                             temp_a2 + 0x12, 1, 1, 1, 0x000000FF);
        }
    }
}

void func_800A86E8(struct_8018D9E0_entry* arg0) {
    gDisplayListHead =
        draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x64, arg0->row + 0x27, 1, 1, 1, 0xFF);
}

// Credit for the use of ternary operators goes to LLONSIT
void func_800A874C(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    char sp58[3];
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    s32 temp_s1;
    UNUSED u32 var_v0;
    u32 var_s2;
    set_text_color(1);
    var_s2 = arg0->type == 0x00000065 ? func_800B4E24(0) : func_800B4F2C();
    temp_s1 = var_s2 & 0xFFFFF;
    get_time_record_minutes((temp_s1 ^ 0), sp58);
    text_draw(arg0->column + 5, arg0->row + 0x21, sp58, 0, 0.6f, 0.65f);
    func_80093324(arg0->column + 0xE, arg0->row + 0x21, "'", 0, 0.6f, 0.65f);
    get_time_record_seconds(temp_s1, sp58);
    text_draw(arg0->column + 0x16, arg0->row + 0x21, sp58, 0, 0.6f, 0.65f);
    func_80093324(arg0->column + 0x20, arg0->row + 0x21, "\"", 0, 0.6f, 0.65f);
    get_time_record_centiseconds(temp_s1, sp58);
    text_draw(arg0->column + 0x29, arg0->row + 0x21, sp58, 0, 0.6f, 0.65f);
    var_s2 = (u32) temp_s1 < 0x927C0U ? var_s2 >> 0x14 : 8;
    func_800936B8(arg0->column + 0x60, arg0->row + 0x21, D_800E76A8[var_s2], 0, 0.6f, 0.65f);
}

void func_800A890C(s32 arg0, struct_8018D9E0_entry* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->unk1C < 32) {
        if (1) {}
        temp_t7 = (arg1->unk1C * 65) / 64;
        temp_t1 = arg1->column;
        temp_a2 = arg1->row;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if (arg0 == gCupSelection) {
            if (D_8018EDEC == 1) {
                gDisplayListHead = draw_flash_select_case_slow(gDisplayListHead, temp_t1 + temp_t7, temp_a2,
                                                               (temp_t1 - temp_t7) + 64, temp_a2 + 39);
            } else {
                gDisplayListHead = draw_box_fill(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                                 temp_a2 + 39, 255, 249, 220, 255);
            }
        } else {
            gDisplayListHead = draw_box_fill(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                             temp_a2 + 39, 1, 1, 1, 255);
        }
    }
}

void func_800A8A98(struct_8018D9E0_entry* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 someIndex;

    temp_s2 = arg0->column;
    temp_s3 = arg0->row;
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    for (someIndex = 0; someIndex < 4; someIndex++) {
        if ((someIndex == gCupCourseSelection) && (D_8018EDEC >= 2) && (gModeSelection != GRAND_PRIX)) {
            if ((D_8018EDEC == 2) || (D_8018EDEC == 4)) {
                gDisplayListHead = draw_flash_select_case_slow(
                    gDisplayListHead, D_800E7208[someIndex][0].column + temp_s2, D_800E7208[someIndex][0].row + temp_s3,
                    D_800E7208[someIndex][1].column + temp_s2, D_800E7208[someIndex][1].row + temp_s3);
            } else {
                gDisplayListHead = draw_box_fill(
                    gDisplayListHead, D_800E7208[someIndex][0].column + temp_s2, D_800E7208[someIndex][0].row + temp_s3,
                    D_800E7208[someIndex][1].column + temp_s2, D_800E7208[someIndex][1].row + temp_s3, 0x000000FF,
                    0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            gDisplayListHead = draw_box_fill(
                gDisplayListHead, D_800E7208[someIndex][0].column + temp_s2, D_800E7208[someIndex][0].row + temp_s3,
                D_800E7208[someIndex][1].column + temp_s2, D_800E7208[someIndex][1].row + temp_s3, 1, 1, 1, 0x000000FF);
        }
    }
}

void func_800A8CA4(struct_8018D9E0_entry* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 var_s0;
    struct_8018D9E0_entry* temp_v0;

    temp_v0 = find_8018D9E0_entry_dupe(0x00000064);
    temp_s2 = arg0->column;
    temp_s3 = arg0->row;
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    if (gModeSelection == GRAND_PRIX) {
        if (D_8018EDEC != 3) {
            for (var_s0 = 0; var_s0 < 4; var_s0++) {
                // Wut?
                if ((var_s0 != (temp_v0->unk1C % 4)) != 0) {
                    gDisplayListHead =
                        draw_box(gDisplayListHead, D_800E7208[var_s0][0].column + temp_s2,
                                 D_800E7208[var_s0][0].row + temp_s3, D_800E7208[var_s0][1].column + temp_s2,
                                 D_800E7208[var_s0][1].row + temp_s3, 0, 0, 0, 0x00000064);
                }
            }
        }
    }
}

void func_800A8E14(UNUSED struct_8018D9E0_entry* arg0) {
    set_text_color(TEXT_YELLOW);
    draw_text(0x98, 0x44, D_800E77A8[0], 0, 1.0f, 1.0f);
    func_80093324(0x17, 0x58, D_800E77A8[1], 0, 0.7f, 0.8f);
    func_80093324(0x17, 0x6A, D_800E77A8[2], 0, 0.7f, 0.8f);
}

void func_800A8EC0(struct_8018D9E0_entry* arg0) {
    if (arg0->unk20 != 0) {
        func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, -1);
        set_text_color(TEXT_YELLOW);
        func_80093324(arg0->column + 0x20, arg0->row + 0x28, D_800E7678[arg0->unk20], 0, 0.7f, 0.7f);
    }
}

void func_800A8F48(UNUSED struct_8018D9E0_entry* arg0) {
    UNUSED Gfx* temp_v0_2;
    Unk_D_800E70A0* temp_v0;
    s16 temp_s0;
    s16 temp_v1;
    s32 temp_s2;
    s32 var_s1;

    switch (D_8018EDEC) { /* irregular */
        case 1:
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                if (func_800B639C((gCupSelection * 4) + var_s1) >= 0) {
                    temp_v0 = &D_800E7168[var_s1];
                    temp_v1 = temp_v0->column;
                    temp_s0 = temp_v0->row;
                    temp_s2 = temp_v1 + 0x20;
                    gDisplayListHead =
                        func_80098FC8(gDisplayListHead, temp_s2, (s32) temp_s0, temp_v1 + 0x3F, temp_s0 + 9);
                    gDisplayListHead = func_8009C204(gDisplayListHead, segmented_to_virtual_dupe(D_02004A0C), temp_s2,
                                                     (s32) temp_s0, 2);
                }
            }
            break;
        case 2:
        default:
            if (func_800B639C((gCupSelection * 4) + gCupCourseSelection) >= 0) {
                gDisplayListHead = func_80098FC8(gDisplayListHead, 0x00000057, 0x00000070, 0x00000096, 0x00000081);
                gDisplayListHead = func_8009BA74(gDisplayListHead, D_02004A0C, 0x00000057, 0x00000070);
            }
            break;
    }
}

void func_800A90D4(UNUSED s32 arg0, struct_8018D9E0_entry* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->unk1C < 0x20) {
        if (1) {}
        temp_t7 = (arg1->unk1C * 0x41) / 0x40;
        temp_t1 = arg1->column;
        temp_a2 = arg1->row;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDisplayListHead = draw_box_fill(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 0x40,
                                         temp_a2 + 0x27, 1, 1, 1, 0xFF);
    }
}

void func_800A91D8(struct_8018D9E0_entry* arg0, s32 columnTarget, s32 rowTarget) {
    func_800A9208(arg0, columnTarget);
    func_800A9278(arg0, rowTarget);
}

void func_800A9208(struct_8018D9E0_entry* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->column;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->column += step;
}

void func_800A9278(struct_8018D9E0_entry* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->row;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->row += step;
}

void func_800A92E8(struct_8018D9E0_entry* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->column;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
            if (step < 8) {
                step = 8;
            }
            arg0->column += step;
            if (columnTarget < arg0->column) {
                arg0->column = columnTarget;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
            if (step < -8) {
                step = -8;
            }
            arg0->column += step;
            if (arg0->column < columnTarget) {
                arg0->column = columnTarget;
            }
        }
    }
}

// Appears to be a copy of func_800A9278
UNUSED void func_800A939C(struct_8018D9E0_entry* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->row;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->row += step;
}

void func_800A940C(struct_8018D9E0_entry* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->column;

    if (step != 0) {
        step = 0xC8 / step;
        if (step > 0) {
            if (step >= 0x19) {
                step = 0x18;
            }
            if (step < 0x10) {
                step = 0x10;
            }
            arg0->column += step;
            if (columnTarget < arg0->column) {
                arg0->column = columnTarget;
            }
        } else {
            if (step < -0x18) {
                step = -0x18;
            }
            if (step >= -0xF) {
                step = -0x10;
            }
            arg0->column += step;
            if (arg0->column < columnTarget) {
                arg0->column = columnTarget;
            }
        }
    }
}

void func_800A94C8(struct_8018D9E0_entry* arg0, s32 columnTarget, s32 arg2) {
    s32 step;

    if (columnTarget == arg0->column) {
        arg0->column += arg2;
    } else {
        step = columnTarget - arg0->column;
        if (step != 0) {
            if (step > 0) {
                step = (step / 4) + 1;
                if (step >= 0x11) {
                    step = 0x10;
                }
            } else {
                step = (step / 4) - 1;
                if (step < -0x10) {
                    step = -0x10;
                }
            }
        }
        arg0->column -= step;
    }
}

void func_800A954C(struct_8018D9E0_entry* arg0) {
    // Cycle lasts 26 (0x1A) frames
    if (arg0->cursor == 0) {
        // Move highlight from yellow to red
        arg0->unk20 = (s32) (u32) ((((f32) (0xC - arg0->unk1C) * 127.0f) / 12.0f) + 128.0f);
    } else {
        // Move highlight from red to  yellow
        arg0->unk20 = (s32) (u32) ((((f64) (f32) arg0->unk1C * 127.0) / 12.0) + 128.0);
    }
    arg0->unk1C++;
    if (arg0->unk1C >= 0xC) {
        // Every 13 (0xC) frames reverse direction
        arg0->unk1C = 0;
        arg0->cursor ^= 1;
    }
}

void func_800A9710(struct_8018D9E0_entry* arg0) {
    s32 phi_v0;

    switch (gControllerPakMenuSelection) {
        case CONTROLLER_PAK_MENU_ERASE:
        case CONTROLLER_PAK_MENU_QUIT:
            phi_v0 = 1;
            break;
        case CONTROLLER_PAK_MENU_GO_TO_ERASING:
        case CONTROLLER_PAK_MENU_ERASING:
            phi_v0 = 5;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            phi_v0 = 2;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            phi_v0 = 3;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
            phi_v0 = 4;
            break;
        default:
            phi_v0 = 0;
            break;
    }
    if (phi_v0 != arg0->cursor) {
        arg0->cursor = phi_v0;
        func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E7D34[phi_v0]));
    }
}

void func_800A97BC(struct_8018D9E0_entry* arg0) {
    s32 i;

    switch (*D_800E86D0) {
        case 1:
            arg0->row -= 2;
            if (arg0->row < 0x60) {
                arg0->row = 0x69;
                *D_800E86D0 = 0;
                for (i = 0; i < 8; i++) {
                    if (i < 7) {
                        D_800E86C4[i] = D_800E86C4[i + 1];
                    } else {
                        if ((D_800E86C4[i - 1] == 0x10) || (D_800E86C4[i - 1] == 0)) {
                            D_800E86C4[i] = 0;
                        } else {
                            D_800E86C4[i] = D_800E86C4[i - 1] + 1;
                        }
                    }
                }
                D_800E86C4[0] = D_800E86C4[8] = 0;
            }

            break;
        case 2:
            arg0->row += 2;
            if (arg0->row >= 0x73) {
                arg0->row = 0x69;
                *D_800E86D0 = 0;
                for (i = 8; i > 0; i--) {
                    if (i > 1) {
                        D_800E86C4[i] = D_800E86C4[i - 1];
                    } else {
                        if ((D_800E86C4[i + 1] == 1) || (D_800E86C4[i + 1] == 0)) {
                            D_800E86C4[i] = 0;
                        } else {
                            D_800E86C4[i] = D_800E86C4[i + 1] - 1;
                        }
                    }
                }
                D_800E86C4[0] = D_800E86C4[8] = 0;
                break;
                default:
                    *D_800E86D0 = 0;
                    break;
            }
    }
} // todo: <-- fix these brackets

// Don't know if this belongs here or inside func_800A9A98
//   as a `static const`. But this matches so we'll leave
//   it here for now.
const s8 D_800F0CA0[] = {
    0x03, 0x03, 0x03, 0x02, 0x00, 0x02, 0x02, 0x01, 0x03, 0x02, 0x00, 0x00, 0x03, 0x03, 0x02, 0x00
};

void func_800A9A98(struct_8018D9E0_entry* arg0) {
    s32 sp4;
    s32 var_v0;

    switch (arg0->type) { /* irregular */
        default:
            var_v0 = sp4; // wut?
            break;
        case 0xF:
            var_v0 = D_800F0CA0[gMainMenuSelectionDepth - 1];
            break;
        case 0x5D:
            var_v0 = D_800F0CA0[D_8018EDEC + 11];
            break;
        case 0x33:
            var_v0 = D_800F0CA0[D_8018EDEE + 7];
            break;
    }
    switch (var_v0) { /* switch 1; irregular */
        case 0:       /* switch 1 */
            arg0->unk1C = 0;
            break;
        case 1: /* switch 1 */
            arg0->unk1C = 0x00000020;
            break;
        case 2: /* switch 1 */
            if (arg0->unk1C > 0) {
                arg0->unk1C = (arg0->unk1C - (arg0->unk1C / 12)) - 2;
                if (arg0->unk1C < 0) {
                    arg0->unk1C = 0;
                }
            }
            break;
        case 3: /* switch 1 */
            if (arg0->unk1C < 0x20) {
                arg0->unk1C += 2;
                if (arg0->unk1C >= 0x20) {
                    arg0->unk1C = 0x00000020;
                }
            }
            break;
    }
}

void func_800A9B9C(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            func_800AA280(arg0);
            break;
        case 1:
            func_800AA280(arg0);
            arg0->cursor = 4;
            /* fallthrough */
        case 4:
            if (arg0->unk1C > 0) {
                arg0->unk1C = (arg0->unk1C - (arg0->unk1C / 12)) - 2;
                if (arg0->unk1C < 0) {
                    arg0->unk1C = 0;
                }
            } else {
                arg0->unk1C = 0;
                arg0->cursor = 0;
            }
            break;
        case 2:
            arg0->cursor = 3;
            break;
        case 3:
        default:
            break;
    }
}

void func_800A9C40(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            func_800AA280(arg0);
            if ((gPlayerCount + 0xA) == arg0->type) {
                arg0->cursor = 2;
            } else {
                arg0->cursor = 1;
            }
            break;
        case 4:
            if ((gPlayerCount + 0xA) == arg0->type) {
                arg0->cursor = 2;
                arg0->unk1C = 0;
                break;
            }
            arg0->cursor = 1;
        case 1:
            func_800AA280(arg0);
            if ((gMainMenuSelectionDepth == CONFIRM_OK_SELECTION_FROM_BACK_OUT) ||
                (gMainMenuSelectionDepth == TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT)) {
                arg0->unk1C = 0x00000020;
            } else {
                if (arg0->unk1C < 0x20) {
                    arg0->unk1C += 2;
                    if (arg0->unk1C >= 0x20) {
                        arg0->unk1C = 0x00000020;
                    }
                }
            }
            break;
        case 3:
            if ((gPlayerCount + 0xA) == arg0->type) {
                arg0->cursor = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void func_800A9D5C(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v0;

    if ((gPlayerCount + 0xA) == arg0->type) {
        arg0->priority = 0x0A;
    } else {
        arg0->priority = 6;
    }

    switch (arg0->cursor) {
        case 2:
            if ((gMainMenuSelectionDepth == CONFIRM_OK_SELECTION_FROM_BACK_OUT) ||
                (gMainMenuSelectionDepth == TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT)) {
                arg0->column = 0x00000080;
                arg0->row = 0x0000003E;
            } else {
                func_800A91D8(arg0, 0x00000080, 0x0000003E);
            }
            break;
        case 3:
            temp_v0 = &D_800E70A0[arg0->type - 0xA];
            func_800A91D8(arg0, temp_v0->column, temp_v0->row);
            if ((arg0->column == temp_v0->column) && (arg0->row == temp_v0->row)) {
                arg0->cursor = 0;
            }
            /* fallthrough */
        case 0:
        case 1:
        case 4:
        default:
            break;
    }
}

void func_800A9E58(struct_8018D9E0_entry* arg0) {
    struct_8018D9E0_entry* temp_v0;
    Unk_D_800E70A0* temp_v1_2;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 temp_a1;

    switch (arg0->type) { /* switch 1 */
        case 18:          /* switch 1 */
        case 19:          /* switch 1 */
        case 20:          /* switch 1 */
        case 21:          /* switch 1 */
            sp24 = 18;
            sp1C = 2;
            sp20 = 0;
            break;
        case 22: /* switch 1 */
        case 23: /* switch 1 */
            sp24 = 22;
            sp20 = 2;
            sp1C = 2;
            break;
        case 24: /* switch 1 */
        case 25: /* switch 1 */
            sp24 = 24;
            sp20 = 1;
            sp1C = 1;
            break;
    }

    temp_a1 = gGameModeFromNumPlayersAndRowSelection[gPlayerCount][D_800E86AC[gPlayerCount - 1]];
    switch (arg0->cursor) { /* switch 5; irregular */
        case 0:             /* switch 5 */
            if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                arg0->visible = 0;
            } else {
                arg0->unk20 = D_800E86AC[gPlayerCount - 1];
                switch (gMainMenuSelectionDepth) {                      /* switch 2 */
                    case GAME_MODE_SELECTION:                           /* switch 2 */
                    case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION: /* switch 2 */
                    case CONFIRM_OK_SELECTION:                          /* switch 2 */
                        arg0->visible = 1;
                        temp_v0 = func_800AAE68();
                        arg0->column = temp_v0->column;
                        arg0->row = (D_800E86AC[gPlayerCount - 1] * 0x12) + temp_v0->row + 0x41;
                        arg0->unk1C = 0;
                        arg0->cursor = 1;
                        break;
                    case CONFIRM_OK_SELECTION_FROM_BACK_OUT:       /* switch 2 */
                    case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT: /* switch 2 */
                        temp_v0 = func_800AAE68();
                        temp_v1_2 = &D_800E70E8[arg0->type - sp24];
                        arg0->column = temp_v0->column + temp_v1_2->column;
                        arg0->row = (D_800E86AC[gPlayerCount - 1] * 0x12) + temp_v0->row + temp_v1_2->row;
                        arg0->unk1C = arg0->row - temp_v0->row;
                        arg0->visible = 1;
                        arg0->cursor = 2;
                        break;
                    default: /* switch 2 */
                        arg0->visible = 0;
                        break;
                }
            }
            break;
        case 1:                                                     /* switch 5 */
            switch (gMainMenuSelectionDepth) {                      /* switch 3 */
                case GAME_MODE_SELECTION:                           /* switch 3 */
                case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION: /* switch 3 */
                case CONFIRM_OK_SELECTION:                          /* switch 3 */
                case CONFIRM_OK_SELECTION_FROM_BACK_OUT:            /* switch 3 */
                case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT:      /* switch 3 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->visible = 0;
                        arg0->cursor = 0;
                    } else {
                        if (arg0->unk20 != D_800E86AC[gPlayerCount - 1]) {
                            arg0->cursor = 0;
                        }
                        temp_v0 = func_800AAE68();
                        arg0->column = temp_v0->column + arg0->unk1C;
                        temp_v1_2 = &D_800E70E8[arg0->type - sp24];
                        func_800A92E8(arg0, temp_v0->column + temp_v1_2->column);
                        arg0->unk1C = arg0->column - temp_v0->column;
                        if (arg0->unk1C == temp_v1_2->column) {
                            arg0->cursor = 2;
                            arg0->unk1C = arg0->row - temp_v0->row;
                        }
                    }
                    break;
                default: /* switch 3 */
                    arg0->visible = 0;
                    arg0->cursor = 0;
                    break;
            }
            break;
        case 2:                                                     /* switch 5 */
            switch (gMainMenuSelectionDepth) {                      /* switch 4 */
                case GAME_MODE_SELECTION:                           /* switch 4 */
                case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION: /* switch 4 */
                case CONFIRM_OK_SELECTION:                          /* switch 4 */
                case CONFIRM_OK_SELECTION_FROM_BACK_OUT:            /* switch 4 */
                case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT:      /* switch 4 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->visible = 0;
                        arg0->cursor = 0;
                    } else {
                        if (arg0->unk20 != D_800E86AC[gPlayerCount - 1]) {
                            arg0->cursor = 0;
                        }
                        temp_v0 = func_800AAE68();
                        temp_v1_2 = &D_800E70E8[arg0->type - sp24];
                        arg0->column = temp_v0->column + temp_v1_2->column;
                        arg0->row = temp_v0->row + arg0->unk1C;
                        func_800A91D8(arg0, arg0->column,
                                      (D_800E86AC[gPlayerCount - 1] * 0x12) + temp_v0->row + temp_v1_2->row);
                        arg0->unk1C = arg0->row - temp_v0->row;
                    }
                    break;
                default: /* switch 4 */
                    arg0->visible = 0;
                    arg0->cursor = 0;
                    break;
            }
            break;
    }
}

void func_800AA280(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v0;

    temp_v0 = &D_800E70A0[arg0->type - 0xA];
    if ((gMainMenuSelectionDepth == CONFIRM_OK_SELECTION_FROM_BACK_OUT) ||
        (gMainMenuSelectionDepth == TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT)) {
        arg0->column = temp_v0->column;
        arg0->row = temp_v0->row;
    } else {
        func_800A91D8(arg0, temp_v0->column, temp_v0->row);
    }
}

void func_800AA2EC(struct_8018D9E0_entry* arg0) {
    s32 temp_v0;
    s32 var_t1;

    var_t1 = 0;
    switch (gMainMenuSelectionDepth) {
        case OPTIONS_SELECTION:
        case DATA_SELECTION:
        case PLAYER_NUM_SELECTION:
        case GAME_MODE_SELECTION:
            arg0->cursor = 0;
            break;
        case GAME_MODE_CC_OR_TIME_TRIALS_OPTIONS_SELECTION:
        case CONFIRM_OK_SELECTION:
        case CONFIRM_OK_SELECTION_FROM_BACK_OUT:
        case TIME_TRIALS_DATA_SELECTION_FROM_BACK_OUT:
            if (arg0->cursor != 0) {
                break;
            }
            if (gPlayerCount != 1) {
                break;
            }
            if (D_800E86AC[gPlayerCount - 1] != 1) {
                break;
            }

            if (gControllerPak1State != 0) {
                var_t1 = 0;
                switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                      (u8*) gExtCode, &gControllerPak1FileNote)) { /* switch 1; irregular */
                    case 5:                                                        /* switch 1 */
                        break;
                    case 0: /* switch 1 */
                        arg0->cursor = 1;
                        var_t1 = 1;
                        break;
                    case 2: /* switch 1 */
                        gControllerPak1State = 0;
                        break;
                    default: /* switch 1 */
                        gControllerPak1State = 0;
                        break;
                }
            }
            if (var_t1 == 0) {
                if (gControllerPak1State == 0) {
                    if (check_for_controller_pak(0) == 0) {
                        arg0->cursor = 2;
                        break;
                    }
                    temp_v0 = osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, 0);
                    if (temp_v0 != 0) {
                        switch (temp_v0) {
                            case PFS_ERR_NOPACK:
                            case PFS_ERR_DEVICE:
                                arg0->cursor = 2;
                                break;
                            case PFS_ERR_ID_FATAL:
                                arg0->cursor = 3;
                                break;
                            case PFS_ERR_CONTRFAIL:
                            default:
                                arg0->cursor = 3;
                                break;
                        }
                        return;
                    } else {
                        gControllerPak1State = 1;
                    }
                    if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                      (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                        arg0->cursor = 1;
                        break;
                    }
                    if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed,
                                      &gControllerPak1MaxWriteableFiles) != 0) {
                        arg0->cursor = 3;
                        break;
                    }
                    if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree) != 0) {
                        arg0->cursor = 3;
                        break;
                    }
                    gControllerPak1NumPagesFree = (s32) gControllerPak1NumPagesFree >> 8;
                }
                if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                    arg0->cursor = 5;
                    break;
                }
                if (gControllerPak1NumPagesFree >= 0x79) {
                    arg0->cursor = 1;
                    break;
                }
                arg0->cursor = 5;
            }
            break;
        default:
            break;
    }
}

void func_800AA5C8(struct_8018D9E0_entry* arg0, s8 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->type - 0x2B;
    switch (arg0->unk8) { /* irregular */
        case 0:
            if (func_800AAFCC((s32) arg1) >= 0) {
                arg0->unk8 = 2;
                func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E8340[temp_v1]));
            }
            break;
        case 2:
            if (func_800AAFCC((s32) arg1) < 0) {
                arg0->unk8 = 0;
                func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E8320[temp_v1]));
            }
            break;
    }
}

void func_800AA69C(struct_8018D9E0_entry* arg0) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 temp_a0;
    UNUSED s32 stackPadding0;

    temp_a0 = arg0->type - 0x2B;
    temp_v0 = func_800AAFCC(arg0->type - 0x2B);
    if (temp_v0 >= 0) {
        var_a0 = 1;
    } else {
        var_a0 = 0;
    }
    switch (arg0->unk8) {
        case 0:
            if ((D_8018EDE8[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->unk8 = 1;
                func_8009A594(arg0->D_8018DEE0_index, 0,
                              segmented_to_virtual_dupe_2(gCharacterCelebrateAnimation[temp_a0]));
            } else {
                temp_v0 = random_int(0x00C8U);
                if (temp_v0 >= 0xC6) {
                    arg0->unk8 = 4;
                    func_8009A594(arg0->D_8018DEE0_index, 0,
                                  segmented_to_virtual_dupe_2(gCharacterSingleBlinkAnimation[temp_a0]));
                } else if (temp_v0 >= 0xC5) {
                    arg0->unk8 = 5;
                    func_8009A594(arg0->D_8018DEE0_index, 0,
                                  segmented_to_virtual_dupe_2(gCharacterDoubleBlinkAnimation[temp_a0]));
                }
            }
            break;
        case 1:
            if (D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex >= D_800E8440[temp_a0]) {
                arg0->unk8 = 2;
                func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E83A0[temp_a0]));
            } else if ((D_8018EDE8[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->unk8 = 3;
                func_8009A594(arg0->D_8018DEE0_index,
                              D_800E8460[temp_a0] - D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex,
                              segmented_to_virtual_dupe_2(gCharacterDeselectAnimation[temp_a0]));
            }
            break;
        case 2:
            if ((D_8018EDE8[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->unk8 = 3;
                func_8009A594(arg0->D_8018DEE0_index, 0,
                              segmented_to_virtual_dupe_2(gCharacterDeselectAnimation[temp_a0]));
            }
            break;
        case 3:
            if (D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex >= D_800E8460[temp_a0]) {
                arg0->unk8 = 0;
                func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E8360[temp_a0]));
            } else if ((D_8018EDE8[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->unk8 = 1;
                func_8009A594(arg0->D_8018DEE0_index,
                              D_800E8460[temp_a0] - D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex,
                              segmented_to_virtual_dupe_2(gCharacterCelebrateAnimation[temp_a0]));
            }
            break;
        case 4:
        case 5:
            if ((D_8018EDE8[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->unk8 = 1;
                func_8009A594(arg0->D_8018DEE0_index, 0,
                              segmented_to_virtual_dupe_2(gCharacterCelebrateAnimation[temp_a0]));
            } else {
                if (arg0->unk8 == 4) {
                    var_v0 = D_800E8480[temp_a0];
                } else {
                    var_v0 = D_800E84A0[temp_a0];
                }
                if (D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex >= var_v0) {
                    arg0->unk8 = 0;
                    func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E8360[temp_a0]));
                }
            }
            break;
        default:
            break;
    }
}

void func_800AAA9C(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 3:
            arg0->cursor = 1;
            /* fallthrough */
        case 1:
            if (D_8018EDEE == 3) {
                arg0->unk1C = 0x00000020;
            } else {
                if (arg0->unk1C < 0x20) {
                    arg0->unk1C += (arg0->unk1C / 12) + 2;
                    if (arg0->unk1C >= 0x20) {
                        arg0->unk1C = 0x00000020;
                    }
                }
            }
            /* fallthrough */
        case 0:
            if (func_800AAFCC(arg0->type - 0x2B) >= 0) {
                arg0->cursor = 2;
                arg0->unk1C = 0;
            } else {
                arg0->cursor = 1;
            }
            break;
        case 4:
            if (func_800AAFCC(arg0->type - 0x2B) >= 0) {
                arg0->cursor = 2;
                arg0->unk1C = 0;
            }
            break;
        case 2:
        default:
            break;
    }
}

void func_800AAB90(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 1:
            if (arg0->unk1C > 0) {
                arg0->cursor = 3;
            }
            break;
        case 2:
            arg0->cursor = 4;
            break;
        case 3:
            if (arg0->unk1C > 0) {
                arg0->unk1C = (arg0->unk1C - (arg0->unk1C / 12)) - 2;
                if (arg0->unk1C < 0) {
                    arg0->unk1C = 0;
                }
            } else {
                arg0->unk1C = 0;
                arg0->cursor = 0;
            }
            break;
        case 0:
        case 4:
        default:
            break;
    }
}

void func_800AAC18(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 temp_a1;
    s32 temp_v0;
    UNUSED s32 stackPadding2;
    Unk_D_800E70A0* var_t0;

    temp_a1 = arg0->type - 0x2B;
    switch (arg0->cursor) {
        case 0:
            if (D_8018EDEE == 3) {
                temp_v0 = func_800AAFCC(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &D_800E7188[(gScreenModeSelection * 4) + temp_v0];
                    arg0->column = (s32) var_t0->column;
                    arg0->row = (s32) var_t0->row;
                    arg0->cursor = 2;
                    arg0->unk8 = 2;
                    func_8009A594(arg0->D_8018DEE0_index, 0, segmented_to_virtual_dupe_2(D_800E8340[temp_a1]));
                }
                break;
            }
        case 1:
        case 3:
            var_t0 = &D_800E7108[0][temp_a1];
            arg0->column = (s32) var_t0->column;
            arg0->row = (s32) var_t0->row;
            break;
        case 2:
        case 4:
            if (arg0->cursor == 2) {
                temp_v0 = func_800AAFCC(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &D_800E7188[(gScreenModeSelection * 4) + temp_v0];
                }
            } else {
                var_t0 = &D_800E7108[0][temp_a1];
                if ((var_t0->column == arg0->column) && (var_t0->row == arg0->row)) {
                    arg0->cursor = 0;
                    return;
                }
            }
            if ((arg0->cursor != 2) || (arg0->unk8 != 1)) {
                func_800A91D8(arg0, (s32) var_t0->column, (s32) var_t0->row);
            }
            break;
        default:
            break;
    }
}

void func_800AADD4(struct_8018D9E0_entry* arg0) {
    s32 playerId;
    s8 characterSelectionIndex;

    playerId = arg0->type - 0x34;
    characterSelectionIndex = gCharacterGridSelections[playerId];
    arg0->priority = 0xE - (playerId * 2);
    func_800AAF94(arg0, characterSelectionIndex - 1);
}

void func_800AAE18(struct_8018D9E0_entry* arg0) {
    s32 temp_v0;

    temp_v0 = func_800AAFCC(arg0->type - 0x2B);
    if (temp_v0 >= 0) {
        arg0->priority = 0xE - (temp_v0 * 2);
    } else {
        arg0->priority = 6;
    }
}

/**
 * Similar to find_8018D9E0_entry_dupe, there is potential for a
 * hard lock in the function if no appropriate D_8018D9E0 entry
 * is found.
 **/
struct_8018D9E0_entry* func_800AAE68(void) {
    s32 count = gPlayerCount - 1;
    for (size_t i = 0; i < ARRAY_COUNT(D_8018D9E0); i++) {
        if (D_8018D9E0[i].type == (count + 0xB)) {
            return &D_8018D9E0[i];
        }
    }
    printf("Error: func_800AAE68 returned a null value when searching id 0x%X", count + 0xB);
    return NULL;
    //     struct_8018D9E0_entry *entry = D_8018D9E0;
    //     s32 thing = gPlayerCount - 1;

    //     for(; !(entry > &D_8018D9E0[D_8018D9E0_SIZE]); entry++) {
    //         if ((thing + 0xB) == entry->type) {
    //             goto escape;
    //         }
    //     }

    //     // Something VERY wrong has occurred
    //     while(true);
    // escape:
    //     return entry;
}

/**
 * Similar to find_8018D9E0_entry_dupe, there is potential for a
 * hard lock in the function if no appropriate D_8018D9E0 entry
 * is found.
 **/
struct_8018D9E0_entry* func_800AAEB4(s32 arg0) {

    for (size_t i = 0; i < ARRAY_COUNT(D_8018D9E0); i++) {
        if (D_8018D9E0[i].type == (arg0 + 0x2B)) {
            return &D_8018D9E0[i];
        }
    }
    printf("Error: func_800AAEB4 returned a null value when searching id 0x%X", arg0 + 0x2B);
    return NULL;

    //     struct_8018D9E0_entry *entry = D_8018D9E0;

    //     for(; !(entry > &D_8018D9E0[D_8018D9E0_SIZE]); entry++) {
    //         if ((arg0 + 0x2B) == entry->type) {
    //             goto escape;
    //         }
    //     }

    //     // Something VERY wrong has occurred
    //     while(true);
    // escape:
    //     return entry;
}

/**
 * `arg0` is the desired "type" of the 8018D9E0 entry to
 * search for. If no entry with that "type" is found, this
 * function will enter a `while(1)` loop, hard-locking the
 * game.
 *
 * In practice this never appears to happen, but that is
 * probably as much a matter of luck as it is good
 * reasoning on the original author(s) part.
 **/
struct_8018D9E0_entry* find_8018D9E0_entry_dupe(s32 arg0) {
    for (size_t i = 0; i < ARRAY_COUNT(D_8018D9E0); i++) {
        if (D_8018D9E0[i].type == arg0) {
            return &D_8018D9E0[i];
        }
    }
    printf("Error: find_8018D9E0_entry_dupe returned a null value when searching id 0x%X", arg0);
    return NULL;

    //	struct_8018D9E0_entry *entry = D_8018D9E0;
    //    for (; !(entry > (&D_8018D9E0[D_8018D9E0_SIZE])); entry++) {
    //        if (entry->type == arg0) {
    //            goto escape;
    //        }
    //    }

    // Something VERY wrong has occurred
    //    while(true);
    // escape:
    //    return entry;
}

struct_8018D9E0_entry* find_8018D9E0_entry(s32 arg0) {
    for (size_t i = 0; i < ARRAY_COUNT(D_8018D9E0); i++) {
        if (D_8018D9E0[i].type == arg0) {
            return &D_8018D9E0[i];
        }
    }
    // No printf here as returning null seems to be normal game logic
    // printf("Error: find_8018D9E0_entry returned a null value when searching id 0x%X",
    //             arg0);
    return NULL;

    //    struct_8018D9E0_entry *entry = D_8018D9E0;
    //    for (; !(entry > (&D_8018D9E0[D_8018D9E0_SIZE])); entry++) {
    //        if (entry->type == arg0) {
    //            goto escape;
    //        }
    //    }

    //    return NULL;
    // escape:
    //    return entry;
}

s32 func_800AAF70(s32 arg0) {
    struct_8018D9E0_entry* temp;
    temp = func_800AAEB4(arg0);
    return temp->cursor;
}

void func_800AAF94(struct_8018D9E0_entry* arg0, s32 arg1) {
    struct_8018D9E0_entry* temp_v0;

    temp_v0 = func_800AAEB4(arg1);
    arg0->column = temp_v0->column;
    arg0->row = temp_v0->row;
}

s32 func_800AAFCC(s32 arg0) {
    s32 someIndex = 0;
    s32 ret = 0;

    for (; someIndex < 4; someIndex++) {
        if ((arg0 + 1) == gCharacterGridSelections[someIndex]) {
            ret = 1;
            break;
        }
    }

    if (ret != 0) {
        return someIndex;
    }

    return -1;
}

void func_800AB020(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 1:
            arg0->cursor = 4;
            /* fallthrough */
        case 4:
            if (arg0->unk1C > 0) {
                arg0->unk1C = (arg0->unk1C - (arg0->unk1C / 12)) - 2;
                if (arg0->unk1C < 0) {
                    arg0->unk1C = 0;
                }
            } else {
                arg0->unk1C = 0;
                arg0->cursor = 0;
            }
            break;
        case 2:
            arg0->cursor = 3;
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void func_800AB098(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            if ((gCupSelection + 0x53) == arg0->type) {
                arg0->cursor = 2;
            } else {
                arg0->cursor = 1;
            }
            break;
        case 4:
            if ((gCupSelection + 0x53) == arg0->type) {
                arg0->cursor = 2;
                arg0->unk1C = 0;
                break;
            } else {
                arg0->cursor = 1;
            }
        // Purposeful fallthrough
        case 1:
            if (arg0->unk1C < 32) {
                arg0->unk1C += 2;
                if (arg0->unk1C >= 32) {
                    arg0->unk1C = 32;
                }
            }
            break;
        case 3:
            if ((gCupSelection + 0x53) == arg0->type) {
                arg0->cursor = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void func_800AB164(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* thing = &D_800E7148[arg0->type - 0x53];

    if ((gCupSelection + 0x53) == arg0->type) {
        arg0->priority = 0x0A;
    } else {
        arg0->priority = 4;
    }

    switch (arg0->cursor) {
        case 0:
            thing = &D_800E7148[arg0->type - 0x53];
            arg0->column = thing->column;
            arg0->row = thing->row;
            break;
        case 2:
            func_800A91D8(arg0, 0x00000080, 0x0000003B);
            break;
        case 3:
            thing = &D_800E7148[arg0->type - 0x53];
            func_800A91D8(arg0, thing->column, thing->row);
            if ((arg0->column == thing->column) && (arg0->row == thing->row)) {
                arg0->cursor = 0;
            }
            break;
        case 1:
        case 4:
        default:
            break;
    }
}

void func_800AB260(struct_8018D9E0_entry* arg0) {
    s32 temp = (arg0->type - 0x58);
    if (temp == gCupSelection) {
        arg0->visible = 1;
    } else {
        arg0->visible = 0;
    }
}

void func_800AB290(struct_8018D9E0_entry* arg0) {
    if (arg0->unk1C != gCupSelection) {
        arg0->unk1C = gCupSelection;
        func_8009A594(arg0->D_8018DEE0_index, 0,
                      segmented_to_virtual_dupe_2(D_800E7E34[gCupCourseOrder[gCupSelection][arg0->type - 0x5F]]));
    }
}

#ifdef NON_MATCHING
// Decent work has been done, but not quite right
// https://decomp.me/scratch/YLbkC
void func_800AB314(struct_8018D9E0_entry* arg0) {
    s32 var_a1;
    s32 one = 1;
    s32 four = 4;
    struct_8018D9E0_entry* sp24[4];
    s32 var_v0;

    for (var_a1 = 0; var_a1 < 4; var_a1++) {
        sp24[var_a1] = find_8018D9E0_entry_dupe(var_a1 + 0x5F);
    }
    switch (gModeSelection) {
        default:
            if (D_8018EDEC != one) {
                arg0->cursor = 0;
                arg0->unk20 = 0;
                for (var_a1 = 0; var_a1 < 4; var_a1++) {
                    if (gCupCourseSelection == var_a1) {
                        sp24[var_a1]->visible = one;
                        if (arg0->unk1C != var_a1) {
                            arg0->unk1C = var_a1;
                        }
                    } else {
                        sp24[var_a1]->visible = 0;
                    }
                }
            } else {
                arg0->cursor = 3;
                for (var_a1 = 0; var_a1 < 4; var_a1++) {
                    sp24[var_a1]->visible = one;
                    sp24[var_a1]->priority = 6;
                }
            }
            break;
        case 0:
            switch (arg0->cursor) { /* irregular */
                case 0:
                    if ((arg0->unk1C / 4) == gCupSelection) {
                        arg0->unk20++;
                        if (arg0->unk20 >= 0x33) {
                            arg0->cursor = one;
                            arg0->unk20 = 0;
                            var_v0 = gCupSelection * 4;
                            arg0->unk1C = var_v0 + 1;
                        }
                    } else {
                        arg0->unk20 = 0;
                        arg0->unk1C = gCupSelection * 4;
                    }
                    if (D_8018EDEC == 3) {
                        arg0->cursor = 2;
                        arg0->unk20 = 0;
                    }
                    break;
                case 1:
                    if ((arg0->unk1C / 4) != gCupSelection) {
                        arg0->cursor = 0;
                        arg0->unk20 = 0;
                        arg0->unk1C = 0;
                    } else {
                        arg0->unk20++;
                        if (arg0->unk20 >= 0x1F) {
                            arg0->unk20 = 0;
                            arg0->unk1C = (gCupSelection * 4) + (((arg0->unk1C % 4) + one) % 4);
                        } else {
                            if (D_8018EDEC == 3) {
                                arg0->cursor = 2;
                                arg0->unk20 = 0;
                            }
                        }
                    }
                    break;
                case 2:
                    arg0->unk20++;
                    if (arg0->unk20 >= 0x1A) {
                        arg0->cursor = 3;
                        arg0->unk20 = 0;
                    }
                    if (D_8018EDEC != 3) {
                        arg0->cursor = 0;
                        arg0->unk20 = 0;
                        arg0->unk1C = 0;
                    }
                    break;
                case 3:
                    if (D_8018EDEC != 3) {
                        arg0->cursor = 0;
                        arg0->unk20 = 0;
                        arg0->unk1C = 0;
                    }
                    break;
            }
            switch (arg0->cursor) {
                case 0:
                case 1:
                    for (var_a1 = 0; var_a1 < 4; var_a1++) {
                        if ((arg0->unk1C % 4) == var_a1) {
                            sp24[var_a1]->visible = one;
                        } else {
                            sp24[var_a1]->visible = 0;
                        }
                        sp24[var_a1]->priority = 6;
                    }
                    break;
                case 2:
                    for (var_a1 = 0; var_a1 < 4; var_a1++) {
                        if (var_a1 == (arg0->unk1C % 4)) {
                            sp24[var_a1]->priority = 6;
                        } else if (arg0->unk20 < (var_a1 * 5)) {
                            sp24[var_a1]->priority = four;
                        } else {
                            sp24[var_a1]->priority = 8;
                        }
                        sp24[var_a1]->visible = one;
                    }
                    break;
                case 3:
                    for (var_a1 = 0; var_a1 < 4; var_a1++) {
                        sp24[var_a1]->visible = one;
                        sp24[var_a1]->priority = 6;
                    }
                    break;
                default:
                    break;
            }
            break;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800AB314.s")
#endif

void func_800AB904(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_a1;

    switch (D_8018EDEC) { /* irregular */
        case 1:
            temp_a1 = &D_800E7248[arg0->type - 0x65];
            if (arg0->column != temp_a1->column) {
                func_800A9208(arg0, temp_a1->column);
            }
            break;
        case 2:
        case 3:
            temp_a1 = &D_800E7258[arg0->type - 0x65];
            if (arg0->column != temp_a1->column) {
                func_800A9208(arg0, temp_a1->column);
            }
            break;
    }
}

void func_800AB9B0(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v1;

    if (arg0->unk1C != gCupSelection) {
        arg0->unk1C = gCupSelection;
        arg0->unk20 = func_800B54C0((s32) gCupSelection, gCCSelection);
        func_8009A594(arg0->D_8018DEE0_index, 0,
                      segmented_to_virtual_dupe_2(D_800E7E20[((gCCSelection / 2) * 4) - arg0->unk20]));
        arg0->column = (s32) D_800E7268->column;
        arg0->row = D_800E7268->row;
    }
    temp_v1 = &D_800E7268[arg0->cursor];
    switch (arg0->cursor) { /* irregular */
        case 0:
            func_800A91D8(arg0, (s32) temp_v1->column, (s32) temp_v1->row);
            if (D_8018EDEC == 3) {
                arg0->cursor = 1;
            }
            break;
        case 1:
            func_800A91D8(arg0, (s32) temp_v1->column, (s32) temp_v1->row);
            if (D_8018EDEC == 1) {
                arg0->cursor = 0;
            }
            break;
    }
}

void func_800ABAE8(struct_8018D9E0_entry* arg0) {
    s32 index;

    if (arg0->type == 0x8C) {
        index = 4;
    } else {
        index = arg0->type - 0x78;
    }
    arg0->column = D_800E7430[index].column;
    arg0->row = D_800E7430[index].row;
}

void func_800ABB24(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v1;
    s32 thing = gTimeTrialDataCourseIndex;

    temp_v1 = &D_800E7430[thing / 4];
    arg0->column = temp_v1->column - 2;
    arg0->row = temp_v1->row + ((thing % 4) * 0x32) + 0x13;
    arg0->unk1C += 0x10;
    if (arg0->unk1C >= 0x100) {
        arg0->unk1C -= 0x100;
        arg0->unk20 = (s32) (arg0->unk20 + 1) % 3;
    }
}

void func_800ABBCC(struct_8018D9E0_entry* arg0) {
    s32 temp_v0;
    Unk_D_800E70A0* temp_v1;

    temp_v0 = arg0->type - 0x7C;
    temp_v1 = &D_800E7430[temp_v0 / 4];
    arg0->column = (s32) temp_v1->column;
    arg0->row = temp_v1->row + ((temp_v0 % 4) * 0x32) + 0x14;
}

void func_800ABC38(struct_8018D9E0_entry* arg0) {
    // Huh?
    s32 one = 1;
    func_800ABCF4(arg0);
    switch (D_80164A28) { /* irregular */
        case 1:
            arg0->visible = one;
            break;
        case 2:
            if (arg0->row >= -0x13) {
                arg0->row -= 2;
            } else {
                arg0->type = 0;
            }
            break;
        default:
            if ((gModeSelection != GRAND_PRIX) || (gPlayerCountSelection1 != (s32) 1U) || (D_8018EE08 != 0)) {
                arg0->type = 0;
            } else {
                if (arg0->row >= -0x13) {
                    arg0->row -= 2;
                } else {
                    arg0->type = 0;
                }
            }
            break;
    }
}

void func_800ABCF4(struct_8018D9E0_entry* arg0) {
    f64 temp_f0;

    switch (arg0->cursor) { /* irregular */
        case 0:
            arg0->column = 0;
            arg0->cursor = 1;
            arg0->unk20 = (get_string_width(gCupNames[D_800DC540]) / 2) + 0xA0;
            /* fallthrough */
        case 1:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (s32) (arg0->unk20 - arg0->column) / 4;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 8;
            }
            arg0->unk24 = (f32) (((f64) arg0->unk1C * 0.05) + 1.0);
            if (arg0->column >= (arg0->unk20 - 0x14)) {
                arg0->cursor = 2;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 2:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (s32) (arg0->unk20 - arg0->column) / 4;
            arg0->D_8018DEE0_index++;
            temp_f0 = (f64) (arg0->D_8018DEE0_index - 0xA);
            arg0->unk24 = (f32) ((temp_f0 * 0.0085 * temp_f0) + 0.4);
            if ((arg0->D_8018DEE0_index >= 9) && ((f64) arg0->unk24 > 1.0)) {
                arg0->unk24 = 1.0f;
            }
            break;
    }
}

void func_800ABEAC(struct_8018D9E0_entry* arg0) {
    s32 why = 1;
    func_800ABF68(arg0);
    switch (D_80164A28) {
        case 1:
            arg0->visible = why;
            break;
        case 2:
            if (arg0->row < 0x104) {
                arg0->row += 2;
            } else {
                arg0->type = 0;
            }
            break;
        default:
            if ((gModeSelection != GRAND_PRIX) || (gPlayerCountSelection1 != why) || (D_8018EE08 != 0)) {
                arg0->type = 0;
            } else {
                if (arg0->row < 0x104) {
                    arg0->row += 2;
                } else {
                    arg0->type = 0;
                }
            }
            break;
    }
}

void func_800ABF68(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            arg0->column = 0x140;
            arg0->cursor = 1;
            arg0->unk20 = 0xA0 - (get_string_width(gCourseNames[gCurrentCourseId]) / 2);
            /* fallthrough */
        case 1:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (arg0->column - arg0->unk20) / 4;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 8;
            }
            arg0->unk24 = (arg0->unk1C * 0.05) + 1.0;
            if ((arg0->unk20 + 0x14) >= arg0->column) {
                arg0->cursor = 2;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 2:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (arg0->column - arg0->unk20) / 4;
            arg0->D_8018DEE0_index++;
            arg0->unk24 = ((arg0->D_8018DEE0_index - 0xA) * 0.0085 * (arg0->D_8018DEE0_index - 0xA)) + 0.4;
            if ((arg0->D_8018DEE0_index >= 9) && ((f64) arg0->unk24 > 1.0)) {
                arg0->unk24 = 1.0f;
            }
            break;
    }
}

void func_800AC128(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            arg0->column = 0x00000140;
            arg0->cursor = 1;
            /* fallthrough */
        case 1:
            func_800A940C(arg0, 0x00000064);
            arg0->unk1C = (s32) (arg0->column - 0x64) / 6;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 8;
            }
            arg0->unk24 = (f32) (((f64) arg0->unk1C * 0.07) + 0.6);
            if (arg0->column == 0x00000064) {
                arg0->cursor = 2;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 2:
            arg0->D_8018DEE0_index++;
            arg0->unk1C = 0;
            arg0->unk24 = (f32) (1.5 - ((arg0->D_8018DEE0_index - 0xF) * 0.004 * (arg0->D_8018DEE0_index - 0xF)));
            if ((arg0->D_8018DEE0_index >= 0x10) && ((f64) arg0->unk24 < 0.8)) {
                arg0->cursor = 3;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 3:
            arg0->D_8018DEE0_index++;
            arg0->unk1C = 0;
            arg0->unk24 = (f32) (1.25 - ((arg0->D_8018DEE0_index - 0xF) * 0.002 * (arg0->D_8018DEE0_index - 0xF)));
            if ((arg0->D_8018DEE0_index >= 0xD) && ((f64) arg0->unk24 < 1.0)) {
                arg0->unk24 = 1.0f;
            }
            break;
    }
}

void func_800AC300(struct_8018D9E0_entry* arg0) {
    if (arg0->unk20 < ++arg0->unk1C) {
        arg0->type = 0;
    }
}

void func_800AC324(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            arg0->column = 0x14A;
            arg0->cursor = 1;
            func_800921B4();
            break;
        case 1:
            func_800A9208(arg0, 0xA0);
            if (arg0->column == 0xA0) {
                arg0->cursor = 2;
                arg0->unk20 = 0;
            }
            break;
        case 2:
            arg0->unk20++;
            if (((D_8018D9D8 != 0) || (arg0->unk20 >= 0x5B)) && (D_800DDB24 != 0)) {
                arg0->cursor = 3;
                arg0->unk1C = arg0->column;
                add_8018D9E0_entry(0xAB, 0, 0, 0);
            }
            break;
        case 3:
            arg0->column = arg0->unk1C;
            if (arg0->unk1C < 0x14A) {
                if (D_8018D9D8 != 0) {
                    arg0->unk1C += 0x20;
                } else {
                    arg0->unk1C += 0x10;
                }
            } else {
                arg0->type = 0;
            }
            break;
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/NzdUC
// A really stupid register allocation issue
void func_800AC458(struct_8018D9E0_entry* arg0) {
    s32 var_a1;
    s32 var_t1;

    switch (arg0->cursor) {
        case 0:
            arg0->column = -0x000000A0;
            arg0->cursor = 1;
            for (var_a1 = 0; var_a1 < 4; var_a1++) {
                D_8018D9BC[var_a1] = gGPPointRewards[var_a1];
            }
            arg0->unk20 = arg0->column;
            break;
        case 1:
            arg0->column = arg0->unk20;
            if (D_8018D9D8 != 0) {
                var_a1 = 0x20;
            } else {
                var_a1 = 0x10;
            }
            if ((arg0->unk20 + var_a1) < 0) {
                arg0->unk20 += var_a1;
                D_800DC5EC->screenStartX += var_a1;
                D_800DC5F0->screenStartX -= var_a1;
            } else {
                arg0->unk20 = 0;
                arg0->column = 0;
                arg0->cursor = 2;
                arg0->unk1C = 0;
                D_800DC5EC->screenStartX = 0x00F0;
                D_800DC5F0->screenStartX = 0x0050;
            }
            break;
        case 2:
            arg0->column = 0;
            arg0->unk1C++;
            if (((D_8018D9D8 != 0) || (arg0->unk1C >= 0x1F)) && (D_800DDB24 != 0)) {
                arg0->cursor = 3;
                arg0->unk1C = 0;
                arg0->unk20 = 0;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            var_t1 = 0;
            var_a1 = arg0->cursor - 3;
            arg0->unk1C++;
            if (((arg0->unk1C % 3) == 0) || (D_8018D9D8 != 0)) {
                if (D_8018D9BC[var_a1] > 0) {
                    D_8018D9BC[var_a1]--;
                    gGPPointsByCharacterId[gPlayers[gGPCurrentRacePlayerIdByRank[var_a1]].characterId] += 1;
                    play_sound2(SOUND_ACTION_COUNT_SCORE);
                    var_t1 = 0;
                    if ((D_8018D9BC[var_a1] == 0) && (arg0->unk20 == 0)) {
                        arg0->unk20 = 1;
                        arg0->unk1C = 0;
                    }
                }
            }
            if ((arg0->unk20 != 0) && ((arg0->unk1C > 0xA) || ((D_8018D9D8 != 0) && (arg0->unk1C >= 4)))) {
                var_t1 = 1;
            }
            if (var_t1 != 0) {
                arg0->unk20 = 0;
                arg0->unk1C = 0;
                if (arg0->cursor < 6) {
                    arg0->cursor++;
                } else {
                    arg0->cursor = 7;
                }
            }
            break;
        case 7:
            arg0->unk1C++;
            if ((((D_8018D9D8 != 0) && (arg0->unk1C >= 0xB)) || (arg0->unk1C >= 0x3D)) && (D_800DDB24 != 0)) {
                arg0->cursor = 8;
                arg0->unk1C = 0;
            }
            break;
        case 8:
            arg0->unk1C++;
            if (D_8018D9D8 != 0) {
                arg0->unk1C += 5;
            }
            if (arg0->unk1C >= 0x29) {
                arg0->cursor = 9;
            }
            break;
        case 9:
            arg0->unk1C--;
            if (D_8018D9D8 != 0) {
                arg0->unk1C -= 5;
            }
            if (arg0->unk1C <= 0) {
                arg0->cursor = 0x0000000A;
                arg0->unk1C = 0;
                if (gCupCourseSelection == 3) {
                    for (var_a1 = 0; var_a1 < 8; var_a1++) {
                        if (D_80164478[gCharacterIdByGPOverallRank[var_a1]] < gPlayerCount) {
                            func_800B536C(var_a1);
                            break;
                        }
                    }
                }
            }
            break;
        case 10:
            arg0->unk1C++;
            if (arg0->unk1C > 0) {
                arg0->cursor = 0x0000000B;
                arg0->unk1C = 0;
                func_800921B4();
            }
            break;
        case 11:
            if ((D_8018D9D8 != 0) && (D_800DDB24 != 0)) {
                arg0->cursor = 0x0000000C;
                arg0->unk20 = arg0->row;
                play_sound2(SOUND_ACTION_NEXT_COURSE);
            }
            break;
        case 12:
            arg0->row = arg0->unk20;
            if (arg0->unk20 < 0xF0) {
                arg0->unk20 += 0x10;
                D_800DC5EC->screenStartY += 0x10;
                D_800DC5F0->screenStartY -= 0x10;
            } else {
                arg0->unk20 = 0;
                arg0->cursor = 0x0000000D;
                arg0->unk1C = 0;
                D_800DC5EC->screenStartY = 0x012C;
                D_800DC5F0->screenStartY = -0x003C;
                D_8015F894 = 4;
                func_800CA330(0x19U);
            }
            break;
        case 13:
        default:
            break;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/code_80091750/func_800AC458.s")
#endif

void func_800AC978(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) { /* irregular */
        case 0:
            arg0->column = 0x14A;
            arg0->cursor = 1;
            arg0->unk1C = 0xFF;
            break;
        case 1:
            func_800A9208(arg0, 0xA0);
            if (arg0->column == 0xA0) {
                arg0->cursor = 2;
            }
            break;
        case 2:
            break;
        case 3:
            if (arg0->unk1C != 0) {
                arg0->unk1C -= 0x33;
            }
            break;
    }
}

void func_800ACA14(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) { /* irregular */
        case 0:
            if (arg0->unk20 >= 0xB) {
                arg0->unk1C += 3;
            }
            if (arg0->unk1C >= 0x65) {
                arg0->cursor = 0x0000000B;
                arg0->unk1C = 0;
            }
            break;
        case 11:
        case 12:
            if (func_800B4520()) {
                break;
            }

            if ((gControllerFive->buttonPressed | gControllerFive->stickPressed) & 0x800) {
                if (arg0->cursor >= 0xC) {
                    arg0->cursor--;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                    arg0->unk8 = -1;
                }
            }
            if ((gControllerFive->buttonPressed | gControllerFive->stickPressed) & 0x400) {
                if (arg0->cursor < 0xC) {
                    arg0->cursor++;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                    arg0->unk8 = 1;
                }
            }
            if (gControllerFive->buttonPressed & 0x9000) {
                func_8009DFE0(0x0000001E);
                func_800CA330(0x19U);
                play_sound2(SOUND_ACTION_CONTINUE_UNKNOWN);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->unk20 < 0x28) {
        arg0->unk20++;
    }
}

void func_800ACC50(struct_8018D9E0_entry* arg0) {
    s32 var_s0;

    switch (arg0->cursor) {
        case 0:
            arg0->unk1C += 3;
            if (arg0->unk1C >= 0x65) {
                arg0->cursor = 1;
                arg0->unk1C = 0;
                for (var_s0 = 0; var_s0 < gPlayerCount; var_s0++) {
                    add_8018D9E0_entry(var_s0 + 0xB1, 0, 0, (s8) (5 - var_s0));
                }
            }
            break;
        case 1:
            if (find_8018D9E0_entry_dupe(0x000000B1)->cursor >= 2) {
                arg0->cursor = 2;
            }
            break;
        case 2:
            arg0->unk1C += 0x20;
            if (arg0->unk1C >= 0x100) {
                if (gModeSelection == VERSUS) {
                    arg0->cursor = (s32) D_8018EDF4;
                } else {
                    arg0->cursor = (s32) D_8018EDF6;
                }
                arg0->unk1C = 0;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            if (func_800B4520() == 0) {
                if ((gControllerFive->buttonPressed | gControllerFive->stickPressed) & 0x800) {
                    if (arg0->cursor >= 0xB) {
                        arg0->cursor--;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = -1;
                    }
                }
                if ((gControllerFive->buttonPressed | gControllerFive->stickPressed) & 0x400) {
                    if (arg0->cursor < 0xD) {
                        arg0->cursor++;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = 1;
                    }
                }
                if (gControllerFive->buttonPressed & 0x9000) {
                    func_8009DFE0(0x0000001E);
                    play_sound2(SOUND_MENU_OK_CLICKED);
                    if (gModeSelection == VERSUS) {
                        D_8018EDF4 = (s8) arg0->cursor;
                    } else {
                        D_8018EDF6 = (s8) arg0->cursor;
                    }
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void func_800ACF40(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v0_2;
    s32 temp_a2;
    s32 temp_a1;
    s32 var_v1;
    UNUSED s32 stackPadding0;

    temp_a2 = arg0->type - 0xB1;
    temp_a1 = D_800EFD64[gCharacterSelections[arg0->type - 0xB1]];
    switch (arg0->cursor) {
        case 0:
            arg0->column = D_800E72F8.column;
            arg0->row = D_800E72F8.row;
            arg0->cursor = 1;
            break;
        case 1:
            temp_v0_2 = &D_800E7300[((gPlayerCount - 2) * 4) + temp_a2];
            func_800A9208(arg0, temp_v0_2->column);
            func_800A9278(arg0, temp_v0_2->row);
            if (arg0->column == temp_v0_2->column) {
                arg0->cursor = 2;
                arg0->unk20 = 0;
            }
            break;
        case 2:
            var_v1 = 0;
            switch (gModeSelection) { /* switch 1; irregular */
                case 2:               /* switch 1 */
                    if (gGPCurrentRaceRankByPlayerId[temp_a2] != 0) {
                        var_v1 = 1;
                    }
                    break;
                case 3: /* switch 1 */
                    if (temp_a2 != gPlayerWinningIndex) {
                        var_v1 = 1;
                    }
                    break;
            }
            if (var_v1 == 0) {
                arg0->unk20++;
                if (arg0->unk20 >= 0x1F) {
                    if (find_8018D9E0_entry_dupe(0x000000B0)->cursor >= 2) {
                        func_8009A640(arg0->D_8018DEE0_index, 0, temp_a2,
                                      segmented_to_virtual_dupe_2(gCharacterCelebrateAnimation[temp_a1]));
                        arg0->cursor = 3;
                        func_800CA24C(temp_a2);
                        func_800C90F4(temp_a2, (gCharacterSelections[temp_a2] * 0x10) + 0x29008007);
                    }
                }
            }
            break;
        case 3:
            if (D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex >= D_800E8440[temp_a1]) {
                func_8009A640(arg0->D_8018DEE0_index, 0, temp_a2, segmented_to_virtual_dupe_2(D_800E83A0[temp_a1]));
                arg0->cursor = 4;
            }
            break;
        case 4:
        default:
            break;
    }
}

void func_800AD1A4(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            arg0->column = 0x0000014A;
            arg0->cursor = 1;
            func_800921B4();
            add_8018D9E0_entry(0x000000BB, 0, 0, 0);
            break;
        case 1:
            func_800A9208(arg0, 0x000000A0);
            if (arg0->column == 0x000000A0) {
                arg0->cursor = 2;
                arg0->unk20 = 0;
            }
            break;
        case 2:
            arg0->unk20++;
            if (arg0->unk20 >= 0x15) {
                arg0->cursor = 3;
            }
            break;
        case 3:
            if (D_8018D9D8 != 0) {
                func_800921B4();
                arg0->cursor = 4;
                arg0->unk1C = arg0->column;
                add_8018D9E0_entry(0x000000BA, 0, 0, 0);
            }
            break;
        case 4:
            arg0->column = arg0->unk1C;
            if (arg0->unk1C < 0x14A) {
                if (D_8018D9D8 != 0) {
                    arg0->unk1C += 0x20;
                } else {
                    arg0->unk1C += 0x10;
                }
            } else {
                arg0->type = 0;
            }
            break;
        default:
            break;
    }
}

void func_800AD2E8(struct_8018D9E0_entry* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;
    s32 var_a1;
    s32 index;

    switch (arg0->cursor) { /* switch 3; irregular */
        case 0:             /* switch 3 */
            arg0->column = -0x000000A0;
            arg0->cursor = 1;
            for (index = 0; index < 4; index++) {
                D_8018D9BC[index] = gGPPointRewards[index];
            }
            arg0->unk20 = arg0->column;
            break;
            ;
        case 1: /* switch 3 */
            arg0->column = arg0->unk20;
            if (D_8018D9D8 != 0) {
                var_a1 = 0x20;
            } else {
                var_a1 = 0x10;
            }
            if ((arg0->unk20 + var_a1) < 0) {
                arg0->unk20 += var_a1;
                D_800DC5EC->screenStartX += var_a1;
                D_800DC5F0->screenStartX -= var_a1;
            } else {
                arg0->unk20 = 0;
                arg0->column = 0;
                arg0->cursor = D_8018EDF5;
                if ((arg0->cursor == 9) && (D_80162DF8 == 1)) {
                    arg0->cursor--;
                }
                D_800DC5EC->screenStartX = 0x00F0;
                D_800DC5F0->screenStartX = 0x0050;
            }
            break;
        case 5:  /* switch 3 */
        case 6:  /* switch 3 */
        case 7:  /* switch 3 */
        case 8:  /* switch 3 */
        case 9:  /* switch 3 */
        case 10: /* switch 3 */
            if (func_800B4520() == 0) {
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) {
                    if (arg0->cursor >= 6) {
                        arg0->cursor--;
                        if ((D_80162DF8 == 1) && (arg0->cursor == 9)) {
                            arg0->cursor--;
                        }
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = -1;
                    }
                }
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                    if (arg0->cursor < 0xA) {
                        arg0->cursor++;
                        if ((D_80162DF8 == 1) && (arg0->cursor == 9)) {
                            arg0->cursor++;
                        }
                        if ((arg0->cursor == 0x0000000A) && (D_80162DD4 != 0)) {
                            arg0->cursor -= 2;
                        } else {
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->unk24 < 4.2) {
                                arg0->unk24 += 4.0;
                            }
                            arg0->unk8 = 1;
                        }
                    }
                }
                if (gControllerOne->buttonPressed & 0x9000) {
                    if (arg0->cursor == 0x0000000A) {
                        var_v1 = 0;
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        if (gControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote)) { /* switch 4; irregular */
                                case 5:                                                        /* switch 4 */
                                    break;
                                case 0: /* switch 4 */
                                    func_800B6708();
                                    arg0->cursor = func_800B6348((gCupSelection * 4) + gCupCourseSelection) + 0x11;
                                    var_v1 = 1;
                                    play_sound2(SOUND_MENU_SELECT);
                                    break;
                                case 2: /* switch 4 */
                                    gControllerPak1State = 0;
                                    break;
                                default: /* switch 4 */
                                    gControllerPak1State = 0;
                                    break;
                            }
                        }
                        if (var_v1 == 0) {
                            if (gControllerPak1State == 0) {
                                switch (func_800B5F30()) { /* switch 2 */
                                    case -1:               /* switch 2 */
                                        arg0->cursor = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case -3: /* switch 2 */
                                    case -2: /* switch 2 */
                                        arg0->cursor = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    case 1:  /* switch 2 */
                                    case 11: /* switch 2 */
                                        arg0->cursor = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case 10: /* switch 2 */
                                        arg0->cursor = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    default: /* switch 2 */
                                        var_v1 = 1;
                                        arg0->cursor = 0x0000000C;
                                        break;
                                    case 0:
                                        break;
                                }
                                if (var_v1 != 0) {
                                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                }
                                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                                    func_800B6708();
                                    arg0->cursor = func_800B6348((gCupSelection * 4) + gCupCourseSelection) + 0x11;
                                    play_sound2(SOUND_MENU_SELECT);
                                    return;
                                }
                            }
                            if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                                arg0->cursor = 0x0000000E;
                                play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (gControllerPak1NumPagesFree >= 0x79) {
                                arg0->cursor = 0x00000013;
                                arg0->unk1C = 0;
                                play_sound2(SOUND_MENU_SELECT);
                                return;
                            }
                            arg0->cursor = 0x0000000E;
                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        arg0->unk1C = arg0->cursor;
                        D_8018EDF5 = arg0->cursor;
                        arg0->cursor = 0x0000001E;
                        arg0->unk20 = arg0->row;
                        play_sound2(SOUND_ACTION_NEXT_COURSE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                    }
                }
            }
            break;
        case 11: /* switch 1 */
        case 12: /* switch 1 */
        case 13: /* switch 1 */
        case 14: /* switch 1 */
        case 15: /* switch 1 */
        case 16: /* switch 1 */
        case 26: /* switch 1 */
            if (gControllerOne->buttonPressed & 0xD000) {
                arg0->cursor = 0x0000000A;
                play_sound2(SOUND_MENU_GO_BACK);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
            }
            break;
        case 17: /* switch 1 */
        case 18: /* switch 1 */
            arg0->unk20 = arg0->cursor - 0x11;
            if (func_800B639C((gCupSelection * 4) + gCupCourseSelection) != arg0->unk20) {
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) {
                    if (arg0->cursor >= 0x12) {
                        arg0->cursor--;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = -1;
                    }
                }
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                    if (arg0->cursor < 0x12) {
                        arg0->cursor++;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = 1;
                    }
                }
            }
            if (gControllerOne->buttonPressed & 0x4000) {
                arg0->cursor = 0x0000000A;
                play_sound2(SOUND_MENU_GO_BACK);
                return;
            }
            if (gControllerOne->buttonPressed & 0x9000) {
                thing = &D_8018EE10[arg0->unk20];
                if (thing->ghostDataSaved == 0) {
                    arg0->cursor = 0x00000019;
                    arg0->unk1C = 0;
                } else if (func_800B63F0(arg0->unk20) == 0) {
                    arg0->cursor = 0x00000010;
                } else {
                    arg0->cursor = 0x00000014;
                }
                play_sound2(SOUND_MENU_SELECT);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
            }
            break;
        case 19: /* switch 1 */
            if ((arg0->unk1C == 1) && (func_800B6A68() != 0)) {
                arg0->cursor = 0x0000000F;
                return;
            } else {
                arg0->unk1C++;
                if (arg0->unk1C >= 2) {
                    arg0->cursor = 0x00000011;
                }
            }
            break;
        case 20: /* switch 1 */
        case 21: /* switch 1 */
            if (((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) && (arg0->cursor >= 0x15)) {
                arg0->cursor--;
                play_sound2(SOUND_MENU_CURSOR_MOVE);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
                arg0->unk8 = -1;
            }
            if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                if (arg0->cursor < 0x15) {
                    arg0->cursor++;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                    arg0->unk8 = 1;
                }
            }
            if (gControllerOne->buttonPressed & 0x4000) {
                arg0->cursor = arg0->unk20 + 0x11;
                play_sound2(SOUND_MENU_GO_BACK);
                return;
            }
            if (gControllerOne->buttonPressed & 0x9000) {
                if (arg0->cursor == 0x00000015) {
                    arg0->cursor = 0x00000019;
                    arg0->unk1C = 0;
                    play_sound2(SOUND_MENU_SELECT);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                } else {
                    arg0->cursor = arg0->unk20 + 0x11;
                    play_sound2(SOUND_MENU_GO_BACK);
                    return;
                }
            }
            break;
        case 25: /* switch 1 */
            if (arg0->unk1C == 1) {
                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                                  &gControllerPak1FileNote) != 0) {
                    arg0->cursor = 0x0000001A;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (func_800B6178(arg0->unk20) != 0) {
                    arg0->cursor = 0x0000001A;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->unk1C++;
            if (arg0->unk1C >= 2) {
                arg0->cursor = 0x0000000A;
                return;
            }
            break;
        case 30: /* switch 1 */
            arg0->row = arg0->unk20;
            if (arg0->unk20 < 0xF0) {
                arg0->unk20 += 0x10;
                D_800DC5EC->screenStartY += 0x10;
                D_800DC5F0->screenStartY -= 0x10;
                return;
            }
            switch (arg0->unk1C) { /* switch 3 */
                case 5:            /* switch 3 */
                    D_8015F890 = 0;
                    D_8015F892 = 1;
                    func_802903B0();
                    break;
                case 6: /* switch 3 */
                    func_80290388();
                    break;
                case 7: /* switch 3 */
                    func_80290360();
                    break;
                case 8: /* switch 3 */
                    func_80290338();
                    break;
                case 9: /* switch 3 */
                    D_8015F890 = 1;
                    D_8015F892 = 0;
                    func_802903B0();
                    break;
            }
            arg0->unk20 = 0;
            arg0->cursor = 0x0000001F;
            D_800DC5EC->screenStartY = 0x012C;
            D_800DC5F0->screenStartY = -0x003C;
            D_8015F894 = 4;
            func_800CA330(0x19U);
            break;
        case 31: /* switch 1 */
            arg0->type = 0;
            break;
    }
}
#ifdef VERSION_EU
#define FUNC_800ADF48DEF 70
#else
#define FUNC_800ADF48DEF 60
#endif
void func_800ADF48(struct_8018D9E0_entry* arg0) {
    UNUSED s32 stackPadding;
    struct Controller* controller;

    if (gIsGamePaused != 0) {
        switch (arg0->cursor) {
            case 0:
                arg0->cursor = D_800F0B50[gModeSelection];
                break;
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 41:
            case 42:
            case 43:
            case 44:
                if (func_800B4520() == 0) {
                    controller = &gControllers[gIsGamePaused - 1];
                    if ((controller->buttonPressed | controller->stickPressed) & 0x800) {
                        if (D_800F0B50[gModeSelection] < arg0->cursor) {
                            arg0->cursor--;
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->unk24 < 4.2) {
                                arg0->unk24 += 4.0;
                            }
                            arg0->unk8 = -1;
                        }
                    }
                    if ((controller->buttonPressed | controller->stickPressed) & 0x400) {
                        if (arg0->cursor < D_800F0B54[gModeSelection]) {
                            arg0->cursor++;
                            play_sound2(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->unk24 < 4.2) {
                                arg0->unk24 += 4.0;
                            }
                            arg0->unk8 = 1;
                        }
                    }
                    if (controller->buttonPressed & B_BUTTON) {
                        if (arg0->cursor != D_800F0B50[gModeSelection]) {
                            arg0->cursor = D_800F0B50[gModeSelection];
                            play_sound2(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                    if (controller->buttonPressed & (START_BUTTON | A_BUTTON)) {
                        if (arg0->cursor == D_800F0B50[gModeSelection]) {
                            arg0->cursor = 0;
                            gIsGamePaused = 0;
                            func_8028DF38();
                            func_800C9F90(0U);
                        } else {
                            func_8009DFE0(30);
                            play_sound2(SOUND_ACTION_CONTINUE_UNKNOWN);
                            func_800CA330(FUNC_800ADF48DEF);
                            if (arg0->unk24 < 4.2) {
                                arg0->unk24 += 4.0;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    } else {
        arg0->cursor = 0;
    }
}

void func_800AE218(struct_8018D9E0_entry* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;

    if (arg0->cursor != 0) {
        D_800DC5B8 = 0;
    }
    switch (arg0->cursor) { /* switch 1 */
        case 0:             /* switch 1 */
            if (arg0->unk1C < 0x1E) {
                arg0->unk1C++;
            }
            if (gControllerOne->buttonPressed & 0x1000) {
                arg0->cursor = 0x0000000F;
                play_sound2(SOUND_ACTION_GO_BACK_2);
            } else if (playerHUD[PLAYER_ONE].raceCompleteBool != 0) {
                arg0->cursor = 1;
                arg0->unk1C = 0;
            }
            break;
        default: /* switch 1 */
            break;
        case 1: /* switch 1 */
            arg0->unk1C += 3;
            if (arg0->unk1C >= 0x8D) {
                arg0->cursor = 0x0000000F;
            }
            break;
        case 11: /* switch 1 */
        case 12: /* switch 1 */
        case 13: /* switch 1 */
        case 14: /* switch 1 */
        case 15: /* switch 1 */
        case 16: /* switch 1 */
            if (func_800B4520() == 0) {
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) {
                    if (arg0->cursor >= 0xC) {
                        arg0->cursor--;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = -1;
                    }
                }
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                    if (arg0->cursor < 0x10) {
                        arg0->cursor++;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = 1;
                    }
                }
                if (gControllerOne->buttonPressed & 0x9000) {
                    if (arg0->cursor == 0x00000010) {
                        var_v1 = 0;
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        if (gControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote)) { /* switch 3; irregular */
                                case PFS_ERR_INVALID:                                          /* switch 3 */
                                    break;
                                case PFS_NO_ERROR: /* switch 3 */
                                    func_800B6708();
                                    arg0->cursor = func_800B6348((gCupSelection * 4) + gCupCourseSelection) + 0x1E;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NEW_PACK: /* switch 3 */
                                    gControllerPak1State = 0;
                                    break;
                                default: /* switch 3 */
                                    gControllerPak1State = 0;
                                    break;
                            }
                        }
                        if (var_v1 != 0) {
                            play_sound2(SOUND_MENU_SELECT);
                            return;
                        }
                        if (gControllerPak1State == 0) {
                            switch (func_800B5F30()) { /* switch 2 */
                                case PFS_INVALID_DATA: /* switch 2 */
                                    arg0->cursor = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_FREE_BLOCKS_ERROR: /* switch 2 */
                                case PFS_NUM_FILES_ERROR:   /* switch 2 */
                                    arg0->cursor = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NOPACK: /* switch 2 */
                                case PFS_ERR_DEVICE: /* switch 2 */
                                    arg0->cursor = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_ID_FATAL: /* switch 2 */
                                    arg0->cursor = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                default: /* switch 2 */
                                    var_v1 = 1;
                                    arg0->cursor = 0x00000016;
                                    break;
                                case 0:
                                    break;
                            }
                            if (var_v1 != 0) {
                                play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                              (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                                func_800B6708();
                                arg0->cursor = func_800B6348((gCupSelection * 4) + gCupCourseSelection) + 0x1E;
                                play_sound2(SOUND_MENU_SELECT);
                                return;
                            }
                        }
                        if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                            arg0->cursor = 0x00000018;
                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                        } else if (gControllerPak1NumPagesFree >= 0x79) {
                            arg0->cursor = 0x00000020;
                            arg0->unk1C = 0;
                            play_sound2(SOUND_MENU_SELECT);
                        } else {
                            arg0->cursor = 0x00000018;
                            play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        func_8009DFE0(0x0000001E);
                        play_sound2(SOUND_MENU_OK_CLICKED);
                        func_800CA330(0x19U);
                        func_800CA388(0x19U);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                    }
                }
            }
            break;
        case 21: /* switch 1 */
        case 22: /* switch 1 */
        case 23: /* switch 1 */
        case 24: /* switch 1 */
        case 25: /* switch 1 */
        case 26: /* switch 1 */
        case 41: /* switch 1 */
            if (gControllerOne->buttonPressed & 0xD000) {
                arg0->cursor = 0x00000010;
                play_sound2(SOUND_MENU_GO_BACK);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
            }
            break;
        case 30: /* switch 1 */
        case 31: /* switch 1 */
            arg0->unk20 = (u32) arg0->cursor - 0x1E;
            if (func_800B639C((gCupSelection * 4) + gCupCourseSelection) != arg0->unk20) {
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) {
                    if (arg0->cursor >= 0x1F) {
                        arg0->cursor--;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = -1;
                    }
                }
                if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                    if (arg0->cursor < 0x1F) {
                        arg0->cursor++;
                        play_sound2(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->unk24 < 4.2) {
                            arg0->unk24 += 4.0;
                        }
                        arg0->unk8 = 1;
                    }
                }
            }
            if (gControllerOne->buttonPressed & 0x4000) {
                arg0->cursor = 0x00000010;
                play_sound2(SOUND_MENU_GO_BACK);
            } else if (gControllerOne->buttonPressed & 0x9000) {
                thing = &D_8018EE10[arg0->unk20];
                if (thing->ghostDataSaved == 0) {
                    arg0->cursor = 0x00000028;
                    arg0->unk1C = 0;
                } else if (func_800B63F0(arg0->unk20) == 0) {
                    arg0->cursor = 0x0000001A;
                } else {
                    arg0->cursor = 0x00000023;
                }
                play_sound2(SOUND_MENU_SELECT);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
            }
            break;
        case 32: /* switch 1 */
            if ((arg0->unk1C == 1) && (func_800B6A68() != 0)) {
                arg0->cursor = 0x00000019;
            } else {
                arg0->unk1C++;
                if (arg0->unk1C >= 2) {
                    arg0->cursor = 0x0000001E;
                }
            }
            break;
        case 35: /* switch 1 */
        case 36: /* switch 1 */
            if (((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x800) &&
                ((s32) (u32) arg0->cursor >= 0x24)) {
                arg0->cursor--;
                play_sound2(SOUND_MENU_CURSOR_MOVE);
                if (arg0->unk24 < 4.2) {
                    arg0->unk24 += 4.0;
                }
                arg0->unk8 = -1;
            }
            if ((gControllerOne->buttonPressed | gControllerOne->stickPressed) & 0x400) {
                if (arg0->cursor < 0x24) {
                    arg0->cursor++;
                    play_sound2(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                    arg0->unk8 = 1;
                }
            }
            if (gControllerOne->buttonPressed & 0x4000) {
                arg0->cursor = arg0->unk20 + 0x1E;
                play_sound2(SOUND_MENU_GO_BACK);
            } else if (gControllerOne->buttonPressed & 0x9000) {
                if (arg0->cursor == 0x00000024) {
                    arg0->cursor = 0x00000028;
                    arg0->unk1C = 0;
                    play_sound2(SOUND_MENU_SELECT);
                    if (arg0->unk24 < 4.2) {
                        arg0->unk24 += 4.0;
                    }
                } else {
                    arg0->cursor = arg0->unk20 + 0x1E;
                    play_sound2(SOUND_MENU_GO_BACK);
                }
            }
            break;
        case 40: /* switch 1 */
            if (arg0->unk1C == 1) {
                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                                  &gControllerPak1FileNote) != 0) {
                    arg0->cursor = 0x00000029;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (func_800B6178(arg0->unk20) != 0) {
                    arg0->cursor = 0x00000029;
                    play_sound2(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->unk1C++;
            if (arg0->unk1C >= 2) {
                arg0->cursor = 0x00000010;
            }
            break;
    }
}

void func_800AEC54(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) {
        case 0:
            arg0->column = (get_string_width(D_800E7780) / 2) + 0x140;
            arg0->row = 0x000000DA;
            arg0->cursor = 1;
            func_800C90F4(0U, (D_80162DE4 * 0x10) + 0x29008001);
            break;
        case 1:
            func_800A9208(arg0, 0x000000A0);
            if (arg0->column == 0x000000A0) {
                arg0->cursor = 2;
                arg0->unk1C = 0;
            }
            break;
        case 2:
            arg0->unk1C++;
            if (arg0->unk1C >= 0x3D) {
                arg0->cursor = 3;
                arg0->unk1C = 0;
            }
            break;
        case 4:
            arg0->unk1C++;
            if (arg0->unk1C >= 6) {
                arg0->type = 0;
                break;
            }
            // Purposeful fallthrough
        case 3:
            func_800A94C8(arg0, 0x000000A0, -1);
            if (((arg0->column + 0x14) == -(get_string_width(D_800E7780) / 2)) && (arg0->cursor == 3)) {
                arg0->cursor = 4;
            }
            break;
        default:
            break;
    }
}

void func_800AEDBC(struct_8018D9E0_entry* arg0) {
    if (arg0->unk1C != gTimeTrialDataCourseIndex) {
        arg0->unk1C = (s32) gTimeTrialDataCourseIndex;
        func_8009A594(arg0->D_8018DEE0_index, 0,
                      segmented_to_virtual_dupe_2(
                          D_800E7E34[gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]]));
        if (controller_pak_1_status() == PFS_NO_ERROR) {
            func_800B6708();
        } else {
            D_8018EE10[0].ghostDataSaved = 0;
            D_8018EE10[1].ghostDataSaved = 0;
        }
    }
}

void func_800AEE90(struct_8018D9E0_entry* arg0) {
    if (D_8018EDEC != 0xB) {
        arg0->visible = 0;
    } else {
        arg0->visible = 1;
    }
}

void func_800AEEBC(struct_8018D9E0_entry* arg0) {
    if (D_8018EDEC != 0xC) {
        arg0->visible = 0;
    } else {
        arg0->visible = 1;
    }
}

void func_800AEEE8(struct_8018D9E0_entry* arg0) {
    if (D_8018EDEC != 0xD) {
        arg0->visible = 0;
    } else {
        arg0->visible = 1;
    }
}

void func_800AEF14(struct_8018D9E0_entry* arg0) {
    if (playerHUD[PLAYER_ONE].raceCompleteBool != 0) {
        if ((u32) playerHUD[PLAYER_ONE].someTimer < (u32) (func_800B4E24(4) & 0xFFFFF)) {
            D_8018ED90 = 1;
        }
        arg0->type = 0;
    }
}

void func_800AEF74(struct_8018D9E0_entry* arg0) {
    switch (arg0->cursor) { /* irregular */
        case 0:
            if (D_80162DF8 == 1) {
                arg0->cursor = 1;
                arg0->unk1C = 0;
            } else if (playerHUD[PLAYER_ONE].raceCompleteBool == (s8) 1) {
                arg0->cursor = 2;
            }
            break;
        case 2:
            break;
        case 1:
            arg0->unk1C += 1;
            if (playerHUD[PLAYER_ONE].raceCompleteBool == 1) {
                arg0->cursor = 2;
            }
            break;
    }
}

void func_800AF004(struct_8018D9E0_entry* arg0) {
    UNUSED s32 temp_t1;

    switch (arg0->cursor) {
        case 0:
            arg0->unk1C += 3;
            if (arg0->unk1C >= 0x65) {
                arg0->unk1C = 0;
                arg0->cursor = 1;
                gCupSelection %= 4;
                gCCSelection %= 4;
                add_8018D9E0_entry(0x0000012C, 0, 0, 4);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            arg0->unk1C += 1;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 0;
                arg0->cursor++;
                add_8018D9E0_entry(arg0->cursor + 0x12B, 0, 0, 4);
            }
            break;
        case 5:
            arg0->unk1C += 1;
            if ((arg0->unk1C >= 0x65) &&
                ((gControllerFive->buttonPressed != 0) || (gControllerFive->stickPressed != 0))) {
                arg0->cursor = 6;
                arg0->unk1C = 0;
                if (D_802874D8.unk1D < 3) {
                    play_sound2(SOUND_MENU_OK_CLICKED);
                } else {
                    play_sound2(SOUND_ACTION_CONTINUE_UNKNOWN);
                }
            }
            break;
        case 6:
            func_8009DFE0(0x0000001E);
            func_800CA330(0x19U);
            func_800CA388(0x19U);
            arg0->cursor = 7;
            break;
        case 7:
        default:
            break;
    }
}

void func_800AF1AC(struct_8018D9E0_entry* arg0) {
    Unk_D_800E70A0* temp_v0_2;
    s32 idx = arg0->type - 0x12C;

    switch (arg0->cursor) { /* irregular */
        case 0:
            temp_v0_2 = &D_800E7458[idx];
            arg0->column = temp_v0_2->column;
            arg0->row = temp_v0_2->row;
            arg0->cursor = 1;
            break;
        case 1:
            temp_v0_2 = &D_800E7480[idx];
            func_800A91D8(arg0, temp_v0_2->column, temp_v0_2->row);
            if ((arg0->column == temp_v0_2->column) && (arg0->row == temp_v0_2->row)) {
                arg0->cursor = 2;
            }
            break;
        case 2:
            break;
    }
}

void func_800AF270(struct_8018D9E0_entry* arg0) {
    s32 temp_v1;
    s32 sp30;
    s32 temp_v0;
    Unk_D_800E70A0* thing;

    temp_v1 = arg0->type - 0x12C;
    sp30 = D_802874D8.unk1E;
    temp_v0 = D_800EFD64[sp30];
    switch (arg0->cursor) {
        case 0:
            thing = &D_800E7458[temp_v1];
            arg0->column = thing->column;
            arg0->row = thing->row;
            arg0->cursor = 1;
            break;
        case 1:
            thing = &D_800E7480[temp_v1];
            func_800A91D8(arg0, thing->column, thing->row);
            if ((arg0->column == thing->column) && (arg0->row == thing->row)) {
                arg0->cursor = 2;
                arg0->unk20 = 0;
            }
            break;
        case 2:
            arg0->unk20++;
            if (arg0->unk20 >= 0x1F) {
                if (D_802874D8.unk1D >= 3) {
                    arg0->cursor = 4;
                    func_800CA0B8();
                    func_800C90F4(0U, (sp30 * 0x10) + 0x29008003);
                    func_800CA0A0();
                } else {
                    arg0->cursor = 3;
                    func_8009A640(arg0->D_8018DEE0_index, 0, sp30,
                                  segmented_to_virtual_dupe_2(gCharacterCelebrateAnimation[temp_v0]));
                    func_800CA0B8();
                    func_800C90F4(0U, (sp30 * 0x10) + 0x29008007);
                    func_800CA0A0();
                }
            }
            break;
        case 3:
            if (D_8018DEE0[arg0->D_8018DEE0_index].sequenceIndex >= D_800E8440[temp_v0]) {
                func_8009A640(arg0->D_8018DEE0_index, 0, sp30, segmented_to_virtual_dupe_2(D_800E83A0[temp_v0]));
                arg0->cursor = 4;
            }
            break;
        case 4:
            break;
    }
}

void func_800AF480(struct_8018D9E0_entry* arg0) {
    s32 idx = arg0->type - 0x190;

    if ((D_802850C0[idx].slideDirection == 0) || (D_802850C0[idx].slideDirection != 1)) {
        func_800AF4DC(arg0);
    } else {
        func_800AF740(arg0);
    }
}

void func_800AF4DC(struct_8018D9E0_entry* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    struct_802850C0_entry* temp_v1;

    temp_v0 = arg0->type - 0x190;
    temp_v1 = &D_802850C0[temp_v0];
    arg0->row = temp_v1->row;
    switch (arg0->cursor) {
        case 0:
            arg0->column = temp_v1->startingColumn;
            arg0->cursor = 1;
            arg0->unk20 = temp_v1->columnExtra + (get_string_width(D_802854B0[temp_v0]) * temp_v1->textScaling / 2);
            /* fallthrough */
        case 1:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (s32) (arg0->unk20 - arg0->column) / 4;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 8;
            }
            arg0->unk24 = (arg0->unk1C * 0.05) + 1.0;
            if (arg0->column >= (arg0->unk20 - 0x14)) {
                arg0->cursor = 2;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 2:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (arg0->unk20 - arg0->column) / 4;
            arg0->D_8018DEE0_index += 1;
            arg0->unk24 = ((arg0->D_8018DEE0_index - 0xA) * 0.0085 * (arg0->D_8018DEE0_index - 0xA)) + 0.4;
            if ((arg0->D_8018DEE0_index >= 9) && ((f64) arg0->unk24 > 1)) {
                arg0->unk24 = 1.0f;
                arg0->cursor = 3;
            }
            break;
        case 3:
            if ((u8) D_8018ED91 != 0) {
                arg0->cursor = 4;
            }
            break;
        case 4:
            func_800A94C8(arg0, arg0->unk20, 1);
            if (arg0->row > 480.0) {
                arg0->type = 0;
            }
            break;
        default:
            break;
    }
}

void func_800AF740(struct_8018D9E0_entry* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    struct_802850C0_entry* temp_v1;

    temp_v0 = arg0->type - 0x190;
    temp_v1 = &D_802850C0[temp_v0];
    arg0->row = temp_v1->row;
    switch (arg0->cursor) {
        case 0:
            arg0->column = temp_v1->startingColumn;
            arg0->cursor = 1;
            arg0->unk20 = temp_v1->columnExtra - (get_string_width(D_802854B0[temp_v0]) * temp_v1->textScaling / 2);
            /* fallthrough */
        case 1:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (s32) (arg0->column - arg0->unk20) / 4;
            if (arg0->unk1C >= 9) {
                arg0->unk1C = 8;
            }
            arg0->unk24 = (arg0->unk1C * 0.05) + 1.0;
            if ((arg0->unk20 + 0x14) >= arg0->column) {
                arg0->cursor = 2;
                arg0->D_8018DEE0_index = 0;
            }
            break;
        case 2:
            func_800A9208(arg0, arg0->unk20);
            arg0->unk1C = (arg0->column - arg0->unk20) / 4;
            arg0->D_8018DEE0_index += 1;
            arg0->unk24 = ((arg0->D_8018DEE0_index - 0xA) * 0.0085 * (arg0->D_8018DEE0_index - 0xA)) + 0.4;
            if ((arg0->D_8018DEE0_index >= 9) && ((f64) arg0->unk24 > 1)) {
                arg0->unk24 = 1.0f;
                arg0->cursor = 3;
            }
            break;
        case 3:
            if ((u8) D_8018ED91 != 0) {
                arg0->cursor = 4;
            }
            break;
        case 4:
            func_800A94C8(arg0, arg0->unk20, -1);
            if (arg0->row > 480.0) {
                arg0->type = 0;
            }
            break;
        default:
            break;
    }
}
