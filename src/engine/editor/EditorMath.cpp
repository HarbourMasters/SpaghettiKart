#include "EditorMath.h"

#include <libultraship/libultraship.h>
#include "port/Game.h"
#include "port/Engine.h"
#include <libultra/types.h>
#include "GameObject.h"

#include <vector>
#include <limits>
#include <cmath>

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
    int left = gfx_current_game_window_viewport.x;
    int right = left + OTRGetGameRenderWidth();
    int top = gfx_current_game_window_viewport.y;
    int bottom = top + OTRGetGameRenderHeight();

    // Check if the mouse is within the game render area
    return (mouse.x >= left && mouse.x < right) && (mouse.y >= top && mouse.y < bottom);
}

FVector ScreenRayTrace() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Camera* camera = &cameras[0];

    Ship::Coords mouse = wnd->GetMousePos();
    mouse.x -= gfx_current_game_window_viewport.x;
    mouse.y -= gfx_current_game_window_viewport.y;

    // Get screen dimensions
    uint32_t width = OTRGetGameRenderWidth();
    uint32_t height = OTRGetGameRenderHeight();

    // Convert mouse to NDS screen coordinates
    float x = (2.0f * mouse.x) / width - 1.0f;  // Normalized X: -1 to 1
    float y = 1.0f - (2.0f * mouse.y) / height; // Normalized Y: -1 to 1
    float z = 1.0f; // z is typically 1.0 for the near plane

    FVector4 rayClip = {x, y, z, 1.0f};

    Mat4 perspMtx;
    u16 perspNorm;
    guPerspectiveF(perspMtx, &perspNorm, gCameraZoom[0], OTRGetAspectRatio(), CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);

    Mat4 inversePerspMtx;
    if (Inverse((MtxF*)&perspMtx, (MtxF*)&inversePerspMtx) != 2) {
        FVector4 rayEye = MultiplyMatrixVector(inversePerspMtx, (float*)&rayClip.x);

        Mat4 lookAtMtx;
        guLookAtF(lookAtMtx, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0], camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
        Mat4 inverseViewMtx;
        if (Inverse((MtxF*)&lookAtMtx, (MtxF*)&inverseViewMtx[0][0]) != 2) {
            rayEye.w = 0;
            FVector4 invRayWor = MultiplyMatrixVector(inverseViewMtx, (float*)&rayEye.x);

            FVector direction;
            direction = FVector(invRayWor.x, invRayWor.y, invRayWor.z);

            return direction;
        }
    }
}

bool QueryCollisionRayActor(Vec3f rayOrigin, Vec3f rayDir, Vec3f actorMin, Vec3f actorMax, float* t) {
    float tmin = -FLT_MAX, tmax = FLT_MAX;

    for (int i = 0; i < 3; i++) {
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
    for (int i = 0; i < 4; i++) {
        resultPtr[i] = 0;
        for (int j = 0; j < 4; j++) {
            resultPtr[i] += matrix[j][i] * vector[j];  // Swap [i][j] → [j][i] for column order
        }
    }
    return result;
}

s32 Inverse(MtxF* src, MtxF* dest) {
    MtxF mfCopy;
    s32 i;
    s32 pad;
    f32 temp2;
    f32 temp1;
    s32 thisCol;
    s32 thisRow;

    Copy(src, &mfCopy);
    Clear(dest);
    for (thisCol = 0; thisCol < 4; thisCol++) {
        thisRow = thisCol;
        while ((thisRow < 4) && (fabsf(mfCopy.mf[thisCol][thisRow]) < 0.0005f)) {
            thisRow++;
        }
        if (thisRow == 4) {
            // Reaching row = 4 means the column is either all 0 or a duplicate column.
            // Therefore src is a singular matrix (0 determinant).

            // osSyncPrintf(VT_COL(YELLOW, BLACK));
            // osSyncPrintf("Skin_Matrix_InverseMatrix():逆行列つくれません\n");
            // osSyncPrintf(VT_RST);
            return 2;
        }

        if (thisRow != thisCol) {
            // Diagonal element mf[thisCol][thisCol] is zero.
            // Swap the rows thisCol and thisRow.
            for (i = 0; i < 4; i++) {
                temp1 = mfCopy.mf[i][thisRow];
                mfCopy.mf[i][thisRow] = mfCopy.mf[i][thisCol];
                mfCopy.mf[i][thisCol] = temp1;

                temp2 = dest->mf[i][thisRow];
                dest->mf[i][thisRow] = dest->mf[i][thisCol];
                dest->mf[i][thisCol] = temp2;
            }
        }

        // Scale this whole row such that the diagonal element is 1.
        temp1 = mfCopy.mf[thisCol][thisCol];
        for (i = 0; i < 4; i++) {
            mfCopy.mf[i][thisCol] /= temp1;
            dest->mf[i][thisCol] /= temp1;
        }

        for (thisRow = 0; thisRow < 4; thisRow++) {
            if (thisRow != thisCol) {
                temp1 = mfCopy.mf[thisCol][thisRow];
                for (i = 0; i < 4; i++) {
                    mfCopy.mf[i][thisRow] -= mfCopy.mf[i][thisCol] * temp1;
                    dest->mf[i][thisRow] -= dest->mf[i][thisCol] * temp1;
                }
            }
        }
    }
    return 0;
}

void Copy(MtxF* src, MtxF* dest) {
    dest->xx = src->xx;
    dest->yx = src->yx;
    dest->zx = src->zx;
    dest->wx = src->wx;
    dest->xy = src->xy;
    dest->yy = src->yy;
    dest->zy = src->zy;
    dest->wy = src->wy;
    dest->xz = src->xz;
    dest->yz = src->yz;
    dest->zz = src->zz;
    dest->wz = src->wz;
    dest->xw = src->xw;
    dest->yw = src->yw;
    dest->zw = src->zw;
    dest->ww = src->ww;
}

void Clear(MtxF* mf) {
    mf->xx = 1.0f;
    mf->yy = 1.0f;
    mf->zz = 1.0f;
    mf->ww = 1.0f;
    mf->yx = 0.0f;
    mf->zx = 0.0f;
    mf->wx = 0.0f;
    mf->xy = 0.0f;
    mf->zy = 0.0f;
    mf->wy = 0.0f;
    mf->xz = 0.0f;
    mf->yz = 0.0f;
    mf->wz = 0.0f;
    mf->xw = 0.0f;
    mf->yw = 0.0f;
    mf->zw = 0.0f;
}
FVector DebugPoss = {0, 0, 0};
bool IntersectRayTriangle(const Ray& ray, const Triangle& tri, const FVector& objectPos, float& t) {
    const float EPSILON = 1e-6f;

    // Adjust the triangle vertices by the object's position
    FVector v0 = tri.v0 + objectPos;
    FVector v1 = tri.v1 + objectPos;
    FVector v2 = tri.v2 + objectPos;

    DebugPoss = v0;

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
void Editor_Matrixidentity(Mat4 mtx) {
    register s32 i;
    register s32 k;

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
    FVector pos = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);

    f32 pitch = (cameras[0].rot[2] / 65535.0f) * 360.0f;
    f32 yaw = (cameras[0].rot[1] / 65535.0f) * 360.0f;

    // Convert degrees to radians
    pitch = pitch * M_PI / 180.0f;
    yaw = yaw * M_PI / 180.0f;

    // Compute forward vector
    FVector forward(
        -sinf(yaw),  // X
        -sinf(pitch), // Y
        cosf(yaw)               // Z (vertical component)
    );

    // Move 1000 units forward from the camera position
    return pos + (forward * dist);
}
