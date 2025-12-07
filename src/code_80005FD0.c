#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <stubs.h>
#include "course_metadata.h"
#include <align_asset_macro.h>

#include "code_80005FD0.h"
#include "math_util.h"
#include "code_800029B0.h"
#include "racing/memory.h"
#include "waypoints.h"
#include "camera.h"
#include "actors.h"
#include "actors_extended.h"
#include "render_player.h"
#include "player_controller.h"
#include "update_objects.h"
#include "collision.h"
#include <actor_types.h>
#include "vehicles.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "bomb_kart.h"
#include "courses/all_course_data.h"
#include <assets/models/common_data.h>
#include <common_structs.h>
#include "main.h"
#include "menus.h"
#include "menu_items.h"
#include "audio/external.h"
#include "ending/podium_ceremony_actors.h"
#include "spawn_players.h"
#include "sounds.h"
#include "data/path_spawn_metadata.h"
#include <assets/models/tracks/kalimari_desert/kalimari_desert_data.h>
#include <assets/other/tracks/kalimari_desert/kalimari_desert_data.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_data.h>
#include <assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h>
#include <assets/other/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h>
#include <stdio.h>

#include "port/Game.h"
#include "engine/courses/Course.h"

s32 unk_code_80005FD0_pad[24];
Collision D_80162E70;
s16 D_80162EB0; // Possibly a float.
s16 D_80162EB2; // possibly [3]

CPUBehaviour* gCoursesCPUBehaviour[NUM_COURSES - 1];

// more padding?
s32 D_80162F08[2];

s16 D_80162F10[30];
s16 D_80162F50[30];

// padding, seemingly?
s32 D_80162F90[4];

Vec3f gOffsetPosition;
Vec3f D_80162FB0;
Vec3f D_80162FC0;
s16 gTrainSmokeTimer;
s16 sSomeNearestPathPoint;
s16 D_80162FD0;
f32 gCourseCompletionPercentByRank[NUM_PLAYERS];
/*
The size of many of these arrays is weird. Based on their usage, they would only need
to be 8 entries long (enough for each player). But some are 10 or even 12 long.
Its plausible that this is just some decompilation artifact?
Or maybe at some point in development they had plans for more players?
*/
s16 D_80162FF8[12];
s16 D_80163010[12];
f32 cpu_TargetSpeed[10];
s16 gPreviousAngleSteering[12];
f32 gTrackPositionFactor[10];
f32 D_80163090[10];
bool gIsPlayerInCurve[10];
u16 gCurrentNearestPathPoint;
s16 gIsPlayerNewPathPoint;
s16 D_801630E8[10];
s16 gFerrySmokeTimer;
s32 D_80163100[10];
s32 D_80163128[10];
s32 D_80163150[10];
f32 gPreviousPlayerAiOffsetX[10];
f32 gPreviousPlayerAiOffsetZ[10];
s16 sVehicleSoundRenderCounter;
s32 D_801631CC;
TrackPathPoint* gCurrentTrackLeftPath;
TrackPathPoint* gCurrentTrackRightPath;
s16* gCurrentTrackSectionTypesPath;
s16* gCurrentPathPointExpectedRotationPath;
u16 D_801631E0[12];
u16 D_801631F8[10];
f32 gCurrentCpuTargetSpeed;
f32 gPreviousCpuTargetSpeed[10];
s32 D_80163238;
u16 D_80163240[12];
u16 gWrongDirectionCounter[12];
u16 gIsPlayerWrongDirection[12];
s32 gPreviousLapProgressScore[10];
CPUBehaviour* sCurrentCPUBehaviour;
u16 gCurrentCPUBehaviourId[12];
u16 gPreviousCPUBehaviourId[12];
u16 cpu_BehaviourState[12];
s16 sPlayerAngle[12];
u16 gPlayersTrackSectionId[12];
u16 D_80163330[10];
u16 D_80163344[2];
u16 D_80163348[2];
u16 D_8016334C[8]; // Increased from 2 to 8 to prevent array overflow.
u16 gSpeedCPUBehaviour[12];
s32 D_80163368[4];
s32 gIncrementUpdatePlayer;
s32 D_8016337C;
s16 gCurrentPlayerLookAhead[12];
s16 D_80163398[12];
s16 D_801633B0[12];
s16 D_801633C8[12];
s16 D_801633E0[12];
s16 D_801633F8[12];
s16 D_80163410[4];
f32 D_80163418[4];
f32 D_80163428[4];
f32 D_80163438[4];
s32 gPlayerPathIndex;
f32 gPathStartZ;
f32 gPreviousPlayerZ[10];
s16 gBestRankedHumanPlayer;
s16 gIsInExtra;
s16 D_8016347C;
s16 D_8016347E;
s32 D_80163480;
s32 D_80163484;
s32 D_80163488;
s16 D_8016348C;
s16 cpu_enteringPathIntersection[12];
s16 cpu_exitingPathIntersection[12];
s16 D_801634C0[12];
s16 bStopAICrossing[10];
s16 D_801634EC;
s32 D_801634F0;
s32 D_801634F4;
TrackPositionFactorInstruction gPlayerTrackPositionFactorInstruction[10];
TrackPathPoint* gVehicle2DPathPoint;
s32 gVehicle2DPathSize;
TrackPathPoint* gVehiclePath;
size_t gVehiclePathSize;
TrainStuff gTrainList[NUM_TRAINS];
u16 isCrossingTriggeredByIndex[NUM_CROSSINGS];
u16 sCrossingActiveTimer[NUM_CROSSINGS];
PaddleBoatStuff gPaddleBoats[NUM_PADDLE_BOATS];
VehicleStuff gBoxTruckList[NUM_RACE_BOX_TRUCKS];
VehicleStuff gSchoolBusList[NUM_RACE_SCHOOL_BUSES];
VehicleStuff gTankerTruckList[NUM_RACE_TANKER_TRUCKS];
VehicleStuff gCarList[NUM_RACE_CARS];
s32 D_80163DD8[4];
BombKart gBombKarts[NUM_BOMB_KARTS_MAX];
Collision gBombKartCollision[NUM_BOMB_KARTS_MAX];
struct unexpiredActors gUnexpiredActorsList[8];
CpuItemStrategyData cpu_ItemStrategy[8];
s16 D_80164358;
s16 D_8016435A;
s16 D_8016435C;
s16 gGPCurrentRacePlayerIdByRank[12]; // D_80164360
s16 D_80164378[12];
s32 gLapCountByPlayerId[10];          // D_80164390
s32 gGPCurrentRaceRankByPlayerId[10]; // D_801643B8
s32 gPreviousGPCurrentRaceRankByPlayerId[10];
s32 gGPCurrentRaceRankByPlayerIdDup[10];
u16 gSelectedPathCount;
u16 gNearestPathPointByPlayerId[12];
s32 gNumPathPointsTraversed[10];
s16 gGetPlayerByCharacterId[10];
s32 D_8016448C;
TrackPathPoint* gCurrentTrackPath;
f32 D_80164498[4];
f32 gLapCompletionPercentByPlayerId[10];    // D_801644A8
f32 gCourseCompletionPercentByPlayerId[10]; // D_801644D0
s16 bInMultiPathSection[12];
f32 gPlayerPathY[10];
s16 D_80164538[12];
TrackPathPoint* gTrackPaths[4];
TrackPathPoint* gTrackLeftPaths[4];
TrackPathPoint* gTrackRightPaths[4];
s16* gTrackSectionTypes[4];
s16* gPathExpectedRotation[4];
s16* gTrackConsecutiveCurveCounts[4];
u16 gPathIndexByPlayerId[12]; // D_801645B0
u16 gPathCountByPathIndex[4]; // D_801645C8
s32 D_801645D0[NUM_CAMERAS];
s16* gCurrentTrackConsecutiveCurveCountsPath;
s32 D_801645E8[NUM_CAMERAS];
f32 D_801645F8[NUM_CAMERAS];
s32 D_80164608[NUM_CAMERAS];
f32 D_80164618[NUM_CAMERAS];
s32 D_80164628[NUM_CAMERAS];
f32 D_80164638[NUM_CAMERAS];
f32 D_80164648[NUM_CAMERAS];
f32 D_80164658[NUM_CAMERAS];
s16 gNearestPathPointByCameraId[NUM_CAMERAS];
s16 D_80164670[NUM_CAMERAS];
s16 D_80164678[NUM_CAMERAS];
s16 D_80164680[NUM_CAMERAS];
f32 D_80164688[NUM_CAMERAS];
f32 D_80164698;
f32 D_8016469C;
f32 D_801646A0;
// start padding
s32 D_801646A4;
s32 D_801646A8;
s32 D_801646AC;
s32 D_801646B0;
s32 D_801646B4;
s32 D_801646B8;
s32 D_801646BC;
// end padding
s16 D_801646C0[NUM_CAMERAS];
u32 D_801646C8;
u16 D_801646CC;
UnkStruct_46D0 D_801646D0[NUM_CAMERAS];

// Strings, presented by google translate!
// Note that these are EUC-JP encoded, see:
// https://en.wikipedia.org/wiki/Extended_Unix_Code#EUC-JP

// Force sort immediately after goal
char* D_800EB710 = "ゴール直後の強制ソート\n";
// Forced sort immediately after one goal in 2PGP
char* D_800EB728 = "2PGPで片方がゴール直後の強制ソート\n";
// rank calculation error
char* D_800EB74C = "順位計算エラー！！ (num %d) (rank %d) (e_rank %d)\n";
// Bypass switching error!!!(num %d org_bipas %d bipas %d)
char* D_800EB780 = "バイパス切り替え エラー!!!(num %d  org_bipas %d  bipas %d)\n";
char* D_800EB7BC = "(%d) rap %3d  rate_count_F %10.2f  rap_count_F %10.2f  area %5d \n";
// Enter the maze! enemy %d (%d --> %d)
char* D_800EB800 = "迷路に突入！ enemy %d (%d --> %d)\n";
// Out of the maze! enemy %d (%d --> %d)
char* D_800EB824 = "迷路から出た！ enemy %d (%d --> %d)\n";
char* D_800EB84C = "enemy voice set (%d  slip_flag %x  weapon %x)\n";
// Spin Voice! ! (%d , name %d)
char* D_800EB87C = "スピンヴォイス！！(%d , name %d)\n";
// Damage voice! ! (%d, name %d)
char* D_800EB8A0 = "ダメージヴォイス！！(%d, name %d)\n";
char* D_800EB8C4 = "===== ENEMY DRIVE SUB (%d) =====\n";
// omission
char* D_800EB8E8 = "ENEMY END(手抜き)\n\n";
char* D_800EB8FC = "ENEMY END(手抜き)\n\n";
char* D_800EB910 = "(1)enemy stick angle over!! (%d)\n";
char* D_800EB934 = "ENEMY END\n\n";
char* D_800EB940 = "(2)enemy stick angle over!! (%d)\n";
char* D_800EB964 = "ENEMY END\n\n";
// AREA ERR!!! (group not registered at current centerline %d) %d
char* D_800EB970 = "AREA ERR!!! (現在のセンターライン %d に未登録のグループです) %d\n";
// AREA ERR!!! (Unregistered group) %d
char* D_800EB9B4 = "AREA ERR!!! (未登録のグループです) %d\n";
// get_oga_area_sub_BP() ... Area not found! (b_num = %d)
char* D_800EB9DC = "get_oga_area_sub_BP() ... エリアが見つからないッス！ (b_num = %d)\n";
// Status: (%d, %d, %d)
char* D_800EBA20 = "  状況: (%d, %d, %d) \n";
char* D_800EBA38 = "<%d> (%d, %d, %d) [%d] lng %f\n";
// Wario Stadium Jump failed! ! ! (area %d, y %7.2f)
char* D_800EBA58 = "ワリオスタジアム  ジャンプ失敗！！！ (area %d, y %7.2f)\n";
// I fell in the water! ! Forced to centerline (num %d: area %d ) (%d,%d,%d)
char* D_800EBA94 = "水に落ちた！！  センターラインに強制移動しました (num %d: area %d ) (%d,%d,%d)\n";
// Course match! ! (Slacking: with bump) Forced move to center line (num %d: area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBAE4 = "こーすあうと！！（手抜き中:バンプ有り）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! (Sitting corners: no bump) Forced move to center line (num %d: area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBB60 = "こーすあうと！！（手抜き中:バンプ無し）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! ! Recalculated area (num %d: area %d ==>%d)
char* D_800EBBDC = "こーすあうと！！！    エリアを再計算しました (num %d: area %d ==>%d)\n";
// Direct BOM(%d) (%7.2f, %7.2f, %7.2f)
char* D_800EBC24 = "直接指定のBOM(%d) (%7.2f, %7.2f, %7.2f) \n";
char* D_800EBC50 = "BOM HIT CHECK\n";
char* D_800EBC60 = "BOM HIT !!!!! (%d)\n";
// BOM standby
char* D_800EBC74 = "BOM待機\n";
char* D_800EBC80 = "RESULT BOM area(%d)\n";
// BOM dropped.
char* D_800EBC98 = "BOM が 落ちました。\n";
// Tortoise fire pillar SET failed (TABLE IS FULL)
char* D_800EBCB0 = "カメ用火柱 SET 失敗 (TABLE IS FULL)\n";
// Red turtle fire pillar set error! (category %d)
char* D_800EBCD8 = "赤ガメ火柱セットエラー！ (category %d)\n";
// Blue turtle fire pillar set error! (category %d)
char* D_800EBD00 = "青ガメ火柱セットエラー！ (category %d)\n";
// Thorn Turtle Fire Pillar Set Error! (category %d)
char* D_800EBD28 = "トゲガメ火柱セットエラー！ (category %d)\n";
// Turtle Fire Pillar Initialization! !
char* D_800EBD54 = "カメ火柱初期化！！\n";
// Center line initialization
char* D_800EBD68 = "センターライン初期化\n";
char* D_800EBD80 = "MAP NUMBER %d\n";
char* D_800EBD90 = "center_EX ptr      = %x %x (%x)\n";
char* D_800EBDB4 = "\n";
char* D_800EBDB8 = "center_BP[%d] ptr         = %x %x (%x)\n";
char* D_800EBDE0 = "side_point_L_BP[%d] ptr   = %x %x (%x)\n";
char* D_800EBE08 = "side_point_R_BP[%d] ptr   = %x %x (%x)\n";
char* D_800EBE30 = "curve_BP[%d] ptr          = %x %x (%x)\n";
char* D_800EBE58 = "angle_BP[%d] ptr          = %x %x (%x)\n";
char* D_800EBE80 = "short_cut_data_BP[%d] ptr = %x %x (%x)\n";
char* D_800EBEA8 = "\n";
// Ogawa total memory used = %d
char* D_800EBEAC = "小川の使用メモリー合計 = %d\n";
// Enemy initialization
char* D_800EBECC = "敵初期化\n";
// End of enemy initialization
char* D_800EBED8 = "敵初期化終了\n";
// Bypass CENTER LINE Split start
char* D_800EBEE8 = "バイパス CENTER LINE 分割開始\n";
// Read centerline from ROM (map:%d)
char* D_800EBF08 = "センターラインをROMから読みます (map:%d)\n";
char* D_800EBF34 = "ROM center (BP%d) line adr. = %x (%x)\n";
// Calculate centerline (map:%d)
char* D_800EBF5C = "センターラインを計算します (map:%d)\n";
char* D_800EBF84 = "center (BP%d) line adr. = %x (%x)\n";
char* D_800EBFA8 = "BP center_point_number : %d\n";
// Centerline data error! !
char* D_800EBFC8 = "センターライン データ エラー！！\n";
// Bypass CENTER LINE split end (%d -> %d number)
char* D_800EBFEC = "バイパス CENTER LINE 分割終了 (%d -> %d 個)\n";
// No center line. (map: %d)
char* D_800EC01C = "センターラインが ありません。(map:%d)\n";
// side point calculation (bypass %d)
char* D_800EC044 = "サイドポイント計算 (バイパス %d)\n";
// Curve data calculation (bypass %d)
char* D_800EC068 = "カーブデータ計算 (バイパス %d)\n";
// No center line. (map: %d)
char* D_800EC088 = "センターラインが ありません。(map:%d)\n";
// Angle data calculation (bypass %d)
char* D_800EC0B0 = "アングルデータ計算 (バイパス %d) \n";
// No center line. (map: %d)
char* D_800EC0D4 = "センターラインが ありません。(map:%d)\n";
// Shortcut data calculation (bypass %d)
char* D_800EC0FC = "ショートカットデータ計算 (バイパス %d)\n";
char* D_800EC124 = "extern POINT rom_center_KT%d_BP%d[] = {\n";
char* D_800EC150 = "\t{%d,%d,%d,%d},\n";
char* D_800EC164 = "\t0x8000,0x8000,0x8000,0\n};\n\n";
char* D_800EC184 = "area read from ROM (%d)\n";
// Normal jump! ! ! (%d)
char* D_800EC1A0 = "ノーマルジャンプ！！！(%d)\n";
// Turbo on! ! ! (%d)
char* D_800EC1BC = "ターボオン！！！(%d)\n";
// No cutting corners! ! ! (%d)
char* D_800EC1D4 = "手抜き禁止！！！(%d)\n";
// Action start data error! (num %d, act %d)
char* D_800EC1EC = "アクション開始データエラー！(num %d, act %d)\n";
// Action end data error! (num %d, act %d, old_act_num %d)
char* D_800EC21C = "アクション終了データエラー！(num %d,  act %d,  old_act_num %d)\n";
char* D_800EC25C = "SL : center_point_number : %d\n";
// SL: CENTER LINE split start
char* D_800EC27C = "SL: CENTER LINE 分割開始\n";
// SL: CENTER LINE split ended (%d -> %d indivual)
char* D_800EC298 = "SL: CENTER LINE 分割終了 (%d -> %d 個)\n";
char* D_800EC2C0 = "SHIP : center_point_number : %d\n";
// SHIP: CENTER LINE split start
char* D_800EC2E4 = "SHIP: CENTER LINE 分割開始\n";
// SHIP: CENTER LINE split ended (%d -> %d indivual)
char* D_800EC300 = "SHIP: CENTER LINE 分割終了 (%d -> %d 個)\n";
// General-purpose OBJ character initialization
char* D_800EC32C = "汎用OBJキャラ初期化\n";
// SL OBJ settings
char* D_800EC344 = "SL OBJ設定\n";
// SHIP OBJ settings
char* D_800EC350 = "SHIP OBJ設定\n";
// Track OBJ settings
char* D_800EC360 = "トラックOBJ設定\n";
// Bus OBJ setting
char* D_800EC374 = "バスOBJ設定\n";
// Tank OBJ setting
char* D_800EC384 = "タンクOBJ設定\n";
// RV OBJ settings
char* D_800EC394 = "RV OBJ設定\n";
// Generic OBJ character initialization completed
char* D_800EC3A0 = "汎用OBJキャラ初期化終了\n";
// horn (num %d, permit %d, %d)
char* D_800EC3BC = "クラクション (num %d, permit %d, %d)\n";
char* D_800EC3E4 = "OGA CAMERA INIT (%d)\n";
char* D_800EC3FC = "OGA CAMERA INIT END\n";
// High speed camera ERR !!! (ncx = %f)
char* D_800EC414 = "高速カメラ ERR !!! (ncx = %f)\n";
// High speed camera ERR !!! (ncz = %f)
char* D_800EC434 = "高速カメラ ERR !!! (ncz = %f)\n";
// High speed camera ERR !!! (ecx = %f)
char* D_800EC454 = "高速カメラ ERR !!! (ecx = %f)\n";
// High speed camera ERR !!! (ecz = %f)
char* D_800EC474 = "高速カメラ ERR !!! (ecz = %f)\n";
char* D_800EC494 = "OGA DRIVERS POINT CAMERA MODE \n";
char* D_800EC4B4 = "OGA WINNER CAMERA MODE \n";
char* D_800EC4D0 = "OGA TIMEATTACK QUICK CAMERA INIT \n";
char* D_800EC4F4 = "OGA BATTLE CAMERA INIT win(%d)\n";
char* D_800EC514 = "GOAL! <<rank 1>> camera %d  rank %d\n";
char* D_800EC53C = "GOAL! <<rank 2,3,4>> camera %d  rank %d\n";
char* D_800EC568 = "GOAL! <<rank 5,6,7,8>> camera %d  rank %d\n";
// Camera and cart collided! ! !
char* D_800EC594 = "カメラとカートが衝突しました！！！  (%d)\n";
char* D_800EC5C0 = "<<< ITEM OBJ NUMBER ERR !! >>> item %d  obj_num %d \n";
// <<< BANANA SET HOUSE >>> obj_num %d zure %f
char* D_800EC5F8 = "<<< BANANA SET 失敗 >>> obj_num %d   zure %f \n";
// BANANA Caught in owner check. (num %d)
char* D_800EC628 = "BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC65C = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC670 = "理由: category \n";
// Reason: sparam
char* D_800EC684 = "理由: sparam \n";
// Reason: num
char* D_800EC694 = "理由: num \n";
char* D_800EC6A0 = "BANANA HOLD (num %d  time %d   hold_time %d)\n";
// Installation Caught in BANANA owner check. (num %d)
char* D_800EC6D0 = "設置 BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC708 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC71C = "理由: category \n";
// Reason: sparam
char* D_800EC730 = "理由: sparam \n";
// Reason: num
char* D_800EC740 = "理由: num \n";
// I put BANANA. (num %d)
char* D_800EC74C = "BANANA 置きました。 (num %d)\n";
// <<< BANANA NAGE SET failed >>> obj_num %d
char* D_800EC76C = "<<< BANANA NAGE SET 失敗 >>> obj_num %d \n";
// BANANA NAGE MOVE Caught in owner check. (num %d)
char* D_800EC798 = "BANANA NAGE MOVE 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC7D8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC7EC = "理由: category \n";
// Reason: sparam
char* D_800EC800 = "理由: sparam \n";
// Reason: num
char* D_800EC810 = "理由: num \n";
char* D_800EC81C = "BANANA NAGE END 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC858 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC86C = "理由: category \n";
// Reason: sparam
char* D_800EC880 = "理由: sparam \n";
// Reason: num
char* D_800EC890 = "理由: num \n";
char* D_800EC89C = "G_SHELL HOLD (num %d  time %d   hold_time %d)\n";
// <<< G_SHELL SET failed >>> obj_num %d
char* D_800EC8CC = "<<< G_SHELL SET 失敗 >>> obj_num %d \n";
// <<< G_SHELL SET failed >>> object_count %d
char* D_800EC8F4 = "<<< G_SHELL SET 失敗 >>> object_count %d \n";
// G_SHELL Caught in owner check. (num %d)
char* D_800EC920 = "G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC954 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC968 = "理由: category \n";
// Reason: sparam
char* D_800EC97C = "理由: sparam \n";
// Reason: num
char* D_800EC98C = "理由: num \n";
// Just before launch G_SHELL Caught in owner check. (num %d)
char* D_800EC998 = "発射直前 G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC9D8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC9EC = "理由: category \n";
// Reason: sparam
char* D_800ECA00 = "理由: sparam \n";
// Reason: num
char* D_800ECA10 = "理由: num \n";
// G_SHELL firing (num %d)
char* D_800ECA1C = "G_SHELL 発射 (num %d)\n";
char* D_800ECA34 = "R_SHELL HOLD (num %d  time %d   hold_time %d  obj_num %d)\n";
// <<< R_SHELL SET failed >>> obj_num %d
char* D_800ECA70 = "<<< R_SHELL SET 失敗 >>> obj_num %d \n";
// <<< R_SHELL SET failed >>> object_count %d
char* D_800ECA98 = "<<< R_SHELL SET 失敗 >>> object_count %d \n";
// R_SHELL Caught in owner check. (num %d)
char* D_800ECAC4 = "R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECAF8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECB0C = "理由: category \n";
// Reason: sparam
char* D_800ECB20 = "理由: sparam \n";
// Reason: num
char* D_800ECB30 = "理由: num \n";
char* D_800ECB3C = "R_SHELL SHOOT (num %d  time %d   hold_time %d  obj_num %d)\n";
// Just before launch R_SHELL Caught in owner check. (num %d)
char* D_800ECB78 = "発射直前 R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECBB8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECBCC = "理由: category \n";
// Reason: sparam
char* D_800ECBE0 = "理由: sparam \n";
// Reason: num
char* D_800ECBF0 = "理由: num \n";
// R_SHELL firing (num %d)
char* D_800ECBFC = "R_SHELL 発射 (num %d)\n";
char* D_800ECC14 = "S_BANANA HOLD (num %d  time %d   hold_time %d)\n";
// <<< SUPER_BANANA SET failed >>> obj_num %d
char* D_800ECC44 = "<<< SUPER_BANANA SET 失敗 >>> obj_num %d \n";
// <<< SUPER_BANANA SET failed >>> object_count %d
char* D_800ECC70 = "<<< SUPER_BANANA SET 失敗 >>> object_count %d \n";
// S_BANANA Caught in owner check. (num %d)
char* D_800ECCA0 = "S_BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: category
char* D_800ECCD8 = "理由: category \n";
// Reason: sparam
char* D_800ECCEC = "理由: sparam \n";
// Reason: sb_ok
char* D_800ECCFC = "理由: sb_ok \n";
char* D_800ECD0C = "S_BANANA RELEASE (num %d  time %d )\n";
// <<< FAKE IBOX SET failed >>> obj_num %d
char* D_800ECD34 = "<<< FAKE IBOX SET 失敗 >>> obj_num %d \n";
// IBOX Caught in owner check. (num %d)
char* D_800ECD5C = "IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECD90 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECDA4 = "理由: category \n";
// Reason: sparam
char* D_800ECDB8 = "理由: sparam \n";
// Reason: num
char* D_800ECDC8 = "理由: num \n";
char* D_800ECDD4 = "FBOX HOLD (num %d  time %d   hold_time %d)\n";
// Installation IBOX owner check failed. (num %d)
char* D_800ECE00 = "設置 IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECE38 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECE4C = "理由: category \n";
// Reason: sparam
char* D_800ECE60 = "理由: sparam \n";
// Reason: num
char* D_800ECE70 = "理由: num \n";
// Ray START (%d)
char* D_800ECE7C = "雷START (%d)\n";
// Ray END (%d)
char* D_800ECE8C = "雷END (%d)\n";
// ---------- Initialization of commendation table
char* D_800ECE98 = "---------- 表彰台初期化\n";
// map_number = %d -> 20 Rewriting.
char* D_800ECEB4 = "map_number = %d - > 20 書き換え中。\n";
// OGA Recognition move begins
char* D_800ECEDC = "OGA 表彰 move 開始\n";
// I called the display of the 4th place person.
char* D_800ECEF0 = "４位の人の表示をコールしました。\n";
// Arrive at the podium
char* D_800ECF14 = "表彰台に到着\n";
// Everyone gather!
char* D_800ECF24 = "全員集合！\n";
// Arrive on the road
char* D_800ECF30 = "道路に到着\n";
// 4th place finished
char* D_800ECF3C = "４位の人終了\n";
// OGA commendation move end
char* D_800ECF4C = "OGA 表彰 move 終了\n";
char* D_800ECF60 = "OGAWA DEBUG DRAW\n";

s16 get_angle_between_path(Vec3f arg0, Vec3f arg1) {
    s16 temp_ret;
    s16 phi_v1;

    temp_ret = get_angle_between_two_vectors(arg0, arg1);
    phi_v1 = temp_ret;
    if (gIsMirrorMode != 0) {
        phi_v1 = -temp_ret;
    }
    return phi_v1;
}

s32 is_collide_with_vehicle(f32 vehicleX, f32 vehicleZ, f32 vehicleVelocityX, f32 vehicleVelocityZ, f32 distanceX,
                            f32 distanceY, f32 playerX, f32 playerZ) {
    f32 velocity;
    f32 temp_f18;

    velocity = sqrtf((vehicleVelocityX * vehicleVelocityX) + (vehicleVelocityZ * vehicleVelocityZ));
    if (velocity < 0.01f) {
        return 0;
    }
    temp_f18 =
        ((vehicleVelocityX / velocity) * (playerX - vehicleX)) + ((vehicleVelocityZ / velocity) * (playerZ - vehicleZ));
    if ((-distanceX < temp_f18) && (temp_f18 < distanceX)) {
        temp_f18 = ((vehicleVelocityZ / velocity) * (playerX - vehicleX)) +
                   (-(vehicleVelocityX / velocity) * (playerZ - vehicleZ));
        if ((-distanceY < temp_f18) && (temp_f18 < distanceY)) {
            return 1;
        }
    }
    return 0;
}

void adjust_position_by_angle(Vec3f newPos, Vec3f oldPos, s16 orientationY) {
    f32 x_dist;
    f32 z_dist;
    f32 temp1;
    f32 temp2;
    f32 sine;
    f32 cosine;

    if (gIsMirrorMode != 0) {
        orientationY = -orientationY;
    }
    x_dist = newPos[0] - oldPos[0];
    z_dist = newPos[2] - oldPos[2];
    sine = sins(orientationY);
    cosine = coss(orientationY);
    temp1 = ((x_dist * cosine) + (z_dist * sine));
    temp2 = ((z_dist * cosine) - (x_dist * sine));
    newPos[0] = oldPos[0] + temp1;
    newPos[2] = oldPos[2] + temp2;
}

s32 set_vehicle_render_distance_flags(Vec3f vehiclePos, f32 renderDistance, s32 flags) {
    Camera* camera;
    Player* player;
    f32 x;
    f32 z;
    f32 playerX;
    f32 playerZ;
    s32 i;
    s32 flag;
    s8 numScreens;

    x = vehiclePos[0];
    z = vehiclePos[2];
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            numScreens = 1;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            numScreens = 2;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            numScreens = gPlayerCount;
            break;
        default:
            numScreens = 1;
            break;
    }
    flag = flags;
    if (!gDemoMode) {
        player = gPlayerOne;
        // Checks distance from each player.
        for (i = 0; i < gPlayerCount; i++, player++) {
            if (((player->type & PLAYER_HUMAN) != 0) && ((player->type & PLAYER_CPU) == 0)) {
                playerX = player->pos[0];
                playerZ = player->pos[2];

                // Is player within render distance
                if (((playerX - renderDistance) < x) && ((playerX + renderDistance) > x) &&
                    ((playerZ - renderDistance) < z) && ((playerZ + renderDistance) > z)) {
                    // Sets the render flag to on for each player.
                    flag |= (RENDER_VEHICLE << i);
                } else {
                    // Sets the render flag to off for each player.
                    flag &= ~(RENDER_VEHICLE << i);
                }
            }
        }
    } else { // Demo cinematic uses the camera to check render distance
        camera = camera1;
        for (i = 0; i < numScreens; i++, camera++) {
            playerX = camera->pos[0];
            playerZ = camera->pos[2];
            if (((playerX - renderDistance) < x) && (x < (playerX + renderDistance)) &&
                ((playerZ - renderDistance) < z) && (z < (playerZ + renderDistance))) {
                flag |= (RENDER_VEHICLE << i);
            } else {
                flag &= ~(RENDER_VEHICLE << i);
            }
        }
    }
    if (CVarGetInteger("gNoCulling", 0) == 1) {
        flag |= (RENDER_VEHICLE << PLAYER_ONE) | (RENDER_VEHICLE << PLAYER_TWO) | (RENDER_VEHICLE << PLAYER_THREE) |
                (RENDER_VEHICLE << PLAYER_FOUR);
    }
    return flag;
}

void detect_wrong_player_direction(s32 playerId, Player* player) {
    s16 playerAngle;
    s16 rotationDifference;
    s16 pathPointAngle;
    s16 pathIndex;
    u32 pathPoint;

    pathIndex = (s16) gPathIndexByPlayerId[playerId];
    pathPoint = gNearestPathPointByPlayerId[playerId];

    playerAngle = (s16) ((s16) player->rotation[1] / DEGREES_CONVERSION_FACTOR);
    pathPointAngle = (s16) ((s16) gPathExpectedRotation[pathIndex][pathPoint] / DEGREES_CONVERSION_FACTOR);

    rotationDifference = playerAngle - pathPointAngle;

    if (rotationDifference < 0) {
        rotationDifference = -rotationDifference;
    }

    if ((gNumPathPointsTraversed[playerId] < gPreviousLapProgressScore[playerId]) &&
        (rotationDifference >= SEVERE_WRONG_DIRECTION_MIN) && (rotationDifference < SEVERE_WRONG_DIRECTION_MAX)) {
        gWrongDirectionCounter[playerId]++;
        if ((gWrongDirectionCounter[playerId]) >= WRONG_DIRECTION_FRAMES_LIMIT) {
            gIsPlayerWrongDirection[playerId] = 1;
            gWrongDirectionCounter[playerId] = WRONG_DIRECTION_FRAMES_LIMIT;
            gPlayers[playerId].effects |= REVERSE_EFFECT;
        }
    } else if ((rotationDifference < SEVERE_CORRECT_DIRECTION_MIN) ||
               (rotationDifference >= SEVERE_CORRECT_DIRECTION_MAX)) {
        gIsPlayerWrongDirection[playerId] = 0;
        gWrongDirectionCounter[playerId] = 0;
        gPlayers[playerId].effects &= ~REVERSE_EFFECT;
    }
    gPreviousLapProgressScore[playerId] = gNumPathPointsTraversed[playerId];
}

void set_places(void) {
    s32 temp_s2;
    f32 temp_f0;
    s32 sp80[8];
    s32 a_really_cool_variable_name;
    UNUSED s32 pad;
    s32 var_t4;
    s32 playerId;
    s32 temp_a0;
    s32 var_t1_3;

    switch (gModeSelection) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            var_t4 = NUM_PLAYERS;
            break;
        case VERSUS:
            var_t4 = gPlayerCount;
            break;
    }

    if (D_8016348C == 0) {
        for (playerId = 0; playerId < var_t4; playerId++) {
            temp_a0 = gGPCurrentRacePlayerIdByRank[playerId];
            sp80[playerId] = temp_a0;
            gCourseCompletionPercentByRank[playerId] = gCourseCompletionPercentByPlayerId[temp_a0];
        }
    } else {
        for (playerId = 0; playerId < var_t4; playerId++) {
            temp_a0 = gGPCurrentRacePlayerIdByRank[playerId];
            sp80[playerId] = temp_a0;
            gCourseCompletionPercentByRank[playerId] = -gTimePlayerLastTouchedFinishLine[temp_a0];
        }
    }

    for (playerId = 0; playerId < var_t4 - 1; playerId++) {
        if ((gPlayers[gGPCurrentRacePlayerIdByRank[playerId]].type & 0x800)) {
            continue;
        }

        for (var_t1_3 = playerId + 1; var_t1_3 < var_t4; var_t1_3++) {
            if (gCourseCompletionPercentByRank[playerId] < gCourseCompletionPercentByRank[var_t1_3]) {
                if (!(gPlayers[gGPCurrentRacePlayerIdByRank[var_t1_3]].type & 0x800)) {
                    temp_s2 = sp80[playerId];
                    sp80[playerId] = sp80[var_t1_3];
                    sp80[var_t1_3] = temp_s2;
                    temp_f0 = gCourseCompletionPercentByRank[playerId];
                    gCourseCompletionPercentByRank[playerId] = gCourseCompletionPercentByRank[var_t1_3];
                    gCourseCompletionPercentByRank[var_t1_3] = temp_f0;
                }
            }
        }
    }

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++) {
        gPreviousGPCurrentRaceRankByPlayerId[playerId] = gGPCurrentRaceRankByPlayerId[playerId];
    }

    for (playerId = 0; playerId < var_t4; playerId++) {
        gGPCurrentRacePlayerIdByRank[playerId] = sp80[playerId];
        gGPCurrentRaceRankByPlayerId[sp80[playerId]] = playerId;
    }

    for (playerId = 0; playerId < var_t4; playerId++) {
        a_really_cool_variable_name = D_80164378[playerId];
        sp80[playerId] = a_really_cool_variable_name;
        gCourseCompletionPercentByRank[playerId] = gCourseCompletionPercentByPlayerId[a_really_cool_variable_name];
    }

    for (playerId = 0; playerId < var_t4 - 1; playerId++) {
        for (var_t1_3 = playerId + 1; var_t1_3 < var_t4; var_t1_3++) {
            if (gCourseCompletionPercentByRank[playerId] < gCourseCompletionPercentByRank[var_t1_3]) {
                temp_s2 = sp80[playerId];
                sp80[playerId] = sp80[var_t1_3];
                sp80[var_t1_3] = temp_s2;
                temp_f0 = gCourseCompletionPercentByRank[playerId];
                gCourseCompletionPercentByRank[playerId] = gCourseCompletionPercentByRank[var_t1_3];
                gCourseCompletionPercentByRank[var_t1_3] = temp_f0;
            }
        }
    }

    for (playerId = 0; playerId < var_t4; playerId++) {
        gGPCurrentRaceRankByPlayerIdDup[sp80[playerId]] = playerId;
        D_80164378[playerId] = sp80[playerId];
    }
}

