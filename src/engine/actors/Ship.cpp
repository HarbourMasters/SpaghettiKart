#include "Ship.h"

#include <libultra/gbi.h>
#include "CoreMath.h"
#include "Matrix.h"

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
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
    Scale = FVector(0.4, 0.4, 0.4);
    
    switch(skin) {
        case GHOSTSHIP:
            Name = "Ghostship";
            _skin = ghostship_Plane_mesh;
            break;
        case SHIP2:
            Name = "Ship_1";
            _skin = ship2_SoH_mesh;
            break;
        case SHIP3:
            Name = "Ship_2";
            _skin = ship3_2Ship_mesh;
            break;
    }
    Model = _skin;
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
