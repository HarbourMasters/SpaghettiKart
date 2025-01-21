#ifndef CODE_80005FD0_H
#define CODE_80005FD0_H

#include "vehicles.h"
#include "camera.h"
#include "waypoints.h"
#include <assets/common_data.h>

struct struct_801642D8 {
    /* 0x0 */ u16 unk0;
    /* 0x2 */ s16 ffff;
    /* 0x4 */ s16 timer; // confirm?
    /* 0x6 */ s16 laps;  // confirm?
    /* 0x8 */ s32 blank;
    /* 0xC */ s32 unkC;
};

struct unexpiredActors {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 actorIndex;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ // s16 compilerPadding;
    /* 0x14 */ u32 unk14;
    /* 0x18 */ s32 unk18;
}; // size = 0x1C

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} Test; // size = 0x10

// Something related to CPU item usage
typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 actorIndex;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
} D_801642D8_entry; // size = 0x10

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} UnkStruct_46D0;

typedef struct {
    s16 x;
    s16 z;
} Path2D;

/* Function Prototypes */
s16 get_angle_between_waypoints(Vec3f, Vec3f);

s32 func_80006018(f32, f32, f32, f32, f32, f32, f32, f32);
void adjust_position_by_angle(Vec3f, Vec3f, s16);
s32 set_vehicle_render_distance_flags(Vec3f, f32, s32);
void func_800065D0(s32, Player*);
void set_places(void);

void func_800070F4(void);
void func_800074D4(void);
s32 func_80007BF8(u16, u16, u16, u16, u16);
void func_80007D04(s32, Player*);
void func_80007FA4(s32, Player*, f32);

void func_80008424(s32, f32, Player*);
s32 func_800088D8(s32, s16, s16);
void func_80008DC0(s32);
s32 func_80008E58(s32, s32);
void func_80008F38(s32);

void func_80009000(s32);
void func_800090F0(s32, Player*);
f32 func_80009258(s32, f32, f32);
void func_8000929C(s32, Player*);
void update_vehicles(void);
void func_800098FC(s32, Player*);
void func_800099EC(s32, Player*);
void func_80009B60(s32);

void func_8000B140(s32);
s32 func_8000B7E4(s32, u16);
s32 func_8000B820(s32);
f32 func_8000B874(f32, f32, u16, s32);
void func_8000B95C(s32, u16, s32);
void func_8000BA14(u16, f32, f32, s16);
void func_8000BBD8(u16, f32, s16);
s16 func_8000BD94(f32, f32, f32, s32);

s16 find_closest_waypoint_track_section(f32, f32, f32, u16, s32*);
s16 func_8000C884(f32, f32, f32, s16, s32, u16);
s16 find_closest_waypoint_with_previous_waypoint(f32, f32, f32, s16, s32);
void func_8000CBA4(f32, f32, f32, s16*, s32);
void func_8000CBF8(f32, f32, f32, s16*, s32);
s16 func_8000CC88(f32, f32, f32, Player*, s32, s32*);
s16 func_8000CD24(f32, f32, f32, s16, Player*, s32, s32);

s16 find_closest_vehicles_waypoint(f32, f32, f32, s16);
s16 func_8000D24C(f32, f32, f32, s32*);
s16 func_8000D2B4(f32, f32, f32, s16, s32);
s16 func_8000D33C(f32, f32, f32, s16, s32);
f32 func_8000D3B8(s32);
void func_8000D438(s32, u16);
s16 func_8000D6D0(Vec3f, s16*, f32, f32, s16, s16);
s16 func_8000D940(Vec3f, s16*, f32, f32, s16);
s16 update_vehicle_following_waypoint(Vec3f, s16*, f32);
void set_bomb_kart_spawn_positions(void);
void func_8000DF8C(s32);

s32 add_actor_in_unexpired_actor_list(s32, s16);
s32 add_red_shell_in_unexpired_actor_list(s32);
s32 add_green_shell_in_unexpired_actor_list(s32);
s32 add_blue_shell_in_unexpired_actor_list(s32);
void delete_actor_in_unexpired_actor_list(s32);
void func_8000EEDC(void);
void generate_player_smoke(void);

void func_8000F0E0(void);
void func_8000F124(void);
void func_8000F2BC(TrackWaypoint*, size_t);
void func_8000F2DC(void);
void func_8000F628(void);

void func_800100F0(s32);
void func_80010218(s32);
f32 func_80010480(s32, u16);
void func_800107C4(s32);
s16 func_80010CB0(s32, s32);
void func_80010DBC(s32);
void func_80010E6C(s32);
f32 func_80010F40(f32, f32, f32, s32, s32);
f32 func_80010FA0(f32, f32, f32, s32, s32);

s32 func_80011014(TrackWaypoint*, TrackWaypoint*, s32, s32);
s32 process_path_data(TrackWaypoint*, TrackWaypoint*);
s32 generate_2d_path(Path2D*, TrackWaypoint*, s32);
void copy_courses_kart_ai_behaviour(void);
void reset_kart_ai_behaviour_none(s32);
void reset_kart_ai_behaviour(s32);
void kart_ai_behaviour_start(s32, Player*);
void kart_ai_behaviour_end(s32, Player*);
void kart_ai_behaviour(s32);
void func_80011EC0(s32, Player*, s32, u16);

