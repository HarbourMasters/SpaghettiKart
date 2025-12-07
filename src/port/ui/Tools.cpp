#include "Tools.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>
#include "port/Game.h"
#include "engine/editor/SceneManager.h"

extern "C" {
#include "code_800029B0.h"
#include "code_80057C60.h"
}

namespace Editor {

    ToolsWindow::~ToolsWindow() {
        SPDLOG_TRACE("destruct tools window");
    }

    void ToolsWindow::InitElement() {
        
    }

    void ToolsWindow::DrawElement() {
        static bool toggleGroundSnap = CVarGetInteger("gEditorSnapToGround", 0);
        static bool toggleBoundary = CVarGetInteger("gEditorBoundary", 0);
        static int selectedTool = 0; // 0: Move, 1: Rotate, 2: Scale

        // Save button
        if (ImGui::Button(ICON_FA_FLOPPY_O, ImVec2(50, 25))) {
            if (gIsEditorPaused) {
                SaveLevel();
            } else {
                printf("[Editor] Cannot save during simulation\n  Please switch back to edit mode!\n\n");
            }
        }

        ImGui::SameLine();

        ImVec4 defaultColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
        // Function to check and highlight the selected button
        auto ToolButton = [&](const char* label, int id) {
            bool isSelected = (selectedTool == id);
            ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
            
            if (ImGui::Button(label, ImVec2(50, 25))) {
                selectedTool = id; // Set the selected tool
                CVarSetInteger("eGizmoMode", id);
            }
    
            ImGui::PopStyleColor();
        };

        // Draw buttons
        ToolButton(ICON_FA_ARROWS, 0);
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Translate");
            ImGui::EndTooltip();
        }
        ImGui::SameLine();
        ToolButton(ICON_FA_REPEAT, 1);
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Rotate");
            ImGui::EndTooltip();
        }
        ImGui::SameLine();
        ToolButton(ICON_FA_EXPAND, 2);
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Scale");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();

        // Snap to ground
        ImGui::PushStyleColor(ImGuiCol_Button, toggleGroundSnap ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);

        if (ImGui::Button(ICON_FA_LINK, ImVec2(50, 25))) {
            toggleGroundSnap = !toggleGroundSnap;

            CVarSetInteger("gEditorSnapToGround", toggleGroundSnap);
        }
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Snap object to surface");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();

        // Boundary
        ImGui::PushStyleColor(ImGuiCol_Button, toggleBoundary ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
        if (ImGui::Button(toggleBoundary ? "Map Boundaries" : "Map Boundaries", ImVec2(125, 25))) {
            toggleBoundary = !toggleBoundary;

            CVarSetInteger("gEditorBoundary", toggleBoundary);
        }
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Stops translated objects from leaving the track area.");
            ImGui::Text("This helps prevent moving objects really far away off the level.");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();

        // Toggle player human/ai
        bool playerToggle = (gPlayerOne->type & PLAYER_CPU);
        ImGui::PushStyleColor(ImGuiCol_Button, defaultColor);
        if (ImGui::Button(playerToggle ? ICON_FA_DESKTOP : ICON_FA_GAMEPAD, ImVec2(50, 25))) {
            if (playerToggle) {
                gPlayerOne->type &= ~PLAYER_CPU;
            } else {
                gPlayerOne->type |= PLAYER_CPU;
            }

        }
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Toggle player 1's AI or human player state.");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();

        // Play/pause button
        ImGui::PushStyleColor(ImGuiCol_Button, defaultColor);
        if (ImGui::Button(gIsEditorPaused ? ICON_FA_PLAY : ICON_FA_STOP, ImVec2(50, 25))) {
            if (gIsEditorPaused) {
                SaveLevel();
                CVarSetInteger("gFreecam", false);
                CM_SetFreeCamera(false);
            } else {
                CM_ResetAudio();
                CVarSetInteger("gFreecam", true);
                CM_SetFreeCamera(true);
            }

            gEditor.ResetGizmo();
            gIsEditorPaused = !gIsEditorPaused;
            gIsInQuitToMenuTransition = 1;
            gQuitToMenuTransitionCounter = 5;
            gGotoMode = RACING;
        }

        ImGui::PopStyleColor();

        ImGui::SameLine();

        // Camera mode button (drive kart and freecam)
        bool isVideoToolSelected = static_cast<bool>(CVarGetInteger("gFreecam", 0));
        ImGui::PushStyleColor(ImGuiCol_Button, defaultColor);
        const char* videoToolLabel = isVideoToolSelected 
            ? ICON_FA_VIDEO_CAMERA " " ICON_FA_PAPER_PLANE 
            : ICON_FA_VIDEO_CAMERA " " ICON_FA_CAR;

        if (ImGui::Button(videoToolLabel, ImVec2(50, 25))) {
            CVarSetInteger("gFreecam", !CVarGetInteger("gFreecam", 0));
            CM_SetFreeCamera(CVarGetInteger("gFreecam", 0));
        }

        ImGui::PopStyleColor();

        ImGui::SameLine();

        // Alter the game speed
        ToolsWindow::GameSpeed();

        ImGui::SameLine();

        // Toggle hud
        ImGui::PushStyleColor(ImGuiCol_Button, gIsHUDVisible ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
        if (ImGui::Button("Toggle HUD", ImVec2(150, 25))) {

            gIsHUDVisible = !gIsHUDVisible;
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();

        // Delete
        if (ImGui::Button(ICON_FA_TRASH_O, ImVec2(50, 25))) {
            gEditor.DeleteObject();
        }
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Delete Selected Actor");
            ImGui::EndTooltip();
        }

        ImGui::SameLine();

        // Delete All button
        if (ImGui::Button(ICON_FA_INTERNET_EXPLORER, ImVec2(50, 25))) {
            ImGui::OpenPopup("ConfirmDeleteAllPopup");
        }

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Delete All Actors");
            ImGui::EndTooltip();
        }

        // Confirmation Popup
        if (ImGui::BeginPopupModal("ConfirmDeleteAllPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete all actors?\nThis action can be undone if you do not save, and then reload the track.");
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(120, 0))) {
                // Defer deletion until race_logic_loop
                bCleanWorld = true;
                gEditor.ResetGizmo();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

    }

    // Fast Forward the game
    void ToolsWindow::GameSpeed() {
        ImVec4 defaultColor = ImGui::GetStyle().Colors[ImGuiCol_Button];

        ImGui::PushStyleColor(ImGuiCol_Button, defaultColor);

        // Decrease
        if (ImGui::Button("-", ImVec2(25, 25))) {
            gTickLogic--;
            if (gTickLogic < 1) gTickLogic = 1; // clamp min
        }

        ImGui::SameLine();

        // Label with current value
        std::string label = "Game Speed: " + std::to_string(gTickLogic);
        if (ImGui::Button(label.c_str(), ImVec2(120, 25))) {
            gTickLogic = 2; // reset on click
        }

        ImGui::SameLine();

        // Increase
        if (ImGui::Button("+", ImVec2(25, 25))) {
            gTickLogic++;
        }

        ImGui::PopStyleColor();
    }
}
