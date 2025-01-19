#ifndef OBJECTS_H
#define OBJECTS_H

#include "spline.h"
#include <common_structs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OBJECT_LIST_SIZE 0x226
#define SOME_OBJECT_INDEX_LIST_SIZE 32

typedef struct {
    /* 0x00 */ f32 sizeScaling;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f origin_pos;
    /* 0x1C */ Vec3f unk_01C;
    /* 0x28 */ Vec3f offset;
    /* 0x34 */ f32 unk_034;
    /* 0x38 */ Vec3f velocity; // acceleration
    /* 0x44 */ f32 surfaceHeight;
    /* 0x48 */ s32 unk_048;
    /* 0x4C */ s32 unk_04C;
    /* 0x50 */ s32 timer;
    /* 0x54 */ s32 status;
    /* 0x58 */ s32 unk_058;
    /* 0x5C */ s32 unk_05C;
#ifndef TARGET_N64
    /* 0x60 */ const char* activeTLUT;
    /* 0x64 */ const char* activeTexture;
#else
    /* 0x60 */ u8* activeTLUT;
    /* 0x64 */ u8* activeTexture;
#endif
    /**
     * "list" is something of a misnomer for the names here
     * they can be pointers to just 1 tlut/texture, but it is common for one or the other
     * to be a pointer to an array of tluts/textures.
     **/
    /* 0x68 */ u8* tlutList; // I feel like this should actually be `u8 (*tlutList)[512]`, but that causes mismatches
#ifndef TARGET_N64
    /* 0x6C */ const char** textureList;
#else
    /* 0x6C */ u8* textureList;
#endif
    /* 0x70 */ Gfx* model;
    /* 0x74 */ Vtx* vertex;
    /* 0x78 */ s8 unk_078[0x04];
    /* 0x7C */ SplineControlPoint* controlPoints;
    /* 0x80 */ SplineData* spline;
    /* 0x84 */ s16 unk_084[0xA];
    /* 0x98 */ u16 animationTimer;
    /* 0x9A */ u16 unk_09A;
    /* 0x9C */ s16 unk_09C;
    /* 0x9E */ s16 unk_09E;
    /* 0xA0 */ s16 primAlpha;
    /* 0xA2 */ s16 unk_0A2;
    /* 0xA4 */ s16 type;
    /* 0xA6 */ s16 state;
    /* 0xA8 */ s16 unk_0A8;
    /* 0xAA */ s16 unk_0AA;
    /* 0xAC */ s16 unk_0AC;
    /* 0xAE */ s16 unk_0AE;
    /* 0xB0 */ s16 unk_0B0;
    /* 0xB2 */ Vec3su orientation; // rotation, I think
    /* 0xB8 */ Vec3su unk_0B8;
    /* 0xBE */ Vec3su direction_angle;
    /* 0xC4 */ u16 unk_0C4;
    /* 0xC6 */ u16 unk_0C6;
    /* 0xC8 */ u16 boundingBoxSize;
    /* 0xCA */ s8 unk_0CA;
    /* 0xCB */ s8 isTimerActive;
    /* 0xCC */ s8 unk_0CC;
    /* 0xCD */ s8 unk_0CD;
    /* 0xCE */ s8 unk_0CE;
    /* 0xCF */ s8 unk_0CF;
    /* 0xD0 */ s8 unk_0D0;
    /* 0xD1 */ s8 unk_0D1;
    /* 0xD2 */ s8 textureListIndex;
    /* 0xD3 */ s8 unk_0D3;
    /* 0xD4 */ s8 unk_0D4;
    /* 0xD5 */ u8 unk_0D5;
    /* 0xD6 */ u8 unk_0D6;
    /* 0xD7 */ u8 unk_0D7;
    /* 0xD8 */ u8 unk_0D8;
    /* 0xD9 */ u8 textureWidth;
    /* 0xDA */ u8 textureHeight;
    /* 0xDB */ u8 unk_0DB;
    /* 0xDC */ u8 unk_0DC;
    /* 0xDD */ u8 unk_0DD;
    /* 0xDE */ s8 unk_0DE;
    /* 0xDF */ u8 unk_0DF;
} Object; // size = 0xE0

extern Object gObjectList[];

