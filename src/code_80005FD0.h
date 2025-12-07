#ifndef CODE_80005FD0_H
#define CODE_80005FD0_H

#include "vehicles.h"
#include "camera.h"
#include "waypoints.h"
#include <assets/textures/common_data.h>

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
    /* 0x00 */ f32 current;
    /* 0x04 */ f32 target;
    /* 0x08 */ f32 step;
    /* 0x0C */ f32 unkC;
} TrackPositionFactorInstruction; // size = 0x10

// Something related to CPU item usage
typedef struct {
    /* 0x00 */ s16 branch;
    /* 0x02 */ s16 actorIndex;
    /* 0x04 */ s16 timer;      // confirm?
    /* 0x06 */ s16 numItemUse; // confirm?
    /* 0x08 */ s16 numDroppedBananaBunch;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 timeBeforeThrow;
} CpuItemStrategyData; // size = 0x10

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} UnkStruct_46D0;

enum CpuItemStrategyEnum {
    CPU_STRATEGY_WAIT_NEXT_ITEM = 0,

    CPU_STRATEGY_ITEM_BANANA,
    CPU_STRATEGY_HOLD_BANANA,
    CPU_STRATEGY_DROP_BANANA,

    CPU_STRATEGY_ITEM_GREEN_SHELL,
    CPU_STRATEGY_HOLD_GREEN_SHELL,
    CPU_STRATEGY_THROW_GREEN_SHELL,

    CPU_STRATEGY_ITEM_RED_SHELL,
    CPU_STRATEGY_HOLD_RED_SHELL,
    CPU_STRATEGY_THROW_RED_SHELL,

    CPU_STRATEGY_ITEM_BANANA_BUNCH,
    CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH,
    CPU_STRATEGY_DROP_BANANA_BUNCH,

    CPU_STRATEGY_ITEM_FAKE_ITEM_BOX,
    CPU_STRATEGY_HOLD_FAKE_ITEM_BOX,
    CPU_STRATEGY_THROW_FAKE_ITEM_BOX,

    CPU_STRATEGY_ITEM_THUNDERBOLT = 0x16,
    CPU_STRATEGY_END_THUNDERBOLT,

    CPU_STRATEGY_ITEM_STAR = 0x19,
    CPU_STRATEGY_END_ITEM_STAR,

    CPU_STRATEGY_ITEM_BOO,
    CPU_STRATEGY_WAIT_END_BOO,

    CPU_STRATEGY_ITEM_MUSHROOM,
    CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM,
    CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM,
    CPU_STRATEGY_ITEM_SUPER_MUSHROOM,
    CPU_STRATEGY_USE_SUPER_MUSHROOM,

    CPU_STRATEGY_THROW_BANANA,
    CPU_STRATEGY_HOLD_THROW_BANANA,
    CPU_STRATEGY_END_THROW_BANANA,

    CPU_STRATEGY_ITEM_BLUE_SPINY_SHELL,
    CPU_STRATEGY_THROW_BLUE_SPINY_SHELL,
    CPU_STRATEGY_HOLD_BLUE_SPINY_SHELL,
};

/* Function Prototypes */
s16 get_angle_between_path(Vec3f, Vec3f);

s32 is_collide_with_vehicle(f32, f32, f32, f32, f32, f32, f32, f32);
void adjust_position_by_angle(Vec3f, Vec3f, s16);
s32 set_vehicle_render_distance_flags(Vec3f, f32, s32);
void detect_wrong_player_direction(s32, Player*);
void set_places(void);

void update_player_rankings(void);
void set_places_end_course_with_time(void);
s32 is_path_point_in_range(u16, u16, u16, u16, u16);
void func_80007D04(s32, Player*);
void func_80007FA4(s32, Player*, f32);

void regulate_cpu_speed(s32, f32, Player*);
bool func_800088D8(s32, s16, s16);
void set_current_path(s32);
s32 update_player_path_selection(s32, s32);
void update_player_completion(s32);

void yoshi_valley_cpu_path(s32);
void update_cpu_path_completion(s32, Player*);
f32 func_80009258(s32, f32, f32);
void update_player_path_completion(s32, Player*);
void update_vehicles(void);
void play_cpu_sound_effect(s32, Player*);
void update_player_timer_sound(s32, Player*);
void update_player(s32);

void func_8000B140(s32);
s32 are_in_curve(s32, u16);
bool is_far_from_path(s32);
f32 calculate_track_position_factor(f32, f32, u16, s32);
void update_player_position_factor(s32, u16, s32);
void calculate_track_offset_position(u16, f32, f32, s16);
void set_track_offset_position(u16, f32, s16);
s16 func_8000BD94(f32, f32, f32, s32);

