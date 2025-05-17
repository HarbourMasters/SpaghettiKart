#ifndef __FRAME_INTERPOLATION_H
#define __FRAME_INTERPOLATION_H

// #include "sf64math.h"
#include <libultraship.h>
#include <common_structs.h>
#include <math_util_2.h>

#ifdef __cplusplus

#include <unordered_map>

std::unordered_map<Mtx*, MtxF> FrameInterpolation_Interpolate(float step);

extern "C" {
#endif

void FrameInterpolation_ShouldInterpolateFrame(bool shouldInterpolate);

void FrameInterpolation_StartRecord(void);

void FrameInterpolation_StopRecord(void);

void FrameInterpolation_RecordMarker(const char* file, int line);

void FrameInterpolation_RecordOpenChild(const void* a, int b);

void FrameInterpolation_RecordCloseChild(void);

void FrameInterpolation_DontInterpolateCamera(void);

int FrameInterpolation_GetCameraEpoch(void);

void FrameInterpolation_RecordActorPosRotMatrix(void);

void FrameInterpolation_RecordMatrixPosRotXYZ(Mat4 out, Vec3f pos, Vec3s orientation);

void FrameInterpolation_RecordMatrixPush(Mat4* mtx);

void FrameInterpolation_RecordMatrixPop(Mat4* mtx);

//void FrameInterpolation_RecordMatrixMult(Matrix* matrix, MtxF* mf, u8 mode);

void FrameInterpolation_RecordMatrixTranslate(Mat4* matrix, Vec3f b);

//void FrameInterpolation_RecordMatrixScale(Matrix* matrix, f32 x, f32 y, f32 z, u8 mode);

void FrameInterpolation_RecordMatrixRotate1Coord(Mat4* matrix, u32 coord, s16 value);

void FrameInterpolation_RecordMatrixMtxFToMtx(MtxF* src, Mtx* dest);

void FrameInterpolation_RecordMatrixToMtx(Mtx* dest, char* file, s32 line);

void FrameInterpolation_RecordMatrixReplaceRotation(MtxF* mf);

//void FrameInterpolation_RecordMatrixRotateAxis(f32 angle, Vec3f* axis, u8 mode);

void FrameInterpolation_RecordSkinMatrixMtxFToMtx(MtxF* src, Mtx* dest);

//void FrameInterpolation_RecordMatrixMultVec3f(Matrix* matrix, Vec3f src, Vec3f dest);

//void FrameInterpolation_RecordMatrixMultVec3fNoTranslate(Matrix* matrix, Vec3f src, Vec3f dest);

#ifdef __cplusplus
}
#endif

#endif // __FRAME_INTERPOLATION_H