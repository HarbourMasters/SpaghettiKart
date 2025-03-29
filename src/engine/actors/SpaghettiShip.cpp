#include "SpaghettiShip.h"

#include <libultra/gbi.h>
#include "Matrix.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/ship_model.h"
}

ASpaghettiShip::ASpaghettiShip(FVector pos) {
    Name = "Spaghetti Ship";
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
    Spawn = pos;
    Spawn.y += 10;
    Scale = {0.4, 0.4, 0.4};
}

void ASpaghettiShip::Tick() {
    static float angle = 0.0f; // Keeps track of the ship's rotation around the circle
    float radius = 150.0f;      // The radius of the circular path
    float speed = 0.01f;       // Speed of rotation

    angle += speed; // Increment the angle to move in a circle

    // Update the position based on a circular path
    Pos[0] = Spawn.x + radius * cosf(angle);
    Pos[2] = Spawn.z + radius * sinf(angle);

    // Rotate to face forward along the circle
    Rot[1] = -static_cast<int16_t>(angle * (32768.0f / M_PI / 2.0f));

    WheelRot.pitch += 500;
}

void ASpaghettiShip::Draw(Camera *camera) {
    Mat4 shipMtx;
    Mat4 objectMtx;
    Mat4 resultMtx;
    Vec3f hullPos = {Pos[0], Pos[1], Pos[2]};
    Vec3s hullRot = {Rot[0], Rot[1], Rot[2]};
    Vec3s rot = {WheelRot.pitch, WheelRot.yaw, WheelRot.roll};

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    ApplyMatrixTransformations(shipMtx, *(FVector*)Pos, *(IRotator*)Rot, Scale);
    if (render_set_position(shipMtx, 0) != 0) {}

    // Render the ships hull
    ApplyMatrixTransformations(objectMtx, {0, 0, 0}, {0, 0, 0}, {1, 1, 1});
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, ship1_spag1_mesh);
    }

    // Front tyre
    ApplyMatrixTransformations(objectMtx, FVector(0, 0, 110), IRotator(0, 0, 0), FVector(1, 1, 1));
    mtxf_identity(shipMtx);
    AddLocalRotation(shipMtx, WheelRot);
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, wheels_Spaghetti_002_mesh);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    
    // Back tyre
    AddLocalRotation(shipMtx, WheelRot);
    ApplyMatrixTransformations(objectMtx, FVector(0, 0, -165), {0, 0, 0}, {1, 1, 1});
    mtxf_multiplication(resultMtx, shipMtx, objectMtx);
    if (render_set_position(resultMtx, 3) != 0) {
        gSPDisplayList(gDisplayListHead++, wheels_Spaghetti_002_mesh);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

bool ASpaghettiShip::IsMod() { return true; }
