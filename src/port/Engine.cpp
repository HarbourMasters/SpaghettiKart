#include "Engine.h"

#include <cstdlib>
#include "ship/utils/StringHelper.h"
#include "GameExtractor.h"
#include "engine/mods/ModManager.h"
#include "ui/ImguiUI.h"
#include "port/ShipCompat.h"
#include "ship/controller/controldevice/controller/mapping/ControllerDefaultMappings.h"
#include <ship/audio/Audio.h>
#include "resource/type/ResourceType.h"
#include "fast/resource/ResourceType.h"
#include "resource/importers/GenericArrayFactory.h"
#include "resource/importers/AudioBankFactory.h"
#include "resource/importers/AudioSampleFactory.h"
#include "resource/importers/AudioSequenceFactory.h"
#include "resource/importers/Vec3fFactory.h"
#include "resource/importers/Vec3sFactory.h"
#include "resource/importers/CPUFactory.h"
#include "resource/importers/CourseVtxFactory.h"
#include "resource/importers/TrackSectionsFactory.h"
#include "resource/importers/TrackPathPointFactory.h"
#include "resource/importers/ActorSpawnDataFactory.h"
#include "resource/importers/UnkActorSpawnDataFactory.h"
#include "resource/importers/ArrayFactory.h"
#include "resource/importers/MinimapFactory.h"
#include "resource/importers/BetterTextureFactory.h"
#include <ship/window/gui/Fonts.h>
#include "ship/window/gui/resource/Font.h"
#include "ship/window/gui/resource/FontFactory.h"
#include "libultraship/window/gui/InputEditorWindow.h"
#include "libultraship/window/gui/GfxDebuggerWindow.h"
#include "libultraship/controller/controldeck/ControlDeck.h"
#include "libultraship/bridge/UltraBridge.h"
#include <ship/core/Context.h>
#include "SpaghettiGui.h"

#include "port/interpolation/FrameInterpolation.h"
#include <fast/Fast3dWindow.h>
#include <fast/interpreter.h>
// #include <Fast3D/gfx_rendering_api.h>
#include <SDL2/SDL.h>

#include <utility>

#ifdef __SWITCH__
#include <ship/port/switch/SwitchImpl.h>
#endif

extern "C" {
bool prevAltAssets = false;
float gInterpolationStep = 0.0f;
#include <macros.h>
#include <fast/resource/factory/DisplayListFactory.h>
#include <fast/resource/factory/TextureFactory.h>
#include <fast/resource/factory/MatrixFactory.h>
#include <ship/resource/factory/BlobFactory.h>
#include <fast/resource/factory/VertexFactory.h>
#include <fast/resource/factory/LightFactory.h>
// #include <PngFactory.h>
#include "audio/internal.h"
#include "audio/GameAudio.h"
}

std::shared_ptr<Ship::SpaghettiGui> gsSpaghettiGui;
std::shared_ptr<Fast::Fast3dWindow> gsFast3dWindow;
std::shared_ptr<LUS::InputEditorWindow> gsInputEditorWindow;
std::shared_ptr<LUS::GfxDebuggerWindow> gsGfxDebuggerWindow;

Fast::Interpreter* GetInterpreter() {
    return static_pointer_cast<Fast::Fast3dWindow>(ShipCompat::GetWindow())
        ->GetInterpreterWeak()
        .lock()
        .get();
}

GameEngine* GameEngine::Instance;

bool CreateDirectoryRecursive(std::string const& dirName, std::error_code& err) {
    err.clear();
    if (!std::filesystem::create_directories(dirName, err)) {
        if (std::filesystem::exists(dirName)) {
            // The folder already exists:
            err.clear();
            return true;
        }
        return false;
    }
    return true;
}

