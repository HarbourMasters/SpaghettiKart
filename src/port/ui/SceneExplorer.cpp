#include "SceneExplorer.h"
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

    SceneExplorerWindow::~SceneExplorerWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void SceneExplorerWindow::DrawElement() {
        ImGui::Text("This is your Scene Explorer window!");
        if (ImGui::Button("Click Me")) {
            // Handle button click (example)
        }
    }
}