void update_player_rankings(void) {
    f32 temp_f0;
    UNUSED s32 pad;
    s32 sp50[8];
    s32 temp_a0;
    s32 temp_t2_2;
    s32 var_a1_2;
    s32 var_a3;
    s32 var_a2;

    switch (gModeSelection) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            var_a2 = 8;
            break;
        case VERSUS:
            var_a2 = gPlayerCount;
            break;
    }

    for (var_a3 = 0; var_a3 < var_a2; var_a3++) {
        temp_a0 = gGPCurrentRacePlayerIdByRank[var_a3];
        sp50[var_a3] = temp_a0;
        gCourseCompletionPercentByRank[var_a3] = -gTimePlayerLastTouchedFinishLine[temp_a0];
    }

    for (var_a3 = 0; var_a3 < (var_a2 - 1); var_a3++) {
        for (var_a1_2 = var_a3 + 1; var_a1_2 < var_a2; var_a1_2++) {
            if (gCourseCompletionPercentByRank[var_a3] < gCourseCompletionPercentByRank[var_a1_2]) {
                temp_t2_2 = sp50[var_a3];
                sp50[var_a3] = sp50[var_a1_2];
                sp50[var_a1_2] = temp_t2_2;
                temp_f0 = gCourseCompletionPercentByRank[var_a3];
                gCourseCompletionPercentByRank[var_a3] = gCourseCompletionPercentByRank[var_a1_2];
                gCourseCompletionPercentByRank[var_a1_2] = temp_f0;
            }
        }
    }

    for (var_a3 = 0; var_a3 < NUM_PLAYERS; var_a3++) {
        gPreviousGPCurrentRaceRankByPlayerId[var_a3] = gGPCurrentRaceRankByPlayerId[var_a3];
    }

    for (var_a3 = 0; var_a3 < var_a2; var_a3++) {
        gGPCurrentRaceRankByPlayerId[sp50[var_a3]] = var_a3;
        gGPCurrentRacePlayerIdByRank[var_a3] = sp50[var_a3];
    }
}

void set_places_end_course_with_time(void) {
    f32 temp_a0;
    s32 temp;
    s32 sp68[8];
    UNUSED s32 pad;
    s32 temp_t1;
    s32 i;
    s32 j;
    s32 this_loops_upper_bound_is_brough_to_you_by_the_number = 8;

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number;) {
        gCourseCompletionPercentByRank[i++] = 0.0f;
    }

    for (j = 0, i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (gPlayers[i].type & 0x800) {
            sp68[j] = i;
            gCourseCompletionPercentByRank[j] = -gTimePlayerLastTouchedFinishLine[i];
            j++;
        }
    }

    temp_t1 = j;
    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (!(gPlayers[i].type & 0x800)) {
            sp68[j] = i;
            gCourseCompletionPercentByRank[j] = gCourseCompletionPercentByPlayerId[i];
            j++;
        }
    }

    for (i = 0; i < (temp_t1 - 1); i++) {
        for (j = i + 1; j < temp_t1; j++) {
            if (gCourseCompletionPercentByRank[i] < gCourseCompletionPercentByRank[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = gCourseCompletionPercentByRank[i];
                gCourseCompletionPercentByRank[i] = gCourseCompletionPercentByRank[j];
                gCourseCompletionPercentByRank[j] = temp_a0;
            }
        }
    }

    for (i = temp_t1; i < (this_loops_upper_bound_is_brough_to_you_by_the_number - 1); i++) {
        for (j = i + 1; j < this_loops_upper_bound_is_brough_to_you_by_the_number; j++) {
            if (gCourseCompletionPercentByRank[i] < gCourseCompletionPercentByRank[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = gCourseCompletionPercentByRank[i];
                gCourseCompletionPercentByRank[i] = gCourseCompletionPercentByRank[j];
                gCourseCompletionPercentByRank[j] = temp_a0;
            }
        }
    }

    for (i = 0; i < 8; i++) {
        gPreviousGPCurrentRaceRankByPlayerId[i] = gGPCurrentRaceRankByPlayerId[i];
    }

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        gGPCurrentRaceRankByPlayerId[sp68[i]] = i;
        gGPCurrentRacePlayerIdByRank[i] = sp68[i];
    }
}

/**
 * Checks if a path point is within a valid range of another path point, accounting for track wrapping
 *
 * @param pathPoint The path point to check
 * @param currentPathPoint The reference path point
 * @param backwardRange Number of path to look behind
 * @param forwardRange Number of path to look ahead
 * @param totalPathPoints Total number of path in the track
 * @return
 *   1: path point is within normal range
 *  -1: path point is within wrapped range
 *   2: path point is out of range
 *   0: invalid range parameters
 */
s32 is_path_point_in_range(u16 pathPoint, u16 currentPathPoint, u16 backwardRange, u16 forwardRange,
                           u16 totalPathPoints) {
    s32 var_v1;

    var_v1 = 0;
    if ((currentPathPoint >= backwardRange) && (currentPathPoint < (totalPathPoints - forwardRange))) {
        if ((pathPoint >= (currentPathPoint - backwardRange)) && ((currentPathPoint + forwardRange) >= pathPoint)) {
            var_v1 = 1;
        }
    } else if ((((currentPathPoint + forwardRange) % totalPathPoints) < pathPoint) &&
               ((((currentPathPoint + totalPathPoints) - backwardRange) % totalPathPoints) >= pathPoint)) {
        var_v1 = -1;
    } else {
        var_v1 = 2;
    }
    return var_v1;
}

void func_80007D04(s32 playerId, Player* player) {
    s16 temp_t1;
    s16 temp_t2;
    s32 var_v0;

    temp_t1 = gNumPathPointsTraversed[gBestRankedHumanPlayer];
    temp_t2 = gNumPathPointsTraversed[playerId];

    if (gGPCurrentRaceRankByPlayerId[playerId] < 2) {
        s16 val1 = gGPCurrentRaceRankByPlayerId[gBestRankedHumanPlayer];
        s16 val2 = temp_t2 - temp_t1;

        if (val2 > 400 && val1 >= 6) {
            player->effects &= ~0x200000;
            player_accelerate_alternative(player);
            D_801634C0[playerId] = 4;
            return;
        }
    } else {
        player->effects |= 0x200000;
        player_accelerate_alternative(player);
        D_801634C0[playerId] = 3;
        return;
    }

    switch (gCCSelection) { // WTF, FAKE ?
        case 3:
            break;
    }

    switch (gCCSelection) {
        case 0:
            var_v0 = 0;
            if (playerId == D_80163344[0]) {
                var_v0 = 0x14;
            }
            break;

        case 1:
            var_v0 = 8;
            if (playerId == D_80163344[0]) {
                var_v0 = 0x18;
            }
            break;

        case 2:
            var_v0 = 0x12;
            if (playerId == D_80163344[0]) {
                var_v0 = 0x24;
            }
            break;

        case 3:
            var_v0 = 8;
            if (playerId == D_80163344[0]) {
                var_v0 = 0x18;
            }
            break;

        default:
            var_v0 = 0;
            break;
    }

    if (temp_t2 < temp_t1) {
        player->effects |= 0x200000;
        player_accelerate_alternative(player);
        D_801634C0[playerId] = 1;
    } else if (temp_t2 < (temp_t1 + var_v0 + 0x32)) {
        player->effects &= ~0x200000;
        player_accelerate_alternative(player);
        D_801634C0[playerId] = 3;
    } else if (D_801631E0[playerId] == 0) {
        player->effects &= ~0x200000;
        player_accelerate_alternative(player);
        D_801634C0[playerId] = 2;
    } else {
        player->effects &= ~0x200000;
        player_decelerate_alternative(player, 1.0f);
        D_801634C0[playerId] = -1;
    }
}

void func_80007FA4(s32 arg0, Player* player, f32 arg2) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;
    s32 test;

    temp_f0 = D_80163418[arg0] - player->pos[0];
    temp_f2 = D_80163438[arg0] - player->pos[2];
    temp_f12 = (temp_f0 * temp_f0) + (temp_f2 * temp_f2);
    if (arg0 == 3) {
        if ((temp_f12 < 25.0f) && (D_80163410[arg0] < 5)) {
            D_80163410[arg0] = 4;
            (arg2 < ((2.0 * 18.0) / 216.0)) ? func_80038BE4(player, 1) : player_decelerate_alternative(player, 1.0f);
        } else if ((temp_f12 < 3600.0f) && (D_80163410[arg0] < 4)) {
            D_80163410[arg0] = 3;
            (arg2 < ((5.0 * 18.0) / 216.0)) ? func_80038BE4(player, 1) : player_decelerate_alternative(player, 5.0f);
        } else {
            (arg2 < ((20.0 * 18.0) / 216.0)) ? func_80038BE4(player, 10) : player_decelerate_alternative(player, 1.0f);
        }
    } else {
        if ((temp_f12 < 25.0f) && (D_80163410[arg0] < 5)) {
            D_80163410[arg0] = 4;
            test = 2;
            (arg2 < ((test * 18.0) / 216.0)) ? func_80038BE4(player, 1) : player_decelerate_alternative(player, 1.0f);
        } else if ((temp_f12 < 4900.0f) && (D_80163410[arg0] < 4)) {
            D_80163410[arg0] = 3;
            test = 5;
            (arg2 < ((test * 18.0) / 216.0)) ? func_80038BE4(player, 1) : player_decelerate_alternative(player, 15.0f);
        } else if ((temp_f12 < 22500.0f) && (D_80163410[arg0] < 3)) {
            D_80163410[arg0] = 2;
            test = 20;
            (arg2 < ((test * 18.0) / 216.0)) ? func_80038BE4(player, 5) : player_decelerate_alternative(player, 1.0f);
        } else if ((temp_f12 < 90000.0f) && (D_80163410[arg0] < 2)) {
            D_80163410[arg0] = 1;
            test = 30;
            (arg2 < ((test * 18.0) / 216.0)) ? func_80038BE4(player, 6) : player_decelerate_alternative(player, 1.0f);
        } else if (D_80163410[arg0] == 0) {
            test = 35;
            (arg2 < (((test ^ 0) * 18.0) / 216.0)) ? func_80038BE4(player, 2)
                                                   : player_decelerate_alternative(player, 1.0f);
        } else {
            player_decelerate_alternative(player, 1.0f);
        }
    }
}

void regulate_cpu_speed(s32 playerId, f32 targetSpeed, Player* player) {
    f32 var_f2;
    f32 var_f0;
    UNUSED s32 thing;
    s32 var_a1;

    var_f2 = player->speed;
    if (!(player->effects & 0x80) && !(player->effects & 0x40) && !(player->effects & 0x20000) &&
        !(player->soundEffects & 0x400000) && !(player->soundEffects & 0x01000000) && !(player->soundEffects & 2) &&
        !(player->soundEffects & 4)) {
        if (IsPodiumCeremony()) {
            func_80007FA4(playerId, player, var_f2);
        } else if ((bStopAICrossing[playerId] == 1) && !(player->effects & (STAR_EFFECT | BOO_EFFECT))) {
            player_decelerate_alternative(player, 10.0f);
            if (player->currentSpeed == 0.0) {
                player->velocity[0] = 0.0f;
                player->velocity[2] = 0.0f;
            }
        } else {
            var_f0 = 3.3333333f;
            switch (gCCSelection) { /* irregular */
                case 1:
                case 3:
                    break;
                case 0:
                    var_f0 = 2.5f;
                    break;
                case 2:
                    var_f0 = 3.75f;
                    break;
            }
            if (var_f2 < var_f0) {
                player->effects &= ~0x00200000;
                player_accelerate_alternative(player);
            } else if (player->type & 0x800) {
                if (var_f2 < targetSpeed) {
                    player->effects &= ~0x00200000;
                    player_accelerate_alternative(player);
                } else {
                    player->effects &= ~0x00200000;
                    player_decelerate_alternative(player, 1.0f);
                }
            } else if ((D_801631E0[playerId] == 1) && (D_80163330[playerId] != 1)) {
                if (func_800088D8(playerId, gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerIdDup[playerId]) ==
                    1) {
                    player->effects |= 0x200000;
                    player_accelerate_alternative(player);
                } else {
                    player->effects &= ~0x00200000;
                    player_decelerate_alternative(player, 1.0f);
                }
            } else {
                var_a1 = 1;
                switch (gSpeedCPUBehaviour[playerId]) { /* switch 1; irregular */
                    case SPEED_CPU_BEHAVIOUR_FAST:      /* switch 1 */
                        player->effects &= ~0x00200000;
                        player_accelerate_alternative(player);
                        break;
                    case SPEED_CPU_BEHAVIOUR_MAX: /* switch 1 */
                        player->effects |= 0x200000;
                        player_accelerate_alternative(player);
                        break;
                    case SPEED_CPU_BEHAVIOUR_SLOW: /* switch 1 */
                        if (((var_f2 / 18.0f) * 216.0f) > 20.0f) {
                            targetSpeed = 1.6666666f;
                        }
                        var_a1 = 0;
                        break;
                    case SPEED_CPU_BEHAVIOUR_NORMAL: /* switch 1 */
                    default:                         /* switch 1 */
                        var_a1 = 0;
                        break;
                }
                if (var_a1 != 1) {
                    if (var_f2 < targetSpeed) {
                        if ((gDemoMode == 1) && (!IsPodiumCeremony())) {
                            player_accelerate_alternative(player);
                        } else if (D_80163330[playerId] == 1) {
                            func_80007D04(playerId, player);
                        } else if (func_800088D8(playerId, gLapCountByPlayerId[playerId],
                                                 gGPCurrentRaceRankByPlayerIdDup[playerId]) == 1) {
                            player->effects |= 0x200000;
                            player_accelerate_alternative(player);
                        } else {
                            player->effects &= ~0x00200000;
                            player_decelerate_alternative(player, 1.0f);
                        }
                    } else {
                        player->effects &= ~0x00200000;
                        if (targetSpeed > 1.0f) {
                            player_decelerate_alternative(player, 2.0f);
                        } else {
                            player_decelerate_alternative(player, 5.0f);
                        }
                    }
                }
            }
        }
    }
}

bool func_800088D8(s32 playerId, s16 lapNum, s16 arg2) {
    Player* player;
    s16* temp_a3;
    s32 progress;
    f32 interp;
    s16 var_t1;
    u16* var_a0_3;
    s16 temp;
    s16 temp2;
    s32 i;
    s32 var_a0;
    s16 var_v0;
    s16 var_a0_4;
    s32* var_v1;
    s16 STEMP_V1;
    s16 STEMP_V0;
    s16 rank;

    D_80163128[playerId] = -1;
    D_80163150[playerId] = -1;
    if (gModeSelection == TIME_TRIALS) {
        return true;
    }
    if (lapNum < 0) {
        return true;
    }
    if (lapNum >= 4) {
        lapNum = 3;
    }
    if (D_80163330[playerId] == 1) {
        return true;
    }
    player = &gPlayers[playerId];
    if (player->type & 0x4000) {
        return true;
    }

    temp_a3 = &CM_GetProps()->AIDistance[lapNum * 8];
    if (arg2 == 0) {
        if (gDemoMode == 1) {
            STEMP_V0 = gNumPathPointsTraversed[playerId];
            STEMP_V1 = gNumPathPointsTraversed[D_80164378[7]];
            progress = STEMP_V0 - STEMP_V1;
            if (progress < 0) {
                progress = -progress;
            }
            if (lapNum < 3) {
                STEMP_V0 = temp_a3[0];
                STEMP_V1 = temp_a3[8];
                interp = gLapCompletionPercentByPlayerId[playerId];
                var_a0 = (STEMP_V1 * interp) + (STEMP_V0 * (1.0f - interp));
            } else {
                var_a0 = temp_a3[0];
            }
            D_80163150[playerId] = var_a0;
            if ((D_80163150[playerId] < progress) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
                return false;
            }
            return true;
        }

        STEMP_V0 = gNumPathPointsTraversed[playerId];
        STEMP_V1 = gNumPathPointsTraversed[gBestRankedHumanPlayer];
        progress = STEMP_V0 - STEMP_V1;
        rank = gGPCurrentRaceRankByPlayerId[gBestRankedHumanPlayer];
        if (((((gPathCountByPathIndex[0] * 2) / 3)) < progress) && ((rank) >= 6)) {
            STEMP_V0 = gNumPathPointsTraversed[playerId];
            STEMP_V1 = temp = gNumPathPointsTraversed[D_80164378[rank - 1]];
            progress = STEMP_V0 - STEMP_V1;
        }
        if (progress < 0) {
            progress = -progress;
        }
        if (lapNum < 3) {
            STEMP_V0 = temp_a3[0];
            STEMP_V1 = temp_a3[8];
            interp = gLapCompletionPercentByPlayerId[playerId];
            var_a0 = (STEMP_V1 * interp) + (STEMP_V0 * (1.0f - interp));
        } else {
            var_a0 = temp_a3[0];
        }
        D_80163150[playerId] = var_a0 = (gCCSelection + 1) * var_a0;
        if ((var_a0 < progress) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            return false;
        }
        return true;
    }

    temp2 = D_80163344[0];
    STEMP_V0 = gNumPathPointsTraversed[playerId];
    STEMP_V1 = gNumPathPointsTraversed[temp2];
    progress = STEMP_V1 - STEMP_V0;
    if (progress < 0) {
        progress = -progress;
    }

    var_v0 = 0;
    for (i = 0; i < 2; i++) {
        if (gGPCurrentRaceRankByPlayerId[D_80163344[i] & 0xFFFF] < arg2) {
            var_v0++;
        }
    }

    var_a0_4 = 0;
    for (i = 0; i < gPlayerCount; i++) {
        if (gGPCurrentRaceRankByPlayerId[i] < arg2) {
            var_a0_4++;
        }
    }

    // FAKE
    var_t1 = (arg2 - (var_v0 & 0xFFFF)) - var_a0_4;
    arg2 -= var_v0;

    if ((var_v0 > 0) || (var_a0_4 > 0)) {
        var_t1++;
    }
    D_80164538[playerId] = var_t1;
    if ((var_t1 < 0) || (var_t1 >= 8)) {
        return false;
    }
    if (lapNum < 3) {
        STEMP_V0 = temp_a3[var_t1 + 0];
        STEMP_V1 = temp_a3[var_t1 + 8];
        interp = gLapCompletionPercentByPlayerId[playerId];
        var_a0 = (STEMP_V1 * interp) + (STEMP_V0 * (1.0f - interp));
    } else {
        var_a0 = temp_a3[var_t1];
    }
    D_80163128[playerId] = progress;
    D_80163150[playerId] = var_a0;
    if (var_a0 < progress) {
        return true;
    }
    return false;
}

void set_current_path(s32 pathIndex) {
    gCurrentTrackPath = gTrackPaths[pathIndex];
    gCurrentTrackLeftPath = gTrackLeftPaths[pathIndex];
    gCurrentTrackRightPath = gTrackRightPaths[pathIndex];
    gCurrentTrackSectionTypesPath = gTrackSectionTypes[pathIndex];
    gCurrentPathPointExpectedRotationPath = gPathExpectedRotation[pathIndex];
    gCurrentTrackConsecutiveCurveCountsPath = gTrackConsecutiveCurveCounts[pathIndex];
    gSelectedPathCount = gPathCountByPathIndex[pathIndex];
}

s32 update_player_path_selection(s32 payerId, s32 pathIndex) {
    f32 posX;
    f32 posY;
    f32 posZ;
    Player* player;
    s32 trackSectionId;
    UNUSED s32 stackPadding;

    player = &gPlayers[payerId];
    posX = player->pos[0];
    posY = player->pos[1];
    posZ = player->pos[2];
    stackPadding = pathIndex;
    trackSectionId = get_track_section_id(player->collision.meshIndexZX);
    gPlayersTrackSectionId[payerId] = trackSectionId;
    sSomeNearestPathPoint = find_closest_path_point_track_section(posX, posY, posZ, trackSectionId, &pathIndex);
    gNearestPathPointByPlayerId[payerId] = sSomeNearestPathPoint;
    if (pathIndex) {};
    gPathIndexByPlayerId[payerId] = pathIndex;
    set_current_path(pathIndex);
    if (stackPadding) {};
    return pathIndex;
}

void update_player_completion(s32 playerId) {
    f32 temp_f0;

    gNumPathPointsTraversed[playerId] =
        (gLapCountByPlayerId[playerId] * gPathCountByPathIndex[0]) + sSomeNearestPathPoint;
    temp_f0 = (f32) gNearestPathPointByPlayerId[playerId] / (f32) gPathCountByPathIndex[gPathIndexByPlayerId[playerId]];
    gLapCompletionPercentByPlayerId[playerId] = temp_f0;
    gCourseCompletionPercentByPlayerId[playerId] = temp_f0;
    gCourseCompletionPercentByPlayerId[playerId] += gLapCountByPlayerId[playerId];
}

void yoshi_valley_cpu_path(s32 playerId) {
    s16 temp_v0;

    temp_v0 = bInMultiPathSection[playerId];
    if (sSomeNearestPathPoint >= 0x6D) {
        bInMultiPathSection[playerId] = 1;
        switch (gPlayerPathIndex) {
            case 0:
                if (sSomeNearestPathPoint >= 0x20F) {
                    bInMultiPathSection[playerId] = 0;
                }
                break;
            case 1:
                if (sSomeNearestPathPoint >= 0x206) {
                    bInMultiPathSection[playerId] = 0;
                }
                break;
            case 2:
                if (sSomeNearestPathPoint >= 0x211) {
                    bInMultiPathSection[playerId] = 0;
                }
                break;
            case 3:
                if (sSomeNearestPathPoint >= 0x283) {
                    bInMultiPathSection[playerId] = 0;
                }
                break;
        }
    }
    if ((temp_v0 == 0) && (bInMultiPathSection[playerId] == 1)) {
        cpu_enteringPathIntersection[playerId] = 1;
    }
    if ((temp_v0 == 1) && (bInMultiPathSection[playerId] == 0)) {
        cpu_exitingPathIntersection[playerId] = 1;
    }
}

void update_cpu_path_completion(s32 playerId, Player* player) {
    f32 posX;
    f32 posY;
    f32 posZ;
    UNUSED f32 pad[2];

    posX = player->pos[0];
    posY = player->pos[1];
    posZ = player->pos[2];
    if (cpu_enteringPathIntersection[playerId] == 1) {
        gPlayerPathIndex = update_player_path_selection(playerId, random_int(4U));
        sSomeNearestPathPoint = update_player_path(posX, posY, posZ, 0, player, playerId, gPlayerPathIndex);
        gNearestPathPointByPlayerId[playerId] = sSomeNearestPathPoint;
        update_player_completion(playerId);
        cpu_enteringPathIntersection[playerId] = 0;
    }
    if (cpu_exitingPathIntersection[playerId] == 1) {
        gPlayerPathIndex = update_player_path_selection(playerId, 0);
        sSomeNearestPathPoint = update_player_path(posX, posY, posZ, 0, player, playerId, gPlayerPathIndex);
        gNearestPathPointByPlayerId[playerId] = sSomeNearestPathPoint;
        update_player_completion(playerId);
        cpu_exitingPathIntersection[playerId] = 0;
    }
}

/**
 * Helps calculate time since player last touched finishline.
 **/
f32 func_80009258(UNUSED s32 playerId, f32 arg1, f32 arg2) {
    f32 temp_f2 = gPathStartZ - arg2;
    f32 temp_f12 = arg1 - gPathStartZ;
    return gCourseTimer - ((COURSE_TIMER_ITER_f * temp_f2) / (temp_f2 + temp_f12));
}

void update_player_path_completion(s32 playerId, Player* player) {
    f32 playerX;
    f32 playerY;
    f32 playerZ;
    s32 var_v0;
    UNUSED s16 pad;
    f32 previousPlayerZ;

    playerX = player->pos[0];
    playerY = player->pos[1];
    playerZ = player->pos[2];
    previousPlayerZ = gPreviousPlayerZ[playerId];
    gIsPlayerNewPathPoint = false;
    D_80163240[playerId] = 0;
    sSomeNearestPathPoint = update_player_path(playerX, playerY, playerZ, gNearestPathPointByPlayerId[playerId], player,
                                               playerId, gPlayerPathIndex);
    gCurrentNearestPathPoint = sSomeNearestPathPoint;
    if (gNearestPathPointByPlayerId[playerId] != sSomeNearestPathPoint) {
        gNearestPathPointByPlayerId[playerId] = sSomeNearestPathPoint;
        gIsPlayerNewPathPoint = true;
        update_player_completion(playerId);
    }
    if (IsPodiumCeremony()) {
        update_player_position_factor(playerId, sSomeNearestPathPoint, gPlayerPathIndex);
        return;
    }
    if ((sSomeNearestPathPoint < 20) || ((gPathCountByPathIndex[gPlayerPathIndex] - 20) < sSomeNearestPathPoint) ||
        (IsKalimariDesert())) {
        s16 var_v1 = 0;
        s16 var_t0 = 0;
        if (IsKalimariDesert()) {
            D_801634EC = 0;
            if (player->effects & 0x200) {
                D_801634EC = 1;
            }
            if (gIsMirrorMode != 0) {
                if (playerX < 300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else if ((playerX < 1300.0f) && (gLapCountByPlayerId[playerId] < 2) && ((D_801634EC == 1))) {
                    var_v1 = 1;
                    var_t0 = 1;
                }
            } else {
                if (playerX > -300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else {
                    if ((playerX > -1300.0f) && (gLapCountByPlayerId[playerId] < 2) && (D_801634EC == 1)) {
                        var_v1 = 1;
                        var_t0 = 1;
                    }
                }
            }
        } else {
            var_v1 = 1;
            var_t0 = 1;
        }
        previousPlayerZ = gPreviousPlayerZ[playerId];
        if ((var_v1 != 0) && (playerZ <= gPathStartZ)) {
            if (gPathStartZ < previousPlayerZ) {
                gLapCountByPlayerId[playerId]++;
                if ((gModeSelection == GRAND_PRIX) && (gLapCountByPlayerId[playerId] == 5)) {
                    if (gGPCurrentRaceRankByPlayerIdDup[playerId] == 7) {
                        // clang-format off
                        for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) { gLapCountByPlayerId[var_v0]--; } // has to be one line to match
                        // clang-format on
                    }
                }
                D_80163240[playerId] = 1;
                update_player_completion(playerId);
                reset_cpu_behaviour(playerId);
                cpu_ItemStrategy[playerId].numItemUse = 0;
                if ((D_8016348C == 0) && !(player->type & PLAYER_CINEMATIC_MODE)) {
                    gTimePlayerLastTouchedFinishLine[playerId] = func_80009258(playerId, previousPlayerZ, playerZ);
                }
            }
        }
        if ((var_t0 != 0) && (previousPlayerZ <= gPathStartZ) && (gPathStartZ < playerZ)) {
            gLapCountByPlayerId[playerId]--;
            update_player_completion(playerId);
        }
    }
    gPreviousPlayerZ[playerId] = playerZ;
    if ((IsYoshiValley()) && (gIsPlayerNewPathPoint == true)) {
        yoshi_valley_cpu_path(playerId);
        if (((player->type & PLAYER_HUMAN) == 0) || (player->type & PLAYER_CPU)) {
            update_cpu_path_completion(playerId, player);
        }
    }
    if ((player->type & PLAYER_HUMAN) && !(player->type & PLAYER_CPU)) {
        detect_wrong_player_direction(playerId, player);
        if ((gModeSelection == GRAND_PRIX) && (gPlayerCount == 2) && (playerId == 0)) {
            if (gGPCurrentRaceRankByPlayerIdDup[PLAYER_ONE] < gGPCurrentRaceRankByPlayerIdDup[PLAYER_TWO]) {
                gBestRankedHumanPlayer = PLAYER_ONE;
            } else {
                gBestRankedHumanPlayer = PLAYER_TWO;
            }
        }
    } else {
        //????
    }
    update_player_position_factor(playerId, sSomeNearestPathPoint, gPlayerPathIndex);
}

void update_vehicles(void) {
    s32 i;
    generate_player_smoke();
    D_8016337C++;

    // CM_TickBombKarts();
    // CM_VehiclesTick();
}

void play_cpu_sound_effect(s32 arg0, Player* player) {
    if (D_80163398[arg0] >= 0xB) {
        if ((player->effects & 0x80) || (player->effects & 0x40) || (player->effects & 0x20000)) {
            func_800C92CC(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0A));
            D_80163398[arg0] = 0;
        }
    }
    if (D_801633B0[arg0] >= 0xB) {
        if ((player->soundEffects & REVERSE_EFFECT) || (player->soundEffects & 0x01000000) ||
            (player->soundEffects & 2) || (player->soundEffects & 4) || (player->effects & HIT_EFFECT)) {
            func_800C92CC(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0B));
            D_801633B0[arg0] = 0;
        }
    }
}

void update_player_timer_sound(s32 playerId, UNUSED Player* unused) {
    s32 var_s0;

    if (D_801633C8[playerId] >= 0x65) {
        for (var_s0 = 0; var_s0 < gPlayerCount; var_s0++) {
            if ((gGPCurrentRaceRankByPlayerId[playerId] < gGPCurrentRaceRankByPlayerId[var_s0]) &&
                (gGPCurrentRaceRankByPlayerId[playerId] == gPreviousGPCurrentRaceRankByPlayerId[var_s0]) &&
                (gGPCurrentRaceRankByPlayerId[var_s0] == gPreviousGPCurrentRaceRankByPlayerId[playerId])) {
                func_800C92CC(playerId, 0x2900800DU);
                D_801633C8[playerId] = 0;
            }
        }
    }
    if (D_801633C8[playerId] < 0x3E8) {
        D_801633C8[playerId]++;
    }
    if (D_80163398[playerId] < 0xC8) {
        D_80163398[playerId]++;
    }
    if (D_801633B0[playerId] < 0xC8) {
        D_801633B0[playerId]++;
    }
}

