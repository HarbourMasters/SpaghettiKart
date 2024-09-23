#include <libultraship.h>

#include <Fast3D/gfx_pc.h>
#include "Engine.h"
#include "engine/World.h"

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
    Cup* mushroom = gWorldInstance.AddCup();
    mushroom->Name = "mushroom cup";
    //mushroom->SetCourses();

    Cup* flower = gWorldInstance.AddCup();
    flower->Name = "flower cup";

    Cup* star = gWorldInstance.AddCup();
    star->Name = "star cup";

    Cup* special = gWorldInstance.AddCup();
    special->Name = "special cup";

    Cup* battle = gWorldInstance.AddCup();
    battle->Name = "battle";
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
