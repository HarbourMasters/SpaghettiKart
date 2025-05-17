#include <libultraship.h>
#include <math.h>
#include "matrix.h"
#include "common_structs.h"
#include "FrameInterpolation.h"

Mtx gIdentityMtx = gdSPDefMtx(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
Matrix gIdentityMatrix = { {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
} };

Matrix* gGfxMatrix;
Matrix sGfxMatrixStack[0x20];
Matrix* gCalcMatrix;
Matrix sCalcMatrixStack[0x20];

Mtx gMainMatrixStack[0x480];
Mtx* gGfxMtx;

void Matrix_InitPerspective(Gfx** dList) {
    u16 norm;
    float near = 10.0f;
    float far = 12800.0f;
    float fov = 45.0f;

    guPerspective(gGfxMtx, &norm, fov, 320.0f / 240.0f, near, far, 1.0f);
    gSPPerspNormalize((*dList)++, norm);
    gSPMatrix((*dList)++, gGfxMtx++, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(gGfxMtx, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -12800.0f, 0.0f, 1.0f, 0.0f);
    gSPMatrix((*dList)++, gGfxMtx++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    Matrix_Copy(gGfxMatrix, &gIdentityMatrix);
}

void Matrix_InitOrtho(Gfx** dList) {
    FrameInterpolation_RecordOpenChild("ortho", 0);
    FrameInterpolation_RecordMarker(__FILE__, __LINE__);
    guOrtho(gGfxMtx, -320.0f / 2, 320.0f / 2, -240.0f / 2, 240.0f / 2, 0.0f, 5.0f, 1.0f);
    gSPMatrix((*dList)++, gGfxMtx++, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(gGfxMtx, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -12800.0f, 0.0f, 1.0f, 0.0f);
    gSPMatrix((*dList)++, gGfxMtx++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    Matrix_Copy(gGfxMatrix, &gIdentityMatrix);
    FrameInterpolation_RecordCloseChild();
}


// Copies src Matrix into dst
void Matrix_Copy(Matrix* dst, Matrix* src) {
    int32_t i;

    for (i = 0; i < 4; i++) {
        dst->mf[i][0] = src->mf[i][0];
        dst->mf[i][1] = src->mf[i][1];
        dst->mf[i][2] = src->mf[i][2];
        dst->mf[i][3] = src->mf[i][3];
    }
}

// Makes a copy of the stack's current matrix and puts it on the top of the stack
void Matrix_Push(Matrix** mtxStack) {
    Matrix_Copy(*mtxStack + 1, *mtxStack);
    (*mtxStack)++;
}

// Removes the top matrix of the stack
void Matrix_Pop(Matrix** mtxStack) {
    (*mtxStack)--;
}

// Copies tf into mtx (MTXF_NEW) or applies it to mtx (MTXF_APPLY)
void Matrix_Mult(Matrix* mtx, Matrix* tf, u8 mode) {
    f32 rx;
    f32 ry;
    f32 rz;
    f32 rw;
    s32 i0;
    s32 i1;
    s32 i2;
    s32 i3;

    if (mode == 1) {
        rx = mtx->mf[0][0];
        ry = mtx->mf[1][0];
        rz = mtx->mf[2][0];
        rw = mtx->mf[3][0];

        for (i0 = 0; i0 < 4; i0++) {
            mtx->mf[i0][0] = (rx * tf->mf[i0][0]) + (ry * tf->mf[i0][1]) + (rz * tf->mf[i0][2]) + (rw * tf->mf[i0][3]);
        }

        rx = mtx->mf[0][1];
        ry = mtx->mf[1][1];
        rz = mtx->mf[2][1];
        rw = mtx->mf[3][1];

        for (i1 = 0; i1 < 4; i1++) {
            mtx->mf[i1][1] = (rx * tf->mf[i1][0]) + (ry * tf->mf[i1][1]) + (rz * tf->mf[i1][2]) + (rw * tf->mf[i1][3]);
        }

        rx = mtx->mf[0][2];
        ry = mtx->mf[1][2];
        rz = mtx->mf[2][2];
        rw = mtx->mf[3][2];

        for (i2 = 0; i2 < 4; i2++) {
            mtx->mf[i2][2] = (rx * tf->mf[i2][0]) + (ry * tf->mf[i2][1]) + (rz * tf->mf[i2][2]) + (rw * tf->mf[i2][3]);
        }

        rx = mtx->mf[0][3];
        ry = mtx->mf[1][3];
        rz = mtx->mf[2][3];
        rw = mtx->mf[3][3];

        for (i3 = 0; i3 < 4; i3++) {
            mtx->mf[i3][3] = (rx * tf->mf[i3][0]) + (ry * tf->mf[i3][1]) + (rz * tf->mf[i3][2]) + (rw * tf->mf[i3][3]);
        }
    } else {
        Matrix_Copy(mtx, tf);
    }
}

// Creates a translation matrix in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY)
void Matrix_Translate(Matrix* mtx, f32 x, f32 y, f32 z, u8 mode) {
    f32 rx;
    f32 ry;
    s32 i;

    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            rx = mtx->mf[0][i];
            ry = mtx->mf[1][i];

            mtx->mf[3][i] += (rx * x) + (ry * y) + (mtx->mf[2][i] * z);
        }
    } else {
        mtx->mf[3][0] = x;
        mtx->mf[3][1] = y;
        mtx->mf[3][2] = z;
        mtx->mf[0][1] = mtx->mf[0][2] = mtx->mf[0][3] = mtx->mf[1][0] = mtx->mf[1][2] = mtx->mf[1][3] = mtx->mf[2][0] =
            mtx->mf[2][1] = mtx->mf[2][3] = 0.0f;
        mtx->mf[0][0] = mtx->mf[1][1] = mtx->mf[2][2] = mtx->mf[3][3] = 1.0f;
    }
}

// Creates a scale matrix in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY)
void Matrix_Scale(Matrix* mtx, f32 xScale, f32 yScale, f32 zScale, u8 mode) {
    f32 rx;
    f32 ry;
    s32 i;

    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            rx = mtx->mf[0][i];
            ry = mtx->mf[1][i];

            mtx->mf[0][i] = rx * xScale;
            mtx->mf[1][i] = ry * yScale;
            mtx->mf[2][i] *= zScale;
        }
    } else {
        mtx->mf[0][0] = xScale;
        mtx->mf[1][1] = yScale;
        mtx->mf[2][2] = zScale;
        mtx->mf[0][1] = mtx->mf[0][2] = mtx->mf[0][3] = mtx->mf[1][0] = mtx->mf[1][2] = mtx->mf[1][3] = mtx->mf[2][0] =
            mtx->mf[2][1] = mtx->mf[2][3] = mtx->mf[3][0] = mtx->mf[3][1] = mtx->mf[3][2] = 0.0f;
        mtx->mf[3][3] = 1.0f;
    }
}

// Creates rotation matrix about the X axis in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY)
void Matrix_RotateX(Matrix* mtx, f32 angle, u8 mode) {
    f32 cs;
    f32 sn;
    f32 ry;
    f32 rz;
    s32 i;

    sn = sinf(angle);
    cs = cosf(angle);
    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            ry = mtx->mf[1][i];
            rz = mtx->mf[2][i];

            mtx->mf[1][i] = (ry * cs) + (rz * sn);
            mtx->mf[2][i] = (rz * cs) - (ry * sn);
        }
    } else {
        mtx->mf[1][1] = mtx->mf[2][2] = cs;
        mtx->mf[1][2] = sn;
        mtx->mf[2][1] = -sn;
        mtx->mf[0][0] = mtx->mf[3][3] = 1.0f;
        mtx->mf[0][1] = mtx->mf[0][2] = mtx->mf[0][3] = mtx->mf[1][0] = mtx->mf[1][3] = mtx->mf[2][0] = mtx->mf[2][3] =
            mtx->mf[3][0] = mtx->mf[3][1] = mtx->mf[3][2] = 0.0f;
    }
}

