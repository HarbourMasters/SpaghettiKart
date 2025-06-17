#include "Engine.h"

#include "StringHelper.h"
#include "GameExtractor.h"
#include "ui/ImguiUI.h"
#include "libultraship/src/Context.h"
#include "resource/type/ResourceType.h"
#include "resource/importers/GenericArrayFactory.h"
#include "resource/importers/AudioBankFactory.h"
#include "resource/importers/AudioSampleFactory.h"
#include "resource/importers/AudioSequenceFactory.h"
#include "resource/importers/Vec3fFactory.h"
#include "resource/importers/Vec3sFactory.h"
#include "resource/importers/KartAIFactory.h"
#include "resource/importers/CourseVtxFactory.h"
#include "resource/importers/TrackSectionsFactory.h"
#include "resource/importers/TrackWaypointFactory.h"
#include "resource/importers/ActorSpawnDataFactory.h"
#include "resource/importers/UnkActorSpawnDataFactory.h"
#include "resource/importers/ArrayFactory.h"
#include "resource/importers/MinimapFactory.h"
#include <Fonts.h>
#include "window/gui/resource/Font.h"
#include "window/gui/resource/FontFactory.h"
#include "SpaghettiGui.h"

#include "port/interpolation/FrameInterpolation.h"
#include <graphic/Fast3D/Fast3dWindow.h>
#include <graphic/Fast3D/interpreter.h>
// #include <Fast3D/gfx_rendering_api.h>
#include <SDL2/SDL.h>

#include <utility>

#ifdef __SWITCH__
#include <port/switch/SwitchImpl.h>
#endif

extern "C" {
bool prevAltAssets = false;
float gInterpolationStep = 0.0f;
#include <macros.h>
#include <DisplayListFactory.h>
#include <TextureFactory.h>
#include <MatrixFactory.h>
#include <BlobFactory.h>
#include <VertexFactory.h>
#include <LightFactory.h>
#include "audio/internal.h"
#include "audio/GameAudio.h"
}

Fast::Interpreter* GetInterpreter() {
    return static_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow())
        ->GetInterpreterWeak()
        .lock()
        .get();
}

GameEngine* GameEngine::Instance;

