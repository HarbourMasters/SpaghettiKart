#ifndef ACTORS_H
#define ACTORS_H

#include "actor_types.h"
#include "code_800029B0.h"
#include "camera.h"

#define END_OF_SPAWN_DATA -0x8000

/** @cond */

void cleanup_red_and_green_shells(struct ShellActor*);
void actor_init(struct Actor*, Vec3f, Vec3s, Vec3f, s16);
void actor_not_rendered(Camera*, struct Actor*);
void actor_rendered(Camera*, struct Actor*);
void func_80297340(Camera*);
void func_802976D8(Vec3s);
void func_802976EC(Collision*, Vec3s);
void func_80297760(struct Actor*, Vec3f);
void func_802977B0(Player*);
void func_802977E4(Player*);
void init_red_shell_texture(void);
void func_8029794C(Vec3f, Vec3s, f32);
void func_802979F8(struct Actor*, f32);
void render_actor_cow(Camera*, Mat4, struct Actor*);
void update_actor_yoshi_egg(struct YoshiValleyEgg*);
void update_actor_static_plant(struct Actor*);
void update_actor_kiwano_fruit(struct KiwanoFruit*);
void update_actor_paddle_boat(struct PaddleWheelBoat*);
void update_actor_train_engine(struct TrainCar*);
void update_actor_train_tender(struct TrainCar*);
void update_actor_train_passenger_car(struct TrainCar*);
void update_actor_piranha_plant(struct PiranhaPlant*);
void render_actor_piranha_plant(Camera*, Mat4, struct PiranhaPlant*);
void render_cows(Camera*, Mat4, struct Actor*);
void evaluate_collision_player_palm_trees(Player*);
void evaluate_collision_players_palm_trees(void);
void func_80298D10(void);
void render_palm_trees(Camera*, Mat4, struct Actor*);
void render_actor_tree_mario_raceway(Camera*, Mat4, struct Actor*);
void render_actor_tree_yoshi_valley(Camera*, Mat4, struct Actor*);
void render_actor_tree_royal_raceway(Camera*, Mat4, struct Actor*);
void render_actor_tree_moo_moo_farm(Camera*, Mat4, struct Actor*);
void func_80299864(Camera*, Mat4, struct Actor*);
void render_actor_tree_bowser_castle(Camera*, Mat4, struct Actor*);
void render_actor_bush_bowser_castle(Camera*, Mat4, struct Actor*);
void render_actor_tree_frappe_snowland(Camera*, Mat4, struct Actor*);
void render_actor_tree_cactus1_kalimari_desert(Camera*, Mat4, struct Actor*);
void render_actor_tree_cactus2_kalimari_desert(Camera*, Mat4, struct Actor*);
void render_actor_tree_cactus3_kalimari_desert(Camera*, Mat4, struct Actor*);
void render_actor_kiwano_fruit(Camera*, Mat4, struct Actor*);
void render_actor_shell(Camera*, Mat4, struct ShellActor*);
void render_actor_green_shell(Camera*, Mat4, struct ShellActor*);
void render_actor_red_shell(Camera*, Mat4, struct ShellActor*);
void render_actor_blue_shell(Camera*, Mat4, struct ShellActor*);
void render_actor_banana(Camera*, Mat4, struct BananaActor*);
void update_actor_wario_sign(struct Actor*);
void update_actor_railroad_crossing(struct RailroadCrossing*);
void update_actor_mario_sign(struct Actor*);
void func_8029AC18(Camera*, Mat4, struct Actor*);
void render_actor_paddle_boat(Camera*, struct PaddleWheelBoat*, Mat4, u16);
void render_actor_box_truck(Camera*, struct Actor*);
void render_actor_school_bus(Camera*, struct Actor*);
void render_actor_car(Camera*, struct Actor*);
void render_actor_tanker_truck(Camera*, struct Actor*);
void render_actor_train_engine(Camera*, struct TrainCar*);
void render_actor_train_tender(Camera*, struct TrainCar*);
void render_actor_train_passenger_car(Camera*, struct TrainCar*);
void render_actor_falling_rock(Camera*, struct FallingRock*);
void spawn_piranha_plants(const char*);
void spawn_palm_trees(struct ActorSpawnData*);
void func_8029CF0C(struct ActorSpawnData*, struct FallingRock*);
void spawn_falling_rocks(struct ActorSpawnData*);
void update_actor_falling_rocks(struct FallingRock*);
void spawn_foliage(const char*);
void spawn_all_item_boxes(const char*);
void init_kiwano_fruit(void);
void destroy_all_actors(void);
void spawn_course_actors(void);
void init_actors_and_load_textures(void);
void play_sound_before_despawn(struct Actor*);
void destroy_actor(struct Actor*);
s16  try_remove_destructable_item(Vec3f, Vec3s, Vec3f, s16);
s16  add_actor_to_empty_slot(Vec3f, Vec3s, Vec3f, s16);
s16  spawn_actor_at_pos(Vec3f, s16);
bool query_and_resolve_collision_player_actor(Player*, Vec3f, f32, f32, f32);
bool collision_mario_sign(Player*, struct Actor*);
bool collision_piranha_plant(Player*, struct PiranhaPlant*);
bool collision_yoshi_egg(Player*, struct YoshiValleyEgg*);
bool collision_tree(Player*, struct Actor*);
bool query_collision_player_vs_actor_item(Player*, struct Actor*);
bool query_collision_actor_vs_actor(struct Actor*, struct Actor*);
void destroy_destructable_actor(struct Actor*);
void play_sound_on_destructible_actor_collision(struct Actor*, struct Actor*);
void evaluate_actor_collision_between_two_destructible_actors(struct Actor*, struct Actor*);
void evaluate_collision_between_player_actor(Player*, struct Actor*);
void evaluate_collision_for_players_and_actors(void);
void evaluate_collision_for_destructible_actors(void);
void func_802A1064(struct FakeItemBox*);
void update_actor_fake_item_box(struct FakeItemBox*);
void init_actor_hot_air_balloon_item_box(f32, f32, f32);
void update_actor_item_box_hot_air_balloon(struct ItemBox*);
void update_actor_item_box(struct ItemBox*);
void render_actor_fake_item_box(Camera*, struct FakeItemBox*);
void render_actor_item_box(Camera*, struct ItemBox*);
void render_actor_wario_sign(Camera*, struct Actor*);
void render_actor_yoshi_egg(Camera*, Mat4, struct YoshiValleyEgg*, u16);
void render_actor_mario_sign(Camera*, Mat4, struct Actor*);
void render_actor_railroad_crossing(Camera*, struct RailroadCrossing*);
void render_actor_palm_tree(Camera*, Mat4, struct PalmTree*);
void render_item_boxes(struct UnkStruct_800DC5EC*);
void render_course_actors(struct UnkStruct_800DC5EC*);
void update_course_actors(void);

// audio/external.c
extern void func_800C98B8(Vec3f, Vec3f, u32);
extern void func_800C99E0(Vec3f, s32);

extern u8 *D_802BA050;
extern u8 *D_802BA054;
extern u8 *D_802BA058;
extern struct Actor *gActorHotAirBalloonItemBox;
extern s8 gTLUTRedShell[]; // tlut 256
extern u16 D_802BA260; // Box Truck sub-type?

// end of definition of actor.c variables

extern s16 gCurrentCourseId;
extern u16 isCrossingTriggeredByIndex[];
extern Lights1 D_800DC610[];

extern s32 D_80162DF8;

extern s8 D_800DC628[];
extern s8 D_800DC630[];
extern s8 D_802B8864[];

/** @endcond */

#endif // ACTORS_H
