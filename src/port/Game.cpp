#include <libultraship.h>

#include "Game.h"
#include "port/Engine.h"

#include <fast/Fast3dWindow.h>
#include <memory>
#include "engine/World.h"
#include "engine/AllTracks.h"

#include "engine/tracks/PodiumCeremony.h"

#include "engine/GarbageCollector.h"

#include "engine/TrainCrossing.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Lakitu.h"

#include "Smoke.h"

#include "engine/HM_Intro.h"

#include "engine/editor/Editor.h"
#include "engine/editor/SceneManager.h"
#include "RegisterContent.h"

#include "engine/cameras/GameCamera.h"
#include "engine/cameras/FreeCamera.h"
#include "engine/cameras/TourCamera.h"
#include "engine/cameras/LookBehindCamera.h"

#include "engine/TrackBrowser.h"

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
static World sWorldInstance;

// Deferred cleaning when clearing all actors in the editor
bool bCleanWorld = false;

std::unique_ptr<PodiumCeremony> gPodiumCeremony;

Cup* gMushroomCup;
Cup* gFlowerCup;
Cup* gStarCup;
Cup* gSpecialCup;
Cup* gBattleCup;

HarbourMastersIntro gMenuIntro;

Editor::Editor gEditor;

s32 gTrophyIndex = NULL;

Registry<TrackInfo> gTrackRegistry;
Registry<ActorInfo, const SpawnParams&> gActorRegistry;

std::unique_ptr<TrackBrowser> gTrackBrowser;

World* GetWorld() {
    return World::Instance;
}

void CustomEngineInit() {
    // Close the editor because lus remembers if it was open
    // This also turns off freecam
    gEditor.Disable();

    RegisterTracks(gTrackRegistry);
    gTrackBrowser = std::make_unique<TrackBrowser>(gTrackRegistry);
    TrackBrowser::Instance->FindCustomTracks();
    TrackBrowser::Instance->Refresh(gTrackRegistry);

    gPodiumCeremony = std::make_unique<PodiumCeremony>();

    gMushroomCup = new Cup("mk:mushroom_cup", "Mushroom Cup", {
        "mk:luigi_raceway", 
        "mk:moo_moo_farm", 
        "mk:koopa_troopa_beach", 
        "mk:kalimari_desert"
    });

    gFlowerCup = new Cup("mk:flower_cup", "Flower Cup", {
        "mk:toads_turnpike", 
        "mk:frappe_snowland", 
        "mk:choco_mountain", 
        "mk:mario_raceway"
    });

    gStarCup = new Cup("mk:star_cup", "Star Cup", {
        "mk:wario_stadium", 
        "mk:sherbet_land", 
        "mk:royal_raceway", 
        "mk:bowsers_castle"
    });

    gSpecialCup = new Cup("mk:special_cup", "Special Cup", {
        "mk:dk_jungle", 
        "mk:yoshi_valley", 
        "mk:banshee_boardwalk", 
        "mk:rainbow_road"
    });

    gBattleCup = new Cup("mk:battle_cup", "Battle Cup", {
        "mk:big_donut", 
        "mk:block_fort", 
        "mk:double_deck", 
        "mk:skyscraper"
    });

    /* Instantiate Cups */
    GetWorld()->AddCup(gMushroomCup);
    GetWorld()->AddCup(gFlowerCup);
    GetWorld()->AddCup(gStarCup);
    GetWorld()->AddCup(gSpecialCup);
    GetWorld()->AddCup(gBattleCup);

    SetMarioRaceway();

    RegisterActors(gActorRegistry);
}

void CustomEngineDestroy() {
    gTrackRegistry.Clear();
    gActorRegistry.Clear();
    gPodiumCeremony = nullptr;
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
    SelectMarioRaceway();
    GetWorld()->SetCurrentCup(gMushroomCup);
    GetWorld()->GetCurrentCup()->CursorPosition = 3;
    GetWorld()->CupIndex = 0;
}

u32 WorldNextCup(void) {
    return GetWorld()->NextCup();
}

u32 WorldPreviousCup(void) {
    return GetWorld()->PreviousCup();
}

void CM_SetCup(void* cup) {
    GetWorld()->SetCurrentCup((Cup*) cup);
}

void* GetCup() {
    return GetWorld()->GetCurrentCup();
}

u32 GetCupIndex(void) {
    return GetWorld()->GetCupIndex();
}

