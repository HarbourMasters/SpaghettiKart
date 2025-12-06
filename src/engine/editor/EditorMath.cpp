#include "EditorMath.h"

#include <libultraship/libultraship.h>
#include "port/Game.h"
#include "port/Engine.h"
#include <libultra/types.h>
#include "GameObject.h"

#include <vector>
#include <limits>
#include <cmath>

#include <graphic/Fast3D/Fast3dWindow.h>
#include <graphic/Fast3D/interpreter.h>

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "defines.h"
#include "actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "camera.h"
}

std::vector<Mtx> EditorMatrix;

bool IsInGameScreen() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mouse = wnd->GetMousePos();

    // Define viewport boundaries
    auto gfx_current_game_window_viewport = GetInterpreter()->mGameWindowViewport;
    int left = gfx_current_game_window_viewport.x;
    int right = left + OTRGetGameRenderWidth();
    int top = gfx_current_game_window_viewport.y;
    int bottom = top + OTRGetGameRenderHeight();

    // Check if the mouse is within the game render area
    return (mouse.x >= left && mouse.x < right) && (mouse.y >= top && mouse.y < bottom);
}

FVector ScreenRayTrace() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Camera* camera = gEditor.eCamera;

    Ship::Coords mouse = wnd->GetMousePos();
    auto gfx_current_game_window_viewport = GetInterpreter()->mGameWindowViewport;
    mouse.x -= gfx_current_game_window_viewport.x;
    mouse.y -= gfx_current_game_window_viewport.y;

    // Get screen dimensions
    uint32_t width = OTRGetGameViewportWidth();
    uint32_t height = OTRGetGameViewportHeight();

    // Convert mouse to NDS screen coordinates
    float x = (2.0f * mouse.x) / width - 1.0f;  // Normalized X: -1 to 1
    float y = 1.0f - (2.0f * mouse.y) / height; // Normalized Y: -1 to 1
    float z = 1.0f; // z is typically 1.0 for the near plane

    FVector4 rayClip = {x, y, z, 1.0f};

    Mat4 perspMtx;
    u16 perspNorm;
    guPerspectiveF(perspMtx, &perspNorm, gCameraFOV[0], OTRGetAspectRatio(), CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);

    Mat4 inversePerspMtx;
    if (InverseMatrix((float*)&perspMtx, (float*)&inversePerspMtx) != 2) {
        FVector4 rayEye = MultiplyMatrixVector(inversePerspMtx, (float*)&rayClip.x);

        Mat4 lookAtMtx;
        guLookAtF(lookAtMtx, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0], camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
        Mat4 inverseViewMtx;
        if (InverseMatrix((float*)&lookAtMtx, (float*)&inverseViewMtx[0][0]) != 2) {
            rayEye.w = 0;
            FVector4 invRayWor = MultiplyMatrixVector(inverseViewMtx, (float*)&rayEye.x);

            FVector direction;
            direction = FVector(invRayWor.x, invRayWor.y, invRayWor.z);

            return direction;
        }
    }
    return FVector(0, 0, 0);
}

bool QueryCollisionRayActor(Vec3f rayOrigin, Vec3f rayDir, Vec3f actorMin, Vec3f actorMax, float* t) {
    float tmin = -FLT_MAX, tmax = FLT_MAX;

    for (size_t i = 0; i < 3; i++) {
        if (fabs(rayDir[i]) > 1e-6f) { // Avoid division by zero
            float t1 = (actorMin[i] - rayOrigin[i]) / rayDir[i];
            float t2 = (actorMax[i] - rayOrigin[i]) / rayDir[i];

            if (t1 > t2) { float temp = t1; t1 = t2; t2 = temp; }

            tmin = fmax(tmin, t1);
            tmax = fmin(tmax, t2);

            if (tmax < tmin) return false; // No intersection
        } else if (rayOrigin[i] < actorMin[i] || rayOrigin[i] > actorMax[i]) {
            return false; // Ray is outside the slab
        }
    }

    *t = tmin; // Distance to first intersection
    return true;
}

FVector4 MultiplyMatrixVector(float matrix[4][4], float vector[4]) {
    FVector4 result;
    float* resultPtr = &result.x;
    for (size_t i = 0; i < 4; i++) {
        resultPtr[i] = 0;
        for (size_t j = 0; j < 4; j++) {
            resultPtr[i] += matrix[j][i] * vector[j];  // Swap [i][j] → [j][i] for column order
        }
    }
    return result;
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
static bool InverseMatrix(const float m[16], float invOut[16]) {
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
             m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
             m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
             m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
              m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] -
             m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] +
             m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] -
             m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +
              m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] +
             m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] -
             m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] +
              m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] -
              m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0) {
        return false;
    }

    det = 1.0 / det;

    for (i = 0; i < 16; i++) {
        invOut[i] = inv[i] * det;
    }

    return true;
}