GameEngine::GameEngine() {
    // Initialize context properties early to recognize paths properly for non-portable builds
    //this->context = Ship::Context::CreateUninitializedInstance("Spaghetti Kart", "spaghettify", "spaghettify.cfg.json");

    this->context = Ship::Context::CreateInstance("Spaghetti Kart", "spaghettify");
    this->context->Init();
    ShipCompat::SetContext(this->context);
#ifdef __SWITCH__
    Ship::Switch::Init(Ship::PreInitPhase);
#endif

#ifdef _WIN32
    AllocConsole();
#endif
    //this->context->InitConfiguration();    // without this line InitConsoleVariables fails at Config::Reload()
    //this->context->InitConsoleVariables(); // without this line the controldeck constructor failes in
                                           // ShipDeviceIndexMappingManager::UpdateControllerNamesFromConfig()


    auto logger = std::make_shared<Ship::Logger>(
        "SpaghettiKart", Ship::Context::GetPathRelativeToAppDirectory("logs/spaghettify.log"));
    context->GetChildren().Add(logger);
    logger->Init();

    auto config = std::make_shared<Ship::Config>(Ship::Context::GetPathRelativeToAppDirectory("spaghettify.cfg.json"),
                                                 nullptr);
    context->GetChildren().Add(config);

    auto consoleVariables = std::make_shared<Ship::ConsoleVariable>(config);
    context->GetChildren().Add(consoleVariables);
    CVarSetConsoleVariable(consoleVariables);

    auto ultraBridge = std::make_shared<LUS::UltraBridge>();
    context->GetChildren().Add(ultraBridge);

    gsSpaghettiGui = std::make_shared<Ship::SpaghettiGui>(std::vector<std::shared_ptr<Ship::GuiWindow>>({}));

    gsFast3dWindow = std::make_shared<Fast::Fast3dWindow>(gsSpaghettiGui, config,
                                                          consoleVariables);

#if (_DEBUG)
    auto defaultLogLevel = spdlog::level::debug;
#else
    auto defaultLogLevel = spdlog::level::info;
#endif

    auto logLevel =
        static_cast<spdlog::level::level_enum>(CVarGetInteger("gDeveloperTools.LogLevel", defaultLogLevel));
    spdlog::set_level(logLevel);
    spdlog::flush_on(logLevel);

    auto threadPool = std::make_shared<Ship::ThreadPool>(std::max(1, (int32_t)(std::thread::hardware_concurrency() - 3 - 1)));
                      context->GetChildren().Add(threadPool);

#ifdef ENABLE_SCRIPTING
    auto keystore = std::make_shared<Ship::Keystore>(config);
    context->GetChildren().Add(keystore);
    auto resourceManager = std::make_shared<Ship::ResourceManager>(threadPool, keystore);
#else
    auto resourceManager = std::make_shared<Ship::ResourceManager>(threadPool);
#endif
    context->GetChildren().Add(resourceManager);
    
    //auto controlDeck = std::make_shared<LUS::ControlDeck>(gsFast3dWindow, consoleVariables);
    //context->GetChildren().Add(controlDeck);
    
   

    auto defaultMappings = std::make_shared<Ship::ControllerDefaultMappings>(
        // KeyboardKeyToButtonMappings
        std::unordered_map<CONTROLLERBUTTONS_T, std::unordered_set<Ship::KbScancode>>{
            { BTN_A, { Ship::KbScancode::LUS_KB_SHIFT} },
            { BTN_B, { Ship::KbScancode::LUS_KB_CONTROL} },
            { BTN_L, { Ship::KbScancode::LUS_KB_Q} },
            { BTN_R, { Ship::KbScancode::LUS_KB_SPACE} },
            { BTN_Z, { Ship::KbScancode::LUS_KB_Z} },
            { BTN_START, { Ship::KbScancode::LUS_KB_ENTER} },
            { BTN_CUP, { Ship::KbScancode::LUS_KB_T} },
            { BTN_CDOWN, { Ship::KbScancode::LUS_KB_G} },
            { BTN_CLEFT, { Ship::KbScancode::LUS_KB_F} },
            { BTN_CRIGHT, { Ship::KbScancode::LUS_KB_H} },
            { BTN_DUP, { Ship::KbScancode::LUS_KB_NUMPAD8} },
            { BTN_DDOWN, { Ship::KbScancode::LUS_KB_NUMPAD2} },
            { BTN_DLEFT, { Ship::KbScancode::LUS_KB_NUMPAD4} },
            { BTN_DRIGHT, { Ship::KbScancode::LUS_KB_NUMPAD6} }
        },
        // KeyboardKeyToAxisDirectionMappings - use built-in LUS defaults
        std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, Ship::KbScancode>>>{
            { Ship::StickIndex::LEFT_STICK, {
                { Ship::Direction::UP, Ship::KbScancode::LUS_KB_ARROWKEY_UP},
                { Ship::Direction::DOWN, Ship::KbScancode::LUS_KB_ARROWKEY_DOWN},
                { Ship::Direction::LEFT, Ship::KbScancode::LUS_KB_ARROWKEY_LEFT},
                { Ship::Direction::RIGHT, Ship::KbScancode::LUS_KB_ARROWKEY_RIGHT}
            }}
        },
        // SDLButtonToButtonMappings
        std::unordered_map<CONTROLLERBUTTONS_T, std::unordered_set<SDL_GameControllerButton>>{
            { BTN_A, { SDL_CONTROLLER_BUTTON_A } },
            { BTN_B, { SDL_CONTROLLER_BUTTON_X } },
            { BTN_START, { SDL_CONTROLLER_BUTTON_START } },
            { BTN_CLEFT, { SDL_CONTROLLER_BUTTON_Y } },
            { BTN_CDOWN, { SDL_CONTROLLER_BUTTON_B } },
            { BTN_DUP, { SDL_CONTROLLER_BUTTON_DPAD_UP } },
            { BTN_DDOWN, { SDL_CONTROLLER_BUTTON_DPAD_DOWN } },
            { BTN_DLEFT, { SDL_CONTROLLER_BUTTON_DPAD_LEFT } },
            { BTN_DRIGHT, { SDL_CONTROLLER_BUTTON_DPAD_RIGHT } },
            { BTN_R, { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER } },
            { BTN_L, { SDL_CONTROLLER_BUTTON_LEFTSHOULDER } }
        },
        // SDLButtonToAxisDirectionMappings - use built-in LUS defaults
        std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, SDL_GameControllerButton>>>(),
        // SDLAxisDirectionToButtonMappings
        std::unordered_map<CONTROLLERBUTTONS_T, std::vector<std::pair<SDL_GameControllerAxis, int32_t>>>{
            { BTN_R, { { SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 1 } } },
            { BTN_Z, { { SDL_CONTROLLER_AXIS_TRIGGERLEFT, 1 } } },
            { BTN_CUP, { { SDL_CONTROLLER_AXIS_RIGHTY, -1 } } },
            { BTN_CRIGHT, { { SDL_CONTROLLER_AXIS_RIGHTX, 1 } } }
        },
        // SDLAxisDirectionToAxisDirectionMappings - use built-in LUS defaults
        std::unordered_map<Ship::StickIndex, std::vector<std::pair<Ship::Direction, std::pair<SDL_GameControllerAxis, int32_t>>>>()
    );

    auto buttonNames = std::unordered_map<CONTROLLERBUTTONS_T, std::string>({
                      { BTN_A, "A" },
                      { BTN_B, "B" },
                      { BTN_L, "L" },
                      { BTN_R, "R" },
                      { BTN_Z, "Z" },
                      { BTN_START, "Start" },
                      { BTN_CLEFT, "CLeft" },
                      { BTN_CRIGHT, "CRight" },
                      { BTN_CUP, "CUp" },
                      { BTN_CDOWN, "CDown" },
                      { BTN_DLEFT, "DLeft" },
                      { BTN_DRIGHT, "DRight" },
                      { BTN_DUP, "DUp" },
                      { BTN_DDOWN, "DDown" },
                  });

    auto controlDeck = std::make_shared<LUS::ControlDeck>(gsFast3dWindow, consoleVariables);
    // auto controlDeck = std::make_shared<LUS::ControlDeck>(std::vector<CONTROLLERBUTTONS_T>(), defaultMappings, buttonNames);
    context->GetChildren().Add(controlDeck);
    const std::string assets_path = Ship::Context::LocateFileAcrossAppDirs(engine_asset_file);
    //this->context->InitResourceManager({assets_path}, {}, 3); // without this line InitWindow fails in Gui::Init()
    //this->context->InitConsole(); // without this line the GuiWindow constructor fails in ConsoleWindow::OnInit()

    try {
        nlohmann::json rmArgs;
        rmArgs["archivePaths"] = std::vector<std::string>{ assets_path };
        rmArgs["validHashes"] = std::vector<uint32_t>{};
        resourceManager->Init(rmArgs);
    } catch (const std::exception& e) {
        SPDLOG_WARN("ResourceManager init deferred: {}", e.what());
    }

    auto console = std::make_shared<Ship::Console>();
    context->GetChildren().Add(console);
    console->Init();

    auto crashHandler = std::make_shared<Ship::CrashHandler>();
    context->GetChildren().Add(crashHandler);

    auto gfxDebugger = std::make_shared<Fast::GfxDebugger>();
    context->GetChildren().Add(gfxDebugger);

    context->GetChildren().Add(gsFast3dWindow);
    
    auto events = std::make_shared<Ship::Events>();
    context->GetChildren().Add(events);

    ultraBridge->Init();
    ultraBridge->UpdateCaches(context);
    
    gsFast3dWindow->Init();
    gsFast3dWindow->SetRendererUCode(ucode_f3dex);

    if (gsSpaghettiGui) {

        gsInputEditorWindow = std::make_shared<LUS::InputEditorWindow>(CVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN, "Input Editor", controlDeck, gsFast3dWindow);
        gsGfxDebuggerWindow = std::make_shared<LUS::GfxDebuggerWindow>(CVAR_GFX_DEBUGGER_WINDOW_OPEN, "GfxDebuggerWindow",  std::dynamic_pointer_cast<Fast::Fast3dWindow>(ShipCompat::GetWindow()), gfxDebugger, resourceManager);
        gsGfxDebuggerWindow->SetOriginalSize(ImVec2(520, 600));
        gsSpaghettiGui->AddGuiWindow(gsInputEditorWindow);
        gsSpaghettiGui->AddGuiWindow(gsGfxDebuggerWindow);// ImVec2(520, 600)));





       // gsGfxDebuggerWindow->mOriginalSize = ImVec2(520, 600);
    }

    SPDLOG_INFO("Spaghetti Kart " SPAGHETTI_VERSION);
    SPDLOG_INFO(CVarGetInteger("gEnableDebugMode", 0) == 0 ? "Debug Mode deactivated" : "Debug Mode activated");

    auto loader = ShipCompat::GetResourceManager()->GetResourceLoader();
    loader->RegisterResourceFactory(std::make_shared<SM64::AudioBankFactoryV0>(), RESOURCE_FORMAT_BINARY, "AudioBank",
                                    static_cast<uint32_t>(SF64::ResourceType::Bank), 0);
    loader->RegisterResourceFactory(std::make_shared<SM64::AudioSampleFactoryV0>(), RESOURCE_FORMAT_BINARY,
                                    "AudioSample", static_cast<uint32_t>(SF64::ResourceType::Sample), 0);
    loader->RegisterResourceFactory(std::make_shared<SM64::AudioSequenceFactoryV0>(), RESOURCE_FORMAT_BINARY,
                                    "AudioSequence", static_cast<uint32_t>(SF64::ResourceType::Sequence), 0);
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3fV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vec3f", static_cast<uint32_t>(SF64::ResourceType::Vec3f), 0);
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3sV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vec3s", static_cast<uint32_t>(SF64::ResourceType::Vec3s), 0);
    loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryGenericArrayV0>(),
                                    RESOURCE_FORMAT_BINARY, "GenericArray",
                                    static_cast<uint32_t>(SF64::ResourceType::GenericArray), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTextureV0>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(Fast::ResourceType::Texture), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTextureV1>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(Fast::ResourceType::Texture), 1);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryVertexV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vertex", static_cast<uint32_t>(Fast::ResourceType::Vertex), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryXMLVertexV0>(), RESOURCE_FORMAT_XML, "Vertex",
                                    static_cast<uint32_t>(Fast::ResourceType::Vertex), 0);

    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryDisplayListV0>(),
                                    RESOURCE_FORMAT_BINARY, "DisplayList",
                                    static_cast<uint32_t>(Fast::ResourceType::DisplayList), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryXMLDisplayListV0>(), RESOURCE_FORMAT_XML,
                                    "DisplayList", static_cast<uint32_t>(Fast::ResourceType::DisplayList), 0);

    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryMatrixV0>(), RESOURCE_FORMAT_BINARY,
                                    "Matrix", static_cast<uint32_t>(Fast::ResourceType::Matrix), 0);
    loader->RegisterResourceFactory(std::make_shared<Ship::ResourceFactoryBinaryBlobV0>(), RESOURCE_FORMAT_BINARY,
                                    "Blob", static_cast<uint32_t>(Ship::ResourceType::Blob), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryLightV0>(), RESOURCE_FORMAT_BINARY,
                                    "Lights1", static_cast<uint32_t>(Fast::ResourceType::Light), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryArrayV0>(), RESOURCE_FORMAT_BINARY,
                                    "Array", static_cast<uint32_t>(MK64::ResourceType::MK_Array), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryCPUV0>(), RESOURCE_FORMAT_BINARY, "CPU",
                                    static_cast<uint32_t>(MK64::ResourceType::CPU), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryCourseVtxV0>(), RESOURCE_FORMAT_BINARY,
                                    "CourseVtx", static_cast<uint32_t>(MK64::ResourceType::CourseVertex), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackSectionsV0>(),
                                    RESOURCE_FORMAT_BINARY, "TrackSections",
                                    static_cast<uint32_t>(MK64::ResourceType::TrackSection), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryXMLTrackSectionsV0>(),
                                    RESOURCE_FORMAT_XML, "TrackSections",
                                    static_cast<uint32_t>(MK64::ResourceType::TrackSection), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackPathPointsV0>(),
                                    RESOURCE_FORMAT_BINARY, "Paths",
                                    static_cast<uint32_t>(MK64::ResourceType::Paths), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryXMLTrackPathPointsV0>(),
                                    RESOURCE_FORMAT_XML, "Paths",
                                    static_cast<uint32_t>(MK64::ResourceType::Paths), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryActorSpawnDataV0>(),
                                    RESOURCE_FORMAT_BINARY, "SpawnData",
                                    static_cast<uint32_t>(MK64::ResourceType::SpawnData), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryUnkActorSpawnDataV0>(),
                                    RESOURCE_FORMAT_BINARY, "UnkSpawnData",
                                    static_cast<uint32_t>(MK64::ResourceType::UnkSpawnData), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryMinimapV0>(), RESOURCE_FORMAT_BINARY,
                                    "Minimap", static_cast<uint32_t>(MK64::ResourceType::Minimap), 0);

    fontMono = CreateFontWithSize(16.0f, "fonts/Inconsolata-Regular.ttf");
    fontMonoLarger = CreateFontWithSize(20.0f, "fonts/Inconsolata-Regular.ttf");
    fontMonoLargest = CreateFontWithSize(24.0f, "fonts/Inconsolata-Regular.ttf");
    fontStandard = CreateFontWithSize(16.0f, "fonts/Montserrat-Regular.ttf");
    fontStandardLarger = CreateFontWithSize(20.0f, "fonts/Montserrat-Regular.ttf");
    fontStandardLargest = CreateFontWithSize(24.0f, "fonts/Montserrat-Regular.ttf");
    ImGui::GetIO().FontDefault = fontMono;
}