void CM_SetCupIndex(size_t index) {
    GetWorld()->SetCupIndex(index);
}

const char* GetCupName(void) {
    return GetWorld()->GetCurrentCup()->Name;
}

void LoadTrack() {
    GetWorld()->GetRaceManager().Load();
}

void CM_VehicleCollision(s32 playerId, Player* player) {
    for (auto& actor : GetWorld()->Actors) {
        if (actor) {
            actor->VehicleCollision(playerId, player);
        }
    }
}

void CM_BombKartsWaypoint(s32 cameraId) {
    for (auto& object : GetWorld()->Objects) {
        if (auto* kart = dynamic_cast<OBombKart*>(object.get())) {
            if (kart != nullptr) {
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
        GetWorld()->mPlayerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::DISABLED;
        GetWorld()->mPlayerBombKart[playerId]._primAlpha = primAlpha;
    } else {
        GetWorld()->mPlayerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::ACTIVE;
        GetWorld()->mPlayerBombKart[playerId]._primAlpha = primAlpha;
    }
}

void CM_DrawBattleBombKarts(s32 cameraId) {
    for (size_t i = 0; i < gPlayerCount; i++) {
        GetWorld()->mPlayerBombKart[i].Draw(i, cameraId);
    }
}

void CM_ClearVehicles(void) {
    GetWorld()->Crossings.clear();
}

void CM_CrossingTrigger() {
    for (auto& crossing : GetWorld()->Crossings) {
        if (crossing) {
            crossing->CrossingTrigger();
        }
    }
}

void CM_AICrossingBehaviour(s32 playerId) {
    for (auto& crossing : GetWorld()->Crossings) {
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
    if (nullptr == GetWorld()->GetTrack()) {
        return;
    }

    // Custom tracks should never use DrawCredits();
    if (GetWorld()->GetTrack()->IsMod()) {
        switch(screen->camera->renderMode) {
            default:
                GetWorld()->GetTrack()->Draw(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        } 
    } else {
        switch(screen->camera->renderMode) {
            case RENDER_FULL_SCENE:
                GetWorld()->GetTrack()->DrawCredits();
                break;
            case RENDER_TRACK_SECTIONS:
                GetWorld()->GetTrack()->Draw(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        }
    }
}

void CM_TickActors() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->TickActors();
    }
}

void CM_DrawActors(Camera* camera) {
    //AActor* a = GetWorld()->ConvertActorToAActor(actor);
    for (const auto& actor : GetWorld()->Actors) {
        if (actor->IsMod()) {
            actor->Draw(camera);
        }
    }

    for (auto* camera : GetWorld()->Cameras) {
        if (auto* tourCam = dynamic_cast<TourCamera*>(camera)) {
            if (tourCam->IsActive()) {
                tourCam->Draw();
            }
        }
    }
}

void CM_DrawStaticMeshActors() {
    GetWorld()->DrawStaticMeshActors();
}

void CM_BeginPlay() {
    static bool tour = false;
    auto track = GetWorld()->GetTrack();
    
    if (nullptr == track) {
        return; 
    }

    if (tour) {
      //  GetWorld()->Cameras[2]->SetActive(true);
       // gScreenOneCtx->camera = GetWorld()->Cameras[2]->Get();
        if (reinterpret_cast<TourCamera*>(GetWorld()->Cameras[2])->IsTourComplete()) {
            tour = false;
            gScreenOneCtx->pendingCamera = &cameras[0];
        }
    }

    GetWorld()->GetRaceManager().PreInit();
    GetWorld()->GetRaceManager().BeginPlay();
    GetWorld()->GetRaceManager().PostInit();
}

Camera* CM_GetPlayerCamera(s32 playerIndex) {
    for (GameCamera* cam : GetWorld()->Cameras) {
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
    for (GameCamera* gameCamera : GetWorld()->Cameras) {
        if (camera == gameCamera->Get()) {
            gameCamera->SetViewProjection();
        }
    }
}

void CM_TickCameras() {
    GetWorld()->TickCameras();
}

Camera* CM_AddCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (GetWorld()->Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    GetWorld()->Cameras.push_back(new GameCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return GetWorld()->Cameras.back()->Get();
}

Camera* CM_AddFreeCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (GetWorld()->Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    GetWorld()->Cameras.push_back(new FreeCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return GetWorld()->Cameras.back()->Get();
}

Camera* CM_AddTourCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (GetWorld()->Cameras.size() >= NUM_CAMERAS) {
        // This is to prevent soft locking the game
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        if (GetWorld()->GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (nullptr == GetWorld()->GetTrack()) {
        // This is to prevent soft locking the game
        if (GetWorld()->GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (GetWorld()->GetTrack()->TourShots.size() == 0) {
        // This is to prevent soft locking the game
        if (GetWorld()->GetTrack()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    GetWorld()->Cameras.push_back(new TourCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    TourCamera* tour = static_cast<TourCamera*>(GetWorld()->Cameras.back());
    tour->SetActive(true);
    return tour->Get();
}

bool CM_IsTourEnabled() {
    if (nullptr != GetWorld()->GetTrack()) {
        if ((GetWorld()->GetTrack()->bTourEnabled) && (gTourComplete == false)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Camera* CM_AddLookBehindCamera(Vec3f spawn, s16 rot, u32 mode) {
    if (GetWorld()->Cameras.size() >= NUM_CAMERAS) {
        printf("Reached the max number of cameras, %d\n", NUM_CAMERAS);
        return nullptr;
    }
    GetWorld()->Cameras.push_back(new LookBehindCamera(FVector(spawn[0], spawn[1], spawn[2]), rot, mode));
    return GetWorld()->Cameras.back()->Get();
}

void CM_AttachCamera(Camera* camera, s32 playerIdx) {
    camera->playerId = playerIdx;
}

void CM_CameraSetActive(size_t idx, bool state) {
    if (idx < GetWorld()->Cameras.size()) {
        GetWorld()->Cameras[idx]->SetActive(state);
    }
}

void CM_SetFreeCamera(bool state) {
    for (auto* cam : GetWorld()->Cameras) {
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
    for (auto* cam : GetWorld()->Cameras) {
        if (cam->Get() == camera) {
            cam->SetActive(true);
        }
    }
}

void CM_TickObjects() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->TickObjects();
    }
}

// A couple objects such as lakitu are ticked inside of process_game_tick which support 60fps.
// This is a fallback to support that.
void CM_TickObjects60fps() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->TickObjects60fps();
    }
}

void CM_DrawObjects(s32 cameraId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->DrawObjects(cameraId);
    }
}

void CM_TickEditor() {
    gEditor.Tick();
}

void CM_DrawEditor() {
    gEditor.Draw();
}

void CM_TickParticles() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->TickParticles();
    }
}

void CM_DrawParticles(s32 cameraId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->DrawParticles(cameraId);
    }
}

void CM_InitClouds() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->InitClouds();
    }
}

void CM_TickClouds(s32 arg0, Camera* camera) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->TickClouds(arg0, camera);
    }
}

void CM_Waypoints(Player* player, int8_t playerId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->Waypoints(player, playerId);
    }
}

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
}

void CM_InitTrackObjects() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->InitTrackObjects();
    }
}

