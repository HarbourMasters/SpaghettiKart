#ifndef CEREMONY_AND_CREDITS_H
#define CEREMONY_AND_CREDITS_H

#include <common_structs.h>
#include "camera.h"

struct struct_80283431 {
    Vec3f unk0;
    Vec3s unkC;
};

struct struct_80283430 {
    s8 unk0;  // index
    s8 unk1;  // speed
    u16 unk2; // point
    s8 unk4;
    s8 unk5;
    Vec3s unk6;
};

struct credits_data_1FA0 {
    Vec3f unk0;
    s8 pad[0x10];
    s8 unk1C;
    s8 unk1D;
    s16 unk1E;
    f32 unk20;
    s8 pad2[0x24];
    s16 unk48[3];
    s16 unk4E[2];
    s16 un52;
    s16 unk54[2];
    s16 unk58;
    s16 unk5A[2];
    s16 unk5E;
    s16 unk60;
    s16 unk62;
    f32 unk64;
    f32 unk68;
    s16 unk6C;
    s16 unk6E;
};

// Cinematic camera? Camera rails?
// CutsceneVariable?
/**
 * Struct used to store cutscene info, like the camera's target position/focus.
 *
 * See the sCutsceneVars[] array in camera.c for more details.
 */
struct CinematicCamera {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f lookAt;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 cutscene;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ Vec3f unk24;
    /* 0x30 */ Vec3f unk30;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3s unk48;
    /* 0x4E */ Vec3s unk4E;
    /* 0x54 */ Vec3s unk54;
    /* 0x5A */ Vec3s unk5A;
    /* 0x60 */ s16 unk60;
    /* 0x62 */ s16 unk62;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
}; // size = 0x70

struct struct_80282C40 {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    u8 unk3;
    s8 unk4;
    s8 unk5;
    Vec3s unk6;
};

struct struct_80285D80 {
    u8 unk0[6];
    Vec3s unk6;
};

struct struct_80286A04 {
    u8 unk0;
    u8 unk1;
    struct struct_80285D80* unk4;
    struct struct_80285D80* unk8;
    u16 unkC;
};

struct CutsceneSplinePoint {
    s8 index;
    u8 speed;
    Vec3s point;
};

struct Cutscene {
    /* 0x0 */ void (*shot)(struct CinematicCamera*);
    /* 0x4 */ s16 duration;
    /* 0x6 */ // s16 pad;
}; /* size = 0x8 */

/* Function Prototypes */

void init_cinematic_camera(void);
s32 func_80283648(Camera*);
void vec3f_set_dupe(Vec3f, f32, f32, f32);
void vec3s_set_dupe(Vec3s, s16, s16, s16);
void vec3f_clear(Vec3f);
void vec3s_clear(Vec3s);
void vec3f_copy_return_dupe(Vec3f, Vec3f);
void vec3s_copy_dupe(Vec3s, Vec3s);
void func_80282040(void);
void func_80282048(void);
void func_80282050(Vec3f, Vec3f, s16);
void func_802820F8(Vec3f, Vec3f, s16);
s32 f32_lerp(f32*, f32, f32);
s32 func_80282200(Vec3s, s16, s16);
s32 set_transition_colour_fade_alpha_ending(f32*, f32, f32);
s32 func_80282364(s16*, s16, s16);
void reset_spline(void);
void func_80282434(struct CinematicCamera*);
void func_80282454(Vec3f, Vec3f, f32*, s16*, s16*);
void func_80282504(Vec3f, Vec3f, f32, s16, s16);
void func_802825C8(Vec3f, Vec3f, Vec3f, Vec3s);
void func_80282700(f32, Vec3f, f32*, f32[], f32[], f32[], f32[]);
s32 move_point_along_spline(Vec3f, f32*, struct struct_80283430[], s16*, f32*);
void func_80282BE4(struct struct_80283430*, s8, u8, s8, Vec3s, s32);
void func_80282C40(struct struct_80283430*, struct struct_80282C40*, s32);
s32 func_80282D90(struct CinematicCamera*, struct struct_80286A04*, struct struct_80286A04*, s32);
void func_80282E58(struct CinematicCamera*, struct struct_80282C40*, s32);
void func_80282EAC(s32, struct CinematicCamera*, s16, s16, s16);
void func_80282F00(s16*, s16);
void func_80282F44(s32, struct CinematicCamera*, Camera*);
void func_802830B4(struct CinematicCamera*, s16, s16, s16);
void func_80283100(struct CinematicCamera*, f32*);
void func_80283240(s16);
s32 cutscene_event(CameraEvent event, struct CinematicCamera*, s16, s16);
s32 func_80283330(s32);
s32 func_8028336C(struct CinematicCamera*, Camera*);
s32 func_80283428(void);
void func_80283968(struct CinematicCamera*);
void func_80283994(struct CinematicCamera*);
void func_802839B4(struct CinematicCamera*);
void func_802839CC(struct CinematicCamera*);
void func_802839E0(struct CinematicCamera*);
void play_sound_welcome(struct CinematicCamera*);
void func_80283A34(struct CinematicCamera*);
void play_sound_congratulation(struct CinematicCamera*);
void play_sound_balloon_pop(struct CinematicCamera*);
void play_sound_fish(struct CinematicCamera*);
void play_sound_fish_2(struct CinematicCamera*);
void play_sound_shoot_trophy(struct CinematicCamera*);
void play_sound_podium(struct CinematicCamera*);
void play_sound_trophy(struct CinematicCamera*);
void func_80283A54(struct CinematicCamera*);
void func_80283A7C(struct CinematicCamera*);
void func_80283B6C(struct CinematicCamera*);
void func_80283BA4(struct CinematicCamera*);
void func_80283BF0(struct CinematicCamera*);
void func_80283C14(struct CinematicCamera*);
void func_80283C38(struct CinematicCamera*);
void func_80283C58(struct CinematicCamera*);
void func_80283C78(struct CinematicCamera*);
void func_80283CA8(struct CinematicCamera*);
void func_80283CD0(struct CinematicCamera*);
void play_sound_farewell(struct CinematicCamera*);
void func_80283D2C(struct CinematicCamera*);
void func_80283EA0(struct CinematicCamera*);
void func_80283ED0(struct CinematicCamera*);
void func_80283EF8(struct CinematicCamera*);
void func_80283F6C(struct CinematicCamera*);
void func_80283FCC(struct CinematicCamera*);
void func_80283FF4(struct CinematicCamera*);
void func_80284068(struct CinematicCamera*);
void func_802840C8(struct CinematicCamera*);
void func_80284154(struct CinematicCamera*);
void func_80284184(struct CinematicCamera*);
void func_802841E8(struct CinematicCamera*);
void func_8028422C(struct CinematicCamera*);
void func_802842A8(struct CinematicCamera*);
void func_802842D8(struct CinematicCamera*);
void func_80284308(struct CinematicCamera*);
void func_80284418(struct CinematicCamera*);
void func_80284494(struct CinematicCamera*);
void func_802844FC(struct CinematicCamera*);
void func_8028454C(struct CinematicCamera*);
void func_802845EC(struct CinematicCamera*);
void func_8028461C(struct CinematicCamera*);
void func_80284648(struct CinematicCamera*);
void func_802846AC(void);
void func_802846B4(struct CinematicCamera*);
void func_802846E4(struct CinematicCamera*);
void func_802847CC(struct CinematicCamera*);
void play_cutscene(struct CinematicCamera*);
void transition_sliding_borders(void);

