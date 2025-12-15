#include "Properties.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "ship/Context.h"
#include <variant>

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "engine/SpawnParams.h"
#include "engine/editor/Editor.h"
#include "port/Game.h"
#include "src/engine/World.h"

#include "engine/vehicles/Train.h"

extern "C" {
#include "actors.h"
}

namespace TrackEditor {

    PropertiesWindow::~PropertiesWindow() {
        SPDLOG_TRACE("destruct properties window");
    }

    void PropertiesWindow::DrawElement() {

        std::visit([this](auto* obj) {

            using T = std::decay_t<decltype(*obj)>;  // Get the type the pointer is pointing to
            if (nullptr == obj) {
                return;
            }

            const char* title;

            // Get the actors display name, handling AActor (and the C version), OObject, and GameObject types
            if constexpr (std::is_same_v<T, AActor>) {
                if (obj->IsMod()) {
                    title = obj->Name;
                } else {
                    title = get_actor_display_name(obj->Type);
                }
            } else if constexpr (std::is_same_v<T, OObject>) {
                title = obj->Name;
            } else if constexpr (std::is_same_v<T, GameObject>) {
                title = obj->Name;
            } else {
                title = "Unknown type";
            }

            // Center Actor Title Text
            float windowWidth = ImGui::GetWindowSize().x;
            float textWidth = ImGui::CalcTextSize(title).x;

            ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
            // Display actor title
            ImGui::Text(title);

            // Display actor resource name. ex. mk:cloud
            if (obj->ResourceName[0] != '\0') { // Params is unset for some train components
                ImGui::Text("Resource Name %s", obj->ResourceName);
            }

            obj->DrawEditorProperties();

            }, gEditor.eObjectPicker.eGizmo._selected);

            // This allowed the user to alter the bounding box of the editor selection if it was too small.
            // if (selected->Collision == GameObject::CollisionType::BOUNDING_BOX) {
            //     ImGui::Separator();
            //     ImGui::Text("Editor Bounding Box Size:");
            //     ImGui::PushID("BoundingBoxSize");
            //     ImGui::DragFloat("##BoundingBoxSize", &selected->BoundingBoxSize, 0.1f);
            //     ImGui::SameLine();
            //     if (ImGui::Button(ICON_FA_UNDO)) { selected->BoundingBoxSize = 2.0f; }
            //     ImGui::PopID();
            // }
    }

}
