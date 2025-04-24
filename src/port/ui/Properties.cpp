#include "Properties.h"
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
#include "src/engine/World.h"

namespace Editor {

    PropertiesWindow::~PropertiesWindow() {
        SPDLOG_TRACE("destruct properties window");
    }

    void PropertiesWindow::DrawElement() {
        GameObject* selected = gEditor.eObjectPicker.eGizmo._selected;

        if (nullptr == selected) {
            return;
        }

        ImGui::Begin("Properties");

        if (selected->Pos) {
            ImGui::Text("Location");
            ImGui::SameLine();

            bool positionChanged = ImGui::DragFloat3("##Location", &selected->Pos->x, 0.1f);

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
                selected->Pos->x = 0.0f;
                selected->Pos->y = 0.0f;
                selected->Pos->z = 0.0f;
                positionChanged = true; // also counts as a change
            }

            if (positionChanged) {
                gEditor.eObjectPicker.eGizmo.Pos = *selected->Pos;
            }
        }

        if (selected->Rot) {
            ImGui::Text("Rotation");
            ImGui::SameLine();

            // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
            int rot[3] = {
                selected->Rot->pitch,
                selected->Rot->yaw,
                selected->Rot->roll
            };

            if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
                for (int i = 0; i < 3; i++) {
                    // Wrap around 0–65535
                    rot[i] = (rot[i] % 65536 + 65536) % 65536;
                }

                selected->Rot->pitch = static_cast<uint16_t>(rot[0]);
                selected->Rot->yaw   = static_cast<uint16_t>(rot[1]);
                selected->Rot->roll  = static_cast<uint16_t>(rot[2]);
            }

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
                selected->Rot->pitch = 0;
                selected->Rot->yaw   = 0;
                selected->Rot->roll  = 0;
            }
        }

        if (selected->Scale) {
            ImGui::Text("Scale   ");
            ImGui::SameLine();

            ImGui::DragFloat3("##Scale", &selected->Scale->x, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
                selected->Scale->x = 1.0f;
                selected->Scale->y = 1.0f;
                selected->Scale->z = 1.0f;
            }
        }

        if (selected->Collision == GameObject::CollisionType::BOUNDING_BOX) {
            ImGui::Separator();
            ImGui::Text("Editor Bounding Box Size:");
            ImGui::PushID("BoundingBoxSize");
            ImGui::DragFloat("##BoundingBoxSize", &selected->BoundingBoxSize, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->BoundingBoxSize = 2.0f; }
            ImGui::PopID();
        }

        ImGui::End();
    }
}