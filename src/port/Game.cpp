#include <libultraship.h>

#include "Game.h"
#include "port/Engine.h"

#include <graphic/Fast3D/Fast3dWindow.h>
#include "engine/World.h"
#include "engine/tracks/Track.h"
#include "engine/tracks/MarioRaceway.h"
#include "engine/tracks/ChocoMountain.h"
#include "engine/tracks/BowsersCastle.h"
#include "engine/tracks/BansheeBoardwalk.h"
#include "engine/tracks/YoshiValley.h"
#include "engine/tracks/FrappeSnowland.h"
#include "engine/tracks/KoopaTroopaBeach.h"
#include "engine/tracks/RoyalRaceway.h"
#include "engine/tracks/LuigiRaceway.h"
#include "engine/tracks/MooMooFarm.h"
#include "engine/tracks/ToadsTurnpike.h"
#include "engine/tracks/KalimariDesert.h"
#include "engine/tracks/SherbetLand.h"
#include "engine/tracks/RainbowRoad.h"
#include "engine/tracks/WarioStadium.h"
#include "engine/tracks/BlockFort.h"
#include "engine/tracks/Skyscraper.h"
#include "engine/tracks/DoubleDeck.h"
#include "engine/tracks/DKJungle.h"
#include "engine/tracks/BigDonut.h"
#include "engine/tracks/TestTrack.h"

#include "engine/tracks/PodiumCeremony.h"

#include "engine/GarbageCollector.h"

#include "engine/TrainCrossing.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Lakitu.h"

#include "Smoke.h"

#include "engine/HM_Intro.h"

#include "engine/editor/Editor.h"
#include "engine/editor/SceneManager.h"
#include "RegisteredActors.h"

#include "engine/cameras/GameCamera.h"
#include "engine/cameras/FreeCamera.h"
#include "engine/cameras/TourCamera.h"
#include "engine/cameras/LookBehindCamera.h"

#ifdef _WIN32
#include <locale.h>
#endif