// Creates rotation matrix about the Y axis in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY)
void Matrix_RotateY(Matrix* mtx, f32 angle, u8 mode) {
    f32 cs;
    f32 sn;
    f32 rx;
    f32 rz;
    s32 i;

    sn = sinf(angle);
    cs = cosf(angle);
    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            rx = mtx->mf[0][i];
            rz = mtx->mf[2][i];

            mtx->mf[0][i] = (rx * cs) - (rz * sn);
            mtx->mf[2][i] = (rx * sn) + (rz * cs);
        }
    } else {
        mtx->mf[0][0] = mtx->mf[2][2] = cs;
        mtx->mf[0][2] = -sn;
        mtx->mf[2][0] = sn;
        mtx->mf[1][1] = mtx->mf[3][3] = 1.0f;
        mtx->mf[0][1] = mtx->mf[0][3] = mtx->mf[1][0] = mtx->mf[1][2] = mtx->mf[1][3] = mtx->mf[2][1] = mtx->mf[2][3] =
            mtx->mf[3][0] = mtx->mf[3][1] = mtx->mf[3][2] = 0.0f;
    }
}

// Creates rotation matrix about the Z axis in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY)
void Matrix_RotateZ(Matrix* mtx, f32 angle, u8 mode) {
    f32 cs;
    f32 sn;
    f32 rx;
    f32 ry;
    s32 i;

    sn = sinf(angle);
    cs = cosf(angle);
    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            rx = mtx->mf[0][i];
            ry = mtx->mf[1][i];

            mtx->mf[0][i] = (rx * cs) + (ry * sn);
            mtx->mf[1][i] = (ry * cs) - (rx * sn);
        }
    } else {
        mtx->mf[0][0] = mtx->mf[1][1] = cs;
        mtx->mf[0][1] = sn;
        mtx->mf[1][0] = -sn;
        mtx->mf[2][2] = mtx->mf[3][3] = 1.0f;
        mtx->mf[0][2] = mtx->mf[0][3] = mtx->mf[1][2] = mtx->mf[1][3] = mtx->mf[2][0] = mtx->mf[2][1] = mtx->mf[2][3] =
            mtx->mf[3][0] = mtx->mf[3][1] = mtx->mf[3][2] = 0.0f;
    }
}