typedef struct {
    /* 0x00 */ f32 sizeScaling;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f origin_pos;
    /* 0x1C */ Vec3f unk_01C;
    /* 0x28 */ Vec3f offset;
    /* 0x34 */ f32 unk_034;
    /* 0x38 */ Vec3f velocity;
    /* 0x44 */ f32 unk_044;
    /* 0x48 */ s32 unk_048;
    /* 0x4C */ s32 unk_04C;
    /* 0x50 */ s32 timer;
    /* 0x54 */ s32 status;
    /* 0x58 */ s32 unk_058;
    /* 0x5C */ s32 unk_05C;
    /* 0x60 */ u8* activeTLUT;
    /* 0x64 */ u8* activeTexture;
    /**
     * "list" is something of a misnomer for the names here
     * they can be pointers to just 1 tlut/texture, but it is common for one or the other
     * to be a pointer to an array of tluts/textures.
     **/
    /* 0x68 */ u8* tlutList; // I feel like this should actually be `u8 (*tlutList)[512]`, but that causes mismatches
    /* 0x6C */ u8* textureList;
    /* 0x70 */ Gfx* model;
    /* 0x74 */ Vtx* vertex;
    /* 0x78 */ s8 unk_078[0x04];
    /* 0x7C */ Vec4s* unk_07C;
    /* 0x80 */ Vec4s* unk_080; // unk_080[][4]?
    /* 0x84 */ s16 unk_084[0xA];
    /* 0x98 */ u16 goldenMushroomTimer;
    /* 0x9A */ u16 unk_09A;
    /* 0x9C */ s16 unk_09C;
    /* 0x9E */ s16 unk_09E;
    /* 0xA0 */ s16 primAlpha;
    /* 0xA2 */ s16 unk_0A2;
    /* 0xA4 */ s16 currentItem;
    /* 0xA6 */ s16 itemDisplayState; // Usually a state tracker
    /* 0xA8 */ s16 unk_0A8;
    /* 0xAA */ s16 unk_0AA;
    /* 0xAC */ s16 unk_0AC;
    /* 0xAE */ s16 unk_0AE;
    /* 0xB0 */ s16 unk_0B0;
    /* 0xB2 */ Vec3su orientation; // rotation, I think
    /* 0xB8 */ Vec3su unk_0B8;
    /* 0xBE */ Vec3su direction_angle;
    /* 0xC4 */ u16 unk_0C4;
    /* 0xC6 */ u16 unk_0C6;
    /* 0xC8 */ u16 unk_0C8;
    /* 0xCA */ s8 unk_0CA;
    /* 0xCB */ s8 isTimerActive;
    /* 0xCC */ s8 unk_0CC;
    /* 0xCD */ s8 unk_0CD;
    /* 0xCE */ s8 unk_0CE;
    /* 0xCF */ s8 unk_0CF;
    /* 0xD0 */ s8 unk_0D0;
    /* 0xD1 */ s8 unk_0D1;
    /* 0xD2 */ s8 textureListIndex;
    /* 0xD3 */ s8 unk_0D3;
    /* 0xD4 */ s8 unk_0D4;
    /* 0xD5 */ u8 unk_0D5;
    /* 0xD6 */ u8 unk_0D6;
    /* 0xD7 */ u8 unk_0D7;
    /* 0xD8 */ u8 unk_0D8;
    /* 0xD9 */ u8 textureWidth;
    /* 0xDA */ u8 textureHeight;
    /* 0xDB */ u8 unk_0DB;
    /* 0xDC */ u8 unk_0DC;
    /* 0xDD */ u8 unk_0DD;
    /* 0xDE */ s8 unk_0DE;
    /* 0xDF */ u8 unk_0DF;
} ItemWindowObjects; // size = 0xE0

// This are other lists of indices in gObjectList.

/**
 * @brief Status flags for objects
 */
#define VISIBLE 0x00040000

/**
 * Use unknown. An object is reserved and its index is saved to
 * this variable, but it appears to go unreferenced
 **/
extern s32 D_80183DA0;

//! Lakitu?
extern s32 gIndexLakituList[];

#define DELETED_OBJECT_ID -1
#define NULL_OBJECT_ID -1

//! Appears to be a list of object list indices for the Item Window part of the HUD
extern s32 gItemWindowObjectByPlayerId[];

// Used for loop bounds when accessing indexObjectList1
extern s16 D_80165750;

/**
 * These seem to be limits on different object types in Moo Moo Farm
 * See init_course_object in code_8006E9C0.c
 * Maybe max number of active moles in a given group of moles?
 */
extern s32 D_8018D1C8;
extern s32 D_8018D1D0;
extern s32 D_8018D1D8;

