#ifndef MATH_UTIL_2_H
#define MATH_UTIL_2_H

#include <libultraship.h>
#include <common_structs.h>
#include "camera.h"

extern Mat4* gInterpolationMatrix;

/* Function Prototypes */

// Unused functions
void operator_or(s32*, s32);
void operator_and_not(s32*, s32);
void operator_xor(s32*, s32);
bool func_80040E84(s32*, s32); // UNUSED
s32 func_80040EA4(s32*, s32);
s32 f32_step_up_towards_alternate(f32*, f32, f32*);
s32 f32_step_down_towards_alternate(f32*, f32, f32*);
s32 s32_step_up_towards_alternate(s32*, s32, s32*);
s32 s32_step_down_towards_alternate(s32*, s32, s32*);
s32 s16_step_up_towards_alternate(s16*, s16, s16*);
s32 s16_step_down_towards_alternate(s16*, s16, s16*);
s32 s32_step_towards(s32*, s32, s32);
s32 is_within_distance_2d(f32, f32, f32, f32, f32);
s32 func_80041680(f32, f32);
s32 func_800416AC(f32, f32);
void func_80041F54(s32, s32);
void func_80042000(u16);
void func_800420A8(f32);
void func_8004214C(u16, f32);
void func_800421FC(s32, s32, f32);
void func_800423F0(Mat4, u16, u16, u16);
void func_8004252C(Mat4, u16, u16);
void mtxf_mult_first_column(Mat4, f32);
void mtxf_mult_second_column(Mat4, f32);
void mtxf_mult_third_column(Mat4, f32);
void rsp_set_matrix_diff_translation_scale(Vec3f, Vec3f, f32);

s32 f32_step_up_towards(f32*, f32, f32);
s32 f32_step_down_towards(f32*, f32, f32);
s32 s32_step_up_towards(s32*, s32, s32);
s32 s32_step_down_towards(s32*, s32, s32);
s32 s16_step_up_towards(s16*, s16, s16);
s32 u16_step_up_towards(u16*, u16, u16);
s32 s16_step_down_towards(s16*, s16, s16);
s32 u16_step_down_towards(u16*, s32, s32);
s32 s16_step_towards(s16*, s16, s16);
s32 f32_step_towards(f32*, f32, f32);
s32 func_80041658(f32, f32);
f32 func_800416D8(f32 x, f32 z, u16 angle);
f32 func_80041724(f32, f32, u16);
s32 get_angle_between_xy(f32, f32, f32, f32);
u16 func_800417B4(u16, u16);
s32 func_800418AC(f32, f32, Vec3f);
s32 func_800418E8(f32, f32, Vec3f);
s32 func_80041924(Collision*, Vec3f);
bool is_particle_on_screen(Vec3f, Camera*, u16);
void func_800419F8(void);
void mtfx_translation_x_y(Mat4, s32, s32);
void mtxf_u16_rotate_z(Mat4, u16);
void mtxf_scale_x_y(Mat4, f32);
void mtxf_rotate_z_scale_x_y(Mat4, u16, f32);
void mtxf_translation_x_y_rotate_z_scale_x_y(Mat4, s32, s32, u16, f32);
void func_80041D24(void);
void func_80041D34(void);
void set_matrix_hud_screen(void);
void func_80042330(s32, s32, u16, f32);
void func_80042330_unchanged(s32 x, s32 y, u16 angle, f32 scale);
void func_80042330_portrait(s32, s32, u16, f32, s16);
void func_80042330_wide(s32, s32, u16, f32);
void mtxf_set_matrix_transformation(Mat4, Vec3f, Vec3su, f32);
void mtxf_set_matrix_scale_transl(Mat4, Vec3f, Vec3f, f32);
s32 mtxf_set_matrix_gObjectList(s32, Mat4);
void set_transform_matrix(Mat4 dest, Vec3f orientationVector, Vec3f positionVector, u16 rotationAngle,
                          f32 scaleFactor);
void vec3f_rotate_x_y(Vec3f, Vec3f, Vec3s);
void rsp_set_matrix_transformation(Vec3f, Vec3su, f32);
void rsp_set_matrix_transformation_inverted_x_y_orientation(Vec3f, Vec3su, f32);
void rsp_set_matrix_transl_rot_scale(Vec3f, Vec3f, f32);
void rsp_set_matrix_gObjectList(s32);

#endif // MATH_UTIL_2_H