extern "C" {
#include "main.h"
#include "audio/load.h"
#include "audio/external.h"
#include "render_courses.h"
#include "menus.h"
#include "update_objects.h"
#include "spawn_players.h"
#include "src/enhancements/collision_viewer.h"
#include "code_800029B0.h"
// #include "engine/wasm.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

// Create the world instance
World gWorldInstance;

// Deferred cleaning when clearing all actors in the editor
bool bCleanWorld = false;

std::shared_ptr<PodiumCeremony> gPodiumCeremony;

Cup* gMushroomCup;
Cup* gFlowerCup;
Cup* gStarCup;
Cup* gSpecialCup;
Cup* gBattleCup;

HarbourMastersIntro gMenuIntro;

Editor::Editor gEditor;

s32 gTrophyIndex = NULL;

void CustomEngineInit() {
    /* Add all tracks to the global track list */
    std::shared_ptr<Track> mario         = gWorldInstance.AddTrack(std::make_shared<MarioRaceway>());
    std::shared_ptr<Track> choco         = gWorldInstance.AddTrack(std::make_shared<ChocoMountain>());
    std::shared_ptr<Track> bowser        = gWorldInstance.AddTrack(std::make_shared<BowsersCastle>());
    std::shared_ptr<Track> banshee       = gWorldInstance.AddTrack(std::make_shared<BansheeBoardwalk>());
    std::shared_ptr<Track> yoshi         = gWorldInstance.AddTrack(std::make_shared<YoshiValley>());
    std::shared_ptr<Track> frappe        = gWorldInstance.AddTrack(std::make_shared<FrappeSnowland>());
    std::shared_ptr<Track> koopa         = gWorldInstance.AddTrack(std::make_shared<KoopaTroopaBeach>());
    std::shared_ptr<Track> royal         = gWorldInstance.AddTrack(std::make_shared<RoyalRaceway>());
    std::shared_ptr<Track> luigi         = gWorldInstance.AddTrack(std::make_shared<LuigiRaceway>());
    std::shared_ptr<Track> mooMoo        = gWorldInstance.AddTrack(std::make_shared<MooMooFarm>());
    std::shared_ptr<Track> toads         = gWorldInstance.AddTrack(std::make_shared<ToadsTurnpike>());
    std::shared_ptr<Track> kalimari      = gWorldInstance.AddTrack(std::make_shared<KalimariDesert>());
    std::shared_ptr<Track> sherbet       = gWorldInstance.AddTrack(std::make_shared<SherbetLand>());
    std::shared_ptr<Track> rainbow       = gWorldInstance.AddTrack(std::make_shared<RainbowRoad>());
    std::shared_ptr<Track> wario         = gWorldInstance.AddTrack(std::make_shared<WarioStadium>());
    std::shared_ptr<Track> block         = gWorldInstance.AddTrack(std::make_shared<BlockFort>());
    std::shared_ptr<Track> skyscraper    = gWorldInstance.AddTrack(std::make_shared<Skyscraper>());
    std::shared_ptr<Track> doubleDeck    = gWorldInstance.AddTrack(std::make_shared<DoubleDeck>());
    std::shared_ptr<Track> dkJungle      = gWorldInstance.AddTrack(std::make_shared<DKJungle>());
    std::shared_ptr<Track> bigDonut      = gWorldInstance.AddTrack(std::make_shared<BigDonut>());
//    std::shared_ptr<Track> harbour       = gWorldInstance.AddTrack(std::make_shared<Harbour>());
    std::shared_ptr<Track> testTrack    = gWorldInstance.AddTrack(std::make_shared<TestTrack>());

    gPodiumCeremony = std::make_unique<PodiumCeremony>();

    // Construct cups with vectors of Track* (non-owning references)
    gMushroomCup = new Cup("mk:mushroom_cup", "Mushroom Cup", {
        luigi, mooMoo, koopa, kalimari
    });

    gFlowerCup = new Cup("mk:flower_cup", "Flower Cup", {
        toads, frappe, choco, mario
    });

    gStarCup = new Cup("mk:star_cup", "Star Cup", {
        wario, sherbet, royal, bowser
    });

    gSpecialCup = new Cup("mk:special_cup", "Special Cup", {
        dkJungle, yoshi, banshee, rainbow
    });

    gBattleCup = new Cup("mk:battle_cup", "Battle Cup", {
        bigDonut, block, doubleDeck, skyscraper
    });

    /* Instantiate Cups */
    gWorldInstance.AddCup(gMushroomCup);
    gWorldInstance.AddCup(gFlowerCup);
    gWorldInstance.AddCup(gStarCup);
    gWorldInstance.AddCup(gSpecialCup);
    gWorldInstance.AddCup(gBattleCup);

    //SelectMarioRaceway(); // This results in a nullptr
    SetMarioRaceway();

    RegisterGameActors();
}

void CustomEngineDestroy() {
    delete gMushroomCup;
    delete gFlowerCup;
    delete gStarCup;
    delete gSpecialCup;
    delete gBattleCup;
}

extern "C" {

void HM_InitIntro() {
    gMenuIntro.HM_InitIntro();
}

void HM_TickIntro() {
    gMenuIntro.HM_TickIntro();
}

void HM_DrawIntro() {
    gMenuIntro.HM_DrawIntro();
}

// Set default track; mario raceway
void SetMarioRaceway(void) {
    SetTrackById(0);
    gWorldInstance.SetCurrentCup(gMushroomCup);
    gWorldInstance.GetCurrentCup()->CursorPosition = 3;
    gWorldInstance.CupIndex = 0;
}

u32 WorldNextCup(void) {
    return gWorldInstance.NextCup();
}

u32 WorldPreviousCup(void) {
    return gWorldInstance.PreviousCup();
}

void CM_SetCup(void* cup) {
    gWorldInstance.SetCurrentCup((Cup*) cup);
}

void* GetCup() {
    return gWorldInstance.GetCurrentCup();
}

u32 GetCupIndex(void) {
    return gWorldInstance.GetCupIndex();
}

void CM_SetCupIndex(size_t index) {
    gWorldInstance.SetCupIndex(index);
}

const char* GetCupName(void) {
    return gWorldInstance.GetCurrentCup()->Name;
}

void LoadTrack() {
    gWorldInstance.GetRaceManager().Load();
}

// Unload can be call to frequently so even when if the track wasn't loaded before
void UnLoadTrack() {
    gWorldInstance.GetRaceManager().UnLoad();
}

size_t GetTrackIndex() {
    return gWorldInstance.TrackIndex;
}

void SetTrack(const char* name) {
    gWorldInstance.SetTrack(name);
}

void NextTrack() {
    gWorldInstance.NextTrack();
}

void PreviousTrack() {
    gWorldInstance.PreviousTrack();
}

void SetTrackById(s32 track) {
    if (track < 0 || track >= gWorldInstance.Tracks.size()) {
        return;
    }
    gWorldInstance.TrackIndex = track;
    gWorldInstance.SetCurrentTrack(gWorldInstance.Tracks[gWorldInstance.TrackIndex]);
}

void CM_VehicleCollision(s32 playerId, Player* player) {
    for (auto& actor : gWorldInstance.Actors) {
        if (actor) {
            actor->VehicleCollision(playerId, player);
        }
    }
}

void CM_BombKartsWaypoint(s32 cameraId) {
    for (auto& object : gWorldInstance.Objects) {
        if (auto kart = dynamic_cast<OBombKart*>(object)) {
            if (kart) {
                kart->Waypoint(cameraId);
            }
        }
    }
}

void CM_DisplayBattleBombKart(s32 playerId, s32 primAlpha) {

    if ((playerId < 0) || (playerId > 4)) {
        return;
    }

    if (primAlpha == 0) {
        gWorldInstance.playerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::DISABLED;
        gWorldInstance.playerBombKart[playerId]._primAlpha = primAlpha;
    } else {
        gWorldInstance.playerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::ACTIVE;
        gWorldInstance.playerBombKart[playerId]._primAlpha = primAlpha;
    }
}

void CM_DrawBattleBombKarts(s32 cameraId) {
    for (size_t i = 0; i < gPlayerCount; i++) {
        gWorldInstance.playerBombKart[i].Draw(i, cameraId);
    }
}

void CM_ClearVehicles(void) {
    gWorldInstance.Crossings.clear();
}

void CM_CrossingTrigger() {
    for (auto& crossing : gWorldInstance.Crossings) {
        if (crossing) {
            crossing->CrossingTrigger();
        }
    }
}

void CM_AICrossingBehaviour(s32 playerId) {
    for (auto& crossing : gWorldInstance.Crossings) {
        if (crossing) {
            crossing->AICrossingBehaviour(playerId);
        }
    }
}

s32 CM_GetCrossingOnTriggered(uintptr_t* crossing) {
    TrainCrossing* ptr = (TrainCrossing*) crossing;
    if (ptr) {
        return ptr->OnTriggered;
    }
}

/**
 * Tracks are rendered in two ways
 * 1) Track sections --> The scene is split into multiple sections and rendered piece by piece
 * 2) Full scene --> The entire scene is rendered at once
 * 
 * Custom tracks only use the Render() method, and they only render the full scene.
 * They do not use DrawCredits() and they do not use track sections.
 */
void CM_DrawTrack(ScreenContext* screen) {
    if (nullptr == gWorldInstance.GetTrack()) {
        return;
    }

    // Custom tracks should never use DrawCredits();
    if (gWorldInstance.GetTrack()->IsMod()) {
        switch(screen->camera->renderMode) {
            default:
                gWorldInstance.GetTrack()->Draw(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        } 
    } else {
        switch(screen->camera->renderMode) {
            case RENDER_FULL_SCENE:
                gWorldInstance.GetTrack()->DrawCredits();
                break;
            case RENDER_TRACK_SECTIONS:
                gWorldInstance.GetTrack()->Draw(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        }
    }
}

void CM_TickActors() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.TickActors();
    }
}

void CM_DrawActors(Camera* camera) {
    //AActor* a = gWorldInstance.ConvertActorToAActor(actor);
    for (const auto& actor : gWorldInstance.Actors) {
        if (actor->IsMod()) {
            actor->Draw(camera);
        }
    }

    for (auto* camera : gWorldInstance.Cameras) {
        if (auto* tourCam = dynamic_cast<TourCamera*>(camera)) {
            if (tourCam->IsActive()) {
                tourCam->Draw();
            }
        }
    }
}

void CM_DrawStaticMeshActors() {
    gWorldInstance.DrawStaticMeshActors();
}

void CM_BeginPlay() {
    static bool tour = false;
    auto track = gWorldInstance.GetTrack();
    
    if (nullptr == track) {
        return; 
    }

    if (tour) {
      //  gWorldInstance.Cameras[2]->SetActive(true);
       // gScreenOneCtx->camera = gWorldInstance.Cameras[2]->Get();
        if (reinterpret_cast<TourCamera*>(gWorldInstance.Cameras[2])->IsTourComplete()) {
            tour = false;
            gScreenOneCtx->pendingCamera = &cameras[0];
        }
    }

    if (gWorldInstance.GetTrack()) {
        // This line should likely be moved.
        // It's here so PreInit is after the scene file has been loaded
        // It used to be at the start of BeginPlay
        Editor::LoadLevel(gWorldInstance.GetTrack().get(), gWorldInstance.GetTrack()->SceneFilePtr);
    }

    gWorldInstance.GetRaceManager().PreInit();
    gWorldInstance.GetRaceManager().BeginPlay();
    gWorldInstance.GetRaceManager().PostInit();
}

Camera* CM_GetPlayerCamera(s32 playerIndex) {
    for (GameCamera* cam : gWorldInstance.Cameras) {
        // Make sure this is a player camera and not a different type of camera
        if (typeid(*cam) == typeid(GameCamera)) {
            Camera* camera = cam->Get();
            if (camera->playerId == playerIndex) {
                return camera;
            }
        }
    }
    return nullptr;
}

void CM_SetViewProjection(Camera* camera) {
    for (GameCamera* gameCamera : gWorldInstance.Cameras) {
        if (camera == gameCamera->Get()) {
            gameCamera->SetViewProjection();
        }
    }
}

void CM_TickCameras() {
    gWorldInstance.TickCameras();
}

Camera* CM_AddCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (gWorldInstance.Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    gWorldInstance.Cameras.push_back(new GameCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return gWorldInstance.Cameras.back()->Get();
}

Camera* CM_AddFreeCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (gWorldInstance.Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    gWorldInstance.Cameras.push_back(new FreeCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return gWorldInstance.Cameras.back()->Get();
}

Camera* CM_AddTourCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (gWorldInstance.Cameras.size() >= NUM_CAMERAS) {
        // This is to prevent soft locking the game
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        if (gWorldInstance.GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (nullptr == gWorldInstance.GetTrack()) {
        // This is to prevent soft locking the game
        if (gWorldInstance.GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (gWorldInstance.GetTrack()->TourShots.size() == 0) {
        // This is to prevent soft locking the game
        if (gWorldInstance.GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    gWorldInstance.Cameras.push_back(new TourCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    TourCamera* tour = static_cast<TourCamera*>(gWorldInstance.Cameras.back());
    tour->SetActive(true);
    return tour->Get();
}

bool CM_IsTourEnabled() {
    if (nullptr != gWorldInstance.GetTrack()) {
        if ((gWorldInstance.GetTrack()->bTourEnabled) && (gTourComplete == false)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Camera* CM_AddLookBehindCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (gWorldInstance.Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    gWorldInstance.Cameras.push_back(new LookBehindCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return gWorldInstance.Cameras.back()->Get();
}

void CM_AttachCamera(Camera* camera, s32 playerIdx) {
    camera->playerId = playerIdx;
}

void CM_CameraSetActive(size_t idx, bool state) {
    if (idx < gWorldInstance.Cameras.size()) {
        gWorldInstance.Cameras[idx]->SetActive(state);
    }
}

void CM_SetFreeCamera(bool state) {
    for (auto* cam : gWorldInstance.Cameras) {
        if (cam->Get() == gScreenOneCtx->freeCamera) {
            if (state) {
                gScreenOneCtx->pendingCamera = gScreenOneCtx->freeCamera;
                cam->SetActive(true);
            } else {
                if (nullptr != gScreenOneCtx->raceCamera) {
                    if (gGamestate == RACING) {
                        gScreenOneCtx->pendingCamera = gScreenOneCtx->raceCamera;
                        cam->SetActive(false);
                    } else {
                        cam->SetActive(false);
                    }
                }
            }
        }
    }
}

void CM_ActivateTourCamera(Camera* camera) {
    for (auto* cam : gWorldInstance.Cameras) {
        if (cam->Get() == camera) {
            cam->SetActive(true);
        }
    }
}

void CM_TickObjects() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.TickObjects();
    }
}

// A couple objects such as lakitu are ticked inside of process_game_tick which support 60fps.
// This is a fallback to support that.
void CM_TickObjects60fps() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.TickObjects60fps();
    }
}

void CM_DrawObjects(s32 cameraId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.DrawObjects(cameraId);
    }
}

void CM_TickEditor() {
    gEditor.Tick();
}

void CM_DrawEditor() {
    gEditor.Draw();
}

void CM_Editor_SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY) {
    gEditor.SetLevelDimensions(minX, maxX, minZ, maxZ, minY, maxY);
}

void CM_TickParticles() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.TickParticles();
    }
}

void CM_DrawParticles(s32 cameraId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.DrawParticles(cameraId);
    }
}

void CM_InitClouds() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->InitClouds();
    }
}

void CM_TickClouds(s32 arg0, Camera* camera) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->TickClouds(arg0, camera);
    }
}

void CM_Waypoints(Player* player, int8_t playerId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->Waypoints(player, playerId);
    }
}

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
}

