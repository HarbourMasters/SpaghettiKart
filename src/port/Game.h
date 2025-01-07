#ifndef _GAME_H
#define _GAME_H

#include <libultraship.h>
#include "engine/Engine.h"

#ifdef __cplusplus
extern "C" {
#include "camera.h"
#endif
#include "actor_types.h"

u32 WorldNextCup(void);

u32 WorldPreviousCup(void);

u32 GetCupIndex(void);

const char* GetCupName(void);

void LoadCourse();

CProperties* GetCoursePropsA();

size_t GetCourseIndex();

void SetCourse(const char* name);

void NextCourse();
void PreviousCourse();

void CourseManager_SetCup(void*);

void CourseManager_SpawnVehicles();

void CourseManager_UpdateVehicles();

void CourseManager_LoadTextures();

void CourseManager_RenderCourse(struct UnkStruct_800DC5EC* arg0);

void CourseManager_RenderCredits();

void CourseManager_SpawnActors();

void CM_SpawnStarterLakitu();
void CM_ActivateFinishLakitu(s32 playerId);
void CM_ActivateSecondLapLakitu(s32 playerId);
void CM_ActivateFinalLapLakitu(s32 playerId);
void CM_ActivateReverseLakitu(s32 playerId);

bool cm_DoesFinishlineExist();

void CourseManager_InitClouds();

void CourseManager_DrawActors(Camera* camera, struct Actor* actor);

void CourseManager_TickObjects();
void CourseManager_TickObjects60fps();
void CourseManager_DrawObjects(s32 cameraId);

void CM_TickParticles(void);
void CM_DrawParticles(s32 cameraId);

void CourseManager_UpdateClouds(s32 arg0, Camera* camera);

void CourseManager_Waypoints(Player* player, int8_t playerId);

void CourseManager_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                      f32* arg6, f32* arg7);

void CourseManager_MinimapSettings();

void CourseManager_InitCourseObjects();

void CourseManager_UpdateCourseObjects();

void CourseManager_RenderCourseObjects(s32 cameraId);

void CourseManager_SomeSounds();

void CourseManager_CreditsSpawnActors();

void CourseManager_WhatDoesThisDo(Player* player, int8_t playerId);

void CourseManager_WhatDoesThisDoAI(Player* player, int8_t playerId);

void CourseManager_MinimapFinishlinePosition();

void CourseManager_SetStaffGhost();

CProperties* CourseManager_GetProps();

void CourseManager_SpawnBombKarts();

void CourseManager_BombKartsWaypoint(s32 cameraId);

void CourseManager_ScrollingTextures();

s32 CourseManager_GetCrossingOnTriggered(uintptr_t* crossing);

void CourseManager_VehiclesSpawn();

void CourseManager_DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                             uint16_t playerDirection);

void CourseManager_AICrossingBehaviour(s32 playerId);

void CourseManager_ClearVehicles(void);

void CourseManager_DrawVehicles(s32 playerId);

void CourseManager_CrossingTrigger();

void CourseManager_VehiclesCollision(s32 playerId, Player* player);

void CourseManager_VehiclesTick();

void CourseManager_TickBombKarts();

void CourseManager_TickActors();

void CourseManager_TrainSmokeTick(void);

void CourseManager_DrawBattleBombKarts(s32 cameraId);

void CourseManager_DrawBombKarts(s32 cameraId);

void CourseManager_TrainSmokeDraw(s32 cameraId);

size_t GetCupCursorPosition();

void SetCupCursorPosition(size_t position);

size_t GetCupSize();

void SetCourseFromCup();

void* GetCourse(void);

void SetCourseById(s32 course);

void SetCourseByClass(void* course);

struct Actor* m_GetActor(size_t index);
void m_DeleteActor(size_t index);
struct Actor* m_AddBaseActor(void);
size_t m_GetActorSize();
size_t m_FindActorIndex(struct Actor* actor);
void m_ActorCollision(Player* player, struct Actor* actor);
void CM_CleanWorld(void);

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
