#include "FreecamWindow.h"
#include "port/ui/PortMenu.h"
#include "port/Game.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>
#include "engine/cameras/FreeCamera.h"
#include "enhancements/freecam/freecam_engine.h"
#include "enhancements/freecam/freecam.h"

extern "C" {
// typedef struct {
//     Vec3f pos;
//     Vec3f velocity;
//     Vec3f lookAt;
//     Vec3f lookAtVelocity;
// } FreeCam;

extern f32 gDampValue;
extern f32 gRotDampValue;
extern f32 gFreecamSpeed;
extern f32 gFreecamSpeedMultiplier;
extern f32 gFreecamFollowFactor;
extern char* D_800E76A8[];
extern u32 fRankIndex;
extern u32 fTargetPlayer;
extern u32 bFreecamUseController;
void freecam_get_player_from_character(s32 characterId);
}

namespace GameUI {
extern std::shared_ptr<PortMenu> mPortMenu;
namespace Freecam {

static s32 sReadyUpBool = false;

int32_t controllerType = 0;
std::unordered_map<int32_t, const char*> controlType = {{ 0, "Mouse/Keyboard" }, { 1, "Controller" }};

uint32_t focusPlayer;

bool IsPlayerValid(const char* string) {
    return string != NULL && (strncmp(string, "\xA1\xBC\xA1\xBC\xA1\xBC\xA1\xBC", 8) != 0);
}

void RegisterFreecamWidgets() {
    mPortMenu->AddSidebarEntry("Enhancements", "Freecam", 2);
    WidgetPath path = { "Enhancements", "Freecam", SECTION_COLUMN_1 };
    //const float framerate = ImGui::GetIO().Framerate;
    //const float deltatime = ImGui::GetIO().DeltaTime;
    //ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    //mPortMenu->AddWidget(path, "Controller mode is not configured yet.", WIDGET_TEXT);

    //static int current_item = 0;
    mPortMenu->AddWidget(path, "Human Interface Device", WIDGET_COMBOBOX)
        .ValuePointer(&controllerType)
        .Callback([](WidgetInfo& info) { bFreecamUseController = (uint32_t)*std::get<int32_t*>(info.valuePointer); })
        .Options(UIWidgets::ComboboxOptions().ComboMap(controlType));

    mPortMenu->AddWidget(path, "Keyboard: Move: W,A,S,D, Up: Space, Down: Shift\n  Faster: Ctrl, Look: Right-mouse button\n  Target Player Mode: F, Next: M, Previous: N", WIDGET_TEXT);
    mPortMenu->AddWidget(path, "Controller: Up: A, Down: B, Faster: Z\n  Target Player Mode: R, Next: Right DPad, Previous: Left DPad\n  Driving Mode: L and R Buttons", WIDGET_TEXT);
    mPortMenu->AddWidget(path, "Enable Freecam", WIDGET_CVAR_CHECKBOX)
        .CVar("gFreecam")
        .Options(UIWidgets::CheckboxOptions({{ .tooltip = "Allows you to fly around the track"}}))
        .Callback([](WidgetInfo& info) {
            bool state = (bool) CVarGetInteger("gFreecam", false);
            CM_SetFreeCamera(state);
        });

    mPortMenu->AddWidget(path, "Camera Damping", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gDampValue)
        .Options(UIWidgets::FloatSliderOptions().Min(0.970f).Max(1.0f).Step(0.001f).Format("%.3f"));
    mPortMenu->AddWidget(path, "Camera Speed", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gFreecamSpeed)
        .Options(UIWidgets::FloatSliderOptions().Min(1.0f).Max(30.0f).Step(0.1f).Format("%.1f"));
    mPortMenu->AddWidget(path, "Camera Speed Multiplier", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gFreecamSpeedMultiplier)
        .Options(UIWidgets::FloatSliderOptions().Min(1.5f).Max(15.0f).Step(0.1f).Format("%.1f"));
    mPortMenu->AddWidget(path, "Camera Rotator Damping", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gRotDampValue)
        .Options(UIWidgets::FloatSliderOptions().Min(0.0f).Max(1.0f).Step(0.01f).Format("%.2f"));
    mPortMenu->AddWidget(path, "Follow Factor", WIDGET_SLIDER_FLOAT)
        .ValuePointer(&gFreecamFollowFactor)
        .Options(UIWidgets::FloatSliderOptions().Min(0.0f).Max(1.0f).Step(0.01f).Format("%.2f"));

    mPortMenu->AddWidget(path, "Target Player", WIDGET_TEXT);
    mPortMenu->AddWidget(path, "None", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) { fTargetPlayer = false; });

    mPortMenu->AddWidget(path, "Player 1", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[0]);
            if (!info.isHidden) {
                info.name = D_800E76A8[0];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(0);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 2", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[1]);
            if (!info.isHidden) {
                info.name = D_800E76A8[1];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(1);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 3", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[2]);
            if (!info.isHidden) {
                info.name = D_800E76A8[2];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(2);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 4", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[3]);
            if (!info.isHidden) {
                info.name = D_800E76A8[3];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(3);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 5", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[4]);
            if (!info.isHidden) {
                info.name = D_800E76A8[4];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(4);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 6", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[5]);
            if (!info.isHidden) {
                info.name = D_800E76A8[5];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(5);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 7", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[6]);
            if (!info.isHidden) {
                info.name = D_800E76A8[6];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(6);
            fTargetPlayer = true;
        });
    mPortMenu->AddWidget(path, "Player 8", WIDGET_BUTTON)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = !IsPlayerValid(D_800E76A8[7]);
            if (!info.isHidden) {
                info.name = D_800E76A8[7];
            }
        })
        .Options(UIWidgets::ButtonOptions().Size(UIWidgets::Sizes::Inline))
        .SameLine(true)
        .Callback([](WidgetInfo& info) {
            freecam_get_player_from_character(7);
            fTargetPlayer = true;
        });
}

static RegisterMenuInitFunc initFunc(RegisterFreecamWidgets);

} // namespace Freecam
} // namespace GameUI
