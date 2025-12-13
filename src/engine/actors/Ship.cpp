#include "Ship.h"

#include <libultra/gbi.h>
#include "CoreMath.h"
#include "port/Game.h"
#include "Matrix.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/ghostship_model.h"
#include "courses/harbour/ship2_model.h"
#include "courses/harbour/ship3_model.h"
}

AShip::AShip(const SpawnParams& params) : AActor(params) {
    ResourceName = "hm:ship";
    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    //Spawn.y += 10;
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
    Scale = FVector(0.4, 0.4, 0.4);

    SpawnSkin = static_cast<AShip::Skin>(params.Type.value_or(0));
    switch(SpawnSkin) {
        case Skin::GHOSTSHIP:
            Name = "Ghostship (HM64)";
            _skin = ghostship_Plane_mesh;
            break;
        case Skin::SHIP2:
            Name = "Ship 1 (HM64)";
            _skin = ship2_SoH_mesh;
            break;
        case Skin::SHIP3:
            Name = "Ship 2 (HM64)";
            _skin = ship3_2Ship_mesh;
            break;
    }
    Model = (const char*)_skin;
}

void AShip::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = ResourceName;
    params.Type = static_cast<int16_t>(SpawnSkin);
    params.Location = SpawnPos;
    params.Rotation = SpawnRot;
    params.Scale = SpawnScale;
    params.Speed = Speed;
}

void AShip::BeginPlay() {
    // Prevent collision mesh from being generated extra times.
    if (Editor_IsEnabled()) {
        if (Triangles.size() == 0) {
            Editor::GenerateCollisionMesh(this, (Gfx*)_skin, Scale.y);
        }
    }
}

void AShip::Tick() {
    // static float angle = 0.0f; // Keeps track of the ship's rotation around the circle
    // float radius = 150.0f;      // The radius of the circular path
    // float speed = 0.01f;       // Speed of rotation

    // angle += speed; // Increment the angle to move in a circle

    // // Update the position based on a circular path
    // Pos.x = Spawn.x + radius * cosf(angle);
    // Pos.z = Spawn.z + radius * sinf(angle);

    // // Rotate to face forward along the circle
    // Rot.yaw = -static_cast<int16_t>(angle * (32768.0f / M_PI / 2.0f));
}

bool AShip::IsMod() { return true; }

void AShip::DrawEditorProperties() {
    ImGui::Text("Ship Type");
    ImGui::SameLine();

    int32_t type = static_cast<int32_t>(SpawnSkin);
    const char* items[] = { "Ghostship", "Ship 2", "Ship 3" };

    if (ImGui::Combo("##Type", &type, items, IM_ARRAYSIZE(items))) {
        SpawnSkin = static_cast<AShip::Skin>(type);

        switch(SpawnSkin) {
            case Skin::GHOSTSHIP:
                Name = "Ghostship";
                _skin = ghostship_Plane_mesh;
                break;
            case Skin::SHIP2:
                Name = "Ship_1";
                _skin = ship2_SoH_mesh;
                break;
            case Skin::SHIP3:
                Name = "Ship_2";
                _skin = ship3_2Ship_mesh;
                break;
        }
        Model = (const char*)_skin;
    }

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
}