void generate_train_waypoints(void);
void generate_ferry_waypoints(void);
void spawn_vehicle_on_road(Vec3f position, Vec3s rotation, Vec3f velocity, s32 waypointIndex,
                           s32 someMultiplierTheSequel, f32 speed);
void set_vehicle_pos_waypoint(TrainCarStuff*, Path2D*, u16);
void init_vehicles_trains(size_t, size_t, f32);
void sync_train_components(TrainCarStuff*, s16);
void update_vehicle_trains(void);
void func_80012DC0(s32, Player*);

void func_80013054(void);
void check_ai_crossing_distance(s32);
void init_vehicles_ferry(void);
void update_vehicle_paddle_boats(void);
void func_80013854(Player*);
void initialize_toads_turnpike_vehicle(f32, f32, s32, s32, VehicleStuff*, TrackWaypoint*);
f32 func_80013C74(s16, s16);
void update_vehicle_follow_waypoint(VehicleStuff*);
void func_80013F7C(s32, Player*, VehicleStuff*, f32, f32, s32, u32);

f32 func_800145A8(s16, f32, s16);
void func_800146B8(s32, s32, VehicleStuff*);
void init_vehicles_box_trucks(void);
void update_vehicle_box_trucks(void);
void func_800148C4(s32, Player*);
void func_8001490C(s32);
void init_vehicles_school_buses(void);
void update_vehicle_school_bus(void);
void func_80014A18(s32, Player*);
void func_80014A60(s32);
void init_vehicles_trucks(void);
void update_vehicle_tanker_trucks(void);
void func_80014B6C(s32, Player*);
void func_80014BB4(s32);
void init_vehicles_cars(void);
void update_vehicle_cars(void);
void func_80014CC0(s32, Player*);
void func_80014D08(s32);
void func_80014D30(s32, s32);
void func_80014DE4(s32);
f32 func_80014EE4(f32, s32);

void calculate_camera_up_vector(Camera*, s32);
void func_8001530C(void);
void func_80015314(s32, f32, s32);
void func_80015390(Camera*, Player*, s32);
void func_80015544(s32, f32, s32, s32);
void func_8001577C(Camera*, UNUSED Player*, s32, s32);
void func_80015A9C(s32, f32, s32, s16);
void func_80015C94(Camera*, Player*, s32, s32);

void func_800162CC(s32, f32, s32, s16);
void func_80016494(Camera*, Player*, s32, s32);
void func_80016C3C(s32, f32, s32);

void func_80017720(s32, f32, s32, s16);
void func_800178F4(Camera*, Player*, s32, s32);
void func_80017F10(s32, f32, s32, s16);

void func_800180F0(Camera*, Player*, s32, s32);
void func_80018718(s32, f32, s32, s16);
void func_800188F4(Camera*, Player*, s32, s32);

void func_80019118(s32, f32, s32, s16);
void func_8001933C(Camera*, UNUSED Player*, s32, s32);
void func_8001968C(void);
void func_8001969C(s32, f32, s32, s16);
void func_80019760(Camera*, UNUSED Player*, s32, s32);
void func_80019890(s32, s32);
void func_80019B50(s32, u16);
void func_80019C50(s32);
void func_80019D2C(Camera*, Player*, s32);
void func_80019DE4(void);
void func_80019DF4(void);
void func_80019E58(void);
void func_80019ED0(void);
void func_80019FB4(s32);

void func_8001A0A4(u16*, Camera*, Player*, s8, s32);
void func_8001A0DC(u16*, Camera*, Player*, s8, s32);
void func_8001A124(s32, s32);
s32 func_8001A310(s32, s32);
void func_8001A348(s32, f32, s32);
void func_8001A3D8(s32, f32, s32);
void func_8001A450(s32, s32, s32);
void func_8001A518(s32, s32, s32);
void func_8001A588(u16*, Camera*, Player*, s8, s32);
void func_8001AAAC(s16, s16, s16);
void func_8001AB00(void);
void kart_ai_decisions_branch_item(s32, s16*, s32);
void func_8001ABE0(s32, D_801642D8_entry*);
void func_8001ABEC(struct struct_801642D8*);
void kart_ai_use_item_strategy(s32);

void func_8001BE78(void);

void func_8001C05C(void);
void func_8001C14C(void);
void render_bomb_karts_wrap(s32);
void func_8001C42C(void);

/* This is where I'd put my static data, if I had any */

