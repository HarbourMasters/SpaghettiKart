#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include "math_util.h"
#include "animation.h"
#include "memory.h"
#include <main.h>
#include <libultra/gbi.h>
#include "code_80057C60.h"
#include "engine/Matrix.h"

Vec3s sOriginalPosAnimation;
s16 isNotTheFirst;
s16 sMatrixShouldNotPop;
s16 sMatrixStackSize;

void convert_to_fixed_point_matrix_animation(Mtx* dest, Mat4 src) {
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

void mtxf_translate_rotate2(Mat4 dest, Vec3f pos, Vec3s angle) {
    register f32 sx = sins(angle[0]);
    register f32 cx = coss(angle[0]);

    register f32 sy = sins(angle[1]);
    register f32 cy = coss(angle[1]);

    register f32 sz = sins(angle[2]);
    register f32 cz = coss(angle[2]);

    dest[0][0] = cy * cz;
    dest[0][1] = cy * sz;
    dest[0][2] = -sy;
    dest[0][3] = 0.0f;

    dest[1][0] = sx * sy * cz - cx * sz;
    dest[1][1] = sx * sy * sz + cx * cz;
    dest[1][2] = sx * cy;
    dest[1][3] = 0.0f;

    dest[2][0] = cx * sy * cz + sx * sz;
    dest[2][1] = cx * sy * sz - sx * cz;
    dest[2][2] = cx * cy;
    dest[2][3] = 0.0f;

    dest[3][0] = pos[0];
    dest[3][1] = pos[1];
    dest[3][2] = pos[2];
    dest[3][3] = 1.0f;
}

void render_limb_or_add_mtx(Armature* arg0, s16* arg1, AnimationLimbVector arg2, s32 timeCycle) {
    Vec3f pos;
    Vec3s angle;
    Mat4 modelMatrix;
    s32 i;
    s32 some_offset;
    Gfx* model;
    Gfx* virtualModel;
    virtualModel = arg0->model;
    if (isNotTheFirst == 0) {
        for (i = 0; i < 3; i++) {
            pos[i] = sOriginalPosAnimation[i] + arg0->pos[i];
        }
        isNotTheFirst += 1;
    } else {
        for (i = 0; i < 3; i++) {
            pos[i] = arg0->pos[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (timeCycle < arg2[i].animation_length) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        angle[i] = arg1[arg2[i].indexCycle + some_offset];
    }

    mtxf_translate_rotate2(modelMatrix, pos, angle);
    // convert_to_fixed_point_matrix_animation(&gGfxPool->mtxHud[gMatrixHudCount], modelMatrix);
    sMatrixStackSize += 1;
    // gSPMatrix(gDisplayListHead++, (&gGfxPool->mtxHud[gMatrixHudCount++]),
    //           G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    AddHudMatrix(modelMatrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    if (virtualModel != NULL) {
        model = (virtualModel);
        gSPDisplayList(gDisplayListHead++, model);
    }
}

void render_armature(Armature* animation, Animation* arg1, s16 timeCycle) {
    UNUSED u32* temp;
    s16* angle_array;
    s32 some_offset;
    AnimationLimbVector* animation_cycle_list;
    s32 animation_type;
    s32 someIndex;

    angle_array = (arg1->angle_array);
    animation_cycle_list = (arg1->animation_cycle_spec_vector);
    sMatrixStackSize = 0;
    isNotTheFirst = 0;
    for (someIndex = 0; someIndex < 3; someIndex++) {
        if (timeCycle < (*animation_cycle_list)[someIndex].animation_length) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        sOriginalPosAnimation[someIndex] = angle_array[(*animation_cycle_list)[someIndex].indexCycle + some_offset];
    }
    animation_cycle_list++;
    sMatrixShouldNotPop = 0;
    do {
        animation_type = animation->type;
        switch (animation_type) { /* irregular */
            case STOP_ANIMATION:
                break;
            case DISABLE_AUTOMATIC_POP_MATRIX:
                sMatrixShouldNotPop = 1;
                break;
            case POP_MATRIX:
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                sMatrixStackSize -= 1;
                break;
            case RENDER_MODEL_OR_ADD_POS:
                if (sMatrixShouldNotPop == 0) {
                    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                    sMatrixStackSize -= 1;
                }
                render_limb_or_add_mtx(animation, angle_array, *animation_cycle_list, (s32) timeCycle);
                sMatrixShouldNotPop = 0;
                animation_cycle_list++;
                break;
        }
        animation = (Armature*) ((uintptr_t*) animation + animation->size);
    } while (animation_type != STOP_ANIMATION);
}

s16 render_animated_model(Armature* virtualArmature, Animation** virtualListAnimation, s16 animationIndex,
                          s16 timeCycle) {
    Armature* armature;
    Animation* animation;
    Animation** listAnimation;

    armature = (virtualArmature);
    listAnimation = (virtualListAnimation);      // Convert the array's address
    animation = (listAnimation[animationIndex]); // Convert an array element's address
    if (timeCycle >= animation->animation_length) {
        timeCycle = 0;
    }
    render_armature(armature, animation, timeCycle);
    timeCycle++;
    if (timeCycle >= animation->animation_length) {
        timeCycle = 0;
    }
    return timeCycle;
}

s16 get_animation_length(Animation** addr, s16 offset) {
    Animation** item = (addr);
    Animation* temp = (Animation*) ((void*) item[offset]);
    return temp->animation_length - 1;
}
