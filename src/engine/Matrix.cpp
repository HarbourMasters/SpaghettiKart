#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/World.h"

extern "C" {
#include "common_structs.h"
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
 * Use GetMatrix first
 */
void AddMatrixFixed(std::vector<Mtx>& stack, s32 flags) {
    // Load the matrix
    gSPMatrix(gDisplayListHead++, &stack.back(), flags);
}

// Used in func_80095BD0
void SetTextMatrix(f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    Mat4 mtx;
    mtx[0][0] = arg3;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = arg4;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[2][3] = 0.0f;
    mtx[3][0] = arg1;
    mtx[3][1] = arg2;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;

    AddMatrix(gWorldInstance.Mtx.Effects, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
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
