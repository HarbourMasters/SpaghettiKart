#include "PortMenu.h"
#include "UIWidgets.h"
#include "port/Game.h"
#include "window/gui/GuiMenuBar.h"
#include "window/gui/GuiElement.h"
#include <variant>
#include "StringHelper.h"
#include <spdlog/fmt/fmt.h>
#include <variant>
#include <tuple>
#include "ResolutionEditor.h"

#include "courses/Course.h"
#include "courses/KalimariDesert.h"
#include "courses/ToadsTurnpike.h"

#ifdef __SWITCH__
#include <port/switch/SwitchImpl.h>
#endif

extern "C" {
extern s32 gGamestateNext;
extern s32 gMenuSelection;
#include "audio/external.h"
#include "defines.h"
}

namespace GameUI {
extern std::shared_ptr<PortMenu> mPortMenu;

using namespace UIWidgets;

void PortMenu::AddSidebarEntry(std::string sectionName, std::string sidebarName, uint32_t columnCount) {
    assert(!sectionName.empty());
    assert(!sidebarName.empty());
    menuEntries.at(sectionName).sidebars.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
    menuEntries.at(sectionName).sidebarOrder.push_back(sidebarName);
}

WidgetInfo& PortMenu::AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType) {
    assert(!widgetName.empty());                        // Must be unique
    assert(menuEntries.contains(pathInfo.sectionName)); // Section/header must already exist
    assert(menuEntries.at(pathInfo.sectionName).sidebars.contains(pathInfo.sidebarName)); // Sidebar must already exist
    std::unordered_map<std::string, SidebarEntry>& sidebar = menuEntries.at(pathInfo.sectionName).sidebars;
    uint8_t column = pathInfo.column;
    if (sidebar.contains(pathInfo.sidebarName)) {
        while (sidebar.at(pathInfo.sidebarName).columnWidgets.size() < column + 1) {
            sidebar.at(pathInfo.sidebarName).columnWidgets.push_back({});
        }
    }
    SidebarEntry& entry = sidebar.at(pathInfo.sidebarName);
    entry.columnWidgets.at(column).push_back({ .name = widgetName, .type = widgetType });
    WidgetInfo& widget = entry.columnWidgets.at(column).back();
    switch (widgetType) {
        case WIDGET_CHECKBOX:
        case WIDGET_CVAR_CHECKBOX:
            widget.options = std::make_shared<CheckboxOptions>();
            break;
        case WIDGET_SLIDER_FLOAT:
        case WIDGET_CVAR_SLIDER_FLOAT:
            widget.options = std::make_shared<FloatSliderOptions>();
            break;
        case WIDGET_SLIDER_INT:
        case WIDGET_CVAR_SLIDER_INT:
            widget.options = std::make_shared<IntSliderOptions>();
            break;
        case WIDGET_COMBOBOX:
        case WIDGET_CVAR_COMBOBOX:
        case WIDGET_AUDIO_BACKEND:
        case WIDGET_VIDEO_BACKEND:
            widget.options = std::make_shared<ComboboxOptions>();
            break;
        case WIDGET_BUTTON:
            widget.options = std::make_shared<ButtonOptions>();
            break;
        case WIDGET_WINDOW_BUTTON:
            widget.options = std::make_shared<ButtonOptions>(ButtonOptions{ .size = Sizes::Inline });
            break;
        case WIDGET_COLOR_24:
        case WIDGET_COLOR_32:
            break;
        case WIDGET_SEARCH:
        case WIDGET_SEPARATOR:
        case WIDGET_SEPARATOR_TEXT:
        case WIDGET_TEXT:
        default:
            widget.options = std::make_shared<WidgetOptions>();
    }
    return widget;
}

