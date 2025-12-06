/**
 * @file ceremony_and_credits.c
 * Implements cinematic camera for ceremony and credits scenes.
 * Camera moves via splines.
 */

#include <libultraship.h>
#include <macros.h>
#include <segments.h>
#include <common_structs.h>
#include "math_util.h"
#include "ceremony_and_credits.h"
#include "code_800029B0.h"
#include "code_80280000.h"
#include "objects.h"
#include "main.h"
#include "camera.h"
#include "audio/external.h"
#include <sounds.h>
#include "podium_ceremony_actors.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "defines.h"

#include "port/Engine.h"
#include "port/Game.h"

f32 D_802856B0 = 98.0f;
f32 D_802856B4 = 12.0f;
f32 gOrderedSizeSlidingBorders = 52.0f;
static const f32 D_802856BC = 52.0f;
f32 gSizeSlidingBorders = 0.0f;
s32 D_802856C4 = 0;

s32 D_802856C8[3] = { 0 }; // padding?

s16 sCutsceneShot;
s16 gCutsceneShotTimer;
s32 D_802876D4;
s32 D_802876D8;
s32 D_802876DC; // fake/padding? Or D8 is array?

CinematicCamera D_802876E0;
struct struct_80283431 D_80287750[10];
struct struct_80283430 D_80287818[32];
struct struct_80283430 D_80287998[32];
f32 sCutsceneSplineSegmentProgress;
s16 sCutsceneSplineSegment;
s16 D_80287B1E;
s8 D_80287B20;

void vec3f_set_dupe(Vec3f dest, f32 arg1, f32 arg2, f32 arg3) {
    dest[0] = arg1;
    dest[1] = arg2;
    dest[2] = arg3;
}

void vec3s_set_dupe(Vec3s dest, s16 arg1, s16 arg2, s16 arg3) {
    dest[0] = arg1;
    dest[1] = arg2;
    dest[2] = arg3;
}

void vec3f_clear(Vec3f arg0) {
    arg0[0] = arg0[1] = arg0[2] = 0.0f;
}

void vec3s_clear(Vec3s arg0) {
    arg0[0] = arg0[1] = arg0[2] = 0;
}