void CM_InitTrackObjects() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->InitTrackObjects();
    }
}

void CM_TickTrackObjects() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->TickTrackObjects();
    }
    TrainSmokeTick();
}

void CM_DrawTrackObjects(s32 cameraId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->DrawTrackObjects(cameraId);
    }

    TrainSmokeDraw(cameraId);
}

void CM_SomeSounds() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->SomeSounds();
    }
}

void CM_CreditsSpawnActors() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->CreditsSpawnActors();
    }
}

void CM_WhatDoesThisDo(Player* player, int8_t playerId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->WhatDoesThisDo(player, playerId);
    }
}

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->WhatDoesThisDoAI(player, playerId);
    }
}

void CM_SetStaffGhost() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->SetStaffGhost();
    }
}

Properties* CM_GetProps() {
    if (gWorldInstance.GetTrack()) {
        return &gWorldInstance.GetTrack()->Props;
    }
    return NULL;
}

Properties* CM_GetPropsTrackId(s32 trackId) {
    return &gWorldInstance.Tracks[trackId]->Props;
}

void CM_ScrollingTextures() {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->ScrollingTextures();
    }
}

void CM_DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection) {
    if (gWorldInstance.GetTrack()) {
        gWorldInstance.GetTrack()->DrawWater(screen, pathCounter, cameraRot, playerDirection);
    }
}

