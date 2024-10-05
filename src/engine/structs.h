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