GameEngine::GameEngine() {

    const std::string main_path = Ship::Context::GetPathRelativeToAppDirectory("mk64.o2r");
    const std::string assets_path = Ship::Context::LocateFileAcrossAppDirs("spaghetti.o2r");

    std::vector<std::string> archiveFiles;

#ifdef __SWITCH__
    Ship::Switch::Init(Ship::PreInitPhase);
    Ship::Switch::Init(Ship::PostInitPhase);
#endif

#ifdef _WIN32
    AllocConsole();
#endif

    if (std::filesystem::exists(main_path)) {
        archiveFiles.push_back(main_path);
    } else {
        if (ShowYesNoBox("No O2R Files", "No O2R files found. Generate one now?") == IDYES) {
            if (!GenAssetFile()) {
                ShowMessage("Error", "An error occured, no O2R file was generated.\n\nExiting...");
                exit(1);
            } else {
                archiveFiles.push_back(main_path);
            }
        } else {
            exit(1);
        }
    }

    if (std::filesystem::exists(assets_path)) {
        archiveFiles.push_back(assets_path);
    }

    if (const std::string patches_path = Ship::Context::GetPathRelativeToAppDirectory("mods");
        !patches_path.empty() && std::filesystem::exists(patches_path)) {
        if (std::filesystem::is_directory(patches_path)) {
            for (const auto& p : std::filesystem::recursive_directory_iterator(patches_path)) {
                auto ext = p.path().extension().string();
                if (StringHelper::IEquals(ext, ".zip") || StringHelper::IEquals(ext, ".o2r")) {
                    archiveFiles.push_back(p.path().generic_string());
                }
            }
        }
    }

    this->context = Ship::Context::CreateUninitializedInstance("Spaghetti Kart", "spaghettify", "spaghettify.cfg.json");

    this->context->InitConfiguration();    // without this line InitConsoleVariables fails at Config::Reload()
    this->context->InitConsoleVariables(); // without this line the controldeck constructor failes in
                                           // ShipDeviceIndexMappingManager::UpdateControllerNamesFromConfig()

    auto controlDeck = std::make_shared<LUS::ControlDeck>();

    this->context->InitResourceManager(archiveFiles, {}, 3); // without this line InitWindow fails in Gui::Init()
    this->context->InitConsole(); // without this line the GuiWindow constructor fails in ConsoleWindow::InitElement()

    auto gui = std::make_shared<Ship::SpaghettiGui>(std::vector<std::shared_ptr<Ship::GuiWindow>>({}));
    auto wnd = std::make_shared<Fast::Fast3dWindow>(gui);

    // auto wnd = std::make_shared<Fast::Fast3dWindow>(std::vector<std::shared_ptr<Ship::GuiWindow>>({}));
    // auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());

    this->context->Init(archiveFiles, {}, 3, { 26800, 512, 1100 }, wnd, controlDeck);

#ifndef __SWITCH__
    Ship::Context::GetInstance()->GetLogger()->set_level(
        (spdlog::level::level_enum) CVarGetInteger("gDeveloperTools.LogLevel", 1));
    Ship::Context::GetInstance()->GetLogger()->set_pattern("[%H:%M:%S.%e] [%s:%#] [%l] %v");
#endif

    wnd->SetRendererUCode(ucode_f3dex);
    this->context->InitGfxDebugger();

    auto loader = context->GetResourceManager()->GetResourceLoader();
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
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryTextureV0>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(Fast::ResourceType::Texture), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryTextureV1>(), RESOURCE_FORMAT_BINARY,
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
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryKartAIV0>(), RESOURCE_FORMAT_BINARY,
                                    "KartAI", static_cast<uint32_t>(MK64::ResourceType::KartAI), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryCourseVtxV0>(), RESOURCE_FORMAT_BINARY,
                                    "CourseVtx", static_cast<uint32_t>(MK64::ResourceType::CourseVertex), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackSectionsV0>(),
                                    RESOURCE_FORMAT_BINARY, "TrackSections",
                                    static_cast<uint32_t>(MK64::ResourceType::TrackSection), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryXMLTrackSectionsV0>(), RESOURCE_FORMAT_XML,
                                    "TrackSections", static_cast<uint32_t>(MK64::ResourceType::TrackSection), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryBinaryTrackWaypointsV0>(),
                                    RESOURCE_FORMAT_BINARY, "Waypoints",
                                    static_cast<uint32_t>(MK64::ResourceType::Waypoints), 0);
    loader->RegisterResourceFactory(std::make_shared<MK64::ResourceFactoryXMLTrackWaypointsV0>(), RESOURCE_FORMAT_XML,
                                    "Paths", static_cast<uint32_t>(MK64::ResourceType::Waypoints), 0);
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
        exit(1);
    }

    auto game = extractor->ValidateChecksum();
    if (!game.has_value()) {
        ShowMessage("Unsupported ROM",
                    "The provided ROM is not supported.\n\nCheck the readme for a list of supported versions.");
        exit(1);
    }

    ShowMessage(("Found " + game.value()).c_str(),
                "The extraction process will now begin.\n\nThis may take a few minutes.", SDL_MESSAGEBOX_INFORMATION);

    return extractor->GenerateOTR();
}

