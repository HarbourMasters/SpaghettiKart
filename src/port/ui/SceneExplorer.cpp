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

namespace Editor {

    SceneExplorerWindow::~SceneExplorerWindow() {
        SPDLOG_TRACE("destruct scene explorer window");
    }

    void SceneExplorerWindow::DrawElement() {
        ImGui::Text("Scene");

        size_t id = 0; // id for now because we don't have unique names atm
        for (auto& object : gEditor.eGameObjects) {
            // Convert const char* to std::string before formatting
            std::string objectName = object->Name ? object->Name : "Obj";

            // Ensure unique label using index
            std::string label = fmt::format("{}##{}", objectName, id);

            if (ImGui::Button(label.c_str())) {
                gEditor.SelectObjectFromSceneExplorer(object);
            }
            id += 1;
        }
    }
}