void vec3f_copy_return_dupe(Vec3f dest, Vec3f src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

void vec3s_copy_dupe(Vec3s dest, Vec3s src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

void func_80282040(void) {
}

void func_80282048(void) {
}

UNUSED void rotate_y_vec3f(Vec3f dest, Vec3f src, s16 angle) {
    Vec3f sp2C;

    vec3f_copy_return_dupe(sp2C, src);
    dest[0] = (sp2C[2] * sins(angle)) + (sp2C[0] * coss(angle));
    dest[1] = sp2C[1];
    dest[2] = (sp2C[2] * coss(angle)) - (sp2C[0] * sins(angle));
}

UNUSED void rotate_x_vec3f(Vec3f dest, Vec3f src, s16 angle) {
    Vec3f sp2C;

    vec3f_copy_return_dupe(sp2C, src);
    dest[2] = (sp2C[2] * coss(angle)) - (sp2C[1] * sins(angle));
    dest[1] = (sp2C[2] * sins(angle)) + (sp2C[1] * coss(angle));
    dest[0] = sp2C[0];
}

/**
 * Target f32 at given rate.
 * Used for targeting playerTwo and playerThree.
 * Also targets the trophy.
 */
s32 f32_lerp(f32* dest, f32 src, f32 lerp) {
    if (lerp > 1.0f) {
        lerp = 1.0f;
    }

    *dest = *dest + ((src - *dest) * lerp);

    if (src == *dest) {
        return 0;
    }
    return 1;
}

UNUSED bool ease_out_transition(s16* var, s16 dest, s16 speed_factor) {
    s16 temp_v0 = *var;

    if (speed_factor == 0) {
        *var = dest;
    } else {
        temp_v0 -= dest;
        temp_v0 -= (temp_v0 / speed_factor);
        temp_v0 += dest;
        *var = temp_v0;
    }

    if (dest == *var) {
        return false;
    }
    return true;
}

// Calculates fade in/out
bool adjust_f32_value_transition(f32* var, f32 dest, f32 speed_factor) {
    f32 temp_f0 = dest - *var;

    if (speed_factor < 0.0f) {
        speed_factor = -1.0f * speed_factor;
    }

    if (temp_f0 > 0.0f) {
        temp_f0 -= speed_factor;
        if (temp_f0 > 0.0f) {
            *var = dest - temp_f0;
        } else {
            *var = dest;
        }
    } else {
        temp_f0 += speed_factor;
        if (temp_f0 < 0.0f) {
            *var = dest - temp_f0;
        } else {
            *var = dest;
        }
    }

    if (dest == *var) {
        return false;
    }
    return true;
}

bool adjust_s16_value_transition(s16* var, s16 goal, s16 speed_factor) {
    s16 temp_v0 = goal - *var;

    if (speed_factor < 0) {
        speed_factor = speed_factor * -1;
    }
    if (temp_v0 > 0) {
        temp_v0 -= speed_factor;
        if (temp_v0 >= 0) {
            *var = goal - temp_v0;
        } else {
            *var = goal;
        }
    } else {
        temp_v0 += speed_factor;
        if (temp_v0 <= 0) {
            *var = goal - temp_v0;
        } else {
            *var = goal;
        }
    }

    if (goal == *var) {
        return false;
    }
    return true;
}

void reset_spline(void) {
    sCutsceneSplineSegment = 0;
    sCutsceneSplineSegmentProgress = 0.0f;
    D_80287B1E = 0;
    D_80287B20 = 0;
}

void reset_spline_wrap(UNUSED CinematicCamera* arg0) {
    reset_spline();
}

void calculate_distance_angle_y_and_angle_y_to_xz(Vec3f vec1, Vec3f vec2, f32* distance, s16* angleYToXZ, s16* angleY) {
    f32 xdist;
    f32 ydist;
    f32 zdist;

    xdist = vec2[0] - vec1[0];
    ydist = vec2[1] - vec1[1];
    zdist = vec2[2] - vec1[2];
    *distance = sqrtf((xdist * xdist) + (ydist * ydist) + (zdist * zdist));
    *angleYToXZ = atan2s(ydist, sqrtf((xdist * xdist) + (zdist * zdist)));
    *angleY = atan2s(xdist, zdist);
}

void apply_distance_angle_y_and_angle_y_to_xz(Vec3f vec1, Vec3f vec2, f32 coef, s16 angleYToXZ, s16 angleY) {
    vec2[0] = vec1[0] + (coef * coss(angleYToXZ) * sins(angleY));
    vec2[1] = vec1[1] + (coef * sins(angleYToXZ));
    vec2[2] = vec1[2] + (coef * coss(angleYToXZ) * coss(angleY));
}

UNUSED void aborting_cinematic_function(Vec3f arg0, Vec3f arg1, Vec3f arg2, Vec3s arg3) {
    Vec3f sp3C;
    Vec3f sp30;

    // What's up with this? Why do we copy arg1 to an unused local variable?
    vec3f_copy_return_dupe(sp3C, arg1);
    sp30[2] = -((arg2[2] * coss(arg3[0])) - (arg2[1] * sins(arg3[0])));
    sp30[1] = (arg2[2] * sins(arg3[0])) + (arg2[1] * coss(arg3[0]));
    sp30[0] = arg2[0];
    arg0[0] = arg1[0] + (sp30[2] * sins(arg3[1])) + (sp30[0] * coss(arg3[1]));
    arg0[1] = arg1[1] + sp30[1];
    arg0[2] = arg1[2] + (sp30[2] * coss(arg3[1])) - (sp30[0] * sins(arg3[1]));
}

void evaluate_cubic_spline(f32 arg0, Vec3f point, f32* arg2, f32 controlPoints1[], f32 controlPoints2[],
                           f32 controlPoints3[], f32 controlPoints4[]) {
    f32 B[4];

    if (arg0 > 1.0f) {
        arg0 = 1.0f;
    }
    B[0] = (((1.0f - arg0) * (1.0f - arg0)) * (1.0f - arg0)) / 6.0f;
    B[1] = ((((arg0 * arg0) * arg0) / 2.0f) - (arg0 * arg0)) + 0.6666667f;
    B[2] = ((((((-arg0) * arg0) * arg0) / 2.0f) + ((arg0 * arg0) / 2.0f)) + (arg0 / 2.0f)) + 0.16666667f;
    B[3] = ((arg0 * arg0) * arg0) / 6.0f;

    point[0] =
        B[0] * controlPoints1[0] + B[1] * controlPoints2[0] + B[2] * controlPoints3[0] + B[3] * controlPoints4[0];
    point[1] =
        B[0] * controlPoints1[1] + B[1] * controlPoints2[1] + B[2] * controlPoints3[1] + B[3] * controlPoints4[1];
    point[2] =
        B[0] * controlPoints1[2] + B[1] * controlPoints2[2] + B[2] * controlPoints3[2] + B[3] * controlPoints4[2];
    *arg2 = B[0] * controlPoints1[3] + B[1] * controlPoints2[3] + B[2] * controlPoints3[3] + B[3] * controlPoints4[3];
}

/**
 * Computes the point that is `progress` percent of the way through segment `splineSegment` of `spline`,
 * and stores the result in `p`. `progress` and `splineSegment` are updated if `progress` becomes >= 1.0.
 *
 * When neither of the next two points' speeds == 0, the number of frames is between 1 and 255. Otherwise
 * it's infinite.
 *
 * To calculate the number of frames it will take to progress through a spline segment:
 * If the next two speeds are the same and nonzero, it's 1.0 / firstSpeed.
 *
 * s1 and s2 are short hand for first/secondSpeed. The progress at any frame n is defined by a recurrency relation:
 *      p(n+1) = (s2 - s1 + 1) * p(n) + s1
 * Which can be written as
 *      p(n) = (s2 * ((s2 - s1 + 1)^(n) - 1)) / (s2 - s1)
 *
 * Solving for the number of frames:
 *      n = log(((s2 - s1) / s1) + 1) / log(s2 - s1 + 1)
 *
 * @return 1 if the point has reached the end of the spline, when `progress` reaches 1.0 or greater, and
 * the 4th CutsceneSplinePoint in the current segment away from spline[splineSegment] has an index of -1.
 */
s32 move_point_along_spline(Vec3f point, f32* arg1, struct struct_80283430 spline[], s16* splineSegment,
                            f32* progress) {
    s32 finished = 0;
    Mat4 controlPoints;
    s32 i = 0;
    f32 u = *progress;
    f32 progressChange;
    f32 firstSpeed = 0;
    f32 secondSpeed = 0;
    s32 segment = *splineSegment;

    if (*splineSegment < 0) {
        u = 0;
    }

    if ((spline[segment].unk0 == -1) || (spline[segment + 1].unk0 == -1) || (spline[segment + 2].unk0 == -1)) {
        return 1;
    }

    for (i = 0; i < 4; i++) {
        controlPoints[i][0] = spline[segment + i].unk6[0];
        controlPoints[i][1] = spline[segment + i].unk6[1];
        controlPoints[i][2] = spline[segment + i].unk6[2];
        controlPoints[i][3] = spline[segment + i].unk4 * 256.0f;
    }

    evaluate_cubic_spline(u, point, arg1, controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);

    if (spline[*splineSegment + 1].unk2 != 0) {
        firstSpeed = 1.0f / spline[*splineSegment + 1].unk2;
    }

    if (spline[*splineSegment + 2].unk2 != 0) {
        secondSpeed = 1.0f / spline[*splineSegment + 2].unk2;
    }

#ifdef VERSION_EU
    if (gGamestate == CREDITS_SEQUENCE) {
        firstSpeed *= 1.14999997f;
        secondSpeed *= 1.14999997f;
    }
#endif

    progressChange = (((secondSpeed - firstSpeed)) * *progress + firstSpeed);
    if (1 <= (*progress += progressChange)) {
        (*splineSegment)++;
        if (spline[*splineSegment + 3].unk0 == -1) {
            *splineSegment = 0;
            finished = 1;
        }
        (*progress)--;
    }
    return finished;
}

void func_80282BE4(struct struct_80283430* arg0, s8 arg1, u8 arg2, s8 arg3, Vec3s arg4, s32 arg5) {
    arg0->unk0 = arg1;
    arg0->unk2 = arg2;
    arg0->unk4 = arg3;
    if (arg5) {
        arg0->unk6[0] = -arg4[0];
    } else {
        arg0->unk6[0] = arg4[0];
    }
    arg0->unk6[1] = arg4[1];
    arg0->unk6[2] = arg4[2];
}

void func_80282C40(struct struct_80283430* arg0, struct struct_80282C40* arg1, s32 arg2) {
    s32 i = 0;
    s32 j = 0;
    func_80282BE4(&arg0[j], arg1[j].unk0, arg1[j].unk3, arg1[j].unk4, arg1[j].unk6, arg2);

    j++;
    goto dummy_label_888430;
    while (true) {
        do {

        dummy_label_888430:;
            func_80282BE4(&arg0[j], arg1[i].unk0, arg1[i].unk3, arg1[i].unk4, arg1[i].unk6, arg2);
            j++;
            i++;

        } while (arg1[i].unk0 != -1);
        if (j + 3 <= 30) {
            func_80282BE4(&arg0[j], arg1->unk0, arg1[i].unk3, arg1[i].unk4, arg1[i].unk6, arg2);
            func_80282BE4(&arg0[j + 1], arg1->unk0, 0, arg1[i].unk4, arg1[i].unk6, arg2);
            func_80282BE4(&arg0[j + 2], arg1->unk0, 0, arg1[i].unk4, arg1[i].unk6, arg2);
            func_80282BE4(&arg0[j + 3], -1, 0, arg1[i].unk4, arg1[i].unk6, arg2);
            break;
        }
    }
}

s32 move_cinematic_camera_along_spline(CinematicCamera* camera, struct struct_80286A04* arg1,
                                       struct struct_80286A04* arg2, s32 arg3) {
    s32 res;

    cutscene_event(reset_spline_wrap, camera, 0, 0);
    func_80282C40(D_80287818, (struct struct_80282C40*) arg1, arg3);
    func_80282C40(D_80287998, (struct struct_80282C40*) arg2, arg3);

    if (0) {}; // debug stub?

    res = move_point_along_spline(camera->lookAt, &camera->unk18, D_80287818, &sCutsceneSplineSegment,
                                  &sCutsceneSplineSegmentProgress) |
          move_point_along_spline(camera->pos, &camera->unk18, D_80287998, &sCutsceneSplineSegment,
                                  &sCutsceneSplineSegmentProgress);
    return res;
}

void func_80282E58(CinematicCamera* camera, struct struct_80282C40* arg1, s32 arg2) {
    func_80282C40(D_80287818, arg1, arg2);
    move_point_along_spline(camera->lookAt, &camera->unk18, D_80287818, &sCutsceneSplineSegment,
                            &sCutsceneSplineSegmentProgress);
}

void func_80282EAC(s32 arg0, CinematicCamera* arg1, s16 arg2, s16 arg3, s16 arg4) {
    if ((arg0 >= 0) && (arg0 < 3)) {
        if (arg1->unk48[arg0] < arg2) {
            arg1->unk48[arg0] = arg2;
            arg1->unk5A[arg0] = arg3;
            arg1->unk54[arg0] = arg4;
        }
    }
}

void func_80282F00(s16* arg0, s16 arg1) {
    if (arg1 == -0x8000) {
        *arg0 = (*arg0 & 0x8000) + 0xC000;
        return;
    }
    *arg0 += arg1;
}

void func_80282F44(s32 arg0, CinematicCamera* arg1, Camera* camera) {
    f32 distance;
    UNUSED s32 pad[2];
    s16 angleCam[2];
    Vec3f pos;
    Vec3f lookat;

    vec3f_set_dupe(pos, camera->pos[0], camera->pos[1], camera->pos[2]);
    vec3f_set_dupe(lookat, camera->lookAt[0], camera->lookAt[1], camera->lookAt[2]);
    if ((arg0 == 0) || (arg0 == 1)) {
        if ((arg1->unk48[0] != 0) || (arg1->unk48[1] != 0)) {
            calculate_distance_angle_y_and_angle_y_to_xz(pos, lookat, &distance, &angleCam[0], &angleCam[1]);
            angleCam[arg0] += (((f32) arg1->unk48[arg0]) * sins(arg1->unk4E[arg0]));
            if ((angleCam[0] < 0x3800) && (angleCam[0] >= -0x37FF)) {
                apply_distance_angle_y_and_angle_y_to_xz(pos, lookat, distance, angleCam[0], angleCam[1]);
            }
            func_80282F00(&arg1->unk4E[arg0], arg1->unk54[arg0]);
            if (adjust_s16_value_transition(&arg1->unk48[arg0], 0, arg1->unk5A[arg0]) == false) {
                arg1->unk4E[arg0] = 0;
            }
        }
        camera->lookAt[0] = lookat[0];
        camera->lookAt[1] = lookat[1];
        camera->lookAt[2] = lookat[2];
    }
}

void func_802830B4(CinematicCamera* arg0, s16 arg1, s16 arg2, s16 arg3) {
    if (arg0->unk60 < arg1) {
        arg0->unk60 = arg1;
        arg0->unk6C = arg2;
        arg0->unk68 = arg3;
    }
}

void func_80283100(CinematicCamera* arg0, f32* arg1) {
    if (arg0->unk60 != 0) {
        arg0->unk6E = (coss((u16) arg0->unk64) * arg0->unk60) / 256;
        arg0->unk64 += arg0->unk68;
        adjust_s16_value_transition(&arg0->unk60, 0, arg0->unk6C);
    } else {
        arg0->unk64 = 0.0f;
    }
    *arg1 = arg0->unk20 + (f32) arg0->unk6E;
}

void func_80283240(s16 arg0) {
    if (arg0 == 1) {
        func_80282EAC(0, &D_802876E0, 0x100, 0x10, 0x4000);
        func_80282EAC(1, &D_802876E0, 0x80, 0x20, 0x2000);
        func_802830B4(&D_802876E0, 0x300, 0x20, 0x4000);
    }
}

/**
 * Call the event while `start` <= gCutsceneShotTimer <= `end`
 * If `end` is -1, call for the rest of the shot.
 */
s32 cutscene_event(CameraEvent event, CinematicCamera* camera, s16 start, s16 end) {
    if (gCutsceneShotTimer >= start) {
        if ((end == -1) || (end >= gCutsceneShotTimer)) {
            event(camera);
        }
    }
    return 0;
}

s32 func_80283330(s32 arg0) {
    if (arg0 != D_802876D8) {
        D_802876D8 = arg0;
        sCutsceneShot = 0;
        gCutsceneShotTimer = 0;
        D_802876D4 = 0;
    }
    return D_802876D8;
}

extern s32 D_802876D8;

//! @todo Does this match as a return of s8?
s32 func_8028336C(UNUSED CinematicCamera* arg0, UNUSED Camera* camera) {
    u8 sp20[] = { 2, 3, 4, 5, 5, 5, 5, 5 };
    if (D_802876D8 != 0) {
        return D_802876D8;
    }
    switch (gGamestate) {
        case ENDING:
            D_802876D8 = sp20[D_802874D8.unk1D];
            break;

        case CREDITS_SEQUENCE:
            D_802876D8 = 6;
            break;
    }

    if (gGamestate == CREDITS_SEQUENCE) {
        func_80283330(6);
    }
    return D_802876D8;
}

s32 cinematic_stub(void) {
    return 0;
}

void init_cinematic_camera(void) {
    s32 i;
    CinematicCamera* camera = &D_802876E0;

    D_802876D8 = 0;
    camera->cutscene = 0;
    D_802856C4 = (s32) D_800DC5E4;
    vec3f_clear(camera->lookAt);
    vec3f_set_dupe(camera->pos, 0.0f, 0.0f, 500.0f);
    vec3f_clear(camera->unk30);
    vec3f_set_dupe(camera->unk24, 0.0f, 0.0f, 500.0f);
    vec3f_set_dupe(camera->unk3C, 0.0f, 1.0f, 0.0f);
    camera->unk18 = 0.0f;
    vec3s_clear(camera->unk48);
    vec3s_clear(camera->unk4E);
    vec3s_clear(camera->unk54);
    vec3s_clear(camera->unk5A);
    camera->unk60 = 0;
    camera->unk64 = 0.0f;
    camera->unk68 = 0.0f;
    camera->unk6C = 0;
    camera->unk6E = 0;
    camera->unk20 = gCameraFOV[0];
    sCutsceneShot = 0;
    gCutsceneShotTimer = 0;
    D_802876D4 = 0;
    reset_spline();

    for (i = 0; i < 32; i++) {
        D_80287818[i].unk0 = -1;
        D_80287998[i].unk0 = -1;
    }

    for (i = 0; i < 10; i++) {
        vec3f_clear(D_80287750[i].unk0);
        vec3s_clear(D_80287750[i].unkC);
    }

    gSizeSlidingBorders = 0.0f;

    if (gGamestate == ENDING) {
        D_802856B0 = 120.0f;
        D_802856B4 = 12.0f;
        gOrderedSizeSlidingBorders = 120.0f;
    } else {
        D_802856B0 = 98.0f;
        D_802856B4 = 12.0f;
        gOrderedSizeSlidingBorders = 52.0f;
    }
}

// weird link between camera and cinematic camera where pos and lookAt are swapped
s32 func_80283648(Camera* camera) {
    s16 angleYToXZ;
    s16 angleY;
    f32 var_f2;
    f32 distance;
    Vec3f pos;
    Vec3f lookAt;
    Vec3f up;
    CinematicCamera* cinematicCamera = &D_802876E0;

    cinematic_stub();
    vec3f_copy_return_dupe(pos, camera->pos);
    vec3f_copy_return_dupe(lookAt, camera->lookAt);
    vec3f_copy_return_dupe(up, camera->up);
    cinematicCamera->cutscene = func_8028336C(cinematicCamera, camera);
    if (cinematicCamera->cutscene != 0) {
        vec3f_copy_return_dupe(cinematicCamera->lookAt, camera->pos);
        vec3f_copy_return_dupe(cinematicCamera->pos, camera->lookAt);
        play_cutscene(cinematicCamera);
        calculate_distance_angle_y_and_angle_y_to_xz(cinematicCamera->lookAt, cinematicCamera->pos, &distance,
                                                     &angleYToXZ, &angleY);
        if (angleYToXZ >= 0x3800) {
            angleYToXZ = 0x3800;
        }
        if (angleYToXZ < -0x37FF) {
            angleYToXZ = -0x3800;
        }
        if ((angleYToXZ == 0x3800) || (angleYToXZ == -0x3800)) {
            apply_distance_angle_y_and_angle_y_to_xz(cinematicCamera->lookAt, cinematicCamera->pos, distance,
                                                     angleYToXZ, angleY);
        }
        if (cinematicCamera->unk18 > 65536.0f) {
            cinematicCamera->unk18 -= 65536.0f;
        }
        if (cinematicCamera->unk18 < -65536.0f) {
            cinematicCamera->unk18 += 65536.0f;
        }
        // Huh?
        if (1) {
            var_f2 = cinematicCamera->unk18;
        }
        if (var_f2 < 0.0f) {
            var_f2 = 65536.0f + var_f2;
        }
        camera->up[0] = sins(var_f2) * coss(angleY);
        camera->up[1] = coss(var_f2);
        camera->up[2] = -sins(var_f2) * sins(angleY);
        vec3f_copy_return_dupe(camera->pos, cinematicCamera->lookAt);
        vec3f_copy_return_dupe(camera->lookAt, cinematicCamera->pos);
        if ((gGamestate == CREDITS_SEQUENCE) && (gIsMirrorMode != 0)) {
            camera->pos[0] = -camera->pos[0];
            camera->lookAt[0] = -camera->lookAt[0];
        }
    }
    func_80282F44(0, cinematicCamera, camera);
    func_80282F44(1, cinematicCamera, camera);
    func_80283100(cinematicCamera, gCameraFOV);
    vec3f_copy_return_dupe(cinematicCamera->unk30, camera->pos);
    vec3f_copy_return_dupe(cinematicCamera->unk24, camera->lookAt);
    vec3f_copy_return_dupe(cinematicCamera->unk3C, camera->up);
    return D_802876D8;
}

void wrap_func_8028100C(UNUSED CinematicCamera* camera) {
    func_8028100C(-0xC6C, 0xD2, -0x1EF);
}

void wrap_func_80280FFC(UNUSED CinematicCamera* camera) {
    func_80280FFC();
}

void animation_apears_sliding_borders(UNUSED CinematicCamera* camera) {
    gOrderedSizeSlidingBorders = 52.0f;
}

void animation_disapears_sliding_borders(UNUSED CinematicCamera* camera) {
    gOrderedSizeSlidingBorders = 0.0f;
}

void wrap_func_80092C80(UNUSED CinematicCamera* camera) {
    func_80092C80();
}

/**
 * Played at beginning of credits.
 */
void play_sound_welcome(UNUSED CinematicCamera* camera) {
    if (D_800DC5E4 == 0) {
        play_sound2(SOUND_INTRO_WELCOME);
    }
}

void wrap_func_800CA0CC(UNUSED CinematicCamera* camera) {
    func_800CA0CC();
}

/**
 * Played after receiving trophy.
 */
void play_sound_congratulation(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_CONGRATULATION);
}

/**
 * Played in ceremony opening with balloons.
 */
void play_sound_balloon_pop(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_BALLOON_POP);
}

