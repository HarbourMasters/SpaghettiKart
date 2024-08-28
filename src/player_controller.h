#ifndef player_controller_H
#define player_controller_H

#include <common_structs.h>
#include "camera.h"

/* Function Prototypes */

void func_8002E594(Player*, Camera*, s8, s8);

s32 get_player_index_for_player(Player*);
void func_80027DA8(Player*, s8);
void func_80027EDC(Player*, s8);

void func_80029B4C(Player*, f32, f32, f32);
void func_8002F730(Player* player, Camera* camera, s8 arg2, s8 playerId);

void func_80028864(Player*, Camera*, s8, s8);
void func_80028C44(Player*, Camera*, s8, s8);
void func_80028D3C(Player*, Camera*, s8, s8);
void func_80028E70(Player*, Camera*, s8, s8);
void func_80028F5C(s32, s32, s32, s32);
void func_80028F70(void);

void func_80029060(void);
void func_80029150(void);
void func_80029158(void);
void func_800291E8(void);
void func_800291F0(void);
void func_800291F8(void);
void func_80029200(Player*, s8);
void func_8002934C(Player*, Camera*, s8, s8);

void func_8002A194(Player*, f32, f32, f32);
void func_8002A5F4(Vec3f, f32, Vec3f, f32, f32);
void func_8002A704(Player*, s8);
void func_8002A79C(Player*, s8);
void func_8002A8A4(Player*, s8);
void kart_hop(Player*);
void func_8002AAC0(Player*);
void func_8002AB70(Player*);
void func_8002AE20(void);
void func_8002AE28(void);
void func_8002AE30(void);
void func_8002AE38(Player*, s8, f32, f32, f32, f32);

void func_8002B218(Player*);
void apply_sound_effect(Player*, s8, s8);
void func_8002B5C0(Player*, s8, s8);
void func_8002B830(Player*, s8, s8);
void func_8002B8A4(Player*, Player*);
void func_8002B9CC(Player*, s8, s32);
void func_8002BD58(Player*);
void func_8002BF4C(Player*, s8);

void func_8002C11C(Player*);
void func_8002C17C(Player*, s8);
void func_8002C4F8(Player*, s8);
void func_8002C7E4(Player*, s8, s8);
void func_8002C954(Player*, s8, Vec3f);
void apply_effect(Player*, s8, s8);

void func_8002D028(Player*, s8);
void func_8002D268(Player*, Camera*, s8, s8);

void func_8002E4C4(Player*);
void control_kart_ai_movement(Player*, Camera*, s8, s8);

void func_8002FCA8(Player*, s8);
void func_8002FE84(Player*, f32);

f32 func_80030150(Player*, s8);
void func_80030A34(Player*);
void detect_triple_a_combo_a_released(Player*);
void detect_triple_a_combo_a_pressed(Player*);
void player_speed(Player*);
void decelerate_ai_player(Player*, f32);
void detect_triple_b_combo_b_released(Player*);
void detect_triple_b_combo_b_pressed(Player*);
void func_800323E4(Player*);
void func_80032700(Player*);
void func_80032CB0(Player*, f32);
void func_80032D94(Player*);
void func_80033280(Player*, f32);
void func_800332E8(Player*, s32);
void func_800337CC(Player*, f32, s32);
void func_80033850(Player*, f32);
void func_80033884(Player*, s32*, s32*, s32, s32, s32, s32);
void func_80033940(Player*, s32*, s32, s32, f32);
void func_800339C4(Player*, s32*, s32, s32, f32);
void func_80033A40(Player*, s32*, s32*, s32, s32, s32, f32);
void func_80033AE0(Player*, struct Controller*, s8);

void func_8003680C(Player*, s16);
void func_80036C5C(Player*);
void func_80036CB4(Player*);
void func_80036DB4(Player*, Vec3f, Vec3f);

void func_800371F4(Player*, Vec3f, Vec3f);
void func_80037614(Player*, Vec3f, Vec3f);
void func_8003777C(Player*, Vec3f, Vec3f);
void func_800378E8(Player*, Vec3f, Vec3f);
void func_80037A4C(Player*, Vec3f, Vec3f);
void func_80037BB4(Player* player, Vec3f);
void func_80037CFC(Player*, struct Controller*, s8);