/* This is where I'd put my static data, if I had any */

extern s32 D_80283FCC;
extern s32 D_80283FF4;
extern f32 D_802856B0;
extern f32 D_802856B4;
extern f32 D_802856B8;
extern f32 D_802856BC;
extern f32 D_802856C0;
extern s32 D_802856C4;
extern s32 D_802856C8[]; // padding?
extern s16 sCutsceneShot;
extern s16 gCutsceneShotTimer;
extern s32 D_802876D4;
extern s32 D_802876D8;
extern s32 D_802876DC; // fake/padding? Or D8 is array?
extern struct CinematicCamera D_802876E0;
extern struct struct_80283431 D_80287750[];
extern struct struct_80283430 D_80287818[];
extern struct struct_80283430 D_80287998[];
extern f32 sCutsceneSplineSegmentProgress;
extern s16 sCutsceneSplineSegment;
extern s16 D_80287B1E;
extern s8 D_80287B20;
extern struct struct_80282C40 D_802856DC[];
extern struct struct_80282C40 D_80285718[];
extern struct struct_80282C40 D_80285754[];
extern struct struct_80282C40 D_80285784[];
extern struct struct_80282C40 D_802857B4[];
extern struct struct_80282C40 D_802857CC[];
extern struct struct_80282C40 D_802857F0[];
extern struct struct_80282C40 D_80285850[];
extern struct struct_80282C40 D_802858B0[];
extern struct struct_80282C40 D_802858C8[];
extern struct struct_80282C40 D_80285910[];
extern struct struct_80282C40 D_80285928[];
extern struct struct_80282C40 D_80285940[];
extern struct struct_80282C40 D_80285A10[];
extern struct struct_80282C40 D_80285A4C[];
extern struct struct_80282C40 D_80285A88[];
extern struct struct_80282C40 D_80285AB8[];
extern struct struct_80282C40 D_80285AE8[];
extern struct struct_80282C40 D_80285B00[];
extern struct struct_80282C40 D_80285B18[];
extern struct struct_80282C40 D_80285B54[];
extern struct struct_80282C40 D_80285B90[];
extern struct struct_80282C40 D_80285BA8[];
extern struct struct_80282C40 D_80285C38[];
extern struct struct_80282C40 D_80285C74[];
extern struct Cutscene D_80285D10[];
extern s16 D_80285D14;
extern s32 D_802876D4;
extern s32 D_802876D8;
// extern struct credits_data_1FA0 *D_802876E0;

#endif // CEREMONY_AND_CREDITS_H
