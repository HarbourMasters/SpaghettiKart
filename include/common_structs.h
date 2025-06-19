#ifndef _COMMON_STRUCTS_H_
#define _COMMON_STRUCTS_H_

#ifndef TARGET_N64
#include <libultraship.h>
#else
#include <ultra64.h>
#endif

typedef f32 Vec3f[3];
typedef f32 Vec4f[4];

typedef s32 Vec3iu[3];

typedef s16 Vec3s[3];
typedef u16 Vec3su[3];
typedef s16 Vec4s[4];

typedef f32 Mat3[3][3];
typedef f32 Mat4[4][4];

// might not be real, used by func_8002C954
typedef struct {
    f32 x, y, z;
} Vec3fs;

// This was added as a silly idea:
// In the data to use "A, B, Z, R" instead of hex numbers.
typedef enum { A = 0x80, B = 0x40, Z = 0x20, R = 0x10 } GhostController;

/***  types.h  ***/

typedef struct {
    /* 0x0 */ s16 pathPointStart;
    /* 0x2 */ s16 pathPointEnd;
    /* 0x4 */ s32 type;
} CPUBehaviour; // size = 0x8

enum SpTaskState {
    SPTASK_STATE_NOT_STARTED,
    SPTASK_STATE_RUNNING,
    SPTASK_STATE_INTERRUPTED,
    SPTASK_STATE_FINISHED,
    SPTASK_STATE_FINISHED_DP
};

struct SPTask {
    /*0x00*/ OSTask task;
    /*0x40*/ OSMesgQueue* msgqueue;
    /*0x44*/ OSMesg msg;
    /*0x48*/ enum SpTaskState state;
}; // size = 0x4C, align = 0x8

struct VblankHandler {
    OSMesgQueue* queue;
    OSMesg msg;
};

struct D_80150158 {
    s16 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct Controller {
    s16 rawStickX;
    s16 rawStickY;
    s16 rightRawStickX;
    s16 rightRawStickY;
    u16 button;          // HeldButton
    u16 buttonPressed;   // OnTriggered
    u16 buttonDepressed; // OffTriggered
    u16 stickDirection;
    u16 stickPressed;   // OffTriggered
    u16 stickDepressed; // OnTriggered
};

// Camera path struct? Or something like that. For GP race won scene?
struct UnkStruct_80287500 {
    Vec3f unk0; // xyz coordinates?
    f32 unkC;
    f32 unk10;
    f32 unk14; // rotation?
    s32 unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
};

struct UnkStruct_800DDB40 {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u16 unk24;
    u16 unk26;
};

struct UnkStruct_802B53C8 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};
// end math util structs

struct UnkStruct_800DDB68 {
    s32* D_800ED600;
    s32* D_800ED608;
    s32* D_800ED610;
    s32* D_800ED618;
    s32* D_800ED620;
    s32* D_800ED628;
    s32* D_800ED630;
    s32* D_800ED638;
};
/*** Types.h end ***/

