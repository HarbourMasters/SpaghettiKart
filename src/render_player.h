#ifndef RENDER_PLAYER_H
#define RENDER_PLAYER_H

#include <common_structs.h>
#include "buffers.h"
#include "camera.h"

#define SOME_TEXTURE_POINTER_MATH 0x800

/* Function Prototypes */

void func_8001F980(s32*, s32*);
void func_8001F9E4(Player*, Camera*, s8);
u16 check_player_camera_collision(Player*, Camera*, f32, f32);
u16 func_8001FD78(Player*, f32, f32, f32);
void init_render_player(Player*, Camera*, s8, s8);
void load_kart_texture_and_render_kart_particles(s32 screenIdx);
void try_rendering_player(Player*, s8, s8);
void render_players(Camera* camera, s32 screenIdx);
void func_80021B0C(void);
void func_80021C78(void);
void func_80021D40(void);
void func_80021DA8(void);
void mtxf_translate_rotate(Mat4, Vec3f, Vec3s);
void func_80021F50(Mat4, Vec3f);
void failed_fixed_point_matrix_conversion(Mtx*, Mat4);
void convert_to_fixed_point_matrix(Mtx*, Mat4);
bool adjust_angle(s16*, s16, s16);
void move_s32_towards(s32*, s32, f32);
void move_f32_towards(f32*, f32, f32);
void move_s16_towards(s16*, s16, f32);
void move_u16_towards(u16*, s16, f32);
void func_80022744(void);
void func_8002276C(void);
void func_80022A98(Player*, s8);
void func_80022B50(Player*, s8);
void func_80022BC4(Player*, s8);
void func_80022CA8(Player*, s8, s8, s8);
void func_80022D60(Player*, s8, s8, s8);
void func_80022DB4(Player*, s8);
void func_80022E84(Player*, s8, s8, s8);
void change_player_color_effect_rgb(Player*, s8, s32, f32);
void change_player_color_effect_cmy(Player*, s8, s32, f32);
bool is_player_under_light_luigi_raceway(Player*, s8);
void render_light_environment_on_player(Player*, s8);
void func_800235AC(Player*, s8);
void func_80023BF0(Player*, s8, s8, s8);
void render_player_shadow(Player*, s8, s8);
void render_player_shadow_credits(Player*, s8, s8);
void render_kart(Player*, s8, s8, s8);
void render_ghost(Player*, s8, s8, s8);
void func_80025DE8(Player*, s8, s8, s8);
void render_player_ice_reflection(Player*, s8, s8, s8);
void render_player(Player*, s8, s8);
void func_80026A48(Player*, s8);
void update_wheel_palette(Player*, s8, s8, s8);
void update_player_kart_hop(Player*);
void func_8002701C(void);
void func_80027024(s32, s32, s32);

/* This is where I'd put my static data, if I had any */

extern struct_D_802F1F80* gPlayerPalette;

extern s32 D_80164A28;
extern s16 gPlayersToRenderPlayerId[];
extern s16 D_80164ABE[];
extern s16 gPlayersToRenderScreenId[];
extern Player* gPlayersToRenderPlayer[];
extern s16 gMatrixEffectCount;
extern u16 gPlayerRedEffect[];
extern u16 gPlayerGreenEffect[];
extern u16 gPlayerBlueEffect[];
extern u16 gPlayerCyanEffect[];
extern u16 gPlayerMagentaEffect[];
extern u16 gPlayerYellowEffect[];
extern s32 D_80164B80[];

extern s32 gPlayerStarEffectStartTime[];

extern s32 gPlayersToRenderCount;
extern void* D_800DDB5C[];
extern Vtx* gPlayerVtx[];
extern f32 gCharacterSize[];
extern s32 D_800DDE74[];
extern Vtx D_800E51D0[];
extern Vtx D_800E5210[];
extern f64 D_800ED680;
extern f64 D_800ED688;
extern f32 D_800ED6A8;

// There are actually found in data_0DD0A0_2_0.s
extern Vtx gPlayerOneVtx[];
extern Vtx gPlayerTwoVtx[];
extern Vtx gPlayerThreeVtx[];
extern Vtx gPlayerFourVtx[];
extern Vtx gPlayerFiveVtx[];
extern Vtx gPlayerSixVtx[];
extern Vtx gPlayerSevenVtx[];
extern Vtx gPlayerEightVtx[];