/**
 * This should only be ran once per track, otherwise animation/timings might become sped up.
 */
void CM_SpawnStarterLakitu() {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }

    for (size_t i = 0; i < gPlayerCountSelection1; i++) {
        OLakitu* lakitu = new OLakitu(i, OLakitu::LakituType::STARTER);
        gWorldInstance.Lakitus[i] = lakitu;
        gWorldInstance.AddObject(lakitu);
    }
}

// Checkered flag lakitu
void CM_ActivateFinishLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::FINISH);
}

void CM_ActivateSecondLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::SECOND_LAP);
}

void CM_ActivateFinalLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::FINAL_LAP);
}

void CM_ActivateReverseLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::REVERSE);
}

size_t GetCupCursorPosition() {
    return gWorldInstance.GetCurrentCup()->CursorPosition;
}

void SetCupCursorPosition(size_t position) {
    gWorldInstance.GetCurrentCup()->SetTrack(position);
    // gWorldInstance.CurrentCup->CursorPosition = position;
}

size_t GetCupSize() {
    return gWorldInstance.GetCurrentCup()->GetSize();
}

void SetTrackFromCup() {
    gWorldInstance.SetCurrentTrack(gWorldInstance.GetCurrentCup()->GetTrack());
}

void* GetTrack(void) {
    return gWorldInstance.GetTrack().get();
}