void CM_TickTrackObjects() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->TickTrackObjects();
    }
    TrainSmokeTick();
}

void CM_DrawTrackObjects(s32 cameraId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->DrawTrackObjects(cameraId);
    }

    TrainSmokeDraw(cameraId);
}

void CM_SomeSounds() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->SomeSounds();
    }
}

void CM_CreditsSpawnActors() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->CreditsSpawnActors();
    }
}

void CM_WhatDoesThisDo(Player* player, int8_t playerId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->WhatDoesThisDo(player, playerId);
    }
}

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->WhatDoesThisDoAI(player, playerId);
    }
}

void CM_SetStaffGhost() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->SetStaffGhost();
    }
}

// This should only be used for checking if the track has changed
uintptr_t CM_GetTrack() {
    return (uintptr_t) (void*) GetWorld()->GetTrack();
}

Properties* CM_GetProps() {
    if (GetWorld()->GetTrack()) {
        return &GetWorld()->GetTrack()->Props;
    }
    return NULL;
}

void CM_ScrollingTextures() {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->ScrollingTextures();
    }
}

void CM_DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection) {
    if (GetWorld()->GetTrack()) {
        GetWorld()->GetTrack()->DrawWater(screen, pathCounter, cameraRot, playerDirection);
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
        auto lakitu = std::make_unique<OLakitu>(i, OLakitu::LakituType::STARTER);
        GetWorld()->Lakitus[i] = lakitu.get();
        GetWorld()->AddObject(std::move(lakitu));
    }
}