FVector TransformVecByMatrix(const FVector& vec, const float mtx[4][4]) {
    FVector result;
    result.x = vec.x * mtx[0][0] + vec.y * mtx[1][0] + vec.z * mtx[2][0] + mtx[3][0];
    result.y = vec.x * mtx[0][1] + vec.y * mtx[1][1] + vec.z * mtx[2][1] + mtx[3][1];
    result.z = vec.x * mtx[0][2] + vec.y * mtx[1][2] + vec.z * mtx[2][2] + mtx[3][2];
    return result;
}

FVector TransformVecDirection(const FVector& dir, const float mtx[4][4]) {
    FVector result;
    result.x = dir.x * mtx[0][0] + dir.y * mtx[1][0] + dir.z * mtx[2][0];
    result.y = dir.x * mtx[0][1] + dir.y * mtx[1][1] + dir.z * mtx[2][1];
    result.z = dir.x * mtx[0][2] + dir.y * mtx[1][2] + dir.z * mtx[2][2];
    return result;
}

Ray RayToLocalSpace(MtxF mtx, const Ray& ray) {
    MtxF inverse;

    if (InverseMatrix((float*)&mtx, (float*)&inverse) != 2) {
        FVector localRayOrigin = TransformVecByMatrix(ray.Origin, (float(*)[4])&inverse);
        FVector localRayDir = TransformVecDirection(ray.Direction, (float(*)[4])&inverse);
        return Ray{localRayOrigin, localRayDir.Normalize()};
    }
    return Ray{}; // Fail. Return empty ray
}

bool IntersectRayTriangle(const Ray& ray, const Triangle& tri, float& t) {
    constexpr float EPSILON = 1e-6f;

    // Adjust the triangle vertices by the object's position
    FVector v0 = tri.v0;
    FVector v1 = tri.v1;
    FVector v2 = tri.v2;

    FVector edge1 = v1 - v0;
    FVector edge2 = v2 - v0;
    FVector h = ray.Direction.Cross(edge2);
    float a = edge1.Dot(h);

    if (std::abs(a) < EPSILON)
        return false; // Ray is parallel to triangle

    float f = 1.0f / a;
    FVector s = ray.Origin - v0;
    float u = f * s.Dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    FVector q = s.Cross(edge1);
    float v = f * ray.Direction.Dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * edge2.Dot(q);
    return t > EPSILON;
}

bool IntersectRayTriangleAndTransform(const Ray& ray, FVector pos, const Triangle& tri, float& t) {
    constexpr float EPSILON = 1e-6f;

    // Adjust the triangle vertices by the object's position
    FVector v0 = tri.v0 + pos;
    FVector v1 = tri.v1 + pos;
    FVector v2 = tri.v2 + pos;

    FVector edge1 = v1 - v0;
    FVector edge2 = v2 - v0;
    FVector h = ray.Direction.Cross(edge2);
    float a = edge1.Dot(h);

    if (std::abs(a) < EPSILON)
        return false; // Ray is parallel to triangle

    float f = 1.0f / a;
    FVector s = ray.Origin - v0;
    float u = f * s.Dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    FVector q = s.Cross(edge1);
    float v = f * ray.Direction.Dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * edge2.Dot(q);
    return t > EPSILON;
}

std::optional<FVector> QueryHandleIntersection(MtxF mtx, Ray ray, const Triangle& tri) {
    float t;
    Ray localRay = RayToLocalSpace(mtx, ray);
    if (IntersectRayTriangle(localRay, tri, t)) {
        FVector localClickPosition = localRay.Origin + localRay.Direction * t;
        FVector worldClickPosition = TransformVecByMatrix(localClickPosition, (float(*)[4])&mtx);
        return worldClickPosition; // Stop checking objects if we selected a Gizmo handle
    }
    return std::nullopt;
}