void update_player(s32 playerId) {
    UNUSED s32 pad[14];
    s16 var_a0_2;
    s16 newAngle;
    u16 pathIndex;

    f32 distX;
    f32 minAngle;

    s16 angle;
    s16 steeringSensitivity;

    s32 maxAngle;
    Player* player;
    UNUSED s32 pad3[10];
    TrackPathPoint* pathPoint;
    f32 onePointFive = 1.5f;

    player = &gPlayers[playerId];
    if ((s32) CM_GetProps()->AIMaximumSeparation >= 0) {
        D_80163100[playerId] += 1;
        if (playerId == 0) {
            gIncrementUpdatePlayer++;
            if (gIncrementUpdatePlayer & 1) {
                D_80163488 += 1;
            }
        }
        if (!(player->type & PLAYER_EXISTS)) {
            gNumPathPointsTraversed[playerId] = -20;
            gCourseCompletionPercentByPlayerId[playerId] = -1000.0f;
            gLapCompletionPercentByPlayerId[playerId] = -1000.0f;
            return;
        }
        D_801633E0[playerId] = 0;
        // clang-format off
        if (player->pos[0] < gCourseMinX) {            D_801633E0[playerId] = 1;        }
        if (gCourseMaxX < player->pos[0]) {            D_801633E0[playerId] = 2;        }
        if (player->pos[2] < gCourseMinZ) {            D_801633E0[playerId] = 3;        }
        if (gCourseMaxZ < player->pos[2]) {            D_801633E0[playerId] = 4;        }
        // clang-format on

        if (!(player->unk_0CA & 2) && !(player->unk_0CA & 8)) {
            gPlayerPathIndex = gPathIndexByPlayerId[playerId];
            set_current_path(gPlayerPathIndex);

            // Changed to allow spawning vehicles in any track
            // Collision is handled in the vehicles class now

            // switch (gCurrentCourseId) { /* irregular */
            // case COURSE_KALIMARI_DESERT:
            CM_VehicleCollision(playerId, player);
            // handle_trains_interactions(playerId, player);
            if (playerId == 0) {
                CM_CrossingTrigger();
                // func_80013054();
            }
            // break;
            // case COURSE_DK_JUNGLE:
            // handle_paddle_boats_interactions(player);
            // break;
            // case COURSE_TOADS_TURNPIKE:
            // handle_box_trucks_interactions(playerId, player);
            // handle_school_buses_interactions(playerId, player);
            // handle_tanker_trucks_interactions(playerId, player);
            // handle_cars_interactions(playerId, player);
            // break;
            // }
            if (player->type & PLAYER_CINEMATIC_MODE) {
                player->effects &= ~REVERSE_EFFECT;
                player->unk_044 &= ~1;
            }
            update_player_path_completion(playerId, player);
            if ((!IsPodiumCeremony()) && ((D_80163240[playerId] == 1) || (playerId == 0))) {
                set_places();
            }
            if (player->type & PLAYER_CPU) {
                if ((gIsPlayerNewPathPoint == true) && (!IsPodiumCeremony())) {
                    cpu_behaviour(playerId);
                }
                // one update it try to use an item, the other it doesn't
                if ((playerId & 1) != (gIncrementUpdatePlayer & 1)) {
                    cpu_use_item_strategy(playerId);
                }
                update_player_timer_sound(playerId, player);
                D_80162FD0 = 0;
                switch (gModeSelection) { /* switch 1; irregular */
                    case 1:               /* switch 1 */
                    case 2:               /* switch 1 */
                    case 3:               /* switch 1 */
                        break;
                    case 0: /* switch 1 */
                        break;
                }
                D_801631E0[playerId] = false;
                if ((player->effects & UNKNOWN_EFFECT_0x1000) && (!IsPodiumCeremony())) {
                    D_801631E0[playerId] = true;
                }
                if ((D_801646CC == 1) || (player->type & PLAYER_CINEMATIC_MODE) || (IsPodiumCeremony())) {
                    if (!IsToadsTurnpike()) {
                        gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                    }
                    gPlayerTrackPositionFactorInstruction[playerId].unkC = 0.0f;
                }
                if (gPlayerPathIndex > 0) {
                    gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                    gPlayerTrackPositionFactorInstruction[playerId].unkC = 0.0f;
                }
                gPlayerPathY[playerId] =
                    gTrackPaths[gPlayerPathIndex][gNearestPathPointByPlayerId[playerId]].y + 4.3f;
                if ((D_801631F8[playerId] == 1) && (D_801631E0[playerId] == false)) {
                    set_player_height(player);
                }
                if (D_801631E0[playerId] == true) {
                    player->pos[1] = gPlayerPathY[playerId];
                }
                D_801631F8[playerId] = D_801631E0[playerId];

                // Old vehicle draw method was here

                if ((IsYoshiValley()) || (IsPodiumCeremony())) {
                    gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                } else if (IsToadsTurnpike()) {
                    // update_player_track_position_factor_from_box_trucks(playerId);
                    // update_player_track_position_factor_from_buses(playerId);
                    // update_player_track_position_factor_from_tanker_truck(playerId);
                    // update_player_track_position_factor_from_cars(playerId);
                }

                // switch (gCurrentCourseId) {
                //     case COURSE_YOSHI_VALLEY:
                //     case COURSE_AWARD_CEREMONY:
                //         gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                //         break;
                //     default:
                //         break;
                //     case COURSE_TOADS_TURNPIKE:
                //         update_player_track_position_factor_from_box_trucks(playerId);
                //         update_player_track_position_factor_from_buses(playerId);
                //         update_player_track_position_factor_from_tanker_truck(playerId);
                //         update_player_track_position_factor_from_cars(playerId);
                //         break;
                // }

                if (D_801631E0[playerId] == true) {
                    D_801630E8[playerId] = 0;
                    player->effects &= ~0x10;
                    if ((playerId & 1) != (gIncrementUpdatePlayer & 1)) {
                        apply_cpu_turn(player, 0);
                        regulate_cpu_speed(playerId, gPreviousCpuTargetSpeed[playerId], player);
                        return;
                    }
                    if ((gPlayerCount > 0) && (gPlayerCount < 3) && (D_80163330[playerId] == 1) &&
                        (D_8016334C[playerId] < gGPCurrentRaceRankByPlayerId[playerId])) {
                        gPreviousCpuTargetSpeed[playerId] = 8.333333f;
                    } else if (D_80162FD0 == (s16) 1U) {
                        gPreviousCpuTargetSpeed[playerId] = CM_GetProps()->D_0D0096B8[gCCSelection];
                        gPlayerTrackPositionFactorInstruction[playerId].target = -0.5f;
                    } else if (gCurrentTrackConsecutiveCurveCountsPath[sSomeNearestPathPoint] > 0) {
                        gPreviousCpuTargetSpeed[playerId] = CM_GetProps()->CurveTargetSpeed[gCCSelection];
                    } else {
                        gPreviousCpuTargetSpeed[playerId] = CM_GetProps()->NormalTargetSpeed[gCCSelection];
                    }
                    CM_AICrossingBehaviour(playerId);
                    // check_ai_crossing_distance(playerId);

                    cpu_track_position_factor(playerId);
                    determine_ideal_cpu_position_offset(playerId, gCurrentNearestPathPoint);
                    distX = gOffsetPosition[0] - player->pos[0];
                    minAngle = gOffsetPosition[2] - player->pos[2];
                    if (!(player->effects & 0x80) && !(player->effects & 0x40) && !(player->effects & 0x800)) {
                        if (((distX * distX) + (minAngle * minAngle)) > 6400.0f) {
                            if (gPlayerPathIndex == 0) {
                                func_8000B140(playerId);
                                if (D_80162FF8[playerId] > 0) {
                                    pathIndex = gCurrentNearestPathPoint + 5;
                                    pathIndex %= gSelectedPathCount;
                                    set_track_offset_position(pathIndex, D_80163090[playerId], gPlayerPathIndex);
                                }
                            }
                            player->rotation[1] = -get_angle_between_two_vectors(player->pos, gOffsetPosition);
                        } else {
                            player->rotation[1] =
                                gPathExpectedRotation[gPlayerPathIndex]
                                                     [(gCurrentNearestPathPoint + 4) % gSelectedPathCount];
                        }
                    }
                    apply_cpu_turn(player, 0);
                    regulate_cpu_speed(playerId, gPreviousCpuTargetSpeed[playerId], player);
                    return;
                }
                if ((D_801630E8[playerId] == 1) || (D_801630E8[playerId] == -1)) {
                    player->effects |= DRIFTING_EFFECT;
                }
                if (D_801630E8[playerId] != 0) {
                    sPlayerAngle[playerId] = -get_angle_between_two_vectors(player->oldPos, player->pos);
                    var_a0_2 =
                        (gCurrentPathPointExpectedRotationPath[(sSomeNearestPathPoint + 2) % gSelectedPathCount] *
                         0x168) /
                        65535;
                    newAngle = (sPlayerAngle[playerId] * 0x168) / 65535;
                    if (var_a0_2 < -0xB4) {
                        var_a0_2 += 0x168;
                    }
                    if (var_a0_2 > 0xB4) {
                        var_a0_2 -= 0x168;
                    }
                    if (newAngle < -0xB4) {
                        newAngle += 0x168;
                    }
                    if (newAngle > 0xB4) {
                        newAngle -= 0x168;
                    }
                    steeringSensitivity = var_a0_2 - newAngle;
                    if (steeringSensitivity < -0xB4) {
                        steeringSensitivity += 0x168;
                    }
                    if (steeringSensitivity > 0xB4) {
                        steeringSensitivity -= 0x168;
                    }
                    switch (D_801630E8[playerId]) {
                        case -1:
                            if (steeringSensitivity > 5) {
                                D_801630E8[playerId] = 0;
                                player->effects &= ~DRIFTING_EFFECT;
                            }
                            break;
                        case 1:
                            if (steeringSensitivity < -5) {
                                D_801630E8[playerId] = 0;
                                player->effects &= ~DRIFTING_EFFECT;
                            }
                            break;
                        default:
                            break;
                    }
                }

                if ((playerId & 1) != (gIncrementUpdatePlayer & 1)) {
                    apply_cpu_turn(player, gPreviousAngleSteering[playerId]);
                    regulate_cpu_speed(playerId, gPreviousCpuTargetSpeed[playerId], player);
                    return;
                }
                gIsPlayerInCurve[playerId] = are_in_curve(playerId, sSomeNearestPathPoint);
                determine_ideal_cpu_position_offset(playerId, sSomeNearestPathPoint);
                if (!IsPodiumCeremony()) {
                    if (gNumPathPointsTraversed[playerId] <= 10) {
                        pathIndex = gCurrentNearestPathPoint;
                        if ((gNumPathPointsTraversed[playerId] > 0) && (IsToadsTurnpike())) {
                            pathIndex += 20;
                            pathIndex %= gSelectedPathCount;
                            set_track_offset_position(pathIndex, 0.0f, 0);
                            gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                        } else {
                            pathIndex += 8;
                            pathIndex %= gSelectedPathCount;
                            set_track_offset_position(pathIndex, gTrackPositionFactor[playerId], gPlayerPathIndex);
                            gPlayerTrackPositionFactorInstruction[playerId].current = gTrackPositionFactor[playerId];
                        }
                    }
                    if ((D_80162FD0 == 1) && (D_80162FF8[playerId] == 0)) {
                        pathIndex = gCurrentNearestPathPoint + 7;
                        pathIndex %= gSelectedPathCount;
                        set_track_offset_position(pathIndex, -0.7f, gPlayerPathIndex);
                    }

                    if (gPlayerPathIndex == 0) {
                        func_8000B140(playerId);
                        if (D_80162FF8[playerId] > 0) {
                            pathIndex = gCurrentNearestPathPoint + 5;

                            pathIndex %= gSelectedPathCount;
                            set_track_offset_position(pathIndex, D_80163090[playerId], gPlayerPathIndex);
                        }
                    }
                }
                if (IsPodiumCeremony()) {
                    switch (D_80163410[playerId]) { /* switch 3; irregular */
                        case 3:                     /* switch 3 */
                            gOffsetPosition[0] = D_80163418[playerId];
                            gOffsetPosition[2] = D_80163438[playerId];
                            break;
                        case 4: /* switch 3 */
                            pathPoint = &gTrackPaths[playerId][(gNearestPathPointByPlayerId[playerId] + 0xA) %
                                                               gPathCountByPathIndex[playerId]];
                            gOffsetPosition[0] = pathPoint->x;
                            gOffsetPosition[2] = pathPoint->z;
                            break;
                    }
                }
                gOffsetPosition[0] = (gPreviousPlayerAiOffsetX[playerId] + gOffsetPosition[0]) * 0.5f; // average
                gOffsetPosition[2] = (gPreviousPlayerAiOffsetZ[playerId] + gOffsetPosition[2]) * 0.5f; // average
                gPreviousPlayerAiOffsetX[playerId] = gOffsetPosition[0];
                gPreviousPlayerAiOffsetZ[playerId] = gOffsetPosition[2];
                minAngle = onePointFive * 182.0f;
                maxAngle = -onePointFive * 182.0f;

                angle = -get_angle_between_two_vectors(player->pos, gOffsetPosition);
                angle -= (newAngle = player->rotation[1]);
                if ((s16) minAngle < angle) {
                    angle = minAngle;
                }
                if (angle < (s16) maxAngle) {
                    angle = maxAngle;
                }
                steeringSensitivity = CM_GetProps()->AISteeringSensitivity;
                switch (gCurrentTrackSectionTypesPath[playerId]) { /* switch 4; irregular */
                    case RIGHT_CURVE:                              /* switch 4 */
                        if (gTrackPositionFactor[playerId] > (0.5f * 1.0f)) {
                            steeringSensitivity = 0x0014;
                        }
                        if (gTrackPositionFactor[playerId] < -0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        break;
                    case LEFT_CURVE: /* switch 4 */
                        if (gTrackPositionFactor[playerId] > 0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        if (gTrackPositionFactor[playerId] < -0.5f) {
                            steeringSensitivity = 0x0014;
                        }
                        break;
                }
                if ((cpu_BehaviourState[playerId] == CPU_BEHAVIOUR_STATE_RUNNING) &&
                    ((gTrackPositionFactor[playerId] > 0.9f) || (gTrackPositionFactor[playerId] < -0.9f))) {
                    D_801630E8[playerId] = 0;
                    player->effects &= ~DRIFTING_EFFECT;
                }
                if (player->effects & 2) {
                    switch (D_801630E8[playerId]) {
                        case 1:
                            newAngle = 0x0035;
                            break;
                        case -1:
                            newAngle = -0x0035;
                            break;
                        default:
                            newAngle =
                                (gPreviousAngleSteering[playerId] + ((angle * steeringSensitivity) / minAngle)) / 2;
                            break;
                    }
                } else if (player->effects & (UNKNOWN_EFFECT_0x10000000 | UNKNOWN_EFFECT_0xC)) {
                    newAngle = 0;
                } else {
                    newAngle = (gPreviousAngleSteering[playerId] + ((angle * steeringSensitivity) / minAngle)) / 2;
                }
                apply_cpu_turn(player, newAngle);
                gPreviousAngleSteering[playerId] = newAngle;
                if ((gIsPlayerInCurve[playerId] == true) || (D_801630E8[playerId] == 1) ||
                    (D_801630E8[playerId] == -1) ||
                    (player->effects & (UNKNOWN_EFFECT_0x10000000 | UNKNOWN_EFFECT_0xC))) {
                    cpu_TargetSpeed[playerId] = CM_GetProps()->CurveTargetSpeed[gCCSelection];
                } else {
                    cpu_TargetSpeed[playerId] = CM_GetProps()->NormalTargetSpeed[gCCSelection];
                }
                if ((gTrackPositionFactor[playerId] > 0.9f) || (gTrackPositionFactor[playerId] < -0.9f)) {
                    cpu_TargetSpeed[playerId] = CM_GetProps()->OffTrackTargetSpeed[gCCSelection];
                }
                if (D_80162FD0 == 1) {
                    cpu_TargetSpeed[playerId] = CM_GetProps()->D_0D0096B8[gCCSelection];
                }

                if (CVarGetInteger("gEnableCustomCC", 0) == 1) {
#define calc_a(x, y, x2, y2) (y2 - y) / (x2 - x)
#define calc_b(x, y, b) y - (b * x)
                    f32 a;
                    f32 b;

#define calc(table)                                   \
    a = calc_a(50, table[CC_50], 150, table[CC_150]); \
    b = calc_b(50, table[CC_50], a);                  \
    cpu_TargetSpeed[playerId] = a * CVarGetFloat("gCustomCC", 150.0f) + b;
                    // end of define
                    if ((gIsPlayerInCurve[playerId] == true) || (D_801630E8[playerId] == 1) ||
                        (D_801630E8[playerId] == -1) ||
                        (player->effects & (UNKNOWN_EFFECT_0x10000000 | UNKNOWN_EFFECT_0xC))) {
                        calc(CM_GetProps()->CurveTargetSpeed);
                    } else {
                        calc(CM_GetProps()->NormalTargetSpeed);
                    }
                    if ((gTrackPositionFactor[playerId] > 0.9f) || (gTrackPositionFactor[playerId] < -0.9f)) {
                        calc(CM_GetProps()->OffTrackTargetSpeed);
                    }
                    if (D_80162FD0 == 1) {
                        calc(CM_GetProps()->D_0D0096B8);
                    }
#undef calc_a
#undef calc_b
#undef calc
                }

                if ((D_801630E8[playerId] == 2) || (D_801630E8[playerId] == -2) || (D_801630E8[playerId] == 3)) {
                    cpu_TargetSpeed[playerId] = 3.3333333f;
                }
                // Override cpu speed for harder cpu enhancment
                if (CVarGetInteger("gHarderCPU", 0) == 1) {
                    cpu_TargetSpeed[playerId] = player->topSpeed * 1.5f;
                }

                gCurrentCpuTargetSpeed = cpu_TargetSpeed[playerId];
                player->effects &= ~UNKNOWN_EFFECT_0x200000;
                gPreviousCpuTargetSpeed[playerId] = gCurrentCpuTargetSpeed;
                CM_AICrossingBehaviour(playerId);
                // check_ai_crossing_distance(playerId);
                regulate_cpu_speed(playerId, gCurrentCpuTargetSpeed, player);
            }
        }
    }
}

void func_8000B140(s32 playerId) {
    s32 i;
    f32 temp_f12;
    f32 temp_f14;
    s32 j;
    f32 temp_f16;
    f32 temp_ft2;
    s16 currPathPoint;
    s16 temp_v1_2;
    f32 temp_f22;
    f32 temp_f0_2;
    f32 var_f18;
    f32 var_f20;
    UNUSED s32 pad[5];
    s16 spB0[8];
    UNUSED f32 pad2;
    s16 sp9C[8];
    UNUSED f32 pad3;
    f32 temp_f2;
    f32 sp74[8];
    s32 temp_a1_2;
    Player* player;
    player = &gPlayers[playerId];

    if (player->effects & DRIFTING_EFFECT) {
        return;
    }

    if (D_801630E8[playerId] == 1) {
        return;
    }

    if (D_801630E8[playerId] == -1) {
        return;
    }

    if (gTrackPositionFactor[playerId] < -1.0f) {
        return;
    }

    if (gTrackPositionFactor[playerId] > 1.0f) {
        return;
    }

    // Exclude heavyweights
    if (player->characterId == WARIO) {
        return;
    }

    if (player->characterId == BOWSER) {
        return;
    }

    if (player->characterId == DK) {
        return;
    }

    // Skip if player has star
    if (player->effects & STAR_EFFECT) {
        return;
    }

    currPathPoint = gNearestPathPointByPlayerId[playerId];
    temp_f22 = (player->speed / 18.0f) * 216.0f;
    for (i = 0; i < 8; i++) {
        sp9C[i] = -1;
        spB0[i] = 0x03E8;
    }

    if (D_80163010[playerId] > 0) {
        D_80163010[playerId]--;
        if (D_80163010[playerId] <= 0) {
            D_80162FF8[playerId] = 0;
        }
    }
    j = 0;
    i = 0;
    while (i < 8) {
        if (i != playerId) {
            player = &gPlayers[i];
            if ((player->type & PLAYER_EXISTS)) {
                temp_v1_2 = gNearestPathPointByPlayerId[i];
                temp_f0_2 = (player->speed / 18.0f) * 216.0f;
                temp_f2 = temp_f22 - 5.0f;
                if (temp_f0_2 < temp_f2) {
                    if (is_path_point_in_range(temp_v1_2, currPathPoint, 0, 0x0014U, gSelectedPathCount) > 0) {
                        temp_a1_2 = temp_v1_2 - currPathPoint;
                        sp9C[j] = i;
                        if (temp_a1_2 > 0) {
                            spB0[j] = temp_a1_2;
                        } else {
                            spB0[j] = (temp_v1_2 + gSelectedPathCount) - currPathPoint;
                        }
                        sp74[j] = temp_f2 - temp_f0_2;
                        j++;
                    }
                }
            }
        }
        i++;
        if (j >= 2) {
            break;
        }
    }

    if (j == 0) {
        return;
    }

    var_f18 = 1.0f;
    var_f20 = -1.0f;
    for (i = 0; i < j; i++) {
        temp_f2 = gTrackPositionFactor[sp9C[i]];
        if ((temp_f2 > (-1.0f)) && (temp_f2 < 1.0f)) {

            temp_f12 = temp_ft2 = ((0.2f * (20.0f / (spB0[i] + 20.0f))) * ((sp74[i]) + 10.0f)) / 20.0f;

            if ((var_f18 == 1.0f) && (var_f20 == (-1.0f))) {
                var_f18 = temp_f2 - temp_f12;
                var_f20 = temp_f2 + temp_f12;
            } else {
                temp_f14 = temp_f2 - temp_f12;
                temp_f16 = temp_f2 + temp_f12;
                if ((temp_f14 < var_f18) && (temp_f16 > var_f18)) {
                    var_f18 = temp_f14;
                }
                if ((temp_f16 > var_f20) && (temp_f14 < var_f20)) {
                    var_f20 = temp_f16;
                }
            }
        }
    }

    if (var_f20 < var_f18) {
        return;
    }

    if (gTrackPositionFactor[playerId] < var_f18) {
        return;
    }

    if (var_f20 < gTrackPositionFactor[playerId]) {
        return;
    }

    if (var_f20 > 1.0f) {
        var_f20 = 1.0f;
    }
    if (var_f18 < (-1.0f)) {
        var_f18 = -1.0f;
    }
    if ((var_f18 + 1.0f) < (1.0f - var_f20)) {
        D_80163010[playerId] = 0x003C;
        D_80162FF8[playerId] = 1;
        D_80163090[playerId] = var_f20;
    } else {
        D_80163010[playerId] = 0x003C;
        D_80162FF8[playerId] = 2;
        D_80163090[playerId] = var_f18;
    }
}

s32 are_in_curve(UNUSED s32 arg0, u16 waypointIndex) {
    s16 thing = gCurrentTrackConsecutiveCurveCountsPath[waypointIndex];
    if (thing > 0) {
        return 1;
    }
    return 0;
}

bool is_far_from_path(s32 playerIndex) {
    f32 value = gTrackPositionFactor[playerIndex];
    if ((1.1f <= value) || (value <= -1.1f)) {
        return true;
    }
    return false;
}

f32 calculate_track_position_factor(f32 posX, f32 posZ, u16 waypointIndex, s32 pathIndex) {
    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 squaredDistance;
    f32 math;
    TrackPathPoint* thing1;
    TrackPathPoint* thing2;

    thing1 = &gTrackLeftPaths[pathIndex][waypointIndex];
    thing2 = &gTrackRightPaths[pathIndex][waypointIndex];

    x1 = thing1->x;
    z1 = thing1->z;
    x2 = thing2->x;
    z2 = thing2->z;

    squaredDistance = ((x2 - x1) * (x2 - x1)) + ((z2 - z1) * (z2 - z1));
    if (squaredDistance < 0.01f) {
        return 0.0f;
    }
    math = ((2.0f * ((x2 - x1) * (posX - x1) + (z2 - z1) * (posZ - z1))) / squaredDistance) - 1.0f;
    return math;
}

void update_player_position_factor(s32 playerId, u16 waypointIndex, s32 pathIndex) {
    UNUSED Vec3f pad;
    gTrackPositionFactor[playerId] = 0.0f;
    if (CM_GetProps()->AIMaximumSeparation >= 0.0f) {
        if ((gPlayers[playerId].type & 0x8000) != 0) {
            gTrackPositionFactor[playerId] = calculate_track_position_factor(
                gPlayers[playerId].pos[0], gPlayers[playerId].pos[2], waypointIndex, pathIndex);
        }
    }
}

void calculate_track_offset_position(u16 waypointIndex, f32 arg1, f32 arg2, s16 pathIndex) {
    UNUSED s32 pad[4];
    f32 waypointOneX;
    f32 waypointOneZ;
    f32 waypointTwoX;
    f32 waypointTwoZ;
    UNUSED s32 pad2;
    f32 xdiff;
    f32 zdiff;
    f32 distance;
    UNUSED f32 temp_f12;
    UNUSED f32 temp_f2_2;
    UNUSED TrackPathPoint* path;
    TrackPathPoint* waypointTwo;
    TrackPathPoint* waypointOne;

    waypointOne = &gTrackPaths[pathIndex][waypointIndex];
    waypointOneX = waypointOne->x;
    waypointOneZ = waypointOne->z;
    waypointTwo = &gTrackPaths[pathIndex][(waypointIndex + 1) % gSelectedPathCount];
    waypointTwoX = waypointTwo->x;
    waypointTwoZ = waypointTwo->z;
    zdiff = waypointTwoZ - waypointOneZ;
    xdiff = waypointTwoX - waypointOneX;
    if (xdiff && xdiff) {}
    distance = sqrtf((xdiff * xdiff) + (zdiff * zdiff));
    if (distance < 0.01f) {
        gOffsetPosition[0] = waypointTwoX;
        gOffsetPosition[2] = waypointTwoZ;
    } else {
        gOffsetPosition[0] = ((0.5f - (arg1 * 0.5f)) * (((arg2 * zdiff) / distance) + waypointOneX)) +
                             ((1.0f - (0.5f - (arg1 * 0.5f))) * (((arg2 * -zdiff) / distance) + waypointOneX));
        gOffsetPosition[2] = ((0.5f - (arg1 * 0.5f)) * (((arg2 * -xdiff) / distance) + waypointOneZ)) +
                             ((1.0f - (0.5f - (arg1 * 0.5f))) * (((arg2 * xdiff) / distance) + waypointOneZ));
    }
}

void set_track_offset_position(u16 waypointIndex, f32 arg1, s16 pathIndex) {
    TrackPathPoint* path1;
    TrackPathPoint* path2;
    f32 x1;
    f32 z1;
    f32 x3;
    f32 z3;
    f32 x2;
    f32 z2;
    f32 x4;
    f32 z4;
    f32 temp_f0;
    f32 temp_f12;

    path1 = &gTrackLeftPaths[pathIndex][waypointIndex];
    path2 = &gTrackRightPaths[pathIndex][waypointIndex];

    x1 = (f32) path1->x;
    z1 = (f32) path1->z;

    x2 = (f32) path2->x;
    z2 = (f32) path2->z;

    waypointIndex += 1;
    waypointIndex = waypointIndex % gPathCountByPathIndex[pathIndex];

    path1 = &gTrackLeftPaths[pathIndex][waypointIndex];
    path2 = &gTrackRightPaths[pathIndex][waypointIndex];

    x3 = (f32) path1->x;
    z3 = (f32) path1->z;

    x4 = (f32) path2->x;
    z4 = (f32) path2->z;

    temp_f0 = 0.5f - (arg1 / 2.0f);
    temp_f12 = 1.0f - temp_f0;
    gOffsetPosition[0] = ((temp_f0 * (x1 + x3)) / 2.0f) + ((temp_f12 * (x2 + x4)) / 2.0f);
    gOffsetPosition[2] = ((temp_f0 * (z1 + z3)) / 2.0f) + ((temp_f12 * (z2 + z4)) / 2.0f);
}

s16 func_8000BD94(f32 posX, f32 posY, f32 posZ, s32 pathIndex) {
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 considerSquaredDistance;
    f32 minimumSquaredDistance;
    s32 considerWaypointIndex;
    s32 pathWaypointCount;
    s16 nearestWaypointIndex;
    TrackPathPoint* pathWaypoints;
    TrackPathPoint* considerWaypoint;

    pathWaypoints = gTrackPaths[pathIndex];
    pathWaypointCount = gPathCountByPathIndex[pathIndex];
    considerWaypoint = &pathWaypoints[0];
    x_dist = (f32) considerWaypoint->x - posX;
    y_dist = (f32) considerWaypoint->y - posY;
    z_dist = (f32) considerWaypoint->z - posZ;
    minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
    nearestWaypointIndex = 0;
    for (considerWaypointIndex = 1; considerWaypointIndex < pathWaypointCount;
         considerWaypoint++, considerWaypointIndex++) {
        x_dist = (f32) considerWaypoint->x - posX;
        y_dist = (f32) considerWaypoint->y - posY;
        z_dist = (f32) considerWaypoint->z - posZ;
        considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (considerSquaredDistance < minimumSquaredDistance) {
            nearestWaypointIndex = considerWaypointIndex;
            minimumSquaredDistance = considerSquaredDistance;
        }
    }
    return nearestWaypointIndex;
}

s16 find_closest_path_point_track_section(f32 posX, f32 posY, f32 posZ, u16 trackSectionId, s32* pathIndex) {
    TrackPathPoint* pathWaypoints;
    TrackPathPoint* considerWaypoint;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 considerSquaredDistance;
    f32 minimumSquaredDistance;
    s32 considerWaypointIndex;
    s32 pathWaypointCount;
    s32 temp_t0;
    s32 var_a1;
    s32 var_t1;
    s32 considerPathIndex;
    s32 var_t4;
    s16 nearestWaypointIndex;

    minimumSquaredDistance = 1000000.0f;
    temp_t0 = *pathIndex;
    nearestWaypointIndex = 0;
    var_t1 = 0;
    var_a1 = 0;
    pathWaypoints = gTrackPaths[temp_t0];
    pathWaypointCount = gPathCountByPathIndex[temp_t0];
    considerWaypoint = &pathWaypoints[0];
    for (considerWaypointIndex = 0; considerWaypointIndex < pathWaypointCount;
         considerWaypointIndex++, considerWaypoint++) {
        if ((considerWaypoint->trackSectionId == trackSectionId) || (IsPodiumCeremony())) {
            var_t1 = 1;
            x_dist = (f32) considerWaypoint->x - posX;
            y_dist = (f32) considerWaypoint->y - posY;
            z_dist = (f32) considerWaypoint->z - posZ;
            considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (considerSquaredDistance < minimumSquaredDistance) {
                nearestWaypointIndex = considerWaypointIndex;
                var_a1 = 1;
                minimumSquaredDistance = considerSquaredDistance;
            }
        }
    }
    if (var_t1 == 0) {
        for (considerPathIndex = 0; considerPathIndex < 4; considerPathIndex++) {
            if ((considerPathIndex != temp_t0) && (D_80163368[considerPathIndex] >= 2)) {
                pathWaypoints = gTrackPaths[considerPathIndex];
                considerWaypoint = &pathWaypoints[0];
                pathWaypointCount = gPathCountByPathIndex[considerPathIndex];
                for (considerWaypointIndex = 0; considerWaypointIndex < pathWaypointCount;
                     considerWaypointIndex++, considerWaypoint++) {
                    if (considerWaypoint->trackSectionId == trackSectionId) {
                        x_dist = (f32) considerWaypoint->x - posX;
                        y_dist = (f32) considerWaypoint->y - posY;
                        z_dist = (f32) considerWaypoint->z - posZ;
                        considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
                        if (considerSquaredDistance < minimumSquaredDistance) {
                            nearestWaypointIndex = considerWaypointIndex;
                            var_t4 = considerPathIndex;
                            var_a1 = 2;
                            minimumSquaredDistance = considerSquaredDistance;
                        }
                    }
                }
            }
        }
    }
    if (var_a1 == 0) {
        pathWaypoints = gTrackPaths[0];
        pathWaypointCount = gPathCountByPathIndex[0];
        considerWaypoint = &pathWaypoints[0];
        x_dist = (f32) considerWaypoint->x - posX;
        y_dist = (f32) considerWaypoint->y - posY;
        z_dist = (f32) considerWaypoint->z - posZ;
        minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        nearestWaypointIndex = 0;
        for (considerWaypointIndex = 1; considerWaypointIndex < pathWaypointCount;
             considerWaypoint++, considerWaypointIndex++) {
            x_dist = (f32) considerWaypoint->x - posX;
            y_dist = (f32) considerWaypoint->y - posY;
            z_dist = (f32) considerWaypoint->z - posZ;
            considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (considerSquaredDistance < minimumSquaredDistance) {
                nearestWaypointIndex = considerWaypointIndex;
                var_t4 = 0;
                var_a1 = 2;
                minimumSquaredDistance = considerSquaredDistance;
            }
        }
    }
    if (var_a1 == 2) {
        *pathIndex = var_t4;
    }
    return nearestWaypointIndex;
}

/**
 * Tries to find the waypoint nearest to (posX, posY, posZ)
 * Only consider waypoints in the same segment as trackSectionId
 * Only considers waypoints within 500 units of(posX, posY, posZ)
 * Looks 3 waypoints behind and 6 waypoints ahead of waypointIndex
 **/
s16 update_path_index_with_track(f32 posX, f32 posY, f32 posZ, s16 waypointIndex, s32 pathIndex, u16 trackSectionId) {
    s16 nearestWaypointIndex;
    s16 searchIndex;
    s16 considerIndex;
    s32 pathWaypointCount;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 minimumDistance;
    f32 squaredDistance;
    TrackPathPoint* pathWaypoints;
    TrackPathPoint* considerWaypoint;

    nearestWaypointIndex = -1;
    minimumDistance = 250000.0f;
    pathWaypointCount = gPathCountByPathIndex[pathIndex];
    pathWaypoints = gTrackPaths[pathIndex];
    for (searchIndex = waypointIndex - 3; searchIndex < waypointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the number of waypoints in a given path
        // This is done to ensure we access gTrackPaths at a valid index
        considerIndex = (searchIndex + pathWaypointCount) % pathWaypointCount;
        considerWaypoint = &pathWaypoints[considerIndex];
        if (considerWaypoint->trackSectionId == trackSectionId) {
            x_dist = considerWaypoint->x - posX;
            y_dist = considerWaypoint->y - posY;
            z_dist = considerWaypoint->z - posZ;
            squaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (squaredDistance < minimumDistance) {
                minimumDistance = squaredDistance;
                nearestWaypointIndex = considerIndex;
            }
        }
    }
    return nearestWaypointIndex;
}

/**
 * Tries to find the waypoint nearest to (posX, posY, posZ)
 * Only considers waypoints within 400 units of (posX, posY, posZ)
 * Looks 3 waypoints behind and 6 waypoints ahead of waypointIndex
 **/
s16 update_path_index(f32 posX, f32 posY, f32 posZ, s16 waypointIndex, s32 pathIndex) {
    s16 nearestWaypointIndex;
    s16 searchIndex;
    s16 considerIndex;
    s32 waypointFound;
    s32 pathWaypointCount;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 minimumDistance;
    f32 squaredDistance;
    TrackPathPoint* pathWaypoints;
    TrackPathPoint* considerWaypoint;

    waypointFound = 0;
    nearestWaypointIndex = -1;
    minimumDistance = 160000.0f;
    pathWaypointCount = gPathCountByPathIndex[pathIndex];
    pathWaypoints = gTrackPaths[pathIndex];
    for (searchIndex = waypointIndex - 3; searchIndex < waypointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the number of waypoints in a given path
        // This is done to ensure we access gTrackPaths at a valid index
        considerIndex = (searchIndex + pathWaypointCount) % pathWaypointCount;
        considerWaypoint = &pathWaypoints[considerIndex];
        x_dist = considerWaypoint->x - posX;
        y_dist = considerWaypoint->y - posY;
        z_dist = considerWaypoint->z - posZ;
        squaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (squaredDistance < minimumDistance) {
            minimumDistance = squaredDistance;
            nearestWaypointIndex = considerIndex;
            waypointFound = 1;
        }
    }
    if (waypointFound == 0) {
        for (searchIndex = waypointIndex - 3; searchIndex < waypointIndex + 7; searchIndex++) {
            considerIndex = ((searchIndex + pathWaypointCount) % pathWaypointCount);
            considerWaypoint = &pathWaypoints[considerIndex];
            /**
             * This fake match is done to stop the compiler from optimzing out considerWaypoint.
             * Maybe if no waypoint was found some debugging info was printed out, but come
             * production time they removed the debug printing but not the loop?
             **/
            if (considerWaypoint && considerWaypoint) {};
        }
    }
    return nearestWaypointIndex;
}

void tweak_path_index_wario_stadium(UNUSED f32 posX, f32 posY, UNUSED f32 posZ, s16* waypointIndex, UNUSED s32 arg4) {
    s16 var_v0;

    var_v0 = *waypointIndex;
    if ((IsWarioStadium()) && (var_v0 >= 0x475) && (var_v0 < 0x480) && (posY < 0.0f)) {
        var_v0 = 0x0398;
    }
    *waypointIndex = var_v0;
}

void adjust_path_at_start_line(UNUSED f32 posX, UNUSED f32 posY, f32 posZ, s16* waypointIndex, s32 pathIndex) {
    s16 temp;
    temp = *waypointIndex;
    if (temp == 0) {
        if (gPathStartZ < posZ) {
            temp = gPathCountByPathIndex[pathIndex] - 1;
        }
    } else if (((temp + 1) == gPathCountByPathIndex[pathIndex]) && (posZ <= gPathStartZ)) {
        temp = 0;
    }
    *waypointIndex = temp;
}

s16 update_path_index_track_section(f32 posX, f32 posY, f32 posZ, Player* player, s32 playerId, s32* pathIndex) {
    u16 trackSectionId;
    s16 ret;

    trackSectionId = get_track_section_id(player->collision.meshIndexZX);
    if ((trackSectionId <= 0) || (trackSectionId >= 0x33)) {
        trackSectionId = gPlayersTrackSectionId[playerId];
    }
    gPlayersTrackSectionId[playerId] = trackSectionId;
    ret = find_closest_path_point_track_section(posX, posY, posZ, trackSectionId, pathIndex);
    gPathIndexByPlayerId[playerId] = *pathIndex;
    return ret;
}

s16 update_player_path(f32 posX, f32 posY, f32 posZ, s16 waypointIndex, Player* player, s32 playerId, s32 pathIndex) {
    s16 newWaypoint;
    UNUSED s16 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    TrackPathPoint* temp_v1;

    if ((player->type & 0x4000) && !(player->type & 0x1000)) {
        newWaypoint = update_path_index_with_track(posX, posY, posZ, waypointIndex, pathIndex,
                                                   (u16) get_track_section_id(player->collision.meshIndexZX));
        if (newWaypoint == -1) {
            newWaypoint = update_path_index_track_section(posX, posY, posZ, player, playerId, &pathIndex);
        }
    } else {
        if (D_801631E0[playerId] == 1) {
            if (player->unk_0CA & 1) {
                temp_v1 = &gTrackPaths[pathIndex][waypointIndex];
                player->pos[0] = (f32) temp_v1->x;
                player->pos[1] = (f32) temp_v1->y;
                player->pos[2] = (f32) temp_v1->z;
                player->unk_0CA &= ~0x0001;
                return waypointIndex;
            }
            if (playerId == ((s32) D_80163488 % 8)) {
                check_bounding_collision(&player->collision, 10.0f, posX, posY, posZ);
                gPlayersTrackSectionId[playerId] = get_track_section_id(player->collision.meshIndexZX);
                newWaypoint = update_path_index_with_track(posX, posY, posZ, waypointIndex, pathIndex,
                                                           gPlayersTrackSectionId[playerId]);
                if (newWaypoint == -1) {
                    newWaypoint = update_path_index(posX, posY, posZ, waypointIndex, pathIndex);
                }
                if (newWaypoint == -1) {
                    newWaypoint = find_closest_path_point_track_section(posX, posY, posZ,
                                                                        gPlayersTrackSectionId[playerId], &pathIndex);
                    temp_v1 = &gTrackPaths[pathIndex][newWaypoint];
                    player->pos[0] = (f32) temp_v1->x;
                    player->pos[1] = (f32) temp_v1->y;
                    player->pos[2] = (f32) temp_v1->z;
                }
            } else {
                newWaypoint = update_path_index(posX, posY, posZ, waypointIndex, pathIndex);
                if (newWaypoint == -1) {
                    newWaypoint = func_8000BD94(posX, posY, posZ, pathIndex);
                    temp_v1 = &gTrackPaths[pathIndex][newWaypoint];
                    posX = (f32) temp_v1->x;
                    posY = (f32) temp_v1->y;
                    posZ = (f32) temp_v1->z;
                    player->pos[0] = posX;
                    player->pos[1] = posY;
                    player->pos[2] = posZ;
                    check_bounding_collision(&player->collision, 10.0f, posX, posY, posZ);
                    gPlayersTrackSectionId[playerId] = get_track_section_id(player->collision.meshIndexZX);
                }
            }
        } else {
            newWaypoint = update_path_index(posX, posY, posZ, waypointIndex, pathIndex);
            if (newWaypoint == -1) {
                newWaypoint = update_path_index_track_section(posX, posY, posZ, player, playerId, &pathIndex);
            }
        }
        tweak_path_index_wario_stadium(posX, posY, posZ, &newWaypoint, pathIndex);
    }
    adjust_path_at_start_line(posX, posY, posZ, &newWaypoint, pathIndex);
    return newWaypoint;
}

s16 find_closest_vehicles_path_point(f32 xPos, UNUSED f32 yPos, f32 zPos, s16 waypointIndex) {
    f32 xdiff;
    f32 zdiff;
    f32 minimumDistance;
    f32 considerSquaredDistance;
    s16 realIndex;
    s16 minimumIndex;
    s16 considerIndex;
    TrackPathPoint* considerWaypoint;

    minimumDistance = 250000.0f;
    minimumIndex = -1;
    for (realIndex = waypointIndex - 2; realIndex < waypointIndex + 7; realIndex++) {
        considerIndex = realIndex;
        if (realIndex < 0) {
            considerIndex = realIndex + gVehiclePathSize;
        }
        considerIndex %= gVehiclePathSize;
        considerWaypoint = &gVehiclePath[considerIndex];
        xdiff = considerWaypoint->x - xPos;
        zdiff = considerWaypoint->z - zPos;
        considerSquaredDistance = (xdiff * xdiff) + (zdiff * zdiff);
        if (considerSquaredDistance < minimumDistance) {
            minimumDistance = considerSquaredDistance;
            minimumIndex = considerIndex;
        }
    }
    if (minimumIndex == -1) {
        minimumIndex = waypointIndex;
    }
    return minimumIndex;
}

s16 func_8000D24C(f32 posX, f32 posY, f32 posZ, s32* pathIndex) {
    UNUSED s32 pad;
    Collision sp24;

    check_bounding_collision(&sp24, 10.0f, posX, posY, posZ);
    return find_closest_path_point_track_section(posX, posY, posZ, get_track_section_id(sp24.meshIndexZX), pathIndex);
}

s16 func_8000D2B4(f32 posX, f32 posY, f32 posZ, s16 waypointIndex, s32 pathIndex) {
    s16 waypoint;

    waypoint = update_path_index(posX, posY, posZ, waypointIndex, pathIndex);
    if (waypoint == -1) {
        waypoint = func_8000D24C(posX, posY, posZ, &pathIndex);
    }
    adjust_path_at_start_line(posX, posY, posZ, &waypoint, pathIndex);
    return waypoint;
}

s16 func_8000D33C(f32 posX, f32 posY, f32 posZ, s16 waypointIndex, s32 pathIndex) {
    s16 waypoint;

    waypoint = update_path_index(posX, posY, posZ, waypointIndex, pathIndex);
    if (waypoint == -1) {
        waypoint = func_8000D24C(posX, posY, posZ, &pathIndex);
    }
    return waypoint;
}

f32 cpu_track_position_factor(s32 playerId) {
    TrackPositionFactorInstruction* temp_v0;
    f32 target;
    f32 current;

    temp_v0 = &gPlayerTrackPositionFactorInstruction[playerId];
    current = temp_v0->current;
    target = temp_v0->target;
    if (current < target) {
        current += temp_v0->step;
        if (target < current) {
            current = target;
        }
    } else if (target < current) {
        current -= temp_v0->step;
        if (current < target) {
            current = target;
        }
    }

    temp_v0->current = current;
    return current;
}

void determine_ideal_cpu_position_offset(s32 arg0, u16 arg1) {
    UNUSED s32 stackPadding0;
    f32 sp48;
    f32 sp44;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    f32 stackPadding4;
    f32 stackPadding5;
    f32 sp2C;
    s32 var_a2;
    s16 temp_v1;
    u16 thing;

    temp_v1 = gCurrentTrackConsecutiveCurveCountsPath[arg1];
    var_a2 = 6;
    sp2C = cpu_track_position_factor(arg0);
    thing = arg1;

    if (IsPodiumCeremony()) {
        var_a2 = 1;
    } else if (IsToadsTurnpike()) {
        var_a2 = 7;
    } else if (IsYoshiValley()) {
    } else {
        if (temp_v1 < 6) {
            var_a2 = 8;
        } else if (temp_v1 >= 0x15) {
            var_a2 = 20;
        }
    }

    if (var_a2 >= 8) {
        if ((gTrackPositionFactor[arg0] > 0.75f) && (gCurrentTrackSectionTypesPath[thing] == 0)) {
            var_a2 = 7;
        }
        if ((gTrackPositionFactor[arg0] < -0.75f) && (gCurrentTrackSectionTypesPath[thing] == 1)) {
            var_a2 = 7;
        }
    }
    if (is_far_from_path(arg0) == 1) {
        var_a2 = 5;
    }
    if (gCurrentPlayerLookAhead[arg0] < var_a2) {
        gCurrentPlayerLookAhead[arg0]++;
    }
    if (var_a2 < gCurrentPlayerLookAhead[arg0]) {
        gCurrentPlayerLookAhead[arg0]--;
    }
    arg1 = (gCurrentPlayerLookAhead[arg0] + arg1) % gSelectedPathCount;
    set_track_offset_position(arg1, sp2C, gPlayerPathIndex);
    sp48 = gOffsetPosition[0];
    sp44 = gOffsetPosition[2];
    set_track_offset_position(((arg1 + 1) % gSelectedPathCount) & 0xFFFF, sp2C, gPlayerPathIndex);
    stackPadding5 = gOffsetPosition[0];
    gOffsetPosition[0] = (sp48 + stackPadding5) * 0.5f;
    stackPadding4 = gOffsetPosition[2];
    gOffsetPosition[2] = (sp44 + stackPadding4) * 0.5f;
}

s16 func_8000D6D0(Vec3f position, s16* waypointIndex, f32 speed, f32 arg3, s16 pathIndex, s16 arg5) {
    f32 temp1;
    f32 temp2;
    f32 midX;
    UNUSED s16 stackPadding1;
    s16 waypoint1;
    s16 waypoint2;
    f32 pad3;
    f32 midY;
    f32 pad4;
    f32 midZ;
    f32 distance;
    f32 oldPosX;
    f32 oldPosY;
    f32 oldPosZ;
    f32 var_f2;
    f32 var_f12;
    f32 var_f14;
    s16 temp_v0;
    s32 temp_v1;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    Vec3f oldPos;
    TrackPathPoint* path;

    path = gTrackPaths[pathIndex];
    oldPos[0] = position[0];
    oldPos[1] = position[1];
    oldPos[2] = position[2];
    oldPosX = position[0];
    oldPosY = position[1];
    oldPosZ = position[2];
    temp_v0 = func_8000D2B4(oldPosX, oldPosY, oldPosZ, *waypointIndex, (s32) pathIndex);
    *waypointIndex = temp_v0;
    temp_v1 = temp_v0 + arg5;
    waypoint1 = temp_v1 % gPathCountByPathIndex[pathIndex];
    waypoint2 = (temp_v1 + 1) % gPathCountByPathIndex[pathIndex];
    set_track_offset_position(waypoint1, arg3, pathIndex);
    pad3 = gOffsetPosition[0];
    pad4 = gOffsetPosition[2];
    set_track_offset_position(waypoint2, arg3, pathIndex);
    temp1 = gOffsetPosition[0];
    temp2 = gOffsetPosition[2];
    midY = (path[waypoint1].y + path[waypoint2].y) * 0.5f;
    midX = (pad3 + temp1) * 0.5f;
    midZ = (pad4 + temp2) * 0.5f;
    xdiff = midX - oldPosX;
    ydiff = midY - oldPosY;
    zdiff = midZ - oldPosZ;
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance > 0.01f) {
        var_f2 = ((xdiff * speed) / distance) + oldPosX;
        var_f12 = ((ydiff * speed) / distance) + oldPosY;
        var_f14 = ((zdiff * speed) / distance) + oldPosZ;
    } else {
        var_f2 = oldPosX;
        var_f12 = oldPosY;
        var_f14 = oldPosZ;
    }
    position[0] = var_f2;
    position[1] = var_f12;
    position[2] = var_f14;
    return get_angle_between_path(oldPos, position);
}

s16 func_8000D940(Vec3f pos, s16* waypointIndex, f32 speed, f32 arg3, s16 pathIndex) {
    UNUSED f32 pad;
    f32 thing1;
    f32 thing2;
    UNUSED s16 stackPadding1;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED s16 stackPadding2;
    f32 pad2;
    f32 midX;
    f32 pad3;
    f32 midY;
    f32 midZ;
    f32 distance;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 var_f2;
    f32 var_f12;
    f32 var_f14;
    s16 temp_v0;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    s32 waypointCount;
    Vec3f sp54;

    sp54[0] = pos[0];
    sp54[1] = pos[1];
    sp54[2] = pos[2];
    waypointCount = gPathCountByPathIndex[pathIndex];
    temp_f20 = pos[0];
    temp_f22 = pos[1];
    temp_f24 = pos[2];
    temp_v0 = func_8000D2B4(temp_f20, temp_f22, temp_f24, *waypointIndex, (s32) pathIndex);
    *waypointIndex = temp_v0;
    waypoint1 = ((temp_v0 + waypointCount) - 3) % waypointCount;
    waypoint2 = ((temp_v0 + waypointCount) - 4) % waypointCount;
    set_track_offset_position(waypoint1, arg3, pathIndex);
    pad2 = gOffsetPosition[0];
    pad3 = gOffsetPosition[2];
    set_track_offset_position(waypoint2, arg3, pathIndex);
    thing1 = gOffsetPosition[0];
    thing2 = gOffsetPosition[2];
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) * 0.5f;
    midX = (pad2 + thing1) * 0.5f;
    midZ = (pad3 + thing2) * 0.5f;
    xdiff = midX - temp_f20;
    ydiff = midY - temp_f22;
    zdiff = midZ - temp_f24;
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance > 0.01f) {
        var_f2 = ((xdiff * speed) / distance) + temp_f20;
        var_f12 = ((ydiff * speed) / distance) + temp_f22;
        var_f14 = ((zdiff * speed) / distance) + temp_f24;
    } else {
        var_f2 = temp_f20;
        var_f12 = temp_f22;
        var_f14 = temp_f24;
    }
    pos[0] = var_f2;
    pos[1] = var_f12;
    pos[2] = var_f14;
    return get_angle_between_path(sp54, pos);
}