struct Actor* CM_GetActor(size_t index) {
    if (index < gWorldInstance.Actors.size()) {
        AActor* actor = gWorldInstance.Actors[index];
        return reinterpret_cast<struct Actor*>(reinterpret_cast<char*>(actor) + sizeof(void*));
    } else {
        // throw std::runtime_error("GetActor() index out of bounds");
        return NULL;
    }
}

size_t CM_FindActorIndex(Actor* actor) {
    // Move the ptr back to look at the vtable.
    // This gets us the proper C++ class instead of just the variables used in C.
    AActor* a = reinterpret_cast<AActor*>(reinterpret_cast<char*>(actor) - sizeof(void*));
    auto actors = gWorldInstance.Actors;

    auto it = std::find(actors.begin(), actors.end(), static_cast<AActor*>(a));
    if (it != actors.end()) {
        return std::distance(actors.begin(), it);
    }
    printf("FindActorIndex() actor not found\n");
    return -1;
}

void CM_DeleteActor(size_t index) {
    std::vector<AActor*> actors = gWorldInstance.Actors;
    if (index < actors.size()) {
        actors.erase(actors.begin() + index);
    }
}

/**
 * Clean up actors and other game objects.
 */
void CM_CleanWorld(void) {
    gWorldInstance.CleanWorld();
}

