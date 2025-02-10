#include "SpaghettiShip.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/ship_model.h"
}

ASpaghettiShip::ASpaghettiShip(FVector pos) {
    Spawn = pos;
    Spawn.y += 10;
}

void ASpaghettiShip::Tick() {
    static float angle = 0.0f; // Keeps track of the ship's rotation around the circle
    float radius = 150.0f;      // The radius of the circular path
    float speed = 0.01f;       // Speed of rotation

    angle += speed; // Increment the angle to move in a circle

    // Update the position based on a circular path
    Pos.x = Spawn.x + radius * cosf(angle);
    Pos.z = Spawn.z + radius * sinf(angle);

    // Rotate to face forward along the circle
    Rot.yaw = -static_cast<int16_t>(angle * (32768.0f / M_PI / 2.0f));


    WheelRot.pitch += 500;
}

void ASpaghettiShip::Draw(Camera *camera) {
    Mat4 shipMtx;
    Mat4 objectMtx;
    Mat4 resultMtx;
    Vec3f hullPos = {Pos.x, Pos.y, Pos.z};
    Vec3s hullRot = {Rot.pitch, Rot.yaw, Rot.roll};
    Vec3s rot = {WheelRot.pitch, WheelRot.yaw, WheelRot.roll};

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    // empty/null mtx as a base
    mtxf_pos_rotation_xyz(shipMtx, hullPos, hullRot);
    mtxf_scale(shipMtx, 0.4);
    if (render_set_position(shipMtx, 0) != 0) {}

    // Render the ships hull
    Vec3f hullPos2 = {0, 0, 0};
    mtxf_translate(objectMtx, hullPos2);
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, ship1_Spaghetti_mesh);
    }

    // Front tyre
    Vec3f pos = {0, 0, 110};
    mtxf_rotate_x(shipMtx, WheelRot.pitch);
    mtxf_translate(objectMtx, pos);
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, wheels_Spaghetti_002_mesh);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    
    // Back tyre
    Vec3f pos2 = {0, 0, -165};
    mtxf_rotate_x(shipMtx, WheelRot.pitch);
    mtxf_translate(objectMtx, pos2);
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, wheels_Spaghetti_002_mesh);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

bool ASpaghettiShip::IsMod() { return true; }