s16 update_vehicle_following_path(Vec3f pos, s16* waypointIndex, f32 speed) {
    f32 origXPos;
    f32 origYPos;
    f32 origZPos;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    UNUSED s32 stackPadding5;
    UNUSED s32 stackPadding6;
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    f32 farWaypointAverageX;
    f32 farWaypointAverageY;
    f32 farWaypointAverageZ;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 distance;
    f32 newX;
    f32 newY;
    f32 newZ;
    s16 newWaypointIndex;
    s16 farWaypoint1;
    s16 farWaypoint2;
    TrackPathPoint* temp_a0;
    TrackPathPoint* temp_a2;
    Vec3f sp38;

    origXPos = pos[0];
    origYPos = pos[1];
    origZPos = pos[2];
    sp38[0] = pos[0];
    sp38[1] = pos[1];
    sp38[2] = pos[2];
    newWaypointIndex = find_closest_vehicles_path_point(origXPos, origYPos, origZPos, *waypointIndex);
    *waypointIndex = newWaypointIndex;
    farWaypoint1 = (newWaypointIndex + 3) % gVehiclePathSize;
    farWaypoint2 = (newWaypointIndex + 4) % gVehiclePathSize;
    temp_a0 = &gVehiclePath[farWaypoint1];
    temp_a2 = &gVehiclePath[farWaypoint2];
    farWaypointAverageX = (temp_a0->x + temp_a2->x) * 0.5f;
    farWaypointAverageY = (temp_a0->y + temp_a2->y) * 0.5f;
    farWaypointAverageZ = (temp_a0->z + temp_a2->z) * 0.5f;
    x_dist = farWaypointAverageX - origXPos;
    y_dist = farWaypointAverageY - origYPos;
    z_dist = farWaypointAverageZ - origZPos;
    distance = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (distance > 0.01f) {
        newX = ((x_dist * speed) / distance) + origXPos;
        newY = ((y_dist * speed) / distance) + origYPos;
        newZ = ((z_dist * speed) / distance) + origZPos;
    } else {
        newX = origXPos;
        newY = origYPos;
        newZ = origZPos;
    }
    pos[0] = newX;
    // These tracks use a 2D path. So we need fallback for that.
    if (IsKalimariDesert() || IsDkJungle()) {
        pos[1] = pos[1];
    } else {
        pos[1] = newY;
    }
    pos[2] = newZ;
    return get_angle_between_path(sp38, pos);
}

void set_bomb_kart_spawn_positions(void) {
    UNUSED Collision* var_s2;
    f32 startingXPos;
    f32 startingZPos;
    f32 startingYPos;
    s32 var_s3;
    TrackPathPoint* temp_v0;
    UNUSED BombKart* var_s0;
    BombKartSpawn* bombKartSpawn;

    for (var_s3 = 0; var_s3 < NUM_BOMB_KARTS_VERSUS; var_s3++) {
        // bombKartSpawn = &gBombKartSpawns[gCurrentCourseId][var_s3];
        if (IsYoshiValley()) {
            startingXPos = bombKartSpawn->startingXPos;
            startingZPos = bombKartSpawn->startingZPos;
            startingYPos = spawn_actor_on_surface(startingXPos, 2000.0f, startingZPos);
        } else if (IsPodiumCeremony()) {
            temp_v0 = &gTrackPaths[3][bombKartSpawn->waypointIndex];
            startingXPos = temp_v0->x;
            startingYPos = temp_v0->y;
            startingZPos = temp_v0->z;
        } else {
            temp_v0 = &gTrackPaths[0][bombKartSpawn->waypointIndex];
            startingXPos = temp_v0->x;
            startingYPos = temp_v0->y;
            startingZPos = temp_v0->z;
        }

        gBombKarts[var_s3].bombPos[0] = startingXPos;
        gBombKarts[var_s3].bombPos[1] = startingYPos;
        gBombKarts[var_s3].bombPos[2] = startingZPos;
        gBombKarts[var_s3].wheel1Pos[0] = startingXPos;
        gBombKarts[var_s3].wheel1Pos[1] = startingYPos;
        gBombKarts[var_s3].wheel1Pos[2] = startingZPos;
        gBombKarts[var_s3].wheel2Pos[0] = startingXPos;
        gBombKarts[var_s3].wheel2Pos[1] = startingYPos;
        gBombKarts[var_s3].wheel2Pos[2] = startingZPos;
        gBombKarts[var_s3].wheel3Pos[0] = startingXPos;
        gBombKarts[var_s3].wheel3Pos[1] = startingYPos;
        gBombKarts[var_s3].wheel3Pos[2] = startingZPos;
        gBombKarts[var_s3].wheel4Pos[0] = startingXPos;
        gBombKarts[var_s3].wheel4Pos[1] = startingYPos;
        gBombKarts[var_s3].wheel4Pos[2] = startingZPos;
        gBombKarts[var_s3].waypointIndex = bombKartSpawn->waypointIndex;
        gBombKarts[var_s3].unk_3C = bombKartSpawn->unk_04;
        gBombKarts[var_s3].bounceTimer = 0;
        gBombKarts[var_s3].circleTimer = 0;
        gBombKarts[var_s3].state = bombKartSpawn->startingState;
        gBombKarts[var_s3].unk_4A = 0;
        gBombKarts[var_s3].unk_4C = 1;
        gBombKarts[var_s3].yPos = startingYPos;
        check_bounding_collision(&gBombKartCollision[var_s3], 2.0f, startingXPos, startingYPos, startingZPos);
    }
}

void func_8000DF8C(s32 bombKartId) {
    UNUSED s32 stackPadding0;
    f32 sp118;
    f32 var_f18;
    TrackPathPoint* temp_v0_2;
    f32 temp_f0_3;
    f32 sp108;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    f32 temp_f14;
    UNUSED s32 stackPadding4;
    f32 temp_f16;
    UNUSED s32 stackPadding5;
    UNUSED s32 stackPadding6;
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    UNUSED s32 stackPadding9;
    UNUSED s32 stackPaddingA;
    UNUSED s32 stackPaddingB;
    f32 temp_f0;
    f32 temp_f0_4;
    u16 spCA;
    f32 spC4;
    u16 spC2;
    f32 temp_f12;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f14_2;
    f32 spAC;
    UNUSED s32 stackPaddingC;
    f32 temp_f16_2;
    f32 spA0;
    f32 temp_f2;
    f32 temp_f2_4;
    f32 sp94;
    UNUSED s32 stackPaddingD;
    f32 var_f20;
    f32 sp88;
    f32 var_f22;
    f32 var_f24;
    u16 sp7E;
    u16 sp7C;
    UNUSED u16 sp4C;
    u16 temp_t6;
    u16 temp_t7;
    u16 var_s1;
    s32 var_a0;
    UNUSED s32 stackPaddingE;
    TrackPathPoint* temp_v0_4;
    BombKart* bombKart;
    BombKart* bombKart2;
    Collision* temp_a0_4;
    Player* var_v0;

    bombKart = &gBombKarts[bombKartId];

    sp7E = bombKart->state;

    if (sp7E == 0) {
        return;
    }

    if (((bombKart->unk_4A != 1) || (IsPodiumCeremony()))) {
        var_f22 = bombKart->bombPos[0];
        var_f20 = bombKart->bombPos[1];
        var_f24 = bombKart->bombPos[2];
        spCA = bombKart->waypointIndex;
        spC4 = bombKart->unk_3C;
        spC2 = bombKart->someRot;
        sp7C = bombKart->bounceTimer;
        var_s1 = bombKart->circleTimer;
        if ((sp7E != 0) && (sp7E != 4)) {
            if (1) {}
            if (IsPodiumCeremony()) {
                if (D_8016347E == 1) {
                    var_v0 = gPlayerFour;
                    temp_f0 = var_f22 - var_v0->pos[0];
                    temp_f2 = var_f20 - var_v0->pos[1];
                    temp_f12 = var_f24 - var_v0->pos[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                        var_s1 = 0;
                        sp7E = 4;
                        var_v0->soundEffects |= 0x400000;
                        var_v0->type &= ~0x2000;
                    }
                }
            } else {

                for (var_a0 = 0; var_a0 < gPlayerCount; var_a0++) {
                    var_v0 = &gPlayers[var_a0];
                    if (!(var_v0->effects & 0x80000000)) {
                        temp_f0 = var_f22 - var_v0->pos[0];
                        temp_f2 = var_f20 - var_v0->pos[1];
                        temp_f12 = var_f24 - var_v0->pos[2];
                        if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                            sp7E = 4;
                            var_s1 = 0;
                            if (IsFrappeSnowland()) {
                                var_v0->soundEffects |= 0x01000000;
                            } else {
                                var_v0->soundEffects |= 0x400000;
                            }
                        }
                    }
                }
            }
        }
        switch (sp7E) {
            case 1:
                var_s1 = (var_s1 + 356) % 360;
                temp_t6 = (var_s1 * 0xFFFF) / 360;
                sp118 = coss(temp_t6) * 25.0;
                temp_f0_3 = sins(temp_t6) * 25.0;
                temp_v0_2 = &gTrackPaths[0][spCA];
                var_f22 = temp_v0_2->x + sp118;
                var_f20 = bombKart->yPos + 3.5f;
                var_f24 = temp_v0_2->z + temp_f0_3;
                D_80162FB0[0] = var_f22;
                D_80162FB0[1] = var_f20;
                D_80162FB0[2] = var_f24;
                temp_t7 = (((var_s1 + 1) % 360) * 0xFFFF) / 360;
                sp118 = coss(temp_t7) * 25.0;
                temp_f0_3 = sins(temp_t7) * 25.0;
                D_80162FC0[0] = temp_v0_2->x + sp118;
                D_80162FC0[1] = temp_v0_2->y;
                D_80162FC0[2] = temp_v0_2->z + temp_f0_3;
                spC2 = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            case 2:
                var_s1 = (var_s1 + 4) % 360;
                temp_t6 = (var_s1 * 0xFFFF) / 360;
                sp118 = coss(temp_t6) * 25.0;
                temp_f0_3 = sins(temp_t6) * 25.0;
                temp_v0_2 = &gTrackPaths[0][spCA];
                var_f22 = temp_v0_2->x + sp118;
                var_f20 = bombKart->yPos + 3.5f;
                var_f24 = temp_v0_2->z + temp_f0_3;
                D_80162FB0[0] = var_f22;
                D_80162FB0[1] = var_f20;
                D_80162FB0[2] = var_f24;
                temp_t7 = (((var_s1 + 1) % 360) * 0xFFFF) / 360;
                sp118 = coss(temp_t7) * 25.0;
                temp_f0_3 = sins(temp_t7) * 25.0;
                D_80162FC0[0] = temp_v0_2->x + sp118;
                D_80162FC0[1] = temp_v0_2->y;
                D_80162FC0[2] = temp_v0_2->z + temp_f0_3;
                spC2 = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            case 3:
                var_f20 = bombKart->yPos + 3.5f;
                spC2 = 0;
                break;

            case 5:
                if ((D_8016347C == 0) || (gNearestPathPointByPlayerId[3] < 5)) {
                    break;
                } else {
                    spCA = func_8000D2B4(var_f22, var_f20, var_f24, spCA, 3);
                    if ((spCA < 0) || (gPathCountByPathIndex[3] < spCA)) {
                        spCA = 0;
                    }
                    if (((s32) spCA) < 0x1A) {
                        temp_v0_2 = &gTrackPaths[3][(spCA + 1) % gPathCountByPathIndex[3]];
                        D_80162FB0[0] = temp_v0_2->x;
                        D_80162FB0[1] = temp_v0_2->y;
                        D_80162FB0[2] = temp_v0_2->z;
                        temp_v0_4 = &gTrackPaths[3][(spCA + 2) % gPathCountByPathIndex[3]];
                        D_80162FC0[0] = temp_v0_4->x;
                        D_80162FC0[1] = temp_v0_4->y;
                        D_80162FC0[2] = temp_v0_4->z;
                        spC2 = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                    } else {
                        D_80162FB0[0] = var_f22;
                        D_80162FB0[1] = var_f20;
                        D_80162FB0[2] = var_f24;
                        D_80162FC0[0] = -2409.197f;
                        D_80162FC0[1] = 0.0f;
                        D_80162FC0[2] = -355.254f;
                        spC2 = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                    }
                    temp_f14 = ((D_80162FB0[0] + D_80162FC0[0]) * 0.5f) - var_f22;
                    temp_f16 = ((D_80162FB0[2] + D_80162FC0[2]) * 0.5f) - var_f24;
                    temp_f0_4 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16));
                    if (temp_f0_4 > 0.01f) {
                        var_f22 += (bombKart->unk_3C * temp_f14) / temp_f0_4;
                        var_f24 += (bombKart->unk_3C * temp_f16) / temp_f0_4;
                    } else {
                        var_f22 += temp_f14 / 5.0f;
                        var_f24 += temp_f16 / 5.0f;
                    }
                    temp_a0_4 = &gBombKartCollision[bombKartId];
                    var_f20 = calculate_surface_height(var_f22, 2000.0f, var_f24, temp_a0_4->meshIndexZX) + 3.5f;
                    if (var_f20 < (-1000.0)) {
                        var_f20 = bombKart->bombPos[1];
                    }
                    check_bounding_collision(temp_a0_4, 10.0f, var_f22, var_f20, var_f24);
                }
                break;
            case 4:
                temp_v0_2 = &gTrackPaths[0][spCA];
                D_80162FB0[0] = temp_v0_2->x;
                D_80162FB0[1] = temp_v0_2->y;
                D_80162FB0[2] = temp_v0_2->z;
                temp_v0_4 = &gTrackPaths[0][(spCA + 1) % gPathCountByPathIndex[0]];
                D_80162FC0[0] = temp_v0_4->x;
                D_80162FC0[1] = temp_v0_4->y;
                D_80162FC0[2] = temp_v0_4->z;
                var_f20 += 3.0f - (var_s1 * 0.3f);
                spC2 = (get_angle_between_two_vectors(D_80162FB0, D_80162FC0) * 0xFFFF) / 65520;
                break;
            default:
                break;
        }

        if (sp7E == 4) {
            sp108 = 2.0f * var_s1;
            sp118 = coss(0xFFFF - spC2) * var_s1;
            var_f18 = sins(0xFFFF - spC2) * var_s1;
            var_s1++;
            temp_f2_4 = (var_f20 - 2.3f) + (sp108 / 3.0f);
            spAC = temp_f2_4;
            spA0 = temp_f2_4;
            sp94 = temp_f2_4;
            sp88 = temp_f2_4;
            if (var_s1 >= 31) {
                sp7E = 0;
            }
        } else {
            sp118 = coss(0xFFFF - spC2) * 1.5f;
            var_f18 = sins(0xFFFF - spC2) * 1.5f;
            temp_f16_2 = var_f20 - 2.3f;
            temp_f12_3 = (sp7C % 3) * 0.15f;
            temp_f14_2 = temp_f16_2 - temp_f12_3;
            temp_f12_4 = temp_f16_2 + temp_f12_3;
            spAC = temp_f14_2;
            sp94 = temp_f14_2;
            spA0 = temp_f12_4;
            sp88 = temp_f12_4;
            var_f20 += sins((sp7C * 0x13FFEC) / 360);
            sp7C = (sp7C + 1) % 18;
        }
        bombKart2 = bombKart;
        bombKart2->wheel1Pos[0] = (sp118 - var_f18) + var_f22;
        bombKart2->wheel1Pos[1] = spAC;
        bombKart2->wheel1Pos[2] = (var_f18 + sp118) + var_f24;
        bombKart2->wheel2Pos[0] = (var_f18 + sp118) + var_f22;
        bombKart2->wheel2Pos[1] = spA0;
        bombKart2->wheel2Pos[2] = (var_f18 - sp118) + var_f24;
        bombKart2->wheel3Pos[0] = ((-sp118) - var_f18) + var_f22;
        bombKart2->wheel3Pos[1] = sp94;
        bombKart2->wheel3Pos[2] = ((-var_f18) + sp118) + var_f24;
        bombKart2->wheel4Pos[0] = ((-sp118) + var_f18) + var_f22;
        bombKart2->wheel4Pos[1] = sp88;
        bombKart2->wheel4Pos[2] = ((-var_f18) - sp118) + var_f24;
        bombKart2->bombPos[0] = var_f22;
        bombKart2->bombPos[1] = var_f20;
        bombKart2->bombPos[2] = var_f24;
        bombKart2->waypointIndex = spCA;
        bombKart2->unk_3C = spC4;
        bombKart2->someRot = spC2;
        bombKart2->state = sp7E;
        bombKart2->bounceTimer = sp7C;
        bombKart2->circleTimer = var_s1;
    }
}

s32 add_actor_in_unexpired_actor_list(s32 actorIndex, s16 arg1) {
    s32 i;
    s32 a2 = 0;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if (gUnexpiredActorsList[i].unkC == 0) {
            gUnexpiredActorsList[i].unkC = 1;
            gUnexpiredActorsList[i].actorIndex = actorIndex;
            gUnexpiredActorsList[i].unk10 = arg1;
            gUnexpiredActorsList[i].unk14 = 0;
            a2 = 1;
            break;
        }
    }
    if (a2 == 0) {
        return -2;
    }
    return 0;
}

s32 add_red_shell_in_unexpired_actor_list(s32 actorIndex) {
    struct Actor* actor = CM_GetActor(actorIndex);
    if (actor->type != ACTOR_RED_SHELL) {
        return -1;
    }
    return add_actor_in_unexpired_actor_list(actorIndex, 0);
}

s32 add_green_shell_in_unexpired_actor_list(s32 actorIndex) {
    struct Actor* actor = CM_GetActor(actorIndex);
    if (actor->type != ACTOR_GREEN_SHELL) {
        return -1;
    }
    return add_actor_in_unexpired_actor_list(actorIndex, 1);
}

s32 add_blue_shell_in_unexpired_actor_list(s32 arg0) {
    struct Actor* actor = CM_GetActor(arg0);
    if (actor->type != ACTOR_BLUE_SPINY_SHELL) {
        return -1;
    }
    return add_actor_in_unexpired_actor_list(arg0, 2);
}

void delete_actor_in_unexpired_actor_list(s32 actorIndex) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &gUnexpiredActorsList[i];
        if (actorIndex == phi->actorIndex) {
            phi->unkC = 0;
            phi->actorIndex = 1000; // out of bounds
        }
    }
}

void func_8000EEDC(void) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &gUnexpiredActorsList[i];
        phi->unkC = 0;
        phi->actorIndex = 1000; // out of bounds
    }
}

void generate_player_smoke(void) {
    s32 someIndex;
    f32 var_f20;
    struct Actor* temp_s1;
    struct unexpiredActors* var_s0;

    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        var_s0 = &gUnexpiredActorsList[someIndex];
        if (var_s0->unkC == 1) {
            temp_s1 = GET_ACTOR(var_s0->actorIndex);
            var_s0->unk14++;
            switch (var_s0->unk10) {
                case 0:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.3f;
                    } else {
                        var_f20 = 0.9f;
                    }
                    break;
                case 1:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.15f;
                    } else {
                        var_f20 = 0.45f;
                    }
                    break;
                case 2:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.15f;
                    } else {
                        var_f20 = 0.45f;
                    }
                    break;
                default:
                    var_f20 = 1.0f;
                    break;
            }
            if (!(var_s0->unk14 & 1)) {
                init_smoke_particle(temp_s1->pos, ((random_int(30) + 20) * var_f20) / 50.0f, var_s0->unk10);
            }
        }
    }
}

void func_8000F0E0(void) {
    s32 i;
    for (i = 0; i < NUM_CAMERAS; i++) {
        D_80164670[i] = 0;
        D_80164678[i] = 0;
    }
}

void func_8000F124(void) {
    s32 var_a1;
    s32 i, j;

    for (j = 0; j < 2; j++) {
        D_80163348[j] = 0;
    }

    if (gDemoMode == 1) {
        return;
    }
    if (gModeSelection != GRAND_PRIX) {
        return;
    }

    for (i = 0; i < 2; i++) {

        while (1) {
            D_80163348[i] = random_int(8);

            if (gPlayerCount > 2) {
                break;
            }
            if (gPlayerCount < 1) {
                break;
            }

            var_a1 = 0;

            for (j = 0; j < gPlayerCount; j++) {
                if (gCharacterSelections[j] == D_80163348[i]) {
                    var_a1 = 1;
                }
            }
            for (j = 0; j < i; j++) {
                if (D_80163348[j] == D_80163348[i]) {
                    var_a1 = 1;
                }
            }
            if (var_a1 == 0) {
                break;
            }
        }
    }
}

// Delete track waypoints
void clear_path_point(TrackPathPoint* arg0, size_t size) {
    bzero((void*) arg0, size * sizeof(TrackPathPoint));
}

// Appears to allocate memory for each course.
void init_course_path_point(void) {

    _struct_gCoursePathSizes_0x10* ptr = &CM_GetProps()->PathSizes;
    s32 temp;
    s32 i;

    D_80163368[0] = (s32) ptr->unk0;
    D_80163368[1] = (s32) ptr->unk2;
    D_80163368[2] = (s32) ptr->unk4;
    D_80163368[3] = (s32) ptr->unk6;

    temp = ptr->unk8;
    gVehicle2DPathPoint = get_next_available_memory_addr(temp * sizeof(TrackPathPoint));
    gVehiclePath = gVehicle2DPathPoint;

    // Podium ceremony appears to allocate 1 * 8 bytes of data. Which would be aligned to 0x10.
    for (i = 0; i < 4; i++) {
        gTrackPaths[i] = get_next_available_memory_addr(D_80163368[i] * sizeof(TrackPathPoint));
        gTrackLeftPaths[i] = get_next_available_memory_addr(D_80163368[i] * sizeof(TrackPathPoint));
        gTrackRightPaths[i] = get_next_available_memory_addr(D_80163368[i] * sizeof(TrackPathPoint));
        gTrackSectionTypes[i] = get_next_available_memory_addr(D_80163368[i] * 2);
        gPathExpectedRotation[i] = get_next_available_memory_addr(D_80163368[i] * 2);
        gTrackConsecutiveCurveCounts[i] = get_next_available_memory_addr(D_80163368[i] * 2);
    }

    gCurrentTrackPath = gTrackPaths[0];
    gCurrentTrackLeftPath = gTrackLeftPaths[0];
    gCurrentTrackRightPath = gTrackRightPaths[0];
    gCurrentTrackSectionTypesPath = gTrackSectionTypes[0];
    gCurrentPathPointExpectedRotationPath = gPathExpectedRotation[0];
    gCurrentTrackConsecutiveCurveCountsPath = gTrackConsecutiveCurveCounts[0];

    // zero allocated memory?
    //! @warning does not appear to zero all the above allocated variables.
    for (i = 0; i < 4; i++) {
        clear_path_point(gTrackPaths[i], D_80163368[i]);
        clear_path_point(gTrackLeftPaths[i], D_80163368[i]);
        clear_path_point(gTrackRightPaths[i], D_80163368[i]);
    }

    // Skip several cpu cycles.
    for (i = 0; i < 4; i++) {}

    for (i = 0; i < 4; i++) {
        if (D_80163368[i] >= 2) {
            load_track_path(i);
            calculate_track_boundaries(i);
            analyze_track_section(i);
            analyse_angle_path(i);
            analyze_curved_path(i);
        }
    }

    gSelectedPathCount = *gPathCountByPathIndex;

    CM_ClearVehicles();

    // set_bomb_kart_spawn_positions();
    func_8000EEDC();
}

void init_players(void) {

    UNUSED Camera* camera;
    s32 temp_v0_3;
    s32 i;
    TrackPositionFactorInstruction* var_s5;
    UNUSED s32 temp_v1;
    UNUSED s32 pad;

    for (i = 0; i < NUM_PLAYERS; i++) {
        Player* player = &gPlayerOne[i];

        gPreviousAngleSteering[i] = 0;
        D_80162FF8[i] = 0;
        D_80163010[i] = 0;
        if (!IsPodiumCeremony()) {
            update_player_position_factor(i, 0, 0);
        }
        //! todo: @BUG this doesn't seem right. This variable is metadata.
        cpu_TargetSpeed[i] = CM_GetProps()->CurveTargetSpeed[gCCSelection];
        D_801630E8[i] = 0;
        D_80163100[i] = 0;
        gPreviousPlayerAiOffsetX[i] = 0.0f;
        gPreviousPlayerAiOffsetZ[i] = 0.0f;
        gPreviousCpuTargetSpeed[i] = 0.0f;
        gLapCountByPlayerId[i] = -1;
        gCourseCompletionPercentByPlayerId[i] = 0.0f;
        gTimePlayerLastTouchedFinishLine[i] = 0.0f;
        if (gModeSelection == GRAND_PRIX) {
            if (1) {};
            if (1) {}; // Maybe some debug code?
            gGPCurrentRaceRankByPlayerId[i] = (s32) D_80165270[i];
            gPreviousGPCurrentRaceRankByPlayerId[i] = (s32) D_80165270[i];
        } else {
            gGPCurrentRaceRankByPlayerId[i] = i;
            gPreviousGPCurrentRaceRankByPlayerId[i] = i;
        }
        temp_v0_3 = gGPCurrentRaceRankByPlayerId[i];
        gGPCurrentRacePlayerIdByRank[temp_v0_3] = (s16) i;
        D_80164378[temp_v0_3] = (s16) i;
        gGPCurrentRaceRankByPlayerIdDup[i] = temp_v0_3;
        gWrongDirectionCounter[i] = 0;
        gIsPlayerWrongDirection[i] = 0;
        D_801631E0[i] = 0;
        D_801631F8[i] = 0;
        gNumPathPointsTraversed[i] = -20;
        gPreviousLapProgressScore[i] = -20;
        gGetPlayerByCharacterId[gPlayers[i].characterId] = (s16) i;
        gTrackPositionFactor[i] = 0.0f;
        D_80163090[i] = 0.0f;
        var_s5 = &gPlayerTrackPositionFactorInstruction[i];
        var_s5->unkC = CM_GetProps()->AIMinimumSeparation * (f32) (((i + 1) % 3) - 1);
        var_s5->target = var_s5->unkC;
        var_s5->current = 0.0f;
        var_s5->step = 0.015f;
        reset_cpu_behaviour_none(i);
        gSpeedCPUBehaviour[i] = 0;
        bInMultiPathSection[i] = 0;
        D_80163398[i] = 0;
        D_801633B0[i] = 0;
        D_801633C8[i] = 0;
        D_801633F8[i] = 0;
        gPlayersTrackSectionId[i] = 0;
        gPreviousPlayerZ[i] = player->pos[2];
        gCurrentPlayerLookAhead[i] = 6;
        if (gPlayers[i].type & PLAYER_HUMAN) {
            D_80163330[i] = 3;

        } else {
            D_80163330[i] = 0;
        }

        cpu_enteringPathIntersection[i] = 0;
        cpu_exitingPathIntersection[i] = 0;
        D_80163128[i] = -1;
        D_80163150[i] = -1;
        D_80164538[i] = -1;
        D_801634C0[i] = 0;
        bStopAICrossing[i] = 0;
        gIsPlayerInCurve[i] = true;
    }

#ifdef AVOID_UB
    for (i = 0; i < NUM_CROSSINGS; i++) {
        sCrossingActiveTimer[i] = 0;
    }
#else
    sCrossingActiveTimer[0] = 0;
    sCrossingActiveTimer[1] = 0;
#endif
    if (gDemoMode == DEMO_MODE_INACTIVE) {

        if (gModeSelection == GRAND_PRIX) {
            for (i = 0; i < 2; i++) {
                D_80163344[i] = gGetPlayerByCharacterId[D_80163348[i]];
                D_80163330[D_80163344[i]] = 1;
                D_8016334C[D_80163344[i]] = i;
            }
        }
    }
    if ((gDemoUseController == 1) && (!IsPodiumCeremony())) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            D_80163330[i] = 0;
        }
        if (gModeSelection == VERSUS) {

            D_80163344[0] = cameras->playerId;
            D_80163330[D_80163344[0]] = 1;
            D_8016334C[D_80163344[0]] = 0;

            for (i = 1; i < 2; i++) {
                D_80163344[i] = cameras[i].playerId;
                D_80163330[D_80163344[i]] = 1;
                D_8016334C[D_80163344[i]] = i;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        if (1) {};
        gPathIndexByPlayerId[i] = 0;
        gNearestPathPointByPlayerId[i] =
            gPathCountByPathIndex[gPathIndexByPlayerId[i]] - gGPCurrentRaceRankByPlayerId[i] - 4;
    }

    gIsInExtra = 0;
    if (gCCSelection == CC_EXTRA) {
        gIsInExtra = 1;
    }

    for (i = 0; i < 30; i++) {
        D_80162F10[i] = -1;
        D_80162F50[i] = -1;
    }

    D_801631CC = 100000; // 100,000
    D_80164698 = 0.0f;
    D_8016469C = 100.0f;
    D_801646A0 = 0.0f;
    D_80164358 = 0;
    D_8016435A = 1;
    D_8016435C = 1;
    gBestRankedHumanPlayer = 0;
    gIncrementUpdatePlayer = 0;
    D_8016337C = 0;
    gPathStartZ = (f32) gTrackPaths[0][0].z; // [i][2]
    D_801634F0 = 0;
    D_801634F4 = 0;
    D_80163488 = 0;
    D_8016348C = 0;
    D_801634EC = 0;
    func_8001AB00();
    if (gDemoUseController == 1) {
        if (gDemoMode == 1) {

            for (i = 0; i < NUM_PLAYERS; i++) {
                if (D_80163330[i] == 1) {
                    gPlayers[i].soundEffects |= 0x02000000;
                }
            }
        }
    }
    copy_courses_cpu_behaviour();
}

