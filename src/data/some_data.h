#ifndef SOME_DATA_HH
#define SOME_DATA_HH
// SOME_DATA_HH named this way prevent conflict with <assets/some_data.h>

#include <libultra/gbi.h>
#include "spline.h"
#include <assets/textures/other_textures.h>
#include "objects.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Vtx gPlayerOneVtx[];
extern Vtx gPlayerTwoVtx[];
extern Vtx gPlayerThreeVtx[];
extern Vtx gPlayerFourVtx[];
extern Vtx gPlayerFiveVtx[];
extern Vtx gPlayerSixVtx[];
extern Vtx gPlayerSevenVtx[];
extern Vtx gPlayerEightVtx[];
extern Vtx D_800E51D0[];
extern Vtx D_800E5210[];
extern Vtx gBalloonVertexPlane1[];
extern Vtx gBalloonVertexPlane2[];
extern u8* gCourseOutlineTextures[];
extern s16 D_800E5520[];
extern s16 minimapDimensions[];
extern u16 D_800E55A0[];
extern u16 D_800E55B0[];
extern u8 D_800E55D0[][3];
extern s32 D_800E55F8[];
extern s32 D_800E5618[];
extern s8 D_800E5628[];
extern Vec3s gFireBreathsSpawns[];
extern s16 D_800E5740[];
extern s16 D_800E579C[];
extern s16 D_800E57F8[];
extern ThwompSpawn gThomwpSpawns50CC[];
extern ThwompSpawn gThwompSpawns100CCExtra[];
extern ThwompSpawn gThomwpSpawns150CC[];
extern u64 D_800E5974;
extern SplineData D_800E5988;
extern SplineData D_800E5A44;
extern SplineData D_800E5B08;
extern SplineData D_800E5BD4;
extern SplineData D_800E5C90;
extern SplineData D_800E5D54;
extern SplineData D_800E5D78;
extern SplineData* D_800E5D9C[];
extern SplineData* D_800E5DB0;
extern s8 D_800E5DB4[];
extern YVFlagPoleSpawn gFlagpoleSpawns[];
extern HegdehogSpawn gHedgehogSpawns[];
extern Vec3s gHedgehogPatrolPoints[];
extern SnowmanSpawn gSnowmanSpawns[];
extern CrabSpawn gCrabSpawns[];
extern s16 D_800E5FD0[];
extern SplineData D_800E6034;
extern SplineData D_800E60F0;
extern SplineData D_800E61B4;
extern SplineData D_800E6280;
extern Vec3s D_800E634C[];
extern MoleSpawnUnion gMoleSpawns;
extern SplineData D_800E641C;
extern SplineData D_800E64D8;
extern SplineData D_800E659C;
extern SplineData D_800E6668;
extern SplineData* D_800E6724[];
extern SplineData* D_800E672C[];
extern Vec3f D_800E6734[];
// This should really be `extern Vec3s gTorchSpawns[];`
extern s16 gTorchSpawns[];
extern SplineData D_800E67B8;
extern SplineData D_800E6834;
extern SplineData D_800E68E0;
extern SplineData D_800E694C;
extern SplineData D_800E69B0;
extern SplineData D_800E69F4;
extern CloudData gLuigiRacewayClouds[];
extern CloudData gYoshiValleyMooMooFarmClouds[];
extern CloudData gKoopaTroopaBeachClouds[];
extern CloudData gRoyalRacewayClouds[];
extern CloudData gSherbetLandClouds[];
extern CloudData gKalimariDesertClouds[];
extern StarData gToadsTurnpikeRainbowRoadStars[];
extern StarData gWarioStadiumStars[];
extern u8 D_800E6F30[][3];
extern u8 D_800E6F48[][3];

#ifdef __cplusplus
}
#endif

#endif
