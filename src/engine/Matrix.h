#ifndef _MATRIX_HEADER_
#define _MATRIX_HEADER_

#include <libultraship.h>

#include "common_structs.h"
#include "CoreMath.h"

#ifdef __cplusplus
extern "C" {
void ApplyMatrixTransformations(Mat4 mtx, FVector pos, IRotator rot, FVector scale);
void ApplySphericalBillBoard(Mat4 mat, FVector pos, FVector scale, s32 cameraIndex);
void AddLocalRotation(Mat4 mat, IRotator rot);
#endif
void ClearMatrixPools(void);
void AddHudMatrix(Mat4 mtx, s32 flags);
void AddObjectMatrix(Mat4 mtx, s32 flags);
void AddEffectMatrix(Mat4 mtx, s32 flags);
void AddEffectMatrixOrtho(void);

Mtx* GetScreenMatrix(void); // Orthogonal projection for UI, skybox, etc.
Mtx* GetOrthoMatrix(void);
Mtx* GetPerspMatrix(size_t cameraId);
Mtx* GetLookAtMatrix(size_t cameraId);

Mtx* GetKartMatrix(size_t playerId);
Mtx* GetShadowMatrix(size_t playerId);

void SetTextMatrix(Mat4 mf, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
Gfx* AddTextMatrix(Gfx* displayListHead, Mat4 mtx);
Mtx* GetEffectMatrix(void);
void ClearObjectsMatrixPool(void);
void AddKartMatrix(Mat4 mtx, s32 flags);

#ifdef __cplusplus
}
#endif

#endif // _MATRIX_HEADER_
