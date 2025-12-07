#ifndef SKYBOX_AND_SPLITSCREEN_H
#define SKYBOX_AND_SPLITSCREEN_H

#include <libultraship.h>
#include <libultra/gbi.h>
#include "code_800029B0.h"

#define G_CLEAR_ALL_MODES 0xFFFFFFFF

/* Function Prototypes */

void func_802A4A0C(Vtx*, struct UnkStruct_800DC5EC*, s32, s32, f32*);
void set_screen(void);
void set_editor_screen(void);
void func_802A3730(struct UnkStruct_800DC5EC*);
void func_802A38AC(void);
void func_802A38B4(void);
void func_802A39E0(struct UnkStruct_800DC5EC*);
void init_z_buffer(void);
void init_rdp(void);
void func_802A40A4(void);
void func_802A40AC(void);
void func_802A40B4(void);
void func_802A40BC(void);
void func_802A40C4(void);
void func_802A40CC(void);
void func_802A40D4(void);
void func_802A40DC(void);
s32 set_viewport2(void);
void set_viewport(void);
void select_framebuffer(void);
void func_802A4300(void);
void func_802A450C(Vtx*);
void func_802A487C(Vtx*, struct UnkStruct_800DC5EC*, s32, s32, f32*);
void func_802A4D18(void);
void func_802A4EF4(void);
void func_802A5004(void);
void func_802A50EC(void);
void func_802A51D4(void);
void func_802A52BC(void);
void func_802A53A4(void);
void func_802A54A8(void);
void func_802A5590(void);
void func_802A5678(void);
void func_802A5760(void);
void render_player_one_1p_screen(void);
void render_player_one_2p_screen_vertical(void);
void render_player_two_2p_screen_vertical(void);
void render_player_one_2p_screen_horizontal(void);
void render_player_two_2p_screen_horizontal(void);
void render_player_one_3p_4p_screen(void);
void render_player_two_3p_4p_screen(void);
void render_player_three_3p_4p_screen(void);
void render_player_four_3p_4p_screen(void);
void render_screens(s32 mode, s32 cameraId, s32 playerId);
void func_802A74BC(void);
void copy_framebuffer(s32, s32, s32, s32, u16*, u16*);
void func_802A7728(void);
void func_802A7940(void);

extern Vp D_802B8880[];

#endif
