#include <libultraship.h>
#include <mk64.h>
#include <macros.h>
#include <math_util.h>
#include <main.h>
#include "buffers.h"
#include "buffers/trig_tables.h"
#include "math.h"
#include "memory.h"
#include "engine/Matrix.h"
#include "course.h"
#include "port/Game.h"
#include <port/interpolation/FrameInterpolation.h>
#include <port/interpolation/matrix.h>
#pragma intrinsic(sqrtf, fabs)

s32 D_802B91C0[2] = { 13, 13 };
Vec3f D_802B91C8 = { 0.0f, 0.0f, 0.0f };

// Mtx gIdentityMatrix = {
//     toFixedPointMatrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0),
// };

// This functions looks similar to a segment of code from func_802A4A0C in skybox_and_splitscreen.c
UNUSED s32 func_802B4F60(UNUSED s32 arg0, Vec3f arg1, UNUSED s32 arg2, UNUSED f32 arg3, UNUSED f32 arg4) {
    Mat4 sp30;
    f32 sp2C;
    f32 sp28;
    Vec3f sp1C;
    vec3f_copy_return(sp1C, arg1);
    sp28 = sp1C[0];
    sp2C = sp1C[1];
    // wut?
    if (sp2C && sp2C) {};
    sp2C = ((sp30[0][3] * sp28) + (sp30[1][3] * sp2C) + (sp30[2][3] * sp1C[2])) + sp30[3][3];
    // double wut?
    if (sp28 && sp28) {};
    mtxf_translate_vec3f_mat4(sp1C, sp30);
    if (0.0f >= sp2C) {
        return 0;
    } else {
        return 1;
    }
}

UNUSED void func_802B4FF0() {
}

/**
 * Inserts matrix into the rsp. Position, rotation and mode of where to render the next object and check number of
 * object already render Note that gMatrixObjectCount gets reset at the beginning of the game loop. So no cleanup needs
 * to be performed.
 */
s32 render_set_position(Mat4 mtx, s32 arg1) {
    if (gMatrixObjectCount >= MTX_OBJECT_POOL_SIZE) {
        return 0;
    }
    // mtxf_to_mtx(&gGfxPool->mtxObject[gMatrixObjectCount], arg0);
    switch (arg1) { /* irregular */
        case 0:
            AddObjectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxObject[gMatrixObjectCount++]),
            //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 1:
            AddObjectMatrix(mtx, G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxObject[gMatrixObjectCount++]),
            //           G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 3:
            AddObjectMatrix(mtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxObject[gMatrixObjectCount++]),
            //           G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
        case 2:
            AddObjectMatrix(mtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxObject[gMatrixObjectCount++]),
            //           G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
    }
    return 1;
}

f32 func_802B51E8(Vec3f arg0, Vec3f arg1) {
    f32 sub_y;
    f32 sub_z;
    f32 sub_x;

    sub_x = arg1[0] - arg0[0];
    sub_y = arg1[1] - arg0[1];
    sub_z = arg1[2] - arg0[2];
    return (sub_x * sub_x) + (sub_y * sub_y) + sub_z + sub_z;
}

s32 get_angle_between_two_vectors(Vec3f arg0, Vec3f arg1) {
    f32 temp_v1;
    f32 temp_v2;
    temp_v1 = arg1[0] - arg0[0];
    temp_v2 = arg1[2] - arg0[2];

    return atan2s(temp_v1, temp_v2);
}

// get_angle_between_two_vectors
u32 func_802B5258(Vec3f arg0, Vec3s arg1) {
    f32 temp_v1;
    f32 temp_v2;
    temp_v1 = arg1[0] - arg0[0];
    temp_v2 = arg1[2] - arg0[2];

    return atan2s(temp_v1, temp_v2);
}

void vec3f_set(Vec3f arg0, f32 arg1, f32 arg2, f32 arg3) {
    arg0[0] = arg1;
    arg0[1] = arg2;
    arg0[2] = arg3;
}

void vec3s_set(Vec3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    arg0[0] = arg1;
    arg0[1] = arg2;
    arg0[2] = arg3;
}

// These functions have bogus return values.
// Disable the compiler warning.
#pragma GCC diagnostic push

#ifdef __GNUC__
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wreturn-stack-address"
#else
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif
#endif

void* vec3f_copy_return(Vec3f dest, Vec3f src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    //! @warning function returns address of local variable
    return &dest;
}

void vec3s_copy(Vec3s dest, Vec3s src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

UNUSED void* vec3f_set_return(Vec3f dest, f32 x, f32 y, f32 z) {
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return &dest;
}

// Copy mat1 to mat2
void mtxf_copy(Mat4 mat1, Mat4 mat2) {
    s32 row;
    s32 column;

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            mat2[row][column] = mat1[row][column];
        }
    }
}

// mtxf_copy
void mtxf_copy_n_element(s32* dest, s32* src, s32 n) {
    while (n-- > 0) {
        *dest++ = *src++;
    }
}

// Transform a matrix to a matrix identity
void mtxf_identity(Mat4 mtx) {
    register s32 i;
    register s32 k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < 4; k++) {
            mtx[i][k] = (i == k) ? 1.0f : 0.0f;
        }
    }
}

