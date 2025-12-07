#include <libultraship.h>

#include "Game.h"
#include "port/Engine.h"

#include <graphic/Fast3D/Fast3dWindow.h>
#include "engine/World.h"
#include "engine/courses/Course.h"
#include "engine/courses/MarioRaceway.h"
#include "engine/courses/ChocoMountain.h"
#include "engine/courses/BowsersCastle.h"
#include "engine/courses/BansheeBoardwalk.h"
#include "engine/courses/YoshiValley.h"
#include "engine/courses/FrappeSnowland.h"
#include "engine/courses/KoopaTroopaBeach.h"
#include "engine/courses/RoyalRaceway.h"
#include "engine/courses/LuigiRaceway.h"
#include "engine/courses/MooMooFarm.h"
#include "engine/courses/ToadsTurnpike.h"
#include "engine/courses/KalimariDesert.h"
#include "engine/courses/SherbetLand.h"
#include "engine/courses/RainbowRoad.h"
#include "engine/courses/WarioStadium.h"
#include "engine/courses/BlockFort.h"
#include "engine/courses/Skyscraper.h"
#include "engine/courses/DoubleDeck.h"
#include "engine/courses/DKJungle.h"
#include "engine/courses/BigDonut.h"
#include "engine/courses/TestCourse.h"

#include "engine/courses/PodiumCeremony.h"

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
// #include "engine/wasm.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

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
    /* Add all courses to the global course list */
    std::shared_ptr<Course> mario         = gWorldInstance.AddCourse(std::make_shared<MarioRaceway>());
    std::shared_ptr<Course> choco         = gWorldInstance.AddCourse(std::make_shared<ChocoMountain>());
    std::shared_ptr<Course> bowser        = gWorldInstance.AddCourse(std::make_shared<BowsersCastle>());
    std::shared_ptr<Course> banshee       = gWorldInstance.AddCourse(std::make_shared<BansheeBoardwalk>());
    std::shared_ptr<Course> yoshi         = gWorldInstance.AddCourse(std::make_shared<YoshiValley>());
    std::shared_ptr<Course> frappe        = gWorldInstance.AddCourse(std::make_shared<FrappeSnowland>());
    std::shared_ptr<Course> koopa         = gWorldInstance.AddCourse(std::make_shared<KoopaTroopaBeach>());
    std::shared_ptr<Course> royal         = gWorldInstance.AddCourse(std::make_shared<RoyalRaceway>());
    std::shared_ptr<Course> luigi         = gWorldInstance.AddCourse(std::make_shared<LuigiRaceway>());
    std::shared_ptr<Course> mooMoo        = gWorldInstance.AddCourse(std::make_shared<MooMooFarm>());
    std::shared_ptr<Course> toads         = gWorldInstance.AddCourse(std::make_shared<ToadsTurnpike>());
    std::shared_ptr<Course> kalimari      = gWorldInstance.AddCourse(std::make_shared<KalimariDesert>());
    std::shared_ptr<Course> sherbet       = gWorldInstance.AddCourse(std::make_shared<SherbetLand>());
    std::shared_ptr<Course> rainbow       = gWorldInstance.AddCourse(std::make_shared<RainbowRoad>());
    std::shared_ptr<Course> wario         = gWorldInstance.AddCourse(std::make_shared<WarioStadium>());
    std::shared_ptr<Course> block         = gWorldInstance.AddCourse(std::make_shared<BlockFort>());
    std::shared_ptr<Course> skyscraper    = gWorldInstance.AddCourse(std::make_shared<Skyscraper>());
    std::shared_ptr<Course> doubleDeck    = gWorldInstance.AddCourse(std::make_shared<DoubleDeck>());
    std::shared_ptr<Course> dkJungle      = gWorldInstance.AddCourse(std::make_shared<DKJungle>());
    std::shared_ptr<Course> bigDonut      = gWorldInstance.AddCourse(std::make_shared<BigDonut>());
//    std::shared_ptr<Course> harbour       = gWorldInstance.AddCourse(std::make_shared<Harbour>());
    std::shared_ptr<Course> testCourse    = gWorldInstance.AddCourse(std::make_shared<TestCourse>());

    gPodiumCeremony = std::make_unique<PodiumCeremony>();

    // Construct cups with vectors of Course* (non-owning references)
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

