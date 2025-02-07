#include "ImguiUI.h"
#include "UIWidgets.h"
#include "ResolutionEditor.h"
#include "GameInfoWindow.h"
#include "MultiplayerWindow.h"
#include "FreecamWindow.h"

#include <spdlog/spdlog.h>
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "libultraship/src/Context.h"

#include <imgui_internal.h>
#include <libultraship/libultraship.h>
#include <Fast3D/gfx_pc.h>
#include "port/Engine.h"
#include "PortMenu.h"

extern "C" {
    extern s32 gGamestateNext;
    extern s32 gMenuSelection;
    #include "audio/external.h"
    #include "defines.h"
}

namespace GameUI {
std::shared_ptr<GameMenuBar> mGameMenuBar;
std::shared_ptr<PortMenu> mPortMenu;
std::shared_ptr<Ship::GuiWindow> mConsoleWindow;
std::shared_ptr<Ship::GuiWindow> mStatsWindow;
std::shared_ptr<Ship::GuiWindow> mInputEditorWindow;
std::shared_ptr<Ship::GuiWindow> mGfxDebuggerWindow;
std::shared_ptr<Ship::GuiWindow> mGameInfoWindow;
std::shared_ptr<Ship::GuiWindow> mMultiplayerWindow;

void SetupGuiElements() {
    auto gui = Ship::Context::GetInstance()->GetWindow()->GetGui();

    mGameMenuBar = std::make_shared<GameMenuBar>("gOpenMenuBar", CVarGetInteger("gOpenMenuBar", 0));
    gui->SetMenuBar(mGameMenuBar);

    mPortMenu = std::make_shared<PortMenu>("gOpenMenu", "Port Menu");
    gui->SetMenu(mPortMenu);

    mMultiplayerWindow = gui->GetGuiWindow("Multiplayer");
    if (mMultiplayerWindow == nullptr) {
        SPDLOG_ERROR("Could not find multiplayer window");
    }

    mGameInfoWindow = gui->GetGuiWindow("GameInfo");
    if (mGameInfoWindow == nullptr) {
        SPDLOG_ERROR("Could not find game info window");
    }

    mStatsWindow = gui->GetGuiWindow("Stats");
    if (mStatsWindow == nullptr) {
        SPDLOG_ERROR("Could not find stats window");
    }

    mConsoleWindow = gui->GetGuiWindow("Console");
    if (mConsoleWindow == nullptr) {
        SPDLOG_ERROR("Could not find console window");
    }

    mInputEditorWindow = gui->GetGuiWindow("Input Editor");
    if (mInputEditorWindow == nullptr) {
        SPDLOG_ERROR("Could not find input editor window");
        return;
    }

    mGfxDebuggerWindow = gui->GetGuiWindow("GfxDebuggerWindow");
    if (mGfxDebuggerWindow == nullptr) {
        SPDLOG_ERROR("Could not find input GfxDebuggerWindow");
    }

    mGameInfoWindow = std::make_shared<GameInfo::GameInfoWindow>("gGameInfoEnabled", "Game Info");
    gui->AddGuiWindow(mGameInfoWindow);
}

void Destroy() {
    mGameInfoWindow = nullptr;
    mConsoleWindow = nullptr;
    mStatsWindow = nullptr;
    mInputEditorWindow = nullptr;
}

std::string GetWindowButtonText(const char* text, bool menuOpen) {
    char buttonText[100] = "";
    if (menuOpen) {
        strcat(buttonText, ICON_FA_CHEVRON_RIGHT " ");
    }
    strcat(buttonText, text);
    if (!menuOpen) {
        strcat(buttonText, "  ");
    }
    return buttonText;
}
} // namespace GameUI

static const char* filters[3] = {
#ifdef __WIIU__
    "",
#else
    "Three-Point",
#endif
    "Linear", "None"
};

