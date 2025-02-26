#pragma once

#include <libultraship.h>
#include "../CoreMath.h"

extern "C" {
#include "common_structs.h"
}

/**
 * Projects 2D cursor into the game world
 * 
 * @return FVector ray direction
 * 
 * ray.x = camera->pos[0] + direction.x * length;
 * ray.y = camera->pos[1] + direction.y * length;
 * ray.z = camera->pos[2] + direction.z * length;
 */
FVector ScreenRayTrace();
bool QueryCollisionRayActor(Vec3f rayOrigin, Vec3f rayDir, Vec3f actorMin, Vec3f actorMax, float* t);
FVector4 MultiplyMatrixVector(float matrix[4][4], float vector[4]);
s32 Inverse(MtxF* src, MtxF* dest);
void Copy(MtxF* src, MtxF* dest);
void Clear(MtxF* mf);