// Creates rotation matrix about a given vector axis in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY).
// The vector specifying the axis does not need to be a unit vector.
void Matrix_RotateAxis(Matrix* mtx, f32 angle, f32 axisX, f32 axisY, f32 axisZ, u8 mode) {
    f32 rx;
    f32 ry;
    f32 rz;
    f32 norm;
    f32 cxx;
    f32 cyx;
    f32 czx;
    f32 cxy;
    f32 cyy;
    f32 czy;
    f32 cxz;
    f32 cyz;
    f32 czz;
    f32 xx;
    f32 yy;
    f32 zz;
    f32 xy;
    f32 yz;
    f32 xz;
    f32 sinA;
    f32 cosA;

    norm = sqrtf((axisX * axisX) + (axisY * axisY) + (axisZ * axisZ));
    if (norm != 0.0) {
        axisX /= norm;
        axisY /= norm;
        axisZ /= norm;
        sinA = sinf(angle);
        cosA = cosf(angle);
        xx = axisX * axisX;
        yy = axisY * axisY;
        zz = axisZ * axisZ;
        xy = axisX * axisY;
        yz = axisY * axisZ;
        xz = axisX * axisZ;

        if (mode == 1) {
            cxx = (1.0f - xx) * cosA + xx;
            cyx = (1.0f - cosA) * xy + axisZ * sinA;
            czx = (1.0f - cosA) * xz - axisY * sinA;

            cxy = (1.0f - cosA) * xy - axisZ * sinA;
            cyy = (1.0f - yy) * cosA + yy;
            czy = (1.0f - cosA) * yz + axisX * sinA;

            cxz = (1.0f - cosA) * xz + axisY * sinA;
            cyz = (1.0f - cosA) * yz - axisX * sinA;
            czz = (1.0f - zz) * cosA + zz;

            // loop doesn't seem to work here.
            rx = mtx->mf[0][0];
            ry = mtx->mf[0][1];
            rz = mtx->mf[0][2];
            mtx->mf[0][0] = (rx * cxx) + (ry * cxy) + (rz * cxz);
            mtx->mf[0][1] = (rx * cyx) + (ry * cyy) + (rz * cyz);
            mtx->mf[0][2] = (rx * czx) + (ry * czy) + (rz * czz);

            rx = mtx->mf[1][0];
            ry = mtx->mf[1][1];
            rz = mtx->mf[1][2];
            mtx->mf[1][0] = (rx * cxx) + (ry * cxy) + (rz * cxz);
            mtx->mf[1][1] = (rx * cyx) + (ry * cyy) + (rz * cyz);
            mtx->mf[1][2] = (rx * czx) + (ry * czy) + (rz * czz);

            rx = mtx->mf[2][0];
            ry = mtx->mf[2][1];
            rz = mtx->mf[2][2];
            mtx->mf[2][0] = (rx * cxx) + (ry * cxy) + (rz * cxz);
            mtx->mf[2][1] = (rx * cyx) + (ry * cyy) + (rz * cyz);
            mtx->mf[2][2] = (rx * czx) + (ry * czy) + (rz * czz);
        } else {
            mtx->mf[0][0] = (1.0f - xx) * cosA + xx;
            mtx->mf[0][1] = (1.0f - cosA) * xy + axisZ * sinA;
            mtx->mf[0][2] = (1.0f - cosA) * xz - axisY * sinA;
            mtx->mf[0][3] = 0.0f;

            mtx->mf[1][0] = (1.0f - cosA) * xy - axisZ * sinA;
            mtx->mf[1][1] = (1.0f - yy) * cosA + yy;
            mtx->mf[1][2] = (1.0f - cosA) * yz + axisX * sinA;
            mtx->mf[1][3] = 0.0f;

            mtx->mf[2][0] = (1.0f - cosA) * xz + axisY * sinA;
            mtx->mf[2][1] = (1.0f - cosA) * yz - axisX * sinA;
            mtx->mf[2][2] = (1.0f - zz) * cosA + zz;
            mtx->mf[2][3] = 0.0f;

            mtx->mf[3][0] = mtx->mf[3][1] = mtx->mf[3][2] = 0.0f;
            mtx->mf[3][3] = 1.0f;
        }
    }
}

