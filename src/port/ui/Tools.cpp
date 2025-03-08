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

extern "C" {
#include "code_800029B0.h"
}

namespace EditorNamespace {

    ToolsWindow::~ToolsWindow() {
        SPDLOG_TRACE("destruct tools window");
    }

    void ToolsWindow::InitElement() {
        
    }

    void ToolsWindow::DrawElement() {
        static bool toggleState = CVarGetInteger("gEditorSnapToGround", 0);
        static int selectedTool = 0; // 0: Move, 1: Rotate, 2: Scale
        static int selectedPlayTool = 0; // 0: Play, 1: Paused
        
        ImVec4 defaultColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
        // Function to check and highlight the selected button
        auto ToolButton = [&](const char* label, int id) {
            bool isSelected = (selectedTool == id);
            ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
            
            if (ImGui::Button(label, ImVec2(125, 25))) {
                selectedTool = id; // Set the selected tool
            }
    
            ImGui::PopStyleColor();
        };
    
        // Draw buttons
        ToolButton("Move", 0);
        ImGui::SameLine();
        ToolButton("Rotate", 1);
        ImGui::SameLine();
        ToolButton("Scale", 2);

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, toggleState ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
        if (ImGui::Button(toggleState ? "SNAP TO GROUND" : "SNAP TO GROUND", ImVec2(125, 25))) {
            toggleState = !toggleState;

            CVarSetInteger("gEditorSnapToGround", toggleState);
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();

        if (ImGui::Button("Player One AI/Human", ImVec2(160, 25))) {
            if (gPlayerOne->type & PLAYER_KART_AI) {
                gPlayerOne->type &= ~PLAYER_KART_AI;
            } else {
                gPlayerOne->type |= PLAYER_KART_AI;
            }
        }

        ImGui::SameLine();

        auto PlayButton = [&](const char* label, int id) {
            bool isSelected = (selectedPlayTool == id);
            ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.4f, 0.7f, 0.9f, 1.0f) : defaultColor);
            
            if (ImGui::Button(label, ImVec2(125, 25))) {
                selectedPlayTool = id; // Set the selected tool
                gIsEditorPaused = (id == 1);
            }
    
            ImGui::PopStyleColor();
        };

        // Draw buttons
        PlayButton("Play", 0);
        ImGui::SameLine();
        PlayButton("Pause", 1);
    }
}