void play_sound_fish(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_FISH);
}

void play_sound_fish_2(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_FISH_2);
}

void play_sound_shoot_trophy(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_SHOOT_TROPHY);
}

void play_sound_podium(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_PODIUM);
}

/**
 * Played in background nearly the entire ceremony
 * Begins again or plays louder once the trophy appears.
 */
void play_sound_trophy(UNUSED CinematicCamera* camera) {
    play_sound2(SOUND_CEREMONY_TROPHY);
}

void func_80283B6C(UNUSED CinematicCamera* camera) {
    func_800CA0B8();
    func_800C9060(0, SOUND_ACTION_EXPLOSION);
    func_800CA0A0();
}

void func_80283BA4(UNUSED CinematicCamera* camera) {
    func_800CA0B8();
    func_800C90F4(0, (gPlayerFour->characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x04));
    func_800CA0A0();
}

void func_80283BF0(UNUSED CinematicCamera* camera) {
    play_sequence2(MUSIC_SEQ_AWARD_CEREMONY_BUILDUP);
}

//
void func_80283C14(UNUSED CinematicCamera* camera) {
    play_sequence2(MUSIC_SEQ_AWARD_CEREMONY_1ST_3RD);
}

void wrap_func_800CB134(UNUSED CinematicCamera* camera) {
    func_800CB134();
}

void wrap_func_800CB14C(UNUSED CinematicCamera* camera) {
    func_800CB14C();
}

void func_80283C78(UNUSED CinematicCamera* arg0) {
    if (D_800DC5E4 == 0) {
        play_sequence2(MUSIC_SEQ_STAFF_ROLL);
    }
}

void func_80283CA8(UNUSED CinematicCamera* camera) {
    func_800CA008(0, 3);
}

void func_80283CD0(UNUSED CinematicCamera* camera) {
    if (D_800DC5E4 == 0) {
        func_800CA008(0, 2);
    }
}

/**
 * End of credits farewell.
 * "Hey, you're very good. See you next time!"
 */
void play_sound_farewell(UNUSED CinematicCamera* arg0) {
    play_sound2(SOUND_CREDITS_FAREWELL);
}

