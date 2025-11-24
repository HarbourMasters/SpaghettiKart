#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/World.h"
#include "src/port/interpolation/FrameInterpolation.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "math_util_2.h"
}

void AddMatrix(std::deque<Mtx>& stack, Mat4 mtx, s32 flags) {
    // Push a new matrix to the stack
    stack.emplace_back();

    // Convert to a fixed-point matrix
    FrameInterpolation_RecordMatrixMtxFToMtx((MtxF*)mtx, &stack.back());
    guMtxF2L(mtx, &stack.back());

    // Load the matrix
    gSPMatrix(gDisplayListHead++, &stack.back(), flags);
}

Mtx* GetMatrix(std::deque<Mtx>& stack) {
    stack.emplace_back();
    return &stack.back();
}

/**
 * Push a fixed point matrix to the stack
 * Use GetMatrix() before calling this
 */
void AddMatrixFixed(std::deque<Mtx>& stack, s32 flags) {
    // Load the matrix
    gSPMatrix(gDisplayListHead++, &stack.back(), flags);
}

// Used in func_80095BD0
void SetTextMatrix(Mat4 mf, f32 x, f32 y, f32 arg3, f32 arg4) {
    FrameInterpolation_Record_SetTextMatrix((Mat4*)&mf, x, y, arg3, arg4);
    mf[0][0] = arg3;
    mf[0][1] = 0.0f;
    mf[0][2] = 0.0f;
    mf[0][3] = 0.0f;
    mf[1][0] = 0.0f;
    mf[1][1] = arg4;
    mf[1][2] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][0] = 0.0f;
    mf[2][1] = 0.0f;
    mf[2][2] = 1.0f;
    mf[2][3] = 0.0f;
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = 0.0f;
    mf[3][3] = 1.0f;
}