void DrawSettingsMenu() {
//    if (UIWidgets::BeginMenu("Settings")) {
//         if (UIWidgets::BeginMenu("Audio")) {
//             UIWidgets::CVarSliderFloat("Master Volume", "gGameMasterVolume", 0.0f, 1.0f, 1.0f, {
//                 .format = "%.0f%%",
//                 .isPercentage = true,
//             });
//             if (UIWidgets::CVarSliderFloat("Main Music Volume", "gMainMusicVolume", 0.0f, 1.0f, 1.0f,
//             {
//                 .format = "%.0f%%",
//                 .isPercentage = true,
//             })) {
//                 audio_set_player_volume(SEQ_PLAYER_LEVEL, CVarGetFloat("gMainMusicVolume", 1.0f));
//             }
//             if (UIWidgets::CVarSliderFloat("Sound Effects Volume", "gSFXMusicVolume",
//             0.0f, 1.0f, 1.0f, {
//                 .format = "%.0f%%",
//                 .isPercentage = true,
//             })) {
//                 audio_set_player_volume(SEQ_PLAYER_SFX, CVarGetFloat("gSFXMusicVolume", 1.0f));
//             }
//             if (UIWidgets::CVarSliderFloat("Environment Volume", "gEnvironmentVolume",
//             0.0f, 1.0f, 1.0f, {
//                 .format = "%.0f%%",
//                 .isPercentage = true,
//             })) {
//                 audio_set_player_volume(SEQ_PLAYER_ENV, CVarGetFloat("gEnvironmentVolume", 1.0f));
//             }
//
//             static std::unordered_map<Ship::AudioBackend, const char*> audioBackendNames = {
//                     { Ship::AudioBackend::WASAPI, "Windows Audio Session API" },
//                     { Ship::AudioBackend::SDL, "SDL" },
//             };
//
//             ImGui::Text("Audio API (Needs reload)");
//             auto currentAudioBackend = Ship::Context::GetInstance()->GetAudio()->GetCurrentAudioBackend();
//
//             if (Ship::Context::GetInstance()->GetAudio()->GetAvailableAudioBackends()->size() <= 1) {
//                 UIWidgets::DisableComponent(ImGui::GetStyle().Alpha * 0.5f);
//             }
//             if (ImGui::BeginCombo("##AApi", audioBackendNames[currentAudioBackend])) {
//                 for (uint8_t i = 0; i <
//                 Ship::Context::GetInstance()->GetAudio()->GetAvailableAudioBackends()->size(); i++) {
//                     auto backend = Ship::Context::GetInstance()->GetAudio()->GetAvailableAudioBackends()->data()[i];
//                     if (ImGui::Selectable(audioBackendNames[backend], backend == currentAudioBackend)) {
//                         Ship::Context::GetInstance()->GetAudio()->SetCurrentAudioBackend(backend);
//                     }
//                 }
//                 ImGui::EndCombo();
//             }
//             if (Ship::Context::GetInstance()->GetAudio()->GetAvailableAudioBackends()->size() <= 1) {
//                 UIWidgets::ReEnableComponent("");
//             }
//
//             ImGui::EndMenu();
//         }
//
//        UIWidgets::Spacer(0);
//
//        if (UIWidgets::BeginMenu("Controller")) {
//            UIWidgets::WindowButton("Controller Mapping", "gInputEditorWindow", GameUI::mInputEditorWindow);
//
//            UIWidgets::Spacer(0);
//
//#ifndef __SWITCH__
//            UIWidgets::CVarCheckbox(
//                "Menubar Controller Navigation", "gControlNav",
//                { .tooltip = "Allows controller navigation of the SOH menu bar (Settings, Enhancements,...)\nCAUTION: "
//                             "This will disable game inputs while the menubar is visible.\n\nD-pad to move between "
//                             "items, A to select, and X to grab focus on the menu bar" });
//#endif
//            UIWidgets::CVarCheckbox("Show Inputs", "gInputEnabled",
//                                    { .tooltip = "Shows currently pressed inputs on the bottom right of the screen" });
//            if (CVarGetInteger("gInputEnabled", 0)) {
//                UIWidgets::CVarSliderFloat(
//                    "Input Scale", "gInputScale", 1.0f, 3.0f, 1.0f,
//                    {
//                        .tooltip = "Sets the on screen size of the displayed inputs from the Show Inputs setting",
//                        .format = "%.1fx",
//                    });
//            }
//
//            ImGui::EndMenu();
//        }
//
//        ImGui::EndMenu();
//    }
//
//    ImGui::SetCursorPosY(0.0f);
//    if (UIWidgets::BeginMenu("Graphics")) {
//        UIWidgets::WindowButton("Resolution Editor", "gAdvancedResolutionEditorEnabled",
//                                GameUI::mAdvancedResolutionSettingsWindow);
//
//        UIWidgets::Spacer(0);
//
//        // Previously was running every frame, and nothing was setting it? Maybe a bad copy/paste?
//        // Ship::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(CVarGetFloat("gInternalResolution", 1));
//        // UIWidgets::Tooltip("Multiplies your output resolution by the value inputted, as a more intensive but
//        // effective form of anti-aliasing");
//#ifndef __WIIU__
//        if (UIWidgets::CVarSliderInt(
//                "MSAA: %d", "gMSAAValue", 1, 8, 1,
//                { .tooltip =
//                      "Activates multi-sample anti-aliasing when above 1x up to 8x for 8 samples for every pixel" })) {
//            Ship::Context::GetInstance()->GetWindow()->SetMsaaLevel(CVarGetInteger("gMSAAValue", 1));
//        }
//#endif
//
//        { // FPS Slider
//            const int minFps = 30;
//            static int maxFps;
//            if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
//                Ship::WindowBackend::FAST3D_DXGI_DX11) {
//                maxFps = 360;
//            } else {
//                maxFps = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
//            }
//            int currentFps = 0;
//#ifdef __WIIU__
//            UIWidgets::Spacer(0);
//            // only support divisors of 60 on the Wii U
//            if (currentFps > 60) {
//                currentFps = 60;
//            } else {
//                currentFps = 60 / (60 / currentFps);
//            }
//
//            int fpsSlider = 1;
//            if (currentFps == 30) {
//                ImGui::Text("FPS: Original (30)");
//            } else {
//                ImGui::Text("FPS: %d", currentFps);
//                if (currentFps == 30) {
//                    fpsSlider = 2;
//                } else { // currentFps == 60
//                    fpsSlider = 3;
//                }
//            }
//            if (CVarGetInteger("gMatchRefreshRate", 0)) {
//                UIWidgets::DisableComponent(ImGui::GetStyle().Alpha * 0.5f);
//            }
//
//            if (ImGui::Button(" - ##WiiUFPS")) {
//                fpsSlider--;
//            }
//            ImGui::SameLine();
//            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7.0f);
//
//            UIWidgets::Spacer(0);
//
//            ImGui::PushItemWidth(std::min((ImGui::GetContentRegionAvail().x - 60.0f), 260.0f));
//            ImGui::SliderInt("##WiiUFPSSlider", &fpsSlider, 1, 3, "", ImGuiSliderFlags_AlwaysClamp);
//            ImGui::PopItemWidth();
//
//            ImGui::SameLine();
//            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7.0f);
//            if (ImGui::Button(" + ##WiiUFPS")) {
//                fpsSlider++;
//            }
//
//            if (CVarGetInteger("gMatchRefreshRate", 0)) {
//                UIWidgets::ReEnableComponent("");
//            }
//            if (fpsSlider > 3) {
//                fpsSlider = 3;
//            } else if (fpsSlider < 1) {
//                fpsSlider = 1;
//            }
//
//            if (fpsSlider == 1) {
//                currentFps = 20;
//            } else if (fpsSlider == 2) {
//                currentFps = 30;
//            } else if (fpsSlider == 3) {
//                currentFps = 60;
//            }
//            CVarSetInteger("gInterpolationFPS", currentFps);
//            Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
//#else
//            bool matchingRefreshRate =
//                CVarGetInteger("gMatchRefreshRate", 0) &&
//                Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() != Ship::WindowBackend::FAST3D_DXGI_DX11;
//            UIWidgets::CVarSliderInt((currentFps == 20) ? "FPS: Original (20)" : "FPS: %d", "gInterpolationFPS", minFps,
//                                     maxFps, 1, { .disabled = matchingRefreshRate });
//#endif
//            if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
//                Ship::WindowBackend::FAST3D_DXGI_DX11) {
//                UIWidgets::Tooltip(
//                    "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. "
//                    "This is purely "
//                    "visual and does not impact game logic, execution of glitches etc.\n\n"
//                    "A higher target FPS than your monitor's refresh rate will waste resources, and might give a worse "
//                    "result.");
//            } else {
//                UIWidgets::Tooltip(
//                    "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. This is purely "
//                    "visual and does not impact game logic, execution of glitches etc.");
//            }
//        } // END FPS Slider
//
//        if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
//            UIWidgets::Spacer(0);
//            if (ImGui::Button("Match Refresh Rate")) {
//                int hz = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
//                if (hz >= 30 && hz <= 360) {
//                    CVarSetInteger("gInterpolationFPS", hz);
//                    Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
//                }
//            }
//        } else {
//            UIWidgets::PaddedEnhancementCheckbox("Match Refresh Rate", "gMatchRefreshRate", true, false);
//        }
//
//        UIWidgets::Tooltip("Matches interpolation value to the current game's window refresh rate");
//
//        if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
//            UIWidgets::PaddedEnhancementSliderInt(
//                CVarGetInteger("gExtraLatencyThreshold", 0) == 0 ? "Jitter fix: Off" : "Jitter fix: >= %d FPS",
//                "##ExtraLatencyThreshold", "gExtraLatencyThreshold", 0, 360, "", 0, true, true, false);
//            UIWidgets::Tooltip("When Interpolation FPS setting is at least this threshold, add one frame of input lag "
//                               "(e.g. 16.6 ms for 60 FPS) in order to avoid jitter. This setting allows the CPU to "
//                               "work on one frame while GPU works on the previous frame.\nThis setting should be used "
//                               "when your computer is too slow to do CPU + GPU work in time.");
//        }
//
//        UIWidgets::PaddedSeparator(true, true, 3.0f, 3.0f);
//
//        static std::unordered_map<Ship::WindowBackend, const char*> windowBackendNames = {
//            { Ship::WindowBackend::FAST3D_DXGI_DX11, "DirectX" },
//            { Ship::WindowBackend::FAST3D_SDL_OPENGL, "OpenGL" },
//            { Ship::WindowBackend::FAST3D_SDL_METAL, "Metal" },
//        };
//
//        ImGui::Text("Renderer API (Needs reload)");
//        Ship::WindowBackend runningWindowBackend = Ship::Context::GetInstance()->GetWindow()->GetWindowBackend();
//        Ship::WindowBackend configWindowBackend;
//        int configWindowBackendId = Ship::Context::GetInstance()->GetConfig()->GetInt("Window.Backend.Id", -1);
//        if (Ship::Context::GetInstance()->GetWindow()->IsAvailableWindowBackend(configWindowBackendId)) {
//            configWindowBackend = static_cast<Ship::WindowBackend>(configWindowBackendId);
//        } else {
//            configWindowBackend = runningWindowBackend;
//        }
//
//        if (Ship::Context::GetInstance()->GetWindow()->GetAvailableWindowBackends()->size() <= 1) {
//            UIWidgets::DisableComponent(ImGui::GetStyle().Alpha * 0.5f);
//        }
//        if (ImGui::BeginCombo("##RApi", windowBackendNames[configWindowBackend])) {
//            for (size_t i = 0; i < Ship::Context::GetInstance()->GetWindow()->GetAvailableWindowBackends()->size();
//                 i++) {
//                auto backend = Ship::Context::GetInstance()->GetWindow()->GetAvailableWindowBackends()->data()[i];
//                if (ImGui::Selectable(windowBackendNames[backend], backend == configWindowBackend)) {
//                    Ship::Context::GetInstance()->GetConfig()->SetInt("Window.Backend.Id", static_cast<int>(backend));
//                    Ship::Context::GetInstance()->GetConfig()->SetString("Window.Backend.Name",
//                                                                         windowBackendNames[backend]);
//                    Ship::Context::GetInstance()->GetConfig()->Save();
//                }
//            }
//            ImGui::EndCombo();
//        }
//        if (Ship::Context::GetInstance()->GetWindow()->GetAvailableWindowBackends()->size() <= 1) {
//            UIWidgets::ReEnableComponent("");
//        }
//
//        if (Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync()) {
//            UIWidgets::PaddedEnhancementCheckbox("Enable Vsync", "gVsyncEnabled", true, false);
//        }
//
//        if (Ship::Context::GetInstance()->GetWindow()->SupportsWindowedFullscreen()) {
//            UIWidgets::PaddedEnhancementCheckbox("Windowed fullscreen", "gSdlWindowedFullscreen", true, false);
//        }
//
//        if (Ship::Context::GetInstance()->GetWindow()->GetGui()->SupportsViewports()) {
//            UIWidgets::PaddedEnhancementCheckbox("Allow multi-windows", "gEnableMultiViewports", true, false, false, "",
//                                                 UIWidgets::CheckboxGraphics::Cross, true);
//            UIWidgets::Tooltip("Allows windows to be able to be dragged off of the main game window. Requires a reload "
//                               "to take effect.");
//        }
//
//        // If more filters are added to LUS, make sure to add them to the filters list here
//        ImGui::Text("Texture Filter (Needs reload)");
//
//        UIWidgets::EnhancementCombobox("gTextureFilter", filters, 0);
//
//        UIWidgets::Spacer(0);
//
//        Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGameOverlay()->DrawSettings();
//
//        ImGui::EndMenu();
//    }
}