// Add a translation vector to a matrix, mat is the matrix to add, dest is the destination matrix, pos is the
// translation vector
void add_translate_mat4_vec3f(Mat4 mat, Mat4 dest, Vec3f pos) {
    FrameInterpolation_RecordMatrixTranslate(dest, pos);
    dest[3][0] = mat[3][0] + pos[0];
    dest[3][1] = mat[3][1] + pos[1];
    dest[3][2] = mat[3][2] + pos[2];
    dest[3][3] = mat[3][3];
    dest[0][0] = mat[0][0];
    dest[0][1] = mat[0][1];
    dest[0][2] = mat[0][2];
    dest[0][3] = mat[0][3];
    dest[1][0] = mat[1][0];
    dest[1][1] = mat[1][1];
    dest[1][2] = mat[1][2];
    dest[1][3] = mat[1][3];
    dest[2][0] = mat[2][0];
    dest[2][1] = mat[2][1];
    dest[2][2] = mat[2][2];
    dest[2][3] = mat[2][3];

    /*
     * mat(0,0)        mat(0,1)        mat(0,2)        mat(0,3)
     * mat(1,0)        mat(1,1)        mat(1,2)        mat(1,3)
     * mat(2,0)        mat(2,1)        mat(2,2)        mat(2,3)
     * mat(3,0)+pos(0) mat(3,1)+pos(1) mat(3,2)+pos(2) mat(3,3)
     */
}

// Light version of add_translate_mat4_vec3f
UNUSED void add_translate_mat4_vec3f_lite(Mat4 mat, Mat4 dest, Vec3f pos) {
    dest[3][0] = mat[3][0] + pos[0];
    dest[3][1] = mat[3][1] + pos[1];
    dest[3][2] = mat[3][2] + pos[2];
}

// create a translation matrix
void mtxf_translate(Mat4 dest, Vec3f b) {
    FrameInterpolation_RecordMatrixTranslate(dest, b);
    mtxf_identity(dest);
    dest[3][0] = b[0];
    dest[3][1] = b[1];
    dest[3][2] = b[2];
}

// Note the use of `2` which generates diff asm than just using floats (2.0f).
void func_802B5564(Mat4 arg0, u16* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp;
    s32 i, j;
    mtxf_identity(arg0);
    arg2 *= 0.017453292222222222;
    temp = cosf(arg2 / 2) / sinf(arg2 / 2);
    arg0[0][0] = temp / arg3;
    arg0[1][1] = temp;
    arg0[2][2] = (arg4 + arg5) / (arg4 - arg5);
    arg0[2][3] = -1.0f;
    arg0[3][2] = (2 * arg4 * arg5) / (arg4 - arg5);
    arg0[3][3] = 0.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            arg0[i][j] *= arg6;
        }
    }

    if (arg1 != 0) {
        if ((arg4 + arg5) <= 2.0) {
            *arg1 = 0xFFFF;
        } else {
            *arg1 = 131072.0 / (arg4 + arg5);
            if (*arg1 <= 0) {
                *arg1 = 1;
            }
        }
    }
}

// Appears to only be for the skybox. mtxf_lookat from sm64 with some modifications.
void func_802B5794(Mat4 mtx, Vec3f from, Vec3f to) {
    // register from sm64 but not required for matching.
    register f32 invLength;
    f32 xColY;
    f32 yColY;
    f32 zColY;
    f32 xColZ;
    f32 yColZ;
    f32 zColZ;
    f32 xColX;
    f32 yColX;
    f32 zColX;

    xColY = 0.0f;
    yColY = 1.0f;
    zColY = 0.0f;

    xColZ = to[0] - from[0];
    yColZ = to[1] - from[1];
    zColZ = to[2] - from[2];

    invLength = -1.0 / sqrtf(xColZ * xColZ + yColZ * yColZ + zColZ * zColZ);
    xColZ *= invLength;
    yColZ *= invLength;
    zColZ *= invLength;

    xColX = yColY * zColZ - zColY * yColZ;
    yColX = zColY * xColZ - xColY * zColZ;
    zColX = xColY * yColZ - yColY * xColZ;

    invLength = 1.0 / sqrtf(xColX * xColX + yColX * yColX + zColX * zColX);

    xColX *= invLength;
    yColX *= invLength;
    zColX *= invLength;

    xColY = yColZ * zColX - zColZ * yColX;
    yColY = zColZ * xColX - xColZ * zColX;
    zColY = xColZ * yColX - yColZ * xColX;

    invLength = 1.0 / sqrtf(xColY * xColY + yColY * yColY + zColY * zColY);
    xColY *= invLength;
    yColY *= invLength;
    zColY *= invLength;

    mtx[0][0] = xColX;
    mtx[1][0] = yColX;
    mtx[2][0] = zColX;
    mtx[3][0] = -(from[0] * xColX + from[1] * yColX + from[2] * zColX);

    mtx[0][1] = xColY;
    mtx[1][1] = yColY;
    mtx[2][1] = zColY;
    mtx[3][1] = -(from[0] * xColY + from[1] * yColY + from[2] * zColY);

    mtx[0][2] = xColZ;
    mtx[1][2] = yColZ;
    mtx[2][2] = zColZ;
    mtx[3][2] = -(from[0] * xColZ + from[1] * yColZ + from[2] * zColZ);

    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
}

// create a rotation matrix around the x axis
void mtxf_rotate_x(Mat4 mat, s16 angle) {
    FrameInterpolation_RecordMatrixRotate1Coord(&mat, 0, angle);
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle);

    mtxf_identity(mat);
    mat[1][1] = cos_theta;
    mat[1][2] = sin_theta;
    mat[2][1] = -sin_theta;
    mat[2][2] = cos_theta;

    /*
     * 1, 0, 0, 0,
     * 0, cos_theta, sin_theta, 0,
     * 0, -sin_theta, cos_theta, 0,
     * 0, 0, 0, 1
     */
}

// create a rotation matrix around the y axis
void mtxf_rotate_y(Mat4 mat, s16 angle) {
    FrameInterpolation_RecordMatrixRotate1Coord(&mat, 1, angle);
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle);

    mtxf_identity(mat);
    mat[0][0] = cos_theta;
    mat[0][2] = -sin_theta;
    mat[2][0] = sin_theta;
    mat[2][2] = cos_theta;

    /*
     * cos_theta, 0, -sin_theta, 0,
     * 0, 1, 0, 0,
     * sin_theta, 0, cos_theta, 0,
     * 0, 0, 0, 1
     */
}

