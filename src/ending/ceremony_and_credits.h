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
typedef struct {
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
} CinematicCamera; // size = 0x70

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
    u8 courseId;
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
    /* 0x0 */ void (*shot)(CinematicCamera*);
    /* 0x4 */ s16 duration;
    /* 0x6 */ // s16 pad;
}; /* size = 0x8 */

typedef void (*CameraEvent)(CinematicCamera* c);
typedef CameraEvent CutsceneShot;

/* Function Prototypes */

void init_cinematic_camera(void);
s32 func_80283648(Camera*);
void vec3s_set_dupe(Vec3s, s16, s16, s16);
void vec3s_clear(Vec3s);
void vec3s_copy_dupe(Vec3s, Vec3s);
s32 f32_lerp(f32*, f32, f32);
bool ease_out_transition(s16*, s16, s16);
bool adjust_f32_value_transition(f32*, f32, f32);
bool adjust_s16_value_transition(s16*, s16, s16);
void reset_spline(void);
void reset_spline_wrap(CinematicCamera*);
void calculate_distance_angle_y_and_angle_y_to_xz(Vec3f, Vec3f, f32*, s16*, s16*);
void apply_distance_angle_y_and_angle_y_to_xz(Vec3f, Vec3f, f32, s16, s16);
void aborting_cinematic_function(Vec3f, Vec3f, Vec3f, Vec3s);
void evaluate_cubic_spline(f32, Vec3f, f32*, f32[], f32[], f32[], f32[]);
s32 move_point_along_spline(Vec3f, f32*, struct struct_80283430[], s16*, f32*);
void func_80282BE4(struct struct_80283430*, s8, u8, s8, Vec3s, s32);
void func_80282C40(struct struct_80283430*, struct struct_80282C40*, s32);
s32 move_cinematic_camera_along_spline(CinematicCamera*, struct struct_80286A04*, struct struct_80286A04*, s32);
void func_80282E58(CinematicCamera*, struct struct_80282C40*, s32);
void func_80282EAC(s32, CinematicCamera*, s16, s16, s16);
void func_80282F00(s16*, s16);
void func_80282F44(s32, CinematicCamera*, Camera*);
void func_802830B4(CinematicCamera*, s16, s16, s16);
void func_80283100(CinematicCamera*, f32*);
void func_80283240(s16);
s32 cutscene_event(CameraEvent event, CinematicCamera*, s16, s16);
s32 func_80283330(s32);
s32 func_8028336C(CinematicCamera*, Camera*);
s32 cinematic_stub(void);
void wrap_func_8028100C(CinematicCamera*);
void wrap_func_80280FFC(CinematicCamera*);
void animation_apears_sliding_borders(CinematicCamera*);
void animation_disapears_sliding_borders(CinematicCamera*);
void wrap_func_80092C80(CinematicCamera*);
void play_sound_welcome(CinematicCamera*);
void wrap_func_800CA0CC(CinematicCamera*);
void play_sound_congratulation(CinematicCamera*);
void play_sound_balloon_pop(CinematicCamera*);
void play_sound_fish(CinematicCamera*);
void play_sound_fish_2(CinematicCamera*);
void play_sound_shoot_trophy(CinematicCamera*);
void play_sound_podium(CinematicCamera*);
void play_sound_trophy(CinematicCamera*);
void func_80283A54(CinematicCamera*);
void func_80283A7C(CinematicCamera*);
void func_80283B6C(CinematicCamera*);
void func_80283BA4(CinematicCamera*);
void func_80283BF0(CinematicCamera*);
void func_80283C14(CinematicCamera*);
void wrap_func_800CB134(CinematicCamera*);
void wrap_func_800CB14C(CinematicCamera*);
void func_80283C78(CinematicCamera*);
void func_80283CA8(CinematicCamera*);
void func_80283CD0(CinematicCamera*);
void play_sound_farewell(CinematicCamera*);
void func_80283D2C(CinematicCamera*);
void func_80283EA0(CinematicCamera*);
void copy_player_two_in_camera(CinematicCamera*);
void lerp_player_two_in_camera(CinematicCamera*);
void func_80283F6C(CinematicCamera*);
void copy_player_three_in_camera(CinematicCamera*);
void lerp_player_three_in_camera(CinematicCamera*);
void func_80284068(CinematicCamera*);
void func_802840C8(CinematicCamera*);
void func_80284154(CinematicCamera*);
void func_80284184(CinematicCamera*);
void func_802841E8(CinematicCamera*);
void func_8028422C(CinematicCamera*);
void func_802842A8(CinematicCamera*);
void func_802842D8(CinematicCamera*);
void func_80284308(CinematicCamera*);
void func_80284418(CinematicCamera*);
void func_80284494(CinematicCamera*);
void func_802844FC(CinematicCamera*);
void func_8028454C(CinematicCamera*);
void func_802845EC(CinematicCamera*);
void func_8028461C(CinematicCamera*);
void func_80284648(CinematicCamera*);
void func_802846AC(void);
void func_802846B4(CinematicCamera*);
void func_802846E4(CinematicCamera*);
void func_802847CC(CinematicCamera*);
void play_cutscene(CinematicCamera*);
void ceremony_transition_sliding_borders(void);

/* This is where I'd put my static data, if I had any */

extern s32 D_80283FCC;
extern s32 D_80283FF4;
extern f32 D_802856B0;
extern f32 D_802856B4;
extern f32 gOrderedSizeSlidingBorders;
extern f32 D_802856BC;
extern f32 gSizeSlidingBorders;
extern s32 D_802856C4;
extern s32 D_802856C8[]; // padding?
extern s16 sCutsceneShot;
extern s16 gCutsceneShotTimer;
extern s32 D_802876D4;
extern s32 D_802876D8;
extern s32 D_802876DC; // fake/padding? Or D8 is array?
extern CinematicCamera D_802876E0;
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
extern struct Cutscene gCutScene[];
extern s16 D_80285D14;
extern s32 D_802876D4;
extern s32 D_802876D8;
// extern struct credits_data_1FA0 *D_802876E0;

#endif // CEREMONY_AND_CREDITS_H