//! Camera rail spline animation
struct struct_80282C40 D_802856DC[] = {
    // go to
    { 0, 0, 0, 0, 0, 0, { 0xF576, 0x014E, 0xFE70 } },  { 0, 0, 0, 0, 0, 0, { 0xF576, 0x014E, 0xFE70 } },
    { 0, 0, 0, 0, 0, 0, { 0xF422, 0x0103, 0xFE3C } },  { 0, 0, 0, 0, 0, 0, { 0xF3E8, 0x0016, 0xFE34 } },
    { -1, 0, 0, 0, 0, 0, { 0xF3E8, 0x0016, 0xFE34 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285718[] = {
    { 0, 0, 0, 61, 0, 0, { 0xF493, 0x0309, 0xFE4E } },  { 0, 0, 0, 149, 0, 0, { 0xF494, 0x030A, 0xFE4E } },
    { 0, 0, 0, 94, 0, 0, { 0xF243, 0x0179, 0xFDF0 } },  { 0, 0, 0, 60, 0, 0, { 0xF213, 0x00B1, 0xFDE9 } },
    { -1, 0, 0, 60, 0, 0, { 0xF213, 0x008D, 0xFDE9 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285754[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF4F4, 0xFFE2, 0xFF67 } },
    { 0, 0, 0, 0, 0, 0, { 0xF51A, 0x0002, 0xFF5D } },
    { 0, 0, 0, 0, 0, 0, { 0xF57B, 0x004A, 0xFFB8 } },
    { -1, 0, 0, 0, 0, 0, { 0xF608, 0x0065, 0xFFE4 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285784[] = {
    { 0, 0, 0, 186, 0, 0, { 0xF33E, 0x0009, 0x0053 } },
    { 0, 0, 0, 89, 0, 0, { 0xF329, 0xFFA4, 0xFF8F } },
    { 0, 0, 0, 60, 0, 0, { 0xF39C, 0x004F, 0xFF2A } },
    { -1, 0, 0, 45, 0, 0, { 0xF44A, 0x00BE, 0xFF16 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802857B4[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF22C, 0xFFF4, 0x0067 } },
    { -1, 0, 0, 0, 0, 0, { 0xF22C, 0xFFF4, 0x0067 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802857CC[] = {
    { 0, 0, 0, 90, 0, 0, { 0xF3F8, 0xFFDB, 0xFC39 } },
    { 0, 0, 0, 90, 0, 0, { 0xF419, 0xFFF8, 0xFC3B } },
    { -1, 0, 0, 101, 0, 0, { 0xF454, 0x000E, 0xFBFF } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802857F0[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF208, 0x004F, 0xFDC9 } }, { 0, 0, 0, 0, 0, 0, { 0xF20F, 0x004E, 0xFDCB } },
    { 0, 0, 0, 0, 0, 0, { 0xF23D, 0x0049, 0xFDDC } }, { 0, 0, 0, 0, 0, 0, { 0xF284, 0x0044, 0xFDEF } },
    { 0, 0, 0, 0, 0, 0, { 0xF2BE, 0x0041, 0xFDFA } }, { 0, 0, 0, 0, 0, 0, { 0xF2E5, 0x003E, 0xFE04 } },
    { 0, 0, 0, 0, 0, 0, { 0xF303, 0x0039, 0xFE0A } }, { -1, 0, 0, 0, 0, 0, { 0xF325, 0x0033, 0xFE11 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285850[] = {
    { 0, 0, 0, 30, 0, 0, { 0xF3D7, 0x0004, 0xFE77 } }, { 0, 0, 0, 30, 0, 0, { 0xF3E3, 0x000C, 0xFE6F } },
    { 0, 0, 0, 50, 0, 0, { 0xF421, 0x001E, 0xFE53 } }, { 0, 0, 0, 50, 0, 0, { 0xF468, 0x001C, 0xFE69 } },
    { 0, 0, 0, 30, 0, 0, { 0xF4A4, 0x0016, 0xFE68 } }, { 0, 0, 0, 30, 0, 0, { 0xF4C9, 0xFFFE, 0xFE70 } },
    { 0, 0, 0, 30, 0, 0, { 0xF4E3, 0xFFE1, 0xFE76 } }, { -1, 0, 0, 30, 0, 0, { 0xF505, 0xFFD1, 0xFE72 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802858B0[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF42B, 0x002D, 0xFE46 } },
    { -1, 0, 0, 0, 0, 0, { 0xF42B, 0x002D, 0xFE46 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802858C8[] = {
    { 0, 0, 0, 30, 0, 0, { 0xF246, 0x0073, 0xFDE7 } },
    { -1, 0, 0, 30, 0, 0, { 0xF246, 0x0073, 0xFDE7 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802858E0[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF39F, 0x003C, 0xFE2F } },
    { -1, 0, 0, 0, 0, 0, { 0xF39F, 0x003C, 0xFE2F } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_802858F8[] = {
    { 0, 0, 0, 1, 0, 0, { 0xF245, 0x0187, 0xFDA1 } },
    { -1, 0, 0, 1, 0, 0, { 0xF245, 0x0187, 0xFDA1 } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285910[] = {
    { 0, 0, 0, 0, 0, 0, { 0xF4A0, 0x00B7, 0xFF6C } },
    { -1, 0, 0, 0, 0, 0, { 0xF4A0, 0x00B7, 0xFF6C } },
};

/**
 * @brief spline animation
 *
 */
struct struct_80282C40 D_80285928[] = {
    { 0, 0, 0, 6, 0, 0, { 0xF340, 0x0025, 0xFE28 } },
    { -1, 0, 0, 6, 0, 0, { 0xF340, 0x0025, 0xFE28 } },
};

// Is some of this unused?
struct struct_80282C40 D_80285940[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C3, 0x006B, 0xFE1A } },
    { 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF203, 0xFF99, 0xFE62 } },
    { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, { 0xF200, 0xFFA4, 0xFE6D } },
    { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, { 0xF1DA, 0x01AF, 0xFE7F } },
    { 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF1DA, 0x014B, 0xFE7F } },
    { 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF1DA, 0x00E7, 0xFE7F } },
    { 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, { 0xF1E1, 0x0015, 0xFE7D } },
    { 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF1E1, 0x000A, 0xFE71 } },
    { 0xFF, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF1E0, 0x000C, 0xFE71 } },
};

void func_80283D2C(CinematicCamera* camera) {
    gOrderedSizeSlidingBorders = 120.0f;
    cutscene_event(func_80283CA8, camera, 0, 0);
    cutscene_event(wrap_func_800CA0CC, camera, 1, 1);
    cutscene_event(func_80283BF0, camera, 0, 0);
    cutscene_event(play_sound_balloon_pop, camera, 45, 45);
    cutscene_event(play_sound_balloon_pop, camera, 65, 65);
    cutscene_event(play_sound_balloon_pop, camera, 70, 70);
    cutscene_event(play_sound_balloon_pop, camera, 94, 94);
    cutscene_event(play_sound_balloon_pop, camera, 110, 110);
    cutscene_event(play_sound_balloon_pop, camera, 130, 130);
    cutscene_event(play_sound_balloon_pop, camera, 152, 152);
    cutscene_event(play_sound_balloon_pop, camera, 160, 160);
    cutscene_event(wrap_func_80280FFC, camera, gCutScene[0].duration - 60, gCutScene[0].duration - 60);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_802856DC,
                                       (struct struct_80286A04*) D_80285718, 0);
}

void func_80283EA0(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285754,
                                       (struct struct_80286A04*) D_80285784, 0);
}

void copy_player_two_in_camera(CinematicCamera* camera) {
    vec3f_copy_return_dupe(camera->pos, gPlayerTwo->pos);
}

void lerp_player_two_in_camera(CinematicCamera* camera) {
    f32_lerp(&camera->pos[0], gPlayerTwo->pos[0], 0.12f);
    f32_lerp(&camera->pos[1], gPlayerTwo->pos[1], 0.12f);
    f32_lerp(&camera->pos[2], gPlayerTwo->pos[2], 0.12f);
}

void func_80283F6C(CinematicCamera* camera) {
    cutscene_event(copy_player_two_in_camera, camera, 0, 0);
    cutscene_event(lerp_player_two_in_camera, camera, 0, -1);
    func_80282E58(camera, (struct struct_80282C40*) D_802857B4, 0);
}

void copy_player_three_in_camera(CinematicCamera* camera) {
    vec3f_copy_return_dupe(camera->pos, gPlayerThree->pos);
}

void lerp_player_three_in_camera(CinematicCamera* camera) {
    f32_lerp(&camera->pos[0], gPlayerThree->pos[0], 0.12f);
    f32_lerp(&camera->pos[1], gPlayerThree->pos[1], 0.12f);
    f32_lerp(&camera->pos[2], gPlayerThree->pos[2], 0.12f);
}

void func_80284068(CinematicCamera* camera) {
    cutscene_event(copy_player_three_in_camera, camera, 0, 0);
    cutscene_event(lerp_player_three_in_camera, camera, 0, -1);
    func_80282E58(camera, (struct struct_80282C40*) D_802857CC, 0);
}

void func_802840C8(CinematicCamera* camera) {
    cutscene_event(func_80283C14, camera, 5, 5);

    switch (D_802876D8) {
        case 2:
            func_80283EA0(camera);
            break;
        case 3:
            func_80283F6C(camera);
            break;
        case 4:
            func_80284068(camera);
            break;
    }
}

void func_80284154(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285910,
                                       (struct struct_80286A04*) D_80285928, 0);
}

void func_80284184(CinematicCamera* camera) {
    f32 trophy;

    if (gTrophyIndex != NULL) {
        trophy = ((gObjectList[gTrophyIndex].pos[1] - camera->lookAt[1]) * 0.9f) + camera->lookAt[1];
        f32_lerp(&camera->pos[1], trophy, 0.5);
    }
}

void func_802841E8(CinematicCamera* camera) {
    func_80282E58(camera, (struct struct_80282C40*) D_80285940, 0);
    vec3f_set_dupe(camera->pos, -3202.0f, 90.0f, -478.0f);
}

void func_8028422C(CinematicCamera* camera) {
    cutscene_event(play_sound_shoot_trophy, camera, 6, 6);
    cutscene_event(play_sound_trophy, camera, 30, 30);
    cutscene_event(func_802841E8, camera, 0, 0);
    cutscene_event(func_80284184, camera, 6, -1);
}

void func_802842A8(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_802858B0,
                                       (struct struct_80286A04*) D_802858C8, 0);
}

void func_802842D8(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_802857F0,
                                       (struct struct_80286A04*) D_80285850, 0);
}

void func_80284308(CinematicCamera* camera) {
    Player** sp30[4] = { &gPlayerOne, &gPlayerTwo, &gPlayerThree, &gPlayerFour };
    Player* ply;
    f32 x;
    f32 y;
    f32 z;

    cutscene_event(play_sound_congratulation, camera, 140, 140);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_802858E0,
                                       (struct struct_80286A04*) D_802858F8, 0);

    ply = *(sp30[0] + D_802874D8.unk1D);

    x = ply->pos[0] - gPlayerOne->pos[0];
    y = ply->pos[1] - gPlayerOne->pos[1];
    z = ply->pos[2] - gPlayerOne->pos[2];

    camera->lookAt[0] += x;
    camera->lookAt[2] += z;
    camera->pos[0] += x;
    camera->pos[2] += z;
    camera->lookAt[1] += y;
    camera->pos[1] += y;
}

struct struct_80282C40 D_80285A10[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF33D, 0x002F, 0xFE5A } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF36B, 0x0028, 0xFE76 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3A6, 0x0027, 0xFE6F } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3C8, 0x002A, 0xFE4D } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3CF, 0x002F, 0xFE33 } },
};

struct struct_80282C40 D_80285A4C[] = {
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF4AC, 0xFFC1, 0xFD1A } },
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF3D1, 0xFFF2, 0xFC8F } },
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF2BA, 0xFFF2, 0xFCBA } },
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF219, 0xFFF7, 0xFD56 } },
    { 0xFF, 0x00, 0x00, 45, 0x00, 0x00, { 0xF1E8, 0xFFED, 0xFDD9 } },
};

struct struct_80282C40 D_80285A88[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3D8, 0x0012, 0xFE0E } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3D8, 0x0012, 0xFE0E } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3D6, 0x001A, 0xFE0F } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3D7, 0x001D, 0xFE0F } },
};

struct struct_80282C40 D_80285AB8[] = {
    { 0x00, 0x00, 0x00, 75, 0x00, 0x00, { 0xF1FB, 0x006C, 0xFE85 } },
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF1FB, 0x006C, 0xFE85 } },
    { 0x00, 0x00, 0x00, 45, 0x00, 0x00, { 0xF225, 0x00FB, 0xFE7A } },
    { 0xFF, 0x00, 0x00, 45, 0x00, 0x00, { 0xF21C, 0x00EB, 0xFE7C } },
};

