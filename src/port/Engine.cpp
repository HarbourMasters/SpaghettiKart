#include "Engine.h"

#include "StringHelper.h"
#include "ui/ImguiUI.h"
#include "libultraship/src/Context.h"
#include "resource/type/ResourceType.h"
#include "resource/importers/GenericArrayFactory.h"
#include "resource/importers/Vec3fFactory.h"
#include "resource/importers/Vec3sFactory.h"
#include "resource/importers/KartAIFactory.h"
#include "resource/importers/CourseVtxFactory.h"
#include "resource/importers/TrackSectionsFactory.h"
#include "resource/importers/TrackWaypointFactory.h"
#include "resource/importers/ActorSpawnDataFactory.h"
#include "resource/importers/UnkActorSpawnDataFactory.h"
#include "resource/importers/ArrayFactory.h"
#include <Fast3D/Fast3dWindow.h>

#include <Fast3D/gfx_pc.h>
#include <Fast3D/gfx_rendering_api.h>
#include <SDL2/SDL.h>

#include <utility>

extern "C" {
float gInterpolationStep = 0.0f;
#include <macros.h>
#include <DisplayListFactory.h>
#include <TextureFactory.h>
#include <MatrixFactory.h>
#include <BlobFactory.h>
#include <VertexFactory.h>
#include <LightFactory.h>
}

GameEngine* GameEngine::Instance;

GameEngine::GameEngine() {
    std::vector<std::string> OTRFiles;
    if (const std::string spaghetti_path = Ship::Context::GetPathRelativeToAppDirectory("spaghetti.otr");
        std::filesystem::exists(spaghetti_path)) {
        OTRFiles.push_back(spaghetti_path);
    }
    if (const std::string ship_otr_path = Ship::Context::GetPathRelativeToAppBundle("ship.otr");
        std::filesystem::exists(ship_otr_path)) {
        OTRFiles.push_back(ship_otr_path);
    }
    if (const std::string patches_path = Ship::Context::GetPathRelativeToAppDirectory("mods");
        !patches_path.empty() && std::filesystem::exists(patches_path)) {
        if (std::filesystem::is_directory(patches_path)) {
            for (const auto& p : std::filesystem::recursive_directory_iterator(patches_path)) {
                if (StringHelper::IEquals(p.path().extension().string(), ".otr")) {
                    OTRFiles.push_back(p.path().generic_string());
                }
            }
        }
    }

    this->context = Ship::Context::CreateInstance("Spaghettify", "skart64", "spaghettify.cfg.json", OTRFiles, {}, 3);

    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());

    wnd->SetRendererUCode(ucode_f3dex);
    this->context->InitGfxDebugger();

    auto loader = context->GetResourceManager()->GetResourceLoader();
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3fV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vec3f", static_cast<uint32_t>(SF64::ResourceType::Vec3f), 0);
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3sV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vec3s", static_cast<uint32_t>(SF64::ResourceType::Vec3s), 0);
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryGenericArrayV0>(),
                                    RESOURCE_FORMAT_BINARY, "GenericArray",
                                    static_cast<uint32_t>(SF64::ResourceType::GenericArray), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryTextureV0>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(LUS::ResourceType::Texture), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryTextureV1>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(LUS::ResourceType::Texture), 1);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryVertexV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vertex", static_cast<uint32_t>(LUS::ResourceType::Vertex), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryDisplayListV0>(), RESOURCE_FORMAT_BINARY,
                                    "DisplayList", static_cast<uint32_t>(LUS::ResourceType::DisplayList), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryMatrixV0>(), RESOURCE_FORMAT_BINARY,
                                    "Matrix", static_cast<uint32_t>(LUS::ResourceType::Matrix), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryBlobV0>(), RESOURCE_FORMAT_BINARY,
                                    "Blob", static_cast<uint32_t>(LUS::ResourceType::Blob), 0);
    loader->RegisterResourceFactory(std::make_shared<LUS::ResourceFactoryBinaryLightV0>(), RESOURCE_FORMAT_BINARY,
                                    "Lights1", static_cast<uint32_t>(LUS::ResourceType::Light), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryArrayV0>(), RESOURCE_FORMAT_BINARY,
                                    "Array", static_cast<uint32_t>(MK64::ResourceType::MK_Array), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryKartAIV0>(), RESOURCE_FORMAT_BINARY,
                                    "KartAI", static_cast<uint32_t>(MK64::ResourceType::KartAI), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryCourseVtxV0>(), RESOURCE_FORMAT_BINARY,
                                    "CourseVtx", static_cast<uint32_t>(MK64::ResourceType::CourseVertex), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackSectionsV0>(),
                                    RESOURCE_FORMAT_BINARY, "TrackSections",
                                    static_cast<uint32_t>(MK64::ResourceType::TrackSection), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackWaypointsV0>(),
                                    RESOURCE_FORMAT_BINARY, "Waypoints",
                                    static_cast<uint32_t>(MK64::ResourceType::Waypoints), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryActorSpawnDataV0>(),
                                    RESOURCE_FORMAT_BINARY, "SpawnData",
                                    static_cast<uint32_t>(MK64::ResourceType::SpawnData), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryUnkActorSpawnDataV0>(),
                                    RESOURCE_FORMAT_BINARY, "UnkSpawnData",
                                    static_cast<uint32_t>(MK64::ResourceType::UnkSpawnData), 0);
}

