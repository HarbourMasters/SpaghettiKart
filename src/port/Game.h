#ifndef _GAME_H
#define _GAME_H

#include <libultraship.h>
#include "engine/tracks/Track.h"
#include "engine/HM_Intro.h"

#ifdef __cplusplus
#include "engine/editor/Editor.h"
#include "engine/Registry.h"
class Track;
struct Properties;
class World;

template<class T, typename... TArgs> T* SpawnActor(TArgs&&... args) {
    return T::Spawn(std::forward<TArgs>(args)...);
}

extern "C" {
#endif
#include "camera.h"
#include "actor_types.h"
#include "code_800029B0.h"

extern s32 gTrophyIndex;

#ifdef __cplusplus
extern Editor::Editor gEditor;
extern HarbourMastersIntro gMenuIntro;
extern bool bCleanWorld;
extern Registry<TrackInfo> gTrackRegistry;
extern Registry<ActorInfo, const SpawnParams&> gActorRegistry;
World* GetWorld(void); // Retrieve the world instance
#endif
// NOLINTBEGIN(readability-identifier-naming)

uintptr_t CM_GetTrack();
Properties* CM_GetProps();

void HM_InitIntro(void);
void HM_TickIntro(void);
void HM_DrawIntro(void);

void CM_DisplayBattleBombKart(s32 playerId, s32 primAlpha);
void CM_DrawBattleBombKarts(s32 cameraId);

u32 WorldNextCup(void);

u32 WorldPreviousCup(void);

u32 GetCupIndex(void);

const char* GetCupName(void);

void LoadTrack();

void CM_SetCup(void*);

void CM_SetCupIndex(size_t index);

void CM_DrawTrack(ScreenContext* arg0);

void CM_SpawnStarterLakitu();
void CM_ActivateFinishLakitu(s32 playerId);
void CM_ActivateSecondLapLakitu(s32 playerId);
void CM_ActivateFinalLapLakitu(s32 playerId);
void CM_ActivateReverseLakitu(s32 playerId);

void CM_InitClouds();

void CM_DrawActors(Camera* camera);
void CM_DrawStaticMeshActors();

Camera* CM_GetPlayerCamera(s32 playerIndex);
void CM_SetViewProjection(Camera* camera);
void CM_TickCameras();
Camera* CM_AddCamera(Vec3f spawn, s16 rot, u32 mode);
Camera* CM_AddFreeCamera(Vec3f spawn, s16 rot, u32 mode);
Camera* CM_AddTourCamera(Vec3f spawn, s16 rot, u32 mode);
bool CM_IsTourEnabled();
Camera* CM_AddLookBehindCamera(Vec3f spawn, s16 rot, u32 mode);
void CM_AttachCamera(Camera* camera, s32 playerIdx);
void CM_SetFreeCamera(bool state);
void CM_CameraSetActive(size_t idx, bool state);
void CM_ActivateTourCamera(Camera* camera);
void CM_TickObjects();
void CM_TickObjects60fps();
void CM_DrawObjects(s32 cameraId);

void CM_TickEditor();
void CM_DrawEditor();
void CM_TickDraw();
void Editor_ClearMatrix();
void Editor_CleanWorld();

void CM_TickParticles(void);
void CM_DrawParticles(s32 cameraId);

void CM_TickClouds(s32 arg0, Camera* camera);

void CM_Waypoints(Player* player, int8_t playerId);

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7);

void CM_InitTrackObjects();

void CM_TickTrackObjects();

void CM_DrawTrackObjects(s32 cameraId);

void CM_SomeSounds();

void CM_CreditsSpawnActors();

void CM_WhatDoesThisDo(Player* player, int8_t playerId);

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId);

void CM_SetStaffGhost();

void CM_BombKartsWaypoint(s32 cameraId);

void CM_ScrollingTextures();

s32 CM_GetCrossingOnTriggered(uintptr_t* crossing);

void CM_BeginPlay();

void CM_DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection);

void CM_AICrossingBehaviour(s32 playerId);

void CM_ClearVehicles(void);

void CM_CrossingTrigger();

void CM_VehicleCollision(s32 playerId, Player* player);

void CM_TickActors();

void CM_DrawBombKarts(s32 cameraId);

void SetMarioRaceway(void);

size_t GetCupCursorPosition();

void SetCupCursorPosition(size_t position);

size_t GetCupSize();

void* GetTrack(void);

struct Actor* CM_GetActor(size_t index);
void CM_DeleteActor(size_t index);
struct Actor* CM_AddBaseActor();
void CM_ActorBeginPlay(struct Actor* actor);
void CM_ActorGenerateCollision(struct Actor* actor);
void Editor_AddLight(s8* direction);
size_t CM_GetActorSize();
size_t CM_FindActorIndex(struct Actor* actor);
void CM_ActorCollision(Player* player, struct Actor* actor);
void CM_CleanCameras(void);
void CM_CleanWorld(void);

f32 CM_GetWaterLevel(Vec3f pos, Collision* collision);

bool IsMarioRaceway();
bool IsLuigiRaceway();
bool IsChocoMountain();
bool IsBowsersCastle();
bool IsBansheeBoardwalk();
bool IsYoshiValley();
bool IsFrappeSnowland();
bool IsKoopaTroopaBeach();
bool IsRoyalRaceway();
bool IsMooMooFarm();
bool IsToadsTurnpike();
bool IsKalimariDesert();
bool IsSherbetLand();
bool IsRainbowRoad();
bool IsWarioStadium();
bool IsBlockFort();
bool IsSkyscraper();
bool IsDoubleDeck();
bool IsDkJungle();
bool IsBigDonut();
bool IsPodiumCeremony();

void SelectMarioRaceway();
void SelectLuigiRaceway();
void SelectChocoMountain();
void SelectBowsersCastle();
void SelectBansheeBoardwalk();
void SelectYoshiValley();
void SelectFrappeSnowland();
void SelectKoopaTroopaBeach();
void SelectRoyalRaceway();
void SelectMooMooFarm();
void SelectToadsTurnpike();
void SelectKalimariDesert();
void SelectSherbetLand();
void SelectRainbowRoad();
void SelectWarioStadium();
void SelectBlockFort();
void SelectSkyscraper();
void SelectDoubleDeck();
void SelectDkJungle();
void SelectBigDonut();
void SelectPodiumCeremony();

void* GetMushroomCup(void);

void* GetFlowerCup(void);

void* GetStarCup(void);

void* GetSpecialCup(void);

void* GetBattleCup(void);

void* GetCup();

void CM_RunGarbageCollector(void);
void CM_ResetAudio(void);

// NOLINTEND(readability-identifier-naming)

#ifdef __cplusplus
}
#endif

#endif // _GAME_H
