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

namespace Editor {

    ToolsWindow::~ToolsWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void ToolsWindow::InitElement() {
        
    }

    void ToolsWindow::DrawElement() {
        static bool toggleState = false;
        ImGui::PushStyleColor(ImGuiCol_Button, toggleState ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button(toggleState ? "v" : "v", ImVec2(50, 25))) {
            toggleState = !toggleState;

            CVarSetInteger("gEditorSnapToGround", toggleState);
        }
        ImGui::PopStyleColor();
    }
}