void DrawMenuBarIcon() {
    static bool gameIconLoaded = false;
    if (!gameIconLoaded) {
        // Ship::Context::GetInstance()->GetWindow()->GetGui()->LoadTexture("Game_Icon",
        // "textures/icons/gIcon.png");
        gameIconLoaded = false;
    }

    if (Ship::Context::GetInstance()->GetWindow()->GetGui()->GetTextureByName("Game_Icon")) {
#ifdef __SWITCH__
        ImVec2 iconSize = ImVec2(20.0f, 20.0f);
        float posScale = 1.0f;
#elif defined(__WIIU__)
        ImVec2 iconSize = ImVec2(16.0f * 2, 16.0f * 2);
        float posScale = 2.0f;
#else
        ImVec2 iconSize = ImVec2(20.0f, 20.0f);
        float posScale = 1.5f;
#endif
        ImGui::SetCursorPos(ImVec2(5, 2.5f) * posScale);
        ImGui::Image(Ship::Context::GetInstance()->GetWindow()->GetGui()->GetTextureByName("Game_Icon"), iconSize);
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(25, 0) * posScale);
    }
}

void DrawGameMenu() {

}

void DrawEnhancementsMenu() {
    if (UIWidgets::BeginMenu("Enhancements")) {


        if (UIWidgets::BeginMenu("Gameplay")) {

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void DrawCheatsMenu() {

}

const char* debugInfoPages[6] = {
    "Object", "Check Surface", "Map", "Stage", "Effect", "Enemy",
};

void DrawDebugMenu() {

}

void GameMenuBar::DrawElement() {
    if (ImGui::BeginMenuBar()) {
        DrawMenuBarIcon();

        DrawGameMenu();

        ImGui::SetCursorPosY(0.0f);

        //DrawSettingsMenu();

        ImGui::SetCursorPosY(0.0f);

        //DrawEnhancementsMenu();

        ImGui::SetCursorPosY(0.0f);

        //DrawCheatsMenu();

        ImGui::SetCursorPosY(0.0f);

        //DrawDebugMenu();

        ImGui::EndMenuBar();
    }
}