// Checkered flag lakitu
void CM_ActivateFinishLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    GetWorld()->Lakitus[playerId]->Activate(OLakitu::LakituType::FINISH);
}

void CM_ActivateSecondLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    GetWorld()->Lakitus[playerId]->Activate(OLakitu::LakituType::SECOND_LAP);
}

void CM_ActivateFinalLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    GetWorld()->Lakitus[playerId]->Activate(OLakitu::LakituType::FINAL_LAP);
}

void CM_ActivateReverseLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    GetWorld()->Lakitus[playerId]->Activate(OLakitu::LakituType::REVERSE);
}

size_t GetCupCursorPosition() {
    return GetWorld()->GetCurrentCup()->CursorPosition;
}

void SetCupCursorPosition(size_t position) {
    GetWorld()->GetCurrentCup()->SetTrack(position);
    // GetWorld()->CurrentCup->CursorPosition = position;
}

size_t GetCupSize() {
    return GetWorld()->GetCurrentCup()->GetSize();
}

void* GetTrack(void) {
    return GetWorld()->GetTrack();
}

struct Actor* CM_GetActor(size_t index) {
    if (index < GetWorld()->Actors.size()) {
        AActor* actor = GetWorld()->Actors[index].get();
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
    auto& actors = GetWorld()->Actors;

    auto it = std::find_if(actors.begin(), actors.end(), [a](const std::unique_ptr<AActor>& ptr) {
        return ptr.get() == a;
    });

    if (it != actors.end()) {
        return std::distance(actors.begin(), it);
    }
    printf("FindActorIndex() actor not found\n");
    return -1;
}

void CM_DeleteActor(size_t index) {
    auto& actors = GetWorld()->Actors;
    if (index < actors.size()) {
        actors.erase(actors.begin() + index);
    }
}

/**
 * Clean up actors and other game objects.
 */
void CM_CleanWorld(void) {
    GetWorld()->CleanWorld();
}

void CM_CleanCameras(void) {
    for (auto& camera : GetWorld()->Cameras) {
        delete camera;
    }

    GetWorld()->Cameras.clear();
}

struct Actor* CM_AddBaseActor() {
    return (struct Actor*) GetWorld()->AddBaseActor();
}

void CM_ActorBeginPlay(struct Actor* actor) {
    GetWorld()->ActorBeginPlay(actor);
}

void CM_ActorGenerateCollision(struct Actor* actor) {
    AActor* act = GetWorld()->ConvertActorToAActor(actor);

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
    return GetWorld()->Actors.size();
}

void CM_ActorCollision(Player* player, Actor* actor) {
    AActor* a = GetWorld()->ConvertActorToAActor(actor);

    if (a->IsMod()) {
        a->Collision(player, a);
    }
}

f32 CM_GetWaterLevel(Vec3f pos, Collision* collision) {
    FVector fPos = {pos[0], pos[1], pos[2]};
    return GetWorld()->GetTrack()->GetWaterLevel(fPos, collision);
}

// clang-format off
bool IsMarioRaceway()     { return dynamic_cast<MarioRaceway*>(GetWorld()->GetTrack()) != nullptr; }
bool IsLuigiRaceway()     { return dynamic_cast<LuigiRaceway*>(GetWorld()->GetTrack()) != nullptr; }
bool IsChocoMountain()    { return dynamic_cast<ChocoMountain*>(GetWorld()->GetTrack()) != nullptr; }
bool IsBowsersCastle()    { return dynamic_cast<BowsersCastle*>(GetWorld()->GetTrack()) != nullptr; }
bool IsBansheeBoardwalk() { return dynamic_cast<BansheeBoardwalk*>(GetWorld()->GetTrack()) != nullptr; }
bool IsYoshiValley()      { return dynamic_cast<YoshiValley*>(GetWorld()->GetTrack()) != nullptr; }
bool IsFrappeSnowland()   { return dynamic_cast<FrappeSnowland*>(GetWorld()->GetTrack()) != nullptr; }
bool IsKoopaTroopaBeach() { return dynamic_cast<KoopaTroopaBeach*>(GetWorld()->GetTrack()) != nullptr; }
bool IsRoyalRaceway()     { return dynamic_cast<RoyalRaceway*>(GetWorld()->GetTrack()) != nullptr; }
bool IsMooMooFarm()       { return dynamic_cast<MooMooFarm*>(GetWorld()->GetTrack()) != nullptr; }
bool IsToadsTurnpike()    { return dynamic_cast<ToadsTurnpike*>(GetWorld()->GetTrack()) != nullptr; }
bool IsKalimariDesert()   { return dynamic_cast<KalimariDesert*>(GetWorld()->GetTrack()) != nullptr; }
bool IsSherbetLand()      { return dynamic_cast<SherbetLand*>(GetWorld()->GetTrack()) != nullptr; }
bool IsRainbowRoad()      { return dynamic_cast<RainbowRoad*>(GetWorld()->GetTrack()) != nullptr; }
bool IsWarioStadium()     { return dynamic_cast<WarioStadium*>(GetWorld()->GetTrack()) != nullptr; }
bool IsBlockFort()        { return dynamic_cast<BlockFort*>(GetWorld()->GetTrack()) != nullptr; }
bool IsSkyscraper()       { return dynamic_cast<Skyscraper*>(GetWorld()->GetTrack()) != nullptr; }
bool IsDoubleDeck()       { return dynamic_cast<DoubleDeck*>(GetWorld()->GetTrack()) != nullptr; }
bool IsDkJungle()         { return dynamic_cast<DKJungle*>(GetWorld()->GetTrack()) != nullptr; }
bool IsBigDonut()         { return dynamic_cast<BigDonut*>(GetWorld()->GetTrack()) != nullptr; }
bool IsPodiumCeremony()   { return dynamic_cast<PodiumCeremony*>(GetWorld()->GetTrack()) != nullptr; }

void SelectMarioRaceway()       { GetWorld()->SetCurrentTrack(std::make_unique<MarioRaceway>()); }
void SelectLuigiRaceway()       { GetWorld()->SetCurrentTrack(std::make_unique<LuigiRaceway>()); }
void SelectChocoMountain()      { GetWorld()->SetCurrentTrack(std::make_unique<ChocoMountain>()); }
void SelectBowsersCastle()      { GetWorld()->SetCurrentTrack(std::make_unique<BowsersCastle>()); }
void SelectBansheeBoardwalk()   { GetWorld()->SetCurrentTrack(std::make_unique<BansheeBoardwalk>()); }
void SelectYoshiValley()        { GetWorld()->SetCurrentTrack(std::make_unique<YoshiValley>()); }
void SelectFrappeSnowland()     { GetWorld()->SetCurrentTrack(std::make_unique<FrappeSnowland>()); }
void SelectKoopaTroopaBeach()   { GetWorld()->SetCurrentTrack(std::make_unique<KoopaTroopaBeach>()); }
void SelectRoyalRaceway()       { GetWorld()->SetCurrentTrack(std::make_unique<RoyalRaceway>()); }
void SelectMooMooFarm()         { GetWorld()->SetCurrentTrack(std::make_unique<MooMooFarm>()); }
void SelectToadsTurnpike()      { GetWorld()->SetCurrentTrack(std::make_unique<ToadsTurnpike>()); }
void SelectKalimariDesert()     { GetWorld()->SetCurrentTrack(std::make_unique<KalimariDesert>()); }
void SelectSherbetLand()        { GetWorld()->SetCurrentTrack(std::make_unique<SherbetLand>()); }
void SelectRainbowRoad()        { GetWorld()->SetCurrentTrack(std::make_unique<RainbowRoad>()); }
void SelectWarioStadium()       { GetWorld()->SetCurrentTrack(std::make_unique<WarioStadium>()); }
void SelectBlockFort()          { GetWorld()->SetCurrentTrack(std::make_unique<BlockFort>()); }
void SelectSkyscraper()         { GetWorld()->SetCurrentTrack(std::make_unique<Skyscraper>()); }
void SelectDoubleDeck()         { GetWorld()->SetCurrentTrack(std::make_unique<DoubleDeck>()); }
void SelectDkJungle()           { GetWorld()->SetCurrentTrack(std::make_unique<DKJungle>()); }
void SelectBigDonut()           { GetWorld()->SetCurrentTrack(std::make_unique<BigDonut>()); }
void SelectPodiumCeremony()     { GetWorld()->SetCurrentTrack(std::move(gPodiumCeremony)); }
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
