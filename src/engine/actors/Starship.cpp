#include "Starship.h"

#include <libultra/gbi.h>
#include "Matrix.h"
#include "port/Game.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/starship_model.h"
}

AStarship::AStarship(const SpawnParams& params) : AActor(params) {
    Name = "Starship";
    ResourceName = "hm:starship";
    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    SetLocation(pos);
    IRotator rot = params.Rotation.value_or(IRotator(0, 0, 0));
    Rot[0] = rot.pitch;
    Rot[1] = rot.yaw;
    Rot[2] = rot.roll;
    Scale = params.Scale.value_or(FVector(0, 0, 0));
    Speed = params.Speed.value_or(0.01f);
    SpeedB = params.SpeedB.value_or(150.0f);
    Model = (const char*)starship_Cube_mesh;
}

void AStarship::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.SpeedB = SpeedB;
}

void AStarship::BeginPlay() {
    // Prevent collision mesh from being generated extra times.
    if (Editor_IsEnabled()) {
        if (Triangles.size() == 0) {
            Editor::GenerateCollisionMesh(this, (Gfx*)Model, 1.0f);
        }
    }
}

void AStarship::Tick() {
    static float angle = 0.0f;

    angle += Speed;

    // Move relative to the initial position

    FVector pos = GetLocation();
    FVector spawn = SpawnPos;
    pos.x = spawn.x + SpeedB * cosf(angle);
    pos.z = spawn.z + SpeedB * sinf(angle);
    SetLocation(pos);

    // Keep y from changing (or adjust it if necessary)
    //Pos.y = Spawn.y;

    // Rotate to face forward along the circle
    Rot[1] = angle * (180.0f / M_PI) + 90.0f;
}

bool AStarship::IsMod() { return true; }

void AStarship::DrawEditorProperties() {
    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = FVector(Pos[0], Pos[1], Pos[2]);
    if (ImGui::DragFloat3("##Location", (float*)&location)) {
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
        FVector location = FVector(0, 0, 0);
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::Text("Rotation");
    ImGui::SameLine();

    IRotator objRot = GetRotation();

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
        Rotate(newRot);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
        IRotator rot = IRotator(0, 0, 0);
        Rotate(rot);
    }

    FVector scale = GetScale();
    ImGui::Text("Scale   ");
    ImGui::SameLine();

    ImGui::DragFloat3("##Scale", (float*)&scale, 0.1f);
    SetScale(scale);
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetScale")) {
        FVector scale = FVector(0.4f, 0.4f, 0.4f);
        SetScale(scale);
    }

    ImGui::Text("Speed");
    ImGui::SameLine();

    float speed = Speed;
    if (ImGui::DragFloat("##Speed", &speed, 0.01f)) {
        Speed = speed;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
        Speed = 0.0f;
    }

    ImGui::Text("Radius");
    ImGui::SameLine();

    float speed2 = SpeedB;
    if (ImGui::DragFloat("##SpeedB", &speed2, 5.0f)) {
        SpeedB = speed2;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeedB")) {
        SpeedB = 0.0f;
    }
}
