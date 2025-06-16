#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>
#include "math_util_2.h"
#include "main.h"
#include "math_util.h"
#include "objects.h"

#include "memory.h"
#include "collision.h"
#include "render_player.h"
#include "code_80057C60.h"
#include "defines.h"
#include "camera.h"

#include "port/Engine.h"
#include "engine/Matrix.h"
#include "port/interpolation/FrameInterpolation.h"

#pragma intrinsic(sqrtf)

Mat4 sInterpolationMatrixStack[0x1000];
Mat4* gInterpolationMatrix = &sInterpolationMatrixStack[0];

UNUSED void operator_or(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 | arg1);
}

UNUSED void operator_and_not(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 & ~arg1);
}

UNUSED void operator_xor(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 ^ arg1);
}

UNUSED bool func_80040E84(s32* arg0, s32 arg1) {
    bool phi_v1;

    phi_v1 = false;
    if ((*arg0 & arg1) != 0) {
        phi_v1 = true;
    }
    return phi_v1;
}

UNUSED s32 func_80040EA4(s32* arg0, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if ((*arg0 & arg1) == 0) {
        phi_v1 = 1;
    }
    return phi_v1;
}

void vec3f_copy(Vec3f dest, Vec3f arg1) {
    dest[0] = arg1[0];
    dest[1] = arg1[1];
    dest[2] = arg1[2];
}