void PortMenu::AddSettings() {
    // Add Settings menu
    AddMenuEntry("Settings", "gSettings.Menu.SettingsSidebarSection");
    // General Settings
    AddSidebarEntry("Settings", "General", 3);
    WidgetPath path = { "Settings", "General", SECTION_COLUMN_1 };

    static bool isFullscreen = false;
    AddWidget(path, "Toggle Fullscreen", WIDGET_CHECKBOX)
        .ValuePointer(&isFullscreen)
        .PreFunc([](WidgetInfo& info) { isFullscreen = Ship::Context::GetInstance()->GetWindow()->IsFullscreen(); })
        .Callback([](WidgetInfo& info) { Ship::Context::GetInstance()->GetWindow()->ToggleFullscreen(); })
        .Options(CheckboxOptions().Tooltip("Toggles Fullscreen On/Off."));

    AddWidget(path, "Startup Behaviour", WIDGET_CVAR_COMBOBOX)
        .CVar("gSkipIntro")
        .Options(ComboboxOptions()
            .ComboMap(introBehaviourOptions)
            .Tooltip("Select which scene or menu the game launch to."));

    AddWidget(path, "Menu Theme", WIDGET_CVAR_COMBOBOX)
        .CVar("gSettings.Menu.Theme")
        .Options(ComboboxOptions()
                     .Tooltip("Changes the Theme of the Menu Widgets.")
                     .ComboMap(menuThemeOptions)
                     .DefaultIndex(Colors::LightBlue));
    AddWidget(path, "Menu Extent", WIDGET_CVAR_COMBOBOX)
        .CVar("gSettings.Menu.Extent")
        .Options(ComboboxOptions()
                     .Tooltip("Changes the extent of the onscreen menu")
                     .ComboMap(menuExtentOptions)
                     .DefaultIndex(MenuExtent::Condensed));

    AddWidget(path, "Menu Scale: %.0fx", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Menu.Scale")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gSettings.Menu.Extent", 0); })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the scale for the menu.")
                     .Min(1.0f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .Format("%.1f")
                     .Step(0.1f));
    AddWidget(path, "Controller pak screen", WIDGET_CVAR_CHECKBOX)
        .CVar("gControllerPakScreen")
        .Options(CheckboxOptions().Tooltip(
            "Enables the Controller Pak screen when starting the game."));
#if not defined(__SWITCH__) and not defined(__WIIU__)
    AddWidget(path, "Menu Controller Navigation", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_IMGUI_CONTROLLER_NAV)
        .Options(CheckboxOptions().Tooltip(
            "Allows controller navigation of the Spaghetti menu (Settings, Enhancements,...)\nCAUTION: "
            "This will disable game inputs while the menu is visible.\n\nD-pad to move between "
            "items, A to select, B to move up in scope."));
    AddWidget(path, "Cursor Always Visible", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.CursorVisibility")
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetForceCursorVisibility(
                CVarGetInteger("gSettings.CursorVisibility", 0));
        })
        .Options(CheckboxOptions().Tooltip("Makes the cursor always visible, even in full screen."));
#endif
    AddWidget(path, "Search In Sidebar", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SidebarSearch")
        .Callback([](WidgetInfo& info) {
            if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
                mPortMenu->InsertSidebarSearch();
            } else {
                mPortMenu->RemoveSidebarSearch();
            }
        })
        .Options(CheckboxOptions().Tooltip(
            "Displays the Search menu as a sidebar entry in Settings instead of in the header."));
    AddWidget(path, "Search Input Autofocus", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SearchAutofocus")
        .Options(CheckboxOptions().Tooltip(
            "Search input box gets autofocus when visible. Does not affect using other widgets."));
    AddWidget(path, "Alt Assets Tab hotkey", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Mods.AlternateAssetsHotkey")
        .Options(
            CheckboxOptions().Tooltip("Allows pressing the Tab key to toggle alternate assets").DefaultValue(true));
    AddWidget(path, "Open App Files Folder", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            std::string filesPath = Ship::Context::GetInstance()->GetAppDirectoryPath();
            SDL_OpenURL(std::string("file:///" + std::filesystem::absolute(filesPath).string()).c_str());
        })
        .Options(ButtonOptions().Tooltip("Opens the folder that contains the save and mods folders, etc."));

    // Audio Settings
    path.sidebarName = "Audio";
    AddSidebarEntry("Settings", "Audio", 3);
    AddWidget(path, "Master Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gGameMasterVolume")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the overall sound volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Main Music Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gMainMusicVolume")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the background music volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Sound Effects Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSFXMusicVolume")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the sound effects volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Environment Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnvironmentVolume")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the environment volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Audio API", WIDGET_AUDIO_BACKEND);

    // Graphics Settings
    static int32_t maxFps;
    const char* tooltip = "";
    if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
        maxFps = 360;
        tooltip = "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. This is "
                  "purely visual and does not impact game logic, execution of glitches etc.\n\nA higher target "
                  "FPS than your monitor's refresh rate will waste resources, and might give a worse result.";
    } else {
        maxFps = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
        tooltip = "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. This is "
                  "purely visual and does not impact game logic, execution of glitches etc.";
    }
    path.sidebarName = "Graphics";
    AddSidebarEntry("Settings", "Graphics", 3);
    AddWidget(path, "Renderer API (Needs reload)", WIDGET_VIDEO_BACKEND);