struct Skybox {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

typedef struct {
    u8 button;
    s8 frameDuration;
    s8 stickY;
    s8 stickX;
} StaffGhost;
// 80160ADC Banana's remaining

typedef struct {
    /* 0x00 */ u16 unk30;
    /* 0x02 */ u16 unk32;
    /* 0x04 */ u16 unk34;
    /* 0x06 */ u16 meshIndexYX;
    /* 0x08 */ u16 meshIndexZY;
    // This may be an index to the tilemap?
    /* 0x0A */ u16 meshIndexZX;
    /* 0x0C */ Vec3f surfaceDistance; // Appears to be distance from actor to surface for zx, yx, and zy planes.
    /* 0x18 */ Vec3f unk48;
    /* 0x24 */ Vec3f unk54;
    /* 0x30 */ Vec3f orientationVector;
    /* 0x3C */ f32 unk6C;
} Collision;

typedef struct {
    /* 0x00 */ Vec3f unk_000;
    /* 0x0C */ f32 unk_00C;
    /* 0x10 */ u16 unk_010;
    /* 0x12 */ u16 unk_012;
    /* 0x14 */ f32 unk_014;
    /* 0x18 */ f32 unk_018;
    /* 0x1C */ s16 unk_01C;
    /* 0x1E */ s16 unk_01E;
    /* 0x20 */ s16 unk_020;
    /* 0x22 */ s16 unk_022;
    /* 0x24 */ f32 unk_024;
    /* 0x28 */ f32 unk_028;
    /* 0x2C */ s16 unk_02C;
    /* 0x2E */ s16 unk_02E;
    /* 0x30 */ s16 unk_030;
    /* 0x32 */ s16 unk_032;
    /* 0x34 */ s16 unk_034;
    /* 0x36 */ s16 unk_036;
    /* 0x38 */ s16 unk_038;
    /* 0x3A */ s16 unk_03A;
    /* 0x3C */ s16 unk_03C;
    /* 0x3E */ s16 unk_03E;
    /* 0x40 */ s16 unk_040;
    /* 0x42 */ s16 unk_042;
    /* 0x44 */ s16 unk_044;
    /* 0x46 */ s16 unk_046;
} UnkPlayerStruct258; // size = 0x48

// This struct is almost identical to the GBI Vtx_t type,
// except that its missing the "flag" member.
typedef struct {
    s16 ob[3]; /* x, y, z */
    s16 tc[2]; /* texture coord */
    s8 ca[4];  /* color & alpha */
} CourseVtx;

/*
This struct has been copied (with only minor modifications) from
https://github.com/micro500/mariokart64/wiki/Surface-Map
on January 23rd, 2022
The original author is assumed to be RenaKunisaki
*/
typedef struct {
    /* 0x00 */ u16 flags;
    // Top bytes is a collections of flags, bottom byte is of unknown purpose
    // bit 7: 1 = only tangible if landed on, not if driven onto?
    //       very weird. game crashes sometimes when playing with this.
    // bit 6: 1 = Lakitu can drop you here (XXX verify)
    // bit 4: 1 = out of bounds
    // bit 3: 1 = player tumbles upon contact (may fall right through)
    /* 0x02 */ u16 surfaceType;

    // For AABB bounding-box style collision. Box style collision is cheaper than checking each vtx.
    /* 0x04 */ s16 minX;  // Minimum x coordinate
    s16 minY;             // Minimum y coordinate
    s16 minZ;             // Minimum z coordinate
    s16 maxX;             // Maximum x coordinate
    s16 maxY;             // Maximum y coordinate
    /* 0x0A */ s16 maxZ;  // Maximum z coordinate
    /* 0x10 */ Vtx* vtx1; // pointer to the 3 vertices of this poly
    Vtx* vtx2;
    Vtx* vtx3;
    // Face normal. Should really be Vec3f normal
    /* 0x1C */ f32 normalX;
    /* 0x20 */ f32 normalY;
    /* 0x24 */ f32 normalZ;
    /* 0x28 */ f32 distance;
} CollisionTriangle; // size = 0x2C

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 surfaceType; // Surface type that the tyre is touching.
    /* 0x0D */ u8 surfaceFlags;
    /* 0x0E */ u16 collisionMeshIndex; // Index into gCollisionMesh
                                       // Height of tyre attached to ground. When flying it floats with the kart.
    /* 0x10 */ f32 baseHeight;
    // Something lighting related. 1 when in a shaded region, 2 when in a tree's shadow
    // 3 when getting crushed by a whomp, but curiously only the front left tyre will ever have this value
    /* 0x14 */ s32 unk_14;
} KartTyre; // size = 0x18

#define FRONT_LEFT 0
#define FRONT_RIGHT 1
#define BACK_LEFT 2
#define BACK_RIGHT 3

struct UnkPlayerInner {
    /* 0xDB4 */ s16 unk0;
    /* 0xDB6 */ s16 unk2;
    /* 0xDB8 */ f32 unk4;
    /* 0xDBC */ f32 unk8;
    /* 0xDC0 */ f32 unkC;
    /* 0xDC4 */ f32 unk10;
    /* 0xDC8 */ f32 unk14;
    /* 0xDCC */ s16 unk18;
    /* 0xDCE */ s16 unk1A;
    /* 0xDD0 */ s16 unk1C;
    /* 0xDD2 */ s16 unk1E;
    /* 0xDD4 */ s16 unk20;
};