// @arg index from 0 to 3.

// Processes course path by index.
// @arg index from 0 to 3.
// Each course can have 1-4 course paths.
void load_track_path(s32 pathIndex) {

    TrackPathPoint* ptr;
    TrackPathPoint* pathDest;
    TrackPathPoint* path;
    s32 var_v0;
    s32 sp24;
    UNUSED s32 pad[2];
    s16 bInvalidPath;
    s32 i;

    if (CM_GetProps()->AIMaximumSeparation >= 1.0f) {
        pathDest = gTrackPaths[pathIndex];
        bInvalidPath = 1;

        if (IsPodiumCeremony()) { // Only podium ceremony
            // Course path included in course_data which has already been loaded into memory.
            // This is how we get the addr to our path data.
            path = CM_GetProps()->PathTable2[pathIndex];
            ptr = path;

            for (i = 0; i < 3000; i++, ptr++) {
                if ((u16) ptr->x == 0x8000) {
                    sp24 = i - 1;
                    bInvalidPath = 0;
                    break;
                }
            }

            // If path data higher than 3000 something has gone wrong.
            // Skip processing the data.
            //! @todo Confirm this comment
            if (!bInvalidPath) {
                var_v0 = func_80011014(pathDest, path, sp24, pathIndex);
                gPathCountByPathIndex[pathIndex] = (u16) var_v0;
            } else {
                printf("PathTable is invalid. It has %d path points\n  It may also be missing the end tag.\n", i);
            }
        } else { // ALL TRACKS
            TrackPathPoint* pathSrc = CM_GetProps()->PathTable2[pathIndex];
            if (pathSrc == NULL) {
                printf("code_80005FD0.c: Path %d in Course::PathTable2, was NULL.\n  Your track is missing a path\n",
                       pathIndex);
            }

            var_v0 = process_path_data(pathDest, pathSrc);
            gPathCountByPathIndex[pathIndex] = (u16) var_v0;
        }
    }
}

void calculate_track_boundaries(s32 pathIndex) {
    f32 waypointWidth;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 x_dist;
    f32 z_dist;
    f32 neg_x_dist;
    f32 neg_z_dist;
    f32 xz_dist;
    s32 temp_f16;
    s32 waypointIndex;
    TrackPathPoint* waypoint;
    TrackPathPoint* nextWaypoint;
    TrackPathPoint* var_s1;
    TrackPathPoint* var_s2;

    if (CM_GetProps()->AIMaximumSeparation >= 0.0f) {
        waypointWidth = CM_GetProps()->AIMaximumSeparation;
        waypoint = &gTrackPaths[pathIndex][0];
        var_s1 = &gTrackLeftPaths[pathIndex][0];
        var_s2 = &gTrackRightPaths[pathIndex][0];
        for (waypointIndex = 0; waypointIndex < gPathCountByPathIndex[pathIndex]; waypointIndex++, var_s1++, var_s2++) {
            x1 = waypoint->x;
            y1 = waypoint->y;
            z1 = waypoint->z;
            waypoint++;
            nextWaypoint = &gTrackPaths[pathIndex][(waypointIndex + 1) % ((s32) gPathCountByPathIndex[pathIndex])];
            x2 = nextWaypoint->x;
            y2 = nextWaypoint->y;
            z2 = nextWaypoint->z;
            x_dist = x2 - x1;
            z_dist = z2 - z1;
            neg_x_dist = x1 - x2;
            neg_z_dist = z1 - z2;
            xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
            temp_f16 = (f32) ((y1 + y2) * 0.5);
            var_s1->x = ((waypointWidth * z_dist) / xz_dist) + x1;
            var_s1->y = temp_f16;
            var_s1->z = ((waypointWidth * neg_x_dist) / xz_dist) + z1;
            var_s2->x = ((waypointWidth * neg_z_dist) / xz_dist) + x1;
            var_s2->y = temp_f16;
            var_s2->z = ((waypointWidth * x_dist) / xz_dist) + z1;
        }
    }
}

f32 calculate_track_curvature(s32 pathIndex, u16 waypointIndex) {
    f32 temp_f10_2;
    f32 temp_f8;
    UNUSED f32 pad;
    TrackPathPoint* pathWaypoints;
    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;
    f32 temp_f8_2;
    f32 temp_f10;
    s32 waypointCount;
    TrackPathPoint* waypoint3;
    TrackPathPoint* waypoint2;
    TrackPathPoint* waypoint1;
    f32 root2;
    f32 root1;

    if (CM_GetProps()->AIMaximumSeparation <= -1.0f) {
        return 0.0f;
    }
    waypointCount = gPathCountByPathIndex[pathIndex];
    pathWaypoints = gTrackPaths[pathIndex];
    waypoint1 = &pathWaypoints[waypointIndex];
    waypoint2 = &pathWaypoints[(waypointIndex + 1) % waypointCount];
    waypoint3 = &pathWaypoints[(waypointIndex + 2) % waypointCount];
    x1 = waypoint1->x;
    z1 = waypoint1->z;
    x2 = waypoint2->x;
    z2 = waypoint2->z;
    x3 = waypoint3->x;
    z3 = waypoint3->z;
    temp_f8_2 = (((x2 + x3) * 0.5) - x1);
    temp_f10 = (((z2 + z3) * 0.5) - z1);
    waypoint1 = &pathWaypoints[(waypointIndex + 3) % waypointCount];
    waypoint2 = &pathWaypoints[(waypointIndex + 4) % waypointCount];
    waypoint3 = &pathWaypoints[(waypointIndex + 5) % waypointCount];
    x1 = waypoint1->x;
    z1 = waypoint1->z;
    x2 = waypoint2->x;
    z2 = waypoint2->z;
    x3 = waypoint3->x;
    z3 = waypoint3->z;
    temp_f10_2 = (((x2 + x3) * 0.5) - x1);
    temp_f8 = (((z2 + z3) * 0.5) - z1);
    root1 = sqrtf((temp_f10 * temp_f10) + (temp_f8_2 * temp_f8_2));
    root2 = sqrtf((temp_f10_2 * temp_f10_2) + (temp_f8 * temp_f8));
    return -((temp_f10 * temp_f10_2) - (temp_f8_2 * temp_f8)) / (root2 * root1);
}

void analyze_track_section(s32 pathIndex) {
    f64 sectionCurvature;
    UNUSED s32 pad;
    s32 k;
    s32 i;
    s32 j;
    s16* currentSection;
    s32 pathPointCount;
    s16* nextSection;

    if ((s32) CM_GetProps()->AIMaximumSeparation >= 0) {
        pathPointCount = gPathCountByPathIndex[pathIndex];
        currentSection = gTrackSectionTypes[pathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            sectionCurvature = calculate_track_curvature(pathIndex, i);
            *currentSection = STRAIGHT;
            if (sectionCurvature > 0.1) {
                *currentSection = RIGHT_CURVE;
            }
            if (sectionCurvature < -0.1) {
                *currentSection = LEFT_CURVE;
            }
        }
        currentSection = gTrackSectionTypes[pathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            if (*currentSection == STRAIGHT) {
                // Look ahead for next curved section
                for (j = 1; j < pathPointCount; j++) {
                    nextSection = &gTrackSectionTypes[pathIndex][(i + j) % pathPointCount];
                    switch (*nextSection) {
                        case RIGHT_LEANING_CURVE:
                        case RIGHT_CURVE:
                            for (k = 0; k < j; k++) {
                                gTrackSectionTypes[pathIndex][(i + k) % pathPointCount] = RIGHT_LEANING_CURVE;
                            }
                            i += j;
                            currentSection += j;
                            j = pathPointCount;
                            break;
                        case LEFT_LEANING_CURVE:
                        case LEFT_CURVE:
                            for (k = 0; k < j; k++) {
                                gTrackSectionTypes[pathIndex][(i + k) % pathPointCount] = LEFT_LEANING_CURVE;
                            }
                            i += j;
                            currentSection += j;
                            j = pathPointCount;
                            break;
                    }
                }
            }
        }
    }
}

// Seemingly calculates the atan2 angle between a waypoint and its forward neighbor
s16 calculate_angle_path(s32 pathIndex, s32 waypointIndex) {
    s16 ret;
    Vec3f sp30;
    Vec3f sp24;
    TrackPathPoint* temp_v0;

    temp_v0 = &gTrackPaths[pathIndex][waypointIndex];
    sp30[0] = temp_v0->x;
    sp30[1] = temp_v0->y;
    sp30[2] = temp_v0->z;
    temp_v0 = &gTrackPaths[pathIndex][(waypointIndex + 1) % gPathCountByPathIndex[pathIndex]];
    sp24[0] = temp_v0->x;
    sp24[1] = temp_v0->y;
    sp24[2] = temp_v0->z;
    ret = get_angle_between_two_vectors(sp30, sp24);
    return -ret;
}

// Populates gPathExpectedRotation
void analyse_angle_path(s32 pathIndex) {
    s32 waypointIndex;
    u16* angle;

    if (CM_GetProps()->AIMaximumSeparation >= 0.0f) {
        for (angle = (u16*) &gPathExpectedRotation[pathIndex][0], waypointIndex = 0;
             waypointIndex < gPathCountByPathIndex[pathIndex]; waypointIndex++, angle++) {
            *angle = calculate_angle_path(pathIndex, waypointIndex);
        }
    }
}

void analyze_curved_path(s32 pathIndex) {
    s16* trackCurveCount;
    s16 curveCount;
    s16 temp_t0;
    s32 pathPointCount;
    s16* trackSectionType;
    s32 i, j;

    pathPointCount = gPathCountByPathIndex[pathIndex];
    trackSectionType = gTrackSectionTypes[pathIndex];
    trackCurveCount = gTrackConsecutiveCurveCounts[pathIndex];

    for (i = 0; i < pathPointCount; i++) {
        curveCount = 0;
        for (j = 0; j < pathPointCount; j++) {
            temp_t0 = trackSectionType[(i + j) % pathPointCount];
            if ((temp_t0 == LEFT_LEANING_CURVE) || (temp_t0 == RIGHT_LEANING_CURVE)) {
                curveCount += 1;
            } else {
                break;
            }
            if (IsPodiumCeremony()) {
                break;
            }
        }
        *trackCurveCount = curveCount;
        trackCurveCount++;
    }
}

f32 func_80010F40(f32 arg0, f32 arg1, f32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    arg1 = spawn_actor_on_surface(arg0, 2000.0f, arg2);
    check_bounding_collision(&D_80162E70, 1.0f, arg0, arg1, arg2);
    return arg1;
}

f32 func_80010FA0(f32 arg0, f32 arg1, f32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    arg1 = spawn_actor_on_surface(arg0, (f32) ((f64) arg1 + 30.0), arg2);
    check_bounding_collision(&D_80162E70, 10.0f, arg0, arg1, arg2);
    return arg1;
}

/**
 * @return's the number of items processed.
 */
s32 func_80011014(TrackPathPoint* pathDest, TrackPathPoint* path, s32 numPathPoints, UNUSED s32 pathIndex) {
    f32 temp_f24_2;
    f32 temp_f2_3;
    f32 var_f20_2;
    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;
    f32 x1_2;
    f32 z1_3;
    f32 temp_f20;
    f32 var_f30;
    f32 temp_f22;
    f32 temp_f16;
    s32 i;

    f32 j;
    s32 var_s0;
    f32 var_f28;

    TrackPathPoint* point1;
    TrackPathPoint* point2;
    TrackPathPoint* point3;
    f32 temp;
    UNUSED TrackPathPoint* dest;
    var_f30 = 0.0f;
    var_s0 = 0;
    temp_f20 = (f32) path[0].x;
    temp_f22 = (f32) path[0].z;
    var_f28 = func_80010F40(temp_f20, 2000.0f, temp_f22, 0, 0);

    for (i = 0; i < numPathPoints; i++) {
        point1 = &path[i % numPathPoints];
        point2 = &path[(i + 1) % numPathPoints];
        point3 = &path[(s32) (i + 2) % numPathPoints];
        x1 = (f32) point1->x;
        z1 = (f32) point1->z;
        x2 = (f32) point2->x;
        z2 = (f32) point2->z;
        x3 = (f32) point3->x;
        z3 = (f32) point3->z;

        temp = 0.05 / (sqrtf(((x2 - x1) * (x2 - x1)) + ((z2 - z1) * (z2 - z1))) +
                       (sqrtf(((x3 - x2) * (x3 - x2)) + ((z3 - z2) * (z3 - z2)))));

        for (j = 0.0f; j <= 1.0; j += temp) {

            temp_f2_3 = (f32) ((1.0 - j) * 0.5 * (1.0 - j));
            z1_3 = (f32) (((1.0 - j) * j) + 0.5);
            temp_f16 = (f32) (j * 0.5 * j);

            temp_f24_2 = (temp_f2_3 * x1) + (z1_3 * x2) + (temp_f16 * x3);
            x1_2 = (temp_f2_3 * z1) + (z1_3 * z2) + (temp_f16 * z3);

            var_f30 +=
                sqrtf(((temp_f24_2 - temp_f20) * (temp_f24_2 - temp_f20)) + ((x1_2 - temp_f22) * (x1_2 - temp_f22)));

            temp_f20 = temp_f24_2;
            temp_f22 = x1_2;

            if ((var_f30 > 20.0f) || ((i == 0) && (j == 0.0))) {
                if (gIsMirrorMode) {
                    // temp_f12 = -temp_f24_2;
                    pathDest->x = (s16) -temp_f24_2;
                    var_f20_2 = func_80010FA0(-temp_f24_2, var_f28, x1_2, 0, var_s0);
                } else {
                    pathDest->x = (s16) temp_f24_2;
                    var_f20_2 = func_80010FA0(temp_f24_2, var_f28, x1_2, 0, var_s0);
                }

                pathDest->z = (s16) temp_f22;
                pathDest->trackSectionId = get_track_section_id(D_80162E70.meshIndexZX);

                if (var_f20_2 < -500.0) {
                    var_f20_2 = var_f28;
                } else {

                    if (IsRainbowRoad()) {
                        if (var_f20_2 < (var_f28 - 15.0)) {
                            var_f20_2 = (f32) var_f28 - 15.0;
                        }
                    } else if (IsWarioStadium()) {
                        if ((var_s0 >= 1140) && (var_s0 <= 1152)) {
                            var_f20_2 = var_f28;
                        } else {
                            if (var_f20_2 < (var_f28 - 10.0)) {
                                var_f20_2 = (f32) (var_f28 - 4.0);
                            }
                        }
                    } else if (IsDkJungle()) {
                        if ((var_s0 > 204) && (var_s0 < 220)) {
                            var_f20_2 = var_f28;
                        } else {
                            if (var_f20_2 < (var_f28 - 10.0)) {
                                var_f20_2 = (f32) (var_f28 - 4.0);
                            }
                        }
                    } else {
                        if (var_f20_2 < (var_f28 - 10.0)) {
                            var_f20_2 = (f32) var_f28 - 10.0;
                        }
                    }
                }
                var_f28 = var_f20_2;
                pathDest->y = (s16) (s32) var_f20_2;
                var_f30 = 0.0f;
                pathDest++;
                var_s0 += 1;
            }
        }
    }
    return var_s0;
}

// Returns number of waypoints processed.
s32 process_path_data(TrackPathPoint* dest, TrackPathPoint* src) {
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_a3;
    s32 var_v0;
    s32 var_v1;
    u16 temp_t0;

    var_v1 = 0;
    for (var_v0 = 0; var_v0 < 0x7D0; var_v0++) {
        temp_a0 = src->x;
        temp_a2 = src->y;
        temp_a3 = src->z;
        temp_t0 = src->trackSectionId;
        src++;
        if (((temp_a0 & 0xFFFF) == 0x8000) && ((temp_a2 & 0xFFFF) == 0x8000) && ((temp_a3 & 0xFFFF) == 0x8000)) {
            break;
        }
        if (gIsMirrorMode != 0) {
            dest->x = -temp_a0;
        } else {
            dest->x = temp_a0;
        }
        var_v1++;
        dest->y = temp_a2;
        dest->z = temp_a3;
        dest->trackSectionId = temp_t0;
        dest++;
    }
    return var_v1;
}

s32 generate_2d_path(TrackPathPoint* pathDest, TrackPathPoint* pathSrc, s32 numWaypoints) {
    f32 temp_f14_3;
    f32 temp_f16_2;
    UNUSED s32 pad;

    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;

    UNUSED s32 pad2;
    f32 temp_f24;

    f32 spA8;
    f32 temp_f26;
    f32 spA0;

    f32 temp_f2_3;

    TrackPathPoint* point1;
    f32 j;
    TrackPathPoint* point2;
    TrackPathPoint* point3;
    s32 i;
    f32 temp_f6 = 0.0f;
    s32 nbElement;
    f32 sp7C;

    spA8 = pathSrc[0].x;
    spA0 = pathSrc[0].z;
    nbElement = 0;

    for (i = 0; i < numWaypoints; i++) {
        point1 = &pathSrc[((i % numWaypoints))];
        point2 = &pathSrc[(((i + 1) % numWaypoints))];
        point3 = &pathSrc[(((i + 2) % numWaypoints))];
        x1 = point1->x;
        z1 = point1->z;
        x2 = point2->x;
        z2 = point2->z;
        x3 = point3->x;
        z3 = point3->z;

        sp7C = 0.05 / (sqrtf(((x2 - x1) * (x2 - x1)) + ((z2 - z1) * (z2 - z1))) +
                       sqrtf(((x3 - x2) * (x3 - x2)) + ((z3 - z2) * (z3 - z2))));

        for (j = 0.0f; j <= 1.0; j += sp7C) {
            temp_f2_3 = (1.0 - j) * 0.5 * (1.0 - j);
            temp_f14_3 = ((1.0 - j) * j) + 0.5;
            temp_f16_2 = j * 0.5 * j;

            temp_f24 = (temp_f2_3 * x1) + (temp_f14_3 * x2) + (temp_f16_2 * x3);
            temp_f26 = (temp_f2_3 * z1) + (temp_f14_3 * z2) + (temp_f16_2 * z3);
            temp_f6 += sqrtf(((temp_f24 - spA8) * (temp_f24 - spA8)) + ((temp_f26 - spA0) * (temp_f26 - spA0)));
            spA8 = temp_f24;
            spA0 = temp_f26;
            if ((temp_f6 > 20.0f) || ((i == 0) && (j == 0.0))) {
                if (gIsMirrorMode) {
                    pathDest->x = (s16) -spA8;
                } else {
                    pathDest->x = (s16) spA8;
                }
                pathDest->z = spA0;
                nbElement += 1;
                pathDest++;
                temp_f6 = 0.0f;
            }
        }
    }
    return nbElement;
}

void copy_courses_cpu_behaviour(void) {
    s32 i;
    for (i = 0; i < NUM_COURSES - 1; i++) {
        gCoursesCPUBehaviour[i] = LOAD_ASSET(CM_GetProps()->AIBehaviour);
    }
}

void reset_cpu_behaviour_none(s32 playerIndex) {
    gCurrentCPUBehaviourId[playerIndex] = 0;
    gPreviousCPUBehaviourId[playerIndex] = 0;
    cpu_BehaviourState[playerIndex] = CPU_BEHAVIOUR_STATE_NONE;
}

void reset_cpu_behaviour(s32 playerIndex) {
    gCurrentCPUBehaviourId[playerIndex] = 0;
    gPreviousCPUBehaviourId[playerIndex] = 0;
    cpu_BehaviourState[playerIndex] = CPU_BEHAVIOUR_STATE_START;
}

void cpu_behaviour_start(s32 playerId, Player* player) {
    u16 playerWaypoint;
    s16 pathPointStart;
    s16 pathPointEnd;
    s32 behaviourType;
    UNUSED s32 test;

    CPUBehaviour* beh = (CPUBehaviour*) LOAD_ASSET(CM_GetProps()->AIBehaviour);

    sCurrentCPUBehaviour = &((CPUBehaviour*) LOAD_ASSET(CM_GetProps()->AIBehaviour))[gCurrentCPUBehaviourId[playerId]];

    playerWaypoint = gNearestPathPointByPlayerId[playerId];

    pathPointStart = sCurrentCPUBehaviour->pathPointStart;
    pathPointEnd = sCurrentCPUBehaviour->pathPointEnd;
    behaviourType = sCurrentCPUBehaviour->type;

    if ((pathPointStart == -1) && (pathPointEnd == -1)) {
        sCurrentCPUBehaviour = &((CPUBehaviour*) LOAD_ASSET(CM_GetProps()->AIBehaviour))[0];
        reset_cpu_behaviour_none(playerId);
        return;
    }
    if ((u32) playerWaypoint == (u32) pathPointStart) {
        cpu_BehaviourState[playerId] = CPU_BEHAVIOUR_STATE_RUNNING;
        gPreviousCPUBehaviourId[playerId] = gCurrentCPUBehaviourId[playerId];
        gCurrentCPUBehaviourId[playerId]++;
        switch (behaviourType) {
            case BEHAVIOUR_1:
                func_80011EC0(playerId, player, player->unk_07C >> 0x10, playerWaypoint);
                break;
            case BEHAVIOUR_HOP:
                kart_hop(player);
                player->effects &= ~0x10;
                D_801630E8[playerId] = 0;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
                gPlayerTrackPositionFactorInstruction[playerId].target = 0.0f;
                break;
            case BEHAVIOUR_DRIVE_LEFT:
                gPlayerTrackPositionFactorInstruction[playerId].target = -0.6f;
                break;
            case BEHAVIOUR_DRIVE_OUTER:
                gPlayerTrackPositionFactorInstruction[playerId].target = 0.6f;
                break;
            case BEHAVIOUR_NORMAL_SPEED:
                gSpeedCPUBehaviour[playerId] = SPEED_CPU_BEHAVIOUR_NORMAL;
                break;
            case BEHAVIOUR_FAST_SPEED:
                gSpeedCPUBehaviour[playerId] = SPEED_CPU_BEHAVIOUR_FAST;
                break;
            case BEHAVIOUR_SLOW_SPEED:
                gSpeedCPUBehaviour[playerId] = SPEED_CPU_BEHAVIOUR_SLOW;
                break;
            case BEHAVIOUR_MAX_SPEED:
                gSpeedCPUBehaviour[playerId] = SPEED_CPU_BEHAVIOUR_MAX;
                break;
            case BEHAVIOUR_9:
                D_801633F8[playerId] = 1;
                D_801631E0[playerId] = 0;
                gPlayers[playerId].effects &= ~0x1000;
                break;
            case BEHAVIOUR_10:
                D_801633F8[playerId] = 0;
                break;
        }
    }
}

void cpu_behaviour_end(s32 playerIndex, Player* player) {
    u16 nearestWaypoint;
    u32 pathPointEnd;
    s32 behaviourType;

    sCurrentCPUBehaviour =
        &((CPUBehaviour*) LOAD_ASSET(CM_GetProps()->AIBehaviour))[gPreviousCPUBehaviourId[playerIndex]];
    nearestWaypoint = gNearestPathPointByPlayerId[playerIndex];
    behaviourType = sCurrentCPUBehaviour->type;
    pathPointEnd = sCurrentCPUBehaviour->pathPointEnd;
    if (nearestWaypoint >= pathPointEnd) {
        switch (behaviourType) {
            case BEHAVIOUR_1:
                player->effects &= ~0x10;
                D_801630E8[playerIndex] = 0;
                cpu_BehaviourState[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
            case BEHAVIOUR_DRIVE_LEFT:
            case BEHAVIOUR_DRIVE_OUTER:
                gPlayerTrackPositionFactorInstruction[playerIndex].target =
                    gPlayerTrackPositionFactorInstruction[playerIndex].unkC;
                cpu_BehaviourState[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_HOP:
            case BEHAVIOUR_NORMAL_SPEED:
            case BEHAVIOUR_FAST_SPEED:
            case BEHAVIOUR_SLOW_SPEED:
            case BEHAVIOUR_9:
            case BEHAVIOUR_10:
            case BEHAVIOUR_MAX_SPEED:
                cpu_BehaviourState[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            default:
                break;
        }
    }
}

void cpu_behaviour(s32 playerIndex) {
    Player* player = gPlayerOne + playerIndex;

    switch (cpu_BehaviourState[playerIndex]) {
        case CPU_BEHAVIOUR_STATE_NONE:
            break;
        case CPU_BEHAVIOUR_STATE_START:
            cpu_behaviour_start(playerIndex, player);
            break;
        case CPU_BEHAVIOUR_STATE_RUNNING:
            cpu_behaviour_end(playerIndex, player);
            break;
    }
}

void func_80011EC0(s32 arg0, Player* player, s32 arg2, UNUSED u16 arg3) {
    if ((((player->speed / 18.0f) * 216.0f) >= 45.0f) && (D_801630E8[arg0] == 0)) {
        switch (gCurrentTrackSectionTypesPath[sSomeNearestPathPoint]) {
            case RIGHT_LEANING_CURVE:
            case RIGHT_CURVE:
                if ((arg2 >= -9) && (D_80162FF8[arg0] == 0)) {
                    if ((gTrackPositionFactor[arg0] > -0.8) && (gTrackPositionFactor[arg0] < 0.5)) {
                        kart_hop(player);
                        player->effects |= DRIFTING_EFFECT;
                        D_801630E8[arg0] = 1;
                        break;
                    }
                }
                D_801630E8[arg0] = 2;
                break;
            case LEFT_LEANING_CURVE:
            case LEFT_CURVE:
                if ((arg2 < 0xA) && (D_80162FF8[arg0] == 0)) {
                    if ((gTrackPositionFactor[arg0] > -0.5) && (gTrackPositionFactor[arg0] < 0.8)) {
                        kart_hop(player);
                        player->effects |= DRIFTING_EFFECT;
                        D_801630E8[arg0] = -1;
                        break;
                    }
                }
                D_801630E8[arg0] = -2;
                break;
        }
    } else {
        D_801630E8[arg0] = 3;
    }
}

#define GET_PATH_LENGTH(waypoint)               \
    for (i = 0;; i++) {                         \
        if ((u16) waypoint[i].x == 0x8000) { \
            break;                              \
        }                                       \
    }

void generate_train_path(void) {
    s32 i;
    TrackPathPoint* temp;
    TrackPathPoint* waypoint = (TrackPathPoint*) LOAD_ASSET(d_course_kalimari_desert_track_unknown_waypoints);
    GET_PATH_LENGTH(waypoint)

    temp = gVehicle2DPathPoint;
    gVehicle2DPathSize = generate_2d_path(temp, waypoint, i - 1);
    D_80162EB0 = spawn_actor_on_surface(temp[0].x, 2000.0f, temp[0].z);

    // Set the trains path
    gVehiclePath = gVehicle2DPathPoint;
    gVehiclePathSize = gVehicle2DPathSize;
}

void generate_ferry_path(void) {
    TrackPathPoint* waypoint;
    s32 i;

    waypoint = (TrackPathPoint*) LOAD_ASSET(d_course_dks_jungle_parkway_unknown_waypoints2);

    GET_PATH_LENGTH(waypoint)

    gVehicle2DPathSize = generate_2d_path(gVehicle2DPathPoint, waypoint, i - 1);
    D_80162EB2 = -40;

    // Set the boats path
    gVehiclePath = gVehicle2DPathPoint;
    gVehiclePathSize = gVehicle2DPathSize;
}

void spawn_vehicle_on_road(Vec3f position, Vec3s rotation, Vec3f velocity, s32 waypointIndex,
                           s32 someMultiplierTheSequel, f32 speed) {
    f32 origXPos;
    UNUSED f32 pad;
    f32 origZPos;

    origXPos = position[0];
    origZPos = position[2];
    if (gIsInExtra == 0) {
        func_8000D6D0(position, (s16*) &waypointIndex, speed, someMultiplierTheSequel, 0, 3);
        rotation[0] = 0;
        rotation[1] = -0x8000;
        rotation[2] = 0;
    } else {
        func_8000D940(position, (s16*) &waypointIndex, speed, someMultiplierTheSequel, 0);
        rotation[0] = 0;
        rotation[1] = 0;
        rotation[2] = 0;
    }
    velocity[0] = position[0] - origXPos;
    velocity[2] = position[2] - origZPos;
}

void set_vehicle_pos_path_point(TrainCarStuff* trainCar, TrackPathPoint* pos, u16 waypoint) {
    trainCar->position[0] = (f32) pos->x;

    // Fallback for these tracks because they use a 2D path
    if (IsKalimariDesert() || IsDkJungle()) {
        trainCar->position[1] = (f32) D_80162EB0;
    } else {
        trainCar->position[1] = (f32) pos->y;
    }
    trainCar->position[2] = (f32) pos->z;
    trainCar->actorIndex = -1;
    trainCar->waypointIndex = waypoint;
    trainCar->isActive = 0;
    trainCar->velocity[0] = 0.0f;
    trainCar->velocity[1] = 0.0f;
    trainCar->velocity[2] = 0.0f;
}

/**
 * Set waypoint spawn locations for each rolling stock
 * The railroad has 465 waypoints
 * @arg speed default 5.0f, 120.0f is about the maximum usable value
 */
void init_vehicles_trains(size_t i, size_t numCarriages, f32 speed) {
    u16 waypointOffset;
    TrainCarStuff* ptr1;
    TrackPathPoint* pos;
    s32 j;

    gTrainList[i].numCarriages = numCarriages;

    // for (i = 0; i < NUM_TRAINS; i++) {
    //  outputs 160 or 392 depending on the train.
    //  Wraps the value around to always output a valid waypoint.
    waypointOffset = (((i * gVehiclePathSize) / 2) + 160) % gVehiclePathSize;

    // 120.0f is about the maximum usable value
    gTrainList[i].speed = speed;
    for (j = 0; j < gTrainList[i].numCarriages; j++) {
        waypointOffset += 4;
        ptr1 = &gTrainList[i].passengerCars[j];
        pos = &gVehiclePath[waypointOffset];
        set_vehicle_pos_path_point(ptr1, pos, waypointOffset);
    }
    // Smaller offset for the tender
    waypointOffset += 3;
    ptr1 = &gTrainList[i].tender;
    pos = &gVehiclePath[waypointOffset];
    set_vehicle_pos_path_point(ptr1, pos, waypointOffset);
    waypointOffset += 4;
    ptr1 = &gTrainList[i].locomotive;
    pos = &gVehiclePath[waypointOffset];
    set_vehicle_pos_path_point(ptr1, pos, waypointOffset);

    // Only use locomotive unless overwritten below.
    gTrainList[i].numCars = LOCOMOTIVE_ONLY;
    //}

    // Spawn all rolling stock in single player mode.
    switch (gScreenModeSelection) {
        case SCREEN_MODE_1P: // single player
                             // for (i = 0; i < NUM_TRAINS; i++) {
            gTrainList[i].tender.isActive = 1;

            // clang-format off
                // Same line required for matching...
                for (j = 0; j < gTrainList[i].numCarriages; j++) { gTrainList[i].passengerCars[j].isActive = 1; }
            // clang-format on

            gTrainList[i].numCars = NUM_TENDERS + gTrainList[i].numCarriages;
            //}
            break;

        // Spawn locomotive, tender, and one passenger car in versus 2/3 player mode.
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: // multiplayer fall-through
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (gModeSelection != GRAND_PRIX) {
                // for (i = 0; i < NUM_TRAINS; i++) {
                gTrainList[i].tender.isActive = 1;
                gTrainList[i].passengerCars[4].isActive = 1;
                gTrainList[i].numCars = NUM_TENDERS + NUM_2P_PASSENGER_CARS;
                //}
            }
            break;
    }

    gTrainSmokeTimer = 0;
}
/**
 * @brief sync the train components vehicle with the actor
 *
 * @param trainCar
 * @param orientationY
 */
void sync_train_components(TrainCarStuff* trainCar, s16 orientationY) {
    struct TrainCar* trainCarActor;

    trainCarActor = (struct TrainCar*) GET_ACTOR(trainCar->actorIndex);
    trainCarActor->pos[0] = trainCar->position[0];
    trainCarActor->pos[1] = trainCar->position[1];
    trainCarActor->pos[2] = trainCar->position[2];
    if (gIsMirrorMode != 0) {
        trainCarActor->rot[1] = -orientationY;
    } else {
        trainCarActor->rot[1] = orientationY;
    }
    trainCarActor->velocity[0] = trainCar->velocity[0];
    trainCarActor->velocity[2] = trainCar->velocity[2];
}

void update_vehicle_trains(void) {
    UNUSED s32 pad[3];
    f32 temp_f20;
    TrainCarStuff* car;
    u16 oldWaypointIndex;
    s16 orientationYUpdate;
    f32 temp_f22;
    s32 i;
    s32 j;
    Vec3f smokePos;

    gTrainSmokeTimer += 1;

    for (i = 0; i < NUM_TRAINS; i++) {
        oldWaypointIndex = (u16) gTrainList[i].locomotive.waypointIndex;

        temp_f20 = gTrainList[i].locomotive.position[0];
        temp_f22 = gTrainList[i].locomotive.position[2];

        orientationYUpdate = update_vehicle_following_path(
            gTrainList[i].locomotive.position, (s16*) &gTrainList[i].locomotive.waypointIndex, gTrainList[i].speed);

        gTrainList[i].locomotive.velocity[0] = gTrainList[i].locomotive.position[0] - temp_f20;
        gTrainList[i].locomotive.velocity[2] = gTrainList[i].locomotive.position[2] - temp_f22;

        sync_train_components(&gTrainList[i].locomotive, orientationYUpdate);

        if ((oldWaypointIndex != gTrainList[i].locomotive.waypointIndex) &&
            ((gTrainList[i].locomotive.waypointIndex == 0x00BE) ||
             (gTrainList[i].locomotive.waypointIndex == 0x0140))) { // play crossing bell sound
            func_800C98B8(gTrainList[i].locomotive.position, gTrainList[i].locomotive.velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
        } else if (random_int(100) == 0) { // play train whistle sound
            func_800C98B8(gTrainList[i].locomotive.position, gTrainList[i].locomotive.velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
        }

        gTrainList[i].someFlags = set_vehicle_render_distance_flags(
            gTrainList[i].locomotive.position, TRAIN_SMOKE_RENDER_DISTANCE, gTrainList[i].someFlags);
        // Renders locomotive smoke on all screens if any player is within range.
        if ((((s16) gTrainSmokeTimer % 5) == 0) && (gTrainList[i].someFlags != 0)) {
            smokePos[0] = gTrainList[i].locomotive.position[0];
            smokePos[1] = (f32) ((f64) gTrainList[i].locomotive.position[1] + 65.0);
            smokePos[2] = (f32) ((f64) gTrainList[i].locomotive.position[2] + 25.0);
            adjust_position_by_angle(smokePos, gTrainList[i].locomotive.position, orientationYUpdate);
            spawn_train_smoke(i, smokePos, 1.1f);
        }

        car = &gTrainList[i].tender;

        if (car->isActive == 1) {
            temp_f20 = car->position[0];
            temp_f22 = car->position[2];
            orientationYUpdate =
                update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, gTrainList[i].speed);
            car->velocity[0] = car->position[0] - temp_f20;
            car->velocity[2] = car->position[2] - temp_f22;
            sync_train_components(car, orientationYUpdate);
        }

        for (j = 0; j < gTrainList[i].numCarriages; j++) {
            car = &gTrainList[i].passengerCars[j];
            if (car->isActive == 1) {
                temp_f20 = car->position[0];
                temp_f22 = car->position[2];

                orientationYUpdate =
                    update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, gTrainList[i].speed);
                car->velocity[0] = car->position[0] - temp_f20;
                car->velocity[2] = car->position[2] - temp_f22;
                sync_train_components(car, orientationYUpdate);
            }
        }
    }
}

void handle_trains_interactions(s32 playerId, Player* player) {
    TrainCarStuff* trainCar;
    f32 playerPosX;
    f32 playerPosZ;
    f32 x_dist;
    f32 z_dist;
    s32 trainIndex;
    s32 passengerCarIndex;

    if (D_801631E0[playerId] != 1) {
        if (!(player->effects & 0x01000000)) {
            playerPosX = player->pos[0];
            playerPosZ = player->pos[2];
            for (trainIndex = 0; trainIndex < NUM_TRAINS; trainIndex++) {
                trainCar = &gTrainList[trainIndex].locomotive;
                x_dist = playerPosX - trainCar->position[0];
                z_dist = playerPosZ - trainCar->position[2];
                if ((x_dist > -100.0) && (x_dist < 100.0)) {
                    if ((z_dist > -100.0) && (z_dist < 100.0)) {
                        if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                    trainCar->velocity[2], 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            player->soundEffects |= REVERSE_SOUND_EFFECT;
                        }
                        trainCar = &gTrainList[trainIndex].tender;
                        if (trainCar->isActive == 1) {
                            if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2],
                                                        trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                        playerPosX, playerPosZ) == 1) {
                                player->soundEffects |= REVERSE_SOUND_EFFECT;
                            }
                        }
                    }
                }

                for (passengerCarIndex = 0; passengerCarIndex < gTrainList[trainIndex].numCarriages;
                     passengerCarIndex++) {
                    trainCar = &gTrainList[trainIndex].passengerCars[passengerCarIndex];
                    x_dist = playerPosX - trainCar->position[0];
                    z_dist = playerPosZ - trainCar->position[2];
                    if (trainCar->isActive == 1) {
                        if ((x_dist > -100.0) && (x_dist < 100.0)) {
                            if ((z_dist > -100.0) && (z_dist < 100.0)) {
                                if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2],
                                                            trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                            playerPosX, playerPosZ) == 1) {
                                    player->soundEffects |= REVERSE_SOUND_EFFECT;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Appears to check if the train is close to the crossing.
 * Implements sCrossingActiveTimer as a counter
 */
void func_80013054(void) {
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f12;
    s32 i;
    isCrossingTriggeredByIndex[0] = 0;
    isCrossingTriggeredByIndex[1] = 0;

    for (i = 0; i < NUM_TRAINS; i++) {
        temp_f16 = gTrainList[i].locomotive.waypointIndex / ((f32) gVehiclePathSize);
        temp_f18 = 0.72017354f;
        temp_f12 = 0.42299348f;

        if (((temp_f12 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) gTrainList[i].numCars) * 0.01) + (temp_f12 + 0.01)))) {

            isCrossingTriggeredByIndex[0] = 1;
        }
        if (((temp_f18 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) gTrainList[i].numCars) * 0.01) + (temp_f18 + 0.01)))) {

            isCrossingTriggeredByIndex[1] = 1;
        }
    }

    for (i = 0; i < NUM_CROSSINGS; i++) {
        if (isCrossingTriggeredByIndex[i] == 1) {
            sCrossingActiveTimer[i] += 1;
        } else {
            sCrossingActiveTimer[i] = 0;
        }
    }
}

