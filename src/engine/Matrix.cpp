#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/World.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "math_util_2.h"
}

void AddMatrix(std::vector<Mtx>& stack, Mat4 mtx, s32 flags) {
    // Reserve space if needed to avoid reallocation overhead
    stack.reserve(1000);

    // Push a new matrix to the stack
    stack.emplace_back();

    // Convert to a fixed-point matrix
    guMtxF2L(mtx, &stack.back());

    // Load the matrix
    gSPMatrix(gDisplayListHead++, &stack.back(), flags);
}

Mtx* GetMatrix(std::vector<Mtx>& stack) {
    stack.emplace_back();
    return &stack.back();
}

/**
 * Use GetMatrix() first
 */
void AddMatrixFixed(std::vector<Mtx>& stack, s32 flags) {
    // Load the matrix
    gSPMatrix(gDisplayListHead++, &stack.back(), flags);
}

// Used in func_80095BD0
Mtx* SetTextMatrix(f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    Mat4 matrix;
    matrix[0][0] = arg3;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f;
    matrix[1][1] = arg4;
    matrix[1][2] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
    matrix[2][3] = 0.0f;
    matrix[3][0] = arg1;
    matrix[3][1] = arg2;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;
    Mtx* mtx = GetMatrix(gWorldInstance.Mtx.Effects);
    guMtxF2L(matrix, mtx);

    return mtx;
}

void ApplyMatrixTransformations(Mat4 mtx, FVector pos, IRotator rot, FVector scale) {
    f32 sine1, cosine1;
    f32 sine2, cosine2;
    f32 sine3, cosine3;

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
        AddMatrix(gWorldInstance.Mtx.Hud, mtx, flags);
    }

    void AddObjectMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Objects, mtx, flags);
    }

    void AddShadowMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Shadows, mtx, flags);
    }

    void AddKartMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Karts, mtx, flags);
    }

    void AddEffectMatrix(Mat4 mtx, s32 flags) {
        AddMatrix(gWorldInstance.Mtx.Effects, mtx, flags);
    }

    void AddEffectMatrixFixed(s32 flags) {
        AddMatrixFixed(gWorldInstance.Mtx.Effects, flags);
    }

    void AddEffectMatrixOrtho(void) {
        auto& stack = gWorldInstance.Mtx.Effects;
        stack.emplace_back();

        guOrtho(&stack.back(), 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
        
        gSPMatrix(gDisplayListHead++, &stack.back(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    }

    Mtx* GetEffectMatrix(void) {
        return GetMatrix(gWorldInstance.Mtx.Effects);
    }


    /**
     * Note that the game doesn't seem to clear all of these at the beginning of a new frame.
     * We might need to adjust which ones we clear.
     */
    void ClearMatrixPools(void) {
        gWorldInstance.Mtx.Hud.clear();
        gWorldInstance.Mtx.Objects.clear();
        gWorldInstance.Mtx.Shadows.clear();
        gWorldInstance.Mtx.Karts.clear();
        gWorldInstance.Mtx.Effects.clear();
    }

    void ClearHudMatrixPool(void) {
        gWorldInstance.Mtx.Hud.clear();
    }
    void ClearEffectsMatrixPool(void) {
        gWorldInstance.Mtx.Effects.clear();
    }

    void ClearObjectsMatrixPool(void) {
        gWorldInstance.Mtx.Objects.clear();
    }
}