#ifndef __APPLE__
    AddWidget(path, "Internal Resolution: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar(CVAR_INTERNAL_RESOLUTION)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(
                CVarGetFloat(CVAR_INTERNAL_RESOLUTION, 1));
        })
        .PreFunc([](WidgetInfo& info) {
            if (mPortMenu->disabledMap.at(DISABLE_FOR_ADVANCED_RESOLUTION_ON).active &&
                mPortMenu->disabledMap.at(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_ADVANCED_RESOLUTION_ON);
                info.activeDisables.push_back(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON);
            } else if (mPortMenu->disabledMap.at(DISABLE_FOR_LOW_RES_MODE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_LOW_RES_MODE_ON);
            }
        })
        .Options(
            FloatSliderOptions()
                .Tooltip("Multiplies your output resolution by the value inputted, as a more intensive but effective "
                         "form of anti-aliasing.")
                .ShowButtons(false)
                .IsPercentage()
                .Format("")
                .Min(0.5f)
                .Max(4.0f));
#endif
#ifndef __WIIU__
    AddWidget(path, "Anti-aliasing (MSAA): %d", WIDGET_CVAR_SLIDER_INT)
        .CVar(CVAR_MSAA_VALUE)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetMsaaLevel(CVarGetInteger(CVAR_MSAA_VALUE, 1));
        })
        .Options(
            IntSliderOptions()
                .Tooltip("Activates MSAA (multi-sample anti-aliasing) from 2x up to 8x, to smooth the edges of "
                         "rendered geometry.\n"
                         "Higher sample count will result in smoother edges on models, but may reduce performance.")
                .Min(1)
                .Max(8)
                .DefaultValue(1));
#endif

    AddWidget(path, "Current FPS: %d", WIDGET_CVAR_SLIDER_INT)
        .CVar("gInterpolationFPS")
        .Callback([](WidgetInfo& info) {
            int32_t defaultValue = std::static_pointer_cast<IntSliderOptions>(info.options)->defaultValue;
            if (CVarGetInteger(info.cVar, defaultValue) == defaultValue) {
                info.name = "Current FPS: Original (%d)";
            } else {
                info.name = "Current FPS: %d";
            }
        })
        .PreFunc([](WidgetInfo& info) {
            if (mPortMenu->disabledMap.at(DISABLE_FOR_MATCH_REFRESH_RATE_ON).active)
                info.activeDisables.push_back(DISABLE_FOR_MATCH_REFRESH_RATE_ON);
        })
        .Options(IntSliderOptions().Tooltip(tooltip).Min(30).Max(maxFps).DefaultValue(30));
    AddWidget(path, "Match Refresh Rate", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            int hz = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
            if (hz >= 30 && hz <= 360) {
                CVarSetInteger("gInterpolationFPS", hz);
                Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
            }
        })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_NOT_DIRECTX).active; })
        .Options(ButtonOptions().Tooltip("Matches interpolation value to the current game's window refresh rate."));
    AddWidget(path, "Match Refresh Rate", WIDGET_CVAR_CHECKBOX)
        .CVar("gMatchRefreshRate")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_DIRECTX).active; })
        .Options(CheckboxOptions().Tooltip("Matches interpolation value to the current game's window refresh rate."));
    AddWidget(path, "Jitter fix : >= % d FPS", WIDGET_CVAR_SLIDER_INT)
        .CVar("gExtraLatencyThreshold")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_NOT_DIRECTX).active; })
        .Options(IntSliderOptions()
                     .Tooltip("When Interpolation FPS setting is at least this threshold, add one frame of input "
                              "lag (e.g. 16.6 ms for 60 FPS) in order to avoid jitter. This setting allows the "
                              "CPU to work on one frame while GPU works on the previous frame.\nThis setting "
                              "should be used when your computer is too slow to do CPU + GPU work in time.")
                     .Min(0)
                     .Max(360)
                     .DefaultValue(80));
    AddWidget(path, "Enable Vsync", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_VSYNC_ENABLED)
        .PreFunc([](WidgetInfo& info) { info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_NO_VSYNC).active; })
        .Options(CheckboxOptions().Tooltip("Enables Vsync."));
    AddWidget(path, "Windowed Fullscreen", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SDL_WINDOWED_FULLSCREEN)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_NO_WINDOWED_FULLSCREEN).active;
        })
        .Options(CheckboxOptions().Tooltip("Enables Windowed Fullscreen Mode."));
    AddWidget(path, "Allow multiple ImGui windows", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ENABLE_MULTI_VIEWPORTS)
        .PreFunc(
            [](WidgetInfo& info) { info.isHidden = mPortMenu->disabledMap.at(DISABLE_FOR_NO_MULTI_VIEWPORT).active; })
        .Options(CheckboxOptions()
                     .Tooltip("Allows multiple ImGui windows to be opened at once (Does not effect the game or the splitscreen modes). Requires a reload to take effect.")
                     .DefaultValue(true));
    AddWidget(path, "Texture Filter (Needs reload)", WIDGET_CVAR_COMBOBOX)
        .CVar(CVAR_TEXTURE_FILTER)
        .Options(ComboboxOptions().Tooltip("Sets the applied Texture Filtering.").ComboMap(textureFilteringMap));

    path.sidebarName = "Controls";
    AddSidebarEntry("Settings", "Controls", 1);
    AddWidget(path,
              "This interface can be a little daunting. Please bear with us as we work to improve the experience "
              "and address some known issues.\n"
              "\n"
              "At first glance, you may notice several input devices displayed below the 'Clear All' button. "
              "Some of these might be other controllers connected to your computer, while others may be "
              "duplicated controllers (a known issue). We recommend clicking on the box with the " ICON_FA_EYE
              " icon and the name of any disconnected or unused controllers to hide their inputs. Make sure the "
              "target controller remains visible.\n"
              "\n"
              "If you encounter issues connecting your controller or registering inputs, try closing Steam or "
              "any other external input software. Alternatively, test a different controller to determine if "
              "it's a compatibility issue.\n",
              WIDGET_TEXT);
    AddWidget(path, "Bindings", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Popout Bindings Window", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN)
        .WindowName("Input Editor")
        .Options(ButtonOptions().Tooltip("Enables the separate Bindings Window.").Size(Sizes::Inline));
}
int32_t motionBlurStrength;