struct struct_80282C40 D_80285AE8[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3A3, 0x004C, 0xFE22 } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF3A3, 0x004C, 0xFE22 } },
};

struct struct_80282C40 D_80285B00[] = {
    { 0x00, 0x00, 0x00, 30, 0x00, 0x00, { 0xF1BA, 0x0092, 0xFE22 } },
    { 0xFF, 0x00, 0x00, 30, 0x00, 0x00, { 0xF1BA, 0x0092, 0xFE22 } },
};

struct struct_80282C40 D_80285B18[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF496, 0x0029, 0xFF27 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF496, 0x0029, 0xFF27 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF4D5, 0xFFE3, 0xFF70 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF54D, 0xFFE8, 0xFF9B } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF54D, 0xFFE8, 0xFF9B } },
};

struct struct_80282C40 D_80285B54[] = {
    { 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, { 0xF307, 0x012F, 0xFE96 } },
    { 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, { 0xF307, 0x012F, 0xFE96 } },
    { 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, { 0xF326, 0x00CE, 0xFF12 } },
    { 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, { 0xF35A, 0x0000, 0xFF9E } },
    { 0xFF, 0x00, 0x00, 0x41, 0x00, 0x00, { 0xF35A, 0x0000, 0xFF9E } },
};

struct struct_80282C40 D_80285B90[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF5BB, 0x0008, 0xFE7E } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF5BB, 0x0008, 0xFE7E } },
};

struct struct_80282C40 D_80285BA8[] = {
    { 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, { 0xF7A6, 0x0044, 0xFECC } },
    { 0xFF, 0x00, 0x00, 0x05, 0x00, 0x00, { 0xF7A6, 0x0044, 0xFECC } },

    // Unused? What's going on here? D_80285BC0 (no label existed)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF690, 0x0018, 0xFE9E } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF690, 0x0018, 0xFE9E } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF690, 0x0018, 0xFE9E } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF6B9, 0x0021, 0xFEA5 } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF6CD, 0x001B, 0xFEA9 } },

    { 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, { 0xF4A5, 0xFFF0, 0xFE49 } },
    { 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, { 0xF4A5, 0xFFF0, 0xFE49 } },
    { 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, { 0xF4A5, 0xFFF0, 0xFE49 } },
    { 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF4CE, 0x0046, 0xFE4D } },
    { 0xFF, 0x00, 0x00, 0x1E, 0x00, 0x00, { 0xF4EB, 0x0084, 0xFE5C } },
};

struct struct_80282C40 D_80285C38[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF4ED, 0xFFEB, 0xFF66 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF4ED, 0xFFEB, 0xFF66 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF408, 0x0028, 0xFE82 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF32B, 0x0062, 0xFDD6 } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF2A4, 0x006A, 0xFDA5 } },
};

struct struct_80282C40 D_80285C74[] = {
    { 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, { 0xF616, 0xFFD8, 0x00F7 } },
    { 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, { 0xF616, 0xFFD8, 0x00F7 } },
    { 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, { 0xF558, 0xFFBB, 0xFFE4 } },
    { 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, { 0xF481, 0xFFF0, 0xFF30 } },
    { 0xFF, 0x00, 0x00, 0x28, 0x00, 0x00, { 0xF414, 0xFFED, 0xFEE1 } },
};

struct struct_80282C40 D_80285CB0[] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF551, 0xFFE7, 0xFFA5 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF551, 0xFFE7, 0xFFA5 } },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF54E, 0xFFE7, 0xFF82 } },
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, { 0xF544, 0xFFFA, 0xFF74 } },
};

struct struct_80282C40 D_80285CE0[] = {
    { 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, { 0xF362, 0xFFF3, 0xFF62 } },
    { 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, { 0xF362, 0xFFF3, 0xFF62 } },
    { 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, { 0xF392, 0xFFF3, 0x0068 } },
    { 0xFF, 0x00, 0x00, 0x19, 0x00, 0x00, { 0xF3E1, 0xFF47, 0x00A2 } },
};

struct Cutscene gCutScene[] = {
    { func_80283D2C, 330 }, { func_802840C8, 270 }, { func_802842D8, 247 },
    { func_80284418, 200 }, { func_80284494, 170 }, { func_802844FC, 108 },
    { func_8028422C, 140 }, { func_802842A8, 270 }, { func_80284308, 0x7FFF },
};

void func_80284418(CinematicCamera* camera) {
    cutscene_event(play_sound_podium, camera, 0x52, 0x52);
    cutscene_event(play_sound_podium, camera, 0x48, 0x48);
    cutscene_event(play_sound_podium, camera, 0x3D, 0x3D);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285A10,
                                       (struct struct_80286A04*) D_80285A4C, 0);
}

void func_80284494(CinematicCamera* camera) {
    cutscene_event(play_sound_fish_2, camera, 0x1E, 0x1E);
    cutscene_event(wrap_func_8028100C, camera, 0, 0);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285A88,
                                       (struct struct_80286A04*) D_80285AB8, 0);
}

void func_802844FC(CinematicCamera* camera) {
    cutscene_event(play_sound_fish, camera, 0x3B, 0x3B);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285AE8,
                                       (struct struct_80286A04*) D_80285B00, 0);
}

void func_8028454C(CinematicCamera* camera) {
    cutscene_event(func_80283CA8, camera, 0, 0);
    cutscene_event(wrap_func_800CA0CC, camera, 1, 1);
    cutscene_event(wrap_func_800CB134, camera, 0, 0);
    cutscene_event(wrap_func_80280FFC, camera, 0x3C, 0x3C);
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285B18,
                                       (struct struct_80286A04*) D_80285B54, 0);
}

void func_802845EC(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285B90,
                                       (struct struct_80286A04*) D_80285BA8, 0);
}

void func_8028461C(CinematicCamera* camera) {
    func_80283240(1);
    func_80283B6C(camera);
}

void func_80284648(CinematicCamera* camera) {
    cutscene_event(func_802845EC, camera, 0, 0);
    cutscene_event(func_8028461C, camera, 0x110, 0x110);
    cutscene_event(func_80283BA4, camera, 0x115, 0x115);
}

UNUSED void func_802846AC(void) {
}

void func_802846B4(CinematicCamera* camera) {
    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285C38,
                                       (struct struct_80286A04*) D_80285C74, 0);
}

//! @todo What does this even do?
void func_802846E4(CinematicCamera* camera) {

    move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80285CB0,
                                       (struct struct_80286A04*) D_80285CE0, 0);
    camera->lookAt[0] += (gPlayerFour->pos[0] - -2796.0f); // <-- rodata
    camera->lookAt[1] += (gPlayerFour->pos[1] - -29.0f);
    camera->lookAt[2] += (gPlayerFour->pos[2] - -97.0f);
    camera->pos[0] += (gPlayerFour->pos[0] - -2796.0f);
    camera->pos[1] += (gPlayerFour->pos[1] - -29.0f);
    camera->pos[2] += (gPlayerFour->pos[2] - -97.0f);
}

struct Cutscene D_80285D58[] = {
    { func_8028454C, 300 }, { func_80284154, 175 },    { func_802846B4, 200 },
    { func_802846E4, 184 }, { func_80284648, 0x7FFF },
};

struct struct_80285D80 D_80285D80[] = {
    { { 0, 0, 0, 0, 0, 0 }, { 0xffc6, 0x0000, 0xfc02 } },    { { 0, 0, 0, 0, 4, 0 }, { 0xffb9, 0x0005, 0xff53 } },
    { { 0, 0, 0, 0, 10, 0 }, { 0xfec3, 0x0036, 0x009e } },   { { -1, 0, 0, 0, 0, 0 }, { 0xfc1a, 0xffdd, 0x0298 } },

    { { 0, 0, 0, 60, 0, 0 }, { 0xfeb1, 0xff45, 0xfd76 } },   { { 0, 0, 0, 60, 4, 0 }, { 0xfea7, 0xff73, 0x00da } },
    { { 0, 0, 0, 60, 10, 0 }, { 0xfd94, 0xff9b, 0x020b } },  { { -1, 0, 0, 60, 0, 0 }, { 0xfa7b, 0x003c, 0x039c } },

