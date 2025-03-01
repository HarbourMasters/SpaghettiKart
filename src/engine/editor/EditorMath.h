#pragma once

#include <libultraship.h>
#include "../CoreMath.h"

extern "C" {
#include "common_structs.h"
}

enum class CollisionType {
    VTX_INTERSECT,
    BOUNDING_BOX,
    BOUNDING_SPHERE
};

struct Ray {
    FVector Origin;
    FVector Direction;
};

struct Triangle {
    FVector v0, v1, v2;
};

struct GameObject {
    FVector* Pos;
    Gfx* Model;
    std::vector<Triangle> Triangles;
    CollisionType Collision;
    float BoundingBoxSize;
};

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
void DrawObj();

bool IntersectRayTriangle(const Ray& ray, const Triangle& tri, const FVector& objectPos, float& t);