// Converts the current Gfx matrix to a Mtx
void Matrix_ToMtx(Mtx* dest) {
    // LTODO: We need to validate this
    guMtxF2L(gGfxMatrix->mf, dest);
}

// Converts the Mtx src to a Matrix, putting the result in dest
void Matrix_FromMtx(Mtx* src, Matrix* dest) {
    guMtxF2L(src->m, dest->mf);
}

// Applies the transform matrix mtx to the vector src, putting the result in dest
void Matrix_MultVec3f(Matrix* mtx, Vec3f* src, Vec3f* dest) {
    *dest[0] = (mtx->mf[0][0] * *src[0]) + (mtx->mf[1][0] * *src[1]) + (mtx->mf[2][0] * *src[2]) + mtx->mf[3][0];
    *dest[1] = (mtx->mf[0][1] * *src[0]) + (mtx->mf[1][1] * *src[1]) + (mtx->mf[2][1] * *src[2]) + mtx->mf[3][1];
    *dest[2] = (mtx->mf[0][2] * *src[0]) + (mtx->mf[1][2] * *src[1]) + (mtx->mf[2][2] * *src[2]) + mtx->mf[3][2];
}

// Applies the linear part of the transformation matrix mtx to the vector src, ignoring any translation that mtx might
// have. Puts the result in dest.
void Matrix_MultVec3fNoTranslate(Matrix* mtx, Vec3f* src, Vec3f* dest) {
    *dest[0] = (mtx->mf[0][0] * *src[0]) + (mtx->mf[1][0] * *src[1]) + (mtx->mf[2][0] * *src[2]);
    *dest[1] = (mtx->mf[0][1] * *src[0]) + (mtx->mf[1][1] * *src[1]) + (mtx->mf[2][1] * *src[2]);
    *dest[2] = (mtx->mf[0][2] * *src[0]) + (mtx->mf[1][2] * *src[1]) + (mtx->mf[2][2] * *src[2]);
}