bool GameEngine::GenAssetFile() {
    auto extractor = new GameExtractor();

    if (!extractor->SelectGameFromUI()) {
        ShowMessage("Error", "No ROM selected.\n\nExiting...");
        // _Exit, not exit: this runs before the game world is initialized, so running the
        // global World destructor (CleanWorld) would dereference still-null singletons and crash.
        _Exit(1);
    }

    auto game = extractor->ValidateChecksum();
    if (!game.has_value()) {
        ShowMessage("Unsupported ROM",
                    "The provided ROM is not supported.\n\nCheck the readme for a list of supported versions.");
        _Exit(1);
    }

    ShowMessage(("Found " + game.value()).c_str(),
                "The extraction process will now begin.\n\nThis may take a few minutes.", SDL_MESSAGEBOX_INFORMATION);

    return extractor->GenerateOTR();
}

uint32_t GameEngine::GetInterpolationFPS() {
    if (CVarGetInteger("gMatchRefreshRate", 0)) {
        return ShipCompat::GetWindow()->GetCurrentRefreshRate();

    } else if (CVarGetInteger("gVsyncEnabled", 1) ||
               !ShipCompat::GetWindow()->CanDisableVerticalSync()) {
        return std::min<uint32_t>(ShipCompat::GetWindow()->GetCurrentRefreshRate(),
                                  CVarGetInteger("gInterpolationFPS", 30));
    }

    return CVarGetInteger("gInterpolationFPS", 30);
}

