#ifndef _MATRIX_HEADER_
#define _MATRIX_HEADER_

#include <libultraship.h>

#include "common_structs.h"
#include "CoreMath.h"

#ifdef __cplusplus
extern "C" {
void ApplyMatrixTransformations(Mat4 mtx, FVector pos, IRotator rot, FVector scale);
void AddLocalRotation(Mat4 mat, IRotator rot);
#endif
void ClearMatrixPools(void);
void AddHudMatrix(Mat4 mtx, s32 flags);
void AddObjectMatrix(Mat4 mtx, s32 flags);
void AddEffectMatrix(Mat4 mtx, s32 flags);
void AddEffectMatrixOrtho(void);
void AddEffectMatrixFixed(s32 flags);
Mtx* SetTextMatrix(f32 arg1, f32 arg2, f32 arg3, f32 arg4);
Mtx* GetEffectMatrix(void);
void ClearHudMatrixPool(void);
void ClearEffectsMatrixPool(void);
void ClearObjectsMatrixPool(void);
void AddKartMatrix(Mat4 mtx, s32 flags);
void AddShadowMatrix(Mat4 mtx, s32 flags);

#ifdef __cplusplus
}
#endif

#endif // _MATRIX_HEADER_