extern Collision D_80162E70;
extern s16 D_80162EB0; // Possibly a float.
extern s16 D_80162EB2; // possibly [3]
extern KartAIBehaviour* gCoursesKartAIBehaviour[];
extern s16 D_80162F10[];
extern s16 D_80162F50[];
extern Vec3f D_80162FA0;
extern Vec3f D_80162FB0;
extern Vec3f D_80162FC0;
extern s16 gTrainSmokeTimer;
extern s16 D_80162FD0;
extern f32 gCourseCompletionPercentByRank[]; // D_80162FD8
extern s16 D_80162FF8[];
extern s16 D_80163010[];
extern f32 D_80163028[];
extern s16 D_80163050[];
extern f32 D_80163090[];
extern s32 D_801630B8[];
extern u16 D_801630E0;
extern s16 D_801630E2;
extern s16 D_801630E8[];
extern s16 gFerrySmokeTimer;
extern s32 D_80163100[];
extern s32 D_80163128[];
extern s32 D_80163150[];
extern f32 D_80163178[];
extern f32 D_801631A0[];
extern s16 D_801631C8;
extern s32 D_801631CC;
extern u16 D_801631E0[];
extern u16 D_801631F8[];
extern f32 D_8016320C;
extern f32 D_80163210[];
extern s32 D_80163238;
extern u16 D_80163240[];
extern u16 D_80163258[];
extern u16 D_80163270[];
extern s32 D_80163288[];
extern KartAIBehaviour* sCurrentKartAIBehaviour;
extern u16 gCurrentKartAIBehaviourId[];
extern u16 gPreviousKartAIBehaviourId[];
extern u16 gKartAIBehaviourState[];

enum { KART_AI_BEHAVIOUR_STATE_NONE, KART_AI_BEHAVIOUR_STATE_START, KART_AI_BEHAVIOUR_STATE_RUNNING };

extern s16 D_80163300[];
extern u16 D_80163330[];
extern u16 D_80163344[];
extern u16 D_80163348[];
extern u16 D_8016334C[];
extern u16 gSpeedKartAIBehaviour[];

enum {
    SPEED_KART_AI_BEHAVIOUR_NORMAL,
    SPEED_KART_AI_BEHAVIOUR_FAST,
    SPEED_KART_AI_BEHAVIOUR_SLOW,
    SPEED_KART_AI_BEHAVIOUR_MAX
};

extern s32 D_80163378;
extern s32 D_8016337C;
extern s16 D_80163380[];
extern s16 D_80163398[];
extern s16 D_801633B0[];
extern s16 D_801633C8[];
extern s16 D_801633E0[];
extern s16 D_801633F8[];
extern s16 D_80163410[];
extern f32 D_80163418[];
extern f32 D_80163428[];
extern f32 D_80163438[];
extern f32 D_80163450[];
extern s16 D_80163478;
// 0 or 1, only 1 when when in extra (mirror) mode
extern s16 D_8016347A;
extern s16 D_8016347C;
extern s16 D_8016347E;
extern s32 D_80163480;
extern s32 D_80163484;
extern s32 D_80163488;
extern s16 D_8016348C;
extern s16 D_801634C0[];
extern s16 bStopAICrossing[];
extern s16 D_801634EC;
extern s32 D_801634F0;
extern s32 D_801634F4;
extern Test D_801634F8[];
extern Path2D* gVehicle2DWaypoint;
extern s32 gVehicle2DWaypointLength;
extern u16 isCrossingTriggeredByIndex[];
extern u16 sCrossingActiveTimer[];
extern s32 D_80163DD8[];
extern struct unexpiredActors gUnexpiredActorsList[];
extern D_801642D8_entry D_801642D8[];
extern s16 D_80164358;
extern s16 D_8016435A;
extern s16 D_8016435C;
extern s16 gGPCurrentRacePlayerIdByRank[]; // D_80164360
extern s16 D_80164378[];
extern s32 gLapCountByPlayerId[];          // D_80164390
extern s32 gGPCurrentRaceRankByPlayerId[]; // D_801643B8
extern s32 D_801643E0[];
extern s32 D_80164408[];
extern s16 D_80164478[];
extern s32 D_8016448C;
extern f32 D_80164498[];
extern f32 gLapCompletionPercentByPlayerId[];    // D_801644A8
extern f32 gCourseCompletionPercentByPlayerId[]; // D_801644D0
extern f32 D_80164510[];
extern s16 D_80164538[];
extern s32 D_801645D0[];
extern s32 D_801645E8[];
extern f32 D_801645F8[];
extern s32 D_80164608[];
extern f32 D_80164618[];
extern s32 D_80164628[];
extern f32 D_80164638[];
extern f32 D_80164648[];
extern f32 D_80164658[];
extern s16 D_80164670[];
extern s16 D_80164678[];
extern s16 D_80164680[];
extern f32 D_80164688[];
extern f32 D_80164698;
extern f32 D_8016469C;
extern f32 D_801646A0;
extern s16 D_801646C0[];
extern u32 D_801646C8;
extern u16 D_801646CC;
extern UnkStruct_46D0 D_801646D0[];

// See bss_80005FD0.s
extern f32 gCourseCompletionPercentByRank[NUM_PLAYERS];
extern s32 D_801643E0[];
extern s32 D_8016448C;
extern u16 D_801637BE;
extern u16 D_80163E2A;

// extern Gfx D_0D0076F8[];

#endif
