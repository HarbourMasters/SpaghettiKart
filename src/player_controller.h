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
void func_80028F70(void);

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
void update_player_environment_and_hazard_state(Player*, s8);
void func_8002C7E4(Player*, s8, s8);
void func_8002C954(Player*, s8, Vec3f);
void apply_effect(Player*, s8, s8);

void func_8002D028(Player*, s8);
void func_8002D268(Player*, Camera*, s8, s8);

void set_player_height(Player*);
void control_cpu_movement(Player*, Camera*, s8, s8);

void func_8002FCA8(Player*, s8);
void func_8002FE84(Player*, f32);

f32 func_80030150(Player*, s8);
void update_player_kart_hop(Player*);
void detect_triple_a_combo_a_released(Player*);
void detect_triple_a_combo_a_pressed(Player*);
void player_accelerate_alternative(Player*);
void player_decelerate_alternative(Player*, f32);
void detect_triple_b_combo_b_released(Player*);
void detect_triple_b_combo_b_pressed(Player*);
void func_800323E4(Player*);
void player_accelerate_during_start_sequence(Player*);
void player_decelerate_during_start_sequence(Player*, f32);
void player_accelerate(Player*);
void player_decelerate(Player*, f32);
void player_accelerate_global(Player*, s32);
void player_decelerate_global(Player*, f32, s32);
void func_80033850(Player*, f32);
void func_80033884(Player*, s32*, s32*, s32, s32, s32, s32);
void func_80033940(Player*, s32*, s32, s32, f32);
void func_800339C4(Player*, s32*, s32, s32, f32);
void func_80033A40(Player*, s32*, s32*, s32, s32, s32, f32);
void func_80033AE0(Player*, struct Controller*, s8);

void apply_cpu_turn(Player*, s16);
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

void handle_a_press_for_player_during_race(Player*, struct Controller*, s8);
void handle_a_press_for_all_players_during_race(void);
s16 func_80038534(struct Controller*);
s16 func_800388B0(struct Controller*);
void func_80038BE4(Player*, s16);
void func_80038C6C(Player*, Camera*, s8, s8);

/* This is where I'd put my static data, if I had any */

extern s16 cpu_forMario[];
extern s16 cpu_forLuigi[];
extern s16 cpu_forYoshi[];
extern s16 cpu_forToad[];
extern s16 cpu_forDk[];
extern s16 cpu_forWario[];
extern s16 cpu_forPeach[];
extern s16 cpu_forBowser[];
extern s16* cpu_forPlayer[];

extern s16 cpu_forMarioAndLuigi[];
extern s16 cpu_forMarioAndYoshi[];
extern s16 cpu_forMarioAndToad[];
extern s16 cpu_forMarioAndDk[];
extern s16 cpu_forMarioAndWario[];
extern s16 cpu_forMarioAndPeach[];
extern s16 cpu_forMarioAndBowser[];
extern s16 cpu_forLuigiAndMario[];
extern s16 cpu_forLuigiAndYoshi[];
extern s16 cpu_forLuigiAndToad[];
extern s16 cpu_forLuigiAndDk[];
extern s16 cpu_forLuigiAndWario[];
extern s16 cpu_forLuigiAndPeach[];
extern s16 cpu_forLuigiAndBowser[];
extern s16 cpu_forYoshiAndMario[];
extern s16 cpu_forYoshiAndLuigi[];
extern s16 cpu_forYoshiAndToad[];
extern s16 cpu_forYoshiAndDk[];
extern s16 cpu_forYoshiAndWario[];
extern s16 cpu_forYoshiAndPeach[];
extern s16 cpu_forYoshiAndBowser[];
extern s16 cpu_forToadAndMario[];
extern s16 cpu_forToadAndLuigi[];
extern s16 cpu_forToadAndYoshi[];
extern s16 cpu_forToadAndDk[];
extern s16 cpu_forToadAndWario[];
extern s16 cpu_forToadAndPeach[];
extern s16 cpu_forToadAndBowser[];
extern s16 cpu_forDkAndMario[];
extern s16 cpu_forDkAndLuigi[];
extern s16 cpu_forDkAndToad[];
extern s16 cpu_forDkAndYoshi[];
extern s16 cpu_forDkAndWario[];
extern s16 cpu_forDkAndPeach[];
extern s16 cpu_forDkAndBowser[];
extern s16 cpu_forWarioAndMario[];
extern s16 cpu_forWarioAndLuigi[];
extern s16 cpu_forWarioAndYoshi[];
extern s16 cpu_forWarioAndDk[];
extern s16 cpu_forWarioAndToad[];
extern s16 cpu_forWarioAndPeach[];
extern s16 cpu_forWarioAndBowser[];
extern s16 cpu_forPeachAndMario[];
extern s16 cpu_forPeachAndLuigi[];
extern s16 cpu_forPeachAndYoshi[];
extern s16 cpu_forPeachAndDk[];
extern s16 cpu_forPeachAndWario[];
extern s16 cpu_forPeachAndToad[];
extern s16 cpu_forPeachAndBowser[];
extern s16 cpu_forBowserAndMario[];
extern s16 cpu_forBowserAndLuigi[];
extern s16 cpu_forBowserAndYoshi[];
extern s16 cpu_forBowserAndDk[];
extern s16 cpu_forBowserAndWario[];
extern s16 cpu_forBowserAndToad[];
extern s16 cpu_forBowserAndPeach[];
extern s16* gListCPUforMario[];
extern s16* gListCPUforLuigi[];
extern s16* gListCPUforYoshi[];
extern s16* gListCPUforToad[];
extern s16* gListCPUforDk[];
extern s16* gListCPUforWario[];
extern s16* gListCPUforPeach[];
extern s16* gListCPUforBowser[];
extern s16** cpu_forTwoPlayer[];

extern s16 D_801656F0;

#endif