void PortMenu::AddEnhancements() {
    AddMenuEntry("Enhancements", "gSettings.Menu.EnhancementsSidebarSection");
    WidgetPath path = { "Enhancements", "General", SECTION_COLUMN_1 };
    AddSidebarEntry("Enhancements", "General", 3);
    // UIWidgets::WindowButton("Multiplayer", "gMultiplayerWindowEnabled", GameUI::mMultiplayerWindow,
    //                         { .tooltip = "Shows the multiplayer window" });
    //     UIWidgets::WindowButton("Freecam", "gFreecam", GameUI::mFreecamWindow,
    //                             { .tooltip = "Allows you to fly around the course" });
    AddWidget(path, "No multiplayer feature cuts", WIDGET_CVAR_CHECKBOX)
        .CVar("gMultiplayerNoFeatureCuts")
        .Options(CheckboxOptions().Tooltip("Allows full train and jumbotron in multiplayer, etc."));
    AddWidget(path, "Widescreen portrait spacing", WIDGET_CVAR_CHECKBOX)
        .CVar("gBetterResultPortraits")
        .Options(CheckboxOptions().Tooltip("Alters result portrait spacing for better aesthetics on widescreen"));
    AddWidget(path, "No Level of Detail (LOD)", WIDGET_CVAR_CHECKBOX)
        .CVar("gDisableLod")
        .Options(CheckboxOptions().Tooltip(
            "Disable Level of Detail (LOD) to avoid models using lower poly versions at a distance"));
    AddWidget(path, "Disable Culling", WIDGET_CVAR_CHECKBOX)
        .CVar("gNoCulling")
        .Options(CheckboxOptions().Tooltip("Disable original culling of mk64"));
    AddWidget(path, "Disable Rubberbanding", WIDGET_CVAR_CHECKBOX)
        .CVar("gDisableRubberbanding")
        .Options(CheckboxOptions().Tooltip("Disable rubberbanding in the game."));
    AddWidget(path, "Far Frustrum", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gFarFrustrum")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gNoCulling", 0); })
        .Options(FloatSliderOptions()
                     .Min(0.0f)
                     .Max(10000.0f)
                     .DefaultValue(10000.0f)
                     .Tooltip("Say how Far the Frustrum are when 'Disable Culling' are enable")
                     .Step(10.0f));
    AddWidget(path, "Enable Custom CC", WIDGET_CVAR_CHECKBOX).CVar("gEnableCustomCC");
    AddWidget(path, "Custom CC", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gCustomCC")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gEnableCustomCC", 0); })
        .Options(FloatSliderOptions().Min(0.0f).Max(1000.0f).DefaultValue(150.0f).Step(10.0f));

    AddWidget(path, "Enable Digital Speedometer", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnableDigitalSpeedometer")
        .Options(CheckboxOptions().Tooltip("Welcome to the modern era"));

    AddWidget(path, "Harder CPU", WIDGET_CVAR_CHECKBOX).CVar("gHarderCPU");

    AddWidget(path, "Show Spaghetti version", WIDGET_CVAR_CHECKBOX)
        .CVar("gShowSpaghettiVersion")
        .Options(CheckboxOptions().Tooltip("Show the Spaghetti Kart version on the Mario Kart menu").DefaultValue(true));

    AddWidget(path, "Enable Look Behind Camera", WIDGET_CVAR_CHECKBOX)
        .CVar("gLookBehind")
        .Options(CheckboxOptions().Tooltip("Press C-Left to look behind you"));

    AddRulesets();

    path = { "Enhancements", "Cheats", SECTION_COLUMN_1 };
    AddSidebarEntry("Enhancements", "Cheats", 3);
    AddWidget(path, "Moon Jump", WIDGET_CVAR_CHECKBOX).CVar("gEnableMoonJump");
    AddWidget(path, "Disable Wall Collision", WIDGET_CVAR_CHECKBOX)
        .CVar("gNoWallColision")
        .Options(CheckboxOptions().Tooltip("Disable wall collision."));
    AddWidget(path, "Min Height", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gMinHeight")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gNoWallColision", 0); })
        .Options(FloatSliderOptions().Min(-50.0f).Max(50.0f).DefaultValue(0.0f).Tooltip(
            "When Disable Wall Collision are enable what is the minimal height you can get."));

