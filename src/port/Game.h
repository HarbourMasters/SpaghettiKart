#ifndef _GAME_H
#define _GAME_H

#include <libultraship.h>
#include "engine/Engine.h"

#ifdef __cplusplus
extern "C" {
#include "camera.h"
#endif

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

void CourseManager_InitClouds();

void CourseManager_UpdateClouds(s32 arg0, Camera* camera);

void CourseManager_Waypoints(Player* player, int8_t playerId);

void CourseManager_GenerateCollision();

void CourseManager_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                      f32* arg6, f32* arg7);

void CourseManager_MinimapSettings();

void CourseManager_InitCourseObjects();

void CourseManager_UpdateCourseObjects();

void CourseManager_RenderCourseObjects(s32 cameraId);

void CourseManager_SomeSounds();

void CourseManager_SetCourseVtxColours();

void CourseManager_WhatDoesThisDo(Player* player, int8_t playerId);

void CourseManager_WhatDoesThisDoAI(Player* player, int8_t playerId);

void CourseManager_MinimapFinishlinePosition();

void CourseManager_SetStaffGhost();

CProperties* CourseManager_GetProps();

void CourseManager_SpawnBombKarts();

void CourseManager_Water();

size_t GetCupCursorPosition();

void SetCupCursorPosition(size_t position);

size_t GetCupSize();

void SetCourseFromCup();

void* GetCourse(void);

void SetCourseByClass(void* course);

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

#ifdef __cplusplus
}
#endif

#endif // _GAME_H