s32 f32_step_up_towards(f32* value, f32 target, f32 step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value += step;
        if (target <= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 f32_step_down_towards(f32* value, f32 target, f32 step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value -= step;
        if (*value <= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 s32_step_up_towards(s32* value, s32 target, s32 step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value = *value + step;
        targetReached = 0;
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 s32_step_down_towards(s32* value, s32 target, s32 step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value = *value - step;
        ;
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 s16_step_up_towards(s16* value, s16 target, s16 step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value = *value + step;
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 u16_step_up_towards(u16* value, u16 target, u16 step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value += step;
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 s16_step_down_towards(s16* value, s16 target, s16 step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value -= step;
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

// wtf is up with the argument types for this one?
s32 u16_step_down_towards(u16* value, s32 target, s32 step) {
    s32 targetReached = 0;
    s32 temp = *value;

    if (target < temp) {
        temp -= step;
        if (target >= temp) {
            temp = target;
            targetReached = 1;
        }
        *value = temp;
    }
    return targetReached;
}

UNUSED s32 f32_step_up_towards_alternate(f32* value, f32 target, f32* step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value += *step;
        if (target <= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 f32_step_down_towards_alternate(f32* value, f32 target, f32* step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value -= *step;
        if (*value <= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 s32_step_up_towards_alternate(s32* value, s32 target, s32* step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value += *step;
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 s32_step_down_towards_alternate(s32* value, s32 target, s32* step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value -= *step;
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 s16_step_up_towards_alternate(s16* value, s16 target, s16* step) {
    s32 targetReached = 0;

    if (*value < target) {
        *value += *step;
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 s16_step_down_towards_alternate(s16* value, s16 target, s16* step) {
    s32 targetReached = 0;

    if (target < *value) {
        *value -= *step;
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 s16_step_towards(s16* value, s16 target, s16 step) {
    s32 targetReached = 0;

    if (*value < target) {
        if (step >= 0) {
            *value += step;
        } else {
            *value -= step;
        }
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }
    } else if (target < *value) {
        if (step >= 0) {
            *value -= step;
        } else {
            *value += step;
        }
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 s32_step_towards(s32* value, s32 target, s32 step) {
    s32 temp_v0;
    s32 targetReached;

    temp_v0 = *value;
    targetReached = 0;
    if (temp_v0 < target) {
        if (step >= 0) {
            *value = (s32) (temp_v0 + step);
        } else {
            *value = (s32) (temp_v0 - step);
        }
        if (*value >= target) {
            *value = target;
            targetReached = 1;
        }

    } else if (target < temp_v0) {
        if (step >= 0) {
            *value = (s32) (temp_v0 - step);
        } else {
            *value = (s32) (temp_v0 + step);
        }
        if (target >= *value) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 f32_step_towards(f32* value, f32 target, f32 step) {
    s32 targetReached = 0;

    if (*value < target) {
        if (step >= 0.0f) {
            *value += step;
        } else {
            *value -= step;
        }
        if (target <= *value) {
            *value = target;
            targetReached = 1;
        }
    } else if (target < *value) {
        if (step >= 0.0f) {
            *value -= step;
        } else {
            *value += step;
        }
        if (*value <= target) {
            *value = target;
            targetReached = 1;
        }
    }
    return targetReached;
}

Vec3f* vec3f_set_xyz(Vec3f arg0, f32 arg1, f32 arg2, f32 arg3) {
    arg0[0] = arg1;
    arg0[1] = arg2;
    arg0[2] = arg3;
    return (Vec3f*) &arg0;
}

Vec3f* vec3f_normalize(Vec3f dest) {
    f32 invsqrt = 1.0f / sqrtf(dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2]);

    dest[0] = dest[0] * invsqrt;
    dest[1] = dest[1] * invsqrt;
    dest[2] = dest[2] * invsqrt;
    return (Vec3f*) &dest;
}

Vec3f* vec3f_cross_product(Vec3f dest, Vec3f arg1, Vec3f arg2) {

    dest[0] = (arg1[1] * arg2[2]) - (arg2[1] * arg1[2]);
    dest[1] = (arg1[2] * arg2[0]) - (arg2[2] * arg1[0]);
    dest[2] = (arg1[0] * arg2[1]) - (arg2[0] * arg1[1]);

    return (Vec3f*) &dest;
}

UNUSED s32 is_within_distance_2d(f32 x1, f32 y1, f32 x2, f32 y2, f32 distance) {
    f32 x;
    f32 y;
    s32 ret = 0;

    x = x2 - x1;
    y = y2 - y1;
    if (((x * x) + (y * y)) <= (distance * distance)) {
        ret = 1;
    }
    return ret;
}

s32 func_80041658(f32 arg0, f32 arg1) {
    return -atan2s(arg0, arg1) & 0xFFFF;
}

UNUSED s32 func_80041680(f32 arg0, f32 arg1) {
    return atan2s(arg1, arg0);
}

UNUSED s32 func_800416AC(f32 arg0, f32 arg1) {
    return atan2s(arg1, arg0);
}

f32 func_800416D8(f32 x, f32 z, u16 angle) {
    f32 cosAngle;

    cosAngle = coss(angle);
    return (cosAngle * x) - (sins(angle) * z);
}

f32 func_80041724(f32 x, f32 z, u16 angle) {
    f32 sinAngle;

    sinAngle = sins(angle);
    return (coss(angle) * z) + (sinAngle * x);
}

s32 get_angle_between_xy(f32 x1, f32 x2, f32 y1, f32 y2) {
    return atan2s(x2 - x1, y2 - y1);
}

u16 func_800417B4(u16 angle1, u16 angle2) {
    u16 out_angle;

    if ((angle1 >> 8) != (angle2 >> 8)) {
        out_angle = angle2 - angle1;
        if (out_angle < 0x400) {
            out_angle = angle1 + 0x80;
        } else if (out_angle < 0x800) {
            out_angle = angle1 + 0x200;
        } else if (out_angle < 0x4000) {
            out_angle = angle1 + 0x400;
        } else if (out_angle < 0x8000) {
            out_angle = angle1 + 0x700;
        } else if (out_angle < 0xC000) {
            out_angle = angle1 - 0x700;
        } else if (out_angle < 0xF800) {
            out_angle = angle1 - 0x400;
        } else if (out_angle < 0xFC00) {
            out_angle = angle1 - 0x200;
        } else {
            out_angle = angle1 - 0x80;
        }
    } else {
        out_angle = angle2;
    }
    return out_angle;
}

s32 func_800418AC(f32 arg0, f32 arg1, Vec3f arg2) {
    return atan2s(arg0 - arg2[0], arg1 - arg2[2]);
}

s32 func_800418E8(f32 arg0, f32 arg1, Vec3f arg2) {
    return atan2s(arg0 - arg2[1], arg1 - arg2[2]);
}

s32 func_80041924(Collision* arg0, Vec3f arg1) {
    s32 ret = 0;

    check_bounding_collision(arg0, 10.0f, arg1[0], arg1[1], arg1[2]);
    if (arg0->unk34 == 1) {
        ret = 1;
    }
    return ret;
}

bool is_particle_on_screen(Vec3f arg0, Camera* arg1, u16 arg2) {
    u16 temp_t9;
    s32 ret;

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        return true;
    }

    ret = false;
    temp_t9 = (get_angle_between_xy(arg1->pos[0], arg0[0], arg1->pos[2], arg0[2]) + (arg2 / 2)) - arg1->rot[1];
    if ((temp_t9 >= 0) && (arg2 >= temp_t9)) {
        ret = true;
    }
    return ret;
}

void func_800419F8(void) {
    Vec3f pos;
    Vec3f vec;

    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 120.0f;
    vec3f_rotate_x_y(vec, pos, (s16*) D_80165834);
    D_80165840[0] = vec[0];
    D_80165840[1] = vec[1];
    D_80165840[2] = vec[2];
}

UNUSED void func_80041A70(void) {
}

void mtfx_translation_x_y(Mat4 arg0, s32 x, s32 y) {
    arg0[0][0] = 1.0f;
    arg0[1][1] = 1.0f;
    arg0[2][2] = 1.0f;
    arg0[1][0] = 0.0f;
    arg0[2][0] = 0.0f;
    arg0[0][1] = 0.0f;
    arg0[3][0] = x;
    arg0[2][1] = 0.0f;
    arg0[0][2] = 0.0f;
    arg0[1][2] = 0.0f;
    arg0[3][2] = 0.0f;
    arg0[3][1] = y;
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;

    /*
     * 1 0 0 x
     * 0 1 0 y
     * 0 0 1 0
     * 0 0 0 1
     */
}

void mtxf_u16_rotate_z(Mat4 dest, u16 angle) {
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle);

    dest[0][0] = cos_theta;
    dest[1][0] = -sin_theta;
    dest[1][1] = cos_theta;
    dest[0][1] = sin_theta;
    dest[2][0] = 0.0f;
    dest[3][0] = 0.0f;
    dest[2][1] = 0.0f;
    dest[3][1] = 0.0f;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
}

void mtxf_scale_x_y(Mat4 dest, f32 scale) {
    dest[1][0] = 0.0f;
    dest[2][0] = 0.0f;
    dest[3][0] = 0.0f;
    dest[0][1] = 0.0f;
    dest[2][1] = 0.0f;
    dest[3][1] = 0.0f;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
    dest[0][0] = scale;
    dest[1][1] = scale;
}

UNUSED void mtxf_rotate_z_scale_x_y(Mat4 dest, u16 angle, f32 scale) {
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle) * scale;

    dest[2][0] = 0.0f;
    dest[0][0] = cos_theta;
    dest[1][1] = cos_theta;
    dest[2][1] = 0.0f;
    dest[1][0] = -sin_theta * scale;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[0][1] = sin_theta * scale;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][0] = 1.0f;
    dest[3][1] = 1.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
}

/**
 * @brief arg1 and arg2 are s32's into floats?!?
 *
 * @param dest
 * @param x
 * @param y
 * @param angle
 * @param scale
 */
void mtxf_translation_x_y_rotate_z_scale_x_y(Mat4 dest, s32 x, s32 y, u16 angle, f32 scale) {
    f32 sin_theta = sins(angle);
    f32 cos_theta = coss(angle) * scale;
    FrameInterpolation_RecordMatrixPosRotScaleXY(dest, x, y, angle, scale);

    dest[2][0] = 0.0f;
    dest[0][0] = cos_theta;
    dest[1][0] = (-sin_theta) * scale;
    dest[3][0] = (f32) x;
    dest[1][1] = cos_theta;
    dest[0][1] = sin_theta * scale;
    dest[2][1] = 0.0f;
    dest[3][1] = (f32) y;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
}

// Likely D_801658**[index] = 1; * denotes wildcard
void func_80041D24(void) {
    D_801658FE = 1;
}

void guOrtho(Mtx*, f32, f32, f32, f32, f32, f32, f32); /* extern */

void func_80041D34(void) {
    guOrtho(&D_80183D60, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            guOrtho(&gGfxPool->mtxOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            guOrtho(&gGfxPool->mtxOrtho, 0.0f, 160.0f, 120.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            if (D_801658FE == 0) {
                guOrtho(&gGfxPool->mtxOrtho, 0.0f, 320.0f, 120.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            } else {
                guOrtho(&gGfxPool->mtxOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            guOrtho(&gGfxPool->mtxOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
    }
}

void set_matrix_hud_screen(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxOrtho),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
}

UNUSED void func_80041F54(s32 x, s32 y) {
    Mat4 matrix;

    mtfx_translation_x_y(matrix, x, y);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_80042000(u16 angle_z) {
    Mat4 matrix;

    mtxf_u16_rotate_z(matrix, angle_z);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_800420A8(f32 scale) {
    Mat4 matrix;

    mtxf_scale_x_y(matrix, scale);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_8004214C(u16 angle, f32 scale) {
    Mat4 matrix;

    mtxf_rotate_z_scale_x_y(matrix, angle, scale);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_800421FC(s32 x, s32 y, f32 scale) {
    Mat4 matrix;

    mtfx_translation_x_y(matrix, x, y);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    mtxf_scale_x_y(matrix, scale);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
}

void func_80042330(s32 x, s32 y, u16 angle, f32 scale) {
    Mat4 matrix;
    // printf("panel %d %d %d\n", x, (s32)OTRGetDimensionFromLeftEdge(x), (s32)OTRGetDimensionFromLeftEdge(0));

    if (gHUDModes != 2) {
        if (x < (SCREEN_WIDTH / 2)) {
            x = (s32) OTRGetDimensionFromLeftEdge(x);
        } else {
            x = (s32) OTRGetDimensionFromRightEdge(x);
        }
    }

    mtxf_translation_x_y_rotate_z_scale_x_y(matrix, x, y, angle, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void func_80042330_unchanged(s32 x, s32 y, u16 angle, f32 scale) {
    Mat4 matrix;
    // printf("panel %d %d %d\n", x, (s32)OTRGetDimensionFromLeftEdge(x), (s32)OTRGetDimensionFromLeftEdge(0));

    mtxf_translation_x_y_rotate_z_scale_x_y(matrix, x, y, angle, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

// Allows a different way of lining up the portraits at the end of race sequence
void func_80042330_portrait(s32 x, s32 y, u16 angle, f32 scale, s16 lapCount) {
    Mat4 matrix;
    // printf("panel %d %d %d\n", x, (s32)OTRGetDimensionFromLeftEdge(x), (s32)OTRGetDimensionFromLeftEdge(0));

    if ((gHUDModes != 2) && (D_801657E2 == 0) || (CVarGetInteger("gBetterResultPortraits", 0) == true)) {
        if (x < (SCREEN_WIDTH / 2)) {
            x = (s32) OTRGetDimensionFromLeftEdge(x);
        } else {
            x = (s32) OTRGetDimensionFromRightEdge(x);
        }
    }

    mtxf_translation_x_y_rotate_z_scale_x_y(matrix, x, y, angle, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void func_80042330_wide(s32 x, s32 y, u16 angle, f32 scale) {
    Mat4 matrix;

    if (x < (SCREEN_WIDTH / 2)) {
        x = (s32) OTRGetDimensionFromLeftEdge(x);
    } else {
        x = (s32) OTRGetDimensionFromRightEdge(x);
    }

    mtxf_translation_x_y_rotate_z_scale_x_y(matrix, x, y, angle, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_800423F0(Mat4 arg0, u16 arg1, u16 arg2, u16 arg3) {
    f32 sp3C;
    f32 temp_f20;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 temp_f0;

    sp3C = sins(arg1);
    temp_f20 = coss(arg1);
    sp34 = sins(arg2);
    sp30 = coss(arg2);
    sp2C = sins(arg3);
    temp_f0 = coss(arg3);

    arg0[0][0] = (f32) (sp30 * temp_f0 + (sp3C * sp34 * sp2C));
    arg0[1][0] = (f32) ((-sp30 * sp2C) + (sp3C * sp34 * temp_f0));
    arg0[2][0] = (f32) (temp_f20 * sp34);
    arg0[3][0] = 0.0f;
    arg0[0][1] = (f32) (temp_f20 * sp2C);
    arg0[1][1] = (f32) (temp_f20 * temp_f0);
    arg0[2][1] = (f32) -sp3C;
    arg0[3][1] = 0.0f;
    arg0[0][2] = (f32) ((-sp34 * temp_f0) + (sp3C * sp30 * sp2C));
    arg0[1][2] = (f32) ((sp34 * sp2C) + (sp3C * sp30 * temp_f0));
    arg0[2][2] = (f32) (temp_f20 * sp30);
    arg0[3][2] = 0.0f;
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

UNUSED void func_8004252C(Mat4 arg0, u16 arg1, u16 arg2) {
    f32 sp2C = sins(arg1);
    f32 sp28 = coss(arg1);
    f32 sin_theta_y = sins(arg2);
    f32 cos_theta_y = coss(arg2);

    arg0[1][0] = sp2C * sin_theta_y;
    arg0[2][0] = sp28 * sin_theta_y;
    arg0[0][1] = 0.0f;
    arg0[0][0] = cos_theta_y;
    arg0[2][1] = -sp2C;
    arg0[0][2] = -sin_theta_y;
    arg0[1][1] = sp28;
    arg0[1][2] = sp2C * cos_theta_y;
    arg0[2][2] = sp28 * cos_theta_y;
}

void mtxf_set_matrix_transformation(Mat4 transformMatrix, Vec3f location, Vec3su rotation, f32 scale) {

    FrameInterpolation_RecordSetMatrixTransformation(transformMatrix, location, rotation, scale);
    f32 sinX = sins(rotation[0]);
    f32 cosX = coss(rotation[0]);
    f32 sinY = sins(rotation[1]);
    f32 cosY = coss(rotation[1]);
    f32 sinZ = sins(rotation[2]);
    f32 cosZ = coss(rotation[2]);

    transformMatrix[0][0] = ((cosY * cosZ) + (sinX * sinY * sinZ)) * scale;
    transformMatrix[1][0] = ((-cosY * sinZ) + (sinX * sinY * cosZ)) * scale;
    transformMatrix[2][0] = (cosX * sinY) * scale;
    transformMatrix[3][0] = location[0];
    transformMatrix[0][1] = cosX * sinZ * scale;
    transformMatrix[1][1] = cosX * cosZ * scale;
    transformMatrix[2][1] = -sinX * scale;
    transformMatrix[3][1] = location[1];
    transformMatrix[0][2] = ((-sinY * cosZ) + (sinX * cosY * sinZ)) * scale;
    transformMatrix[1][2] = ((sinY * sinZ) + (sinX * cosY * cosZ)) * scale;
    transformMatrix[2][2] = cosX * cosY * scale;
    transformMatrix[3][2] = location[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;
}

void mtxf_set_matrix_scale_transl(Mat4 transformMatrix, Vec3f vec1, Vec3f vec2, f32 scale) {
    transformMatrix[0][0] = scale;
    transformMatrix[1][0] = 0.0f;
    transformMatrix[2][0] = 0.0f;
    transformMatrix[3][0] = vec1[0] - vec2[0];
    transformMatrix[0][1] = 0.0f;
    transformMatrix[1][1] = -scale;
    transformMatrix[2][1] = 0.0f;
    transformMatrix[3][1] = vec1[1] - vec2[1];
    transformMatrix[0][2] = 0.0f;
    transformMatrix[1][2] = 0.0f;
    transformMatrix[2][2] = -scale;
    transformMatrix[3][2] = vec1[2] - vec2[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;
}

/**
 * @brief Tried to put the definitions in the declares. However, sp3C wants to be at the top.
 * Something may be possible with some padding. Couldn't find a way though. So we get big mess.
 *
 * @param arg0
 * @param arg1
 **/

struct ObjectInterpData2 {
    s32 objectIndex;
    f32 x, y;
};
struct ObjectInterpData2 prevObject2[OBJECT_LIST_SIZE] = { 0 };

s32 mtxf_set_matrix_gObjectList(s32 objectIndex, Mat4 transformMatrix) {
    f32 sinX;
    Object* object = &gObjectList[objectIndex];
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;
    f32 cosX;

    sinX = sins(object->orientation[0]);
    cosX = coss(object->orientation[0]);
    sinY = sins(object->orientation[1]);
    cosY = coss(object->orientation[1]);
    sinZ = sins(object->orientation[2]);
    cosZ = coss(object->orientation[2]);

    transformMatrix[0][0] = object->sizeScaling * ((cosY * cosZ) + (sinX * sinY * sinZ));
    transformMatrix[1][0] = object->sizeScaling * ((-cosY * sinZ) + sinX * sinY * cosZ);
    transformMatrix[2][0] = object->sizeScaling * (cosX * sinY);
    transformMatrix[3][0] = object->pos[0];
    transformMatrix[0][1] = object->sizeScaling * (cosX * sinZ);
    transformMatrix[1][1] = object->sizeScaling * (cosX * cosZ);
    transformMatrix[2][1] = object->sizeScaling * -sinX;
    transformMatrix[3][1] = object->pos[1];
    transformMatrix[0][2] = object->sizeScaling * ((-sinY * cosZ) + (sinX * cosY * sinZ));
    transformMatrix[1][2] = object->sizeScaling * ((sinY * sinZ) + (sinX * cosY * cosZ));
    transformMatrix[2][2] = object->sizeScaling * (cosX * cosY);
    transformMatrix[3][2] = object->pos[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;

    // Search all recorded objects for the one we're drawing
    for (int i = 0; i < OBJECT_LIST_SIZE; i++) {
        if (objectIndex == prevObject2[i].objectIndex) {
            // Coincidence!
            // Skip drawing the object this frame if it warped to the other side of the screen
            if ((fabsf(object->pos[0] - prevObject2[i].x) > 20) || (fabsf(object->pos[1] - prevObject2[i].y) > 20)) {
                prevObject2[objectIndex].x = object->pos[0];
                prevObject2[objectIndex].y = object->pos[1];
                prevObject2[objectIndex].objectIndex = objectIndex;
                // printf("IDX: %d X: %f Y: %f Z: %f\n", objectIndex, object->pos[0], object->pos[1], object->pos[2]);
                return 1;
            }
        }
    }
    prevObject2[objectIndex].x = object->pos[0];
    prevObject2[objectIndex].y = object->pos[1];
    prevObject2[objectIndex].objectIndex = objectIndex;

    return 0;
}

UNUSED void mtxf_mult_first_column(Mat4 arg0, f32 arg1) {
    arg0[0][0] *= arg1;
    arg0[1][0] *= arg1;
    arg0[2][0] *= arg1;
}

UNUSED void mtxf_mult_second_column(Mat4 arg0, f32 arg1) {
    arg0[0][1] *= arg1;
    arg0[1][1] *= arg1;
    arg0[2][1] *= arg1;
}

UNUSED void mtxf_mult_third_column(Mat4 arg0, f32 arg1) {
    arg0[0][2] *= arg1;
    arg0[1][2] *= arg1;
    arg0[2][2] *= arg1;
}

void set_transform_matrix(Mat4 dest, Vec3f orientationVector, Vec3f positionVector, u16 rotationAngle,
                          f32 scaleFactor) {
    Vec3f sp44;
    Vec3f sp38;
    Vec3f sp2C;

    FrameInterpolation_RecordSetTransformMatrix(dest, orientationVector, positionVector, rotationAngle, scaleFactor);
    vec3f_set_xyz(sp44, sins(rotationAngle), 0.0f, coss(rotationAngle));
    vec3f_normalize(orientationVector);
    vec3f_cross_product(sp38, orientationVector, sp44);
    vec3f_normalize(sp38);
    vec3f_cross_product(sp2C, sp38, orientationVector);
    vec3f_normalize(sp2C);
    dest[0][0] = sp38[0] * scaleFactor;
    dest[0][1] = sp38[1] * scaleFactor;
    dest[0][2] = sp38[2] * scaleFactor;
    dest[3][0] = positionVector[0];
    dest[1][0] = orientationVector[0] * scaleFactor;
    dest[1][1] = orientationVector[1] * scaleFactor;
    dest[1][2] = orientationVector[2] * scaleFactor;
    dest[3][1] = positionVector[1];
    dest[2][0] = sp2C[0] * scaleFactor;
    dest[2][1] = sp2C[1] * scaleFactor;
    dest[2][2] = sp2C[2] * scaleFactor;
    dest[3][2] = positionVector[2];
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

// aplly to position a rotation and put in dest
UNUSED void vec3f_rotate(Vec3f dest, Vec3f pos, Vec3s rot) {
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 temp_f4;
    f32 sp64;
    f32 sp60;
    f32 temp_f8;
    f32 sp58;
    f32 sp54;
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sins(rot[0]);
    cosine1 = coss(rot[0]);
    sine2 = sins(rot[1]);
    cosine2 = coss(rot[1]);
    sine3 = sins(rot[2]);
    cosine3 = coss(rot[2]);
    // it's a matrix multiplication
    sp74 = pos[0] * ((cosine2 * cosine3) + ((sine1 * sine2) * sine3));
    temp_f4 = pos[1] * ((-cosine2 * sine3) + ((sine1 * sine2) * cosine3));
    temp_f8 = pos[2] * (cosine1 * sine2);
    sp70 = pos[0] * (cosine1 * sine3);
    sp64 = pos[1] * (cosine1 * cosine3);
    sp58 = pos[2] * -sine1;
    sp6C = pos[0] * ((-sine2 * cosine3) + ((sine1 * cosine2) * sine3));
    sp60 = pos[1] * ((sine2 * sine3) + ((sine1 * cosine2) * cosine3));
    sp54 = pos[2] * (cosine1 * cosine2);
    dest[0] = sp74 + temp_f4 + temp_f8;
    dest[1] = sp70 + sp64 + sp58;
    dest[2] = sp6C + sp60 + sp54;
}

// apply to position a rotation x y only and put in dest
void vec3f_rotate_x_y(Vec3f dest, Vec3f pos, Vec3s rot) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;

    sp2C = pos[0];
    sp28 = pos[1];
    sp24 = pos[2];
    sine1 = sins(rot[0]);
    cosine1 = coss(rot[0]);
    sine2 = sins(rot[1]);
    cosine2 = coss(rot[1]);
    dest[0] = (sp2C * cosine2) - (sp24 * sine2);
    dest[1] = (sp2C * sine1 * sine2) + (sp28 * cosine1) + (sp24 * sine1 * cosine2);
    dest[2] = ((sp2C * cosine1 * sine2) - (sp28 * sine1)) + (sp24 * cosine1 * cosine2);
}

/**
 * @brief set the matrix to a transformation matrix
 *
 * @param translate or position
 * @param orientation
 * @param scale
 */
void rsp_set_matrix_transformation(Vec3f translate, Vec3su orientation, f32 scale) {
    Mat4 matrix;

    mtxf_set_matrix_transformation(matrix, translate, orientation, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void rsp_set_matrix_diff_translation_scale(Vec3f pos1, Vec3f pos2, f32 scale) {
    Mat4 matrix;

    mtxf_set_matrix_scale_transl(matrix, pos1, pos2, scale);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void rsp_set_matrix_transformation_inverted_x_y_orientation(Vec3f arg0, Vec3su arg1, f32 arg2) {
    Mat4 matrix;
    Vec3su orientation;

    orientation[0] = arg1[0] + 0x8000; // change the sign
    orientation[1] = arg1[1] + 0x8000; // change the sign
    orientation[2] = arg1[2];
    mtxf_set_matrix_transformation(matrix, arg0, orientation, arg2);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void rsp_set_matrix_transl_rot_scale(Vec3f arg0, Vec3f arg1, f32 arg2) {
    Mat4 matrix;

    set_transform_matrix(matrix, arg1, arg0, 0, arg2);
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void rsp_set_matrix_gObjectList(s32 transformIndex) {
    Mat4 matrix;

    if (mtxf_set_matrix_gObjectList(transformIndex, matrix)) {
        return;
    }
    // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    AddHudMatrix(matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