uint32_t GameEngine::GetInterpolationFrameCount() {
    return ceil((float) GetInterpolationFPS() / (60.0f / 2 /*gVIsPerFrame*/));
}

extern "C" uint32_t GameEngine_GetInterpolationFrameCount() {
    return GameEngine::GetInterpolationFrameCount();
}

void GameEngine::ShowMessage(const char* title, const char* message, SDL_MessageBoxFlags type) {
#if defined(__SWITCH__)
    SPDLOG_ERROR(message);
#else
    SDL_ShowSimpleMessageBox(type, title, message, nullptr);
    SPDLOG_ERROR(message);
#endif
}

int GameEngine::ShowYesNoBox(const char* title, const char* box) {
    int ret;
#ifdef _WIN32
    ret = MessageBoxA(nullptr, box, title, MB_YESNO | MB_ICONQUESTION);
#elif defined(__SWITCH__)
    SPDLOG_ERROR(box);
    return IDYES;
#else
    SDL_MessageBoxData boxData = { 0 };
    SDL_MessageBoxButtonData buttons[2] = { { 0 } };

    buttons[0].buttonid = IDYES;
    buttons[0].text = "Yes";
    buttons[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[1].buttonid = IDNO;
    buttons[1].text = "No";
    buttons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
    boxData.numbuttons = 2;
    boxData.flags = SDL_MESSAGEBOX_INFORMATION;
    boxData.message = box;
    boxData.title = title;
    boxData.buttons = buttons;
    SDL_ShowMessageBox(&boxData, &ret);
#endif
    return ret;
}

void GameEngine::Create() {
    const auto instance = Instance = new GameEngine();
    InitModsSystem();
    instance->AudioInit();
    instance->gHMAS = new HMAS();
    GameUI::SetupGuiElements();
#if defined(__SWITCH__) || defined(__WIIU__)
    CVarRegisterInteger("gControlNav", 1); // always enable controller nav on switch/wii u
#endif
}

void GameEngine::Destroy() {
    AudioExit();
#ifdef __SWITCH__
    Ship::Switch::Exit();
#endif
    UnloadMods();
    GameUI::Destroy();
    delete GameEngine::Instance;
    GameEngine::Instance = nullptr;
}

bool ShouldClearTextureCacheAtEndOfFrame = false;

void GameEngine::StartFrame() const {
    using Ship::KbScancode;
    const int32_t dwScancode = ShipCompat::GetWindow()->GetLastScancode();
    ShipCompat::GetWindow()->SetLastScancode(-1);

    switch (dwScancode) {
        case KbScancode::LUS_KB_TAB: {
            // Toggle HD Assets
            CVarSetInteger("gEnhancements.Mods.AlternateAssets", !CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0));
            break;
        }
        case KbScancode::LUS_KB_P: {

            break;
        }
        default:
            break;
    }
}