void CM_CleanCameras(void) {
    for (auto& camera : gWorldInstance.Cameras) {
        delete camera;
    }

    gWorldInstance.Cameras.clear();
}

struct Actor* CM_AddBaseActor() {
    return (struct Actor*) gWorldInstance.AddBaseActor();
}

void CM_ActorBeginPlay(struct Actor* actor) {
    gWorldInstance.ActorBeginPlay(actor);
}

void CM_ActorGenerateCollision(struct Actor* actor) {
    AActor* act = gWorldInstance.ConvertActorToAActor(actor);

    if ((nullptr != act->Model) && (act->Model[0] != '\0')) {
        if (act->Triangles.size() == 0) {
            Editor::GenerateCollisionMesh(act, (Gfx*)LOAD_ASSET_RAW(act->Model), 1.0f);
        }
    }
}

void Editor_AddLight(s8* direction) {
    static size_t i = 0;
    gEditor.AddLight(("Light "+std::to_string(i)).c_str(), nullptr, direction);
    i += 1;
}

void Editor_ClearMatrix() {
    gEditor.ClearMatrixPool();
}

void Editor_CleanWorld() {
    if (bCleanWorld) {
        CM_CleanWorld();
        bCleanWorld = false;
    }
}

size_t CM_GetActorSize() {
    return gWorldInstance.Actors.size();
}

void CM_ActorCollision(Player* player, Actor* actor) {
    AActor* a = gWorldInstance.ConvertActorToAActor(actor);

    if (a->IsMod()) {
        a->Collision(player, a);
    }
}

f32 CM_GetWaterLevel(Vec3f pos, Collision* collision) {
    FVector fPos = {pos[0], pos[1], pos[2]};
    return gWorldInstance.GetTrack()->GetWaterLevel(fPos, collision);
}