s16 find_closest_path_point_track_section(f32, f32, f32, u16, s32*);
s16 update_path_index_with_track(f32, f32, f32, s16, s32, u16);
s16 update_path_index(f32, f32, f32, s16, s32);
void tweak_path_index_wario_stadium(f32, f32, f32, s16*, s32);
void adjust_path_at_start_line(f32, f32, f32, s16*, s32);
s16 update_path_index_track_section(f32, f32, f32, Player*, s32, s32*);
s16 update_player_path(f32, f32, f32, s16, Player*, s32, s32);

s16 find_closest_vehicles_path_point(f32, f32, f32, s16);
s16 func_8000D24C(f32, f32, f32, s32*);
s16 func_8000D2B4(f32, f32, f32, s16, s32);
s16 func_8000D33C(f32, f32, f32, s16, s32);
f32 cpu_track_position_factor(s32);
void determine_ideal_cpu_position_offset(s32, u16);
s16 func_8000D6D0(Vec3f, s16*, f32, f32, s16, s16);
s16 func_8000D940(Vec3f, s16*, f32, f32, s16);
s16 update_vehicle_following_path(Vec3f, s16*, f32);
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
void clear_path_point(TrackPathPoint*, size_t);
void init_course_path_point(void);
void init_players(void);

void load_track_path(s32);
void calculate_track_boundaries(s32);
f32 calculate_track_curvature(s32, u16);
void analyze_track_section(s32);
s16 calculate_angle_path(s32, s32);
void analyse_angle_path(s32);
void analyze_curved_path(s32);
f32 func_80010F40(f32, f32, f32, s32, s32);
f32 func_80010FA0(f32, f32, f32, s32, s32);

s32 func_80011014(TrackPathPoint*, TrackPathPoint*, s32, s32);
s32 process_path_data(TrackPathPoint*, TrackPathPoint*);
s32 generate_2d_path(TrackPathPoint*, TrackPathPoint*, s32);
void copy_courses_cpu_behaviour(void);
void reset_cpu_behaviour_none(s32);
void reset_cpu_behaviour(s32);
void cpu_behaviour_start(s32, Player*);
void cpu_behaviour_end(s32, Player*);
void cpu_behaviour(s32);
void func_80011EC0(s32, Player*, s32, u16);

void generate_train_path(void);
void generate_ferry_path(void);
void spawn_vehicle_on_road(Vec3f position, Vec3s rotation, Vec3f velocity, s32 waypointIndex,
                           s32 someMultiplierTheSequel, f32 speed);
void set_vehicle_pos_path_point(TrainCarStuff*, TrackPathPoint*, u16);
void init_vehicles_trains(size_t, size_t, f32);
void sync_train_components(TrainCarStuff*, s16);
void update_vehicle_trains(void);
void handle_trains_interactions(s32, Player*);

void func_80013054(void);
void check_ai_crossing_distance(s32);
void init_vehicles_ferry(void);
void update_vehicle_paddle_boats(void);
void handle_paddle_boats_interactions(Player*);
void initialize_toads_turnpike_vehicle(f32, f32, s32, s32, VehicleStuff*, TrackPathPoint*);
f32 func_80013C74(s16, s16);
void update_vehicle_follow_path_point(VehicleStuff*);
void handle_vehicle_interactions(s32, Player*, VehicleStuff*, f32, f32, s32, u32);

f32 player_track_position_factor_vehicle(s16, f32, s16);
void update_player_track_position_factor_from_vehicle(s32, s32, VehicleStuff*);
void init_vehicles_box_trucks(void);
void update_vehicle_box_trucks(void);
void handle_box_trucks_interactions(s32, Player*);
void update_player_track_position_factor_from_box_trucks(s32);
void init_vehicles_school_buses(void);
void update_vehicle_school_bus(void);
void handle_school_buses_interactions(s32, Player*);
void update_player_track_position_factor_from_buses(s32);
void init_vehicles_trucks(void);
void update_vehicle_tanker_trucks(void);
void handle_tanker_trucks_interactions(s32, Player*);
void update_player_track_position_factor_from_tanker_truck(s32);
void init_vehicles_cars(void);
void update_vehicle_cars(void);
void handle_cars_interactions(s32, Player*);
void update_player_track_position_factor_from_cars(s32);
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
void cpu_decisions_branch_item(s32, s16*, s32);
void func_8001ABE0(s32, CpuItemStrategyData*);
void clear_expired_strategies(CpuItemStrategyData*);
void cpu_use_item_strategy(s32);

void func_8001BE78(void);

void func_8001C05C(void);
void func_8001C14C(void);
void render_bomb_karts_wrap(s32);
void func_8001C42C(void);

/* This is where I'd put my static data, if I had any */

