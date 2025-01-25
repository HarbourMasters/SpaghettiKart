#include "FreecamWindow.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <libultraship/libultraship.h>
#include "spdlog/spdlog.h"
#include <common_structs.h>
#include <defines.h>
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
extern f32 gFreecamSpeed;
extern f32 gFreecamSpeedMultiplier;
extern f32 gFreecamRotateSmoothingFactor;
extern f32 gFreecamFollowFactor;
extern char* D_800E76A8[];
extern u32 fRankIndex;
extern u32 fTargetPlayer;
extern u32 gFreecamControllerType;
void freecam_get_player_from_character(s32 characterId);
}

namespace Freecam {
FreecamWindow::~FreecamWindow() {
    SPDLOG_TRACE("destruct freecam window");
}

void FreecamWindow::InitElement() {
}

static s32 sReadyUpBool = false;

float dampMin = 0.970;
float dampMax = 1.0f;
float minSpeed = 1.0;
float maxSpeed = 30.0f;
float minSpeedMultiplier = 1.5f;
float maxSpeedMultiplier = 15.0f;
float minFreecamRotateFactor = 0.0f;
float maxFreecamRotateFactor = 1.0f;
float minFreecamFollowFactor = 0.0f;
float maxFreecamFollowFactor = 1.0f;

uint32_t focusPlayer;

void FreecamWindow::DrawElement() {
    const float framerate = ImGui::GetIO().Framerate;
    const float deltatime = ImGui::GetIO().DeltaTime;
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    ImGui::Text("Controller mode is not configured yet.");

    const char* items[] = { "Mouse/Keyboard", "Controller" };
    static int current_item = 0;
    if (ImGui::Combo("Dropdown", &current_item, items, IM_ARRAYSIZE(items))) {
        gFreecamControllerType = current_item;
    }

    ImGui::Text("Move: W,A,S,D\nUp: Space, Down: Shift\nFaster: Ctrl\nLook: Right-mouse button\nTarget Player Mode: F, Next: M, Previous: N");
    ImGui::Spacing();
    UIWidgets::CVarCheckbox("Enable Flycam", "gFreecam", { .tooltip = "Allows you to fly around the course" });

    ImGui::Spacing();

    if (ImGui::SliderScalar("Camera Damping", ImGuiDataType_Float, &gDampValue, &dampMin, &dampMax, "%f")) {};
    if (ImGui::SliderScalar("Camera Speed", ImGuiDataType_Float, &gFreecamSpeed, &minSpeed, &maxSpeed, "%f")) {};
    if (ImGui::SliderScalar("Camera Speed Multiplier", ImGuiDataType_Float, &gFreecamSpeedMultiplier,
                            &minSpeedMultiplier, &maxSpeedMultiplier, "%f")) {};
    if (ImGui::SliderScalar("Camera Rotation Smoothing", ImGuiDataType_Float, &gFreecamRotateSmoothingFactor,
                            &minFreecamRotateFactor, &maxFreecamRotateFactor, "%f")) {};
    if (ImGui::SliderScalar("Follow Factor", ImGuiDataType_Float, &gFreecamFollowFactor,
                            &minFreecamFollowFactor, &maxFreecamFollowFactor, "%f")) {};

    ImGui::Spacing();

    ImGui::Text("Target Player");

    if (ImGui::Button("None")) {
        fTargetPlayer = false;
    }

    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        // These are euc-jp characters that look sort of like a hyphen -
        if (D_800E76A8[i] == NULL || D_800E76A8[i] == "\xA1\xBC\xA1\xBC\xA1\xBC\xA1\xBC") {
            break;
        }
        ImGui::SameLine();
        if (ImGui::Button(D_800E76A8[i])) {
            freecam_get_player_from_character(i);
            fTargetPlayer = true;
        }
    }

    ImGui::PopStyleColor();
}

void FreecamWindow::UpdateElement() {
}
} // namespace Freecam
