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

namespace EditorNamespace {

    ToolsWindow::~ToolsWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void ToolsWindow::InitElement() {
        
    }

    void ToolsWindow::DrawElement() {
        static bool toggleState = false;
        static int selectedTool = 0; // 0: Move, 1: Rotate, 2: Scale

        // Function to check and highlight the selected button
        auto ToolButton = [&](const char* label, int id) {
            bool isSelected = (selectedTool == id);
            ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            
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

        ImGui::PushStyleColor(ImGuiCol_Button, toggleState ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button(toggleState ? "SNAP TO GROUND" : "SNAP TO GROUND", ImVec2(125, 25))) {
            toggleState = !toggleState;

            CVarSetInteger("gEditorSnapToGround", toggleState);
        }
        ImGui::PopStyleColor();
    }
}