#if not defined(__SWITCH__) and not defined(__WIIU__)
    path = { "Enhancements", "HM64 Lab", SECTION_COLUMN_1 };
    AddSidebarEntry("Enhancements", "HM64 Lab", 4);
    AddWidget(path, "Work in progress.", WIDGET_TEXT);
    AddWidget(path, "Enable HM64 Labs", WIDGET_CVAR_CHECKBOX)
        .CVar("gEditorEnabled")
        .Callback([](WidgetInfo& info) {
            if (CVarGetInteger("gEditorEnabled", false) == false) {
                gIsEditorPaused = false;
            }
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Tools")->ToggleVisibility();
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Scene Explorer")->ToggleVisibility();
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Content Browser")->ToggleVisibility();
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Track Properties")->ToggleVisibility();
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Properties")->ToggleVisibility();
        })
        .Options(UIWidgets::CheckboxOptions({ { .tooltip = "Edit the universe!" } }));
#endif
}

#ifdef __SWITCH__
static const std::unordered_map<int32_t, const char*> switchCPUProfiles = {
    { Ship::SwitchProfiles::MAXIMUM, "Maximum Performance" },
    { Ship::SwitchProfiles::HIGH, "High Performance" },
    { Ship::SwitchProfiles::BOOST, "Boost Performance" },
    { Ship::SwitchProfiles::STOCK, "Stock Performance" },
    { Ship::SwitchProfiles::POWERSAVINGM1, "Powersaving Mode 1" },
    { Ship::SwitchProfiles::POWERSAVINGM2, "Powersaving Mode 2" },
    { Ship::SwitchProfiles::POWERSAVINGM3, "Powersaving Mode 3" }
};
#endif