//! Limit on some object type (ice chips?) in Sherbet Land
extern s32 D_8018D3C0;

extern Collision D_8018C0B0[];

/**
 * indexObjectList1, indexObjectList2, indexObjectList3, and indexObjectList4 are all lists of indices in gObjectList.
 * init_object_list_index initializes them in such a way that the indicies in each list are not adjacent.
 * First indexObjectList1 gets an unused index, then indexObjectList2, then indexObjectList3, then indexObjectList4, and
 *then it loops.
 *
 * The objects found at the indices in each list appears to be course dependent
 **/

#define NUM_YV_FLAG_POLES 4

typedef struct {
    /* 0x0 */ Vec3s pos;
    /* 0x6 */ u16 rot;
} YVFlagPoleSpawn; // size = 0x8;

extern YVFlagPoleSpawn gFlagpoleSpawns[];

#define NUM_CRABS 0xA

typedef struct {
    /* 0x0 */ s16 startX;
    /* 0x2 */ s16 patrolX;
    /* 0x4 */ s16 startZ;
    /* 0x6 */ s16 patrolZ;
} CrabSpawn;

extern CrabSpawn gCrabSpawns[];

#define NUM_THWOMPS_50CC 8
#define NUM_THWOMPS_100CC_EXTRA 11
#define NUM_THWOMPS_150CC 12

extern Vec3f D_800E6734[];

typedef struct {
    /* 0x0 */ s16 startX;
    /* 0x2 */ s16 startZ;
    /* 0x4 */ s16 unk_4; // Group Id?
    /* 0x6 */ s16 unk_6; // Starting State?
} ThwompSpawn;           // size = 0x8;

extern ThwompSpawn gThomwpSpawns50CC[];
extern ThwompSpawn gThwompSpawns100CCExtra[];
extern ThwompSpawn gThomwpSpawns150CC[];
extern ThwompSpawn* gThowmpSpawnList;

extern s16 gNumActiveThwomps;

#define NUM_NEON_SIGNS 10
#define NUM_CHAIN_CHOMPS 3

#define NUM_PENGUINS 15

/**
 * Snowmen bodies in FrappeSnowland
 * Crabs in Koopa Troopa Beach
 * Hot air balloon in Luigi Raceway?
 * Neon signs in Rainbow Road?
 * Thwomps in Bower's Castle?
 * Penguins in Sherbet Land?
 * Flag Poles in Yoshi Valley?
 */
extern s32 indexObjectList1[];

#define NUM_SEAGULLS 10
#define NUM_SNOWMEN 19
#define NUM_HEDGEHOGS 15

typedef struct {
    /* 0x0 */ Vec3s pos;
    /* 0x6 */ s16 unk_6; // Group Id?
} SnowmanSpawn;          // size = 0x8;

extern SnowmanSpawn gSnowmanSpawns[];

typedef struct {
    /* 0x0 */ Vec3s pos;
    /* 0x6 */ s16 unk_06; // Group Id?
} HegdehogSpawn;          // size = 0x8;

extern HegdehogSpawn gHedgehogSpawns[];
extern Vec3s gHedgehogPatrolPoints[];

/**
 * Snowmen heads in Frappe Snowland
 * Chain Chomps in RaindbowRoad?
 * Trophy in award ceremony?
 * Seagulls in Koopa Troopa Beach?
 * Hedgehogs in Yoshi Valley?
 * Spawn for big fire breath in Bowser's Castle
 */
extern s32 indexObjectList2[];

#define NUM_BOOS 10
#define NUM_FIRE_BREATHS 4

extern Vec3s gFireBreathsSpawns[];

// These should really, really be Vec3s arrays, but that doesn't match
extern s16 D_800E5740[];
extern s16 D_800E579C[];
extern s16 D_800E57F8[];

/**
 * Boos in Banshee Boardwalk
 * Spawners for the 4 small fire breaths inside Bowser's Castle
 */
extern s32 indexObjectList3[];

/**
 * Unused list of object indices
 */
extern s32 indexObjectList4[];

#define gObjectParticle1_SIZE 128
#define NUM_GROUP1_MOLES 8
#define NUM_GROUP2_MOLES 11
#define NUM_GROUP3_MOLES 12
#define NUM_TOTAL_MOLES (NUM_GROUP1_MOLES + NUM_GROUP2_MOLES + NUM_GROUP3_MOLES)
#define NUM_SNOWFLAKES 0x32

