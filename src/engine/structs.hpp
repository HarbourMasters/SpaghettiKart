#pragma once

#include <libultraship.h>

typedef float Vec3f[3];
typedef unsigned short Vec3su[3];

typedef struct {
    /* 0x00 */ u16 unk30;
    /* 0x02 */ u16 unk32;
    /* 0x04 */ u16 unk34;
    /* 0x06 */ u16 meshIndexYX;
    /* 0x08 */ u16 meshIndexZY;
    // This may be an index to the tilemap?
    /* 0x0A */ u16 meshIndexZX;
    /* 0x0C */ float surfaceDistance[3]; // Appears to be distance from actor to surface for zx, yx, and zy planes.
    /* 0x18 */ float unk48[3];
    /* 0x24 */ float unk54[3];
    /* 0x30 */ float orientationVector[3];
    /* 0x3C */ f32 unk6C;
} Collision;

struct Actor {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ s16 rot[3];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct BananaActor {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 state;
    /* 0x08 */ s16 parentIndex;
    /* 0x0A */ s16 bananaId; // ? Appears to indiciate which banana of the bunch this one is
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ s16 playerId;     // Id of the player that owns this banana
    /* 0x12 */ s16 elderIndex;   // Index in gActorList of the next-oldest banana in the bunch
    /* 0x14 */ s16 youngerIndex; // Index in gActorList of the next-youngest banana in the bunch
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ float pos[3];
    /* 0x24 */ float velocity[3];
    /* 0x30 */ Collision unk30;
}; // size = 0x70

typedef struct {
    f32 unk_0;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
} UnkCameraInner;

typedef struct {
    /* 0x00 */ float pos[3];
    /* 0x0C */ Vec3f lookAt;
    // This is expected to be a normalized vector, indicates what direction is "up" for the camera
    /* 0x18 */ Vec3f up;
    // I think these are the "nautical angles" between pos and lookAt
    // rot[0] = roll? Does nothing?, rot[1] = yaw, rot[2] = pitch
    /* 0x24 */ s16 rot[3];
    /* 0x2A */ u16 someBitFlags;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ Vec3f unk_30;
    /* 0x3C */ Vec3f unk_3C;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ Collision collision;
    // When you hit a wall (or another driver) the camera's pos and lookAt bounce up and down. This is the velocity(?)
    // of that bouncing
    /* 0x94 */ UnkCameraInner unk_94;
    // Timer for wall-hit bounce. Counts up instead of down
    /* 0xA0 */ f32 unk_A0;
    /* 0xA4 */ s32 unk_A4;
    /* 0xA8 */ s32 unk_A8;
    /* 0xAC */ s16 unk_AC;
    // Id of the player the camera is following.
    /* 0xAE */ s16 playerId;
    // Seems related to camera movement during drifting
    /* 0xB0 */ s16 unk_B0;
    /* 0xB2 */ s16 unk_B2;
    /* 0xB4 */ f32 unk_B4;
} Camera; /* size = 0xB8 */

typedef struct {
    /* 0x0 */ Vec3s pos;
    // Don't really know what to call this member.
    // It somehow controls the speed of travel along a segment of the spline but I don't really get how it works
    /* 0x6 */ s16 velocity;
} SplineControlPoint; // size = 0x8

// WARNING!!!
// You really, really shouldn't use this type for actual spline data. This is intended as a generic SplineData
// type. I would use a union, but that would cause significant ugliness elsewhere in the codebase
typedef struct {
    // This name is a little misleading
    // The control point arrays have more control points in them than this number indicates. Not sure why though.
    s16 numControlPoints;
    // This has to be an array for this to work, so just make it size 1
    SplineControlPoint controlPoints[];
} SplineData;

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
    /* 0x50 */ s32 unk_050;
    /* 0x54 */ s32 status;
    /* 0x58 */ s32 unk_058;
    /* 0x5C */ s32 unk_05C;
    /* 0x60 */ const char* activeTLUT;
    /* 0x64 */ const char* activeTexture;
    /**
     * "list" is something of a misnomer for the names here
     * they can be pointers to just 1 tlut/texture, but it is common for one or the other
     * to be a pointer to an array of tluts/textures.
     **/
    /* 0x68 */ u8* tlutList; // I feel like this should actually be `u8 (*tlutList)[512]`, but that causes mismatches
    /* 0x6C */ const char** textureList;
    /* 0x70 */ Gfx* model;
    /* 0x74 */ Vtx* vertex;
    /* 0x78 */ s8 unk_078[0x04];
    /* 0x7C */ SplineControlPoint* controlPoints;
    /* 0x80 */ SplineData* spline;
    /* 0x84 */ s16 unk_084[0xA];
    /* 0x98 */ u16 timer;
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
    /* 0xCB */ s8 unk_0CB;
    /* 0xCC */ s8 unk_0CC;
    /* 0xCD */ s8 unk_0CD;
    /* 0xCE */ s8 unk_0CE;
    /* 0xCF */ s8 unk_0CF;
    /* 0xD0 */ s8 unk_0D0;
    /* 0xD1 */ s8 unk_0D1;
    /* 0xD2 */ s8 itemDisplay;
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