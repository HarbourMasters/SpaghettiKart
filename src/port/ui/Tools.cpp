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
            SaveLevel();
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
        if (ImGui::Button(gIsEditorPaused ? ICON_FA_PLAY : ICON_FA_PAUSE, ImVec2(50, 25))) {

            gIsEditorPaused = !gIsEditorPaused;
        }
        ImGui::PopStyleColor();

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
    }
}