// AddMatrix but with custom gfx ptr arg and flags are predefined
Gfx* AddTextMatrix(Gfx* displayListHead, Mat4 mtx) {
    // Push a new matrix to the stack
    gWorldInstance.Mtx.Objects.emplace_back();

    // Convert to a fixed-point matrix
    FrameInterpolation_RecordMatrixMtxFToMtx((MtxF*)mtx, &gWorldInstance.Mtx.Objects.back());
    guMtxF2L(mtx, &gWorldInstance.Mtx.Objects.back());

    // Load the matrix
    gSPMatrix(displayListHead++, &gWorldInstance.Mtx.Objects.back(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    return displayListHead;
}

void ApplyMatrixTransformations(Mat4 mtx, FVector pos, IRotator rot, FVector scale) {
    f32 sine1, cosine1;
    f32 sine2, cosine2;
    f32 sine3, cosine3;
    //FrameInterpolation_ApplyMatrixTransformations((Mat4*)mtx, pos, rot, scale);

    // Compute the sine and cosine of the orientation (Euler angles)
    sine1 = sins(rot.pitch);
    cosine1 = coss(rot.pitch);
    sine2 = sins(rot.yaw);
    cosine2 = coss(rot.yaw);
    sine3 = sins(rot.roll);
    cosine3 = coss(rot.roll);

    // Compute the rotation matrix
    mtx[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    mtx[1][0] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    mtx[2][0] = cosine1 * sine2;
    mtx[3][0] = pos.x;

    mtx[0][1] = cosine1 * sine3;
    mtx[1][1] = cosine1 * cosine3;
    mtx[2][1] = -sine1;
    mtx[3][1] = pos.y;

    mtx[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    mtx[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    mtx[2][2] = cosine1 * cosine2;
    mtx[3][2] = pos.z;

    // Apply scaling
    mtx[0][0] *= scale.x;
    mtx[1][0] *= scale.x;
    mtx[2][0] *= scale.x;
    mtx[0][1] *= scale.y;
    mtx[1][1] *= scale.y;
    mtx[2][1] *= scale.y;
    mtx[0][2] *= scale.z;
    mtx[1][2] *= scale.z;
    mtx[2][2] *= scale.z;
    
    // Set the last row and column for the homogeneous coordinate system
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
}

/* 
 * Spherical billboarding
 * Rotates the object to face the camera
 * Rotates on all three axis
 */
void ApplySphericalBillBoard(Mat4 mat, FVector pos, FVector scale, s32 cameraIndex) {
    Mtx* lookAt = cameras[cameraIndex].lookAtMatrix;
    Mat4 lookAtF;
    guMtxL2F((float(*)[4])&lookAtF, lookAt);

    // Camera Right
    mat[0][0] = lookAtF[0][0];
    mat[1][0] = lookAtF[0][1];
    mat[2][0] = lookAtF[0][2];
    mat[3][0] = 0;

    // Camera Up
    mat[0][1] = lookAtF[1][0];
    mat[1][1] = lookAtF[1][1];
    mat[2][1] = lookAtF[1][2];
    mat[3][1] = 0;

    // Camera Forward
    mat[0][2] = lookAtF[2][0];
    mat[1][2] = lookAtF[2][1];
    mat[2][2] = lookAtF[2][2];
    mat[3][2] = 0;

    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = 0;
    mat[3][3] = 1;

    // Set position
    mat[3][0] = pos.x;
    mat[3][1] = pos.y;
    mat[3][2] = pos.z;

    // Apply scaling
    mat[0][0] *= scale.x;
    mat[1][0] *= scale.x;
    mat[2][0] *= scale.x;
    mat[0][1] *= scale.y;
    mat[1][1] *= scale.y;
    mat[2][1] *= scale.y;
    mat[0][2] *= scale.z;
    mat[1][2] *= scale.z;
    mat[2][2] *= scale.z;
}

void AddLocalRotation(Mat4 mat, IRotator rot) {
    f32 sin_pitch = sins(rot.pitch);
    f32 cos_pitch = coss(rot.pitch);
    f32 sin_yaw = sins(rot.yaw);
    f32 cos_yaw = coss(rot.yaw);
    f32 sin_roll = sins(rot.roll);
    f32 cos_roll = coss(rot.roll);

    // Modify only the rotation part (keep translation intact)
    mat[0][0] = (cos_yaw * cos_roll) + (sin_pitch * sin_yaw * sin_roll);
    mat[0][1] = (cos_pitch * sin_roll);
    mat[0][2] = (-sin_yaw * cos_roll) + (sin_pitch * cos_yaw * sin_roll);
    
    mat[1][0] = (-cos_yaw * sin_roll) + (sin_pitch * sin_yaw * cos_roll);
    mat[1][1] = (cos_pitch * cos_roll);
    mat[1][2] = (sin_yaw * sin_roll) + (sin_pitch * cos_yaw * cos_roll);
    
    mat[2][0] = (cos_pitch * sin_yaw);
    mat[2][1] = -sin_pitch;
    mat[2][2] = (cos_pitch * cos_yaw);
}

// API
extern "C" {

    void AddHudMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Objects, mtx, flags);
    }

    Mtx* GetScreenMatrix(void) {
        return &gWorldInstance.Mtx.Screen2D;
    }

    Mtx* GetOrthoMatrix(void) {
        return &gWorldInstance.Mtx.Ortho;
    }

    Mtx* GetPerspMatrix(size_t cameraId) {
        return &gWorldInstance.Mtx.Persp[cameraId];
    }

    Mtx* GetLookAtMatrix(size_t cameraId) {
        return &gWorldInstance.Mtx.LookAt[cameraId];
    }

    void AddObjectMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Objects, mtx, flags);
    }

    Mtx* GetShadowMatrix(size_t playerId) {
        return &gWorldInstance.Mtx.Shadows[playerId];
    }

    Mtx* GetKartMatrix(size_t playerId) {
        return &gWorldInstance.Mtx.Karts[playerId];
    }

    void AddEffectMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Objects, mtx, flags);
    }

    void AddEffectMatrixOrtho(void) {
        auto& stack = gWorldInstance.Mtx.Objects;
        stack.emplace_back();

        guOrtho(&stack.back(), 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
        
        gSPMatrix(gDisplayListHead++, &stack.back(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    }

    Mtx* GetEffectMatrix(void) {
        return GetMatrix(gWorldInstance.Mtx.Objects);
    }


    /**
     * Note that the game doesn't seem to clear all of these at the beginning of a new frame.
     * We might need to adjust which ones we clear.
     */
    void ClearMatrixPools(void) {
        gWorldInstance.Mtx.Objects.clear();
       // gWorldInstance.Mtx.Shadows.clear();
        //gWorldInstance.Mtx.Karts.clear();
       // gWorldInstance.Mtx.Effects.clear();
    }

    void ClearObjectsMatrixPool(void) {
        gWorldInstance.Mtx.Objects.clear();
    }
}

