#include "FreecamWindow.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <libultraship/libultraship.h>
#include "spdlog/spdlog.h"
#include <common_structs.h>
#include <defines.h>

extern "C" {
//#include "enhancements/freecam/freecam_engine.h"
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
}

namespace Freecam {
FreecamWindow::~FreecamWindow() {
    SPDLOG_TRACE("destruct game info window");
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

void FreecamWindow::DrawElement() {
    const float framerate = ImGui::GetIO().Framerate;
    const float deltatime = ImGui::GetIO().DeltaTime;
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

    ImGui::Text("Freecam");

    UIWidgets::CVarCheckbox("Enable Flycam", "gFreecam", {
        .tooltip = "Allows you to fly around the course"
    });

    if (ImGui::SliderScalar("Camera Damping", ImGuiDataType_Float, &gDampValue, &dampMin, &dampMax, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Speed", ImGuiDataType_Float, &gFreecamSpeed, &minSpeed, &maxSpeed, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Speed Multiplier", ImGuiDataType_Float, &gFreecamSpeedMultiplier, &minSpeedMultiplier, &maxSpeedMultiplier, "%f")) {

    };
    if (ImGui::SliderScalar("Camera Rotation Smoothing", ImGuiDataType_Float, &gFreecamRotateSmoothingFactor, &minFreecamRotateFactor, &maxFreecamRotateFactor, "%f")) {

    };
    
    ImGui::PopStyleColor();
}

void FreecamWindow::UpdateElement() {
}
} // namespace Freecam
