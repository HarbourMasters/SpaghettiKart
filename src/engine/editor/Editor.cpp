#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Editor.h"
#include "port/Engine.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>

#include "engine/actors/Ship.h"
#include "port/Game.h"

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "defines.h"
#include "actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "camera.h"
}

Editor::Editor() {

}

void Editor::Tick() {
    auto wnd = GameEngine::Instance->context->GetWindow();

    if (wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT)) {
        MouseClick();
        _draw = true;
    } else {
        _draw = false;
    }

    if (_heldActor != nullptr) {
        FVector ray = ScreenRayTrace();
        float length = 400.0f;
        _heldActor->Pos[0] = cameras[0].pos[0] + ray.x * length;
        _heldActor->Pos[1] = cameras[0].pos[1] + ray.y * length;
        _heldActor->Pos[2] = cameras[0].pos[2] + ray.z * length;
    }
}

void Editor::MouseClick() {
    FVector ray = ScreenRayTrace();
    //Actor* foundActor = nullptr;
    s32 type = 0;
    bool found = false;
    for (auto& actor : gWorldInstance.Actors) {
        float boundingBox = actor->BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }
        float t;
        float max = 2.0f;
        float min = -2.0f;
        Vec3f boxMin = { actor->Pos[0] + boundingBox * min, 
            actor->Pos[1] + boundingBox * min,
            actor->Pos[2] + boundingBox * min };

            Vec3f boxMax = { actor->Pos[0] + actor->BoundingBoxSize * max, 
                            actor->Pos[1] + actor->BoundingBoxSize * max, 
                            actor->Pos[2] + actor->BoundingBoxSize * max };

        if (Editor::QueryCollisionRayActor(cameras[0].pos, &ray.x, boxMin, boxMax, &t)) {
            if (actor == _heldActor) {
                _heldActor = nullptr;
                break;
            }
            found = true;
            //foundActor = &actor;
            type = actor->Type;
            _heldActor = actor;
            break;
        }
    }
    if (found) {
        printf("FOUND COLLISION %d\n", type);
    } else {
        printf("NO COLLISION\n");
        _heldActor = nullptr;
    }
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
FVector Editor::ScreenRayTrace() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Camera* camera = &cameras[0];

    Ship::Coords mouse = wnd->GetMousePos();

    // Get screen dimensions
    uint32_t width = wnd->GetWidth();
    uint32_t height = wnd->GetHeight();

    // Convert mouse to NDS screen coordinates
    float x = (2.0f * mouse.x) / width - 1.0f;  // Normalized X: -1 to 1
    float y = 1.0f - (2.0f * mouse.y) / height; // Normalized Y: -1 to 1
    float z = 1.0f; // z is typically 1.0 for the near plane

    FVector4 rayClip = {x, y, z, 1.0f};

    Mat4 perspMtx;
    u16 perspNorm;
    guPerspectiveF(perspMtx, &perspNorm, gCameraZoom[0], OTRGetAspectRatio(), CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);

    Mat4 inversePerspMtx;
    if (Editor::Inverse((MtxF*)&perspMtx, (MtxF*)&inversePerspMtx) != 2) {
        FVector4 rayEye = MultiplyMatrixVector(inversePerspMtx, (float*)&rayClip.x);

        Mat4 lookAtMtx;
        guLookAtF(lookAtMtx, camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0], camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
        Mat4 inverseViewMtx;
        if (Editor::Inverse((MtxF*)&lookAtMtx, (MtxF*)&inverseViewMtx[0][0]) != 2) {
            rayEye.w = 0;
            FVector4 invRayWor = MultiplyMatrixVector(inverseViewMtx, (float*)&rayEye.x);

            FVector direction;
            direction = FVector(invRayWor.x, invRayWor.y, invRayWor.z);

            return direction;
        }
    }
}

void Editor::Draw() {
    if (_draw) {
        DrawObj(20);
        DrawObj(100);
    }
}

void Editor::DrawObj(float length) {
    Mat4 mtx;
    Vec3f pos2 = { _ray[0], _ray[1], _ray[2] };
    mtxf_translate(mtx, pos2);
    mtxf_scale(mtx, 0.03);
    render_set_position(mtx, 0);
    gSPDisplayList(gDisplayListHead++, Editor::box_Cube_mesh);
}

FVector4 Editor::MultiplyMatrixVector(float matrix[4][4], float vector[4]) {
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

s32 Editor::Inverse(MtxF* src, MtxF* dest) {
    MtxF mfCopy;
    s32 i;
    s32 pad;
    f32 temp2;
    f32 temp1;
    s32 thisCol;
    s32 thisRow;

    Editor::Copy(src, &mfCopy);
    Editor::Clear(dest);
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

void Editor::Copy(MtxF* src, MtxF* dest) {
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

void Editor::Clear(MtxF* mf) {
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

bool Editor::QueryCollisionRayActor(Vec3f rayOrigin, Vec3f rayDir, Vec3f actorMin, Vec3f actorMax, float* t) {
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