typedef union {
    Vec3s asVec3sList[NUM_TOTAL_MOLES];
    s16 asFlatList[NUM_TOTAL_MOLES * 3];
} MoleSpawnUnion;
extern MoleSpawnUnion gMoleSpawns;

/**
 * Exact use unknown, something related to the mole groups
 * Maybe be indicating that a given mole in a given group is active?
 */
extern s8 D_8018D198[];
extern s8 D_8018D1A8[];
extern s8 D_8018D1B8[];

//! Unknown object index, only set for Kalimari Desert, never read
extern s32 D_8018CF10;

/**
 * List of object list indices used for:
 *   Moles in Moo Moo Farm
 *   Snow flakes in Frappe Snowland
 *   Segments of the fire breath from the statues in Bowser's Castle
 *   Potentially other things
 */
extern s32 gObjectParticle1[];

//! Next free spot in gObjectParticle1? Wraps back around to 0 if it gets bigger than gObjectParticle1_SIZE
extern s32 gNextFreeObjectParticle1;

//! Used for cycling through snowflakes in func_80078790
extern s16 D_8018D174;

#define gObjectParticle2_SIZE 128

/**
 * List of object list indices used for:
 *   Bats in Banshee's Boardwalk (but only 1 player mode?)
 */
extern s32 gObjectParticle2[];

//! Next free spot in gObjectParticle2? Wraps back around to 0 if it gets bigger than gObjectParticle2_SIZE
extern s32 gNextFreeObjectParticle2;

//! Controls number of come object type placed into gObjectParticle2 on Frappe Snowland. So, maybe snowmen/snowflakes?
extern s32 D_8018D3BC;

#define gObjectParticle3_SIZE 128
extern s32 gObjectParticle3[];
//! Next free spot in gObjectParticle3?
extern s32 gNextFreeObjectParticle3;
extern s16 D_80165730;

//! Tracking a count of some object type, don't know what object type yet
extern s16 D_80165738;

#define gObjectParticle4_SIZE 0x40

#define NUM_TORCHES 8

// This should really be `extern Vec3s gTorchSpawns[];`
//! @todo fix this extern
extern s16 gTorchSpawns[];

/**
 * List of object list indices. Used both for the fires in the DK Jungle cave
 * and, seemingly for the trail that shells leave behind them.
 * I think they're using the same texture, which would explain the dual use
 */
extern s32 gObjectParticle4[];

//! Next free spot in gObjectParticle4? Wraps back around to 0 if it gets bigger than gObjectParticle4_SIZE
extern s32 gNextFreeObjectParticle4;

// Maximum number of leaves that can be falling?
#define gLeafParticle_SIZE 0x40
// Number of leaves to spawn each bonk?
#define gLeafParticle_SPAWN_SIZE 0x14

/**
 * Seemingly a list of object list indices used for the leaves that sometimes fall
 * trees when you bonk into them
 */
extern s32 gLeafParticle[];

//! Next free spot in gLeafParticle? Wraps back around to 0 if it gets bigger than gLeafParticle_SIZE
extern s32 gNextFreeLeafParticle;

// Struct used for clouds and stars
// See `func_80070328` and `func_80070250` for cloud initialization
// See `func_800704A0` and `func_800703E0` for star initialization
typedef struct {
    // rotY and posY seem relative to the camera.
    // See `func_800788F8` to see how rotY is used to decide whether and where to display clouds/stars
    // See `func_80078A44` and `func_800789AC` to see how stars and clouds (respectively) are looped over
    /* 0x00 */ u16 rotY;
    /* 0x02 */ u16 posY;
    // Can be bigger than 100!
    /* 0x04 */ u16 scalePercent;
    // There are a couple different cloud shapes, this decides which one is used
    // For stars, this is always 0 since they don't have multiple textures
    /* 0x06 */ u16 subType;
} StarData, CloudData; // size = 0x8

#define D_8018CC80_SIZE 0x64

/**
 * List of object list indices used by the clouds and stars in some stages
 * Also used for snowflakes like gObjectParticle1? Not sure what's up with that
 */
extern s32 D_8018CC80[];

//! Number of used spots in D_8018CC80?
extern s32 D_8018D1F0;

//! Next free spot in D_8018CC80?
extern s32 D_8018D1F8;

//! Also next free spot in D_8018CC80?
extern s16 D_8018D17C;

//! Something related to the display of the clouds/stars?
extern s8 D_8018D230;

//! Some sort of limiter on how many of some object type can spawn
extern s32 D_8018D3C4;

#ifdef __cplusplus
}
#endif

#endif