// Set default course; mario raceway
void SetMarioRaceway(void) {
    SetCourseById(0);
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

void LoadCourse() {
    gWorldInstance.GetRaceManager().Load();
}

// Unload can be call to frequently so even when if the course wasn't loaded before
void UnLoadCourse() {
    gWorldInstance.GetRaceManager().UnLoad();
}

size_t GetCourseIndex() {
    return gWorldInstance.CourseIndex;
}

void SetCourse(const char* name) {
    gWorldInstance.SetCourse(name);
}

void NextCourse() {
    gWorldInstance.NextCourse();
}

void PreviousCourse() {
    gWorldInstance.PreviousCourse();
}

void SetCourseById(s32 course) {
    if (course < 0 || course >= gWorldInstance.Courses.size()) {
        return;
    }
    gWorldInstance.CourseIndex = course;
    gWorldInstance.SetCurrentCourse(gWorldInstance.Courses[gWorldInstance.CourseIndex]);
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
 * They do not use RenderCredits() and they do not use track sections.
 */
void CM_RenderCourse(struct UnkStruct_800DC5EC* screen) {
    if (nullptr == gWorldInstance.GetCurrentCourse()) {
        return;
    }

    // Custom tracks should never use RenderCredits();
    if (gWorldInstance.GetCurrentCourse()->IsMod()) {
        switch(screen->camera->renderMode) {
            default:
                gWorldInstance.GetCurrentCourse()->Render(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        } 
    } else {
        switch(screen->camera->renderMode) {
            case RENDER_FULL_SCENE:
                gWorldInstance.GetCurrentCourse()->RenderCredits();
                break;
            case RENDER_TRACK_SECTIONS:
                gWorldInstance.GetCurrentCourse()->Render(screen);
                break;
            case RENDER_COLLISION_MESH:
                render_collision();
                break;
        }
    }
}

void CM_TickActors() {
    if (gWorldInstance.GetCurrentCourse()) {
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
    auto course = gWorldInstance.GetCurrentCourse();
    
    if (nullptr == course) {
        return; 
    }

    if (tour) {
      //  gWorldInstance.Cameras[2]->SetActive(true);
       // D_800DC5EC->camera = gWorldInstance.Cameras[2]->Get();
        if (reinterpret_cast<TourCamera*>(gWorldInstance.Cameras[2])->IsTourComplete()) {
            tour = false;
            D_800DC5EC->pendingCamera = &cameras[0];
        }
    }

    if (gWorldInstance.GetCurrentCourse()) {
        // This line should likely be moved.
        // It's here so PreInit is after the scene file has been loaded
        // It used to be at the start of BeginPlay
        Editor::LoadLevel(gWorldInstance.GetCurrentCourse().get(), gWorldInstance.GetCurrentCourse()->SceneFilePtr);
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
        if (gWorldInstance.GetCurrentCourse()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (nullptr == gWorldInstance.GetCurrentCourse()) {
        // This is to prevent soft locking the game
        if (gWorldInstance.GetCurrentCourse()->bTourEnabled) {
            spawn_and_set_player_spawns();
        }
        return nullptr;
    }

    if (gWorldInstance.GetCurrentCourse()->TourShots.size() == 0) {
        // This is to prevent soft locking the game
        if (gWorldInstance.GetCurrentCourse()->bTourEnabled) {
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
    if (nullptr != gWorldInstance.GetCurrentCourse()) {
        if ((gWorldInstance.GetCurrentCourse()->bTourEnabled) && (gTourComplete == false)) {
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
        if (cam->Get() == D_800DC5EC->freeCamera) {
            if (state) {
                D_800DC5EC->pendingCamera = D_800DC5EC->freeCamera;
                cam->SetActive(true);
            } else {
                if (nullptr != D_800DC5EC->raceCamera) {
                    if (gGamestate == RACING) {
                        D_800DC5EC->pendingCamera = D_800DC5EC->raceCamera;
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
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.TickObjects();
    }
}

// A couple objects such as lakitu are ticked inside of process_game_tick which support 60fps.
// This is a fallback to support that.
void CM_TickObjects60fps() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.TickObjects60fps();
    }
}

void CM_DrawObjects(s32 cameraId) {
    if (gWorldInstance.GetCurrentCourse()) {
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
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.TickParticles();
    }
}

void CM_DrawParticles(s32 cameraId) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.DrawParticles(cameraId);
    }
}

void CM_InitClouds() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->InitClouds();
    }
}

void CM_UpdateClouds(s32 arg0, Camera* camera) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->UpdateClouds(arg0, camera);
    }
}

void CM_Waypoints(Player* player, int8_t playerId) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->Waypoints(player, playerId);
    }
}

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
}

void CM_InitCourseObjects() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->InitCourseObjects();
    }
}

void CM_UpdateCourseObjects() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->UpdateCourseObjects();
    }
    TrainSmokeTick();
}

void CM_RenderCourseObjects(s32 cameraId) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->RenderCourseObjects(cameraId);
    }

    TrainSmokeDraw(cameraId);
}

void CM_SomeSounds() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->SomeSounds();
    }
}

void CM_CreditsSpawnActors() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->CreditsSpawnActors();
    }
}

void CM_WhatDoesThisDo(Player* player, int8_t playerId) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->WhatDoesThisDo(player, playerId);
    }
}

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->WhatDoesThisDoAI(player, playerId);
    }
}

void CM_SetStaffGhost() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->SetStaffGhost();
    }
}

Properties* CM_GetProps() {
    if (gWorldInstance.GetCurrentCourse()) {
        return &gWorldInstance.GetCurrentCourse()->Props;
    }
    return NULL;
}