extern Collision D_80162E70;
extern s16 D_80162EB0; // Possibly a float.
extern s16 D_80162EB2; // possibly [3]
extern CPUBehaviour* gCoursesCPUBehaviour[];
extern s16 D_80162F10[];
extern s16 D_80162F50[];
extern Vec3f gOffsetPosition;
extern Vec3f D_80162FB0;
extern Vec3f D_80162FC0;
extern s16 gTrainSmokeTimer;
extern s16 D_80162FD0;
extern f32 gCourseCompletionPercentByRank[]; // D_80162FD8
extern s16 D_80162FF8[];
extern s16 D_80163010[];
extern f32 cpu_TargetSpeed[];
extern s16 gPreviousAngleSteering[];
extern f32 D_80163090[];
extern bool gIsPlayerInCurve[];
extern u16 gCurrentNearestPathPoint;
extern s16 gIsPlayerNewPathPoint;
extern s16 D_801630E8[];
extern s16 gFerrySmokeTimer;
extern s32 D_80163100[];
extern s32 D_80163128[];
extern s32 D_80163150[];
extern f32 gPreviousPlayerAiOffsetX[];
extern f32 gPreviousPlayerAiOffsetZ[];
extern s16 sVehicleSoundRenderCounter;
extern s32 D_801631CC;
extern u16 D_801631E0[];
extern u16 D_801631F8[];
extern f32 gCurrentCpuTargetSpeed;
extern f32 gPreviousCpuTargetSpeed[];
extern s32 D_80163238;
extern u16 D_80163240[];
extern u16 gWrongDirectionCounter[];
extern u16 gIsPlayerWrongDirection[];
extern s32 gPreviousLapProgressScore[];
extern CPUBehaviour* sCurrentCPUBehaviour;
extern u16 gCurrentCPUBehaviourId[];
extern u16 gPreviousCPUBehaviourId[];
extern u16 cpu_BehaviourState[];

enum { CPU_BEHAVIOUR_STATE_NONE, CPU_BEHAVIOUR_STATE_START, CPU_BEHAVIOUR_STATE_RUNNING };

extern s16 sPlayerAngle[];
extern u16 D_80163330[];
extern u16 D_80163344[];
extern u16 D_80163348[];
extern u16 D_8016334C[];
extern u16 gSpeedCPUBehaviour[];

enum { SPEED_CPU_BEHAVIOUR_NORMAL, SPEED_CPU_BEHAVIOUR_FAST, SPEED_CPU_BEHAVIOUR_SLOW, SPEED_CPU_BEHAVIOUR_MAX };

extern s32 gIncrementUpdatePlayer;
extern s32 D_8016337C;
extern s16 gCurrentPlayerLookAhead[];
extern s16 D_80163398[];
extern s16 D_801633B0[];
extern s16 D_801633C8[];
extern s16 D_801633E0[];
extern s16 D_801633F8[];
extern s16 D_80163410[];
extern f32 D_80163418[];
extern f32 D_80163428[];
extern f32 D_80163438[];
extern f32 gPreviousPlayerZ[];
extern s16 gBestRankedHumanPlayer;
// 0 or 1, only 1 when when in extra (mirror) mode
extern s16 gIsInExtra;
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
extern TrackPositionFactorInstruction gPlayerTrackPositionFactorInstruction[];
extern TrackPathPoint* gVehicle2DPathPoint;
extern s32 gVehicle2DPathSize;
extern TrackPathPoint* gVehiclePath;
extern size_t gVehiclePathSize;
extern u16 isCrossingTriggeredByIndex[];
extern u16 sCrossingActiveTimer[];
extern s32 D_80163DD8[];
extern struct unexpiredActors gUnexpiredActorsList[];
extern CpuItemStrategyData cpu_ItemStrategy[];
extern s16 D_80164358;
extern s16 D_8016435A;
extern s16 D_8016435C;
extern s16 gGPCurrentRacePlayerIdByRank[]; // D_80164360
extern s16 D_80164378[];
extern s32 gLapCountByPlayerId[];          // D_80164390
extern s32 gGPCurrentRaceRankByPlayerId[]; // D_801643B8
extern s32 gPreviousGPCurrentRaceRankByPlayerId[];
extern s32 gGPCurrentRaceRankByPlayerIdDup[];
extern s16 gGetPlayerByCharacterId[];
extern s32 D_8016448C;
extern f32 D_80164498[];
extern f32 gLapCompletionPercentByPlayerId[];    // D_801644A8
extern f32 gCourseCompletionPercentByPlayerId[]; // D_801644D0
extern f32 gPlayerPathY[];
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
extern s32 D_8016448C;
extern u16 D_801637BE;
extern u16 D_80163E2A;

#define SEVERE_WRONG_DIRECTION_MIN 136
#define SEVERE_WRONG_DIRECTION_MAX 225
#define SEVERE_CORRECT_DIRECTION_MIN 45
#define SEVERE_CORRECT_DIRECTION_MAX 316
#define WRONG_DIRECTION_FRAMES_LIMIT 5

// extern Gfx D_0D0076F8[];

#endif