// void GameEngine::ProcessFrame(void (*run_one_game_iter)()) const {
//     //this->context->GetWindow()->MainLoop(run_one_game_iter);
//     Instance->context->GetWindow()->MainLoop(run_one_game_iter);
// }

void GameEngine::RunCommands(Gfx* pool, const std::vector<std::unordered_map<Mtx*, MtxF>>& mtx_replacements) {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(ShipCompat::GetWindow());

    if (wnd == nullptr) {
        return;
    }

    auto interpreter = wnd->GetInterpreterWeak().lock().get();

    // Process window events for resize, mouse, keyboard events
    wnd->HandleEvents();

    interpreter->mInterpolationIndex = 0;

    for (const auto& mtxStack : mtx_replacements) {
        wnd->DrawAndRunGraphicsCommands(pool, mtxStack);
        interpreter->mInterpolationIndex++;
    }

    bool curAltAssets = CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0);
    if (prevAltAssets != curAltAssets) {
        prevAltAssets = curAltAssets;
        ShipCompat::GetResourceManager()->SetAltAssetsEnabled(curAltAssets);
        gfx_texture_cache_clear();
    }
}

/**
 * During the draw phase, the gfx pool is filled with graphics commands.
 * At the end of the game loop, these commands are sent into lus and interpreted
 * or translated into modern graphics commands
 */
