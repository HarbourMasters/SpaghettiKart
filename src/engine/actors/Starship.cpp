#include "Starship.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/starship_model.h"
}

AStarship::AStarship(FVector pos) {
    Spawn = pos;
}

void AStarship::Tick() {
    static float angle = 0.0f;
    float radius = 150.0f;
    float speed = 0.01f;

    angle += speed;

    // Move relative to the initial position
    Pos.x = Spawn.x + radius * cosf(angle);
    Pos.z = Spawn.z + radius * sinf(angle);

    // Keep y from changing (or adjust it if necessary)
    Pos.y = Spawn.y;

    // Rotate to face forward along the circle
    Rot.yaw = angle * (180.0f / M_PI) + 90.0f;
}

void AStarship::Draw(Camera *camera) {
    Mat4 shipMtx;
    Vec3f hullPos = {Pos.x, Pos.y, Pos.z};
    Vec3s hullRot = {Rot.pitch, Rot.yaw, Rot.roll};

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(shipMtx, hullPos, hullRot);
    mtxf_scale(shipMtx, 1.5);
    if (render_set_position(shipMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, starship_Cube_mesh);
    }
}

bool AStarship::IsMod() { return true; }