// clang-format off
bool IsMarioRaceway()     { return dynamic_cast<MarioRaceway*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsLuigiRaceway()     { return dynamic_cast<LuigiRaceway*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsChocoMountain()    { return dynamic_cast<ChocoMountain*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsBowsersCastle()    { return dynamic_cast<BowsersCastle*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsBansheeBoardwalk() { return dynamic_cast<BansheeBoardwalk*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsYoshiValley()      { return dynamic_cast<YoshiValley*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsFrappeSnowland()   { return dynamic_cast<FrappeSnowland*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsKoopaTroopaBeach() { return dynamic_cast<KoopaTroopaBeach*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsRoyalRaceway()     { return dynamic_cast<RoyalRaceway*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsMooMooFarm()       { return dynamic_cast<MooMooFarm*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsToadsTurnpike()    { return dynamic_cast<ToadsTurnpike*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsKalimariDesert()   { return dynamic_cast<KalimariDesert*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsSherbetLand()      { return dynamic_cast<SherbetLand*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsRainbowRoad()      { return dynamic_cast<RainbowRoad*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsWarioStadium()     { return dynamic_cast<WarioStadium*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsBlockFort()        { return dynamic_cast<BlockFort*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsSkyscraper()       { return dynamic_cast<Skyscraper*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsDoubleDeck()       { return dynamic_cast<DoubleDeck*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsDkJungle()         { return dynamic_cast<DKJungle*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsBigDonut()         { return dynamic_cast<BigDonut*>(gWorldInstance.GetTrack().get()) != nullptr; }
bool IsPodiumCeremony()   { return dynamic_cast<PodiumCeremony*>(gWorldInstance.GetTrack().get()) != nullptr; }

void SelectMarioRaceway()       { gWorldInstance.SetTrackByType<MarioRaceway>(); }
void SelectLuigiRaceway()       { gWorldInstance.SetTrackByType<LuigiRaceway>(); }
void SelectChocoMountain()      { gWorldInstance.SetTrackByType<ChocoMountain>(); }
void SelectBowsersCastle()      { gWorldInstance.SetTrackByType<BowsersCastle>(); }
void SelectBansheeBoardwalk()   { gWorldInstance.SetTrackByType<BansheeBoardwalk>(); }
void SelectYoshiValley()        { gWorldInstance.SetTrackByType<YoshiValley>(); }
void SelectFrappeSnowland()     { gWorldInstance.SetTrackByType<FrappeSnowland>(); }
void SelectKoopaTroopaBeach()   { gWorldInstance.SetTrackByType<KoopaTroopaBeach>(); }
void SelectRoyalRaceway()       { gWorldInstance.SetTrackByType<RoyalRaceway>(); }
void SelectMooMooFarm()         { gWorldInstance.SetTrackByType<MooMooFarm>(); }
void SelectToadsTurnpike()      { gWorldInstance.SetTrackByType<ToadsTurnpike>(); }
void SelectKalimariDesert()     { gWorldInstance.SetTrackByType<KalimariDesert>(); }
void SelectSherbetLand()        { gWorldInstance.SetTrackByType<SherbetLand>(); }
void SelectRainbowRoad()        { gWorldInstance.SetTrackByType<RainbowRoad>(); }
void SelectWarioStadium()       { gWorldInstance.SetTrackByType<WarioStadium>(); }
void SelectBlockFort()          { gWorldInstance.SetTrackByType<BlockFort>(); }
void SelectSkyscraper()         { gWorldInstance.SetTrackByType<Skyscraper>(); }
void SelectDoubleDeck()         { gWorldInstance.SetTrackByType<DoubleDeck>(); }
void SelectDkJungle()           { gWorldInstance.SetTrackByType<DKJungle>(); }
void SelectBigDonut()           { gWorldInstance.SetTrackByType<BigDonut>(); }
void SelectPodiumCeremony()     { gWorldInstance.SetCurrentTrack(gPodiumCeremony); }
// clang-format on

void* GetMushroomCup(void) {
    return gMushroomCup;
}

void* GetFlowerCup(void) {
    return gFlowerCup;
}

void* GetStarCup(void) {
    return gStarCup;
}

void* GetSpecialCup(void) {
    return gSpecialCup;
}

void* GetBattleCup(void) {
    return gBattleCup;
}

// End of frame cleanup of actors, objects, etc.
void CM_RunGarbageCollector(void) {
    RunGarbageCollector();
}

void CM_ResetAudio(void) {
    if(HMAS_IsPlaying(HMAS_MUSIC)){
        HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_VOLUME, HMAS_LINEAR, 10, 0);
        HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_STOP,   HMAS_INSTANT, 1, 0);
    }

    // Fade out music for all sequences and music player indexes 0, and 1
    for (size_t soundId = 0; soundId < MUSIC_SEQ_MAX; soundId++) {
        func_800C3448(0x10100000 | soundId);
        func_800C3448(0x11100000 | soundId);
    }
}
}

void push_frame() {
    GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    thread5_iteration();
    GameEngine::EndAudioFrame();
    // thread5_game_loop();
    // Graphics_ThreadUpdate();w
}

#ifdef _WIN32
int SDL_main(int argc, char** argv) {
#else
#if defined(__cplusplus) && defined(PLATFORM_IOS)
extern "C"
#endif
    int
    main(int argc, char* argv[]) {
#endif
#ifdef _WIN32
    // Allow non-ascii characters for Windows
    setlocale(LC_ALL, ".UTF8");
#endif
    // load_wasm();
    GameEngine::Create();
    audio_init();
    sound_init();

    CustomEngineInit();

    switch(CVarGetInteger("gSkipIntro", 0)) {
        case 0:
            gMenuSelection = HARBOUR_MASTERS_MENU;
            break;
        case 1:
            gMenuSelection = LOGO_INTRO_MENU;
            break;
        case 2:
            gMenuSelection = START_MENU;
            break;
        case 3:
            gMenuSelection = MAIN_MENU;
            break;
    }

    // Debug mode override gSkipIntro
    if (CVarGetInteger("gEnableDebugMode", 0) == true) {
        gMenuSelection = START_MENU;
    }

    thread5_game_loop();
    gEditor.Load();
    while (WindowIsRunning()) {
        push_frame();
    }
    CustomEngineDestroy();
    // GameEngine::Instance->ProcessFrame(push_frame);
    GameEngine::Instance->Destroy();
    return 0;
}