void PortMenu::AddRulesets() {
    WidgetPath path = { "Enhancements", "Rulesets", SECTION_COLUMN_1 };
    AddSidebarEntry("Enhancements", "Rulesets", 3);

    // Requires more testing
    // AddWidget(path, "Number of Laps", WIDGET_CVAR_SLIDER_INT)
    //     .CVar("gNumLaps")
    //     .Options(UIWidgets::IntSliderOptions().Min().Max(20).Step(1).DefaultValue(3));

    AddWidget(path, "No Itemboxes", WIDGET_CVAR_CHECKBOX)
        .CVar("gDisableItemboxes")
        .Options(CheckboxOptions().Tooltip(
            "Prevents Itemboxes from spawning"));
    AddWidget(path, "All Thwomps are Marty", WIDGET_CVAR_CHECKBOX)
        .CVar("gAllThwompsAreMarty")
        .Options(CheckboxOptions().Tooltip(
            "All Thwomps are Marty"));
    AddWidget(path, "All Bomb Karts in Chase Mode", WIDGET_CVAR_CHECKBOX)
        .CVar("gAllBombKartsChase")
        .Options(CheckboxOptions().Tooltip(
            "These karts will chase you!!!"));
    AddWidget(path, "Get the trophies!", WIDGET_CVAR_CHECKBOX)
        .CVar("gGoFish")
        .Options(CheckboxOptions().Tooltip(
            "Collect as many trophies as you can. Racer with the most trophies wins!"));
    AddWidget(path, "Track X Stretch", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gVtxStretch[0])
        .Options(UIWidgets::FloatSliderOptions().Min(0.1f).Max(10.0f).Step(0.1f).Format("%.2f"));
    AddWidget(path, "Track Y Stretch", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gVtxStretch[1])
        .Options(UIWidgets::FloatSliderOptions().Min(0.1f).Max(10.0f).Step(0.1f).Format("%.2f"));
    AddWidget(path, "Track Z Stretch", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gVtxStretch[2])
        .Options(UIWidgets::FloatSliderOptions().Min(0.1f).Max(10.0f).Step(0.1f).Format("%.2f"));


    AddWidget(path, "Trains", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumTrains")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(19).Step(1).DefaultValue(2));
    AddWidget(path, "Carriages", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumCarriages")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(74).Step(1).DefaultValue(5));
    AddWidget(path, "Train has a tender", WIDGET_CVAR_CHECKBOX)
        .CVar("gHasTender")
        .Options(UIWidgets::CheckboxOptions().DefaultValue(1)
        .Tooltip("This option is only valid if there are no carriages on the train"));

    AddWidget(path, "Trucks", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumTrucks")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(50).Step(1).DefaultValue(7));
    AddWidget(path, "Buses", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumBuses")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(50).Step(1).DefaultValue(7));
    AddWidget(path, "Tanker Trucks", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumTankerTrucks")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(50).Step(1).DefaultValue(7));
    AddWidget(path, "Cars", WIDGET_CVAR_SLIDER_INT)
        .CVar("gNumCars")
        .Options(UIWidgets::IntSliderOptions().Min(0).Max(50).Step(1).DefaultValue(7));
}

void PortMenu::AddDevTools() {
    AddMenuEntry("Developer", "gSettings.Menu.DevToolsSidebarSection");
    AddSidebarEntry("Developer", "General", 3);
    WidgetPath path = { "Developer", "General", SECTION_COLUMN_1 };
#ifdef __SWITCH__
    AddWidget(path, "Switch CPU Profile", WIDGET_CVAR_COMBOBOX)
        .CVar("gSwitchPerfMode")
        .Options(ComboboxOptions()
                     .Tooltip("Switches the CPU profile to a different one")
                     .ComboMap(switchCPUProfiles)
                     .DefaultIndex(Ship::SwitchProfiles::STOCK))
        .Callback([](WidgetInfo& info) { Ship::Switch::ApplyOverclock(); });
#endif
    AddWidget(path, "Popout Menu", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.Popout")
        .Options(CheckboxOptions().Tooltip("Changes the menu display from overlay to windowed."));
    AddWidget(path, "Debug Mode", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnableDebugMode")
        .Callback([](WidgetInfo& info) {
            SPDLOG_INFO(CVarGetInteger("gEnableDebugMode", 0) == 0 ? "Debug Mode deactivated" : "Debug Mode activated");
        })
        .Options(CheckboxOptions().Tooltip("Enables Debug Mode."));
    AddWidget(path, "Modify Interpolation Target FPS", WIDGET_CVAR_CHECKBOX)
        .CVar("gModifyInterpolationTargetFPS")
        .Options(CheckboxOptions().Tooltip("For testing frame interpolation."));
    AddWidget(path, "Interpolation Target FPS", WIDGET_CVAR_SLIDER_INT)
        .CVar("gInterpolationTargetFPS")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gModifyInterpolationTargetFPS", 0); })
        .Options(IntSliderOptions()
                     .Tooltip("Sets the target FPS for interpolation. When Modify Interpolation Target FPS are enable")
                     .Min(15)
                     .Max(360)
                     .DefaultValue(60));
    AddWidget(path, "Render Collision", WIDGET_CVAR_CHECKBOX)
        .CVar("gRenderCollisionMesh")
        .Options(CheckboxOptions().Tooltip("Renders the collision mesh instead of the course mesh"));

    path = { "Developer", "Gfx Debugger", SECTION_COLUMN_1 };
    AddSidebarEntry("Developer", "Gfx Debugger", 1);
    AddWidget(path, "Popout Gfx Debugger", WIDGET_WINDOW_BUTTON)
        .CVar("gGfxDebuggerEnabled")
        .Options(ButtonOptions().Tooltip(
            "Enables the Gfx Debugger window, allowing you to input commands. Type help for some examples"))
        .WindowName("GfxDebuggerWindow");

    path = { "Developer", "Stats", SECTION_COLUMN_1 };
    AddSidebarEntry("Developer", "Stats", 1);
    AddWidget(path, "Popout Stats", WIDGET_WINDOW_BUTTON)
        .CVar("gStatsEnabled")
        .Options(ButtonOptions().Tooltip(
            "Shows the stats window, with your FPS and frametimes, and the OS you're playing on"))
        .WindowName("Stats");

    path = { "Developer", "Console", SECTION_COLUMN_1 };
    AddSidebarEntry("Developer", "Console", 1);
    AddWidget(path, "Popout Console", WIDGET_WINDOW_BUTTON)
        .CVar("gConsoleEnabled")
        .Options(ButtonOptions().Tooltip(
            "Enables the console window, allowing you to input commands. Type help for some examples"))
        .WindowName("Console");
}

PortMenu::PortMenu(const std::string& consoleVariable, const std::string& name)
    : Menu(consoleVariable, name, 0, UIWidgets::Colors::LightBlue) {
}

// bool CheckNetworkConnected(disabledInfo& info) {
//     return gNetwork.isConnected;
// }

void PortMenu::InitElement() {
    Ship::Menu::InitElement();
    AddSettings();
    AddEnhancements();
    AddDevTools();

    if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
        InsertSidebarSearch();
    }

    for (auto& initFunc : MenuInit::GetInitFuncs()) {
        initFunc();
    }

    disabledMap = {
        { DISABLE_FOR_FREE_CAM_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gFreecam", 0); }, "Freecam is Enabled" } },
        { DISABLE_FOR_FREE_CAM_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gFreecam", 0); }, "Freecam is Disabled" } },
        { DISABLE_FOR_EDITOR_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gEditorEnabled", 0); }, "Editor is Enabled" } },
        { DISABLE_FOR_EDITOR_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEditorEnabled", 0); }, "Editor is Disabled" } },
        { DISABLE_FOR_DEBUG_MODE_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEnableDebugMode", 0); },
            "Debug Mode is Disabled" } },
        { DISABLE_FOR_NO_VSYNC,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync();
           },
            "Disabling VSync not supported" } },
        { DISABLE_FOR_NO_WINDOWED_FULLSCREEN,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->SupportsWindowedFullscreen();
           },
            "Windowed Fullscreen not supported" } },
        { DISABLE_FOR_NO_MULTI_VIEWPORT,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->GetGui()->SupportsViewports();
           },
            "Multi-viewports not supported" } },
        { DISABLE_FOR_NOT_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() !=
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Available Only on DirectX" } },
        { DISABLE_FOR_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Not Available on DirectX" } },
        { DISABLE_FOR_MATCH_REFRESH_RATE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gMatchRefreshRate", 0); },
            "Match Refresh Rate is Enabled" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution Enabled" } },
        { DISABLE_FOR_VERTICAL_RES_TOGGLE_ON,
          { [](disabledInfo& info) -> bool {
               return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle Enabled" } },
        { DISABLE_FOR_LOW_RES_MODE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_LOW_RES_MODE, 0); }, "N64 Mode Enabled" } },
        //{ DISABLE_FOR_MULTIPLAYER_CONNECTED,
        //  { CheckNetworkConnected, "Multiplayer Connected"}},
    };
}

void PortMenu::UpdateElement() {
    Ship::Menu::UpdateElement();
}

void PortMenu::Draw() {
    Ship::Menu::Draw();
}

void PortMenu::DrawElement() {
    Ship::Menu::DrawElement();
}
} // namespace GameUI