typedef struct {
    /* 0x0000 */ u16 type; // playerType?
    /* 0x0002 */ u16 unk_002;
    /* 0x0004 */ s16 currentRank;
    /* 0x0006 */ u16 unk_006;
    /* 0x0008 */ s16 lapCount;
    /* 0x000A */ char unk_00A[0x2];
    /* 0x000C */ s32 soundEffects;    // Bitflag.
    /* 0x0010 */ s16 currentItemCopy; // Has no effect on what item the players has, It is just a synced copy
    /* 0x0012 */ s16 unk_012;
    /* 0x0014 */ Vec3f pos;
    /* 0x0020 */ Vec3f oldPos;
    /* 0x002C */ Vec3s rotation;
    /* 0x0032 */ char unk_032[0x2];
    /* 0x0034 */ Vec3f velocity;
    /* 0x0040 */ s16 unk_040;
    /* 0x0042 */ s16 unk_042;
    /* 0x0044 */ s16 unk_044;
    /* 0x0046 */ u16 unk_046;
    /* 0x0048 */ Vec4s unk_048;
    /* 0x0050 */ Vec4s unk_050;
    /* 0x0058 */ f32 unk_058;
    /* 0x005C */ f32 unk_05C;
    /* 0x0060 */ f32 unk_060;
    /* 0x0064 */ Vec3f unk_064;
    /* 0x0070 */ f32 boundingBoxSize;
    /* 0x0074 */ f32 unk_074;
    /* 0x0078 */ s16 unk_078;
    /* 0x007A */ s16 unk_07A;
    /* 0x007C */ s32 unk_07C;
    /* 0x0080 */ f32 boostPower;
    /* 0x0084 */ f32 unk_084;
    /* 0x0088 */ f32 unk_088;
    /* 0x008C */ f32 unk_08C;
    /* 0x0090 */ f32 unk_090;
    /* 0x0094 */ f32 speed;
    /* 0x0098 */ f32 unk_098;
    /* 0x009C */ f32 currentSpeed;
    /* 0x00A0 */ f32 unk_0A0;
    /* 0x00A4 */ f32 unk_0A4;
    /* 0x00A8 */ s16 unk_0A8;
    /* 0x00AA */ s16 unk_0AA;
    /* 0x00AC */ s16 unk_0AC;
    /* 0x00AE */ s16 unk_0AE;
    /* 0x00B0 */ s16 unk_0B0;
    /* 0x00B2 */ s16 unk_0B2;
    /* 0x00B4 */ u16 unk_0B4;
    /* 0x00B6 */ u16 unk_0B6;
    /* 0x00B8 */ f32 unk_0B8;
    /* 0x00BC */ u32 effects;
    /* 0x00C0 */ s16 unk_0C0;
    /* 0x00C2 */ s16 unk_0C2;
    /* 0x00C4 */ s16 slopeAccel;
    /* 0x00C6 */ s16 unk_0C6;
    /* 0x00C8 */ s16 unk_0C8;
    /* 0x00CA */ s16 unk_0CA;
    /* 0x00CC */ Vec4s unk_0CC;
    /* 0x00D4 */ Vec4s unk_0D4;
    /* 0x00DC */ s16 boostTimer;
    /* 0x00DE */ u16 unk_0DE;
    /* 0x00E0 */ s16 unk_0E0;
    /* 0x00E2 */ s16 unk_0E2;
    /* 0x00E4 */ f32 unk_0E4;
    /* 0x00E8 */ f32 unk_0E8;
    /* 0x00EC */ f32 kartHopVelocity;
    /* 0x00F0 */ f32 kartHopJerk;
    /* 0x00F4 */ f32 kartHopAcceleration;
    /* 0x00F8 */ u16 surfaceType;
    /* 0x00FA */ s16 unk_0FA;
    /* 0x00FC */ f32 kartFriction;
    /* 0x0100 */ f32 kartGravity;
    /* 0x0104 */ f32 unk_104;
    /* 0x0108 */ f32 unk_108;
    /* 0x010C */ s16 unk_10C;
    /* 0x010E */ char unk_10E[0x2];
    /* 0x0110 */ Collision collision;
    /* 0x0150 */ Mat3 unk_150;
    /* 0x0174 */ Mat3 orientationMatrix;
    /* 0x0198 */ KartTyre tyres[4];
    /* 0x01F8 */ f32 unk_1F8;
    /* 0x01FC */ f32 unk_1FC;
    /* 0x0200 */ u32 unk_200; // May be s32. but less casting required if u32
    /* 0x0204 */ s16 unk_204;
    /* 0x0206 */ s16 unk_206;
    /* 0x0208 */ f32 unk_208;
    /* 0x020C */ f32 unk_20C;
    /* 0x0210 */ f32 unk_210;
    /* 0x0214 */ f32 topSpeed;
    /* 0x0218 */ f32 unk_218;
    /* 0x021C */ f32 unk_21C;
    /* 0x0220 */ s16 nearestPathPointId; // ??
    /* 0x0222 */ s16 unk_222;
    /* 0x0224 */ f32 size;
    /* 0x0228 */ s16 unk_228;
    /* 0x022A */ s16 unk_22A;
    /* 0x022C */ f32 previousSpeed;
    /* 0x0230 */ f32 unk_230;
    /* 0x0234 */ s16 unk_234;
    /* 0x0236 */ s16 unk_236;
    /* 0x0238 */ s16 unk_238;
    /* 0x023A */ s16 unk_23A;
    /* 0x023C */ f32 unk_23C;
    /* 0x0240 */ s32 tyreSpeed;
    /* 0x0244 */ u16 animFrameSelector[4]; // [0] Active texture group
    /* 0x024C */ u16 animGroupSelector[4]; // Based on screen
    /* 0x0254 */ u16 characterId;
    /* 0x0256 */ u16 unk_256;
    /* 0x0258 */ UnkPlayerStruct258 unk_258[40];
    /* 0x0D98 */ s16 unk_D98;
    /* 0x0D9A */ s16 unk_D9A;
    /* 0x0D9C */ f32 unk_D9C;
    /* 0x0DA0 */ f32 unk_DA0;
    /* 0x0DA4 */ s16 unk_DA4;
    /* 0x0DA6 */ s16 unk_DA6;
    /* 0x0DA8 */ f32 unk_DA8;
    /* 0x0DAC */ f32 unk_DAC;
    /* 0x0DB0 */ f32 unk_DB0;
    /* 0x0DB4 */ struct UnkPlayerInner unk_DB4;
    /* 0x0DB6 */ // s16 unk_DB6;
    /* 0x0DB8 */ // f32 unk_DB8;
    /* 0x0DBC */ // f32 unk_DBC;
    /* 0x0DC0 */ // f32 unk_DC0;
    /* 0x0DC4 */ // f32 unk_DC4;
    /* 0x0DC8 */ // f32 unk_DC8;
    /* 0x0DCC */ // s16 unk_DCC;
    /* 0x0DCE */ // s16 unk_DCE;
    /* 0x0DD0 */ // s16 unk_DD0;
    /* 0x0DD2 */ // s16 unk_DD2;
    /* 0x0DD4 */ // s16 unk_DD4;

    u32 nControlFlags; // Is racer human, ai, or networked controlled?
    s32 nCharacter;    // Networked character choice
    s32 nStartingRank;
    u32 nHasAuthority;
} Player; // size = 0xDD8