uint32_t GameEngine::GetInterpolationFPS() {
    if (CVarGetInteger("gMatchRefreshRate", 0)) {
        return Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();

    } else if (CVarGetInteger("gVsyncEnabled", 1) ||
               !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync()) {
        return std::min<uint32_t>(Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate(),
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
    instance->AudioInit();
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
    GameUI::Destroy();
    delete GameEngine::Instance;
    GameEngine::Instance = nullptr;
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
}

// void GameEngine::ProcessFrame(void (*run_one_game_iter)()) const {
//     //this->context->GetWindow()->MainLoop(run_one_game_iter);
//     Instance->context->GetWindow()->MainLoop(run_one_game_iter);
// }

void GameEngine::RunCommands(Gfx* Commands, const std::vector<std::unordered_map<Mtx*, MtxF>>& mtx_replacements) {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());

    if (wnd == nullptr) {
        return;
    }

    auto interpreter = wnd->GetInterpreterWeak().lock().get();

    // Process window events for resize, mouse, keyboard events
    wnd->HandleEvents();

    interpreter->mInterpolationIndex = 0;

    for (const auto& m : mtx_replacements) {
        wnd->DrawAndRunGraphicsCommands(Commands, m);
        interpreter->mInterpolationIndex++;
    }

    bool curAltAssets = CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0);
    if (prevAltAssets != curAltAssets) {
        prevAltAssets = curAltAssets;
        Ship::Context::GetInstance()->GetResourceManager()->SetAltAssetsEnabled(curAltAssets);
        gfx_texture_cache_clear();
    }
}

void GameEngine::ProcessGfxCommands(Gfx* commands) {
    std::vector<std::unordered_map<Mtx*, MtxF>> mtx_replacements;
    int target_fps = ImGui::GetIO().Framerate; // GameEngine::Instance->GetInterpolationFPS();
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

    while (time + original_fps <= next_original_frame) {
        time += original_fps;
        if (time != next_original_frame) {
            mtx_replacements.push_back(FrameInterpolation_Interpolate((float) time / next_original_frame));
        } else {
            mtx_replacements.emplace_back();
        }
    }
    // printf("mtxf size: %d\n", mtx_replacements.size());

    time -= fps;

    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());
    if (wnd != nullptr) {
        // wnd->SetTargetFps(GetInterpolationFPS());
        wnd->SetTargetFps(-1);
        wnd->SetMaximumFrameLatency(1);
    }
    RunCommands(commands, mtx_replacements);

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

        s16 audio_buffer[SAMPLES_PER_FRAME];
        for (int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
            create_next_audio_buffer(audio_buffer + i * (num_audio_samples * 2), num_audio_samples);
        }

        AudioPlayerPlayFrame((u8*) audio_buffer, 2 * num_audio_samples * 4);

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
    const auto resourceMgr = Ship::Context::GetInstance()->GetResourceManager();
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
    if (fontPath == "") {
        ImFontConfig fontCfg = ImFontConfig();
        fontCfg.OversampleH = fontCfg.OversampleV = 1;
        fontCfg.PixelSnapH = true;
        fontCfg.SizePixels = size;
        font = mImGuiIo->Fonts->AddFontDefault(&fontCfg);
    } else {
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->Type = static_cast<uint32_t>(RESOURCE_TYPE_FONT);
        initData->ResourceVersion = 0;
        initData->Path = fontPath;
        std::shared_ptr<Ship::Font> fontData = std::static_pointer_cast<Ship::Font>(
            Ship::Context::GetInstance()->GetResourceManager()->LoadResource(fontPath, false, initData));
        char* fontDataPtr = (char*) malloc(fontData->DataSize);
        memcpy(fontDataPtr, fontData->Data, fontData->DataSize);
        font = mImGuiIo->Fonts->AddFontFromMemoryTTF(fontDataPtr, fontData->DataSize, size);
    }
    // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    float iconFontSize = size * 2.0f / 3.0f;
    static const ImWchar sIconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    mImGuiIo->Fonts->AddFontFromMemoryCompressedBase85TTF(fontawesome_compressed_data_base85, iconFontSize,
                                                          &iconsConfig, sIconsRanges);
    return font;
}

// End

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

static const char* sOtrSignature = "__OTR__";

extern "C" bool GameEngine_OTRSigCheck(const char* data) {
    return strncmp(data, sOtrSignature, strlen(sOtrSignature)) == 0;
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