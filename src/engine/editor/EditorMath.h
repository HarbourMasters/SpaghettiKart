#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include <libultra/types.h>
#include "../CoreMath.h"
#include <vector>
#include "GameObject.h"
#include <graphic/Fast3D/Fast3dWindow.h>
#include <graphic/Fast3D/interpreter.h>

extern "C" {
#include "common_structs.h"
}

extern std::vector<Mtx> EditorMatrix;

struct Ray {
    FVector Origin;
    FVector Direction;
};

struct Triangle {
    FVector v0, v1, v2;
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
bool IsInGameScreen();
FVector ScreenRayTrace();
bool QueryCollisionRayActor(Vec3f rayOrigin, Vec3f rayDir, Vec3f actorMin, Vec3f actorMax, float* t);
FVector4 MultiplyMatrixVector(float matrix[4][4], float vector[4]);
static bool InverseMatrix(const float m[16], float invOut[16]);
void Copy(MtxF* src, MtxF* dest);
void Clear(MtxF* mf);

FVector TransformVecByMatrix(const FVector& vec, const float mtx[4][4]);
FVector TransformVecDirection(const FVector& dir, const float mtx[4][4]);
Ray RayToLocalSpace(MtxF mtx, const Ray& ray);
bool IntersectRayTriangle(const Ray& ray, const Triangle& tri, float& t); // Uses local space not global space.
bool IntersectRayTriangleAndTransform(const Ray& ray, FVector pos, const Triangle& tri, float& t); // Uses global space because no access to mtx
bool IntersectRaySphere(const Ray& ray, const FVector& sphereCenter, float radius, float& t);
/**
 * optional used here so we can check for successful query and return the click position
 */
std::optional<FVector> QueryHandleIntersection(MtxF mtx, Ray ray, const Triangle& tri);

void Editor_MatrixIdentity(Mat4 mtx);
void Editor_AddMatrix(Mat4 mtx, int32_t flags);
float CalculateAngle(const FVector& start, const FVector& end);
void SetDirectionFromRotator(IRotator rot, s8 direction[3]);
void SetRotatorFromDirection(FVector direction, IRotator* rot);
FVector GetPositionAheadOfCamera(f32 dist);
