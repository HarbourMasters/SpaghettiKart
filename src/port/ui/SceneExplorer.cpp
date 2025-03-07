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

#include "engine/editor/Editor.h"
#include "port/Game.h"

namespace EditorNamespace {

    SceneExplorerWindow::~SceneExplorerWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void SceneExplorerWindow::DrawElement() {
        ImGui::Text("Scene");

        int id = 0; // id for now because we don't have unique names atm
        for (auto& object : gEditor.eGameObjects) {
            std::string label = fmt::format("Object {}", id++);
            if (ImGui::Button(label.c_str())) {
                gEditor.SelectObjectFromSceneExplorer(&object);
            }
        }
    }
}
