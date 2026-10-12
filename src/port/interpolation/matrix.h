#pragma once

#define MTXF_NEW 0
#define MTXF_APPLY 1
#include "common_structs.h"

typedef struct {
    float r;
    float g;
    float b;
} Color;

typedef struct {
    float x;
    float y;
    float z;
} Vec3fInterp;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vec3sInterp;

typedef struct {
    f32 m1; f32 m2; f32 m3; f32 m4;
    f32 m5; f32 m6; f32 m7; f32 m8;
} Mat4Interp;

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif
#define M_RTOD	(180.0f / M_PI)
#define SQ(val) ((val) * (val))

#define qs1616(e) ((s32) ((e) *0x00010000))

#define IPART(x) ((qs1616(x) >> 16) & 0xFFFF)
#define FPART(x) (qs1616(x) & 0xFFFF)

#define gdSPDefMtx(xx, yx, zx, wx, xy, yy, zy, wy, xz, yz, zz, wz, xw, yw, zw, ww)                                 \
    {                                                                                                              \
        {                                                                                                          \
            (IPART(xx) << 0x10) | IPART(xy), (IPART(xz) << 0x10) | IPART(xw), (IPART(yx) << 0x10) | IPART(yy),     \
                (IPART(yz) << 0x10) | IPART(yw), (IPART(zx) << 0x10) | IPART(zy), (IPART(zz) << 0x10) | IPART(zw), \
                (IPART(wx) << 0x10) | IPART(wy), (IPART(wz) << 0x10) | IPART(ww), (FPART(xx) << 0x10) | FPART(xy), \
                (FPART(xz) << 0x10) | FPART(xw), (FPART(yx) << 0x10) | FPART(yy), (FPART(yz) << 0x10) | FPART(yw), \
                (FPART(zx) << 0x10) | FPART(zy), (FPART(zz) << 0x10) | FPART(zw), (FPART(wx) << 0x10) | FPART(wy), \
                (FPART(wz) << 0x10) | FPART(ww),                                                                   \
        }                                                                                                          \
    }


typedef MtxF Matrix;

#ifdef __cplusplus
extern "C" {
#endif

extern Mtx gIdentityMtx;
extern Matrix gIdentityMatrix;

extern Matrix* gGfxMatrix;
extern Matrix sGfxMatrixStack[];
extern Matrix* gCalcMatrix;
extern Matrix sCalcMatrixStack[];

extern Mtx gMainMatrixStack[];
extern Mtx* gGfxMtx;

void Matrix_InitPerspective(Gfx** dList);
void Matrix_InitOrtho(Gfx** dList);
void Matrix_Copy(Matrix* dst, Matrix* src);
void Matrix_Push(Matrix** mtxStack);
void Matrix_Pop(Matrix** mtxStack);
void Matrix_Mult(Matrix* mtx, Matrix* tf, u8 mode);
void Matrix_Translate(Matrix* mtx, f32 x, f32 y, f32 z, u8 mode);
void Matrix_Scale(Matrix* mtx, f32 xScale, f32 yScale, f32 zScale, u8 mode);
void Matrix_RotateX(Matrix* mtx, f32 angle, u8 mode);
void Matrix_RotateY(Matrix* mtx, f32 angle, u8 mode);
void Matrix_RotateZ(Matrix* mtx, f32 angle, u8 mode);
void Matrix_RotateAxis(Matrix* mtx, f32 angle, f32 axisX, f32 axisY, f32 axisZ, u8 mode);
void Matrix_ToMtx(Mtx* dest);
void Matrix_FromMtx(Mtx* src, Matrix* dest);
void Matrix_MultVec3f(Matrix* mtx, Vec3f* src, Vec3f* dest);
void Matrix_MultVec3fNoTranslate(Matrix* mtx, Vec3f* src, Vec3f* dest);
void Matrix_GetYRPAngles(Matrix* mtx, Vec3f* rot);
void Matrix_GetXYZAngles(Matrix* mtx, Vec3f* rot);
void Matrix_LookAt(Matrix* mtx, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp,
                   u8 mode);
void Matrix_SetGfxMtx(Gfx** gfx);
void Lights_SetOneLight(Gfx** dList, s32 dirX, s32 dirY, s32 dirZ, s32 colR, s32 colG, s32 colB, s32 ambR, s32 ambG, s32 ambB);

#ifdef __cplusplus
}
#endif