// Expresses the rotational part of the transform mtx as Tait-Bryan angles, in the yaw-pitch-roll (intrinsic YXZ)
// convention used in worldspace calculations
void Matrix_GetYRPAngles(Matrix* mtx, Vec3f* rot) {
    Matrix invYP;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };
    Vec3f originP;
    Vec3f zHat = { 0.0f, 0.0f, 1.0f };
    Vec3f zHatP;
    Vec3f xHat = { 1.0f, 0.0f, 0.0f };
    Vec3f xHatP;

    Matrix_MultVec3fNoTranslate(mtx, &origin, &originP);
    Matrix_MultVec3fNoTranslate(mtx, &zHat, &zHatP);
    Matrix_MultVec3fNoTranslate(mtx, &xHat, &xHatP);
    zHatP[0] -= originP[0];
    zHatP[1] -= originP[1];
    zHatP[2] -= originP[2];
    xHatP[0] -= originP[0];
    xHatP[1] -= originP[1];
    xHatP[2] -= originP[2];
    *rot[1] = atan2f(zHatP[0], zHatP[2]);
    *rot[0] = -atan2f(zHatP[1], sqrtf(SQ(zHatP[0]) + SQ(zHatP[2])));
    Matrix_RotateX(&invYP, -*rot[0], MTXF_NEW);
    Matrix_RotateY(&invYP, -*rot[1], MTXF_APPLY);
    Matrix_MultVec3fNoTranslate(&invYP, &xHatP, &xHat);
    *rot[0] *= M_RTOD;
    *rot[1] *= M_RTOD;
    *rot[2] = atan2f(xHat[1], xHat[0]) * M_RTOD;
}

// Expresses the rotational part of the transform mtx as Tait-Bryan angles, in the extrinsic XYZ convention used in
// modelspace calculations
void Matrix_GetXYZAngles(Matrix* mtx, Vec3f* rot) {
    Matrix invYZ;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };
    Vec3f originP;
    Vec3f xHat = { 1.0f, 0.0f, 0.0f };
    Vec3f xHatP;
    Vec3f yHat = { 0.0f, 1.0f, 0.0f };
    Vec3f yHatP;

    Matrix_MultVec3fNoTranslate(mtx, &origin, &originP);
    Matrix_MultVec3fNoTranslate(mtx, &xHat, &xHatP);
    Matrix_MultVec3fNoTranslate(mtx, &yHat, &yHatP);
    xHatP[0] -= originP[0];
    xHatP[1] -= originP[1];
    xHatP[2] -= originP[2];
    yHatP[0] -= originP[0];
    yHatP[1] -= originP[1];
    yHatP[2] -= originP[2];
    *rot[2] = atan2f(xHatP[1], xHatP[0]);
    *rot[1] = -atan2f(xHatP[2], sqrtf(SQ(xHatP[0]) + SQ(xHatP[1])));
    Matrix_RotateY(&invYZ, -*rot[1], MTXF_NEW);
    Matrix_RotateZ(&invYZ, -*rot[2], MTXF_APPLY);
    Matrix_MultVec3fNoTranslate(&invYZ, &yHatP, &yHat);
    *rot[0] = atan2f(yHat[2], yHat[1]) * M_RTOD;
    *rot[1] *= M_RTOD;
    *rot[2] *= M_RTOD;
}

// Creates a look-at matrix from Eye, At, and Up in mtx (MTXF_NEW) or applies one to mtx (MTXF_APPLY).
// A look-at matrix is a rotation-translation matrix that maps y to Up, z to (At - Eye), and translates to Eye
void Matrix_LookAt(Matrix* mtx, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp,
                   u8 mode) {
    Matrix lookAt;

    guLookAtF(lookAt.mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    Matrix_Mult(mtx, &lookAt, mode);
}

// Converts the current Gfx matrix to a Mtx and sets it to the display list
void Matrix_SetGfxMtx(Gfx** gfx) {
    Matrix_ToMtx(gGfxMtx);
    gSPMatrix((*gfx)++, gGfxMtx++, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}