bool IntersectRaySphere(const Ray& ray, const FVector& sphereCenter, float radius, float& t) {
    const float EPSILON = 1e-6f;

    // Vector from ray origin to sphere center
    FVector oc = ray.Origin - sphereCenter;

    // Quadratic equation coefficients
    float a = ray.Direction.Dot(ray.Direction);
    float b = 2.0f * oc.Dot(ray.Direction);
    float c = oc.Dot(oc) - (radius * radius);

    // Compute discriminant
    float discriminant = (b * b) - (4 * a * c);

    // No intersection if discriminant is negative
    if (discriminant < 0) {
        return false;
    }

    // Compute nearest intersection point
    float sqrtD = sqrtf(discriminant);
    float t0 = (-b - sqrtD) / (2.0f * a);
    float t1 = (-b + sqrtD) / (2.0f * a);

    // Select the closest valid intersection
    if (t0 > EPSILON) {
        t = t0;
        return true;
    } else if (t1 > EPSILON) {
        t = t1;
        return true;
    }

    return false; // Sphere is behind the ray origin
}

// bool FindClosestObject(const Ray& ray, const std::vector<GameObject*>& objects, GameObject* outObject, float& outDistance) {
//     float closestDist = std::numeric_limits<float>::max();
//     bool found = false;

//     for (const auto& obj : objects) {
//         for (const auto& tri : obj.Triangles) {
//             float t;
//             if (IntersectRayTriangle(ray, tri, *obj.Pos, t) && t < closestDist) {
//                 closestDist = t;
//                 outObject = obj;
//                 found = true;
//             }
//         }
//     }

//     if (found) {
//         outDistance = closestDist;
//         return true;
//     }
    
//     return false;
// }

// Transform a matrix to a matrix identity
void Editor_MatrixIdentity(Mat4 mtx) {
    s32 i;
    s32 k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 4; k++) {
            mtx[i][k] = (i == k) ? 1.0f : 0.0f;
        }
    }
}

void Editor_AddMatrix(Mat4 mtx, int32_t flags) {
    EditorMatrix.emplace_back();
    guMtxF2L(mtx, &EditorMatrix.back());
    gSPMatrix(gDisplayListHead++, &EditorMatrix.back(), flags);
}

float CalculateAngle(const FVector& start, const FVector& end) {
    float dot = start.Dot(end);

    float magStart = start.Magnitude();
    float magEnd = end.Magnitude();

    float cosAngle = dot / (magStart * magEnd);
    cosAngle = std::min(1.0f, std::max(-1.0f, cosAngle));

    return acos(cosAngle);
}

void SetDirectionFromRotator(IRotator rot, s8 direction[3]) {
    rot.yaw += 0xC000; //! @warning dumb hack to align the light properly
    float yaw = (rot.yaw) * (M_PI / 32768.0f);  // Convert from n64 binary angles 0-0xFFFF 0-360 degrees to radians
    float pitch = rot.pitch * (M_PI / 32768.0f);

    // Compute unit direction vector
    float x = cosf(yaw) * cosf(pitch);
    float y = -sinf(pitch);
    float z = -sinf(yaw) * cosf(pitch);

    // Scale into -127 to 127 range (not 128 to avoid overflow)
    direction[0] = static_cast<s8>(x * 127.0f);
    direction[1] = static_cast<s8>(y * 127.0f);
    direction[2] = static_cast<s8>(z * 127.0f);

    //printf("Light dir %d %d %d (from rot 0x%X 0x%X 0x%X)\n", direction[0], direction[1], direction[2], rotator[0], rotator[1], rotator[2]);
}

void SetRotatorFromDirection(FVector direction, IRotator* rot) {
    // Compute pitch (inverse of -sinf(pitch))
    float pitch = -asinf(direction.y);

    // Compute yaw (inverse of cosf(yaw) * cosf(pitch))
    float yaw = atan2f(-direction.z, direction.x);

    // Convert back to N64 angles (0-0xFFFF range)
    rot->pitch = (s16)(pitch * (32768.0f / M_PI));
    rot->yaw = (s16)(yaw * (32768.0f / M_PI));
    rot->roll = 0; // Assume no roll, since it's undefined from direction alone
}

FVector GetPositionAheadOfCamera(f32 dist) {
    Camera* camera = gEditor.eCamera;
    FVector pos = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);

    f32 pitch = (camera->rot[2] / 65535.0f) * 360.0f;
    f32 yaw = (camera->rot[1] / 65535.0f) * 360.0f;

    // Convert degrees to radians
    pitch = pitch * M_PI / 180.0f;
    yaw = yaw * M_PI / 180.0f;

    // Compute forward vector
    FVector forward(
        sinf(yaw),  // X
        -sinf(pitch), // Y
        cosf(yaw)               // Z (vertical component)
    );

    // Move 1000 units forward from the camera position
    return pos + (forward * dist);
}