// create a rotation matrix around the z axis
void mtxf_s16_rotate_z(Mat4 mat, s16 angle) {
    FrameInterpolation_RecordMatrixRotate1Coord(&mat, 2, angle);
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle);

    mtxf_identity(mat);
    mat[0][0] = cos_theta;
    mat[0][1] = sin_theta;
    mat[1][0] = -sin_theta;
    mat[1][1] = cos_theta;

    /*
     * cos_theta, sin_theta, 0, 0,
     * -sin_theta, cos_theta, 0, 0,
     * 0, 0, 1, 0,
     * 0, 0, 0, 1
     */
}

void func_802B5CAC(s16 arg0, s16 arg1, Vec3f arg2) {
    f32 sp2C = sins(arg1);
    f32 sp28 = coss(arg1);
    f32 sp24 = sins(arg0);
    f32 temp_f10 = coss(arg0);

    arg2[0] = sp28 * sp24;
    arg2[1] = sp2C;
    arg2[2] = -(sp28 * temp_f10);
}

void func_802B5D30(s16 arg0, s16 arg1, s32 arg2) {
    set_track_light_direction((Lights1*) 0x9000000, arg0, arg1, arg2);
}

void set_track_light_direction(Lights1* addr, s16 pitch, s16 yaw, s32 numLights) {
    UNUSED s32 pad;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    UNUSED s32 pad2[2];
    f32 temp_f10;
    s32 var_v0;
    s8 sp2C[3];
    Lights1* var_s0;

    var_s0 = (Lights1*) addr;
    sp48 = sins(yaw);
    sp44 = coss(yaw);
    sp40 = sins(pitch);
    temp_f10 = coss(pitch);
    sp2C[0] = sp44 * sp40 * 120.0f;
    sp2C[1] = 120.0f * sp48;
    sp2C[2] = sp44 * temp_f10 * -120.0f;
    for (var_v0 = 0; var_v0 < numLights; var_v0++, var_s0++) {
        var_s0->l[0].l.dir[0] = sp2C[0];
        var_s0->l[0].l.dir[1] = sp2C[1];
        var_s0->l[0].l.dir[2] = sp2C[2];
    }
}

// multiply a matrix with a number
void mtxf_scale(Mat4 mat, f32 coef) {
    FrameInterpolation_RecordMatrixScale(mat, coef);
    mat[0][0] *= coef;
    mat[1][0] *= coef;
    mat[2][0] *= coef;
    mat[0][1] *= coef;
    mat[1][1] *= coef;
    mat[2][1] *= coef;
    mat[0][2] *= coef;
    mat[1][2] *= coef;
    mat[2][2] *= coef;
}