void check_ai_crossing_distance(s32 playerId) {
    bStopAICrossing[playerId] = 0;
    if (IsKalimariDesert()) {
        if ((!(D_801631E0[playerId] != 0)) ||
            (set_vehicle_render_distance_flags(gPlayers[playerId].pos, TRAIN_CROSSING_AI_DISTANCE, 0))) {

            if ((isCrossingTriggeredByIndex[1] == 1) && ((sCrossingActiveTimer[1]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {

                if ((sSomeNearestPathPoint > 176) && (sSomeNearestPathPoint < 182)) {
                    bStopAICrossing[playerId] = 1;
                }
            }
            if ((isCrossingTriggeredByIndex[0] == 1) && ((sCrossingActiveTimer[0]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {
                if ((sSomeNearestPathPoint >= 306) && (sSomeNearestPathPoint < 310)) {
                    bStopAICrossing[playerId] = 1;
                }
            }
        }
    }
}

void init_vehicles_ferry(void) {
    PaddleBoatStuff* paddleBoat;
    s32 i;
    TrackPathPoint* temp_a2;
    u16 temp;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        temp = i * 0xB4;
        paddleBoat = &gPaddleBoats[i];
        temp_a2 = &gVehiclePath[temp];
        paddleBoat->position[0] = temp_a2->x;
        paddleBoat->position[1] = D_80162EB2;
        paddleBoat->position[2] = temp_a2->z;
        paddleBoat->waypointIndex = i * 0xB4;
        paddleBoat->actorIndex = -1;

        if (gPlayerCount >= 3) {
            paddleBoat->isActive = 0;
        } else {
            paddleBoat->isActive = 1;
        }
        paddleBoat->velocity[0] = 0.0f;
        paddleBoat->velocity[1] = 0.0f;
        paddleBoat->velocity[2] = 0.0f;
        paddleBoat->speed = 1.6666666f;
        paddleBoat->rotY = 0;
    }
    gFerrySmokeTimer = 0;
}

void update_vehicle_paddle_boats(void) {
    PaddleBoatStuff* paddleBoat;
    TrackPathPoint* waypoint;
    s32 i;
    struct Actor* paddleBoatActor;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f30;
    s16 temp_a1;
    s32 temp;
    s16 var_v1;
    Vec3f sp94;
    Vec3f sp88;
    UNUSED s32 pad;
    Vec3f smokePos;
    UNUSED s32 pad2;
    gFerrySmokeTimer += 1;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        paddleBoat = &gPaddleBoats[i];
        if (paddleBoat->isActive == 1) {
            temp_f26 = paddleBoat->position[0];
            temp_f28 = paddleBoat->position[1];
            temp_f30 = paddleBoat->position[2];
            update_vehicle_following_path(paddleBoat->position, (s16*) &paddleBoat->waypointIndex, paddleBoat->speed);
            paddleBoat->someFlags = set_vehicle_render_distance_flags(paddleBoat->position, BOAT_SMOKE_RENDER_DISTANCE,
                                                                      paddleBoat->someFlags);
            if ((((s16) gFerrySmokeTimer % 10) == 0) && (paddleBoat->someFlags != 0)) {
                smokePos[0] = (f32) ((f64) paddleBoat->position[0] - 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->position[2] + 45.0);
                adjust_position_by_angle(smokePos, paddleBoat->position, paddleBoat->rotY);
                spawn_ferry_smoke(i, smokePos, 1.1f);
                smokePos[0] = (f32) ((f64) paddleBoat->position[0] + 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->position[2] + 45.0);
                adjust_position_by_angle(smokePos, paddleBoat->position, paddleBoat->rotY);
                spawn_ferry_smoke(i, smokePos, 1.1f);
            }
            if (random_int(100) == 0) {
                if (random_int(2) == 0) {
                    func_800C98B8(paddleBoat->position, paddleBoat->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x47));
                } else {
                    func_800C98B8(paddleBoat->position, paddleBoat->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x48));
                }
            }
            sp94[0] = temp_f26;
            sp94[1] = temp_f28;
            sp94[2] = temp_f30;
            waypoint = &gVehiclePath[(paddleBoat->waypointIndex + 5) % gVehiclePathSize];
            sp88[0] = (f32) waypoint->x;
            sp88[1] = (f32) D_80162EB0;
            sp88[2] = (f32) waypoint->z;
            temp_a1 = get_angle_between_path(sp94, sp88);
            temp = temp_a1 - paddleBoat->rotY;
            var_v1 = temp;
            if (var_v1 < 0) {
                var_v1 = -var_v1;
            }
            if (var_v1 >= 0x1771) {
                if (paddleBoat->speed > 0.2) {
                    paddleBoat->speed -= 0.04;
                }
                if (var_v1 >= 0x3D) {
                    var_v1 = 0x003C;
                }
            } else {
                if (paddleBoat->speed < 2.0) {
                    paddleBoat->speed += 0.02;
                }
                if (var_v1 >= 0x1F) {
                    var_v1 = 0x001E;
                }
            }
            if (temp >= 0x8000) {
                paddleBoat->rotY -= var_v1;
            } else if (temp > 0) {
                paddleBoat->rotY += var_v1;
            } else if (temp < -0x7FFF) {
                paddleBoat->rotY += var_v1;
            } else if (temp < 0) {
                paddleBoat->rotY -= var_v1;
            }
            paddleBoat->velocity[0] = paddleBoat->position[0] - temp_f26;
            paddleBoat->velocity[1] = paddleBoat->position[1] - temp_f28;
            paddleBoat->velocity[2] = paddleBoat->position[2] - temp_f30;
            paddleBoatActor = GET_ACTOR(paddleBoat->actorIndex);
            paddleBoatActor->pos[0] = paddleBoat->position[0];
            paddleBoatActor->pos[1] = paddleBoat->position[1];
            paddleBoatActor->pos[2] = paddleBoat->position[2];
            if (gIsMirrorMode != 0) {
                paddleBoatActor->rot[1] = -paddleBoat->rotY;
            } else {
                paddleBoatActor->rot[1] = paddleBoat->rotY;
            }
            paddleBoatActor->velocity[0] = paddleBoat->velocity[0];
            paddleBoatActor->velocity[1] = paddleBoat->velocity[1];
            paddleBoatActor->velocity[2] = paddleBoat->velocity[2];
        }
    }
}

void handle_paddle_boats_interactions(Player* player) {
    s32 someIndex;
    PaddleBoatStuff* tempPaddleWheelBoat;
    f32 x_diff;
    f32 y_diff;
    f32 z_diff;
    f32 playerX;
    f32 playerZ;
    f32 playerY;

    if (!((player->effects & 0x01000000)) && (!(player->effects & HIT_BY_ITEM_EFFECT))) {
        playerX = player->pos[0];
        playerY = player->pos[1];
        playerZ = player->pos[2];
        for (someIndex = 0; someIndex < NUM_ACTIVE_PADDLE_BOATS; someIndex++) {
            tempPaddleWheelBoat = &gPaddleBoats[someIndex];
            if (tempPaddleWheelBoat->isActive == 1) {
                x_diff = playerX - tempPaddleWheelBoat->position[0];
                y_diff = playerY - tempPaddleWheelBoat->position[1];
                z_diff = playerZ - tempPaddleWheelBoat->position[2];
                if ((x_diff > -300.0) && (x_diff < 300.0)) {
                    if ((z_diff > -300.0) && (z_diff < 300.0)) {
                        if ((is_collide_with_vehicle(tempPaddleWheelBoat->position[0], tempPaddleWheelBoat->position[2],
                                                     tempPaddleWheelBoat->velocity[0], tempPaddleWheelBoat->velocity[2],
                                                     200.0f, 60.0f, playerX, playerZ) == 1) &&
                            (y_diff < 60.0)) {
                            player->soundEffects |= 0x80000;
                        }
                    }
                }
            }
        }
    }
}

void initialize_toads_turnpike_vehicle(f32 speedA, f32 speedB, s32 numVehicles, s32 arg3, VehicleStuff* vehicleList,
                                       TrackPathPoint* waypointList) {
    VehicleStuff* veh;
    TrackPathPoint* temp_v0;
    s32 i;
    u16 waypointOffset;
    s32 numWaypoints = gPathCountByPathIndex[0];
    for (i = 0; i < numVehicles; i++) {
        waypointOffset = (((i * numWaypoints) / numVehicles) + arg3) % numWaypoints;
        veh = &vehicleList[i];
        temp_v0 = &waypointList[waypointOffset];
        veh->position[0] = (f32) temp_v0->x;
        veh->position[1] = (f32) temp_v0->y;
        veh->position[2] = (f32) temp_v0->z;
        veh->actorIndex = -1;
        veh->waypointIndex = waypointOffset;
        veh->unused = 0;
        veh->velocity[0] = 0.0f;
        veh->velocity[1] = 0.0f;
        veh->velocity[2] = 0.0f;
        veh->someFlags = 0;
        veh->someFlagsTheSequel = 0;
        if (gModeSelection == TIME_TRIALS) {
            veh->someType = (i % 3);
        } else {
            veh->someType = random_int(3);
        }
        veh->someMultiplierTheSequel = (f32) ((f64) (f32) (veh->someType - 1) * 0.6);
        if (((gCCSelection > CC_50) || (gModeSelection == TIME_TRIALS)) && (veh->someType == 2)) {
            veh->speed = speedA;
        } else {
            veh->speed = speedB;
        }
        veh->rotation[0] = 0;
        veh->rotation[2] = 0;
        if (gIsInExtra == 0) {
            veh->rotation[1] = func_8000D6D0(veh->position, (s16*) &veh->waypointIndex, veh->speed,
                                             veh->someMultiplierTheSequel, 0, 3);
        } else {
            veh->rotation[1] =
                func_8000D940(veh->position, (s16*) &veh->waypointIndex, veh->speed, veh->someMultiplierTheSequel, 0);
        }
    }
    sVehicleSoundRenderCounter = 10;
}

f32 func_80013C74(s16 someType, s16 waypointIndex) {
    f32 var_f2;

    var_f2 = 0.0f;
    if (waypointIndex < 0x28A) {
        switch (someType) {
            case 0:
                var_f2 = -0.7f;
                break;
            case 1:
                break;
            case 2:
                var_f2 = 0.7f;
                break;
            default:
                break;
        }
    } else {
        switch (someType) {
            case 0:
            case 1:
                var_f2 = -0.5f;
                break;
            case 2:
                var_f2 = 0.5f;
                break;
            default:
                break;
        }
    }
    return var_f2;
}

void update_vehicle_follow_path_point(VehicleStuff* vehicle) {
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 temp_f2_2;
    s16 var_a1;
    s16 thing;
    Vec3f sp40;
    Vec3f sp34;
    struct Actor* vehicleActor;

    sp5C = vehicle->position[0];
    sp58 = vehicle->position[1];
    sp54 = vehicle->position[2];
    sp40[0] = sp58;
    sp40[1] = 0.0f;
    sp40[2] = 0.0f;
    temp_f0_2 = func_80013C74(vehicle->someType, vehicle->waypointIndex);
    if (vehicle->someMultiplierTheSequel < temp_f0_2) {
        vehicle->someMultiplierTheSequel = vehicle->someMultiplierTheSequel + 0.06;
        if (temp_f0_2 < vehicle->someMultiplierTheSequel) {
            vehicle->someMultiplierTheSequel = temp_f0_2;
        }
    }
    if (temp_f0_2 < vehicle->someMultiplierTheSequel) {
        vehicle->someMultiplierTheSequel = vehicle->someMultiplierTheSequel - 0.06;
        if (vehicle->someMultiplierTheSequel < temp_f0_2) {
            vehicle->someMultiplierTheSequel = temp_f0_2;
        }
    }
    if (gIsInExtra == 0) {
        var_a1 = func_8000D6D0(vehicle->position, (s16*) &vehicle->waypointIndex, vehicle->speed,
                               vehicle->someMultiplierTheSequel, 0, 3);
    } else {
        var_a1 = func_8000D940(vehicle->position, (s16*) &vehicle->waypointIndex, vehicle->speed,
                               vehicle->someMultiplierTheSequel, 0);
    }
    adjust_angle(&vehicle->rotation[1], var_a1, 100);
    temp_f0_3 = vehicle->position[0] - sp5C;
    temp_f2_2 = vehicle->position[2] - sp54;
    sp34[0] = vehicle->position[1];
    sp34[1] = 0.0f;
    sp34[2] = sqrtf((temp_f0_3 * temp_f0_3) + (temp_f2_2 * temp_f2_2));
    thing = get_angle_between_two_vectors(sp40, sp34);
    adjust_angle(&vehicle->rotation[0], -thing, 100);
    vehicle->velocity[0] = vehicle->position[0] - sp5C;
    vehicle->velocity[1] = vehicle->position[1] - sp58;
    vehicle->velocity[2] = vehicle->position[2] - sp54;
    vehicleActor = GET_ACTOR(vehicle->actorIndex);
    vehicleActor->pos[0] = vehicle->position[0];
    vehicleActor->pos[1] = vehicle->position[1];
    vehicleActor->pos[2] = vehicle->position[2];
    vehicleActor->rot[0] = vehicle->rotation[0];
    if (gIsMirrorMode != 0) {
        vehicleActor->rot[1] = -vehicle->rotation[1];
    } else {
        vehicleActor->rot[1] = vehicle->rotation[1];
    }
    vehicleActor->rot[2] = vehicle->rotation[2];
    vehicleActor->velocity[0] = vehicle->velocity[0];
    vehicleActor->velocity[1] = vehicle->velocity[1];
    vehicleActor->velocity[2] = vehicle->velocity[2];
}

void handle_vehicle_interactions(s32 playerId, Player* player, VehicleStuff* vehicle, f32 arg3, f32 arg4, s32 arg5,
                                 u32 soundBits) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f22;

    s32 i;

    f32 spC4;
    f32 spC0;
    f32 spBC;

    if (((D_801631E0[playerId] != 1) || ((((player->type & PLAYER_HUMAN) != 0)) && !(player->type & PLAYER_CPU))) &&
        !(player->effects & 0x01000000)) {

        spC4 = player->pos[0];
        spC0 = player->pos[1];
        spBC = player->pos[2];

        for (i = 0; i < arg5; i++) {
            temp_f12 = spC4 - vehicle->position[0];
            temp_f22 = spC0 - vehicle->position[1];
            temp_f14 = spBC - vehicle->position[2];

            if (((temp_f12) > -100.0) && ((temp_f12) < 100.0)) {
                if ((temp_f22 > -20.0) && (temp_f22 < 20.0)) {

                    if (((temp_f14) > -100.0) && ((temp_f14) < 100.0)) {
                        if (is_collide_with_vehicle(vehicle->position[0], vehicle->position[2], vehicle->velocity[0],
                                                    vehicle->velocity[2], arg3, arg4, spC4, spBC) == (s32) 1) {
                            player->soundEffects |= REVERSE_SOUND_EFFECT;
                        }
                    }
                }
            }
            if ((player->type & PLAYER_HUMAN) && !(player->type & PLAYER_CPU)) {
                if (((temp_f12) > -300.0) && ((temp_f12) < 300.0) && ((temp_f22 > -20.0)) && (temp_f22 < 20.0) &&
                    (((temp_f14) > -300.0)) && ((temp_f14) < 300.0)) {
                    if ((sVehicleSoundRenderCounter > 0) && (vehicle->someFlags == 0)) {
                        sVehicleSoundRenderCounter -= 1;
                        vehicle->someFlags |= (RENDER_VEHICLE << playerId);
                        func_800C9D80(vehicle->position, vehicle->velocity, soundBits);
                    }
                } else {
                    if (vehicle->someFlags != 0) {
                        vehicle->someFlags &= ~(RENDER_VEHICLE << playerId);
                        if (vehicle->someFlags == 0) {
                            sVehicleSoundRenderCounter += 1;
                            func_800C9EF4(vehicle->position, soundBits);
                        }
                    }
                }

                if (((temp_f12) > -200.0) && ((temp_f12) < 200.0) && ((temp_f22 > -20.0)) && (temp_f22 < 20.0) &&
                    (((temp_f14) > -200.0)) && ((temp_f14) < 200.0)) {
                    if (!(vehicle->someFlagsTheSequel & ((1 << playerId)))) {

                        s32 var_s1 = 0;
                        u16 path = gPathCountByPathIndex[0];
                        s32 t1;
                        s32 t2;

                        switch (gIsInExtra) {
                            case 0:
                                t1 = is_path_point_in_range(vehicle->waypointIndex,
                                                            gNearestPathPointByPlayerId[playerId], 10, 0, path);
                                if ((gIsPlayerWrongDirection[playerId] == 0) && (t1 > 0) &&
                                    (player->speed < vehicle->speed)) {
                                    var_s1 = 1;
                                }
                                if ((gIsPlayerWrongDirection[playerId] == 1) && (t1 > 0)) {
                                    var_s1 = 1;
                                }
                                break;
                            case 1:
                                t2 = is_path_point_in_range(vehicle->waypointIndex,
                                                            gNearestPathPointByPlayerId[playerId], 0, 10, path);
                                if (t2 > 0) {
                                    if (random_int(2) == 0) {
                                        // temp_v1_2 = gIsPlayerWrongDirection[playerId];
                                        if (gIsPlayerWrongDirection[playerId] == 0) {
                                            var_s1 = 1;
                                        }
                                        if ((gIsPlayerWrongDirection[playerId] == 1) &&
                                            (player->speed < vehicle->speed)) {
                                            var_s1 = 1;
                                        }
                                    } else {
                                        vehicle->someFlagsTheSequel |= ((1 << playerId));
                                    }
                                }
                                break;
                        }
                        if (var_s1 == 1) {

                            u32 soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);

                            switch (soundBits) {
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05):
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);
                                    if (random_int(4) == 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3C);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02):
                                    if (random_int(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3D);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3E);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03):
                                    if (random_int(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3F);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x40);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04):
                                    if (random_int(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x41);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x42);
                                    }
                                    break;
                            }
                            vehicle->someFlagsTheSequel |= ((1 << playerId));
                            func_800C98B8(vehicle->position, vehicle->velocity, soundBits2);
                        }
                    }
                } else {
                    if (vehicle->someFlagsTheSequel & ((1 << playerId))) {
                        vehicle->someFlagsTheSequel &= ~((1 << playerId));
                    }
                }
            }
            vehicle++;
        }
    }
}

f32 player_track_position_factor_vehicle(s16 arg0, f32 arg1, s16 arg2) {
    if (arg2 < 0x28A) {
        switch (arg0) {
            case 0:
                if (arg1 < 0.0) {
                    arg1 = 0.0f;
                }
                break;
            case 1:
                if (arg1 < 0.0) {
                    arg1 = -0.8f;
                } else {
                    arg1 = 0.8f;
                }
                break;
            case 2:
                if (arg1 >= 0.0) {
                    arg1 = 0.0f;
                }
                break;
            default:
                break;
        }
    } else {
        switch (arg0) {
            case 0:
            case 1:
                arg1 = 0.5f;
                break;
            case 2:
                arg1 = -0.5f;
                break;
            default:
                break;
        }
    }
    return arg1;
}

void update_player_track_position_factor_from_vehicle(s32 playerId, s32 arg1, VehicleStuff* vehicle) {
    UNUSED s32 var_v1;
    s32 var_v0;
    s32 var_s2;
    s32 waypointCount;
    u16 temp_a1;
    UNUSED VehicleStuff* tempVehicle;

    waypointCount = gPathCountByPathIndex[0];
    if (!(gPlayers[playerId].speed < 1.6666666666666667)) {
        for (var_s2 = 0; var_s2 < arg1; var_s2++, vehicle++) {
            temp_a1 = vehicle->waypointIndex;
            for (var_v0 = 0; var_v0 < 0x18; var_v0 += 3) {
                if (((sSomeNearestPathPoint + var_v0) % waypointCount) == temp_a1) {
                    gPlayerTrackPositionFactorInstruction[playerId].target = player_track_position_factor_vehicle(
                        vehicle->someType, gTrackPositionFactor[playerId], temp_a1);
                    return;
                }
            }
        }
    }
}

void init_vehicles_box_trucks(void) {
    f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;
    s32 numTrucks = NUM_RACE_BOX_TRUCKS;
    if (gModeSelection == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_BOX_TRUCKS;
    }
    initialize_toads_turnpike_vehicle(a, b, numTrucks, 0, gBoxTruckList, &gTrackPaths[0][0]);
}

void update_vehicle_box_trucks(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
        update_vehicle_follow_path_point(&gBoxTruckList[loopIndex]);
    }
}

void handle_box_trucks_interactions(s32 playerId, Player* player) {
    handle_vehicle_interactions(playerId, player, gBoxTruckList, 55.0f, 12.5f, NUM_RACE_BOX_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03));
}

void update_player_track_position_factor_from_box_trucks(s32 playerId) {
    update_player_track_position_factor_from_vehicle(playerId, NUM_RACE_BOX_TRUCKS, gBoxTruckList);
}

void init_vehicles_school_buses(void) {
    s32 numBusses;
    f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;

    numBusses = NUM_RACE_SCHOOL_BUSES;
    if (gModeSelection == TIME_TRIALS) {
        numBusses = NUM_TIME_TRIAL_SCHOOL_BUSES;
    }
    initialize_toads_turnpike_vehicle(a, b, numBusses, 75, gSchoolBusList, &gTrackPaths[0][0]);
}

void update_vehicle_school_bus(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
        update_vehicle_follow_path_point(&gSchoolBusList[loopIndex]);
    }
}

void handle_school_buses_interactions(s32 playerId, Player* player) {
    handle_vehicle_interactions(playerId, player, gSchoolBusList, 70.0f, 12.5f, NUM_RACE_SCHOOL_BUSES,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02));
}

void update_player_track_position_factor_from_buses(s32 playerId) {
    update_player_track_position_factor_from_vehicle(playerId, NUM_RACE_SCHOOL_BUSES, gSchoolBusList);
}

void init_vehicles_trucks(void) {
    s32 numTrucks;
    f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;

    numTrucks = NUM_RACE_TANKER_TRUCKS;
    if (gModeSelection == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_TANKER_TRUCKS;
    }
    initialize_toads_turnpike_vehicle(a, b, numTrucks, 50, gTankerTruckList, &gTrackPaths[0][0]);
}

void update_vehicle_tanker_trucks(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
        update_vehicle_follow_path_point(&gTankerTruckList[loopIndex]);
    }
}

void handle_tanker_trucks_interactions(s32 playerId, Player* player) {
    handle_vehicle_interactions(playerId, player, gTankerTruckList, 55.0f, 12.5f, NUM_RACE_TANKER_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04));
}

void update_player_track_position_factor_from_tanker_truck(s32 playerId) {
    update_player_track_position_factor_from_vehicle(playerId, NUM_RACE_TANKER_TRUCKS, gTankerTruckList);
}

void init_vehicles_cars(void) {
    s32 numCars;
    f32 a = ((gCCSelection * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((gCCSelection * 90.0) / 216.0f) + 2.9166666666666665;

    numCars = NUM_RACE_CARS;
    if (gModeSelection == TIME_TRIALS) {
        numCars = NUM_TIME_TRIAL_CARS;
    }
    initialize_toads_turnpike_vehicle(a, b, numCars, 25, gCarList, &gTrackPaths[0][0]);
}

void update_vehicle_cars(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
        update_vehicle_follow_path_point(&gCarList[loopIndex]);
    }
}

void handle_cars_interactions(s32 playerId, Player* player) {
    handle_vehicle_interactions(playerId, player, gCarList, 11.5f, 8.5f, NUM_RACE_CARS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05));
}

void update_player_track_position_factor_from_cars(s32 playerId) {
    update_player_track_position_factor_from_vehicle(playerId, NUM_RACE_CARS, gCarList);
}

void func_80014D30(s32 cameraId, s32 pathIndex) {
    s16 cameraWaypoint;
    TrackPathPoint* temp_v0;

    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    temp_v0 = &gTrackPaths[pathIndex][cameraWaypoint];
    check_bounding_collision(&cameras[cameraId].collision, 10.0f, (f32) temp_v0->x, (f32) temp_v0->y + 30.0f,
                             (f32) temp_v0->z);
}

void func_80014DE4(s32 cameraIndex) {
    s32 cameraId;

    D_801646CC = 0;
    D_80164678[cameraIndex] = D_80164670[cameraIndex];
    if ((gModeSelection != 1) && ((GetCupCursorPosition() == COURSE_ONE) || (gDemoMode == (u16) 1))) {
        D_80164678[cameraIndex] = 0;
    } else if ((D_80164678[cameraIndex] != 0) && (D_80164678[cameraIndex] != (s16) 1) &&
               (D_80164678[cameraIndex] != 2) && (D_80164678[cameraIndex] != 3)) {
        D_80164678[cameraIndex] = 0;
    }
    D_80164680[cameraIndex] = -1;
    D_80163238 = 0;
    D_801646C0[cameraIndex] = 0;
    D_801646C8 = 0;
    D_801646D0[cameraIndex].unk0 = 0;
    D_801646D0[cameraIndex].unk2 = 0;
    D_801646D0[cameraIndex].unk4 = 0;
    if ((gModeSelection == 1) && (gCourseMapInit == 0)) {
        D_80164678[cameraIndex] = 0;
    }

    for (cameraId = 0; cameraId < NUM_CAMERAS; cameraId++) {
        gNearestPathPointByCameraId[cameraId] = 0;
    }
}

f32 func_80014EE4(f32 arg0, s32 arg1) {
    f32 temp_f0;
    f64 temp_f2;

    temp_f0 = D_80164498[arg1];
    switch (D_80164678[arg1]) { /* irregular */
        default:
            arg0 = 40.0f;
            break;
        case 0:
            temp_f2 = 40.0;
            temp_f2 += temp_f0;
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                    ;
                }
            }
            break;
        case 1:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                    ;
                }
            }
            break;
        case 3:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 0.5;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 0.5;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            break;
        case 2:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            break;
    }
    return arg0;
}

void calculate_camera_up_vector(Camera* camera, s32 cameraIndex) {
    f32 xnorm;
    f32 ynorm;
    f32 znorm;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    f32 sp28;
    u16 thing;

    thing = D_801646C0[cameraIndex];
    if (thing == 0) {
        camera->up[0] = 0.0f;
        camera->up[2] = 0.0f;
        camera->up[1] = 1.0f;
    } else {
        xdiff = camera->lookAt[0] - camera->pos[0];
        ydiff = camera->lookAt[1] - camera->pos[1];
        zdiff = camera->lookAt[2] - camera->pos[2];
        distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
        xnorm = xdiff / distance;
        ynorm = ydiff / distance;
        znorm = zdiff / distance;
        sp28 = 1.0 - coss(thing);
        camera->up[0] = (sp28 * xnorm * ynorm) - (sins(thing) * znorm);
        camera->up[1] = coss(thing) + (sp28 * ynorm * ynorm);
        camera->up[2] = (sins(thing) * xnorm) + (sp28 * ynorm * znorm);
    }
}

UNUSED void func_8001530C(void) {
}

void func_80015314(s32 playerId, UNUSED f32 arg1, s32 cameraId) {
    Camera* temp_a0;
    Player* temp_a1;

    // wtf is up with the pointer accesses here?
    // What aren't they just doing thing = &some_pointer[some_index]?
    temp_a1 = gPlayerOne;
    temp_a0 = camera1;
    temp_a1 += playerId;
    temp_a0 += cameraId;
    temp_a0->unk_2C = temp_a1->rotation[1];
    func_80015390(temp_a0, temp_a1, 0);
}

void func_80015390(Camera* camera, UNUSED Player* player, UNUSED s32 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    Player* temp_s1;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED Vec3f pad2;
    Vec3f sp64;
    UNUSED s32 pad3[9];
    s16 var_a2;

    /*
    Doing the more sensible:
    temp_s1 = &gPlayerOne[camera->playerId];
    leads to some regalloc differences
    */
    temp_s1 = gPlayerOne;
    temp_s1 += camera->playerId;
    if (temp_s1->unk_078 == 0) {
        var_a2 = 0x0064;
    } else if (temp_s1->unk_078 < 0) {
        var_a2 = 0xA0 - (temp_s1->unk_078 / 16);
    } else {
        var_a2 = 0xA0 + (temp_s1->unk_078 / 16);
    }
    if (!((temp_s1->effects & 0x80) || (temp_s1->effects & 0x40))) {
        adjust_angle(&camera->unk_2C, temp_s1->rotation[1], var_a2);
    }
    func_8001D794(temp_s1, camera, sp64, &sp84, &sp80, &sp7C, camera->unk_2C);
    check_bounding_collision(&camera->collision, 10.0f, sp84, sp80, sp7C);
    camera->lookAt[0] = sp64[0];
    camera->lookAt[1] = sp64[1];
    camera->lookAt[2] = sp64[2];
    camera->pos[0] = sp84;
    camera->pos[1] = sp80;
    camera->pos[2] = sp7C;
    temp_f12 = camera->lookAt[0] - camera->pos[0];
    sp90 = camera->lookAt[1] - camera->pos[1];
    temp_f14 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(temp_f12, temp_f14);
    camera->rot[0] = atan2s(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->rot[2] = 0;
}

void func_80015544(s32 playerId, f32 arg1, s32 cameraId, s32 pathIndex) {
    Camera* camera;

    f32 temp_f12;
    f32 temp_f2;
    s32 test = gPathCountByPathIndex[pathIndex];

    D_80164688[cameraId] = arg1;
    camera = cameras + cameraId;
    gNearestPathPointByCameraId[cameraId] = (gNearestPathPointByPlayerId[playerId] + 10) % test;

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], arg1, pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164638[cameraId] = gOffsetPosition[2];

    temp_f2 = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;

    temp_f12 = spawn_actor_on_surface(gOffsetPosition[0], (f32) (temp_f2 + 30.0), gOffsetPosition[2]);
    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        D_80164618[cameraId] = (f32) (temp_f2 + 10.0);
    } else {
        D_80164618[cameraId] = (f32) (temp_f12 + 10.0);
    }
    D_80164648[cameraId] = 0.0f;
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_8001577C(Camera* camera, UNUSED Player* playerArg, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 pad2;
    s32 playerId;
    UNUSED s32 pad3[9];
    Player* player;
    s32 waypointDiff;
    s32 pathIndex;

    playerId = camera->playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    player = gPlayerOne;
    player += playerId;
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    playerWaypoint = gNearestPathPointByPlayerId[playerId];
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x0032U, 0x000FU, gPathCountByPathIndex[pathIndex]) <=
        0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else {
        if (gTrackPositionFactor[playerId] < (-0.7)) {
            waypointDiff = playerWaypoint - cameraWaypoint;
            if ((D_80164688[cameraId] < (-0.5)) && ((waypointDiff * waypointDiff) < 5)) {
                func_8001A348(cameraId, 1.0f, 3);
                goto alable;
            }
        }
        // clang-format off
        // I hate this, but a fakematch is a fakematch
        if (gTrackPositionFactor[playerId] > 0.7) { waypointDiff = playerWaypoint - cameraWaypoint; if ((D_80164688[cameraId] > 0.5) && ((waypointDiff * waypointDiff) < 5)) {
                func_8001A348(cameraId, -1.0f, 2);
                // clang-format on
            }
        }
    }
alable:
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    xdiff = camera->lookAt[0] - camera->pos[0];
    ydiff = camera->lookAt[1] - camera->pos[1];
    zdiff = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff, zdiff);
    camera->rot[0] = atan2s(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->rot[2] = 0;
}