void GameEngine::ProcessGfxCommands(Gfx* pool) {
    std::vector<std::unordered_map<Mtx*, MtxF>> mtx_replacements;
    int target_fps = GameEngine::Instance->GetInterpolationFPS();
    if (CVarGetInteger("gModifyInterpolationTargetFPS", 0)) {
        target_fps = CVarGetInteger("gInterpolationTargetFPS", 60);
    }
    static int last_fps;
    static int last_update_rate;
    static int time;
    int fps = target_fps;
    int original_fps = 60 / 2 /*gVIsPerFrame*/;

    if (target_fps == 30 || original_fps > target_fps) {
        fps = original_fps;
    }

    if (last_fps != fps || last_update_rate != 2 /*gVIsPerFrame*/) {
        time = 0;
    }

    // time_base = fps * original_fps (one second)
    int next_original_frame = fps;

    // Get matrix replacements for intermediate frames
    while (time + original_fps <= next_original_frame) {
        time += original_fps;
        if (time != next_original_frame) {
            mtx_replacements.push_back(FrameInterpolation_Interpolate((float) time / next_original_frame));
        } else {
            mtx_replacements.emplace_back(); // No interpolation for key frames
        }
    }
    // printf("mtxf size: %d\n", mtx_replacements.size());

    time -= fps;

    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(ShipCompat::GetWindow());
    if (wnd != nullptr) {
        wnd->SetTargetFps(GetInterpolationFPS());
        wnd->SetMaximumFrameLatency(1);
    }
    RunCommands(pool, mtx_replacements);

    last_fps = fps;
    last_update_rate = 2;
}

// Audio
void GameEngine::HandleAudioThread() {
    while (audio.running) {
        {
            std::unique_lock<std::mutex> Lock(audio.mutex);
            while (!audio.processing && audio.running) {
                audio.cv_to_thread.wait(Lock);
            }

            if (!audio.running) {
                break;
            }
        }
        std::unique_lock<std::mutex> Lock(audio.mutex);

        int samples_left = AudioPlayerBuffered();
        u32 num_audio_samples = samples_left < AudioPlayerGetDesiredBuffered() ? SAMPLES_HIGH : SAMPLES_LOW;

        s16 nas_buffer[SAMPLES_PER_FRAME] = { 0 };
        f32 hmas_buffer[SAMPLES_PER_FRAME] = { 0 };
        s16 mix_buffer[SAMPLES_PER_FRAME] = { 0 };

        for (size_t i = 0; i < NUM_AUDIO_CHANNELS; i++) {
            create_next_audio_buffer(nas_buffer + i * (num_audio_samples * 2), num_audio_samples);
        }

        GameEngine::Instance->gHMAS->CreateBuffer((u8*)hmas_buffer, 4 * num_audio_samples * sizeof(float));

        float master_vol = CVarGetFloat("gGameMasterVolume", 1.0f);

        for (size_t i = 0; i < SAMPLES_PER_FRAME; i++) {
            mix_buffer[i] = nas_buffer[i] + ((int16_t)(hmas_buffer[i] * 32767.0f) * master_vol);
        }

        AudioPlayerPlayFrame((u8*) mix_buffer, 2 * num_audio_samples * 4);

        audio.processing = false;
        audio.cv_from_thread.notify_one();
    }
}

void GameEngine::StartAudioFrame() {
    {
        std::unique_lock<std::mutex> Lock(audio.mutex);
        audio.processing = true;
    }

    audio.cv_to_thread.notify_one();
}