Properties* CM_GetPropsCourseId(s32 courseId) {
    return &gWorldInstance.Courses[courseId]->Props;
}

void CM_ScrollingTextures() {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->ScrollingTextures();
    }
}

void CM_DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection) {
    if (gWorldInstance.GetCurrentCourse()) {
        gWorldInstance.GetCurrentCourse()->DrawWater(screen, pathCounter, cameraRot, playerDirection);
    }
}

/**
 * This should only be ran once per course, otherwise animation/timings might become sped up.
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
    gWorldInstance.GetCurrentCup()->SetCourse(position);
    // gWorldInstance.CurrentCup->CursorPosition = position;
}

size_t GetCupSize() {
    return gWorldInstance.GetCurrentCup()->GetSize();
}

void SetCourseFromCup() {
    gWorldInstance.SetCurrentCourse(gWorldInstance.GetCurrentCup()->GetCourse());
}

void* GetCourse(void) {
    return gWorldInstance.GetCurrentCourse().get();
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
    return gWorldInstance.GetCurrentCourse()->GetWaterLevel(fPos, collision);
}

// clang-format off
bool IsMarioRaceway()     { return dynamic_cast<MarioRaceway*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsLuigiRaceway()     { return dynamic_cast<LuigiRaceway*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsChocoMountain()    { return dynamic_cast<ChocoMountain*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsBowsersCastle()    { return dynamic_cast<BowsersCastle*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsBansheeBoardwalk() { return dynamic_cast<BansheeBoardwalk*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsYoshiValley()      { return dynamic_cast<YoshiValley*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsFrappeSnowland()   { return dynamic_cast<FrappeSnowland*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsKoopaTroopaBeach() { return dynamic_cast<KoopaTroopaBeach*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsRoyalRaceway()     { return dynamic_cast<RoyalRaceway*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsMooMooFarm()       { return dynamic_cast<MooMooFarm*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsToadsTurnpike()    { return dynamic_cast<ToadsTurnpike*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsKalimariDesert()   { return dynamic_cast<KalimariDesert*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsSherbetLand()      { return dynamic_cast<SherbetLand*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsRainbowRoad()      { return dynamic_cast<RainbowRoad*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsWarioStadium()     { return dynamic_cast<WarioStadium*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsBlockFort()        { return dynamic_cast<BlockFort*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsSkyscraper()       { return dynamic_cast<Skyscraper*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsDoubleDeck()       { return dynamic_cast<DoubleDeck*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsDkJungle()         { return dynamic_cast<DKJungle*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsBigDonut()         { return dynamic_cast<BigDonut*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }
bool IsPodiumCeremony()   { return dynamic_cast<PodiumCeremony*>(gWorldInstance.GetCurrentCourse().get()) != nullptr; }

void SelectMarioRaceway()       { gWorldInstance.SetCourseByType<MarioRaceway>(); }
void SelectLuigiRaceway()       { gWorldInstance.SetCourseByType<LuigiRaceway>(); }
void SelectChocoMountain()      { gWorldInstance.SetCourseByType<ChocoMountain>(); }
void SelectBowsersCastle()      { gWorldInstance.SetCourseByType<BowsersCastle>(); }
void SelectBansheeBoardwalk()   { gWorldInstance.SetCourseByType<BansheeBoardwalk>(); }
void SelectYoshiValley()        { gWorldInstance.SetCourseByType<YoshiValley>(); }
void SelectFrappeSnowland()     { gWorldInstance.SetCourseByType<FrappeSnowland>(); }
void SelectKoopaTroopaBeach()   { gWorldInstance.SetCourseByType<KoopaTroopaBeach>(); }
void SelectRoyalRaceway()       { gWorldInstance.SetCourseByType<RoyalRaceway>(); }
void SelectMooMooFarm()         { gWorldInstance.SetCourseByType<MooMooFarm>(); }
void SelectToadsTurnpike()      { gWorldInstance.SetCourseByType<ToadsTurnpike>(); }
void SelectKalimariDesert()     { gWorldInstance.SetCourseByType<KalimariDesert>(); }
void SelectSherbetLand()        { gWorldInstance.SetCourseByType<SherbetLand>(); }
void SelectRainbowRoad()        { gWorldInstance.SetCourseByType<RainbowRoad>(); }
void SelectWarioStadium()       { gWorldInstance.SetCourseByType<WarioStadium>(); }
void SelectBlockFort()          { gWorldInstance.SetCourseByType<BlockFort>(); }
void SelectSkyscraper()         { gWorldInstance.SetCourseByType<Skyscraper>(); }
void SelectDoubleDeck()         { gWorldInstance.SetCourseByType<DoubleDeck>(); }
void SelectDkJungle()           { gWorldInstance.SetCourseByType<DKJungle>(); }
void SelectBigDonut()           { gWorldInstance.SetCourseByType<BigDonut>(); }
void SelectPodiumCeremony()     { gWorldInstance.SetCurrentCourse(gPodiumCeremony); }
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
    // Timer_Update();
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
