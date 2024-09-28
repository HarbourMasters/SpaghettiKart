#include <libultraship.h>

#include <Fast3D/gfx_pc.h>
#include "Engine.h"
#include "engine/World.h"
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

#include "engine/courses/PodiumCeremony.h"

extern "C" {
#include "main.h"
#include "audio/load.h"
#include "audio/external.h"
#include "networking/networking.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

// Create the world instance
World gWorldInstance;

void CustomEngineInit() {
    /* Instantiate Courses */
    auto marioRaceway = std::make_shared<CourseMarioRaceway>();
    auto chocoMountain = std::make_shared<ChocoMountain>();
    auto bowsersCastle = std::make_shared<BowsersCastle>();
    auto bansheeBoardwalk = std::make_shared<BansheeBoardwalk>();
    auto yoshiValley = std::make_shared<YoshiValley>();
    auto frappeSnowland = std::make_shared<FrappeSnowland>();
    auto koopaTroopaBeach = std::make_shared<KoopaTroopaBeach>();
    auto royalRaceway = std::make_shared<RoyalRaceway>();
    auto luigiRaceway = std::make_shared<LuigiRaceway>();
    auto mooMooFarm = std::make_shared<MooMooFarm>();
    auto toadsTurnpike = std::make_shared<ToadsTurnpike>();
    auto kalimariDesert = std::make_shared<KalimariDesert>();
    auto sherbetLand = std::make_shared<SherbetLand>();
    auto rainbowRoad = std::make_shared<RainbowRoad>();
    auto warioStadium = std::make_shared<WarioStadium>();
    auto blockFort = std::make_shared<BlockFort>();
    auto skyscraper = std::make_shared<Skyscraper>();
    auto doubleDeck = std::make_shared<DoubleDeck>();
    auto dkJungle = std::make_shared<DKJungle>();
    auto bigDonut = std::make_shared<BigDonut>();

    /* Add all courses to the global course list */
    gWorldInstance.Courses.push_back(marioRaceway);
    gWorldInstance.Courses.push_back(chocoMountain);
    gWorldInstance.Courses.push_back(bowsersCastle);
    gWorldInstance.Courses.push_back(bansheeBoardwalk);
    gWorldInstance.Courses.push_back(yoshiValley);
    gWorldInstance.Courses.push_back(frappeSnowland);
    gWorldInstance.Courses.push_back(koopaTroopaBeach);
    gWorldInstance.Courses.push_back(royalRaceway);
    gWorldInstance.Courses.push_back(luigiRaceway);
    gWorldInstance.Courses.push_back(mooMooFarm);
    gWorldInstance.Courses.push_back(toadsTurnpike);
    gWorldInstance.Courses.push_back(kalimariDesert);
    gWorldInstance.Courses.push_back(sherbetLand);
    gWorldInstance.Courses.push_back(rainbowRoad);
    gWorldInstance.Courses.push_back(warioStadium);
    gWorldInstance.Courses.push_back(blockFort);
    gWorldInstance.Courses.push_back(skyscraper);
    gWorldInstance.Courses.push_back(doubleDeck);
    gWorldInstance.Courses.push_back(dkJungle);
    gWorldInstance.Courses.push_back(bigDonut);

    /* Instantiate Cups */
    Cup* mushroom = gWorldInstance.AddCup("mushroom cup", std::vector<std::shared_ptr<Course>>{
            luigiRaceway, mooMooFarm, koopaTroopaBeach, kalimariDesert});
    Cup* flower = gWorldInstance.AddCup("flower cup", std::vector<std::shared_ptr<Course>>{
            toadsTurnpike, frappeSnowland, chocoMountain, marioRaceway});
    Cup* star = gWorldInstance.AddCup("star cup", std::vector<std::shared_ptr<Course>>{
            warioStadium, sherbetLand, royalRaceway, bowsersCastle});
    Cup* special = gWorldInstance.AddCup("special cup", std::vector<std::shared_ptr<Course>>{
            dkJungle, yoshiValley, bansheeBoardwalk, rainbowRoad});
    Cup* battle = gWorldInstance.AddCup("battle", std::vector<std::shared_ptr<Course>>{
            bigDonut, blockFort, doubleDeck, skyscraper});


}

extern "C" {
    u32 WorldNextCup(void) {
        return gWorldInstance.NextCup();
    }

    u32 WorldPreviousCup(void) {
        return gWorldInstance.PreviousCup();
    }

    void SetCupIndex(int16_t courseId) {
        gWorldInstance.SetCupIndex(courseId);
    }

    u32 GetCupIndex(void) {
        printf("Cup Index: %d\n", gWorldInstance.GetCupIndex());
        return gWorldInstance.GetCupIndex();
    }

    const char* GetCupName(void) {
        return gWorldInstance.Cups[gWorldInstance.CupIndex]->Name;
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

    void CourseManager_GenerateCollision() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->GenerateCollision();
        }
    }

    void CourseManager_SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
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

    void CourseManager_RenderCourseObjects() {
        if (gWorldInstance.CurrentCourse) {
            gWorldInstance.CurrentCourse->RenderCourseObjects();
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

    CProperties *CourseManager_GetProps() {
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