void GameEngine::EndAudioFrame() {
    {
        std::unique_lock<std::mutex> Lock(audio.mutex);
        while (audio.processing) {
            audio.cv_from_thread.wait(Lock);
        }
    }
}

void GameEngine::AudioInit() {
    const auto resourceMgr = ShipCompat::GetResourceManager();

    auto audios = std::make_shared<Ship::Audio>(
        Ship::AudioSettings{ .SampleRate = 26800, .SampleLength = 512, .DesiredBuffered = 1100 },
        ShipCompat::GetConfig());
    context->GetChildren().Add(audios);
    audios->Init();

    if (auto ultraBridge = context->GetChildren().GetFirst<LUS::UltraBridge>()) {
        ultraBridge->UpdateCaches(context);
    }

    resourceMgr->LoadResources("sound");
    const auto banksFiles = resourceMgr->GetArchiveManager()->ListFiles("sound/banks/*");
    const auto sequences_files = resourceMgr->GetArchiveManager()->ListFiles("sound/sequences/*");

    Instance->sequenceTable.resize(512);
    Instance->audioSequenceTable.resize(512);
    Instance->banksTable.resize(512);

    for (auto& bank : *banksFiles) {
        auto path = "__OTR__" + bank;
        const auto ctl = static_cast<CtlEntry*>(ResourceGetDataByName(path.c_str()));
        this->bankMapTable[bank] = ctl->bankId;
        SPDLOG_INFO("Loaded bank: {}", bank);
    }

    for (auto& sequence : *sequences_files) {
        if (sequence.find('.') != std::string::npos) {
            continue;
        }
        auto path = "__OTR__" + sequence;
        auto seq = static_cast<AudioSequenceData*>(ResourceGetDataByName(path.c_str()));
        Instance->sequenceTable[seq->id] = path;
        SPDLOG_INFO("Loaded sequence: {}", sequence);
    }

    if (!audio.running) {
        audio.running = true;
        audio.thread = std::thread(HandleAudioThread);
        SPDLOG_INFO("Audio thread started");
    }
}

void GameEngine::AudioExit() {
    {
        std::unique_lock lock(audio.mutex);
        audio.running = false;
    }
    audio.cv_to_thread.notify_all();

    // Wait until the audio thread quit
    audio.thread.join();
}

uint8_t GameEngine::GetBankIdByName(const std::string& name) {
    if (Instance->bankMapTable.contains(name)) {
        return Instance->bankMapTable[name];
    }
    return 0;
}

ImFont* GameEngine::CreateFontWithSize(float size, std::string fontPath) {
    auto mImGuiIo = &ImGui::GetIO();
    ImFont* font;
    // Rasterize the glyph atlas at higher density so menu text stays sharp on HiDPI/Retina
    // displays. Bake at retinaScale * maxMenuScale so the runtime Menu Scale setting
    // (FontGlobalScale) only ever downsamples the atlas rather than stretching it blurry.
    float rasterDensity = 1.0f;
#if defined(__APPLE__)
    constexpr float kRetinaScale = 2.0f;  // Retina backing scale
    constexpr float kMaxMenuScale = 2.0f; // keep in sync with the gSettings.Menu.Scale slider Max
    rasterDensity = kRetinaScale * kMaxMenuScale;
#endif
    if (fontPath == "") {
        ImFontConfig fontCfg = ImFontConfig();
        fontCfg.OversampleH = fontCfg.OversampleV = 1;
        fontCfg.PixelSnapH = true;
        fontCfg.SizePixels = size;
        fontCfg.RasterizerDensity = rasterDensity;
        font = mImGuiIo->Fonts->AddFontDefault(&fontCfg);
    } else {
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->Type = static_cast<uint32_t>(RESOURCE_TYPE_FONT);
        initData->ResourceVersion = 0;
        initData->Identifier.SetPath(fontPath);
        std::shared_ptr<Ship::Font> fontData = std::static_pointer_cast<Ship::Font>(
            ShipCompat::GetResourceManager()->LoadResource(fontPath, false, initData));
        char* fontDataPtr = (char*) malloc(fontData->DataSize);
        memcpy(fontDataPtr, fontData->Data, fontData->DataSize);
        ImFontConfig fontCfg = ImFontConfig();
        fontCfg.RasterizerDensity = rasterDensity;
        font = mImGuiIo->Fonts->AddFontFromMemoryTTF(fontDataPtr, fontData->DataSize, size, &fontCfg);
    }
    // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    float iconFontSize = size * 2.0f / 3.0f;
    static const ImWchar sIconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    iconsConfig.RasterizerDensity = rasterDensity;
    mImGuiIo->Fonts->AddFontFromMemoryCompressedBase85TTF(fontawesome_compressed_data_base85, iconFontSize,
                                                          &iconsConfig, sIconsRanges);
    return font;
}