// look like create a translation and rotation matrix with arg1 position and arg2 rotation
void mtxf_pos_rotation_xyz(Mat4 out, Vec3f pos, Vec3s orientation) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;
    FrameInterpolation_RecordMatrixPosRotXYZ(out, pos, orientation);

    sine1 = sins(orientation[0]);
    cosine1 = coss(orientation[0]);
    sine2 = sins(orientation[1]);
    cosine2 = coss(orientation[1]);
    sine3 = sins(orientation[2]);
    cosine3 = coss(orientation[2]);
    out[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    out[1][0] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    out[2][0] = cosine1 * sine2;
    out[3][0] = pos[0];
    out[0][1] = cosine1 * sine3;
    out[1][1] = cosine1 * cosine3;
    out[2][1] = -sine1;
    out[3][1] = pos[1];
    out[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    out[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    out[2][2] = cosine1 * cosine2;
    out[3][2] = pos[2];
    out[0][3] = 0.0f;
    out[1][3] = 0.0f;
    out[2][3] = 0.0f;
    out[3][3] = 1.0f;
}

UNUSED void func_802B60B4(Mat4 arg0, Vec3s arg1, Vec3s arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sins(arg2[0]);
    cosine1 = coss(arg2[0]);
    sine2 = sins(arg2[1]);
    cosine2 = coss(arg2[1]);
    sine3 = sins(arg2[2]);
    cosine3 = coss(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[0][1] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    arg0[0][2] = cosine1 * sine2;
    arg0[0][3] = (f32) arg1[0];
    arg0[1][0] = (f32) (cosine1 * sine3);
    arg0[1][1] = (f32) (cosine1 * cosine3);
    arg0[1][2] = (f32) -sine1;
    arg0[1][3] = (f32) arg1[1];
    arg0[2][0] = (f32) ((-sine2 * cosine3) + ((sine1 * cosine2) * sine3));
    arg0[2][1] = (f32) ((sine2 * sine3) + ((sine1 * cosine2) * cosine3));
    arg0[2][2] = (f32) (cosine1 * cosine2);
    arg0[2][3] = (f32) arg1[2];
    arg0[3][0] = 0.0f;
    arg0[3][1] = 0.0f;
    arg0[3][2] = 0.0f;
    arg0[3][3] = 1.0f;
}

UNUSED void func_802B6214(Mat4 arg0, Vec3s arg1, Vec3s arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sins(arg2[0]);
    cosine1 = coss(arg2[0]);
    sine2 = sins(arg2[1]);
    cosine2 = coss(arg2[1]);
    sine3 = sins(arg2[2]);
    cosine3 = coss(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[1][0] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    arg0[2][0] = cosine1 * sine2;
    arg0[3][0] = arg1[0];
    arg0[0][1] = cosine1 * sine3;
    arg0[1][1] = cosine1 * cosine3;
    arg0[2][1] = -sine1;
    arg0[3][1] = arg1[1];
    arg0[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    arg0[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    arg0[2][2] = cosine1 * cosine2;
    arg0[3][2] = arg1[2];
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

// look like to normalise vector
UNUSED void func_802B6374(Vec3f arg0) {
    f32 temp_f0;
    temp_f0 = sqrtf((arg0[0] * arg0[0]) + (arg0[1] * arg0[1]) + (arg0[2] * arg0[2]));
    arg0[0] /= temp_f0;
    arg0[1] /= temp_f0;
    arg0[2] /= temp_f0;
}

// translate the vector with a matrix
void mtxf_translate_vec3f_mat3(Vec3f pos, Mat3 mat) {
    f32 new_x;
    f32 new_y;
    f32 new_z;

    new_x = (mat[0][0] * pos[0]) + (mat[0][1] * pos[1]) + (mat[0][2] * pos[2]);
    new_y = (mat[1][0] * pos[0]) + (mat[1][1] * pos[1]) + (mat[1][2] * pos[2]);
    new_z = (mat[2][0] * pos[0]) + (mat[2][1] * pos[1]) + (mat[2][2] * pos[2]);

    pos[0] = new_x;
    pos[1] = new_y;
    pos[2] = new_z;
}

// translate the vector with a matrix (with a matrix 4x4)
void mtxf_translate_vec3f_mat4(Vec3f pos, Mat4 mat) {
    f32 new_x;
    f32 new_y;
    f32 new_z;

    new_x = (mat[0][0] * pos[0]) + (mat[0][1] * pos[1]) + (mat[0][2] * pos[2]);
    new_y = (mat[1][0] * pos[0]) + (mat[1][1] * pos[1]) + (mat[1][2] * pos[2]);
    new_z = (mat[2][0] * pos[0]) + (mat[2][1] * pos[1]) + (mat[2][2] * pos[2]);

    pos[0] = new_x;
    pos[1] = new_y;
    pos[2] = new_z;
}

UNUSED void func_802B64B0(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
}

void func_802B64C4(Vec3f arg0, s16 arg1) {
    f32 sp2C = sins(arg1);
    f32 temp_f0 = coss(arg1);

    f32 temp1 = arg0[0];
    f32 temp2 = arg0[1];
    f32 temp3 = arg0[2];

    arg0[0] = temp_f0 * temp1 - (sp2C * temp3);
    arg0[1] = temp2;
    arg0[2] = sp2C * temp1 + (temp_f0 * temp3);
}

// Rotates the object around the Y axis.
// x,y,z is a direction (not a rotator).
void calculate_orientation_matrix(Mat3 dest, f32 x, f32 y, f32 z, s16 rotationAngle) {
    Mat3 mtx_rot_y;
    Mat3 matrix;
    s32 i, j;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    UNUSED s32 pad[3];
    f32 sinValue;
    f32 cossValue;
    FrameInterpolation_RecordCalculateOrientationMatrix(dest, x, y, z, rotationAngle);

    sinValue = sins(rotationAngle);
    cossValue = coss(rotationAngle);
    mtx_rot_y[0][0] = cossValue;
    mtx_rot_y[2][1] = 0;
    mtx_rot_y[1][2] = 0;

    mtx_rot_y[1][1] = 1;
    mtx_rot_y[2][0] = sinValue;
    mtx_rot_y[0][2] = -sinValue;

    mtx_rot_y[2][2] = cossValue;
    mtx_rot_y[1][0] = 0;
    mtx_rot_y[0][1] = 0;

    if (y == 1) { // set matrix to identity

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }

    } else if (y == -1) { // set matrix to identity with the second column negative

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }

        matrix[1][1] = -1;

    } else {
        a = (f32) - (360.0 - ((f64) (calculate_vector_angle_xy(y) * 180.0f) / M_PI));
        b = -z / sqrtf((x * x) + (z * z));
        c = 0;
        d = x / sqrtf((x * x) + (z * z));
        calculate_rotation_matrix(matrix, a, b, c, d);
    }
    dest[0][0] = (mtx_rot_y[0][0] * matrix[0][0]) + (mtx_rot_y[0][1] * matrix[1][0]) + (mtx_rot_y[0][2] * matrix[2][0]);
    dest[1][0] = (mtx_rot_y[1][0] * matrix[0][0]) + (mtx_rot_y[1][1] * matrix[1][0]) + (mtx_rot_y[1][2] * matrix[2][0]);
    dest[2][0] = (mtx_rot_y[2][0] * matrix[0][0]) + (mtx_rot_y[2][1] * matrix[1][0]) + (mtx_rot_y[2][2] * matrix[2][0]);

    dest[0][1] = (mtx_rot_y[0][0] * matrix[0][1]) + (mtx_rot_y[0][1] * matrix[1][1]) + (mtx_rot_y[0][2] * matrix[2][1]);
    dest[1][1] = (mtx_rot_y[1][0] * matrix[0][1]) + (mtx_rot_y[1][1] * matrix[1][1]) + (mtx_rot_y[1][2] * matrix[2][1]);
    dest[2][1] = (mtx_rot_y[2][0] * matrix[0][1]) + (mtx_rot_y[2][1] * matrix[1][1]) + (mtx_rot_y[2][2] * matrix[2][1]);

    dest[0][2] = (mtx_rot_y[0][0] * matrix[0][2]) + (mtx_rot_y[0][1] * matrix[1][2]) + (mtx_rot_y[0][2] * matrix[2][2]);
    dest[1][2] = (mtx_rot_y[1][0] * matrix[0][2]) + (mtx_rot_y[1][1] * matrix[1][2]) + (mtx_rot_y[1][2] * matrix[2][2]);
    dest[2][2] = (mtx_rot_y[2][0] * matrix[0][2]) + (mtx_rot_y[2][1] * matrix[1][2]) + (mtx_rot_y[2][2] * matrix[2][2]);
}

// include in calculate_orientation_matrix
UNUSED void func_802B68F8(Mat3 matrix, f32 arg1, f32 arg2, f32 arg3) {
    s32 i, j;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    UNUSED f32 pad;

    if (arg2 == 1) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    } else if (arg2 == -1) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
        matrix[1][1] = -1.0f;
    } else {
        a = (f32) - (360.0 - ((f64) (calculate_vector_angle_xy(arg2) * 180.0f) / M_PI));
        b = -arg3 / sqrtf((arg1 * arg1) + (arg3 * arg3));
        c = 0;
        d = arg1 / sqrtf((arg1 * arg1) + (arg3 * arg3));
        calculate_rotation_matrix(matrix, a, b, c, d);
    }
}

void calculate_rotation_matrix(Mat3 destMatrix, s16 rotationAngle, f32 rotationX, f32 rotationY, f32 rotationZ) {
    f32 sinValue;
    f32 cossValue;
    f32 temp_f12;
    f32 temp_f10;
    f32 temp_f2;
    f32 temp;
    UNUSED s32 pad[2];

    sinValue = sins((u16) rotationAngle);
    cossValue = coss((u16) rotationAngle);

    temp_f12 = 1.0f - cossValue;

    temp_f10 = (rotationZ * rotationX) * temp_f12;
    temp_f2 = (rotationY * rotationZ) * temp_f12;
    temp = ((rotationX * rotationY) * temp_f12);

    temp_f12 = rotationX * rotationX;
    destMatrix[0][0] = ((1.0f - temp_f12) * cossValue) + temp_f12;
    destMatrix[2][1] = temp_f2 - (rotationX * sinValue);
    destMatrix[1][2] = temp_f2 + (rotationX * sinValue);

    temp_f12 = rotationY * rotationY;
    destMatrix[1][1] = (((1.0f - temp_f12) * cossValue) + temp_f12);
    destMatrix[2][0] = temp_f10 + (rotationY * sinValue);
    destMatrix[0][2] = temp_f10 - (rotationY * sinValue);

    temp_f12 = rotationZ * rotationZ;
    destMatrix[2][2] = (((1.0f - temp_f12) * cossValue) + temp_f12);
    destMatrix[1][0] = temp - (rotationZ * sinValue);
    destMatrix[0][1] = temp + (rotationZ * sinValue);
}

UNUSED void func_802B6BC0(Mat4 arg0, s16 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 sine;
    f32 cosine;
    f32 temp_f0;
    f32 temp_f12;

    sine = sins(arg1);
    cosine = coss(arg1);
    temp_f0 = sqrtf((arg2 * arg2) + (arg4 * arg4));
    if (temp_f0 != 0.0f) {
        temp_f12 = 1.0f / temp_f0;
        arg0[0][0] = ((-arg4 * cosine) - ((sine * arg3) * arg2)) * temp_f12;
        arg0[0][3] = ((arg4 * sine) - ((cosine * arg3) * arg2)) * temp_f12;
        arg0[1][2] = -arg2;
        arg0[0][1] = sine * temp_f0;
        arg0[1][0] = cosine * temp_f0;
        arg0[1][3] = -arg3;
        arg0[0][2] = ((cosine * arg2) - ((sine * arg3) * arg4)) * temp_f12;
        arg0[1][1] = ((-sine * arg2) - ((cosine * arg3) * arg4)) * temp_f12;
        arg0[2][0] = -arg4;
        arg0[0][3] = 0.0f;
        arg0[1][2] = 0.0f;
        arg0[2][1] = 0.0f;
    } else {
        arg0[0][1] = 0.0f;
        arg0[1][3] = 0.0f;
        arg0[0][2] = 0.0f;
        arg0[1][1] = 0.0f;
        arg0[0][3] = 0.0f;
        arg0[1][2] = 0.0f;
        arg0[2][1] = 0.0f;
        arg0[0][0] = 1.0f;
        arg0[1][0] = 1.0f;
        arg0[2][0] = 1.0f;
    }
}

// look like create a translation and rotation matrix with arg1 position and arg2 rotation
void func_802B6D58(Mat4 arg0, Vec3f arg1, Vec3f arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sins(arg2[0]);
    cosine1 = coss(arg2[0]);
    sine2 = sins(arg2[1]);
    cosine2 = coss(arg2[1]);
    sine3 = sins(arg2[2]);
    cosine3 = coss(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[1][0] = (-cosine2 * sine3) + (sine1 * sine2) * cosine3;
    arg0[2][0] = cosine1 * sine2;
    arg0[3][0] = arg1[0];
    arg0[0][1] = cosine1 * sine3;
    arg0[1][1] = cosine1 * cosine3;
    arg0[2][1] = -sine1;
    arg0[3][1] = arg1[1];
    arg0[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    arg0[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    arg0[2][2] = cosine1 * cosine2;
    arg0[3][2] = arg1[2];
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

void mtxf_multiplication(Mat4 dest, Mat4 mat1, Mat4 mat2) {
    FrameInterpolation_RecordMatrixMult(dest, dest, 0);

    dest[0][0] =
        (mat1[0][0] * mat2[0][0]) + (mat1[0][1] * mat2[1][0]) + (mat1[0][2] * mat2[2][0]) + (mat1[0][3] * mat2[3][0]);
    dest[0][1] =
        (mat1[0][0] * mat2[0][1]) + (mat1[0][1] * mat2[1][1]) + (mat1[0][2] * mat2[2][1]) + (mat1[0][3] * mat2[3][1]);
    dest[0][2] =
        (mat1[0][0] * mat2[0][2]) + (mat1[0][1] * mat2[1][2]) + (mat1[0][2] * mat2[2][2]) + (mat1[0][3] * mat2[3][2]);
    dest[0][3] =
        (mat1[0][0] * mat2[0][3]) + (mat1[0][1] * mat2[1][3]) + (mat1[0][2] * mat2[2][3]) + (mat1[0][3] * mat2[3][3]);
    dest[1][0] =
        (mat1[1][0] * mat2[0][0]) + (mat1[1][1] * mat2[1][0]) + (mat1[1][2] * mat2[2][0]) + (mat1[1][3] * mat2[3][0]);
    dest[1][1] =
        (mat1[1][0] * mat2[0][1]) + (mat1[1][1] * mat2[1][1]) + (mat1[1][2] * mat2[2][1]) + (mat1[1][3] * mat2[3][1]);
    dest[1][2] =
        (mat1[1][0] * mat2[0][2]) + (mat1[1][1] * mat2[1][2]) + (mat1[1][2] * mat2[2][2]) + (mat1[1][3] * mat2[3][2]);
    dest[1][3] =
        (mat1[1][0] * mat2[0][3]) + (mat1[1][1] * mat2[1][3]) + (mat1[1][2] * mat2[2][3]) + (mat1[1][3] * mat2[3][3]);
    dest[2][0] =
        (mat1[2][0] * mat2[0][0]) + (mat1[2][1] * mat2[1][0]) + (mat1[2][2] * mat2[2][0]) + (mat1[2][3] * mat2[3][0]);
    dest[2][1] =
        (mat1[2][0] * mat2[0][1]) + (mat1[2][1] * mat2[1][1]) + (mat1[2][2] * mat2[2][1]) + (mat1[2][3] * mat2[3][1]);
    dest[2][2] =
        (mat1[2][0] * mat2[0][2]) + (mat1[2][1] * mat2[1][2]) + (mat1[2][2] * mat2[2][2]) + (mat1[2][3] * mat2[3][2]);
    dest[2][3] =
        (mat1[2][0] * mat2[0][3]) + (mat1[2][1] * mat2[1][3]) + (mat1[2][2] * mat2[2][3]) + (mat1[2][3] * mat2[3][3]);
    dest[3][0] =
        (mat1[3][0] * mat2[0][0]) + (mat1[3][1] * mat2[1][0]) + (mat1[3][2] * mat2[2][0]) + (mat1[3][3] * mat2[3][0]);
    dest[3][1] =
        (mat1[3][0] * mat2[0][1]) + (mat1[3][1] * mat2[1][1]) + (mat1[3][2] * mat2[2][1]) + (mat1[3][3] * mat2[3][1]);
    dest[3][2] =
        (mat1[3][0] * mat2[0][2]) + (mat1[3][1] * mat2[1][2]) + (mat1[3][2] * mat2[2][2]) + (mat1[3][3] * mat2[3][2]);
    dest[3][3] =
        (mat1[3][0] * mat2[0][3]) + (mat1[3][1] * mat2[1][3]) + (mat1[3][2] * mat2[2][3]) + (mat1[3][3] * mat2[3][3]);
}

/**
 * Convert float matrix 'src' to fixed point matrix 'dest'.
 * The float matrix may not contain entries larger than 65536 or the console
 * crashes. The fixed point matrix has entries with a 16-bit integer part, so
 * the floating point numbers are multiplied by 2^16 before being cast to a s32
 * integer. If this doesn't fit, the N64 and iQue consoles will throw an
 * exception. On Wii and Wii U Virtual Console the value will simply be clamped
 * and no crashes occur.
 */
void mtxf_to_mtx(Mtx* dest, Mat4 src) {
#ifdef AVOID_UB
    // Avoid type-casting which is technically UB by calling the equivalent
    // guMtxF2L function. This helps little-endian systems, as well.
    guMtxF2L(src, dest);
#else
    s32 asFixedPoint;
    register s32 i;
    register s16* a3 = (s16*) dest;      // all integer parts stored in first 16 bytes
    register s16* t0 = (s16*) dest + 16; // all fraction parts stored in last 16 bytes
    register f32* t1 = (f32*) src;

    for (i = 0; i < 16; i++) {
        asFixedPoint = *t1++ * (1 << 16);         //! float-to-integer conversion responsible for PU crashes
        *a3++ = GET_HIGH_S16_OF_32(asFixedPoint); // integer part
        *t0++ = GET_LOW_S16_OF_32(asFixedPoint);  // fraction part
    }
#endif
}

/**
 * Comment from sm64 unverified. mk64 verison is modified
 *
 * Helper function for atan2s. Does a look up of the arctangent of y/x assuming
 * the resulting angle is in range [0, 0x2000] (1/8 of a circle).
 */

u16 atan2_lookup(f32 y, f32 x) {
    u16 ret;

    if (x == 0) {
        ret = gArctanTable[0];
    } else {
        if (1000000.0f < y / x) {
            if (y > 0.0f) {
                ret = 0x4000;
            } else {
                ret = 0xC000;
            }
        } else {
            ret = gArctanTable[(s32) (y / x * 1024 + 0.5f)];
        }
    }
    return ret;
}

/**
 * Compute the angle from (0, 0) to (x, y) as a u16. Given that terrain is in
 * the xz-plane, this is commonly called with (z, x) to get a yaw angle.
 * sm64 but x, y swapped and returns u16.
 */
u16 atan2s(f32 x, f32 y) {
    u16 ret;
    if (x >= 0) {
        if (y >= 0) {
            if (y >= x) {
                ret = atan2_lookup(x, y);
            } else {
                ret = 0x4000 - atan2_lookup(y, x);
            }
        } else {
            y = -y;
            if (y < x) {
                ret = 0x4000 + atan2_lookup(y, x);
            } else {
                ret = 0x8000 - atan2_lookup(x, y);
            }
        }
    } else {
        x = -x;
        if (y < 0) {
            y = -y;
            if (y >= x) {
                ret = 0x8000 + atan2_lookup(x, y);
            } else {
                ret = 0xC000 - atan2_lookup(y, x);
            }
        } else {
            if (y < x) {
                ret = 0xC000 + atan2_lookup(y, x);
            } else {
                ret = -atan2_lookup(x, y);
            }
        }
    }
    return ret;
}

f32 _atan2f(f32 arg0, f32 arg1) {
    return atan2s(arg0, arg1);
}

#ifndef NON_MATCHING // The decomp does not support fabs
UNUSED f32 func_802B79F0(f32 arg0, f32 arg1) {
    f64 halfpi;
    f32 temp_f0;
    UNUSED f32 pad;
    f32 temp_f2;
    f32 var_f16;
    f32 var_f2;
    s32 var_v0;

    var_f16 = arg0 / arg1;
    var_v0 = 0;
    if (fabs(arg1) < fabs(arg0)) {
        var_v0 = 1;
    }
    if (var_v0 != 0) {
        var_f16 = arg1 / arg0;
    }
    temp_f0 = var_f16 * var_f16;
    temp_f2 = temp_f0 * temp_f0;
    var_f16 +=
        ((((((((temp_f2 * ((-0.01600503f) + (temp_f0 * 0.00283406f))) + (-0.07495445f)) + (temp_f0 * 0.04258761f)) *
             (temp_f2 * temp_f2)) +
            (((-0.14202571f) + (temp_f0 * 0.10636754f)) * temp_f2)) +
           (-0.33333066f)) +
          (temp_f0 * 0.19992484f)) *
         (var_f16 * temp_f0));

    if (var_v0 != 0) {
        halfpi = 1.5707963267948966;
        return (arg0 < 0.0f ? -halfpi : halfpi) - var_f16;
    }
    if (arg1 >= 0.0f) {
        return var_f16;
    }
    var_f2 = var_f16 + 3.1415927f;
    if (arg0 < 0.0f) {
        var_f2 = var_f16 - 3.1415927f;
    }
    return var_f2;
}
#endif

UNUSED u16 func_802B7B50(f32 arg0, f32 arg1) {
    return ((_atan2f(arg0, arg1) * 32768.0f) / M_PI);
}

UNUSED void func_802B7C18(f32 arg0) {
    _atan2f(arg0, 1.0f);
}

s16 func_802B7C40(f32 arg0) {
    return atan2s(arg0, 1.0f);
}

UNUSED void func_802B7C6C(f32 arg0) {
    _atan2f(arg0, sqrtf(1.0 - (arg0 * arg0)));
}

s16 func_802B7CA8(f32 arg0) {
    return atan2s(arg0, sqrtf(1.0 - (arg0 * arg0)));
}

f32 calculate_vector_angle_xy(f32 vectorX) {
    return _atan2f(sqrtf(1.0 - (vectorX * vectorX)), vectorX);
}

UNUSED s16 func_802B7D28(f32 arg0) {
    return _atan2f(sqrtf(1.0 - (f64) (arg0 * arg0)), arg0) * 32768.0f / M_PI;
}

u16 random_u16(void) {
    u16 temp1, temp2;

    if (gRandomSeed16 == 22026) {
        gRandomSeed16 = 0;
    }

    temp1 = (gRandomSeed16 & 0x00FF) << 8;
    temp1 = temp1 ^ gRandomSeed16;

    gRandomSeed16 = ((temp1 & 0x00FF) << 8) + ((temp1 & 0xFF00) >> 8);

    temp1 = ((temp1 & 0x00FF) << 1) ^ gRandomSeed16;
    temp2 = (temp1 >> 1) ^ 0xFF80;

    if ((temp1 & 1) == 0) {
        if (temp2 == 43605) {
            gRandomSeed16 = 0;
        } else {
            gRandomSeed16 = temp2 ^ 0x1FF4;
        }
    } else {
        gRandomSeed16 = temp2 ^ 0x8180;
    }

    return gRandomSeed16;
}

u16 random_int(u16 arg0) {
    return arg0 * (((f32) random_u16()) / 65535.0);
}

s16 func_802B7F34(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    return atan2s(arg2 - arg0, arg3 - arg1);
}

void func_802B7F7C(Vec3f arg0, Vec3f arg1, Vec3s dest) {
    f32 x1 = arg0[0];
    f32 y1 = arg0[1];
    f32 z1 = arg0[2];

    f32 x2 = arg1[0];
    f32 y2 = arg1[1];
    f32 z2 = arg1[2];

    dest[1] = func_802B7F34(z1, x1, z2, x2);
    dest[0] = func_802B7F34(y1, z1, y2, z2);
    dest[2] = func_802B7F34(x1, y1, x2, y2);
}

f32 sins(u16 arg0) {
    return gSineTable[arg0 >> 4];
}

f32 coss(u16 arg0) {
    return gCosineTable[arg0 >> 4];
}

s32 is_visible_between_angle(u16 arg0, u16 arg1, u16 arg2) {
    if (arg1 < arg0) {
        if (arg1 >= arg2) {
            return 0;
        }
        if (arg2 >= arg0) {
            return 0;
        }
    } else {
        if ((arg1 >= arg2) && (arg2 >= arg0)) {
            return 0;
        }
    }
    return 1;
}

/**
 * Determines whether an object is within the render distance of a camera.
 *
 * @param cameraPos       The position of the camera in 3D space.
 * @param objectPos       The position of the object in 3D space.
 * @param orientationY    The orientation angle of the object around the Y-axis.
 * @param minDistance     The minimum distance at which the object is considered within render distance.
 * @param fov             The field of view (FOV) of the camera.
 * @param maxDistance     The maximum render distance.
 * @return                The distance between the camera and the object if it's within render distance,
 *                        or -1.0f if it exceeds the render distance.
 */

f32 is_within_render_distance(Vec3f cameraPos, Vec3f objectPos, u16 orientationY, f32 minDistance, f32 fov,
                              f32 maxDistance) {
    u16 angleObject;
    u16 temp_v0;
    f32 distanceX;
    f32 distance;
    f32 distanceY;
    f32 scaleFov;
    f32 maxDistance2;
    s32 plus_fov_angle;
    s32 minus_fov_angle;
    u16 temp;
    s32 count = 0;

    maxDistance *= 6.5f;
    maxDistance2 = 1.0f;
    scaleFov = 1.25;

    f32 extended_fov = ((f32) fov * 0xB6 * scaleFov); // Sets the Culling for objects on the left and right

    distanceX = objectPos[0] - cameraPos[0];
    distanceX = distanceX * distanceX;
    if (maxDistance < distanceX) {
        return -1.0f;
    }

    distanceY = objectPos[2] - cameraPos[2];
    distanceY = distanceY * distanceY;
    if (maxDistance < distanceY) {
        return -1.0f;
    }

    distance = distanceX + distanceY;
    if (distance < minDistance) {
        return distance;
    }

    if (distance > maxDistance) {
        return -1.0f;
    }

    angleObject = get_angle_between_two_vectors(cameraPos, objectPos);
    minus_fov_angle = (orientationY - extended_fov);
    plus_fov_angle = (orientationY + extended_fov);

    if (minDistance == 0.0f) {
        if (is_visible_between_angle((orientationY + extended_fov), (orientationY - extended_fov), angleObject) == 1) {
            if (IsKalimariDesert()) {
                return distance / 6.5f; // set for better DD settings in Desert
            } else {
                return distance / 10.0f; // Items
            }
        }
        return -1.0f;
    }

    if (is_visible_between_angle((u16) plus_fov_angle, (u16) minus_fov_angle, angleObject) == 1) {
        if (IsKalimariDesert()) {
            return distance / 2.0f;
        } else {
            return distance / 10.0f; // DD Vhicles
        }
    }

    temp_v0 = func_802B7CA8(minDistance / distance);
    temp = angleObject + temp_v0;

    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return distance;
    }

    temp = angleObject - temp_v0;
    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return distance;
    }
    return -1.0f;
}

// No idea if arg1 is actually a Mat4 or not, but since this function is unused
// its impossible to know with certainty either way, very close of set_track_light_direction
UNUSED void func_802B8414(uintptr_t addr, Mat4 arg1, s16 arg2, s16 arg3, s32 arg4) {
    UNUSED s32 pad;
    Vec3f sp40;
    s8 sp3C[3];
    s32 var_v0;
    UNUSED s32 pad2[3];
    Lights1* var_s0;

    var_s0 = (Lights1*) addr;
    sins(arg3);
    coss(arg3);
    sins(arg2);
    coss(arg2);
    sp40[0] = 0.0f;
    sp40[1] = 0.0f;
    sp40[2] = 1.0f;
    sp3C[0] = ((sp40[0] * arg1[0][0]) + (sp40[1] * arg1[1][0]) + (sp40[2] * arg1[2][0])) * 127.0f;
    sp3C[1] = ((sp40[0] * arg1[0][1]) + (sp40[1] * arg1[1][1]) + (sp40[2] * arg1[2][1])) * 127.0f;
    sp3C[2] = ((sp40[0] * arg1[0][2]) + (sp40[1] * arg1[1][2]) + (sp40[2] * arg1[2][2])) * 127.0f;
    for (var_v0 = 0; var_v0 < arg4; var_v0++, var_s0++) {
        var_s0->l[0].l.dir[0] = sp3C[0];
        var_s0->l[0].l.dir[1] = sp3C[1];
        var_s0->l[0].l.dir[2] = sp3C[2];
    }
}

#ifdef MIPS_TO_C
// generated by m2c commit beb457dabfc7a01ec6540a5404a6a05097a13602 on Oct-29-2023
void func_802B8614(Player* arg0) {
    f64 sp78;
    f64 sp70;
    f64 sp68;
    f64 sp58;
    f64 sp50;
    f64 sp48;
    f64 sp40;
    f64 sp38;
    f64 temp_f12;
    f64 temp_f14;
    f64 temp_f16;
    f64 temp_f18;
    f64 temp_f20;
    f64 temp_f2;
    f64 temp_f30;
    f64 temp_f4;
    f64 temp_f6;

    temp_f6 = (f64) arg0->tyres[FRONT_RIGHT].pos[0];
    sp78 = temp_f6;
    sp70 = (f64) arg0->tyres[FRONT_RIGHT].baseHeight;
    sp68 = (f64) arg0->tyres[FRONT_RIGHT].pos[2];
    temp_f30 = (f64) arg0->tyres[FRONT_LEFT].pos[0];
    sp58 = (f64) arg0->tyres[FRONT_LEFT].baseHeight;
    sp50 = (f64) arg0->tyres[FRONT_LEFT].pos[2];
    sp48 = (f64) arg0->tyres[BACK_RIGHT].pos[0];
    sp40 = (f64) arg0->tyres[BACK_RIGHT].baseHeight;
    temp_f4 = (f64) arg0->tyres[BACK_RIGHT].pos[2];
    temp_f2 = sp58 - sp70;
    sp38 = temp_f4;
    temp_f12 = temp_f4 - sp50;
    temp_f14 = sp50 - sp68;
    temp_f16 = sp40 - sp58;
    temp_f18 = sp48 - temp_f30;
    temp_f20 = temp_f30 - temp_f6;
    if (0.0 == 0.0) {
        arg0->unk_058 = 0.0f;
        arg0->unk_060 = 0.0f;
        arg0->unk_05C = 1.0f;
        return;
    }
    arg0->unk_058 = (f32) ((f64) (f32) ((temp_f2 * temp_f12) - (temp_f14 * temp_f16)) / 0.0);
    arg0->unk_05C = (f32) ((f64) (f32) ((temp_f14 * temp_f18) - (temp_f20 * temp_f12)) / 0.0);
    arg0->unk_060 = (f32) ((f64) (f32) ((temp_f20 * temp_f16) - (temp_f2 * temp_f18)) / 0.0);
}
#else
GLOBAL_ASM("asm/non_matchings/racing/math_util/func_802B8614.s")
#endif