void func_80015A9C(s32 playerId, f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;

    D_80164688[cameraId] = arg1;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByPlayerId[playerId] + 10;
    gNearestPathPointByCameraId[cameraId] = (gNearestPathPointByCameraId[cameraId]) % gPathCountByPathIndex[pathIndex];

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], arg1, pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;
    D_80164638[cameraId] = gOffsetPosition[2];

    D_80164648[cameraId] = gPlayers[playerId].speed / 5.0f;
    if ((f64) D_80164648[cameraId] < 0.0) {
        D_80164648[cameraId] = 0.0f;
    }

    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_80015C94(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 playerWaypoint;
    s16 cameraWaypoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    s32 playerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    UNUSED s32 stackPadding1;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f20;
    f32 temp_f2_2;
    s32 pathIndex;

    playerId = camera->playerId;
    player = gPlayerOne;
    player += playerId;
    D_80163238 = playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    playerWaypoint = gNearestPathPointByPlayerId[playerId];
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x0032U, 0x000FU, gPathCountByPathIndex[pathIndex]) <=
        0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else if ((gTrackPositionFactor[playerId] < -0.5) && (D_80164688[cameraId] < -0.5)) {
        func_8001A348(cameraId, 1.0f, 7);
    } else if ((gTrackPositionFactor[playerId] > 0.5) && (D_80164688[cameraId] > 0.5)) {
        func_8001A348(cameraId, -1.0f, 6);
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % gPathCountByPathIndex[pathIndex];
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % gPathCountByPathIndex[pathIndex];
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    midX = gOffsetPosition[0] * 0.5;
    midZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    midX += gOffsetPosition[0] * 0.5;
    midZ += gOffsetPosition[2] * 0.5;
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) / 2.0;
    xdiff = midX - D_801645F8[cameraId];
    ydiff = midY - D_80164618[cameraId];
    zdiff = midZ - D_80164638[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = D_801645F8[cameraId] + ((D_80164648[cameraId] * xdiff) / distance);
        midY = D_80164618[cameraId] + ((D_80164648[cameraId] * ydiff) / distance);
        midZ = D_80164638[cameraId] + ((D_80164648[cameraId] * zdiff) / distance);
    } else {
        midX = D_801645F8[cameraId];
        midY = D_80164618[cameraId];
        midZ = D_80164638[cameraId];
    }
    camera->pos[0] = midX;
    camera->pos[2] = midZ;
    temp_f2_2 = spawn_actor_on_surface(midX, (midY + 30.0), midZ);
    if ((temp_f2_2 < (midY - 20.0)) || (temp_f2_2 >= 3000.0)) {
        camera->pos[1] = midY + 10.0;
    } else {
        camera->pos[1] = temp_f2_2 + 8.0;
    }
    D_801645F8[cameraId] = midX;
    D_80164618[cameraId] = midY;
    D_80164638[cameraId] = midZ;
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    xdiff2 = camera->lookAt[0] - camera->pos[0];
    ydiff2 = camera->lookAt[1] - camera->pos[1];
    zdiff2 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff2, zdiff2);
    camera->rot[0] = atan2s(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->rot[2] = 0;
}

void func_800162CC(s32 playerId, f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;

    D_80164688[cameraId] = arg1;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByPlayerId[playerId];
    gNearestPathPointByCameraId[cameraId] = (gNearestPathPointByCameraId[cameraId]) % gPathCountByPathIndex[pathIndex];

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], arg1, pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;
    D_80164638[cameraId] = gOffsetPosition[2];

    D_80164658[cameraId] = gPlayers[playerId].speed;
    D_80164648[cameraId] = gPlayers[playerId].speed;

    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_80016494(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    s32 playerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    UNUSED f32 sp98;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED f32 sp94;
    UNUSED f32 sp90;
    UNUSED s32 stackPadding1;
    s32 pathIndex;
    f32 temp_f2_5;

    playerId = camera->playerId;
    player = gPlayerOne;
    D_80164648[cameraId] += ((D_80164658[cameraId] - D_80164648[cameraId]) * 0.5f);
    D_80163238 = playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    player += playerId;
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    temp_f2_5 = (gTrackPositionFactor[playerId] - D_80164688[cameraId]);
    temp_f2_5 *= temp_f2_5;
    playerWaypoint = gNearestPathPointByPlayerId[playerId];
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x000FU, 0x000FU, gPathCountByPathIndex[pathIndex]) <=
        0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else {
        if ((gTrackPositionFactor[playerId] < 0.0) && (D_80164688[cameraId] < 0.0) && (temp_f2_5 < 0.01)) {
            func_8001A348(cameraId, 1.0f, 5);
        } else {
            if ((gTrackPositionFactor[playerId] > 0.0) && (D_80164688[cameraId] > 0.0) && (temp_f2_5 < 0.01)) {
                func_8001A348(cameraId, -1.0f, 4);
            } else {
                if ((cameraWaypoint < playerWaypoint) && ((playerWaypoint - cameraWaypoint) < 0xA)) {
                    D_80164658[cameraId] = gPlayers[playerId].speed + 0.4;
                }
                if ((playerWaypoint < cameraWaypoint) && ((cameraWaypoint - playerWaypoint) < 0xA)) {
                    D_80164658[cameraId] = gPlayers[playerId].speed - 0.4;
                }
                if (D_80164658[cameraId] > 10.0) {
                    D_80164658[cameraId] = 10.0f;
                }
                if (D_80164658[cameraId] < 0.0) {
                    D_80164658[cameraId] = 0.0f;
                }
            }
        }
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % gPathCountByPathIndex[pathIndex];
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % gPathCountByPathIndex[pathIndex];
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    midX = gOffsetPosition[0] * 0.5;
    midZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    midX += gOffsetPosition[0] * 0.5;
    midZ += gOffsetPosition[2] * 0.5;
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) / 2.0;
    xdiff = midX - D_801645F8[cameraId];
    ydiff = midY - D_80164618[cameraId];
    zdiff = midZ - D_80164638[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = D_801645F8[cameraId] + ((D_80164648[cameraId] * xdiff) / distance);
        midY = D_80164618[cameraId] + ((D_80164648[cameraId] * ydiff) / distance);
        midZ = D_80164638[cameraId] + ((D_80164648[cameraId] * zdiff) / distance);
    } else {
        midX = D_801645F8[cameraId];
        midY = D_80164618[cameraId];
        midZ = D_80164638[cameraId];
    }
    camera->pos[0] = midX;
    camera->pos[2] = midZ;
    temp_f2_5 = spawn_actor_on_surface(midX, midY + 30.0, midZ);
    if ((temp_f2_5 < (midY - 20.0)) || (temp_f2_5 >= 3000.0)) {
        camera->pos[1] = midY + 10.0;
    } else {
        camera->pos[1] = temp_f2_5 + 10.0;
    }
    D_801645F8[cameraId] = midX;
    D_80164618[cameraId] = midY;
    D_80164638[cameraId] = midZ;
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    xdiff2 = camera->lookAt[0] - camera->pos[0];
    ydiff2 = camera->lookAt[1] - camera->pos[1];
    zdiff2 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff2, zdiff2);
    camera->rot[0] = atan2s(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->rot[2] = 0;
}

void func_80016C3C(UNUSED s32 playerId, UNUSED f32 arg1, s32 cameraId) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    f32 temp_f2;
    UNUSED s32 stackPadding2;
    f32 temp_f12;
    TrackPathPoint** path;
    f32 sp54;
    s32 temp_s0;
    s16 sp48;
    s16 sp44;
    Camera* camera;

    if (random_int(0x0064U) < 0x32) {
        D_80164688[cameraId] = 0.1f;
    } else {
        D_80164688[cameraId] = -0.1f;
    }
    D_80163DD8[cameraId] = 0;
    if (IsYoshiValley()) {
        D_80163DD8[cameraId] = random_int(4U);
        D_80164688[cameraId] = 0.0f;
    }
    temp_s0 = gPathCountByPathIndex[D_80163DD8[cameraId]];
    gNearestPathPointByCameraId[cameraId] %= temp_s0;
    set_track_offset_position(gNearestPathPointByCameraId[cameraId], D_80164688[cameraId], 0);
    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = gTrackPaths[0][gNearestPathPointByCameraId[cameraId]].y;
    path = gTrackPaths;
    D_80164638[cameraId] = gOffsetPosition[2];
    D_80164658[cameraId] = 16.666666f;
    D_80164648[cameraId] = 0.0f;
    sp48 = (gNearestPathPointByCameraId[cameraId] + 0xA) % temp_s0;
    sp44 = (gNearestPathPointByCameraId[cameraId] + 0xB) % temp_s0;
    set_track_offset_position(sp48, D_80164688[cameraId], 0);
    temp_f2 = gOffsetPosition[0] * 0.5;
    temp_f12 = gOffsetPosition[2] * 0.5;
    set_track_offset_position(sp44, D_80164688[cameraId], 0);
    temp_f2 += gOffsetPosition[0] * 0.5;
    temp_f12 += gOffsetPosition[2] * 0.5;
    sp48 = (gNearestPathPointByCameraId[cameraId] + 0x5) % temp_s0;
    sp44 = (gNearestPathPointByCameraId[cameraId] + 0x6) % temp_s0;
    sp54 = (path[0][sp48].y + path[0][sp44].y) * 0.5f;
    camera = cameras;
    camera += cameraId;
    camera->lookAt[0] = temp_f2;
    camera->lookAt[2] = temp_f12;
    camera->lookAt[1] = sp54 + 8.0;
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_80017054(Camera* camera, UNUSED Player* player, UNUSED s32 index, s32 cameraId);
void func_80017054(Camera* camera, UNUSED Player* player, UNUSED s32 index, s32 cameraId) {
    s32 stackPadding0;
    s32 stackPadding1;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    s32 stackPadding2;
    s32 stackPadding3;
    f32 camX;
    f32 camY;
    f32 camZ;
    f32 lookAtX;
    f32 lookAtY;
    f32 lookAtZ;
    f32 diffX;
    f32 diffY;
    f32 diffZ;
    s32 stackPadding7;
    s32 stackPadding8;
    s16 waypoint1;
    s16 waypoint2;
    f32 stackPadding9;
    s32 playerId;
    f32 distance;
    s32 pathIndex;
    s32 sp58;
    s16 sp56;

    playerId = camera->playerId;
    D_80164648[cameraId] += (D_80164658[cameraId] - D_80164648[cameraId]) * 0.5f;
    pathIndex = D_80163DD8[cameraId];
    sp58 = gPathCountByPathIndex[pathIndex];
    D_80163238 = playerId;
    sp56 = gNearestPathPointByCameraId[cameraId];
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    if (IsYoshiValley()) {
        if ((sp56 != gNearestPathPointByCameraId[cameraId]) && (gNearestPathPointByCameraId[cameraId] == 1)) {
            D_80163DD8[cameraId] = random_int(4);
            pathIndex = D_80163DD8[cameraId];
            gNearestPathPointByCameraId[cameraId] = func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2],
                                                                  gNearestPathPointByCameraId[cameraId], pathIndex);
        }
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 0xA) % sp58;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 0xB) % sp58;
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    lookAtX = gOffsetPosition[0] * 0.5;
    lookAtZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    lookAtX += gOffsetPosition[0] * 0.5;
    lookAtZ += gOffsetPosition[2] * 0.5;

    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 5) % sp58;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 6) % sp58;
    lookAtY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) * 0.5f;
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % sp58;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % sp58;
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    camX = gOffsetPosition[0] * 0.5;
    camZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    camX += gOffsetPosition[0] * 0.5;
    camZ += gOffsetPosition[2] * 0.5;
    camY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) * 0.5f;

    diffX = camX - D_801645F8[cameraId];
    diffY = camY - D_80164618[cameraId];
    diffZ = camZ - D_80164638[cameraId];

    distance = sqrtf(((diffX * diffX) + (diffY * diffY)) + (diffZ * diffZ));
    if (distance != 0.0) {
        camX = D_801645F8[cameraId] + (((stackPadding9 = D_80164648[cameraId]) * diffX) / distance);
        camY = D_80164618[cameraId] + ((D_80164648[cameraId] * diffY) / distance);
        camZ = D_80164638[cameraId] + ((D_80164648[cameraId] * diffZ) / distance);
    } else {
        camX = D_801645F8[cameraId];
        camY = D_80164618[cameraId];
        camZ = D_80164638[cameraId];
    }

    if (camX < -10000.0 || camX > 10000.0) {
        if (lookAtX < -10000.0 || lookAtX > 10000.0) {}
    }
    camera->pos[0] = camX;
    camera->pos[2] = camZ;
    camera->pos[1] = camY + 10.0;

    D_801645F8[cameraId] = camX;
    D_80164618[cameraId] = camY;
    D_80164638[cameraId] = camZ;

    if (lookAtX < -10000.0 || lookAtX > 10000.0) {}
    if (lookAtZ < -10000.0 || lookAtZ > 10000.0) {}
    camera->lookAt[0] = lookAtX;
    camera->lookAt[1] = lookAtY + 8.0;
    camera->lookAt[2] = lookAtZ;
    func_80014D30(cameraId, pathIndex);
    spAC = camera->lookAt[0] - camera->pos[0];
    spA8 = camera->lookAt[1] - camera->pos[1];
    spA4 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(spAC, spA4);
    camera->rot[0] = atan2s(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    camera->rot[2] = 0;
}

void func_80017720(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    UNUSED s32 pad;

    D_80164688[cameraId] = gTrackPositionFactor[playerId];
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByPlayerId[playerId] + 3;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByCameraId[cameraId] % gPathCountByPathIndex[pathIndex];

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], gTrackPositionFactor[playerId], pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;
    D_80164638[cameraId] = gOffsetPosition[2];

    D_80164658[cameraId] = gPlayers[playerId].speed;
    D_80164648[cameraId] = gPlayers[playerId].speed;

    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_800178F4(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED f32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    f32 distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED f32 var_f2;
    UNUSED f32 var_f18;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED f32 stackPadding1;
    UNUSED f32 var_f14;
    s32 playerId;
    s32 pathIndex;
    s32 waypointCount;

    playerId = camera->playerId;
    player = gPlayerOne;
    D_80164688[cameraId] = gTrackPositionFactor[playerId];
    D_80164648[cameraId] += ((D_80164658[cameraId] - D_80164648[cameraId]) / 2.0f);
    D_80163238 = playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    player += playerId;
    waypointCount = gPathCountByPathIndex[pathIndex];
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    playerWaypoint = (gNearestPathPointByPlayerId[playerId] + 3) % waypointCount;
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x000DU, 1U, waypointCount) <= 0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else {
        if ((cameraWaypoint < playerWaypoint) && ((playerWaypoint - cameraWaypoint) < 3)) {
            D_80164658[cameraId] = gPlayers[playerId].speed + 0.1;
        }
        if ((playerWaypoint < cameraWaypoint) && ((cameraWaypoint - playerWaypoint) < 3)) {
            D_80164658[cameraId] = gPlayers[playerId].speed - 0.1;
        }
        if (D_80164658[cameraId] > 10.0) {
            D_80164658[cameraId] = 10.0f;
        }
        if (D_80164658[cameraId] < 0.0) {
            D_80164658[cameraId] = 0.0f;
        }
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % waypointCount;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % waypointCount;
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    midX = gOffsetPosition[0] * 0.5;
    midZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    midX += gOffsetPosition[0] * 0.5;
    midZ += gOffsetPosition[2] * 0.5;
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) / 2.0;
    xdiff = midX - D_801645F8[cameraId];
    ydiff = midY - D_80164618[cameraId];
    zdiff = midZ - D_80164638[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = D_801645F8[cameraId] + ((D_80164648[cameraId] * xdiff) / distance);
        midY = D_80164618[cameraId] + ((D_80164648[cameraId] * ydiff) / distance);
        midZ = D_80164638[cameraId] + ((D_80164648[cameraId] * zdiff) / distance);
    } else {
        midX = D_801645F8[cameraId];
        midY = D_80164618[cameraId];
        midZ = D_80164638[cameraId];
    }
    camera->pos[0] = midX;
    camera->pos[2] = midZ;
    camera->pos[1] = midY + 10.0;
    D_801645F8[cameraId] = midX;
    D_80164618[cameraId] = midY;
    D_80164638[cameraId] = midZ;
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    xdiff2 = camera->lookAt[0] - camera->pos[0];
    ydiff2 = camera->lookAt[1] - camera->pos[1];
    zdiff2 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff2, zdiff2);
    camera->rot[0] = atan2s(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->rot[2] = 0;
}

void func_80017F10(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = gPathCountByPathIndex[pathIndex];

    D_80164688[cameraId] = gTrackPositionFactor[playerId];
    gNearestPathPointByCameraId[cameraId] = (gNearestPathPointByPlayerId[playerId] + test) - 2;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByCameraId[cameraId] % test;

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], gTrackPositionFactor[playerId], pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;
    D_80164638[cameraId] = gOffsetPosition[2];

    D_80164658[cameraId] = gPlayers[playerId].speed;
    D_80164648[cameraId] = gPlayers[playerId].speed;

    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_800180F0(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED s32 stackPadding3;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    UNUSED s32 stackPadding4;
    f32 distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    Player* player;
    s32 playerId;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED f32 var_f2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f14;
    s32 pathIndex;
    s32 waypointCount;

    playerId = camera->playerId;
    player = gPlayerOne;
    D_80164688[cameraId] = gTrackPositionFactor[playerId];
    D_80164648[cameraId] += ((D_80164658[cameraId] - D_80164648[cameraId]) * 0.5f);
    D_80163238 = playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    waypointCount = gPathCountByPathIndex[pathIndex];
    player += playerId;
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    playerWaypoint = ((gNearestPathPointByPlayerId[playerId] + waypointCount) - 2) % waypointCount;
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 1U, 0x000AU, waypointCount) <= 0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else {
        if ((cameraWaypoint < playerWaypoint) && ((playerWaypoint - cameraWaypoint) < 3)) {
            D_80164658[cameraId] = gPlayers[playerId].speed + 0.1;
        }
        if ((playerWaypoint < cameraWaypoint) && ((cameraWaypoint - playerWaypoint) < 3)) {
            D_80164658[cameraId] = gPlayers[playerId].speed - 0.1;
        }
        if (D_80164658[cameraId] > 10.0) {
            D_80164658[cameraId] = 10.0f;
        }
        if (D_80164658[cameraId] < 0.0) {
            D_80164658[cameraId] = 0.0f;
        }
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % waypointCount;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % waypointCount;
    set_track_offset_position(waypoint1, D_80164688[cameraId], pathIndex);
    midX = gOffsetPosition[0] * 0.5;
    midZ = gOffsetPosition[2] * 0.5;
    set_track_offset_position(waypoint2, D_80164688[cameraId], pathIndex);
    midX += gOffsetPosition[0] * 0.5;
    midZ += gOffsetPosition[2] * 0.5;
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) / 2.0;
    xdiff = midX - D_801645F8[cameraId];
    ydiff = midY - D_80164618[cameraId];
    zdiff = midZ - D_80164638[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = D_801645F8[cameraId] + ((D_80164648[cameraId] * xdiff) / distance);
        midY = D_80164618[cameraId] + ((D_80164648[cameraId] * ydiff) / distance);
        midZ = D_80164638[cameraId] + ((D_80164648[cameraId] * zdiff) / distance);
    } else {
        midX = D_801645F8[cameraId];
        midY = D_80164618[cameraId];
        midZ = D_80164638[cameraId];
    }
    camera->pos[0] = midX;
    camera->pos[2] = midZ;
    camera->pos[1] = player->pos[1] + 10.0;
    D_801645F8[cameraId] = midX;
    D_80164618[cameraId] = midY;
    D_80164638[cameraId] = midZ;
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    sp94 = camera->lookAt[0] - camera->pos[0];
    sp90 = camera->lookAt[1] - camera->pos[1];
    sp8C = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(sp94, sp8C);
    camera->rot[0] = atan2s(sqrtf((sp94 * sp94) + (sp8C * sp8C)), sp90);
    camera->rot[2] = 0;
}

void func_80018718(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = gPathCountByPathIndex[pathIndex];

    D_80164688[cameraId] = gTrackPositionFactor[playerId];
    gNearestPathPointByCameraId[cameraId] = ((gNearestPathPointByPlayerId[playerId] + test) - 5) % test;

    calculate_track_offset_position(gNearestPathPointByCameraId[cameraId], gTrackPositionFactor[playerId], 60.0f,
                                    pathIndex);

    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164618[cameraId] = (f32) gTrackPaths[pathIndex][gNearestPathPointByCameraId[cameraId]].y;
    D_80164638[cameraId] = gOffsetPosition[2];

    D_80164658[cameraId] = gPlayers[playerId].speed;
    D_80164648[cameraId] = gPlayers[playerId].speed;
    D_8016448C = 1;
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_800188F4(Camera* camera, UNUSED Player* unusePlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED s32 stackPadding0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    Player* player;
    f32 distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED f32 sp64;
    UNUSED f32 sp60;
    s16 waypoint1;
    s16 waypoint2;
    UNUSED f32 sp5C;
    f32 temp_f2_4;
    s32 playerId;
    s32 pathIndex;
    s32 waypointCount;

    player = gPlayerOne;
    playerId = camera->playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    waypointCount = gPathCountByPathIndex[pathIndex];
    D_80164648[cameraId] = gPlayers[playerId].speed;
    if (D_8016448C == 0) {
        if (gNearestPathPointByCameraId[cameraId] ==
            (((gNearestPathPointByPlayerId[playerId] + waypointCount) - 6) % waypointCount)) {
            D_8016448C = 1;
        }
        if (D_80164688[cameraId] < (gTrackPositionFactor[playerId] - 0.2)) {
            D_80164648[cameraId] = gPlayers[playerId].speed * 0.7;
        }
        if ((gTrackPositionFactor[playerId] - 0.5) < D_80164688[cameraId]) {
            D_80164688[cameraId] -= 0.01;
        }
        if (D_80164688[cameraId] < -0.9) {
            D_80164688[cameraId] = -0.9f;
            D_80164648[cameraId] = gPlayers[playerId].speed * 0.8;
        }
    } else {
        if (gNearestPathPointByCameraId[cameraId] == ((gNearestPathPointByPlayerId[playerId] + 6) % waypointCount)) {
            D_8016448C = 0;
        }
        if ((gTrackPositionFactor[playerId] + 0.2) < D_80164688[cameraId]) {
            D_80164648[cameraId] = gPlayers[playerId].speed * 1.3;
        }
        if (D_80164688[cameraId] < (gTrackPositionFactor[playerId] + 0.5)) {
            D_80164688[cameraId] += 0.01;
        }
        if (D_80164688[cameraId] > 0.9) {
            D_80164688[cameraId] = 0.9f;
            D_80164648[cameraId] = gPlayers[playerId].speed * 1.2;
        }
    }
    D_80163238 = playerId;
    player += playerId;
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], pathIndex);
    playerWaypoint = gNearestPathPointByPlayerId[playerId];
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x000FU, 0x000FU, waypointCount) <= 0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    }
    waypoint1 = (gNearestPathPointByCameraId[cameraId] + 1) % waypointCount;
    waypoint2 = (gNearestPathPointByCameraId[cameraId] + 2) % waypointCount;
    calculate_track_offset_position(waypoint1, D_80164688[cameraId], 60.0f, pathIndex);
    midX = gOffsetPosition[0] * 0.5;
    midZ = gOffsetPosition[2] * 0.5;
    calculate_track_offset_position(waypoint2, D_80164688[cameraId], 60.0f, pathIndex);
    midX += gOffsetPosition[0] * 0.5;
    midZ += gOffsetPosition[2] * 0.5;
    midY = (gTrackPaths[pathIndex][waypoint1].y + gTrackPaths[pathIndex][waypoint2].y) / 2.0;
    xdiff = midX - D_801645F8[cameraId];
    ydiff = midY - D_80164618[cameraId];
    zdiff = midZ - D_80164638[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = D_801645F8[cameraId] + ((D_80164648[cameraId] * xdiff) / distance);
        midY = D_80164618[cameraId] + ((D_80164648[cameraId] * ydiff) / distance);
        midZ = D_80164638[cameraId] + ((D_80164648[cameraId] * zdiff) / distance);
    } else {
        midX = D_801645F8[cameraId];
        midY = D_80164618[cameraId];
        midZ = D_80164638[cameraId];
    }
    camera->pos[0] = midX;
    camera->pos[2] = midZ;
    temp_f2_4 = spawn_actor_on_surface(midX, midY + 30.0, midZ);
    if ((temp_f2_4 < (midY - 20.0)) || (temp_f2_4 >= 3000.0)) {
        camera->pos[1] = midY + 10.0;
    } else {
        camera->pos[1] = temp_f2_4 + 8.0;
    }
    D_801645F8[cameraId] = midX;
    D_80164618[cameraId] = midY;
    D_80164638[cameraId] = midZ;
    camera->lookAt[0] = player->pos[0];
    camera->lookAt[1] = player->pos[1] + 6.0;
    camera->lookAt[2] = player->pos[2];
    func_80014D30(cameraId, pathIndex);
    spAC = camera->lookAt[0] - camera->pos[0];
    spA8 = camera->lookAt[1] - camera->pos[1];
    spA4 = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(spAC, spA4);
    camera->rot[0] = atan2s(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    camera->rot[2] = 0;
}

void func_80019118(s32 playerId, f32 arg1, s32 cameraId, UNUSED s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = gPathCountByPathIndex[0];
    f32 temp_f12;
    f32 temp_f2;

    D_80164688[cameraId] = arg1;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByPlayerId[playerId] + 12;
    gNearestPathPointByCameraId[cameraId] = gNearestPathPointByCameraId[cameraId] % test;

    set_track_offset_position(gNearestPathPointByCameraId[cameraId], arg1, 0);
    D_801645F8[cameraId] = gOffsetPosition[0];
    D_80164638[cameraId] = gOffsetPosition[2];
    temp_f2 = (f32) gTrackPaths[0][gNearestPathPointByCameraId[cameraId]].y;

    temp_f12 = spawn_actor_on_surface(gOffsetPosition[0], (f32) (temp_f2 + 30.0), gOffsetPosition[2]);

    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        D_80164618[cameraId] = (f32) (temp_f2 + 10.0);
    } else {
        D_80164618[cameraId] = (f32) (temp_f12 + 10.0);
    }
    D_80164648[cameraId] = 0.0f;
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
}

void func_8001933C(Camera* camera, UNUSED Player* playerArg, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraWaypoint;
    s16 playerWaypoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 pad2;
    s32 playerId;
    UNUSED s32 pad3[10];
    Player* player;
    TrackPathPoint* waypoint;
    s32 pathIndex;
    s32 waypointCount;

    playerId = camera->playerId;
    pathIndex = gPathIndexByPlayerId[playerId];
    player = gPlayerOne;
    player += playerId;
    waypointCount = gPathCountByPathIndex[pathIndex];
    gNearestPathPointByCameraId[cameraId] =
        func_8000D33C(camera->pos[0], camera->pos[1], camera->pos[2], gNearestPathPointByCameraId[cameraId], 0);
    playerWaypoint = gNearestPathPointByPlayerId[playerId];
    cameraWaypoint = gNearestPathPointByCameraId[cameraId];
    if (is_path_point_in_range(playerWaypoint, cameraWaypoint, 0x0032U, 0x0014U, waypointCount) <= 0) {
        func_8001A348(cameraId, D_80164688[cameraId], D_80164680[cameraId]);
    } else {
        if ((gTrackPositionFactor[playerId] < -0.5) && ((f64) D_80164688[cameraId] < -0.5)) {
            func_8001A348(cameraId, 1.0f, 0x0000000D);
        } else if ((gTrackPositionFactor[playerId] > 0.5) && ((f64) D_80164688[cameraId] > 0.5)) {
            func_8001A348(cameraId, -1.0f, 0x0000000C);
        }
    }
    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
    waypoint = &gTrackPaths[pathIndex][cameraWaypoint];
    camera->lookAt[0] = (player->pos[0] * 0.8) + (0.2 * waypoint->x);
    camera->lookAt[1] = (player->pos[1] * 0.8) + (0.2 * waypoint->y);
    camera->lookAt[2] = (player->pos[2] * 0.8) + (0.2 * waypoint->z);
    func_80014D30(cameraId, pathIndex);
    xdiff = camera->lookAt[0] - camera->pos[0];
    ydiff = camera->lookAt[1] - camera->pos[1];
    zdiff = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff, zdiff);
    camera->rot[0] = atan2s(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->rot[2] = 0;
}

void func_8001968C(void) {
    D_80164678[0] = 3;
}

void func_8001969C(UNUSED s32 playerId, UNUSED f32 arg1, s32 cameraId, UNUSED s16 pathIndex) {
    TrackPathPoint* waypoint;

    gNearestPathPointByCameraId[cameraId] = gPathCountByPathIndex[0] - 18;

    waypoint = &gTrackPaths[0][gNearestPathPointByCameraId[cameraId]];

    D_801645F8[cameraId] = waypoint->x;
    //! @bug Adding an (f32) cast changes asm, why?
    D_80164618[cameraId] = waypoint->y + 10.0;
    D_80164638[cameraId] = waypoint->z;
    D_80164648[cameraId] = 0.0f;
    D_80164678[cameraId] = 0;
}