// End

extern "C" uint32_t GameEngine_GetSampleRate() {
    auto player = ShipCompat::GetAudio()->GetAudioPlayer();
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

extern "C" CtlEntry* GameEngine_LoadBank(const uint8_t bankId) {
    const auto engine = GameEngine::Instance;

    if (bankId >= engine->bankMapTable.size()) {
        return nullptr;
    }

    if (engine->banksTable[bankId] != nullptr) {
        return engine->banksTable[bankId];
    }

    for (auto& bank : engine->bankMapTable) {
        if (bank.second == bankId) {
            const auto ctl = static_cast<CtlEntry*>(ResourceGetDataByName(("__OTR__" + bank.first).c_str()));
            engine->banksTable[bankId] = ctl;
            return ctl;
        }
    }
    return nullptr;
}

extern "C" uint8_t GameEngine_IsBankLoaded(const uint8_t bankId) {
    const auto engine = GameEngine::Instance;
    GameEngine_LoadBank(bankId);
    return engine->banksTable[bankId] != nullptr;
}

extern "C" void GameEngine_UnloadBank(const uint8_t bankId) {
    const auto engine = GameEngine::Instance;
    engine->banksTable[bankId] = nullptr;
}

extern "C" AudioSequenceData* GameEngine_LoadSequence(const uint8_t seqId) {
    auto engine = GameEngine::Instance;

    if (engine->sequenceTable[seqId].empty()) {
        return nullptr;
    }

    if (engine->audioSequenceTable[seqId] != nullptr) {
        return engine->audioSequenceTable[seqId];
    }

    auto sequences = static_cast<AudioSequenceData*>(ResourceGetDataByName(engine->sequenceTable[seqId].c_str()));
    engine->audioSequenceTable[seqId] = sequences;
    return sequences;
}

extern "C" uint32_t GameEngine_GetSequenceCount() {
    auto engine = GameEngine::Instance;
    return engine->sequenceTable.size();
}

extern "C" uint8_t GameEngine_IsSequenceLoaded(const uint8_t seqId) {
    return GameEngine_LoadSequence(seqId) != nullptr;
}

extern "C" void GameEngine_UnloadSequence(const uint8_t seqId) {
    const auto engine = GameEngine::Instance;
    engine->audioSequenceTable[seqId] = nullptr;
}

extern "C" float GameEngine_GetAspectRatio() {
    auto gfx_current_dimensions = GetInterpreter()->mCurDimensions;
    return gfx_current_dimensions.aspect_ratio;
}

extern "C" uint32_t GameEngine_GetGameVersion() {
    return 0x00000001;
}

static const char* const sOtrSignature = "__OTR__";

extern "C" bool GameEngine_OTRSigCheck(const char* data) {
    return strncmp(data, sOtrSignature, strlen(sOtrSignature)) == 0;
}

extern "C" int32_t GameEngine_ResourceGetTexTypeByName(const char* name) {
    const auto res = std::static_pointer_cast<Fast::Texture>(ResourceLoad(name));

    if (res != nullptr) {
        return (int16_t) res->Type;
    }

    SPDLOG_ERROR("Given texture path {} is a non-existent resource", name);
    return -1;
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

extern "C" float OTRGetAspectRatio() {
    return GetInterpreter()->mCurDimensions.aspect_ratio;
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

/**
 * Centers an item in a given area.
 *
 * Adds the number of extended screen pixels to the location to center.
 * This allows stretching the game window really wide, and the item will stay in-place.
 *
 * This is not for centering in the direct center of the screen.
 *
 * How to use:
 *
 * s32 center = OTRCalculateCenterOfAreaFromRightEdge((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 2));
 * x = center - (texWidth / 2)
 * x2 = center + (texWidth / 2)
 */
extern "C" uint32_t OTRCalculateCenterOfAreaFromRightEdge(int32_t center) {
    return ((OTRGetDimensionFromRightEdge(SCREEN_WIDTH) - SCREEN_WIDTH) / 2) + center;
}

extern "C" uint32_t OTRCalculateCenterOfAreaFromLeftEdge(int32_t center) {
    return ((OTRGetDimensionFromLeftEdge(0) - SCREEN_WIDTH) / 2) + center;
}

// Gets the width of the current render target area
extern "C" uint32_t OTRGetGameRenderWidth() {
    return GetInterpreter()->mCurDimensions.width;
}

// Gets the height of the current render target area
extern "C" uint32_t OTRGetGameRenderHeight() {
    return GetInterpreter()->mCurDimensions.height;
}

extern "C" uint32_t OTRGetGameViewportWidth() {
    return GetInterpreter()->mGameWindowViewport.width;
}

extern "C" uint32_t OTRGetGameViewportHeight() {
    return GetInterpreter()->mGameWindowViewport.height;
}
