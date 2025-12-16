#include <variant>
#include "port/Game.h"

// Draw default properties
void DrawDefaultEditorProperties() {
    std::visit([](auto* obj) {
        using T = std::decay_t<decltype(*obj)>;
        if (nullptr == obj) {
            return;
        }

        // ImGui::Text("Type");
        // ImGui::SameLine();
        // int32_t type = static_cast<int16_t>(obj->SpawnType);
        // if (ImGui::InputInt("##Type", &type)) {
        //     *params.Type = static_cast<int16_t>(type);
        // }

        // if (params.Behaviour.has_value()) {
        //     ImGui::Text("Behaviour");
        //     ImGui::SameLine();
        //     int32_t behaviour = static_cast<int32_t>(params.Behaviour.value());
        //     if (ImGui::InputInt("##Behaviour", &behaviour)) {
        //         *params.Behaviour = static_cast<int16_t>(behaviour);
        //     }
        // }

        // if (params.Skin.has_value()) {
        //     ImGui::Text("Skin");
        //     ImGui::SameLine();
        //     // Copy the current string value into a buffer
        //     char buffer[256];
        //     std::strncpy(buffer, params.Skin->c_str(), sizeof(buffer));
        //     buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

        //     if (ImGui::InputText("##Skin", buffer, sizeof(buffer))) {
        //         *params.Skin = std::string(buffer);
        //     }
        // }

        ImGui::Text("Location");
        ImGui::SameLine();
        FVector location = obj->GetLocation();
        if (ImGui::DragFloat3("##Location", (float*)&location)) {
            obj->Translate(location);
            gEditor.eObjectPicker.eGizmo.Pos = location;
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
            FVector location = FVector(0, 0, 0);
            obj->Translate(location);
            gEditor.eObjectPicker.eGizmo.Pos = location;
        }

        ImGui::Text("Rotation");
        ImGui::SameLine();

        IRotator objRot = obj->GetRotation();

        // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
        int rot[3] = {
            objRot.pitch,
            objRot.yaw,
            objRot.roll
        };

        if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
            for (size_t i = 0; i < 3; i++) {
                // Wrap around 0–65535
                rot[i] = (rot[i] % 65536 + 65536) % 65536;
            }
            IRotator newRot;
            newRot.Set(
                static_cast<uint16_t>(rot[0]),
                static_cast<uint16_t>(rot[1]),
                static_cast<uint16_t>(rot[2])
            );
            obj->Rotate(newRot);
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
            IRotator rot = IRotator(0, 0, 0);
            obj->Rotate(rot);
        }

        FVector scale = obj->GetScale();
        ImGui::Text("Scale   ");
        ImGui::SameLine();

        ImGui::DragFloat3("##Scale", (float*)&scale, 0.1f);
        obj->SetScale(scale);
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
            FVector scale = FVector(1.0f, 1.0f, 1.0f);
            obj->SetScale(scale);
        }

        // FVector velocity = obj->Velocity;
        // ImGui::Text("Velocity");
        // ImGui::SameLine();

        // ImGui::DragFloat3("##Velocity", (float*)&velocity, 0.1f);
        // params.Velocity = velocity;
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetVelocity")) {
        //     params.Velocity = FVector(0.0f, 0.0f, 0.0f);
        // }

        // FVector2D patrol = params.PatrolStart.value();

        // ImGui::Text("Patrol Start");
        // ImGui::SameLine();

        // ImGui::DragFloat2("##PatrolStart", (float*)&patrol);
        // params.PatrolStart = patrol;
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPatrolStart")) {
        //     params.PatrolStart = FVector2D(0.0f, 0.0f);
        // }

        // FVector2D patrol = params.PatrolEnd.value();

        // ImGui::Text("Patrol End");
        // ImGui::SameLine();

        // ImGui::DragFloat2("##PatrolEnd", (float*)&patrol);
        // params.PatrolEnd = patrol;
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPatrolEnd")) {
        //     params.PatrolEnd = FVector2D(0.0f, 0.0f);
        // }

        // IPathSpan span = params.PathSpan.value();

        // ImGui::Text("Path Span");
        // ImGui::SameLine();

        // if (ImGui::DragInt2("##PathSpan", (int*)&span)) {
        //     params.PathSpan = span;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPathSpan")) {
        //     params.PathSpan = IPathSpan(0.0f, 0.0f);
        // }

        // int32_t primAlpha = params.PrimAlpha.value();
        // ImGui::Text("Prim Alpha");
        // ImGui::SameLine();

        // if (ImGui::InputInt("##PrimAlpha", (int*)&primAlpha)) {
        //     params.PrimAlpha = primAlpha;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPrimAlpha")) {
        //     params.PrimAlpha = 0;
        // }

        // int32_t boundingBoxSize = static_cast<int32_t>(params.BoundingBoxSize.value());
        // ImGui::Text("Bounding Box Size");
        // ImGui::SameLine();

        // if (ImGui::InputInt("##BoundingBoxSize", (int*)&boundingBoxSize)) {
        //     if (boundingBoxSize < 0) boundingBoxSize = 0;
        //     params.BoundingBoxSize = static_cast<uint16_t>(boundingBoxSize);
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetBoundingBoxSize")) {
        //     params.BoundingBoxSize = 0;
        // }

        // ImGui::Text("Count");
        // ImGui::SameLine();

        // int count = static_cast<int>(*params.Count);
        // if (ImGui::InputInt("##Count", &count)) {
        //     // Clamp to uint32_t range (only lower bound needed if assuming positive values)
        //     if (count < 0) count = 0;
        //     params.Count = static_cast<uint32_t>(count);
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetCount")) {
        //     params.Count = 0;
        // }

        // ImGui::Text("Left Exit Span");
        // ImGui::SameLine();

        // IPathSpan leftExitSpan = *params.LeftExitSpan;
        // if (ImGui::DragInt2("##LeftExitSpan", (int*)&leftExitSpan)) {
        //     params.LeftExitSpan = leftExitSpan;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetLeftExitSpan")) {
        //     params.LeftExitSpan = IPathSpan(0, 0);
        // }

        // ImGui::Text("Trigger Span");
        // ImGui::SameLine();

        // IPathSpan triggerSpan = *params.TriggerSpan;
        // if (ImGui::DragInt2("##TriggerSpan", (int*)&triggerSpan)) {
        //     params.TriggerSpan = triggerSpan;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetTriggerSpan")) {
        //     params.TriggerSpan = IPathSpan(0, 0);
        // }

        // ImGui::Text("Right Exit Span");
        // ImGui::SameLine();

        // IPathSpan rightExitSpan = *params.RightExitSpan;
        // if (ImGui::DragInt2("##RightExitSpan", (int*)&rightExitSpan)) {
        //     params.RightExitSpan = rightExitSpan;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetRightExitSpan")) {
        //     params.RightExitSpan = IPathSpan(0, 0);
        // }

        // ImGui::Text("Path Index");
        // ImGui::SameLine();

        // int pathIndex = static_cast<int>(params.PathIndex.value());
        // if (ImGui::InputInt("##PathIndex", &pathIndex)) {
        //     if (pathIndex < 0) pathIndex = 0;
        //     params.PathIndex = static_cast<uint32_t>(pathIndex);
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPathIndex")) {
        //     params.PathIndex = 0;
        // }

        // ImGui::Text("Path Point");
        // ImGui::SameLine();

        // int pathPoint = static_cast<int>(params.PathPoint.value());
        // if (ImGui::InputInt("##PathPoint", &pathPoint)) {
        //     if (pathPoint < 0) pathPoint = 0;
        //     params.PathPoint = static_cast<uint32_t>(pathPoint);
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetPathPoint")) {
        //     params.PathPoint = 0;
        // }

        // ImGui::Text("Bool");
        // ImGui::SameLine();

        // bool theBool = params.Bool.value();
        // if (ImGui::Checkbox("##Bool", &theBool)) {
        //     params.Bool = theBool;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetBool")) {
        //     params.Bool = false;
        // }

        // ImGui::Text("Speed");
        // ImGui::SameLine();

        float speed = obj->Speed;
        if (ImGui::DragFloat("##Speed", &speed, 0.1f)) {
            obj->Speed = speed;
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
            obj->Speed = 0.0f;
        }

        // ImGui::Text("SpeedB");
        // ImGui::SameLine();

        // float speed = params.SpeedB.value();
        // if (ImGui::DragFloat("##SpeedB", &speed, 0.1f)) {
        //     *params.SpeedB = speed;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_UNDO "##ResetSpeedB")) {
        //     *params.SpeedB = 0.0f;
        // }
    }, gEditor.eObjectPicker.eGizmo._selected);
}
