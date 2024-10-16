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

#include "engine/courses/PodiumCeremony.h"

#include "engine/TrainCrossing.h"

#include "Smoke.h"

extern "C" {
#include "main.h"
#include "audio/load.h"
#include "audio/external.h"
#include "networking/networking.h"
#include "engine/wasm.h"
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
    gWorldInstance.CurrentCup = gFlowerCup;
    gWorldInstance.CurrentCup->CursorPosition = 3;
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
        printf("Cup Index: %d\n", gWorldInstance.GetCupIndex());
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

    void CourseManager_RenderTrucks(s32 playerId) {
        for (auto& vehicle : gWorldInstance.Vehicles) {
            if (vehicle) {
                vehicle->Draw(playerId);
            }
        }
    }

    void CourseManager_ResetVehicles(void) {
        gWorldInstance.ResetVehicles();
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

    void CourseManager_DrawActors(Camera* camera) {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.DrawActors(camera);
        }
    }

    void CourseManager_SpawnActors() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SpawnActors();
        }
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

    void CourseManager_GenerateCollision() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->GenerateCollision();
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

    void CourseManager_SetCourseVtxColours() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SetCourseVtxColours();
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

    void CourseManager_SpawnBombKarts() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->SpawnBombKarts();
        }
    }

    void CourseManager_Water() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->Water();
        }
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
}

void push_frame() {
    // GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    thread5_iteration();
    // thread5_game_loop();
    // Graphics_ThreadUpdate();w
    // Timer_Update();
    // GameEngine::EndAudioFrame();
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
    load_wasm();
    GameEngine::Create();
    // audio_init();
    // sound_init();

    CustomEngineInit();
    thread5_game_loop();
    while (WindowIsRunning()) {
        push_frame();
    }
    // GameEngine::Instance->ProcessFrame(push_frame);
    GameEngine::Instance->Destroy();
    return 0;
}
