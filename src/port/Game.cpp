#include <libultraship.h>

#include "Game.h"

#include <Fast3D/gfx_pc.h>
#include "Engine.h"
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
#include "engine/actors/AFinishline.h"

#include "engine/courses/PodiumCeremony.h"

#include "engine/ModelLoader.h"
#include "engine/actors/BowserStatue.h"

#include "engine/GarbageCollector.h"

#include "engine/TrainCrossing.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Lakitu.h"

#include "Smoke.h"

extern "C" {
#include "main.h"
#include "audio/load.h"
#include "audio/external.h"
#include "networking/networking.h"
#include "render_courses.h"
//#include "engine/wasm.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

// Create the world instance
World gWorldInstance;

MarioRaceway* gMarioRaceway;
ChocoMountain* gChocoMountain;
BowsersCastle* gBowsersCastle;
BansheeBoardwalk* gBansheeBoardwalk;
YoshiValley* gYoshiValley;
FrappeSnowland* gFrappeSnowland;
KoopaTroopaBeach* gKoopaTroopaBeach;
RoyalRaceway* gRoyalRaceway;
LuigiRaceway* gLuigiRaceway;
MooMooFarm* gMooMooFarm;
ToadsTurnpike* gToadsTurnpike;
KalimariDesert* gKalimariDesert;
SherbetLand* gSherbetLand;
RainbowRoad* gRainbowRoad;
WarioStadium* gWarioStadium;
BlockFort* gBlockFort;
Skyscraper* gSkyscraper;
DoubleDeck* gDoubleDeck;
DKJungle* gDkJungle;
BigDonut* gBigDonut;
PodiumCeremony* gPodiumCeremony;
TestCourse* gTestCourse;

Cup* gMushroomCup;
Cup* gFlowerCup;
Cup* gStarCup;
Cup* gSpecialCup;
Cup* gBattleCup;

ModelLoader gModelLoader;

void CustomEngineInit() {

    gMarioRaceway = new MarioRaceway();
    gChocoMountain = new ChocoMountain();
    gBowsersCastle = new BowsersCastle();
    gBansheeBoardwalk = new BansheeBoardwalk();
    gYoshiValley = new YoshiValley();
    gFrappeSnowland = new FrappeSnowland();
    gKoopaTroopaBeach = new KoopaTroopaBeach();
    gRoyalRaceway = new RoyalRaceway();
    gLuigiRaceway = new LuigiRaceway();
    gMooMooFarm = new MooMooFarm();
    gToadsTurnpike = new ToadsTurnpike();
    gKalimariDesert = new KalimariDesert();
    gSherbetLand = new SherbetLand();
    gRainbowRoad = new RainbowRoad();
    gWarioStadium = new WarioStadium();
    gBlockFort = new BlockFort();
    gSkyscraper = new Skyscraper();
    gDoubleDeck = new DoubleDeck();
    gDkJungle = new DKJungle();
    gBigDonut = new BigDonut();
    gPodiumCeremony = new PodiumCeremony();
    gTestCourse = new TestCourse();

    /* Add all courses to the global course list */
    gWorldInstance.AddCourse(gMarioRaceway);
    gWorldInstance.AddCourse(gChocoMountain);
    gWorldInstance.AddCourse(gBowsersCastle);
    gWorldInstance.AddCourse(gBansheeBoardwalk);
    gWorldInstance.AddCourse(gYoshiValley);
    gWorldInstance.AddCourse(gFrappeSnowland);
    gWorldInstance.AddCourse(gKoopaTroopaBeach);
    gWorldInstance.AddCourse(gRoyalRaceway);
    gWorldInstance.AddCourse(gLuigiRaceway);
    gWorldInstance.AddCourse(gMooMooFarm);
    gWorldInstance.AddCourse(gToadsTurnpike);
    gWorldInstance.AddCourse(gKalimariDesert);
    gWorldInstance.AddCourse(gSherbetLand);
    gWorldInstance.AddCourse(gRainbowRoad);
    gWorldInstance.AddCourse(gWarioStadium);
    gWorldInstance.AddCourse(gBlockFort);
    gWorldInstance.AddCourse(gSkyscraper);
    gWorldInstance.AddCourse(gDoubleDeck);
    gWorldInstance.AddCourse(gDkJungle);
    gWorldInstance.AddCourse(gBigDonut);
    gWorldInstance.AddCourse(gTestCourse);

    gMushroomCup = new Cup("mk:mushroom_cup", "mushroom cup", std::vector<Course*>{ gLuigiRaceway, gMooMooFarm, gKoopaTroopaBeach, gKalimariDesert });
    gFlowerCup = new Cup("mk:flower_cup", "flower cup", std::vector<Course*>{ gToadsTurnpike, gFrappeSnowland, gChocoMountain, gMarioRaceway });
    gStarCup = new Cup("mk:star_cup", "star cup", std::vector<Course*>{ gWarioStadium, gSherbetLand, gRoyalRaceway, gBowsersCastle });
    gSpecialCup = new Cup("mk:special_cup", "special cup", std::vector<Course*>{ gDkJungle, gYoshiValley, gBansheeBoardwalk, gRainbowRoad });
    gBattleCup = new Cup("mk:battle_cup", "battle", std::vector<Course*>{ gBigDonut, gBlockFort, gDoubleDeck, gSkyscraper });

    /* Instantiate Cups */
    gWorldInstance.AddCup(gMushroomCup);
    gWorldInstance.AddCup(gFlowerCup);
    gWorldInstance.AddCup(gStarCup);
    gWorldInstance.AddCup(gSpecialCup);
    gWorldInstance.AddCup(gBattleCup);

    /* Set default course; mario raceway */
    gWorldInstance.CurrentCourse = gMarioRaceway;
    gWorldInstance.CurrentCup = gMushroomCup;
    gWorldInstance.CurrentCup->CursorPosition = 3;
    gWorldInstance.CupIndex = 0;

    ModelLoader::LoadModelList bowserStatueList = {
        .course = gBowsersCastle,
        .gfxBuffer = &gBowserStatueGfx[0],
        .gfxBufferSize = 162,
        .gfxStart = (0x2BB8 / 8), // 0x2BB8 / sizeof(OldGfx)
        .vtxBuffer = &gBowserStatueVtx[0],
        .vtxBufferSize = 717,
        .vtxStart = 1942,
    };

    gModelLoader.Add(bowserStatueList);

    gModelLoader.Load();
}

extern "C" {

    World* GetWorld(void) {
        return &gWorldInstance;
    }

    u32 WorldNextCup(void) {
        return gWorldInstance.NextCup();
    }

    u32 WorldPreviousCup(void) {
        return gWorldInstance.PreviousCup();
    }

    void CourseManager_SetCup(void* cup) {
        gWorldInstance.SetCup((Cup*)cup);
    }

    void* GetCup() {
        return gWorldInstance.CurrentCup;
    }

    u32 GetCupIndex(void) {
        return gWorldInstance.GetCupIndex();
    }

    const char* GetCupName(void) {
        return gWorldInstance.CurrentCup->Name;
    }

    void LoadCourse() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->Load();
        }
    }

    CProperties* GetCoursePropsA() {
        return (CProperties*) gWorldInstance.GetCourseProps();
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
        gWorldInstance.CurrentCourse = gWorldInstance.Courses[gWorldInstance.CourseIndex];
    }

    void CourseManager_SpawnVehicles() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SpawnVehicles();
        }
    }

    void CourseManager_VehiclesSpawn() {
        for (auto& vehicle : gWorldInstance.Vehicles) {
            if (vehicle) {
                vehicle->Spawn();
            }
        }
    }

    void CourseManager_VehiclesTick() {
        for (auto& vehicle : gWorldInstance.Vehicles) {
            if (vehicle) {
                vehicle->Tick();
            }
        }
    }

    void CourseManager_VehiclesCollision(s32 playerId, Player* player) {
        for (auto& vehicle : gWorldInstance.Vehicles) {
            if (vehicle) {
                vehicle->Collision(playerId, player);
            }
        }
    }

    void CourseManager_SpawnBombKarts() {
        for (auto& kart : gWorldInstance.BombKarts) {
            if (kart) {
                kart->Spawn();
            }
        }
    }

    void CourseManager_TickBombKarts() {
        for (auto& kart : gWorldInstance.BombKarts) {
            if (kart) {
                kart->Tick();
            }
        }
    }

    void CourseManager_DrawBombKarts(s32 cameraId) {
        for (auto& kart : gWorldInstance.BombKarts) {
            if (kart) {
                kart->Draw(cameraId);
            }
        }
    }

    void CourseManager_DrawBattleBombKarts(s32 cameraId) {
        for (auto& kart : gWorldInstance.BombKarts) {
            if (kart) {
                kart->DrawBattle(cameraId);
            }
        }
    }

    void CourseManager_BombKartsWaypoint(s32 cameraId) {
        for (auto& kart : gWorldInstance.BombKarts) {
            if (kart) {
                kart->Waypoint(cameraId);
            }
        }
    }

    void CourseManager_DrawVehicles(s32 playerId) {
        for (auto& vehicle : gWorldInstance.Vehicles) {
            if (vehicle) {
                vehicle->Draw(playerId);
            }
        }
    }

    void CourseManager_ClearVehicles(void) {
        gWorldInstance.ClearVehicles();
        gWorldInstance.Crossings.clear();
        gWorldInstance.BombKarts.clear();
    }

    void CourseManager_CrossingTrigger() {
        for (auto& crossing : gWorldInstance.Crossings) {
            if (crossing) {
                crossing->CrossingTrigger();
            }
        }
    }

    void CourseManager_AICrossingBehaviour(s32 playerId) {
        for (auto& crossing : gWorldInstance.Crossings) {
            if (crossing) {
                crossing->AICrossingBehaviour(playerId);
            }
        }
    }

    s32 CourseManager_GetCrossingOnTriggered(uintptr_t* crossing) {
        TrainCrossing* ptr = (TrainCrossing*) crossing;
        if (ptr) {
            return ptr->OnTriggered;
        }
    }

    void CourseManager_TrainSmokeTick(void) {
        TrainSmokeTick();
    }

    void CourseManager_TrainSmokeDraw(s32 cameraId) {
        TrainSmokeDraw(cameraId);
    }

    void CourseManager_LoadTextures() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->LoadTextures();
        }
    }

    void CourseManager_RenderCourse(struct UnkStruct_800DC5EC* arg0) {
        if (gWorldInstance.CurrentCourse->IsMod() == false) {
            if ((CVarGetInteger("gFreecam", 0) == true)) {
                // Render credits courses
                func_8029569C();
                return;
            }
        }

        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->Render(arg0);
        }
    }

    void CourseManager_RenderCredits() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->RenderCredits();
        }
    }

    void CourseManager_TickActors() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.TickActors();
        }
    }

    void CourseManager_DrawActors(Camera* camera, struct Actor* actor) {
        AActor* a = gWorldInstance.ConvertActorToAActor(actor);
        if (a->IsMod()) {
            a->Draw(camera);
        }
    }

    void CourseManager_SpawnActors() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SpawnActors();
        }
    }

    void CourseManager_TickObjects() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.TickObjects();
        }
    }

    // A couple objects such as lakitu are ticked inside of process_game_tick which support 60fps.
    // This is a fallback to support that.
    void CourseManager_TickObjects60fps() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.TickObjects60fps();
        }
    }

    void CourseManager_DrawObjects(s32 cameraId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.DrawObjects(cameraId);
        }
    }

    void CM_TickParticles() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.TickParticles();
        }
    }

    void CM_DrawParticles(s32 cameraId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.DrawParticles(cameraId);
        }
    }

    // Helps prevents users from forgetting to add a finishline to their course
    bool cm_DoesFinishlineExist() {
        for (AActor* actor : gWorldInstance.Actors) {
            if (dynamic_cast<AFinishline*>(actor))  {
                return true;
            }
        }
        return false;
    }

    void CourseManager_InitClouds() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->InitClouds();
        }
    }

    void CourseManager_UpdateClouds(s32 arg0, Camera* camera) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->UpdateClouds(arg0, camera);
        }
    }

    void CourseManager_Waypoints(Player* player, int8_t playerId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->Waypoints(player, playerId);
        }
    }

    void CourseManager_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5,
                                        f32* arg6, f32* arg7) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }

    void CourseManager_MinimapSettings() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->MinimapSettings();
        }
    }

    void CourseManager_InitCourseObjects() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->InitCourseObjects();
        }
    }

    void CourseManager_UpdateCourseObjects() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->UpdateCourseObjects();
        }
    }

    void CourseManager_RenderCourseObjects(s32 cameraId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->RenderCourseObjects(cameraId);
        }
    }

    void CourseManager_SomeSounds() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SomeSounds();
        }
    }

    void CourseManager_CreditsSpawnActors() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->CreditsSpawnActors();
        }
    }

    void CourseManager_WhatDoesThisDo(Player* player, int8_t playerId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->WhatDoesThisDo(player, playerId);
        }
    }

    void CourseManager_WhatDoesThisDoAI(Player* player, int8_t playerId) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->WhatDoesThisDoAI(player, playerId);
        }
    }

    void CourseManager_MinimapFinishlinePosition() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->MinimapFinishlinePosition();
        }
    }

    void CourseManager_SetStaffGhost() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SetStaffGhost();
        }
    }

    CProperties* CourseManager_GetProps() {
        if (gWorldInstance.CurrentCourse) {
            return (CProperties*) &gWorldInstance.CurrentCourse->Props;
        }
    }

    void CourseManager_ScrollingTextures() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->ScrollingTextures();
        }
    }

    void CourseManager_DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->DrawWater(screen, pathCounter, cameraRot, playerDirection);
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
        return gWorldInstance.CurrentCup->CursorPosition;
    }

    void SetCupCursorPosition(size_t position) {
        gWorldInstance.CurrentCup->SetCourse(position);
        //gWorldInstance.CurrentCup->CursorPosition = position;
    }

    size_t GetCupSize() {
        return gWorldInstance.CurrentCup->GetSize();
    }

    void SetCourseFromCup() {
        gWorldInstance.CurrentCourse = gWorldInstance.CurrentCup->GetCourse();
    }

    void* GetCourse(void) {
        return gWorldInstance.CurrentCourse;
    }

    void SetCourseByClass(void* course) {
        gWorldInstance.CurrentCourse = (Course*) course;
    }

    struct Actor* m_GetActor(size_t index) {
        if (index < gWorldInstance.Actors.size()) {
            AActor* actor = gWorldInstance.Actors[index];
            return reinterpret_cast<struct Actor*>(reinterpret_cast<char*>(actor) + sizeof(void*));
        } else {
            //throw std::runtime_error("GetActor() index out of bounds");
            return NULL;
        }
    }

    size_t m_FindActorIndex(Actor* actor) {
        // Move the ptr back to look at the vtable.
        // This gets us the proper C++ class instead of just the variables used in C.
        AActor* a = reinterpret_cast<AActor*>((char*)actor - sizeof(void*));
        auto actors = gWorldInstance.Actors;

        auto it = std::find(actors.begin(), actors.end(), static_cast<AActor*>(a));
        if (it != actors.end()) {
            return std::distance(actors.begin(), it);
        }
        printf("FindActorIndex() actor not found\n");
        return 0;
    }

    void m_DeleteActor(size_t index) {
        std::vector<AActor*> actors = gWorldInstance.Actors;
        if (index < actors.size()) {
            actors.erase(actors.begin() + index);
        }
    }
    
    /**
     * Clean up actors and other game objects.
     */
    void CM_CleanWorld(void) {
        gWorldInstance.Actors.clear();
        gWorldInstance.Objects.clear();
        gWorldInstance.Emitters.clear();
        gWorldInstance.Lakitus.clear();
    }

    struct Actor* m_AddBaseActor(void) {
        return (struct Actor*) gWorldInstance.AddBaseActor();
    }

    size_t m_GetActorSize() {
        return gWorldInstance.Actors.size();
    }

    void m_ActorCollision(Player* player, Actor* actor) {
        AActor* a = gWorldInstance.ConvertActorToAActor(actor);

        if (a->IsMod()) {
            a->Collision(player, a);
        }
    }

    void* GetMarioRaceway(void) {
        return gMarioRaceway;
    }

    void* GetLuigiRaceway(void) {
        return gLuigiRaceway;
    }

    void* GetChocoMountain(void) {
        return gChocoMountain;
    }

    void* GetBowsersCastle(void) {
        return gBowsersCastle;
    }

    void* GetBansheeBoardwalk(void) {
        return gBansheeBoardwalk;
    }

    void* GetYoshiValley(void) {
        return gYoshiValley;
    }

    void* GetFrappeSnowland(void) {
        return gFrappeSnowland;
    }

    void* GetKoopaTroopaBeach(void) {
        return gKoopaTroopaBeach;
    }

    void* GetRoyalRaceway(void) {
        return gRoyalRaceway;
    }

    void* GetMooMooFarm(void) {
        return gMooMooFarm;
    }

    void* GetToadsTurnpike(void) {
        return gToadsTurnpike;
    }

    void* GetKalimariDesert(void) {
        return gKalimariDesert;
    }

    void* GetSherbetLand(void) {
        return gSherbetLand;
    }

    void* GetRainbowRoad(void) {
        return gRainbowRoad;
    }

    void* GetWarioStadium(void) {
        return gWarioStadium;
    }

    void* GetBlockFort(void) {
        return gBlockFort;
    }

    void* GetSkyscraper(void) {
        return gSkyscraper;
    }

    void* GetDoubleDeck(void) {
        return gDoubleDeck;
    }

    void* GetDkJungle(void) {
        return gDkJungle;
    }

    void* GetBigDonut(void) {
        return gBigDonut;
    }

    void* GetPodiumCeremony(void) {
        return gPodiumCeremony;
    }

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
int main(int argc, char* argv[]) {
#endif
     //load_wasm();
    GameEngine::Create();
    audio_init();
    sound_init();

    CustomEngineInit();
    thread5_game_loop();
    while (WindowIsRunning()) {
        push_frame();
    }
    // GameEngine::Instance->ProcessFrame(push_frame);
    GameEngine::Instance->Destroy();
    return 0;
}