    { { 0, 0, 0, 0, 0, 0 }, { 0x04a7, 0x004f, 0x060b } },    { { 0, 0, 0, 0, 0, 0 }, { 0x04a7, 0x004f, 0x060b } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x048a, 0x0068, 0x059a } },    { { 0, 0, 0, 0, 0, 0 }, { 0x0501, 0x0070, 0x04b7 } },
    { { -1, 0, 0, 0, 0, 0 }, { 0x0569, 0x0042, 0x0410 } },   { { 0, 0, 0, 18, 0, 0 }, { 0x051c, 0x00b9, 0x07e5 } },
    { { 0, 0, 0, 40, 0, 0 }, { 0x051c, 0x00b9, 0x07e5 } },   { { 0, 0, 0, 50, 0, 0 }, { 0x04f6, 0x0032, 0x077f } },
    { { 0, 0, 0, 50, 0, 0 }, { 0x040d, 0x0023, 0x0664 } },   { { -1, 0, 0, 50, 0, 0 }, { 0x044d, 0x000c, 0x05a8 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x00d1, 0x0070, 0xf5ab } },    { { 0, 0, 0, 0, 0, 0 }, { 0x00d1, 0x0070, 0xf5ab } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x0145, 0x0043, 0xf624 } },    { { -1, 0, 0, 0, 0, 0 }, { 0x01bb, 0x001b, 0xf6a9 } },
    { { 0, 0, 0, 20, 0, 0 }, { 0x01be, 0xff8b, 0xf723 } },   { { 0, 0, 0, 30, 0, 0 }, { 0x01be, 0xff8b, 0xf721 } },
    { { 0, 0, 0, 60, 0, 0 }, { 0x023d, 0xff7a, 0xf7a5 } },   { { -1, 0, 0, 60, 0, 0 }, { 0x02bb, 0x0047, 0xf854 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfd1a, 0x001f, 0x0aaa } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfd1a, 0x0016, 0x0890 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfd1b, 0x0017, 0x05ee } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfd24, 0x0013, 0x0325 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfcab, 0x0010, 0x01af } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfb52, 0x0013, 0x0094 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfa51, 0x001a, 0x0055 } },    { { -1, 0, 0, 0, 0, 0 }, { 0xf8f5, 0x001b, 0x0053 } },
    { { 0, 0, 0, 50, 0, 0 }, { 0xfd1f, 0xfeb7, 0x094f } },   { { 0, 0, 0, 50, 0, 0 }, { 0xfd21, 0xffdf, 0x069f } },
    { { 0, 0, 0, 50, 0, 0 }, { 0xfd35, 0x0012, 0x03fa } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfd32, 0xffe5, 0x0133 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xfb92, 0x000e, 0x0011 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf993, 0x001c, 0xffb7 } },
    { { 0, 0, 0, 40, 0, 0 }, { 0xf866, 0x0009, 0xfffb } },   { { -1, 0, 0, 30, 0, 0 }, { 0xf712, 0xffe7, 0x00c8 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x079c, 0x00de, 0xf99e } },    { { 0, 0, 0, 0, 0, 0 }, { 0x079c, 0x00de, 0xf99e } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x077f, 0x0099, 0xf9a6 } },    { { 0, 0, 0, 0, 0, 0 }, { 0x0784, 0x005b, 0xf9a2 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x0784, 0x005b, 0xf9a2 } },    { { -1, 0, 0, 0, 0, 0 }, { 0x0784, 0x005b, 0xf9a2 } },
    { { 0, 0, 0, 55, 0, 0 }, { 0x0886, 0xffaa, 0xf863 } },   { { 0, 0, 0, 55, 0, 0 }, { 0x0887, 0xffaa, 0xf864 } },
    { { 0, 0, 0, 55, 0, 0 }, { 0x0960, 0x0010, 0xf9ad } },   { { 0, 0, 0, 66, 0, 0 }, { 0x092b, 0x009e, 0xfaa4 } },
    { { 0, 0, 0, 50, 0, 0 }, { 0x0930, 0x00a9, 0xfa98 } },   { { 0, 0, 0, 50, 0, 0 }, { 0x0932, 0x00a9, 0xfa95 } },
    { { -1, 0, 0, 50, 0, 0 }, { 0x092f, 0x00a1, 0xfa9c } },  { { 0, 0, 0, 0, 0, 0 }, { 0xff37, 0x002d, 0xf9ab } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x007b, 0x0035, 0xfaef } },    { { 0, 0, 0, 0, 0, 0 }, { 0x01a6, 0x002d, 0xfc8e } },
    { { -1, 0, 0, 0, 0, 0 }, { 0x0228, 0x0035, 0xfdad } },   { { 0, 0, 0, 60, 0, 0 }, { 0xfdc7, 0xffe6, 0xfaf6 } },
    { { 0, 0, 0, 60, 0, 0 }, { 0xfed8, 0xffee, 0xfbf4 } },   { { 0, 0, 0, 60, 0, 0 }, { 0xffdc, 0xffe6, 0xfd46 } },
    { { -1, 0, 0, 60, 0, 0 }, { 0x004b, 0xffe7, 0xfe2d } },  { { 0, 0, 0, 0, 0, 0 }, { 0xfc11, 0xffcd, 0x0096 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfc11, 0xffcd, 0x0096 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc16, 0x0096, 0x007c } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfc1e, 0x01e9, 0x0049 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc2c, 0x0419, 0xfff4 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfc4e, 0x0615, 0xffc1 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc31, 0x077f, 0xfff1 } },
    { { -1, 0, 0, 0, 0, 0 }, { 0xfc31, 0x077f, 0xfff1 } },   { { 0, 0, 0, 50, 0, 0 }, { 0xfc60, 0xff8e, 0xfead } },
    { { 0, 0, 0, 26, 0, 0 }, { 0xfc61, 0xff85, 0xfeae } },   { { 0, 0, 0, 45, 0, 0 }, { 0xfc64, 0x0015, 0xfe9f } },
    { { 0, 0, 0, 40, 0, 0 }, { 0xfc66, 0x0104, 0xfe93 } },   { { 0, 0, 0, 40, 0, 0 }, { 0xfcf6, 0x02c5, 0xfec2 } },
    { { 0, 0, 0, 33, 0, 0 }, { 0xfcd2, 0x0446, 0xff38 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfc82, 0x05b0, 0xff46 } },
    { { -1, 0, 0, 37, 0, 0 }, { 0xfc7d, 0x05ac, 0xff4d } },  { { 0, 0, 0, 0, 235, 0 }, { 0xfffb, 0x0047, 0xfe2b } },
    { { 0, 0, 0, 0, 225, 0 }, { 0xfff5, 0x0015, 0xfb8a } },  { { 0, 0, 0, 0, 245, 0 }, { 0x0143, 0x001f, 0xfab2 } },
    { { 0, 0, 0, 0, 39, 0 }, { 0x0230, 0x001d, 0xfaee } },   { { 0, 0, 0, 0, 25, 0 }, { 0x0294, 0x0012, 0xfb89 } },
    { { 0, 0, 0, 0, 228, 0 }, { 0x04b4, 0x001c, 0xfb7b } },  { { 0, 0, 0, 0, 226, 0 }, { 0x0545, 0x0011, 0xfc7c } },
    { { 0, 0, 0, 0, 244, 0 }, { 0x04aa, 0x001b, 0xfd89 } },  { { 0, 0, 0, 0, 0, 0 }, { 0x02f5, 0x000c, 0xfde2 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x00cd, 0xffd6, 0xfde5 } },    { { 0, 0, 0, 0, 13, 0 }, { 0xfefa, 0xffdd, 0xfdcf } },
    { { 0, 0, 0, 0, 16, 0 }, { 0xfdd8, 0xfff9, 0xfe2b } },   { { -1, 0, 0, 0, 0, 0 }, { 0xfd15, 0x0006, 0xff68 } },
    { { 0, 0, 0, 30, 235, 0 }, { 0x0002, 0xffcd, 0xfc47 } }, { { 0, 0, 0, 30, 225, 0 }, { 0x00f6, 0xffc1, 0xf9e6 } },
    { { 0, 0, 0, 30, 245, 0 }, { 0x032d, 0xffbe, 0xfaad } }, { { 0, 0, 0, 20, 39, 0 }, { 0x02c7, 0xffff, 0xfcca } },
    { { 0, 0, 0, 30, 25, 0 }, { 0x047b, 0xffb4, 0xfb52 } },  { { 0, 0, 0, 20, 228, 0 }, { 0x05eb, 0xffed, 0xfcff } },
    { { 0, 0, 0, 30, 226, 0 }, { 0x053d, 0xffbc, 0xfe69 } }, { { 0, 0, 0, 30, 244, 0 }, { 0x02c8, 0xffb7, 0xfde1 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0x0104, 0xffd6, 0xfe02 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfedf, 0xff8c, 0xfdef } },
    { { 0, 0, 0, 30, 13, 0 }, { 0xfd19, 0xff9e, 0xfe46 } },  { { 0, 0, 0, 30, 16, 0 }, { 0xfcc0, 0xffcd, 0xffc7 } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xfc30, 0x0060, 0x011a } },  { { 0, 0, 0, 0, 0, 0 }, { 0xf4cf, 0x0217, 0x07f0 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf575, 0x018b, 0x0622 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf5e3, 0x0123, 0x04ee } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf6a2, 0x01a8, 0x02dc } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf68b, 0x0262, 0x0050 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf7b5, 0x0189, 0xfcdb } },    { { -1, 0, 0, 0, 0, 0 }, { 0xf8a8, 0x012d, 0xf7e1 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xf558, 0x00f2, 0x0673 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf5fe, 0x0065, 0x04a5 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xf665, 0xffe3, 0x0385 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf771, 0x011d, 0x012a } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xf7f7, 0x0186, 0xff48 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf873, 0x0116, 0xfe9c } },
    { { -1, 0, 0, 42, 0, 0 }, { 0xf86d, 0x00e0, 0xf9cc } },  { { 0, 0, 0, 0, 0, 0 }, { 0x06d4, 0x00a6, 0xfae3 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x06d4, 0x00a6, 0xfae3 } },    { { 0, 0, 0, 0, 0, 0 }, { 0x06fe, 0x0066, 0xf9cf } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x04f6, 0x0046, 0xf966 } },    { { 0, 0, 0, 0, 0, 0 }, { 0x017c, 0x0053, 0xfa6c } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfec9, 0x003a, 0xfc36 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc6f, 0xffde, 0xfdc0 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfb17, 0xffbe, 0xfec7 } },    { { -1, 0, 0, 0, 0, 0 }, { 0xf96a, 0x00ab, 0x011b } },
    { { 0, 0, 0, 9, 0, 0 }, { 0x06db, 0xffd3, 0xfca8 } },    { { 0, 0, 0, 30, 0, 0 }, { 0x06dc, 0xffdc, 0xfcac } },
    { { 0, 0, 0, 25, 0, 0 }, { 0x077f, 0xffbe, 0xfb94 } },   { { 0, 0, 0, 25, 0, 0 }, { 0x06b5, 0xffbd, 0xfa16 } },
    { { 0, 0, 0, 25, 0, 0 }, { 0x0359, 0xffd6, 0xfa1e } },   { { 0, 0, 0, 25, 0, 0 }, { 0x005b, 0xffb1, 0xfb2f } },
    { { 0, 0, 0, 25, 0, 0 }, { 0xfdee, 0xff57, 0xfc9c } },   { { 0, 0, 0, 25, 0, 0 }, { 0xfc95, 0xff34, 0xfda4 } },
    { { -1, 0, 0, 25, 0, 0 }, { 0xfa96, 0xfff6, 0xffb7 } },  { { 0, 0, 0, 0, 0, 0 }, { 0xfaef, 0xff6e, 0xfdaa } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfaef, 0xff6e, 0xfdaa } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfb54, 0xff68, 0xfdbf } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfbb5, 0xff64, 0xfde0 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc0d, 0xff64, 0xfdfa } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfc9f, 0xff64, 0xfe2b } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfd80, 0xff61, 0xfe99 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfdca, 0xff66, 0xfeab } },    { { -1, 0, 0, 0, 0, 0 }, { 0xfe06, 0xff6d, 0xfebf } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xf911, 0xff0f, 0xfe1a } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf911, 0xff0f, 0xfe1a } },
    { { 0, 0, 0, 35, 0, 0 }, { 0xf9dd, 0xfe99, 0xfec0 } },   { { 0, 0, 0, 35, 0, 0 }, { 0xfb37, 0xfe86, 0xff8e } },
    { { 0, 0, 0, 35, 0, 0 }, { 0xfcc7, 0xfe7a, 0xff8b } },   { { 0, 0, 0, 35, 0, 0 }, { 0xfe21, 0xff1d, 0xff62 } },
    { { 0, 0, 0, 38, 0, 0 }, { 0xff5c, 0xff8e, 0xff2e } },   { { 0, 0, 0, 30, 0, 0 }, { 0xffa1, 0xff56, 0xff54 } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xffdf, 0xff4e, 0xff5d } },  { { 0, 0, 0, 0, 0, 0 }, { 0x0326, 0x0016, 0xfbe5 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x0326, 0x0016, 0xfbe5 } },    { { 0, 0, 0, 0, 0, 0 }, { 0x0279, 0x001c, 0xfbdc } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x00e2, 0x0014, 0xfc78 } },    { { 0, 0, 0, 0, 0, 0 }, { 0x0010, 0x0015, 0xfdcf } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xffa5, 0x006f, 0xfead } },    { { -1, 0, 0, 0, 0, 0 }, { 0xffb3, 0x008e, 0xff63 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0x0381, 0xffa1, 0xfdc3 } },   { { 0, 0, 0, 30, 0, 0 }, { 0x0381, 0xffa1, 0xfdc3 } },
    { { 0, 0, 0, 35, 0, 0 }, { 0x0237, 0xff21, 0xfd88 } },   { { 0, 0, 0, 35, 0, 0 }, { 0x005d, 0xff63, 0xfe39 } },
    { { 0, 0, 0, 35, 0, 0 }, { 0xfef8, 0x007d, 0xff60 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfe07, 0x00c8, 0xffb7 } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xfdc3, 0x00d1, 0xff6a } },  { { 0, 0, 0, 0, 0, 0 }, { 0xfff9, 0x004d, 0xfd4b } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xff2a, 0x009e, 0xfcf8 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfe1b, 0x0174, 0xfd03 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfc2c, 0x0163, 0xfbea } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfbc2, 0x003c, 0xfaa3 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfbf4, 0x0012, 0xf87d } },    { { -1, 0, 0, 0, 0, 0 }, { 0xfbf4, 0x0012, 0xf87d } },
    { { 0, 0, 0, 37, 0, 0 }, { 0xfedb, 0xff54, 0xfc06 } },   { { 0, 0, 0, 37, 0, 0 }, { 0xfeb0, 0xff91, 0xfb66 } },
    { { 0, 0, 0, 37, 0, 0 }, { 0xfe79, 0x0070, 0xfb62 } },   { { 0, 0, 0, 37, 0, 0 }, { 0xfde5, 0x00ac, 0xfb55 } },
    { { 0, 0, 0, 37, 0, 0 }, { 0xfd9b, 0x0079, 0xfb3c } },   { { 0, 0, 0, 48, 0, 0 }, { 0xfd0d, 0x0037, 0xfa1a } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xfd20, 0x001d, 0xfa0d } },  { { 0, 0, 0, 0, 0, 0 }, { 0x0032, 0x0017, 0xfb25 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0x0032, 0x0011, 0xf7f3 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xffef, 0x000a, 0xf6b4 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xff34, 0x0008, 0xf54e } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfe63, 0x0009, 0xf494 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfdd3, 0x0005, 0xf436 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfcc0, 0x0005, 0xf3ec } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfba1, 0x0004, 0xf3e5 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfa43, 0x0002, 0xf3d4 } },
    { { -1, 0, 0, 0, 0, 0 }, { 0xf96d, 0x0003, 0xf3f1 } },   { { 0, 0, 0, 30, 0, 0 }, { 0x0032, 0xff86, 0xf947 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0x0042, 0xff4f, 0xf627 } },   { { 0, 0, 0, 30, 0, 0 }, { 0x0181, 0xff13, 0xf612 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0x00eb, 0xff7e, 0xf60e } },   { { 0, 0, 0, 30, 0, 0 }, { 0x002b, 0xffc5, 0xf554 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xff90, 0xffb2, 0xf508 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfeb4, 0x0011, 0xf3e0 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xfd8c, 0x001a, 0xf386 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfc2b, 0x0067, 0xf3ae } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xfb56, 0x0051, 0xf3ac } },  { { 0, 0, 0, 0, 0, 0 }, { 0xf49d, 0x001e, 0x003a } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf4c9, 0x003c, 0x0039 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf4f7, 0x005a, 0x0038 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf4f7, 0x005a, 0x0038 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf535, 0x0057, 0x0036 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf6ae, 0x0036, 0x001d } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf6ce, 0x0031, 0x001c } },
    { { -1, 0, 0, 0, 0, 0 }, { 0xf6ce, 0x0031, 0x001c } },   { { 0, 0, 0, 80, 0, 0 }, { 0xf669, 0xff5a, 0x004d } },
    { { 0, 0, 0, 80, 0, 0 }, { 0xf69d, 0xff8d, 0x004b } },   { { 0, 0, 0, 80, 0, 0 }, { 0xf6d6, 0xffc8, 0x003b } },
    { { 0, 0, 0, 5, 0, 0 }, { 0xf6d9, 0xffd3, 0x0040 } },    { { 0, 0, 0, 3, 0, 0 }, { 0xf717, 0xffd0, 0x0045 } },
    { { 0, 0, 0, 4, 0, 0 }, { 0xf894, 0xffc5, 0x000f } },    { { 0, 0, 0, 4, 0, 0 }, { 0xf8b8, 0xffcc, 0x001a } },
    { { -1, 0, 0, 4, 0, 0 }, { 0xf8b8, 0xffcc, 0x0014 } },   { { 0, 0, 0, 0, 0, 0 }, { 0xff61, 0x03ce, 0xf2bf } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfd33, 0x038d, 0xf40f } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfc59, 0x0366, 0xf4df } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfbe7, 0x0321, 0xf5d5 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfb9a, 0x02a9, 0xf704 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfb58, 0x0220, 0xf8b1 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfabf, 0x01f3, 0xfa36 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf9f2, 0x01ef, 0xfb8d } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf943, 0x01fe, 0xfd31 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf950, 0x0200, 0xfea3 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf9cd, 0x0214, 0xff83 } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xface, 0x022d, 0x000d } },    { { -1, 0, 0, 0, 0, 0 }, { 0xfbd8, 0x0231, 0x000d } },
    { { 0, 0, 0, 20, 0, 0 }, { 0xfea7, 0x033c, 0xf478 } },   { { 0, 0, 0, 20, 0, 0 }, { 0xfcb7, 0x0306, 0xf5e1 } },
    { { 0, 0, 0, 20, 0, 0 }, { 0xfbf7, 0x02d4, 0xf6b4 } },   { { 0, 0, 0, 20, 0, 0 }, { 0xfb96, 0x0278, 0xf7a5 } },
    { { 0, 0, 0, 20, 0, 0 }, { 0xfb66, 0x0217, 0xf8e0 } },   { { 0, 0, 0, 20, 0, 0 }, { 0xfb26, 0x01c5, 0xfa9b } },
    { { 0, 0, 0, 20, 0, 0 }, { 0xfadd, 0x01eb, 0xfc29 } },   { { 0, 0, 0, 28, 0, 0 }, { 0xfa89, 0x0208, 0xfd69 } },
    { { 0, 0, 0, 29, 0, 0 }, { 0xfaf3, 0x020a, 0xfe2d } },   { { 0, 0, 0, 28, 0, 0 }, { 0xfb2b, 0x0226, 0xfe09 } },
    { { 0, 0, 0, 28, 0, 0 }, { 0xfb39, 0x0216, 0xfe2c } },   { { 0, 0, 0, 28, 0, 0 }, { 0xfb08, 0x0202, 0xfe1f } },
    { { -1, 0, 0, 28, 0, 0 }, { 0xfb31, 0x023f, 0xfe38 } },  { { 0, 0, 0, 0, 0, 0 }, { 0xfe5d, 0x01f9, 0xf67a } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfd7a, 0x0136, 0xf7ab } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfb8b, 0x0066, 0xfa4f } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf9c4, 0x0032, 0xfc9f } },    { { 0, 0, 0, 0, 0, 0 }, { 0xf7b6, 0x0022, 0xfdda } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xf771, 0x0054, 0xfdec } },    { { -1, 0, 0, 0, 0, 0 }, { 0xf7df, 0x00a6, 0xfed5 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xfd5a, 0x0107, 0xf7db } },   { { 0, 0, 0, 30, 0, 0 }, { 0xfc6f, 0x0063, 0xf918 } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xfa65, 0x002e, 0xfbe0 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf843, 0x0010, 0xfddc } },
    { { 0, 0, 0, 30, 0, 0 }, { 0xf5d2, 0x000c, 0xfe55 } },   { { 0, 0, 0, 30, 0, 0 }, { 0xf57e, 0x006c, 0xfde9 } },
    { { -1, 0, 0, 30, 0, 0 }, { 0xf5f2, 0x00a8, 0xfe82 } },  { { 0, 0, 0, 0, 0, 0 }, { 0xf8e5, 0x0181, 0x054b } },
    { { 0, 0, 0, 0, 0, 0 }, { 0xfae4, 0x00b9, 0x0556 } },    { { 0, 0, 0, 0, 0, 0 }, { 0xfccc, 0x0010, 0x0556 } },
    { { -1, 0, 0, 0, 0, 0 }, { 0xfd42, 0x0047, 0x056d } },   { { 0, 0, 0, 70, 0, 0 }, { 0xf88a, 0x00e8, 0x0378 } },
    { { 0, 0, 0, 70, 0, 0 }, { 0xfc3e, 0x001d, 0x0410 } },   { { 0, 0, 0, 70, 0, 0 }, { 0xfea3, 0x0041, 0x05f7 } },
    { { -1, 0, 0, 70, 0, 0 }, { 0xfeda, 0x0109, 0x0642 } },
};