typedef struct {
    // Something related to time trial ghost data?
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 ghostDataSaved;
    /* 0x05 */ s8 courseIndex;
    /* 0x06 */ u8 characterId;
    /* 0x07 */ u8 unk_07[0x3C];
    /* 0x43 */ u8 pad_43[0x7F - 0x43];
    /* 0x7F */ u8 checksum;
} struct_8018EE10_entry; // size = 0x80

typedef struct {
    uint8_t r, g, b;
} RGB8;

typedef struct {
    /* 0x00 */ u16 red;
    /* 0x02 */ u16 green;
    /* 0x04 */ u16 blue;
    /* 0x06 */ u16 alpha;
} RGBA16; // size = 0x08

// Something related to human players
// Also might be used for the camera during the post race screens
typedef struct {
    /* 0x00 */ f32 unknownScaling; // Looks to be some type of scaling, unknown use
    /* 0x04 */ f32 rankScaling;    // Scaling done on the rank text in the bottom left corner of the screen
                                   // All time measurements are in centiseconds
    /* 0x08 */ u32 someTimer;      // The someTimers seem to always have the same value, the total time since race start
    /* 0x0C */ u32 someTimer1;
    /* 0x10 */ u32 timeLastTouchedFinishLine; // Sum of time of all completed laps
    // Times at which each lap was completed
    union {
        struct {
            /* 0x14 */ u32 lap1CompletionTime;
            /* 0x18 */ u32 lap2CompletionTime;
            /* 0x1C */ u32 lap3CompletionTime;
        };
        u32 lapCompletionTimes[3];
    };
    // Time each lap took to complete
    union {
        struct {
            /* 0x20 */ u32 lap1Duration;
            /* 0x24 */ u32 lap2Duration;
            /* 0x28 */ u32 lap3Duration;
        };
        u32 lapDurations[3];
    };
    // Integer parts of the player's X/Y/X coordinates
    /* 0x2C */ s32 posXInt;
    /* 0x30 */ s32 posYInt;
    /* 0x34 */ s32 posZInt;
    /* 0x38 */ s32 unk_38;
    // Timer for how long the on screen should blink on lap completion
    // Blinking occurs as long as this is a non-zero value.
    // Other on screen effects may override the blinking
    /* 0x3C */ s16 blinkTimer;
    /* 0x3E */ s16 speedometerX;
    /* 0x40 */ s16 speedometerY;
    /* 0x42 */ s16 itemBoxX;
    /* 0x44 */ s16 itemBoxY;
    /* 0x46 */ s16 slideItemBoxX;
    /* 0x48 */ s16 slideItemBoxY;
    // These 2 s16's have values, but I have yet to identify any places that read them
    /* 0x4A */ s16 unk_4A;
    /* 0x4C */ s16 unk_4C;
    /* 0x4E */ s16 timerX; // X coordinate of the on screen timer
    // These 4 X coordinates are "slide" values
    union {
        struct {
            /* 0x50 */ s16 lap1CompletionTimeX; // Pulls double-duty as timerAfterImage1X
            /* 0x52 */ s16 lap2CompletionTimeX; // Pulls double-duty as timerAfterImage2X
            /* 0x54 */ s16 lap3CompletionTimeX;
        };
        /* 0x50 */ s16 lapCompletionTimeXs[3];
    };
    /* 0x56 */ s16 totalTimeX;
    /* 0x58 */ s16 timerY; // Y coordinate of the on screen timer (used as Y coordinate for lap completion times in
                           // post-race screen)
    /* 0x5A */ s16 lapX;   // X coordinate of the on screen lap counter
                           // 0x5C and 0x5E seem to be relative to lapX
    /* 0x5C */ s16 lapAfterImage1X;
    /* 0x5E */ s16 lapAfterImage2X;
    /* 0x60 */ s16 lapY;  // Y coordinate of the on screen lap counter
    /* 0x62 */ s16 rankX; // X coordinate of the on screen rank indicator
    /* 0x64 */ s16 rankY; // Y coordinate of the on screen rank indicator
    /* 0x66 */ s16
        slideRankX; // Adds with the other rankX, used during post race screen to make the rank "slide" into place
    /* 0x68 */ s16
        slideRankY; // Adds with the other rankY, used during post race screen to make the rank "slide" into place
    /* 0x6A */ s16 stagingPosition; // Position to take during race staging
    // These s16's occasionally have values, but I have yet to identify any places that read them
    // They appear to have values when in 3/4 player split screen mode, otherwise they're 0
    /* 0x6C */ s16 unk_6C;
    /* 0x6E */ s16 unk_6E;
    /* 0x70 */ s8 raceCompleteBool; // Indicates if race is over?
    /* 0x71 */ s8 lapCount;         // This increases to 3 when a race is over, while alsoLapCount stays at 2
    /* 0x72 */ s8 alsoLapCount;
    // Related to the timer blinking on lap completion
    // If blinkTimer is counting down:
    //     0: Show timer
    //     1: Hide timer
    //     2: ? Seems to be a transition state, never lasts a long enough for blinkTimer to tick
    /* 0x73 */ s8 blinkState;
    /* 0x74 */ s8 unk_74;
    /* 0x75 */ s8 unk_75;
    /* 0x76 */ u8 itemOverride; // Something related to item generation. If non-zero, it determines the item you get
    /* 0x77 */ s8 unk_77;
    // 0x78 to 0x7F appear to be some type of "state" trackers for the lap and timer text during a race start
    // When a race starts those texts (and their afterimages) slide in and "bounce" a bit. These states control the
    // bouncing (somehow)
    /* 0x78 */ u8 unk_78;
    /* 0x79 */ u8 unk_79;
    /* 0x7A */ u8 unk_7A;
    /* 0x7B */ u8 unk_7B;
    /* 0x7C */ u8 unk_7C;
    /* 0x7D */ u8 unk_7D;
    /* 0x7E */ u8 unk_7E;
    /* 0x7F */ u8 unk_7F;
    /* 0x80 */ u8 unk_80;
    /* 0x81 */ u8 unk_81;
    /* 0x82 */ s8 unk_82;
    /* 0x83 */ s8 unk_83;
} hud_player; // size = 0x84

#define HUD_PLAYERS_SIZE 4

#endif
