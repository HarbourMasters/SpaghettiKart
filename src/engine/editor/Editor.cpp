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
}

void Editor::MouseClick() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Camera* camera = &cameras[0];

    if (!_spawned) {
        _spawned = true;
        object = reinterpret_cast<AShip*>(gWorldInstance.AddActor(new AShip(FVector(0, 0, 0), AShip::Skin::SHIP2)));
    }

    Ship::Coords mouse = wnd->GetMousePos();

    // Get screen dimensions
    uint32_t width = wnd->GetWidth();
    uint32_t height = wnd->GetHeight();

    // Convert mouse to NDS screen coordinates
    float x = (2.0f * mouse.x) / width - 1.0f;  // Normalized X: -1 to 1
    float y = 1.0f - (2.0f * -mouse.y) / height; // Normalized Y: -1 to 1
    float z = 1.0f; // z is typically 1.0 for the near plane

    float rayClip[4] = {x, y, z, 1.0f};

   // printf("rayClip %f %f %f\n", x, y, z);

    Mat4 inversePerspMtx;

    Mat4 perspMtx;
    guMtxL2F(perspMtx, &gGfxPools->mtxPersp[0]);
    Mat4 lookAtMtx;
    guMtxL2F(lookAtMtx, &gGfxPools->mtxLookAt[0]);

    Mat4 combinedMtx;
    mtxf_multiplication(combinedMtx, lookAtMtx, perspMtx);
    Mat4 inverseViewMtx;
    float invRayWor[4];
    Editor::Inverse((MtxF*)&combinedMtx[0][0], (MtxF*)&inverseViewMtx[0][0]);
    MultiplyMatrixVector(inverseViewMtx, rayClip, invRayWor);

    Vec3f direction = {invRayWor[0], invRayWor[1], invRayWor[2]};

    float rayLength = 400.0f;

    float nearPlaneOffset = 9.0f;
    Vec3f rayStart;
    rayStart[0] = camera->pos[0];// + direction[0] * nearPlaneOffset;
    rayStart[1] = camera->pos[1];// + direction[1] * nearPlaneOffset;
    rayStart[2] = camera->pos[2];// + direction[2] * nearPlaneOffset;

    Vec3f rayEnd;
    rayEnd[0] = (rayStart[0] + direction[0] * 400.0); //* (rayLength + ray_wor[0]);
    rayEnd[1] = (rayStart[1] + direction[1] * 400.0);  //* (rayLength + ray_wor[1]);
    rayEnd[2] = (rayStart[2] + direction[2] * 400.0);  // * (rayLength + ray_wor[2]);

    _ray[0] = direction[0];
    _ray[1] = direction[1];
    _ray[2] = direction[2];

    printf("cam pos %f, ray*len %f, cam+ray*len %f, brckt %f\n", camera->pos[0], _ray[0] * 300, camera->pos[0] + _ray[0] * 300, camera->pos[0] + (_ray[0] * 300));

   // printf("pos %f %f %f dir %f %f %f\n", camera->pos[0], camera->pos[1], camera->pos[2], rayLength * ray_wor[0], rayLength * ray_wor[1], rayLength * ray_wor[2]);

    //printf("3 %f %f %f\n", direction[0], direction[1], direction[2]);
    object->Pos.x = rayEnd[0];
    object->Pos.y = rayEnd[1];
    object->Pos.z = rayEnd[2];
}

void Editor::Draw() {
    if (_draw) {
       // DrawObj(0);
        //DrawObj(5);
        //DrawObj(100);
        //DrawObj(500);
        //DrawObj(1000);
        DrawObj(2000);
    }
}

void Editor::DrawObj(float length) {
    Mat4 mtx;
    Vec3f pos2 = { cameras[0].pos[0] + _ray[0] * length, cameras[0].pos[1] + _ray[1] * length,
                   cameras[0].pos[2] + _ray[2] * length };
    mtxf_translate(mtx, pos2);
    mtxf_scale(mtx, 0.1);
    render_set_position(mtx, 0);
    gSPDisplayList(gDisplayListHead++, Editor::box_Cube_mesh);
}

void Editor::MultiplyMatrixVector(float matrix[4][4], float vector[4], float result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = 0.0f;
        for (int j = 0; j < 4; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
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