struct struct_80286A10 {
    u16 unk0[4];
    struct struct_80285D80* unk8;
    struct struct_80285D80* unkC;
};

#ifdef VERSION_EU
struct struct_80286A04 D_80286A04[] = {

    { 0x01, 0x00, &D_80285D80[162], &D_80285D80[162], 0x0087 },
    { 0x00, 0x08, &D_80285D80[0], &D_80285D80[4], 0x00D1 },
    { 0x00, 0x09, &D_80285D80[8], &D_80285D80[13], 0x00D1 },
    { 0x00, 0x0B, &D_80285D80[26], &D_80285D80[34], 0x00D1 },
    { 0x00, 0x05, &D_80285D80[18], &D_80285D80[22], 0x00D1 },
    { 0x00, 0x02, &D_80285D80[42], &D_80285D80[48], 0x00D1 },
    { 0x00, 0x0E, &D_80285D80[259], &D_80285D80[263], 0x00D1 },
    { 0x00, 0x0C, &D_80285D80[55], &D_80285D80[59], 0x00D1 },
    { 0x00, 0x07, &D_80285D80[63], &D_80285D80[71], 0x00D1 },
    { 0x00, 0x01, &D_80285D80[79], &D_80285D80[92], 0x00D1 },
    { 0x00, 0x04, &D_80285D80[105], &D_80285D80[112], 0x00D2 },
    { 0x00, 0x12, &D_80285D80[119], &D_80285D80[128], 0x00D2 },
    { 0x00, 0x00, &D_80285D80[155], &D_80285D80[162], 0x00D2 },
    { 0x00, 0x06, &D_80285D80[169], &D_80285D80[176], 0x00D2 },
    { 0x00, 0x0A, &D_80285D80[183], &D_80285D80[193], 0x00D2 },
    { 0x00, 0x03, &D_80285D80[203], &D_80285D80[211], 0x00D2 },
    { 0x00, 0x0D, &D_80285D80[219], &D_80285D80[232], 0x00D2 },
    { 0x01, 0x00, &D_80285D80[162], &D_80285D80[162], 0x00D2 },
    { 0x02, 0x07, &D_80285D80[245], &D_80285D80[252], 0x00D2 },
};

