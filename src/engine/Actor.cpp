#include <libultraship.h>
#include "Matrix.h"

#include "Actor.h"

extern "C" {
#include "math_util.h"
}

AActor::AActor() {}

// Virtual functions to be overridden by derived classes
void AActor::Tick() {  }
void AActor::Draw(Camera *camera) {
    if (Model) {
        Mat4 mtx;

        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

        ApplyMatrixTransformations(mtx, *(FVector*)Pos, *(IRotator*)Rot, Scale);
        if (render_set_position(mtx, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, Model);
        }
    }
}
void AActor::Collision(Player* player, AActor* actor) {}
void AActor::VehicleCollision(s32 playerId, Player* player){}
void AActor::Destroy() {
    // Set uuid to zero.
    memset(uuid, 0, sizeof(uuid));
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