void func_80019760(Camera* camera, UNUSED Player* player, UNUSED s32 arg2, s32 cameraId) {
    UNUSED s32 pad[2];
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    TrackPathPoint* temp_v1;

    camera->pos[0] = D_801645F8[cameraId];
    camera->pos[1] = D_80164618[cameraId];
    camera->pos[2] = D_80164638[cameraId];
    temp_v1 = &(*gTrackPaths)[gNearestPathPointByCameraId[cameraId]];
    camera->lookAt[0] = (f32) temp_v1->x;
    camera->lookAt[1] = (f32) temp_v1->y;
    camera->lookAt[2] = (f32) temp_v1->z;
    func_80014D30(cameraId, 0);
    xdiff = camera->lookAt[0] - camera->pos[0];
    ydiff = camera->lookAt[1] - camera->pos[1];
    zdiff = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(xdiff, zdiff);
    camera->rot[0] = atan2s(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->rot[2] = 0;
}

void func_80019890(s32 playerId, s32 cameraId) {
    s32 pathIndex;
    Camera* camera = camera1;
    camera += cameraId;
    camera->playerId = playerId;

    D_801646C0[cameraId] = 0;
    pathIndex = gPathIndexByPlayerId[playerId];

    switch (D_80164680[cameraId]) {
        case 0:
            func_80015314(playerId, 0.0f, cameraId);
            break;
        case 2:
            func_80015544(playerId, -1.0f, cameraId, pathIndex);
            break;
        case 3:
            func_80015544(playerId, 1.0f, cameraId, pathIndex);
            break;
        case 6:
            func_80015A9C(playerId, -0.6f, cameraId, (s16) pathIndex);
            break;
        case 7:
            func_80015A9C(playerId, 0.6f, cameraId, (s16) pathIndex);
            break;
        case 4:
            func_800162CC(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 5:
            func_800162CC(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        case 9:
            func_80016C3C(playerId, 0.0f, cameraId);
            break;
        case 1:
            func_80017720(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 14:
            func_80017F10(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 8:
            func_80018718(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 12:
            func_80019118(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 13:
            func_80019118(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        case 15:
            func_8001969C(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 16:
            func_8001969C(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        default:
            func_80015314(playerId, 0.0f, cameraId);
            break;
    }
    gNearestPathPointByCameraId[cameraId] =
        func_8000BD94(camera->pos[0], camera->pos[1], camera->pos[2], (s32) pathIndex);
    if ((s16) D_80164680[cameraId] == 9) {
        D_80163DD8[cameraId] = (s32) pathIndex;
    }
}

void func_80019B50(s32 cameraIndex, u16 arg1) {
    u16 var_v0;

    var_v0 = D_801646C0[cameraIndex];
    if (var_v0 < arg1) {
        if ((arg1 - var_v0) < 0x8000) {
            var_v0 += 0x5A;
            if (arg1 < var_v0) {
                var_v0 = arg1;
            }
            if (var_v0 < 0x5A) {
                var_v0 = 0;
            }
        } else if ((arg1 - var_v0) >= 0x8000) {
            var_v0 -= 0x5A;
        }
    } else if (arg1 < var_v0) {
        if ((var_v0 - arg1) < 0x8000) {
            var_v0 -= 0x5A;
            if (var_v0 < arg1) {
                var_v0 = arg1;
            }
            if (var_v0 >= 0xFFA6) {
                var_v0 = 0;
            }
        } else if ((var_v0 - arg1) >= 0x8000) {
            var_v0 += 0x5A;
            if (!cameraIndex) {} // wtf?
        }
    }
    D_801646C0[cameraIndex] = (s16) var_v0;
}

void func_80019C50(s32 arg0) {
    switch (D_80164678[arg0]) {
        case 0:
            if (D_80164608[arg0] == 1) {
                D_80164678[arg0] = 1;
                func_800C9060(arg0, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x4F));
                D_80164670[arg0] = D_80164678[arg0];
            }
            break;
        case 1:
            if (D_80164608[arg0] == 1) {
                D_80164678[arg0] = 0;
                func_800C9060(arg0, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x50));
                D_80164670[arg0] = D_80164678[arg0];
            }
            break;
    }
}

void look_behind_toggle(s32 cameraIdx) {
    static bool lookBehindActive[NUM_CAMERAS] = {0};
    bool pressed = gControllers[cameraIdx].button & L_CBUTTONS; // button held
    Camera* camera = &cameras[cameraIdx];
    struct UnkStruct_800DC5EC* screenCtx = NULL;

    if (CVarGetInteger("gLookBehind", false) == false) {
        return;
    }

    if (cameras[cameraIdx].playerId < 0 || cameras[cameraIdx].playerId >= 4) {
        return;
    }

    // Get the screen context
    screenCtx = &D_8015F480[cameras[cameraIdx].playerId];

    if (gRaceState == RACE_IN_PROGRESS) {
        // Flip the camera
        if (pressed && !lookBehindActive[cameraIdx]) {
            screenCtx->pendingCamera = screenCtx->lookBehindCamera;
            lookBehindActive[cameraIdx] = true;
        // Unflip the camera
        } else if (!pressed && lookBehindActive[cameraIdx]) {
            screenCtx->pendingCamera = screenCtx->raceCamera;
            lookBehindActive[cameraIdx] = false;
        }
    } else if (lookBehindActive[cameraIdx]) {
        // Restore the original camera
        screenCtx->pendingCamera = screenCtx->raceCamera;
        lookBehindActive[cameraIdx] = false;
    }
}

void func_80019D2C(Camera* camera, Player* player, s32 arg2) {
    s32 playerId;
    s32 nearestWaypoint;

    playerId = camera->playerId;
    if ((gIncrementUpdatePlayer != 0) && (IsLuigiRaceway())) {
        calculate_camera_up_vector(camera, arg2);
        nearestWaypoint = gNearestPathPointByPlayerId[playerId];
        if (((nearestWaypoint >= 0x65) && (nearestWaypoint < 0xFA)) ||
            ((nearestWaypoint >= 0x1AF) && (nearestWaypoint < 0x226))) {
            func_80019B50(arg2, (player->unk_206 * 2));
        } else {
            func_80019B50(arg2, 0U);
        }
    }
}

void func_80019DE4(void) {
    D_801646CC = 1;
}

void func_80019DF4(void) {
    s32 i;
    s32 playerId = gGPCurrentRacePlayerIdByRank[0];
    // clang-format off
    // Has to be on a single line to match. Because IDO hates you :)
    for (i = 0; i < NUM_CAMERAS; i++) { D_80164670[i] = D_80164678[i]; }
    // clang-format on
    camera1->playerId = playerId;
    D_80164678[0] = 1;
    D_801646CC = 2;
}

void func_80019E58(void) {
    D_80164680[0] = 1;
    func_80019890(0, 0);
    D_80164670[0] = D_80164678[0];
    D_80164678[0] = 1;
    D_80164680[1] = 9;
    func_80019890(0, 1);
    D_80164670[1] = D_80164678[1];
    D_80164678[1] = 0;
}

void func_80019ED0(void) {
    s32 i;

    for (i = 0; i < NUM_CAMERAS; i++) {
        D_80164670[i] = D_80164678[i];
    }

    gGPCurrentRacePlayerIdByRank[0] = (s16) gPlayerWinningIndex;

    camera1->playerId = (s16) gPlayerWinningIndex;

    for (i = 0; i < NUM_CAMERAS; i++) {
        D_80164680[i] = 0;
        func_80015314(gPlayerWinningIndex, 0, i);
        D_80164678[i] = 1;
    }
}

void func_80019FB4(s32 cameraId) {
    struct Controller* controller;

    controller = &gControllerOne[cameraId];
    if (controller->buttonPressed & 2) {
        D_801645D0[cameraId] += 1;
    } else {
        D_801645D0[cameraId] = 0;
    }
    if (controller->buttonPressed & 4) {
        D_801645E8[cameraId] += 1;
    } else {
        D_801645E8[cameraId] = 0;
    }
    if (controller->buttonPressed & 8) {
        D_80164608[cameraId] += 1;
    } else {
        D_80164608[cameraId] = 0;
    }
    if (controller->buttonPressed & 1) {
        D_80164628[cameraId] += 1;
    } else {
        D_80164628[cameraId] = 0;
    }
}

void func_8001A0A4(UNUSED u16* arg0, UNUSED Camera* arg1, UNUSED Player* arg2, UNUSED s8 arg3, s32 arg4) {
    func_80019FB4(arg4);
    func_80019C50(arg4);
    look_behind_toggle(arg4);
}

void func_8001A0DC(u16* arg0, Camera* arg1, Player* arg2, s8 arg3, s32 arg4) {
    func_8001A0A4(arg0, arg1, arg2, arg3, arg4);
    func_80019D2C(arg1, arg2, arg4);
}

void func_8001A124(s32 arg0, s32 arg1) {
    switch (gGPCurrentRaceRankByPlayerId[arg0]) { /* irregular */
        case 0:
            if (random_int(0x0064U) < 0x32) {
                D_80164680[arg1] = 0x000C;
            } else {
                D_80164680[arg1] = 0x000D;
            }
            func_800CA270();
            break;
        case 1:
        case 2:
        case 3:
            D_80164680[arg1] = 8;
            break;
        default:
            if (random_int(0x0064U) < 0x32) {
                D_80164680[arg1] = 0x000F;
            } else {
                D_80164680[arg1] = 0x0010;
            }
            break;
    }
}

void func_8001A220(UNUSED s32 arg0, s32 cameraId) {
    switch (random_int(6)) {
        case 0:
            D_80164680[cameraId] = 4;
            break;
        case 1:
            D_80164680[cameraId] = 5;
            break;
        case 2:
            D_80164680[cameraId] = 6;
            break;
        case 3:
            D_80164680[cameraId] = 7;
            break;
        case 4:
            D_80164680[cameraId] = 8;
            break;
        case 5:
            D_80164680[cameraId] = 1;
            break;
        default:
            D_80164680[cameraId] = 8;
            break;
    }
}

s32 func_8001A310(s32 waypoint, s32 arg1) {
    if ((IsBowsersCastle()) && (arg1 != 0) && (waypoint >= 0xE7) && (waypoint < 0x1C2)) {
        arg1 = 0;
    }
    return arg1;
}

void func_8001A348(s32 cameraId, f32 arg1, s32 arg2) {
    UNUSED s32 pad;
    s32 playerId;

    playerId = cameras[cameraId].playerId;
    D_80164688[cameraId] = arg1;
    D_80164680[cameraId] = func_8001A310((s32) gNearestPathPointByCameraId[cameraId], arg2);
    func_80019890(playerId, cameraId);
}

void func_8001A3D8(s32 arg0, f32 arg1, s32 arg2) {
    s32 playerId;

    playerId = cameras[arg0].playerId;
    D_80164688[arg0] = arg1;
    if (arg2 != D_80164680[arg0]) {
        D_80164680[arg0] = arg2;
        func_80019890(playerId, arg0);
    }
}

void func_8001A450(s32 playerId, s32 arg1, s32 arg2) {
    s32 temp_v1;
    s16 waypoint;
    s32 temp_v0;

    if (!(gPlayers[playerId].effects & 0x1000000C)) {
        temp_v1 = D_80164680[arg1];
        waypoint = gNearestPathPointByCameraId[arg1];
        temp_v0 = func_8001A310(waypoint, (temp_v1 + 1) % 10);
        if ((temp_v0 != temp_v1) || (arg2 != playerId)) {
            D_80164680[arg1] = temp_v0;
            func_80019890(arg2, arg1);
        }
    }
}

void func_8001A518(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg1) { /* irregular */
        case 0:
            D_80164680[arg2] = 1;
            break;
        case 1:
        case 2:
        case 3:
            func_8001A450(arg0, arg2, arg0);
            break;
        default:
            D_80164680[arg2] = 0;
            break;
    }
}

void func_8001A588(UNUSED u16* localD_80152300, Camera* camera, Player* player, s8 index, s32 cameraIndex) {
    s32 var_v1;
    UnkStruct_46D0* temp_v0_4;
    s32 sp44;
    s32 playerId;
    playerId = camera->playerId;

    if (gModeSelection == TIME_TRIALS) {
        playerId = 0;
    }
    func_80019FB4(cameraIndex);

    if ((s32) (camera->pos[0] * 10.0) == (s32) ((f64) camera->lookAt[0] * 10.0)) {

        if ((s32) (camera->pos[2] * 10.0) == (s32) ((f64) camera->lookAt[2] * 10.0)) {
            camera->pos[0] = (f32) (camera->pos[0] + 100.0);
            camera->pos[2] = (f32) (camera->pos[2] + 100.0);
        }
    }
    if ((gModeSelection != BATTLE) && (D_80164680[cameraIndex] == -1) && (player->type & 0x800) &&
        ((u16) D_801646CC == 0) && (D_801646C8 == 0)) {
        if (gModeSelection == VERSUS) {
            func_8001A220(playerId, cameraIndex);
        } else {
            func_8001A124((s32) playerId, cameraIndex);
        }
        func_80019890((s32) playerId, cameraIndex);
    }

    if ((D_80164680[cameraIndex] == 14) || (D_80164680[cameraIndex] == 0)) {
        func_80019D2C(camera, player, cameraIndex);
    } else {
        D_801646C0[cameraIndex] = 0;
        calculate_camera_up_vector(camera, cameraIndex);
    }
    switch ((u16) D_801646CC) {
        case 1:
            D_801646C8 += 1;
            if (D_801646C8 >= 501) {
                D_801646C8 = 0;
            }
            if ((cameraIndex == 0) && (((D_801646C8 == 10)) || (D_801646C8 == 11))) {
                func_8001A518((s32) playerId, gGPCurrentRaceRankByPlayerId[playerId], 0);
            }
            if ((gModeSelection != TIME_TRIALS) && (cameraIndex == 1) &&
                (((D_801646C8 == 260)) || (D_801646C8 == 261))) {

                var_v1 = 0;
                if (gPlayerCount == 2) {
                    func_8001A518((s32) playerId, gGPCurrentRaceRankByPlayerId[playerId], 1);
                } else {
                    sp44 = (s32) playerId;
                    while (var_v1 != 8) {
                        playerId += 1;
                        var_v1 += 1;
                        if (playerId >= 8) {
                            playerId = 1;
                        }
                        if ((!(gPlayers[playerId].unk_0CA & 2) && !(gPlayers[playerId].unk_0CA & 8))) {
                            break;
                        }
                    }
                    func_8001A450(sp44, cameraIndex, (s32) playerId);
                }
            }
            break;
        case 2:
            D_801646C8 += 1;
            if (D_801646C8 > 250) {
                D_801646C8 = 0;
            }
            if ((cameraIndex == 0) && (D_801646C8 == 10)) {
                func_8001A450((s32) playerId, cameraIndex, (s32) playerId);
            }
            break;
        default:
            temp_v0_4 = &D_801646D0[cameraIndex];
            if (temp_v0_4->unk0 == (s16) 1) {
                playerId = temp_v0_4->unk4;
                temp_v0_4->unk0 = 0;
                cameras[cameraIndex].playerId = playerId;
                func_8001A3D8(cameraIndex, 0.0f, (s32) temp_v0_4->unk2);
            }
            break;
    }
    func_80019C50(cameraIndex);
    look_behind_toggle(cameraIndex);
    switch (D_80164680[cameraIndex]) {
        case 0:
            func_80015390(camera, player, index);
            break;
        case 2:
        case 3:
            func_8001577C(camera, player, index, cameraIndex);
            break;
        case 6:
        case 7:
            func_80015C94(camera, player, index, cameraIndex);
            break;
        case 4:
        case 5:
            func_80016494(camera, player, index, cameraIndex);
            break;
        case 9:
            func_80017054(camera, player, index, cameraIndex);
            break;
        case 1:
            func_800178F4(camera, player, index, cameraIndex);
            break;
        case 14:
            func_800180F0(camera, player, index, cameraIndex);
            break;
        case 8:
            func_800188F4(camera, player, index, cameraIndex);
            break;
        case 12:
        case 13:
            func_8001933C(camera, player, index, cameraIndex);
            break;
        case 15:
        case 16:
            func_80019760(camera, player, index, cameraIndex);
            break;
        default:
            func_80015390(camera, player, index);
            break;
    }
}

void func_8001AAAC(s16 arg0, s16 arg1, s16 arg2) {
    if (D_801646D0[arg0].unk0 == 0) {
        D_801646D0[arg0].unk0 = 1;
        D_801646D0[arg0].unk2 = arg1;
        D_801646D0[arg0].unk4 = arg2;
    }
}

void func_8001AB00(void) {
    s32 var_v1;

    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++) {
        cpu_ItemStrategy[var_v1].branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
        cpu_ItemStrategy[var_v1].timer = 0;
        cpu_ItemStrategy[var_v1].actorIndex = -1;
        cpu_ItemStrategy[var_v1].numItemUse = 0;
        cpu_ItemStrategy[var_v1].numDroppedBananaBunch = 0;
    }
}

void cpu_decisions_branch_item(UNUSED s32 playerId, s16* branch, s32 itemId) {
    s32 value = -1;
    switch (itemId) {
        case ITEM_NONE:
            value = -1;
            break;
        case ITEM_FAKE_ITEM_BOX:
            value = CPU_STRATEGY_ITEM_FAKE_ITEM_BOX;
            break;
        case ITEM_BOO:
            value = CPU_STRATEGY_ITEM_BOO;
            break;
        case ITEM_BANANA:
            value = CPU_STRATEGY_ITEM_BANANA;
            break;
        case ITEM_THUNDERBOLT:
            value = CPU_STRATEGY_ITEM_THUNDERBOLT;
            break;
        case ITEM_STAR:
            value = CPU_STRATEGY_ITEM_STAR;
            break;
        case ITEM_MUSHROOM:
            value = CPU_STRATEGY_ITEM_MUSHROOM;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            break;
        case ITEM_TRIPLE_MUSHROOM:
            break;
        case ITEM_SUPER_MUSHROOM:
            break;
    }

    if (CVarGetInteger("gHarderCPU", 0) == 1) {
        switch (itemId) {
            case ITEM_NONE:
                value = -1;
                break;
            case ITEM_BANANA_BUNCH:
                value = CPU_STRATEGY_ITEM_BANANA_BUNCH;
                break;
            case ITEM_BLUE_SPINY_SHELL:
                value = CPU_STRATEGY_ITEM_BLUE_SPINY_SHELL;
                break;
            case ITEM_GREEN_SHELL:
                value = CPU_STRATEGY_ITEM_GREEN_SHELL;
                break;
            case ITEM_RED_SHELL:
                value = CPU_STRATEGY_ITEM_RED_SHELL;
                break;
            // case ITEM_TRIPLE_GREEN_SHELL:
            //     value = CPU_STRATEGY_ITEM_TRIPLE_GREEN_SHELL;
            //     break;
            // case ITEM_TRIPLE_RED_SHELL:
            //     value = CPU_STRATEGY_ITEM_TRIPLE_RED_SHELL;
            //     break;
            case ITEM_DOUBLE_MUSHROOM:
                value = CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM;
                break;
            case ITEM_TRIPLE_MUSHROOM:
                value = CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM;
                break;
            case ITEM_SUPER_MUSHROOM:
                value = CPU_STRATEGY_ITEM_SUPER_MUSHROOM;
                break;
        }
    }

    if (value >= 0) {
        *branch = value;
    }
}

void func_8001ABE0(UNUSED s32 arg0, UNUSED CpuItemStrategyData* arg1) {
}

void clear_expired_strategies(CpuItemStrategyData* arg0) {
    if ((arg0->actorIndex < 0) || (arg0->actorIndex >= 0x64)) {
        arg0->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
        arg0->timer = 0;
    }
}

void cpu_use_item_strategy(s32 playerId) {
    Player* player = &gPlayerOne[playerId];
    struct Actor* actor;
    CpuItemStrategyData* cpuStrategy = &cpu_ItemStrategy[playerId];
    TrackPathPoint* pathPoint;
    bool isValidBanana1;
    bool isValidBanana2;

// Only used in this function
#define BANANA_ACTOR(actor) ((struct BananaActor*) (actor))
#define SHELL_ACTOR(actor) ((struct ShellActor*) (actor))
#define FAKE_ITEMBOX_ACTOR(actor) ((struct FakeItemBox*) (actor))
#define BANANA_BUNCH_ACTOR(actor) ((struct BananaBunchParent*) (actor))

    if (gModeSelection == TIME_TRIALS) {
        return;
    }

    if ((u16) D_801646CC == 1) {
        return;
    }

    if (player->type & PLAYER_CINEMATIC_MODE) {
        return;
    }

    switch (cpuStrategy->branch) {
        case CPU_STRATEGY_WAIT_NEXT_ITEM:
            cpuStrategy->actorIndex = -1;

            // Harder CPU Items
            if (((gNumPathPointsTraversed[playerId] + (playerId * 20) + 100) % 8 == 0) && (cpuStrategy->timer >= 512) &&
                (CVarGetInteger("gHarderCPU", 0) == true)) {

                cpu_decisions_branch_item(playerId, &cpuStrategy->branch,
                                          hard_cpu_gen_random_item((s16) gLapCountByPlayerId[playerId],
                                                                   gGPCurrentRaceRankByPlayerId[playerId]));
                // Stock CPU Items
            } else if ((((playerId * 20) + 100) < gNumPathPointsTraversed[playerId]) && (cpuStrategy->timer >= 0x259) &&
                       (cpuStrategy->numItemUse < 3) && (gLapCountByPlayerId[playerId] < 3)) {
                cpu_decisions_branch_item(
                    playerId, &cpuStrategy->branch,
                    cpu_gen_random_item((s16) gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]));
            } else {
                func_8001ABE0(playerId, cpuStrategy);
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA:
            // never true
            if ((gLapCountByPlayerId[playerId] > 0) &&
                (gGPCurrentRaceRankByPlayerId[playerId] > gGPCurrentRaceRankByPlayerId[gBestRankedHumanPlayer]) &&
                (gGPCurrentRaceRankByPlayerId[gBestRankedHumanPlayer] == FIRST_PLACE)) {
                switch (player->characterId) {
                    case DK:
                        if (is_path_point_in_range(gNearestPathPointByPlayerId[playerId],
                                                   gNearestPathPointByPlayerId[gBestRankedHumanPlayer], 40, 2,
                                                   gSelectedPathCount) > 0) {
                            cpuStrategy->branch = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    case PEACH:
                        if (is_path_point_in_range(gNearestPathPointByPlayerId[playerId],
                                                   gNearestPathPointByPlayerId[gBestRankedHumanPlayer], 4, 2,
                                                   gSelectedPathCount) > 0) {
                            cpuStrategy->branch = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    default:
                        if (is_path_point_in_range(gNearestPathPointByPlayerId[playerId],
                                                   gNearestPathPointByPlayerId[gBestRankedHumanPlayer], 10, 2,
                                                   gSelectedPathCount) > 0) {
                            cpuStrategy->branch = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;
                }
            } else if (cpuStrategy->branch == CPU_STRATEGY_ITEM_BANANA) {
                cpuStrategy->actorIndex = use_banana_item(player);
                if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                    player->soundEffects |= HOLD_BANANA_SOUND_EFFECT;
                    cpuStrategy->branch = CPU_STRATEGY_HOLD_BANANA;
                    cpuStrategy->timer = 0;
                    cpuStrategy->numItemUse += 1;
                    cpuStrategy->timeBeforeThrow = (random_int(3) * 20) + 10;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->timer = 0;
                }
            }
            break;

        case CPU_STRATEGY_HOLD_BANANA:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((!(BANANA_ACTOR(actor)->flags & 0x8000)) || (BANANA_ACTOR(actor)->type != ACTOR_BANANA) ||
                (BANANA_ACTOR(actor)->state != HELD_BANANA) || (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flags & 0x8000)) {}
                if (BANANA_ACTOR(actor)->type != 6) {}
                if (BANANA_ACTOR(actor)->state != 0) {}
                if (BANANA_ACTOR(actor)->rot[0] != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
                player->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
            } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                cpuStrategy->branch = CPU_STRATEGY_DROP_BANANA;
            }
            break;

        case CPU_STRATEGY_DROP_BANANA:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(BANANA_ACTOR(actor)->flags & 0x8000)) || (BANANA_ACTOR(actor)->type != ACTOR_BANANA)) ||
                 (BANANA_ACTOR(actor)->state != HELD_BANANA)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flags & 0x8000)) {}
                if (BANANA_ACTOR(actor)->type != 6) {}
                if (BANANA_ACTOR(actor)->state != 0) {}
                if (BANANA_ACTOR(actor)->rot[0] != playerId) {}

            } else {
                BANANA_ACTOR(actor)->state = DROPPED_BANANA;
                BANANA_ACTOR(actor)->velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->velocity[2] = 0.0f;
                if (D_801631E0[playerId] == true) {
                    BANANA_ACTOR(actor)->pos[1] =
                        spawn_actor_on_surface(player->pos[0], player->pos[1] + 30.0, player->pos[2]) +
                        (BANANA_ACTOR(actor)->boundingBoxSize + 1.0f);
                }
            }
            player->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
            cpuStrategy->timer = 0;
            cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            break;

        case CPU_STRATEGY_THROW_BANANA:
            cpuStrategy->actorIndex = use_banana_item(player);
            if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                actor = GET_ACTOR(cpuStrategy->actorIndex);
                BANANA_ACTOR(actor)->state = BANANA_ON_GROUND;
                player->soundEffects |= HOLD_BANANA_SOUND_EFFECT;
                cpuStrategy->branch = CPU_STRATEGY_HOLD_THROW_BANANA;
                cpuStrategy->timer = 0;
                cpuStrategy->numItemUse += 1;
                pathPoint = &gTrackPaths[gPathIndexByPlayerId[0]]
                                        [(gNearestPathPointByPlayerId[gBestRankedHumanPlayer] + 30) %
                                         gPathCountByPathIndex[gPathIndexByPlayerId[gBestRankedHumanPlayer]]];
                BANANA_ACTOR(actor)->velocity[0] = (pathPoint->x - player->pos[0]) / 20.0;
                BANANA_ACTOR(actor)->velocity[1] = ((pathPoint->y - player->pos[1]) / 20.0) + 4.0;
                BANANA_ACTOR(actor)->velocity[2] = (pathPoint->z - player->pos[2]) / 20.0;
                BANANA_ACTOR(actor)->pos[1] = player->pos[1];
                func_800C92CC(playerId, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x09));
                func_800C98B8(player->pos, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x14));
            } else {
                cpuStrategy->timer = 0;
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_THROW_BANANA:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(BANANA_ACTOR(actor)->flags & 0x8000)) || (BANANA_ACTOR(actor)->type != ACTOR_BANANA)) ||
                 (BANANA_ACTOR(actor)->state != BANANA_ON_GROUND)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flags & 0x8000)) {}
                if (BANANA_ACTOR(actor)->type != 6) {}
                if (BANANA_ACTOR(actor)->state != 0) {}
                if (BANANA_ACTOR(actor)->rot[0] != playerId) {}

                cpuStrategy->timer = 0;
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                player->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
            } else {
                BANANA_ACTOR(actor)->velocity[1] -= 0.4;
                BANANA_ACTOR(actor)->pos[0] += BANANA_ACTOR(actor)->velocity[0];
                BANANA_ACTOR(actor)->pos[1] += BANANA_ACTOR(actor)->velocity[1];
                BANANA_ACTOR(actor)->pos[2] += BANANA_ACTOR(actor)->velocity[2];
                if (cpuStrategy->timer > 20) {
                    cpuStrategy->branch = CPU_STRATEGY_END_THROW_BANANA;
                }
            }
            break;

        case CPU_STRATEGY_END_THROW_BANANA:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(BANANA_ACTOR(actor)->flags & 0x8000)) || (BANANA_ACTOR(actor)->type != ACTOR_BANANA)) ||
                 (BANANA_ACTOR(actor)->state != BANANA_ON_GROUND)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flags & 0x8000)) {}
                if (BANANA_ACTOR(actor)->type != 6) {}
                if (BANANA_ACTOR(actor)->state != 0) {}
                if (BANANA_ACTOR(actor)->rot[0] != playerId) {}

            } else {
                BANANA_ACTOR(actor)->state = DROPPED_BANANA;
                BANANA_ACTOR(actor)->velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->velocity[2] = 0.0f;
                BANANA_ACTOR(actor)->pos[1] =
                    spawn_actor_on_surface(BANANA_ACTOR(actor)->pos[0], BANANA_ACTOR(actor)->pos[1] + 30.0,
                                           BANANA_ACTOR(actor)->pos[2]) +
                    (BANANA_ACTOR(actor)->boundingBoxSize + 1.0f);
            }
            player->soundEffects &= ~HOLD_BANANA_SOUND_EFFECT;
            cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->timer = 0;
            break;

        case CPU_STRATEGY_ITEM_GREEN_SHELL:
            if (gNumActors < 80) {
                cpuStrategy->actorIndex = use_green_shell_item(player);
                if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                    cpuStrategy->branch = CPU_STRATEGY_HOLD_GREEN_SHELL;
                    cpuStrategy->timer = 0;
                    cpuStrategy->numItemUse += 1;
                    cpuStrategy->timeBeforeThrow = (random_int(3) * 20) + 10;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_GREEN_SHELL:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (actor->type != ACTOR_GREEN_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(SHELL_ACTOR(actor)->flags & 0x8000)) {}
                if (SHELL_ACTOR(actor)->type != 7) {}
                if (SHELL_ACTOR(actor)->state != 0) {}
                if (SHELL_ACTOR(actor)->rotVelocity != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                cpuStrategy->branch = CPU_STRATEGY_THROW_GREEN_SHELL;
                cpuStrategy->timer = 0;
            }
            break;

        case CPU_STRATEGY_THROW_GREEN_SHELL:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (actor->type != ACTOR_GREEN_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(actor->flags & 0x8000)) {}
                if (actor->type != 7) {}
                if (actor->state != 0) {}
                if (actor->rot[0] != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else {
                SHELL_ACTOR(actor)->state = RELEASED_SHELL;
                cpuStrategy->timer = 0;
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_RED_SHELL:
            if (gNumActors < 80) {
                cpuStrategy->actorIndex = use_red_shell_item(player);
                if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                    cpuStrategy->branch = CPU_STRATEGY_HOLD_RED_SHELL;
                    cpuStrategy->timer = 0;
                    cpuStrategy->numItemUse += 1;
                    cpuStrategy->timeBeforeThrow = (random_int(3) * 20) + 10;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_RED_SHELL:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (SHELL_ACTOR(actor)->type != ACTOR_RED_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(actor->flags & 0x8000)) {}
                if (actor->type != 8) {}
                if (actor->state != 0) {}
                if (actor->rot[0] != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                cpuStrategy->branch = CPU_STRATEGY_THROW_RED_SHELL;
            }
            break;

        case CPU_STRATEGY_THROW_RED_SHELL:
            clear_expired_strategies(cpuStrategy);
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (SHELL_ACTOR(actor)->type != ACTOR_RED_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(actor->flags & 0x8000)) {}
                if (actor->type != 8) {}
                if (actor->state != 0) {}
                if (actor->rot[0] != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else {
                SHELL_ACTOR(actor)->state = RELEASED_SHELL;
                cpuStrategy->timer = 0;
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA_BUNCH:
            if (gNumActors < 80) {
                cpuStrategy->actorIndex = use_banana_bunch_item(player);
                if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH;
                    cpuStrategy->timer = 0;
                    cpuStrategy->numItemUse += 1;
                    cpuStrategy->timeBeforeThrow = (random_int(3) * 20) + 60;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if (BANANA_BUNCH_ACTOR(actor)->state == 6) {

                // FAKE
                if (BANANA_BUNCH_ACTOR(actor)->state != -1) {}
                if (BANANA_BUNCH_ACTOR(actor)->state == 6) {}

                isValidBanana2 = false;

                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[4] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[3] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[2] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[1] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[0] != (-1)) {
                    isValidBanana2 = true;
                }
                if ((BANANA_BUNCH_ACTOR(actor)->type != ACTOR_BANANA_BUNCH) || (isValidBanana2 == false)) {
                    if (BANANA_BUNCH_ACTOR(actor)->type != 14) {}
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->timer = 0;
                } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                    cpuStrategy->branch = CPU_STRATEGY_DROP_BANANA_BUNCH;
                    cpuStrategy->numDroppedBananaBunch = 0;
                    cpuStrategy->timer = 0;
                }
            }
            break;

        case CPU_STRATEGY_DROP_BANANA_BUNCH:
            if (((cpuStrategy->timer) % 10) == 0) {
                if (cpuStrategy->numDroppedBananaBunch < 5) {
                    isValidBanana1 = 0;
                    actor = GET_ACTOR(cpuStrategy->actorIndex);
                    switch (cpuStrategy->numDroppedBananaBunch) {
                        case 0:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[4] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 1:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[3] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 2:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[2] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 3:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[1] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 4:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[0] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;
                    }

                    if (((BANANA_BUNCH_ACTOR(actor)->type == ACTOR_BANANA_BUNCH) &&
                         (BANANA_BUNCH_ACTOR(actor)->state == 6)) &&
                        (isValidBanana1 == true)) {
                        drop_banana_in_banana_bunch((struct BananaBunchParent*) actor);
                    }
                    cpuStrategy->numDroppedBananaBunch += 1;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->timer = 0;
                }
            }
            break;

        case CPU_STRATEGY_ITEM_FAKE_ITEM_BOX:
            cpuStrategy->actorIndex = use_fake_itembox_item(player);
            if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 100)) {
                cpuStrategy->branch = CPU_STRATEGY_HOLD_FAKE_ITEM_BOX;
                cpuStrategy->timer = 0;
                cpuStrategy->numItemUse += 1;
                cpuStrategy->timeBeforeThrow = (random_int(3) * 20) + 10;
            } else {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_FAKE_ITEM_BOX:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->flags & 0x8000)) ||
                  (FAKE_ITEMBOX_ACTOR(actor)->type != ACTOR_FAKE_ITEM_BOX)) ||
                 (FAKE_ITEMBOX_ACTOR(actor)->state != 0)) ||
                (playerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->playerId))) {

                // FAKE
                if (!(actor->flags & 0x8000)) {}
                if (actor->type != 13) {}
                if (actor->state != 0) {}
                if (actor->rot[0] != playerId) {}

                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                cpuStrategy->branch = CPU_STRATEGY_THROW_FAKE_ITEM_BOX;
            }
            break;

        case CPU_STRATEGY_THROW_FAKE_ITEM_BOX:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->flags & 0x8000)) ||
                  (FAKE_ITEMBOX_ACTOR(actor)->type != ACTOR_FAKE_ITEM_BOX)) ||
                 (FAKE_ITEMBOX_ACTOR(actor)->state != 0)) ||
                (playerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->playerId))) {

                // FAKE
                if (!(FAKE_ITEMBOX_ACTOR(actor)->flags & 0x8000)) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->type != 13) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->state != 0) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->rot[0] != playerId) {}

            } else {
                func_802A1064((struct FakeItemBox*) actor);
                if (D_801631E0[playerId] == true) {
                    FAKE_ITEMBOX_ACTOR(actor)->pos[1] = spawn_actor_on_surface(FAKE_ITEMBOX_ACTOR(actor)->pos[0],
                                                                               FAKE_ITEMBOX_ACTOR(actor)->pos[1] + 30.0,
                                                                               FAKE_ITEMBOX_ACTOR(actor)->pos[2]) +
                                                        FAKE_ITEMBOX_ACTOR(actor)->boundingBoxSize;
                }
            }
            cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->timer = 0;
            break;

        case CPU_STRATEGY_ITEM_THUNDERBOLT:
            use_thunder_item(player);
            func_800CAC60(playerId);
            func_8009E5BC();
            cpuStrategy->branch = CPU_STRATEGY_END_THUNDERBOLT;
            cpuStrategy->timer = 0;
            cpuStrategy->numItemUse += 1;
            break;

        case CPU_STRATEGY_END_THUNDERBOLT:
            if (cpuStrategy->timer >= 0xF1) {
                func_800CAD40((s32) ((u8) playerId));
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_STAR:
            player->soundEffects |= STAR_SOUND_EFFECT;
            cpuStrategy->branch = CPU_STRATEGY_END_ITEM_STAR;
            cpuStrategy->timer = 0;
            cpuStrategy->numItemUse += 1;
            break;

        case CPU_STRATEGY_END_ITEM_STAR:
            if (!(player->effects & STAR_EFFECT)) {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->timer = 0;
            break;

        case CPU_STRATEGY_ITEM_BOO:
            player->soundEffects |= BOO_SOUND_EFFECT;
            cpuStrategy->branch = CPU_STRATEGY_WAIT_END_BOO;
            cpuStrategy->timer = 0;
            cpuStrategy->numItemUse += 1;
            break;

        case CPU_STRATEGY_WAIT_END_BOO:
            if (!(player->effects & BOO_EFFECT)) {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->timer = 0;
            break;

        case CPU_STRATEGY_ITEM_MUSHROOM:
            player->soundEffects |= BOOST_SOUND_EFFECT;
            cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->timer = 0;
            cpuStrategy->numItemUse += 1;
            break;

        case CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM:
            if (cpuStrategy->timer >= 0x3D) {
                player->soundEffects |= BOOST_SOUND_EFFECT;
                cpuStrategy->branch = CPU_STRATEGY_ITEM_MUSHROOM;
                cpuStrategy->timer = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM:
            if (cpuStrategy->timer >= 0x3D) {
                player->soundEffects |= BOOST_SOUND_EFFECT;
                cpuStrategy->branch = CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM;
                cpuStrategy->timer = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_SUPER_MUSHROOM:
            cpuStrategy->branch = CPU_STRATEGY_USE_SUPER_MUSHROOM;
            cpuStrategy->timer = 0;
            cpuStrategy->timeBeforeThrow = 0x0258;
            break;

        case CPU_STRATEGY_USE_SUPER_MUSHROOM:
            if ((((s16) cpuStrategy->timer) % 60) == 0) {
                player->soundEffects |= BOOST_SOUND_EFFECT;
                if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                    cpuStrategy->timer = 0;
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            }
            break;
        case CPU_STRATEGY_ITEM_BLUE_SPINY_SHELL:
            if (((s32) gNumActors) < 80) {
                cpuStrategy->actorIndex = use_blue_shell_item(player);
                if ((cpuStrategy->actorIndex >= 0) && (cpuStrategy->actorIndex < 0x64)) {
                    cpuStrategy->branch = CPU_STRATEGY_HOLD_BLUE_SPINY_SHELL;
                    cpuStrategy->timer = 0;
                    cpuStrategy->numItemUse += 1;
                    cpuStrategy->timeBeforeThrow = (random_int(3U) * 0x14) + 0xA;
                } else {
                    cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;
        case CPU_STRATEGY_HOLD_BLUE_SPINY_SHELL:
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (actor->type != ACTOR_BLUE_SPINY_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else if (cpuStrategy->timeBeforeThrow < cpuStrategy->timer) {
                cpuStrategy->branch = CPU_STRATEGY_THROW_BLUE_SPINY_SHELL;
            }
            break;
        case CPU_STRATEGY_THROW_BLUE_SPINY_SHELL:
            clear_expired_strategies(cpuStrategy);
            actor = GET_ACTOR(cpuStrategy->actorIndex);
            if ((((!(SHELL_ACTOR(actor)->flags & 0x8000)) || (SHELL_ACTOR(actor)->type != ACTOR_BLUE_SPINY_SHELL)) ||
                 (SHELL_ACTOR(actor)->state != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->timer = 0;
            } else {
                SHELL_ACTOR(actor)->state = RELEASED_SHELL;
                cpuStrategy->timer = 0;
                cpuStrategy->branch = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        default:
            break;
    }

    if (cpuStrategy->timer < 10000) {
        cpuStrategy->timer += 1;
    }
    if (player->effects & (BOO_EFFECT | BOOST_EFFECT | STAR_EFFECT)) {
        cpuStrategy->timer = 0;
    }
}

#undef BANANA_ACTOR
#undef SHELL_ACTOR
#undef FAKE_ITEMBOX_ACTOR
#undef BANANA_BUNCH

void func_8001BE78(void) {
    Player* temp_s1;
    TrackPathPoint* temp_s0;
    s32 i;

    init_players();
    for (i = 0; i < 4; i++) {
        temp_s1 = &gPlayerOne[i];
        temp_s1->type &= 0xDFFF;
        gPathIndexByPlayerId[i] = i;
        gPlayerTrackPositionFactorInstruction[i].unkC = 0.0f;
        gPlayerTrackPositionFactorInstruction[i].target = 0.0f;
        gPlayerTrackPositionFactorInstruction[i].current = 0.0f;
        switch (i) {
            case 0:
                gNearestPathPointByPlayerId[i] = 6;
                break;
            case 1:
                gNearestPathPointByPlayerId[i] = 1;
                break;
            case 2:
                gNearestPathPointByPlayerId[i] = 6;
                break;
            case 3:
                gNearestPathPointByPlayerId[i] = 1;
                break;
        }
        temp_s0 = &gTrackPaths[i][gNearestPathPointByPlayerId[i]];
        temp_s1->pos[0] = (f32) temp_s0->x;
        temp_s1->pos[1] =
            spawn_actor_on_surface((f32) temp_s0->x, 2000.0f, (f32) temp_s0->z) + temp_s1->boundingBoxSize;
        temp_s1->pos[2] = (f32) temp_s0->z;
        temp_s1->rotation[1] = (s16) *gPathExpectedRotation[i];
        apply_cpu_turn(temp_s1, 0);
        temp_s1++;
        D_80163410[i] = 0;
    }
}

void func_8001C05C(void) {
    init_segment_racing();
    gCurrentCourseId = COURSE_AWARD_CEREMONY;
    SelectPodiumCeremony();
    D_8016347C = 0;
    D_8016347E = 0;
    D_80163480 = 0;
    D_80163484 = 0;
    init_course_path_point();
    func_80014DE4(0);
    func_8001BE78();
    D_80163418[0] = -3202.475097656f;
    D_80163428[0] = 19.166999817f;
    D_80163438[0] = -477.623992920f;
    D_80163418[1] = -3205.080078125f;
    D_80163428[1] = 19.166999817f;
    D_80163438[1] = -462.851989746f;
    D_80163418[2] = -3199.870117188f;
    D_80163428[2] = 19.166999817f;
    D_80163438[2] = -492.395996094f;
    D_80163418[3] = -2409.197021484f;
    D_80163428[3] = 0.0f;
    D_80163438[3] = -355.253997803;
}

void func_8001C14C(void) {
    f32 temp_f0;
    f32 temp_f2;
    s32 var_s1;
    Player* player;

    if (D_8016347C == 1) {
        D_80163480 += 1;
    }
    if ((D_8016347E == 1) && (gBombKarts[0].state == 0) && (D_802874D8.unk1D >= 3)) {
        D_80163484++;
        if (D_80163484 >= 0xF) {
            D_80163484 = 0;
            D_8016347E = 2;
            func_8009265C();
        }
    }
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        if ((var_s1 == 3) && (D_8016347C == 0)) {
            break;
        }

        player = &gPlayerOne[var_s1];
        update_player(var_s1);
        if (!(player->type & 0x2000)) {
            temp_f0 = D_80163418[var_s1] - player->pos[0];
            temp_f2 = D_80163438[var_s1] - player->pos[2];
            if ((f64) ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 1.0) {
                if (var_s1 != 3) {
                    if (1) {}
                    // Why oh why is a ternary required here? Who does that?
                    (D_8016347C == 0) ? (player->type |= 0x2000) : (player->type &= ~0x2000);
                    if ((gPlayerOne->type & 0x2000) && (gPlayerTwo->type & 0x2000) && (gPlayerThree->type & 0x2000)) {
                        D_8016347C = 1;
                        D_80163480 = 0;
                    }
                } else if (D_8016347E == 0) {
                    if (!(player->effects & 0x01000000)) {
                        player->type |= 0x2000;
                    }
                    D_8016347E = 1;
                    D_80163484 = 0;
                } else if (!(player->effects & 0x01000000)) {
                    player->type |= 0x2000;
                }
            }
        }
    }
}

void render_bomb_karts_wrap(s32 cameraId) {
    if (IsPodiumCeremony()) {
        if (gBombKarts[0].waypointIndex >= 16) {
            render_bomb_karts(PLAYER_FOUR);
        }
    } else {
        if (gModeSelection == VERSUS) {
            render_bomb_karts(cameraId);
        }
    }
}

UNUSED void func_8001C42C(void) {
    if (D_800DDB20 == 0) {
        if ((gControllerThree->buttonPressed & 0x20) != 0) {
            D_800DDB20 = 1;
        }
    } else {
        if ((gControllerThree->buttonPressed & 0x20) != 0) {
            D_800DDB20 = 0;
        }
        func_80057C60();
        gSPDisplayList(gDisplayListHead++, D_0D0076F8);
        func_80057CE4();
    }
}
