#include "Starship.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/hm64/starship_model.h"
}

AStarship::AStarship(FVector pos) {
    Pos = pos;
}

void AStarship::Tick() {
    static float angle = 0.0f; // Keeps track of the ship's rotation around the circle
    float radius = 150.0f;      // The radius of the circular path
    float speed = 0.01f;       // Speed of rotation

    angle += speed; // Increment the angle to move in a circle

    // Update the position based on a circular path
    Pos.x = radius * cosf(angle);
    Pos.z = radius * sinf(angle);

    // Rotate to face forward along the circle
    Rot.yaw = angle * (180.0f / M_PI) + 90.0f; // Add 90° offset
}




void AStarship::Draw(Camera *camera) {
    Mat4 shipMtx;
    Vec3f hullPos = {Pos.x, Pos.y, Pos.z};
    Vec3s hullRot = {Rot.pitch, Rot.yaw, Rot.roll};

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(shipMtx, hullPos, hullRot);
    mtxf_scale(shipMtx, 0.4);
    if (render_set_position(shipMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, starship_Cube_mesh);
    }
}

bool AStarship::IsMod() { return true; }
