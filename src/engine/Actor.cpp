#include <libultraship.h>
#include "Matrix.h"
#include "Actor.h"
#include "engine/World.h"

// Editor
#include "engine/editor/Collision.h"

extern "C" {
#include "math_util.h"
}

AActor::AActor() {}
AActor::AActor(SpawnParams params) {
    ResourceName = "mk:actor"; // This needs to be overridden in derived classes
    SpawnPos = params.Location.value_or(FVector{0.0f, 0.0f, 0.0f});
    SpawnRot = params.Rotation.value_or(IRotator{0, 0, 0});
    SpawnScale = params.Scale.value_or(FVector(0, 0, 0));
    Speed = params.Speed.value_or(0.0f);
}

void AActor::BeginPlay() {
    // This makes actors clickable in the editor
    if (Editor_IsEnabled()) {
        if ((nullptr != Model) && (Model[0] != '\0')) {
            // Prevent collision mesh from being generated extra times.
            if (Triangles.size() == 0) {
                Editor::GenerateCollisionMesh(this, (Gfx*)LOAD_ASSET_RAW(Model), 1.0f);
            }
        }
    }
}

// Virtual functions to be overridden by derived classes
void AActor::Tick() {  }
void AActor::Draw(Camera *camera) {
    if (Model) {
        Mat4 mtx;

        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

        ApplyMatrixTransformations(mtx, *(FVector*)Pos, *(IRotator*)Rot, Scale);
        if (render_set_position(mtx, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)Model);
        }
    }
}
void AActor::Collision(Player* player, AActor* actor) {}
void AActor::VehicleCollision(s32 playerId, Player* player){}
void AActor::Destroy() {
    bPendingDestroy = true;
}
bool AActor::IsMod() { return false; }
void AActor::SetLocation(FVector pos) {
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
}
FVector AActor::GetLocation() const {
    return FVector(Pos[0], Pos[1], Pos[2]);
}

IRotator AActor::GetRotation() const {
    IRotator rot;
    rot.Set(Rot[0], Rot[1], Rot[2]);
    return rot;
}

FVector AActor::GetScale() const {
    return Scale;
}

void AActor::SetSpawnParams(SpawnParams& params) {
    params.Name = ResourceName;
    params.Location = SpawnPos;
    params.Rotation = SpawnRot;
    params.Scale = SpawnScale;
    params.Speed = Speed;
}

void AActor::Translate(FVector pos) {
    SpawnPos = pos;
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
}

void AActor::Rotate(IRotator rot) {
    SpawnRot = rot;
    Rot[0] = rot.pitch;
    Rot[1] = rot.yaw;
    Rot[2] = rot.roll;
}

void AActor::SetScale(FVector scale) {
    SpawnScale = scale;
    Scale = scale;
}

