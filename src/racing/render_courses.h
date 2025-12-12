#ifndef RENDER_COURSES_H
#define RENDER_COURSES_H

#include "code_800029B0.h"
#include "../camera.h"
#include "src/engine/tracks/Track.h"

void func_8029122C(ScreenContext*, s32);
s32 func_80290C20(Camera*);
void parse_track_displaylists(TrackSections* asset);
void render_track_sections(const char*[], ScreenContext*);
void func_80291198(void);
void render_track(ScreenContext*);
void func_80295BF8(s32);
void func_80295C6C(void);
void func_80295D50(s16, s16);
void func_80295D6C(void);
void func_80295D88(void);
void func_802966A0(void);

extern s32 D_8015F59C;

extern s32 D_802B87C4;
extern s32 D_802B87C8;
extern s32 D_802B87CC;
extern s32 D_802B87BC;

extern s16 gFogMin;
extern s16 gFogMax;
extern s16 D_802B87D0;
extern s16 D_802B87D4;

extern Lights1 D_800DC610[];

extern u16 gNumCollisionTriangles;

#endif
