#ifndef RENDER_COURSES_H
#define RENDER_COURSES_H

#include "code_800029B0.h"
#include "../camera.h"

typedef struct {
    u32 addr;
    u8 surfaceType;
    u8 sectionId;
    u16 flags;
} TrackSectionsI;

void func_8029122C(struct UnkStruct_800DC5EC*, s32);
s32 func_80290C20(Camera*);
void parse_course_displaylists(const char*);
void render_course_segments(const char*[], struct UnkStruct_800DC5EC*);
void func_80291198(void);
void func_802911C4(void);
void render_mario_raceway(struct UnkStruct_800DC5EC*);
void render_choco_mountain(struct UnkStruct_800DC5EC*);
void render_bowsers_castle(struct UnkStruct_800DC5EC*);
void render_banshee_boardwalk(struct UnkStruct_800DC5EC*);
void render_yoshi_valley(struct UnkStruct_800DC5EC*);
void render_frappe_snowland(struct UnkStruct_800DC5EC*);
void render_koopa_troopa_beach(struct UnkStruct_800DC5EC*);
void render_royal_raceway(struct UnkStruct_800DC5EC*);
void render_luigi_raceway(struct UnkStruct_800DC5EC*);
void render_toads_turnpike(struct UnkStruct_800DC5EC*);
void render_kalimari_desert(struct UnkStruct_800DC5EC*);
void render_sherbet_land(struct UnkStruct_800DC5EC*);
void render_rainbow_road(struct UnkStruct_800DC5EC*);
void render_wario_stadium(struct UnkStruct_800DC5EC*);
void render_block_fort(struct UnkStruct_800DC5EC*);
void render_skyscraper(struct UnkStruct_800DC5EC*);
void render_double_deck(struct UnkStruct_800DC5EC*);
void render_dks_jungle_parkway(struct UnkStruct_800DC5EC*);
void render_big_donut(struct UnkStruct_800DC5EC*);
void func_8029569C(void);
void render_course(struct UnkStruct_800DC5EC*);
void func_80295BF8(s32);
void func_80295C6C(void);
void func_80295D50(s16, s16);
void func_80295D6C(void);
void func_80295D88(void);
void func_802966A0(void);
void func_802969F8(void);

extern s32 D_8015F59C;

extern s32 D_802B87C4;
extern s32 D_802B87C8;
extern s32 D_802B87CC;
extern s32 D_802B87BC;

extern Lights1 D_800DC610[];

extern Lights1 D_800DC610[]; // Light data?

extern u16 gNumCollisionTriangles;

#endif