void GameEngine::Create() {
    const auto instance = Instance = new GameEngine();
    GameUI::SetupGuiElements();
#if defined(__SWITCH__) || defined(__WIIU__)
    CVarRegisterInteger("gControlNav", 1); // always enable controller nav on switch/wii u
#endif
}

void GameEngine::Destroy() {
}

bool ShouldClearTextureCacheAtEndOfFrame = false;

void GameEngine::StartFrame() const {
    using Ship::KbScancode;
    const int32_t dwScancode = this->context->GetWindow()->GetLastScancode();
    this->context->GetWindow()->SetLastScancode(-1);

    switch (dwScancode) {
        case KbScancode::LUS_KB_TAB: {
            // Toggle HD Assets
            CVarSetInteger("gAltAssets", !CVarGetInteger("gAltAssets", 0));
            ShouldClearTextureCacheAtEndOfFrame = true;
            break;
        }
        default:
            break;
    }
    this->context->GetWindow()->StartFrame();
}

// void GameEngine::ProcessFrame(void (*run_one_game_iter)()) const {
//     //this->context->GetWindow()->MainLoop(run_one_game_iter);
//     Instance->context->GetWindow()->MainLoop(run_one_game_iter);
// }

void GameEngine::RunCommands(Gfx* Commands) {
    gfx_run(Commands, {});
    gfx_end_frame();

    if (ShouldClearTextureCacheAtEndOfFrame) {
        gfx_texture_cache_clear();
        ShouldClearTextureCacheAtEndOfFrame = false;
    }
}

void GameEngine::ProcessGfxCommands(Gfx* commands) {
    RunCommands(commands);
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());
    if (wnd != nullptr) {
        wnd->SetTargetFps(30);
        wnd->SetMaximumFrameLatency(1);
    }
}

extern "C" uint32_t GameEngine_GetSampleRate() {
    auto player = Ship::Context::GetInstance()->GetAudio()->GetAudioPlayer();
    if (player == nullptr) {
        return 0;
    }

    if (!player->IsInitialized()) {
        return 0;
    }

    return player->GetSampleRate();
}

extern "C" uint32_t GameEngine_GetSamplesPerFrame() {
    return SAMPLES_PER_FRAME;
}

// End

extern "C" float GameEngine_GetAspectRatio() {
    return gfx_current_dimensions.aspect_ratio;
}

extern "C" uint32_t GameEngine_GetGameVersion() {
    return 0x00000001;
}

static const char* sOtrSignature = "__OTR__";

extern "C" uint8_t GameEngine_OTRSigCheck(const char* data) {
    static const char* sOtrSignaturea = "__OTR__";
    return strncmp(data, sOtrSignaturea, strlen(sOtrSignaturea)) == 0;
}

// struct TimedEntry {
//     uint64_t duration;
//     TimerAction action;
//     int32_t* address;
//     int32_t value;
//     bool active;
// };

// std::vector<TimedEntry> gTimerTasks;

// uint64_t Timer_GetCurrentMillis() {
//     return SDL_GetTicks();
// }

// extern "C" s32 Timer_CreateTask(u64 time, TimerAction action, s32* address, s32 value) {
//     const auto millis = Timer_GetCurrentMillis();
//     TimedEntry entry = {
//         .duration = millis + CYCLES_TO_MSEC_PC(time),
//         .action = action,
//         .address = address,
//         .value = value,
//         .active = true,
//     };

//     gTimerTasks.push_back(entry);

//     return gTimerTasks.size() - 1;
// }

extern "C" void Timer_Increment(int32_t* address, int32_t value) {
    *address += value;
}

extern "C" void Timer_SetValue(int32_t* address, int32_t value) {
    *address = value;
}

// void Timer_CompleteTask(TimedEntry& task) {
//     if (task.action != nullptr) {
//         task.action(task.address, task.value);
//     }
//     task.active = false;
// }

// extern "C" void Timer_Update() {

//     if(gTimerTasks.empty()) {
//         return;
//     }

//     const auto millis = Timer_GetCurrentMillis();

//     for (auto& task : gTimerTasks) {
//         if (task.active && millis >= task.duration) {
//             Timer_CompleteTask(task);
//         }
//     }
// }

extern "C" float OTRGetAspectRatio() {
    return gfx_current_dimensions.aspect_ratio;
}

extern "C" float OTRGetDimensionFromLeftEdge(float v) {
    return (SCREEN_WIDTH / 2 - SCREEN_HEIGHT / 2 * OTRGetAspectRatio() + (v));
}

extern "C" int16_t OTRGetRectDimensionFromLeftEdge(float v) {
    return ((int) floorf(OTRGetDimensionFromLeftEdge(v)));
}

extern "C" float OTRGetDimensionFromRightEdge(float v) {
    return (SCREEN_WIDTH / 2 + SCREEN_HEIGHT / 2 * OTRGetAspectRatio() - (SCREEN_WIDTH - v));
}

extern "C" int16_t OTRGetRectDimensionFromRightEdge(float v) {
    return ((int) ceilf(OTRGetDimensionFromRightEdge(v)));
}

// Gets the width of the current render target area
extern "C" uint32_t OTRGetGameRenderWidth() {
    return gfx_current_dimensions.width;
}

// Gets the height of the current render target area
extern "C" uint32_t OTRGetGameRenderHeight() {
    return gfx_current_dimensions.height;
}
