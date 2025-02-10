#include "Ship.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/ghostship_model.h"
#include "courses/harbour/ship2_model.h"
#include "courses/harbour/ship3_model.h"
}

AShip::AShip(FVector pos, AShip::Skin skin) {
    Spawn = pos;
    Spawn.y += 10;

    switch(skin) {
        case GHOSTSHIP:
            _skin = ghostship_Plane_mesh;
            break;
        case SHIP2:
            _skin = ship2_SoH_mesh;
            break;
        case SHIP3:
            _skin = ship3_2Ship_mesh;
            break;
    }
}

void AShip::Tick() {
    static float angle = 0.0f; // Keeps track of the ship's rotation around the circle
    float radius = 150.0f;      // The radius of the circular path
    float speed = 0.01f;       // Speed of rotation

    angle += speed; // Increment the angle to move in a circle

    // Update the position based on a circular path
    Pos.x = Spawn.x + radius * cosf(angle);
    Pos.z = Spawn.z + radius * sinf(angle);

    // Rotate to face forward along the circle
    Rot.yaw = -static_cast<int16_t>(angle * (32768.0f / M_PI / 2.0f));
}

void AShip::Draw(Camera *camera) {
    Mat4 shipMtx;
    Vec3f hullPos = {Pos.x, Pos.y, Pos.z};
    Vec3s hullRot = {Rot.pitch, Rot.yaw, Rot.roll};

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(shipMtx, hullPos, hullRot);
    mtxf_scale(shipMtx, 0.4);
    if (render_set_position(shipMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, _skin);
    }
}

bool AShip::IsMod() { return true; }
