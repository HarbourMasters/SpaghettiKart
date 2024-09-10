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
extern char* D_800E76A8[];
extern u32 fRankIndex;
extern u32 gFreecamControllerType;
}

namespace Freecam {
FreecamWindow::~FreecamWindow() {
    SPDLOG_TRACE("destruct freecam window");
}

void FreecamWindow::InitElement() {
}

static s32 sReadyUpBool = false;

float dampMin = 0.995;
float dampMax = 1.0f;
float minSpeed = 1.0;
float maxSpeed = 30.0f;
float minSpeedMultiplier = 1.5f;
float maxSpeedMultiplier = 15.0f;
float minFreecamRotateFactor = 0.0f;
float maxFreecamRotateFactor = 1.0f;

uint32_t focusPlayer;

void FreecamWindow::DrawElement() {
    const float framerate = ImGui::GetIO().Framerate;
    const float deltatime = ImGui::GetIO().DeltaTime;
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    ImGui::Text("Freecam: Mouse/Keyboard Requires OpenGL. DX is not supported");

    const char* items[] = { "Mouse/Keyboard", "N64 Controls" };
    static int current_item = 0;
    if (ImGui::Combo("Dropdown", &current_item, items, IM_ARRAYSIZE(items))) {
        gFreecamControllerType = current_item;
    }

    ImGui::Text("Move: W,A,S,D\nUp: Space, Down: Shift\nFaster: Ctrl\nLook: Right-mouse button\nTarget previous player: N, Target next player: M");
    ImGui::Spacing();
    UIWidgets::CVarCheckbox("Enable Flycam", "gFreecam", {
        .tooltip = "Allows you to fly around the course"
    });

    ImGui::Spacing();

    if (ImGui::SliderScalar("Camera Damping", ImGuiDataType_Float, &gDampValue, &dampMin, &dampMax, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Speed", ImGuiDataType_Float, &gFreecamSpeed, &minSpeed, &maxSpeed, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Speed Multiplier", ImGuiDataType_Float, &gFreecamSpeedMultiplier, &minSpeedMultiplier, &maxSpeedMultiplier, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Rotation Smoothing", ImGuiDataType_Float, &gFreecamRotateSmoothingFactor, &minFreecamRotateFactor, &maxFreecamRotateFactor, "%f")) {

    };
    
    ImGui::Spacing();

    ImGui::Text("Target Player");

    if (ImGui::Button("None")) {
        fRankIndex = -1;
    }

    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        if (D_800E76A8[i] == NULL || D_800E76A8[i] == "ーーーー") {
            break;
        }
        ImGui::SameLine();
        if (ImGui::Button(D_800E76A8[i])) {
            fRankIndex = i;
        }
    }

    
    ImGui::PopStyleColor();
}

void FreecamWindow::UpdateElement() {
}
} // namespace Freecam
