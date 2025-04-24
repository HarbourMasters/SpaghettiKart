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
            ImGui::Text("Position:");
            ImGui::PushID("PositionX");
            ImGui::DragFloat("X", &selected->Pos->x, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Pos->x = 0.0f; }
            ImGui::PopID();

            ImGui::PushID("PositionY");
            ImGui::DragFloat("Y", &selected->Pos->y, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Pos->y = 0.0f; }
            ImGui::PopID();

            ImGui::PushID("PositionZ");
            ImGui::DragFloat("Z", &selected->Pos->z, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Pos->z = 0.0f; }
            ImGui::PopID();
        }

        if (selected->Rot) {
            ImGui::Separator();
            ImGui::Text("Rotation (IRotator):");

            int pitch = selected->Rot->pitch;
            ImGui::PushID("RotX");
            if (ImGui::DragInt("Pitch (X)", &pitch, 1.0f, 0, 65535)) {
                selected->Rot->pitch = static_cast<uint16_t>(pitch);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Rot->pitch = 0; }
            ImGui::PopID();

            int yaw = selected->Rot->yaw;
            ImGui::PushID("RotY");
            if (ImGui::DragInt("Yaw (Y)", &yaw, 1.0f, 0, 65535)) {
                selected->Rot->yaw = static_cast<uint16_t>(yaw);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Rot->yaw = 0; }
            ImGui::PopID();

            int roll = selected->Rot->roll;
            ImGui::PushID("RotZ");
            if (ImGui::DragInt("Roll (Z)", &roll, 1.0f, 0, 65535)) {
                selected->Rot->roll = static_cast<uint16_t>(roll);
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Rot->roll = 0; }
            ImGui::PopID();
        }

        if (selected->Scale) {
            ImGui::Separator();
            ImGui::Text("Scale:");

            ImGui::PushID("ScaleX");
            ImGui::DragFloat("X", &selected->Scale->x, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Scale->x = 1.0f; }
            ImGui::PopID();

            ImGui::PushID("ScaleY");
            ImGui::DragFloat("Y", &selected->Scale->y, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Scale->y = 1.0f; }
            ImGui::PopID();

            ImGui::PushID("ScaleZ");
            ImGui::DragFloat("Z", &selected->Scale->z, 0.1f);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO)) { selected->Scale->z = 1.0f; }
            ImGui::PopID();
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