extern u32 gKartMarioWheels0[];
extern u32 gKartMarioWheels1[];
extern u32 gKartLuigiWheels0[];
extern u32 gKartLuigiWheels1[];
extern u32 gKartBowserWheels0[];
extern u32 gKartBowserWheels1[];
extern u32 gKartToadWheels0[];
extern u32 gKartToadWheels1[];
extern u32 gKartYoshiWheels0[];
extern u32 gKartYoshiWheels1[];
extern u32 gKartDKWheels0[];
extern u32 gKartDKWheels1[];
extern u32 gKartPeachWheels0[];
extern u32 gKartPeachWheels1[];
extern u32 gKartWarioWheels0[];
extern u32 gKartWarioWheels1[];

// These all come the kart data stuff, they should end up in their own inc.c eventually
#ifdef TARGET_N64
extern u8* gKartMario168Wheel0[];
extern u8* gKartMario147Wheel0[];
extern u8* gKartMario126Wheel0[];
extern u8* gKartMario105Wheel0[];
extern u8* gKartMario084Wheel0[];
extern u8* gKartMario063Wheel0[];
extern u8* gKartMario042Wheel0[];
extern u8* gKartMario021Wheel0[];
extern u8* gKartMario000Wheel0[];
extern u8* gKartMario269Wheel0[];
extern u8* gKartMario269Wheel0[];
extern u8* gKartMario249Wheel0[];
extern u8* gKartMario229Wheel0[];
extern u8* gKartMario229Wheel0[];
extern u8* gKartMario229Wheel0[];
extern u8* gKartMario209Wheel0[];
extern u8* gKartMario189Wheel0[];
extern u8* gKartMario189Wheel0[];
extern u8* gKartLuigi168Wheel0[];
extern u8* gKartLuigi147Wheel0[];
extern u8* gKartLuigi126Wheel0[];
extern u8* gKartLuigi105Wheel0[];
extern u8* gKartLuigi084Wheel0[];
extern u8* gKartLuigi063Wheel0[];
extern u8* gKartLuigi042Wheel0[];
extern u8* gKartLuigi021Wheel0[];
extern u8* gKartLuigi000Wheel0[];
extern u8* gKartLuigi269Wheel0[];
extern u8* gKartLuigi269Wheel0[];
extern u8* gKartLuigi249Wheel0[];
extern u8* gKartLuigi229Wheel0[];
extern u8* gKartLuigi229Wheel0[];
extern u8* gKartLuigi229Wheel0[];
extern u8* gKartLuigi209Wheel0[];
extern u8* gKartLuigi189Wheel0[];
extern u8* gKartLuigi189Wheel0[];
extern u8* gKartBowser168Wheel0[];
extern u8* gKartBowser147Wheel0[];
extern u8* gKartBowser126Wheel0[];
extern u8* gKartBowser105Wheel0[];
extern u8* gKartBowser084Wheel0[];
extern u8* gKartBowser063Wheel0[];
extern u8* gKartBowser042Wheel0[];
extern u8* gKartBowser021Wheel0[];
extern u8* gKartBowser000Wheel0[];
extern u8* gKartBowser269Wheel0[];
extern u8* gKartBowser269Wheel0[];
extern u8* gKartBowser249Wheel0[];
extern u8* gKartBowser229Wheel0[];
extern u8* gKartBowser229Wheel0[];
extern u8* gKartBowser229Wheel0[];
extern u8* gKartBowser209Wheel0[];
extern u8* gKartBowser189Wheel0[];
extern u8* gKartBowser189Wheel0[];
extern u8* gKartToad168Wheel0[];
extern u8* gKartToad147Wheel0[];
extern u8* gKartToad126Wheel0[];
extern u8* gKartToad105Wheel0[];
extern u8* gKartToad084Wheel0[];
extern u8* gKartToad063Wheel0[];
extern u8* gKartToad042Wheel0[];
extern u8* gKartToad021Wheel0[];
extern u8* gKartToad000Wheel0[];
extern u8* gKartToad269Wheel0[];
extern u8* gKartToad269Wheel0[];
extern u8* gKartToad249Wheel0[];
extern u8* gKartToad229Wheel0[];
extern u8* gKartToad229Wheel0[];
extern u8* gKartToad229Wheel0[];
extern u8* gKartToad209Wheel0[];
extern u8* gKartToad189Wheel0[];
extern u8* gKartToad189Wheel0[];
extern u8* gKartYoshi168Wheel0[];
extern u8* gKartYoshi147Wheel0[];
extern u8* gKartYoshi126Wheel0[];
extern u8* gKartYoshi105Wheel0[];
extern u8* gKartYoshi084Wheel0[];
extern u8* gKartYoshi063Wheel0[];
extern u8* gKartYoshi042Wheel0[];
extern u8* gKartYoshi021Wheel0[];
extern u8* gKartYoshi000Wheel0[];
extern u8* gKartYoshi269Wheel0[];
extern u8* gKartYoshi269Wheel0[];
extern u8* gKartYoshi249Wheel0[];
extern u8* gKartYoshi229Wheel0[];
extern u8* gKartYoshi229Wheel0[];
extern u8* gKartYoshi229Wheel0[];
extern u8* gKartYoshi209Wheel0[];
extern u8* gKartYoshi189Wheel0[];
extern u8* gKartYoshi189Wheel0[];
extern u8* gKartDK168Wheel0[];
extern u8* gKartDK147Wheel0[];
extern u8* gKartDK126Wheel0[];
extern u8* gKartDK105Wheel0[];
extern u8* gKartDK084Wheel0[];
extern u8* gKartDK063Wheel0[];
extern u8* gKartDK042Wheel0[];
extern u8* gKartDK021Wheel0[];
extern u8* gKartDK000Wheel0[];
extern u8* gKartDK269Wheel0[];
extern u8* gKartDK269Wheel0[];
extern u8* gKartDK249Wheel0[];
extern u8* gKartDK229Wheel0[];
extern u8* gKartDK229Wheel0[];
extern u8* gKartDK229Wheel0[];
extern u8* gKartDK209Wheel0[];
extern u8* gKartDK189Wheel0[];
extern u8* gKartDK189Wheel0[];
extern u8* gKartPeach168Wheel0[];
extern u8* gKartPeach147Wheel0[];
extern u8* gKartPeach126Wheel0[];
extern u8* gKartPeach105Wheel0[];
extern u8* gKartPeach084Wheel0[];
extern u8* gKartPeach063Wheel0[];
extern u8* gKartPeach042Wheel0[];
extern u8* gKartPeach021Wheel0[];
extern u8* gKartPeach000Wheel0[];
extern u8* gKartPeach269Wheel0[];
extern u8* gKartPeach269Wheel0[];
extern u8* gKartPeach249Wheel0[];
extern u8* gKartPeach229Wheel0[];
extern u8* gKartPeach229Wheel0[];
extern u8* gKartPeach229Wheel0[];
extern u8* gKartPeach209Wheel0[];
extern u8* gKartPeach189Wheel0[];
extern u8* gKartPeach189Wheel0[];
extern u8* gKartWario168Wheel0[];
extern u8* gKartWario147Wheel0[];
extern u8* gKartWario126Wheel0[];
extern u8* gKartWario105Wheel0[];
extern u8* gKartWario084Wheel0[];
extern u8* gKartWario063Wheel0[];
extern u8* gKartWario042Wheel0[];
extern u8* gKartWario021Wheel0[];
extern u8* gKartWario000Wheel0[];
extern u8* gKartWario269Wheel0[];
extern u8* gKartWario269Wheel0[];
extern u8* gKartWario249Wheel0[];
extern u8* gKartWario229Wheel0[];
extern u8* gKartWario229Wheel0[];
extern u8* gKartWario229Wheel0[];
extern u8* gKartWario209Wheel0[];
extern u8* gKartWario189Wheel0[];
extern u8* gKartWario189Wheel0[];
#endif

extern s16 D_80165020[40];
// Used to calculate difference between previous and current player velocity.
extern Vec3f gPlayerLastVelocity[8];
extern s16 gLastAnimFrameSelector[4][8];
extern s16 gLastAnimGroupSelector[4][8];
extern s16 D_80165150[4][8];
extern s16 D_80165190[4][8];
extern s16 D_801651D0[4][8];

#endif // RENDER_PLAYER_H
