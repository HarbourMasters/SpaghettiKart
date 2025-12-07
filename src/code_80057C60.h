#ifndef CODE_80057C60_H
#define CODE_80057C60_H

#ifdef __cplusplus
extern "C" {
#endif

#include <common_structs.h>
#include "objects.h"
#include "camera.h"
#include "code_800029B0.h"

// code_80057C60

#define RENDER_SCREEN_MODE_1P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_1P
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE PLAYER_THREE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR PLAYER_FOUR + SCREEN_MODE_3P_4P_SPLITSCREEN + 5

typedef struct {
    char unk_00[0x4];
    Vec3f unk_04;
    char unk_10[0x4];
    s32 objectIndex;
    char unk_18[0x8];
} struct_D_8018CE10; // size = 0x20

/** @cond */

void func_8005C674(s8, s16*, s16*, s16*);
void func_80057C60(void);
void func_80057CE4(void);
void func_80057DD0(void);
void func_80057FC4(u32);

void render_object(struct UnkStruct_800DC5EC* screen);
void render_player_snow_effect(Camera* camera);
void render_object_for_player(s32);
void render_snowing_effect(s32);
void func_80058BF4(void);
void func_80058C20(u32);
void render_hud(u32);
void func_80058F48(void);
void func_80058F78(void);
void func_80059AC8(void);

void func_80059024(void);
void func_8005902C(void);
void func_800590D4(void);
void func_800591B4(void);
void func_80059358(void);
void render_hud_2p_horizontal_player_two_horizontal_player_one(void);
void func_800593F0(void);
void render_hud_2p_horizontal_player_two(void);
void draw_simplified_hud(s32);
void func_800594F0(void);
void render_hud_2p_vertical_player_one(void);
void func_80059528(void);
void render_hud_2p_vertical_player_two(void);
void render_hud_lap_3p_4p(s32);
void func_800596A8(void);
void render_hud_1p_multi(void);
void func_80059710(void);
void render_hud_2p_multi(void);
void func_80059750(void);
void render_hud_3p_multi(void);
void func_800597B8(void);
void render_hud_4p_multi(void);
void func_80059820(s32);
void randomize_seed_from_controller(s32);
void func_8005994C(void);
void func_8005995C(void);
void func_80059A88(s32);
void func_80059C50(void);
void func_80059D00(void);

void func_8005A070(void);
void func_8005A14C(s32);
void func_8005A380(void);
void func_8005A3C0(void);
void func_8005A71C(void);
void update_object(void);
void func_8005A99C(void);
void func_8005AA34(void);
void func_8005AA4C(void);
void func_8005AA6C(s32);
void func_8005AA80(void);
void func_8005AA94(s32);
void func_8005AAF0(void);
void func_8005AB20(void);
void func_8005AB60(void);

void func_8005B7A0(void);

void func_8005B914(void);

void func_8005C360(f32);
void func_8005C64C(s32*);
void func_8005C654(s32*);
void func_8005C65C(s32);
void func_8005C6B4(s8, s16*, s16*, s16*);
void func_8005C728(void);
void func_8005C980(void);
void func_8005CB60(s32, s32);

void func_8005D0FC(s32);
void func_8005D18C(void);
void func_8005D1F4(s32);

void func_8005D290(void);
void reset_player_particle_pool(Player*);
void set_particle_position_and_rotation(Player*, UnkPlayerStruct258*, f32, f32, f32, s8, s8);
s32 init_particle_player(UnkPlayerStruct258*, s8, f32);
s32 set_particle_colour(UnkPlayerStruct258*, s32, s16);
s32 set_particle_colour_randomly_varried(UnkPlayerStruct258*, s32, s16);
void set_drift_particles(Player*, s16, s32, s8, s8);
void check_drift_particles_setup_valid(Player*, s16, s32, s8, s8);
void func_8005DAD0(void);
void func_8005DAD8(UnkPlayerStruct258*, s16, s16, s16);
void setup_tyre_particles(Player*, s16, s32, s8, s8);
void func_8005EA94(Player*, s16, s32, s8, s8);
void func_8005ED48(Player*, s16, s32, s8, s8);

void func_8005F90C(Player*, s16, s32, s8, s8);

void func_80060504(Player*, s16, s32, s8, s8);
void func_800608E0(Player*, s16, s32, s8, s8);
void func_80060B14(Player*, s16, s32, s8, s8);
void func_80060BCC(Player*, s16, s32, s8, s8);
void func_80060F50(Player*, s16, s32, s8, s8);

void func_80061094(Player*, s16, s32, s8, s8);
void func_80061130(Player*, s16, s32, s8, s8);
void func_80061224(Player*, s16, s32, s8, s8);
void func_800612F8(Player*, s32, s32, s8, s8);
void func_80061430(Player*, s32, s32, s8, s8);
void func_800615AC(Player*, s16, s32, s8, s8);
void func_80061754(Player*, s16, s32, s32, s32);
void func_8006199C(Player*, s16, s32, s8, s8);
void func_80061A34(Player*, s16, s32, s8, s8);
void func_80061D4C(Player*, s16, s32, s8, s8);
void func_80061EF4(Player*, s16, s32, s8, s8);

void func_800621BC(Player*, s16, s32, s8, s8);
void func_80062484(Player*, UnkPlayerStruct258*, s32);
void func_800624D8(Player*, s32, s32, s8, s8);
void func_800628C0(Player*, s8, s8, s8);
void func_80062914(Player*, s8, s8, s8);
void func_80062968(Player*, s8, s8, s8);
void func_800629BC(Player*, s8, s8, s8);
void func_80062A18(Player*, s8, s8, s8);
void func_80062AA8(Player*, s8, s8, s8);
void func_80062B18(f32*, f32*, f32*, f32, f32, f32, u16, u16);
void func_80062C74(Player*, s16, s32, s32);
void func_80062F98(Player*, s16, s8, s8);

void set_oob_splash_particle_position(Player*, s16, s8, s8);
void func_800631A8(Player*, s16, s8, s8);
void func_80063268(Player*, s16, s8, s8);
void func_80063408(Player*, s16, s8, s8);
void func_800635D4(Player*, s16, s8, s8);
void func_800639DC(Player*, s16, s8, s8);
void func_80063BD4(Player*, s16, s8, s8);
void func_80063D58(Player*, s16, s8, s8);
void func_80063FBC(Player*, s16, s32, s32);

void func_80064184(Player*, s16, s8, s8);
void func_800643A8(Player*, s16, s8, s8);
void func_800644E8(Player*, s16, s8, s8);
void func_80064664(Player*, s16, s8, s8);
void func_800647C8(Player*, s16, s8, s8);
void func_800648E4(Player*, s16, s8, s8);
void func_80064988(Player*, s16, s8, s8);
void func_800649F4(Player*, s16, s8, s8);
void func_80064B30(Player*, s16, s8, s8);
void func_80064C74(Player*, s16, s8, s8);
void func_80064DEC(Player*, s8, s8, s8);
void func_80064EA4(Player*, s8, s8, s8);
void func_80064F88(Player*, s8, s8, s8);

void func_80065030(Player*, s8, s8, s8);
void func_800650FC(Player*, s8, s8, s8);
void func_800651F4(Player*, s8, s8, s8);
void func_800652D4(Vec3f, Vec3s, f32);
void func_8006538C(Player*, s8, s16, s8);
void func_800658A0(Player*, s8, s16, s8);
void render_player_drift_particles(Player*, s8, s16, s8);
void render_player_ground_particles(Player*, s8, s16, s8);

void func_800664E0(Player*, s8, s16, s8);
void func_80066998(Player*, s8, s16, s8);
void func_80066BAC(Player*, s8, s16, s8);

void func_80067280(Player*, s8, s16, s8);
void render_player_boost_spark_particles(Player*, s8, s16, s8);
void render_player_onomatopoeia_whrrrr(Player*, s8, f32, s8, s8);
void render_player_speech_bubble(Player*, s8, u8*, s8, f32, s32);

void render_music_note(Player*, s8, u8*, s8, f32, s32);
void render_player_onomatopoeia_crash(Player*, s8, f32, s8, s8);
void func_80068724(Player*, s8, f32, s8, s8);
void render_player_onomatopoeia_boing(Player*, s8, f32, s8, s8);
void render_player_onomatopoeia_pomp(Player*, s8, f32, s8, s8);

void func_800691B8(Player*, s8, s16, s8);
void func_80069444(Player*, s8, s16, s8);
void render_wall_bonk_star_particles(Player*, s8, s16, s8, f32);
void func_80069938(Player*, s8, s16, s8);
void func_80069BA8(Player*, s8, s16, s8);
void func_80069DB8(Player*, s8, s16, s8);

void func_8006A01C(Player*, s8, s16, s8);
void func_8006A280(Player*, s8, s16, s8);
void init_balloon(Player*, f32, f32, s8, s8, s16);
void func_8006A7C0(Player*, f32, f32, s8, s8);
void render_battle_balloon(Player*, s8, s16, s8);

void init_all_player_balloons(Player*, s8);
void clear_all_player_balloons(Player*, s8);
void pop_player_balloon(Player*, s8);
void set_player_balloon_to_gone(s32, s8, s8);
void func_8006B9CC(Player*, s8);
void func_8006BA94(Player*, s8, s8);
void render_balloon(Vec3f, f32, s16, s16);

void func_8006C0C8(Vec3f, f32, s32, s16);
void func_8006C294(Vec3f, f32, s32, s16);
void func_8006C4D4(Vec3f, f32, s32, s16, s16);
void func_8006C6AC(Player*, s16, s8, s8);
void func_8006C9B8(Player*, s16, s8, s8);
void func_8006CEC0(Player*, s16, s8, s8);

void func_8006D194(Player*, s8, s8);
void func_8006D474(Player*, s8, s8);
void func_8006DC54(Player*, s8, s8);
void func_8006DD3C(Player*, s8, s8);

void func_8006E058(void);
void func_8006E420(Player*, s8, s8);
void render_kart_particles(Player* player, s8 playerId, s8 screenId);
void func_8006E7CC(Player*, s8, s8);
void func_8006E848(Player*, s8, s8);
void func_8006E8C4(Player*, s8, s8);
void func_8006E940(Player*, s8, s8);
void func_80075CA8(void);
void func_80085214();

extern s16 D_800E4730[];
extern u8** D_800E4770[];
extern u8** D_800E47A0[];
extern s32 D_800E47DC[];
extern s32 D_800E480C[];

extern f32 gPlayerWaterLevel[];

extern s32 D_80165590;
extern s32 D_80165594;
extern s32 D_80165598;
extern s32 D_8016559C;
extern s32 D_801655A4;
extern s32 D_801655AC;
extern s32 D_801655B4;
extern s32 D_801655BC;
extern s32 D_801655C0;
extern s32 D_801655C4;
extern s32 D_801655CC;
extern s32 D_801655D8;
extern s32 D_801655E8;
extern s32 D_801655F0;
extern s32 D_801655F8;

extern s32 D_80165608;
extern s32 D_80165618;
extern s32 D_80165628;
extern u32 D_80165638;
extern u32 D_80165648;
extern u32 D_80165658[];
extern s32 D_80165678;
extern u16 D_801656B0;
extern u16 D_801656C0;
extern u16 D_801656D0;
extern u16 D_801656E0;
extern s16 D_801656F0;
extern s16 D_80165708;
extern s16 D_80165710;
// extern s16 D_80165730; -> objects.h
// extern s16 D_80165738; -> objects.h

extern s16 D_80165740;
extern s16 D_80165748;

// extern s16 gNumActiveThwomps; -> objects.h

extern s32 D_80165754;
// extern ThwompSpawn *gThowmpSpawnList; -> objects.h

extern Vec4s D_80165760;
extern s8 D_8016576A;
extern Vec4s D_80165770;
extern s16 D_8016578C;
extern Vec4s D_80165780;

extern s16 D_80165790;
extern s16 D_80165794;
extern s8 D_8016579C;
extern u16 D_8016579E;
extern u16 D_801657A2;
extern s8 D_801657AE;
extern s8 gHUDDisable;
extern s8 D_801657B2;
extern s8 D_801657B4;
extern s8 D_801657B8[];
extern s8 D_801657C8;
extern s8 D_801657D0[];
extern s8 D_801657D8;
extern s8 D_801657E1;
extern s8 D_801657E2;
extern s8 D_801657E3;
extern s8 gHUDModes;
extern s8 D_801657E5;
extern bool D_801657E6;
extern u8 D_801657E7;
extern bool D_801657E8;
extern bool D_801657F0;
extern bool D_801657F8;
extern s32 D_801657FC;

extern s8 D_80165800[2];
extern s32 D_80165804;
extern s8 D_80165808;
extern s32 D_8016580C;
extern bool D_80165810;
extern s32 D_80165814;
extern bool D_80165818;
extern s32 D_8016581C;
extern s8 D_80165820;
extern s8 D_80165828;
extern Vec3su D_8016582C;
extern s8 D_80165832[2];
extern Vec3su D_80165834;
extern s8 D_80165840[];
extern s32 D_80165860;
extern s32 D_8016586C;
extern s32 D_80165878;
extern s32 D_8016587C;
extern s8 D_80165888;
extern s8 D_80165890;
extern s8 D_80165898;
extern s32 D_8016589C;
extern s8 D_801658A8;
extern s8 D_801658BC;
extern s8 D_801658C6;
extern s8 D_801658CE;
extern s8 D_801658D6;
extern s8 D_801658DC;
extern s8 D_801658E4;
extern s8 D_801658EC;
extern s8 D_801658F4;
extern u8 sRandomItemIndex;
extern s8 D_801658FE;
extern u8 gControllerRandom;

extern s16 D_80165900;
extern s8 D_80165908;

extern s8 D_80165A90;
// extern Object gObjectList[]; -> objects.h

extern s32 objectListSize;
extern Mtx D_80183D60;
// extern s32 D_80183DA0; -> objects.h

extern f32 D_80183DA8[];
// extern s32 gIndexLakituList[]; -> objects.h

extern f32 D_80183DC8[];
// extern s32 gIndexObjectBombKart[]; -> bomb_kart.h
// extern s32 gNextFreeObjectParticle1; -> objects.h

extern Vec3f D_80183E40;
// extern s32 gNextFreeObjectParticle2; -> objects.h

extern Vec3f D_80183E50;
// extern s32 gNextFreeObjectParticle3; -> objects.h
// extern s32 gNextFreeObjectParticle4; -> objects.h

extern Vec3f D_80183E70;
// extern s32 gNextFreeLeafParticle; -> objects.h

extern Vec3su D_80183E80;
// extern s32 gItemWindowObjectByPlayerId[]; -> objects.h

extern Vec3su D_80183E98;
// extern s32 indexObjectList1[]; -> objects.h
// extern s32 indexObjectList2[]; -> objects.h

extern const char* gLakituTextureBuffer[4][2];
// extern s32 indexObjectList3[]; -> objects.h

extern const char** gLakituTexturePtr;
// extern s32 indexObjectList4[]; -> objects.h
// extern Collision D_8018C0B0[]; -> objects.h
// extern s32 gObjectParticle1[]; -> objects.h

extern Collision D_8018C3B0;
// extern s32 gObjectParticle2[]; -> objects.h
// extern s32 gObjectParticle3[]; -> objects.h

extern Collision D_8018C830;
// extern s32 gObjectParticle4[]; -> objects.h
// extern s32 gLeafParticle[]; -> objects.h

extern hud_player playerHUD[];
// extern s32 D_8018CC80[]; -> objects.h

extern struct_D_8018CE10 D_8018CE10[];
// extern s32 D_8018CF10; -> objects.h

extern Camera* D_8018CF14;
extern s16 D_8018CF18;
extern Player* D_8018CF1C;
extern s16 D_8018CF20;
extern Player* D_8018CF28[];
extern s16 D_8018CF48;
extern s16 D_8018CF50[];
extern s16 D_8018CF60;
extern s16 D_8018CF68[];
extern s16 D_8018CF78;
extern s16 gGPCurrentRaceCharacterIdByRank[];
extern f32 D_8018CFCC;
extern f32 D_8018CFD4;
extern s16 D_8018CF90;
extern s16 D_8018CF98[];
extern s16 D_8018CFA8;
extern s16 D_8018CFB0;
extern s16 D_8018CFB8;
extern s16 D_8018CFC0;
extern s16 D_8018CFC8;
extern s16 D_8018CFD0;
extern s16 D_8018CFD8;
extern s8 D_8018CFAC[];
extern s8 D_8018CFB4[];
extern s8 D_8018CFBC[];
extern s8 D_8018CFC4[];
extern s16 D_8018CFE0;
extern f32 D_8018CFE4;
extern s16 D_8018CFE8;
extern f32 D_8018CFEC;
extern s16 D_8018CFF0;
extern f32 D_8018CFF4;
extern s16 D_8018CFF8;
extern s16 D_8018D000;
extern s16 D_8018D008;
extern f32 D_8018D00C;
extern s16 D_8018D010;
extern s16 D_8018D018;
extern f32 xOrientation;
extern s16 D_8018D020;
extern f32 D_8018D028[];
extern s16 D_8018D048;
extern f32 D_8018D050[];
extern s16 D_8018D070;
extern f32 D_8018D078[];
extern s16 D_8018D098;
extern f32 D_8018D0A0[];
extern s16 D_8018D0C0;
extern f32 D_8018D0C8[];
extern s16 D_8018D0E8;
extern f32 D_8018D0F0[];
extern s16 D_8018D110;
extern s32 D_8018D114;
extern s32 gMatrixHudCount;
extern s32 D_8018D140;
extern s32 D_8018D150;
extern s32 D_8018D158;
extern s32 D_8018D160;
extern s32 D_8018D168;
extern s16 D_8018D16C;
extern s32 D_8018D170;
// extern s16 D_8018D174; -> objects.h

extern s32 D_8018D178;
// extern s16 D_8018D17C; -> objects.h

extern s32 D_8018D180;
extern s16 D_8018D184;
extern s32 gIsHUDVisible;
extern s16 D_8018D18C;
extern s32 D_8018D190;
// extern s8 D_8018D198[]; -> objects.h

extern s32 D_8018D1A0;
// extern s8 D_8018D1A8[]; -> objects.h

extern s32 D_8018D1B4;
// extern s8 D_8018D1B8[]; -> objects.h

extern s32 D_8018D1C4;
// extern s32 D_8018D1C8; -> objects.h

extern s32 D_8018D1CC;
// extern s32 D_8018D1D0; -> objects.h

extern s32 D_8018D1D4;
// extern s32 D_8018D1D8; -> objects.h

extern s32 D_8018D1DC;
extern f32 D_8018D1E8;
extern s32 D_8018D1EC;
// extern s32 D_8018D1F0; -> objects.h
// extern s32 D_8018D1F8; -> objects.h

extern s32 D_8018D1FC;
extern s16 D_8018D200;
extern s32 D_8018D204;
extern s16 D_8018D208;
extern s32 D_8018D20C;
extern s16 D_8018D210;
extern bool D_8018D214;
extern s16 D_8018D218;
extern s32 D_8018D21C;
extern u8 (*gCloudTexture)[1024];
extern s32 D_8018D224;
extern u8 D_8018D228;
extern s32 D_8018D22C;
// extern s8 D_8018D230; -> objects.h

extern uintptr_t D_8018D240;
extern u8* D_8018D248[];
extern f32 gMinimapPlayerScale;
extern s32 D_8018D2A4;
extern s32 D_8018D2AC;
extern s16 gMinimapWidth;
extern s32 D_8018D2B4;
extern s16 gMinimapHeight;
extern s32 D_8018D2BC;
extern s16 gMinimapX[];
extern s32 D_8018D2C8[];
extern s16 gMinimapY[];
extern s16 gMinimapPlayerX;
extern s16 gMinimapPlayerY;
extern s32 D_8018D314;
extern s32 D_8018D320;
extern s32 D_8018D380;
extern s32 D_8018D384;
extern s32 D_8018D388;
// extern s32 D_8018D3BC; -> objects.h
// extern s32 D_8018D3C0; -> objects.h
// extern s32 D_8018D3C4; -> objects.h

extern s32 D_8018D3D4;
extern s32 D_8018D3D8;
extern s32 D_8018D3DC;
extern s32 D_8018D3E0;
extern s32 D_8018D3E4;
extern s32 D_8018D3E8;
extern s32 D_8018D3EC;
extern s32 D_8018D3F0;
extern s32 D_8018D3F4;
extern s32 D_8018D3F8;
extern s32 gRaceFrameCounter;
extern s32 D_8018D400;
extern s32 D_8018D40C;
extern s32 D_8018D410;
extern u8* D_8018D420;
extern u8* D_8018D424;
extern u8* D_8018D428;
extern u8* D_8018D42C;
extern u8* D_8018D430;
extern u8* D_8018D434;
extern u8* D_8018D438;
extern u8* D_8018D43C;
extern u8* D_8018D440;
extern u8* D_8018D444;
extern u8* D_8018D448;
extern u8* D_8018D44C;
extern u8* D_8018D450;
extern u8* D_8018D454;
extern u8* D_8018D458;
extern u8* D_8018D45C;
extern u8* D_8018D460;
extern u8* D_8018D464;
extern u8* D_8018D468;
extern u8* D_8018D46C;
extern u8* D_8018D470;
extern u8* D_8018D478;
extern u8* D_8018D480;
extern u8* D_8018D484;
extern u8* D_8018D48C;
extern u8* D_8018D490;
extern u8* D_8018D49C;
extern u8* D_8018D4A0;
extern u8* D_8018D4BC;
extern u8* D_8018D4C0;
extern u8* gTextureLoadedLightningBolt0;
extern u8* gTextureLoadedLightningBolt1;
extern Vec3f gPlayerBalloonPosX[];
extern Vec3f gPlayerBalloonPosY[];
extern Vec3f gPlayerBalloonPosZ[];
extern u16 gPlayerBalloonStatus[8][3];
extern Vec3s D_8018D620[];
extern Vec3f D_8018D650[];
extern Vec3f D_8018D6B0[];

extern Vec3f D_8018D710[];
extern Vec3s D_8018D770[];
extern Vec3s D_8018D7A0[];
extern Vec3s D_8018D7D0[];

extern Vec3s D_8018D800[];
extern Vec3s D_8018D830[];
extern Vec3s D_8018D860[];
extern Vec3s D_8018D890[];
extern s16 gPlayerBalloonCount[]; // D_8018D8C0
extern Vec3s gPlayerBalloonDepartingTimer[];

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif
