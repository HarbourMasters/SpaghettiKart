#ifndef __FRAME_INTERPOLATION_H
#define __FRAME_INTERPOLATION_H

// #include "sf64math.h"
#include <libultraship.h>
#include <common_structs.h>

#ifdef __cplusplus

#include <unordered_map>



std::unordered_map<Mtx*, MtxF> FrameInterpolation_Interpolate(float step);

extern "C" {
#endif

#define TAG_ITEM_ADDR(x) ((u32) 0x10000000 | (u32)x)
#define TAG_SMOKE_DUST(x) ((u32) 0x20000000 | (u32) (x))
#define TAG_LETTER(x) ((u32)0x30000000 | (u32) (uintptr_t) (x))
#define TAG_OBJECT(x) ((u32)0x40000000 | (u32) (uintptr_t) (x))

void FrameInterpolation_ShouldInterpolateFrame(bool shouldInterpolate);

void FrameInterpolation_StartRecord(void);

void FrameInterpolation_StopRecord(void);

void FrameInterpolation_RecordMarker(const char* file, int line);

void FrameInterpolation_RecordOpenChild(const void* a, uintptr_t b);

void FrameInterpolation_RecordCloseChild(void);

void FrameInterpolation_DontInterpolateCamera(void);

int FrameInterpolation_GetCameraEpoch(void);

void FrameInterpolation_RecordActorPosRotMatrix(void);

void FrameInterpolation_RecordMatrixPosRotXYZ(Mat4* out, Vec3f pos, Vec3s orientation);

void FrameInterpolation_RecordMatrixPosRotScaleXY(Mat4* matrix, s32 x, s32 y, u16 angle, f32 scale);

void FrameInterpolation_Record_SetTextMatrix(Mat4* matrix, f32 x, f32 y, f32 arg3, f32 arg4);

void FrameInterpolation_RecordMatrixPush(Mat4* matrix);

void FrameInterpolation_RecordMatrixPop(Mat4* matrix);

void FrameInterpolation_RecordMatrixMult(Mat4* matrix, MtxF* mf, u8 mode);

void FrameInterpolation_RecordMatrixTranslate(Mat4* matrix, Vec3f b);

void FrameInterpolation_RecordMatrixScale(Mat4* matrix, f32 scale);

void FrameInterpolation_RecordMatrixRotate1Coord(Mat4* matrix, u32 coord, s16 value);

void FrameInterpolation_RecordMatrixMtxFToMtx(MtxF* src, Mtx* dest);

void FrameInterpolation_RecordMatrixToMtx(Mtx* dest, char* file, s32 line);

void FrameInterpolation_RecordMatrixReplaceRotation(MtxF* mf);

//void FrameInterpolation_RecordMatrixRotateAxis(f32 angle, Vec3f* axis, u8 mode);

void FrameInterpolation_RecordSkinMatrixMtxFToMtx(MtxF* src, Mtx* dest);

//void FrameInterpolation_RecordMatrixMultVec3f(Matrix* matrix, Vec3f src, Vec3f dest);

//void FrameInterpolation_RecordMatrixMultVec3fNoTranslate(Matrix* matrix, Vec3f src, Vec3f dest);

void FrameInterpolation_RecordSetTransformMatrix(Mat4* dest, Vec3f orientationVector, Vec3f positionVector, u16 rotationAngle,
                          f32 scaleFactor);

void FrameInterpolation_RecordSetMatrixTransformation(Mat4* dest, Vec3f location, Vec3su rotation, f32 scale);

void FrameInterpolation_RecordCalculateOrientationMatrix(Mat3*, f32, f32, f32, s16);

void FrameInterpolation_RecordTranslateRotate(Mat4* dest, Vec3f pos, Vec3s rotation);

//void FrameInterpolation_func_80062B18(f32* arg0, f32* arg1, f32* arg2, arg3, arg4, arg5, arg6, arg7);

#ifdef __cplusplus
}
#endif

#endif // __FRAME_INTERPOLATION_H