u16 D_80286B34[] = {
    0x0087, 0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D5,
    0x00D5, 0x00D5, 0x00D5, 0x00D5, 0x00D4, 0x00D4, 0x00D4, 0x00DB, 0x00D2, 0x0000,
};

#else

// 0xC90 (80285D80 size) 3216 decimal.
struct struct_80286A04 D_80286A04[] = {

    { 0x01, 0x00, &D_80285D80[162], &D_80285D80[162], 0x0096 },
    { 0x00, 0x08, &D_80285D80[0], &D_80285D80[4], 0x00F1 },
    { 0x00, 0x09, &D_80285D80[8], &D_80285D80[13], 0x00F1 },
    { 0x00, 0x0B, &D_80285D80[26], &D_80285D80[34], 0x00F1 },
    { 0x00, 0x05, &D_80285D80[18], &D_80285D80[22], 0x00F1 },
    { 0x00, 0x02, &D_80285D80[42], &D_80285D80[48], 0x00F1 },
    { 0x00, 0x0E, &D_80285D80[259], &D_80285D80[263], 0x00F1 },
    { 0x00, 0x0C, &D_80285D80[55], &D_80285D80[59], 0x00F1 },
    { 0x00, 0x07, &D_80285D80[63], &D_80285D80[71], 0x00F1 },
    { 0x00, 0x01, &D_80285D80[79], &D_80285D80[92], 0x00F1 },
    { 0x00, 0x04, &D_80285D80[105], &D_80285D80[112], 0x00F1 },
    { 0x00, 0x12, &D_80285D80[119], &D_80285D80[128], 0x00F0 },
    { 0x00, 0x00, &D_80285D80[155], &D_80285D80[162], 0x00F0 },
    { 0x00, 0x06, &D_80285D80[169], &D_80285D80[176], 0x00F0 },
    { 0x00, 0x0A, &D_80285D80[183], &D_80285D80[193], 0x00F0 },
    { 0x00, 0x03, &D_80285D80[203], &D_80285D80[211], 0x00F0 },
    { 0x00, 0x0D, &D_80285D80[219], &D_80285D80[232], 0x00F0 },
    { 0x01, 0x00, &D_80285D80[162], &D_80285D80[162], 0x00F2 },
    { 0x02, 0x07, &D_80285D80[245], &D_80285D80[252], 0x00F0 },
};

u16 D_80286B34[] = {
    0x0096, 0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F3,
    0x00F3, 0x00F3, 0x00F3, 0x00F3, 0x00F2, 0x00F2, 0x00F2, 0x00F9, 0x00F0, 0x0000,
};
#endif

void func_802847CC(CinematicCamera* camera) {
    u16 sp2E;
    u16 sp2C;
    // D_802856B4 cast to u32 triggers cfc1.
    sp2E = D_80286A04[D_800DC5E4].unkC - (10 - (-(((u16) (u32) D_802856B4))));
    sp2C = D_80286A04[D_800DC5E4].unkC;

    cutscene_event(func_80283CD0, camera, 0, 0);
    cutscene_event(play_sound_welcome, camera, 8, 8);
#ifdef VERSION_EU
    cutscene_event(func_80283C78, camera, 134, 134);
#else
    cutscene_event(func_80283C78, camera, 149, 149);
#endif
    cutscene_event(reset_spline_wrap, camera, 0, 0);
    switch (D_80286A04[D_800DC5E4].unk0) {
        case 1:
            cutscene_event(animation_disapears_sliding_borders, camera, 0, -1);
            cutscene_event(wrap_func_80092C80, camera, sp2E - 0x14, sp2E - 0x14);
            break;

        case 2:
            cutscene_event(animation_apears_sliding_borders, camera, 0, 0);
            cutscene_event(play_sound_farewell, camera, 247, 247);
            move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80286A04[D_800DC5E4].unk4,
                                               (struct struct_80286A04*) D_80286A04[D_800DC5E4].unk8, 0);
            break;
        default:
            cutscene_event(animation_apears_sliding_borders, camera, 0, 0);
            cutscene_event(animation_disapears_sliding_borders, camera, sp2E, sp2E);
            cutscene_event(wrap_func_80092C80, camera, sp2E - 0x14, sp2E - 0x14);
            move_cinematic_camera_along_spline(camera, (struct struct_80286A04*) D_80286A04[D_800DC5E4].unk4,
                                               (struct struct_80286A04*) D_80286A04[D_800DC5E4].unk8, 0);
            break;
    }

#ifndef VERSION_EU
    if (gCCSelection == CC_EXTRA) {
        sp2C = D_80286B34[D_800DC5E4];
    }
#endif

    if (gCutsceneShotTimer == sp2C) {
        if (D_80286A04[D_800DC5E4].unk0 != 2) {
            func_80280268(D_80286A04[D_800DC5E4 + 1].courseId);
        }
    }
}

// Start of credits cutscene?
struct Cutscene D_80286B5C[] = {
    { func_802847CC, 0x7FFF },
};

struct struct_80284AE8 {
    u8 unk0[0x1C];
    u8 unk1C;
};

/**
 * Play the current cutscene until either gCutsceneShotTimer reaches the max time, or c->cutscene is set to 0
 */
void play_cutscene(CinematicCamera* camera) {
    UNUSED s32 pad[3];
    s16 cutsceneDuration;

#define CUTSCENE(id, cutscene)                               \
    case id:                                                 \
        cutsceneDuration = cutscene[sCutsceneShot].duration; \
        cutscene[sCutsceneShot].shot(camera);

    if (!camera->cutscene) {
        return;
    }
    switch (camera->cutscene) {
        CUTSCENE(2, gCutScene)
        break;
        CUTSCENE(3, gCutScene)
        break;
        CUTSCENE(4, gCutScene)
        break;
        CUTSCENE(5, D_80285D58)
        wrap_func_800CB14C(camera);
        break;
        CUTSCENE(6, D_80286B5C)
        break;
    }

#undef CUTSCENE

    if ((cutsceneDuration != 0) && ((gCutsceneShotTimer & 0xC000) == 0)) {

        if (gCutsceneShotTimer < 16383) {
            gCutsceneShotTimer++;
        }
        if (gCutsceneShotTimer == cutsceneDuration) {
            sCutsceneShot++;
            gCutsceneShotTimer = 0;
            reset_spline();
        }
    } else {
        if (gCutsceneShotTimer & 0x4000) {
            gCutsceneShotTimer = 0;
            reset_spline();
        } else {
            D_802876D8 = 0;
            sCutsceneShot = 0;
            gCutsceneShotTimer = 0;
            reset_spline();
        }
    }
}

/**
 * Scene transition
 *
 * Sliding black borders that open horizontally to display scene.
 * Used at the beginning of award ceremony and throughout credits.
 */
void ceremony_transition_sliding_borders(void) {
    f32 uly;
    f32 lry;

    lry = D_802856B0 - gSizeSlidingBorders;
    if (lry < 0.0f) {
        lry = 0.0f;
    }
    uly = D_802856B0 + gSizeSlidingBorders;
    // clang-format off
    // Note that this MUST be on one line. All hail significant whitespace in C!
    do {if (uly > 240.0f) { uly = 239.0f; } } while (0);
    // clang-format on

    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetFillColor(gDisplayListHead++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1)));

    // Add lry - 1 so that the transition border clears the top of the screen properly
    gDPFillWideRectangle(gDisplayListHead++, OTRGetRectDimensionFromLeftEdge(0), 0, OTRGetGameRenderWidth(), (s32)lry - 1);
    gDPFillWideRectangle(gDisplayListHead++, OTRGetRectDimensionFromLeftEdge(0), (s32)uly, OTRGetGameRenderWidth(), 239);
    //gDPFillRectangle(gDisplayListHead++, 0, 0, 319, (s32) lry);
    //gDPFillRectangle(gDisplayListHead++, 0, (s32) uly, 319, 239);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    adjust_f32_value_transition(&gSizeSlidingBorders, gOrderedSizeSlidingBorders, D_802856BC / D_802856B4);
}
