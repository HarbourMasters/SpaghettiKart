#include "Starship.h"

#include <libultra/gbi.h>
#include "Matrix.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "courses/harbour/starship_model.h"
}

AStarship::AStarship(FVector pos) {
    Name = "Starship";
    Spawn = pos;
    SetLocation(pos);
    Scale = FVector(1.5, 1.5, 1.5);
    Model = starship_Cube_mesh;
}

void AStarship::Tick() {
    static float angle = 0.0f;
    float radius = 150.0f;
    float speed = 0.01f;

    angle += speed;

    // Move relative to the initial position

    FVector pos = GetLocation();
    pos.x = Spawn.x + radius * cosf(angle);
    pos.z = Spawn.z + radius * sinf(angle);
    SetLocation(pos);

    // Keep y from changing (or adjust it if necessary)
    //Pos.y = Spawn.y;

    // Rotate to face forward along the circle
    Rot[1] = angle * (180.0f / M_PI) + 90.0f;
}

bool AStarship::IsMod() { return true; }
