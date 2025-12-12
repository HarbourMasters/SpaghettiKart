#include "SceneExplorer.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "ship/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "engine/editor/Editor.h"
#include "port/Game.h"

extern "C" {
#include "actors.h"
}

namespace Editor {

    SceneExplorerWindow::~SceneExplorerWindow() {
        SPDLOG_TRACE("destruct scene explorer window");
    }

    void SceneExplorerWindow::DrawElement() {
        ImGui::Text("Scene");

        size_t id = 0; // id for now because we don't have unique names atm

        for (const auto& actor : GetWorld()->Actors) {
            std::string name;
            if (nullptr != actor->Name && actor->Name[0] != '\0') {
                name = std::string(actor->Name);
            } else {
                if (!actor->IsMod()) {
                    name = std::string(get_actor_display_name(actor->Type));
                } else {
                    name = "Unk Actor";
                }
            }

            // Ensure unique label using index
            std::string label = fmt::format("{}##{}", name, id);

            if (ImGui::Button(label.c_str())) {
                gEditor.SelectObjectFromSceneExplorer(actor.get());
            }

            id += 1;
        }

        for (const auto& object : GetWorld()->Objects) {
            std::string name;
            if (nullptr != object->Name && object->Name[0] != '\0') {
                name = std::string(object->Name);
            } else {
                name = "Unk Object";
            }

            // Ensure unique label using index
            std::string label = fmt::format("{}##{}", name, id);

            if (ImGui::Button(label.c_str())) {
                gEditor.SelectObjectFromSceneExplorer(object.get());
            }
            id += 1;
        }

        for (auto& object : gEditor.eGameObjects) {
            std::string name;
            if (nullptr != object->Name && object->Name[0] != '\0') {
                name = std::string(object->Name);
            } else {
                name = "Unk Editor Obj";
            }

            // Ensure unique label using index
            std::string label = fmt::format("{}##{}", name, id);

            if (ImGui::Button(label.c_str())) {
                gEditor.SelectObjectFromSceneExplorer(object);
            }
            id += 1;
        }
    }
}
