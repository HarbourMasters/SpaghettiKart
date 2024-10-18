#ifndef BOMB_KART_H
#define BOMB_KART_H

#include <common_structs.h>
#include <mk64.h>

#define NUM_BOMB_KARTS_MAX 7
#define NUM_BOMB_KARTS_VERSUS 7
#define NUM_BOMB_KARTS_BATTLE 4

#define BOMB_STATE_INACTIVE 0
#define BOMB_STATE_CCW 1
#define BOMB_STATE_CW 2
#define BOMB_STATE_STATIONARY 3 // Paused?
#define BOMB_STATE_EXPLODED 4   // Or maybe exploding?
#define BOMB_STATE_UNKNOWN 5

typedef struct {
    /* 0x00 */ u16 waypointIndex;
    /* 0x02 */ u16 startingState;
    /* 0x04 */ f32 unk_04; // Used to set the value of unk_3C in BombKart
    /* 0x08 */ f32 startingXPos;
    /* 0x0C */ f32 startingZPos;
    /**
     * These 2 never seem to get referenced. They also only have non-zero
     * data for the Yoshi Valley set of spawns
     **/
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14;
} BombKartSpawn; // size = 0x18

typedef struct {
    /**
     * This struct is in part representative of the
     * bomb kart hazards in VS mode.
     * There may be objects associated with them too
     **/
    /* 0x00 */ Vec3f bombPos;
    /* 0x0C */ Vec3f wheel1Pos;
    /* 0x18 */ Vec3f wheel2Pos;
    /* 0x24 */ Vec3f wheel3Pos;
    /* 0x30 */ Vec3f wheel4Pos;
    /* 0x3C */ f32 unk_3C;
    /* 0x40 */ u16 someRot;       // seems like an angle of some variety, can't figure out its purpose
    /* 0x42 */ u16 waypointIndex; // probably the waypoint that the bomb circles
    /* 0x44 */ u16 state;
    /* 0x46 */ u16 bounceTimer; // timer? state? height?
    /* 0x48 */ u16 circleTimer; // timer? state? Somehow controls how it circles around
    /* 0x4A */ u16 unk_4A;
    /* 0x4C */ s16 unk_4C;
    /* 0x4E */           // s16 compilerPadding;
    /* 0x50 */ f32 yPos; // Y position of the center of the circle
} BombKart;              // size = 0x54

//! Indexes for the objects associated with the Bomb Karts
extern s32 gIndexObjectBombKart[NUM_BOMB_KARTS_MAX];

extern BombKart gBombKarts[NUM_BOMB_KARTS_MAX];
extern Collision gBombKartCollision[NUM_BOMB_KARTS_MAX];

#endif