void func_800381AC(Player*, struct Controller*, s8);
void func_800382DC(void);
s16 func_80038534(struct Controller*);
s16 func_800388B0(struct Controller*);
void func_80038BE4(Player*, s16);
void func_80038C6C(Player*, Camera*, s8, s8);

/* This is where I'd put my static data, if I had any */

extern s16 gKartAIforMario[];
extern s16 gKartAIforLuigi[];
extern s16 gKartAIforYoshi[];
extern s16 gKartAIforToad[];
extern s16 gKartAIforDk[];
extern s16 gKartAIforWario[];
extern s16 gKartAIforPeach[];
extern s16 gKartAIforBowser[];
extern s16* gKartAIforPlayer[];

extern s16 gKartAIforMarioAndLuigi[];
extern s16 gKartAIforMarioAndYoshi[];
extern s16 gKartAIforMarioAndToad[];
extern s16 gKartAIforMarioAndDk[];
extern s16 gKartAIforMarioAndWario[];
extern s16 gKartAIforMarioAndPeach[];
extern s16 gKartAIforMarioAndBowser[];
extern s16 gKartAIforLuigiAndMario[];
extern s16 gKartAIforLuigiAndYoshi[];
extern s16 gKartAIforLuigiAndToad[];
extern s16 gKartAIforLuigiAndDk[];
extern s16 gKartAIforLuigiAndWario[];
extern s16 gKartAIforLuigiAndPeach[];
extern s16 gKartAIforLuigiAndBowser[];
extern s16 gKartAIforYoshiAndMario[];
extern s16 gKartAIforYoshiAndLuigi[];
extern s16 gKartAIforYoshiAndToad[];
extern s16 gKartAIforYoshiAndDk[];
extern s16 gKartAIforYoshiAndWario[];
extern s16 gKartAIforYoshiAndPeach[];
extern s16 gKartAIforYoshiAndBowser[];
extern s16 gKartAIforToadAndMario[];
extern s16 gKartAIforToadAndLuigi[];
extern s16 gKartAIforToadAndYoshi[];
extern s16 gKartAIforToadAndDk[];
extern s16 gKartAIforToadAndWario[];
extern s16 gKartAIforToadAndPeach[];
extern s16 gKartAIforToadAndBowser[];
extern s16 gKartAIforDkAndMario[];
extern s16 gKartAIforDkAndLuigi[];
extern s16 gKartAIforDkAndToad[];
extern s16 gKartAIforDkAndYoshi[];
extern s16 gKartAIforDkAndWario[];
extern s16 gKartAIforDkAndPeach[];
extern s16 gKartAIforDkAndBowser[];
extern s16 gKartAIforWarioAndMario[];
extern s16 gKartAIforWarioAndLuigi[];
extern s16 gKartAIforWarioAndYoshi[];
extern s16 gKartAIforWarioAndDk[];
extern s16 gKartAIforWarioAndToad[];
extern s16 gKartAIforWarioAndPeach[];
extern s16 gKartAIforWarioAndBowser[];
extern s16 gKartAIforPeachAndMario[];
extern s16 gKartAIforPeachAndLuigi[];
extern s16 gKartAIforPeachAndYoshi[];
extern s16 gKartAIforPeachAndDk[];
extern s16 gKartAIforPeachAndWario[];
extern s16 gKartAIforPeachAndToad[];
extern s16 gKartAIforPeachAndBowser[];
extern s16 gKartAIforBowserAndMario[];
extern s16 gKartAIforBowserAndLuigi[];
extern s16 gKartAIforBowserAndYoshi[];
extern s16 gKartAIforBowserAndDk[];
extern s16 gKartAIforBowserAndWario[];
extern s16 gKartAIforBowserAndToad[];
extern s16 gKartAIforBowserAndPeach[];
extern s16* gListKartAIforMario[];
extern s16* gListKartAIforLuigi[];
extern s16* gListKartAIforYoshi[];
extern s16* gListKartAIforToad[];
extern s16* gListKartAIforDk[];
extern s16* gListKartAIforWario[];
extern s16* gListKartAIforPeach[];
extern s16* gListKartAIforBowser[];
extern s16** gKartAIforTwoPlayer[];

extern s16 D_801656F0;

#endif
