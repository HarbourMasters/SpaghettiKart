#ifndef EFFECTS_H
#define EFFECTS_H

#include <common_structs.h>

/* Function Prototypes */

void func_unnamed(void);
s32 func_8008C1D8(s32*);
void func_8008C1E0(s32*, s32);
void func_unnamed1(s32);
void func_unnamed2(s32);
void func_unnamed3(void);
void func_8008C204(void);
void func_unnamed4(void);
void func_8008C214(void);
void func_unnamed5(void);
void func_unnamed6(void);
void func_unnamed7(void);
void func_unnamed8(void);
void func_8008C23C(void);
void func_unnamed9(void);
void func_unnamed10(void);
void func_unnamed11(void);
void func_unnamed12(void);
void func_unnamed13(void);
void func_unnamed14(void);
void func_unnamed15(void);
void func_unnamed16(void);
void func_unnamed17(void);
void func_unnamed18(void);
void func_unnamed19(void);
void func_unnamed20(void);
void func_unnamed21(void);
void func_unnamed22(s32, s32);
void func_unnamed23(void);
void func_unnamed24(void);
void func_unnamed25(void);
void func_unnamed26(void);
void func_unnamed27(void);
void func_unnamed28(void);
void func_unnamed29(void);
void func_unnamed30(void);
void func_unnamed31(void);
void func_unnamed32(void);
void func_unnamed33(void);
void func_8008C310(Player*);
void func_unnamed34(void);
void clean_effect(Player*, s8);
void func_8008C528(Player*, s8);
void func_8008C62C(Player*, s8);
void func_8008C6D0(Player*, s8);
void func_8008C73C(Player*, s8);
void func_8008C8C4(Player*, s8);
void func_8008C9EC(Player*, s8);
void func_8008CDC0(Player*, s8);
void func_8008CEB0(Player*, s8);
void func_8008D0E4(Player*, s8);
void func_8008D0FC(Player*, s8);
void func_8008D170(Player*, s8);
void func_8008D3B0(Player*, s8);
void apply_boost_sound_effect(Player*, s8);
void apply_boost_effect(Player*);
void remove_boost_effect(Player*);
void func_8008D570(Player*, s8);
void func_8008D698(Player*, s8);
void func_8008D760(Player*);
void func_8008D7B0(Player*, s8);
void func_8008D8B4(Player*, s8);
void func_8008D97C(Player*);
void func_8008D9C0(Player*);
void apply_hit_sound_effect(Player*, s8);
void apply_hit_effect(Player*, s8);
void apply_hit_rotating_sound_effect(Player*, s8);
void apply_lightning_effect(Player*, s8);
void remove_lightning_effect(Player*, s8);
void func_8008E4A4(Player*, s8);
void apply_reverse_sound_effect(Player*, s8);
void func_8008E884(Player*, s8);
void apply_hit_by_item_effect(Player*, s8);
void apply_hit_by_item_sound_effect(Player*, s8);
void remove_hit_by_item_effect(Player*, s8);
void apply_boost_ramp_asphalt_sound_effect(Player*, s8);
void apply_boost_ramp_asphalt_effect(Player*);
void remove_boost_ramp_asphalt_effect(Player*);
void apply_boost_ramp_wood_sound_effect(Player*, s8);
void apply_boost_ramp_wood_effect(Player*);
void remove_boost_ramp_wood_effect(Player*);
void func_8008F104(Player*, s8);
void func_8008F1B8(Player*, s8);
void func_8008F3E0(Player*);
void func_8008F3F4(Player*, s8);
void func_8008F494(Player*, s8);
void func_8008F5A4(Player*, s8);
void apply_star_effect(Player*, s8);
void apply_star_sound_effect(Player*, s8);
void func_8008F86C(Player*, s8);
void apply_boo_effect(Player*, s8);
void apply_boo_sound_effect(Player*, s8);
void func_8008FB30(Player*, s8);
void func_8008FC1C(Player*);
void func_8008FC64(Player*, s8);
void func_8008FCDC(Player*, s8);
void func_8008FD4C(Player*, s8);
void func_8008FDA8(Player*, s8);
void func_8008FDF4(Player*, s8);
void func_8008FE84(Player*, s8);
void func_8008FEDC(Player*, s8);
void func_8008FF08(Player*, s8);
void func_80090178(Player*, s8, Vec3f, Vec3f);
void func_80090778(Player*);
void func_80090868(Player*);
void func_80090970(Player*, s8, s8);
bool prevent_item_use(Player*);
void func_800911B4(Player*, s8);
void func_80091298(Player*, s8);

// audio/external.c
void func_800C9250(u8);
void func_800CA59C(u8);
void func_800CA730(u8);
void func_800CA984(u8);
void func_800CAFC0(u8);
void func_800CB064(u8);

extern s32 D_8018D900[];
extern s16 D_8018D920[];
extern s32 D_8018D930[];
extern s32 D_8018D950[];
extern s32 D_8018D970[];
extern s32 D_8018D990[];

/* This is where I'd put my static data, if I had any */

extern f32 D_800E3710[];
extern f32 D_800E3730[];
extern f32 D_800E3750[];
extern f32 D_800E3770[];
extern f32 D_800E37B0[];
extern f32 D_800E3790[];
extern f32 D_800E37D0[];
extern f32 D_800E37F0[];
extern s16 D_80165190[4][8];
extern f32 D_80165280[];
extern s32 gFrameSinceLastACombo[];

#endif
