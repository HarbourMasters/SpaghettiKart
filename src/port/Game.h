#ifndef _GAME_H
#define _GAME_H

#include <libultraship.h>
#include "engine/courses/Course.h"
#include "engine/HM_Intro.h"

#ifdef __cplusplus
#include "engine/editor/Editor.h"
class Course;
extern "C" {
#endif
#include "camera.h"
#include "actor_types.h"

extern s32 gTrophyIndex;

#ifdef __cplusplus
extern Editor::Editor gEditor;
extern HarbourMastersIntro gMenuIntro;
#endif

Properties* CM_GetProps();
Properties* CM_GetPropsCourseId(s32 courseId);

void HM_InitIntro(void);
void HM_TickIntro(void);
void HM_DrawIntro(void);

void CM_SpawnFromLevelProps();

void CM_DisplayBattleBombKart(s32 playerId, s32 primAlpha);
void CM_DrawBattleBombKarts(s32 cameraId);

u32 WorldNextCup(void);

u32 WorldPreviousCup(void);

u32 GetCupIndex(void);

const char* GetCupName(void);

void LoadCourse();

size_t GetCourseIndex();

void SetCourse(const char* name);

void NextCourse();
void PreviousCourse();

void CM_SetCup(void*);

void CM_LoadTextures();

void CM_RenderCourse(struct UnkStruct_800DC5EC* arg0);

void CM_RenderCredits();

void CM_SpawnStarterLakitu();
void CM_ActivateFinishLakitu(s32 playerId);
void CM_ActivateSecondLapLakitu(s32 playerId);
void CM_ActivateFinalLapLakitu(s32 playerId);
void CM_ActivateReverseLakitu(s32 playerId);

bool CM_DoesFinishlineExist();

void CM_InitClouds();

void CM_DrawActors(Camera* camera, struct Actor* actor);
void CM_DrawStaticMeshActors();

void CM_TickObjects();
void CM_TickObjects60fps();
void CM_DrawObjects(s32 cameraId);

void CM_TickEditor();
void CM_DrawEditor();
void CM_Editor_SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY);
void CM_TickDraw();
void Editor_ClearMatrix();

void CM_TickParticles(void);
void CM_DrawParticles(s32 cameraId);

void CM_UpdateClouds(s32 arg0, Camera* camera);

void CM_Waypoints(Player* player, int8_t playerId);

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7);

void CM_InitCourseObjects();

void CM_UpdateCourseObjects();

void CM_RenderCourseObjects(s32 cameraId);

void CM_SomeSounds();

void CM_CreditsSpawnActors();

void CM_WhatDoesThisDo(Player* player, int8_t playerId);

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId);

void CM_SetStaffGhost();

void CM_BombKartsWaypoint(s32 cameraId);

void CM_ScrollingTextures();

s32 CM_GetCrossingOnTriggered(uintptr_t* crossing);

void CM_BeginPlay();

void CM_DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection);

void CM_AICrossingBehaviour(s32 playerId);

void CM_ClearVehicles(void);

void CM_CrossingTrigger();

void CM_VehicleCollision(s32 playerId, Player* player);

void CM_TickActors();

void CM_DrawBombKarts(s32 cameraId);

size_t GetCupCursorPosition();

void SetCupCursorPosition(size_t position);

size_t GetCupSize();

void SetCourseFromCup();

void* GetCourse(void);

void SetCourseById(s32 course);

void SetCourseByClass(void* course);

struct Actor* CM_GetActor(size_t index);
void CM_DeleteActor(size_t index);
struct Actor* CM_AddBaseActor();
void CM_AddEditorObject(struct Actor* actor, const char* name);
void Editor_AddLight(s8* direction);
size_t CM_GetActorSize();
size_t CM_FindActorIndex(struct Actor* actor);
void CM_ActorCollision(Player* player, struct Actor* actor);
void CM_CleanWorld(void);

f32 CM_GetWaterLevel(Vec3f pos, Collision* collision);

void* GetMarioRaceway(void);

void* GetLuigiRaceway(void);

void* GetChocoMountain(void);

void* GetBowsersCastle(void);

void* GetBansheeBoardwalk(void);

void* GetYoshiValley(void);

void* GetFrappeSnowland(void);

void* GetKoopaTroopaBeach(void);

void* GetRoyalRaceway(void);

void* GetMooMooFarm(void);

void* GetToadsTurnpike(void);

void* GetKalimariDesert(void);

void* GetSherbetLand(void);

void* GetRainbowRoad(void);

void* GetWarioStadium(void);

void* GetBlockFort(void);

void* GetSkyscraper(void);

void* GetDoubleDeck(void);

void* GetDkJungle(void);

void* GetBigDonut(void);

void* GetPodiumCeremony(void);

void* GetMushroomCup(void);

void* GetFlowerCup(void);

void* GetStarCup(void);

void* GetSpecialCup(void);

void* GetBattleCup(void);

void* GetCup();

void CM_RunGarbageCollector(void);

#ifdef __cplusplus
}
#endif

#endif // _GAME_H