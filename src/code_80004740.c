#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include "math_util.h"
#include "code_80004740.h"
#include "memory.h"
#include <main.h>
#include <libultra/gbi.h>
#include "code_80057C60.h"

Vec3s D_80162D70;
s16 D_80162D76;
s16 D_80162D78;
s16 D_80162D7A;

void func_80004740(Mtx *dest, Mat4 src) {
#ifdef AVOID_UB
    // Avoid type-casting which is technically UB by calling the equivalent
    // guMtxF2L function. This helps little-endian systems, as well.
    guMtxF2L(src, dest);
#else
    s32 asFixedPoint;
    register s32 i;
    register s16 *a3 = (s16 *) dest;      // all integer parts stored in first 16 bytes
    register s16 *t0 = (s16 *) dest + 16; // all fraction parts stored in last 16 bytes
    register f32 *t1 = (f32 *) src;

    for (i = 0; i < 16; i++) {
        asFixedPoint = *t1++ * (1 << 16); //! float-to-integer conversion responsible for PU crashes
        *a3++ = GET_HIGH_S16_OF_32(asFixedPoint); // integer part
        *t0++ = GET_LOW_S16_OF_32(asFixedPoint);  // fraction part
    }
#endif
}

void mtxf_translate_rotate2(Mat4 dest, Vec3f b, Vec3s c) {
    register f32 sx = sins(c[0]);
    register f32 cx = coss(c[0]);

    register f32 sy = sins(c[1]);
    register f32 cy = coss(c[1]);

    register f32 sz = sins(c[2]);
    register f32 cz = coss(c[2]);

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

    dest[3][0] = b[0];
    dest[3][1] = b[1];
    dest[3][2] = b[2];
    dest[3][3] = 1.0f;
}

void func_80004A1C(animation_type_1 *arg0, s16 *arg1, animation_type_3_triplet arg2, s32 arg3) {
    Vec3f sp94;
    Vec3s sp8C;
    Mat4 sp4C;
    s32 someIndex;
    s32 some_offset;
    Gfx *some_dl;
    Gfx *some_segmented_dl;
    some_segmented_dl = arg0->optional_segmented_dl_address;
    if (D_80162D76 == 0) {
        for (someIndex = 0; someIndex < 3; someIndex++) {
            sp94[someIndex] = D_80162D70[someIndex] + arg0->thing[someIndex];
        }
        D_80162D76 += 1;
    } else {
        for (someIndex = 0; someIndex < 3; someIndex++) {
            sp94[someIndex] = arg0->thing[someIndex];
        }
    }
    for (someIndex = 0; someIndex < 3; someIndex++) {
        if (arg3 < arg2[someIndex].some_limiter) {
            some_offset = arg3;
        } else {
            some_offset = 0;
        }
        sp8C[someIndex] = arg1[arg2[someIndex].some_offset + some_offset];
    }

    mtxf_translate_rotate2(sp4C, sp94, sp8C);
    func_80004740(&gGfxPool->mtxHud[gMatrixHudCount], sp4C);
    D_80162D7A += 1;
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&gGfxPool->mtxHud[gMatrixHudCount++]), G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    if (some_segmented_dl != NULL) {
        some_dl = segmented_to_virtual(some_segmented_dl);
        gSPDisplayList(gDisplayListHead++, some_dl);
    }
}

void func_80004C30(animation_type_1 *arg0, animation_type_2 *arg1, s16 timeCycle) {
    UNUSED u32 *temp;
    s16 *sp40;
    s32 some_offset;
    animation_type_3_triplet *temp_v0;
    s32 new_var;
    s32 someIndex;

    sp40 = segmented_to_virtual(arg1->type_2_array_pointer);
    temp_v0 = segmented_to_virtual(arg1->type_3_array_pointer);
    D_80162D7A = 0;
    D_80162D76 = 0;
    for (someIndex = 0; someIndex < 3; someIndex++) {
        if (timeCycle < (*temp_v0)[someIndex].some_limiter) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        D_80162D70[someIndex] = sp40[(*temp_v0)[someIndex].some_offset + some_offset];
    }
    temp_v0++;
    D_80162D78 = 0;
    do {
        new_var = arg0->type;
        switch (new_var) {                          /* irregular */
        case 3:
            break;
        case 1:
            D_80162D78 = 1;
            break;
        case 2:
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            D_80162D7A -= 1;
            break;
        case 0:
            if (D_80162D78 == 0) {
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                D_80162D7A -= 1;
            }
            func_80004A1C(arg0, sp40, *temp_v0, (s32) timeCycle);
            D_80162D78 = 0;
            temp_v0++;
            break;
        }
        arg0 = (animation_type_1 *) ((uintptr_t *) arg0 + arg0->size);
    } while (new_var != 3);
}

s16 func_80004DFC(animation_type_1 *arg0, animation_type_2 **arg1, s16 arg2, s16 timeCycle) {
    animation_type_1 *sp24;
    animation_type_2 *temp_v0;
    animation_type_2 **sp20;

    sp24 = segmented_to_virtual(arg0);
    sp20 = segmented_to_virtual(arg1); // Convert the array's address
    temp_v0 = segmented_to_virtual(sp20[arg2]); // Convert an array element's address
    if (timeCycle >= temp_v0->animation_length) {
        timeCycle = 0;
    }
    func_80004C30(sp24, temp_v0, timeCycle);
    timeCycle++;
    if (timeCycle >= temp_v0->animation_length) {
        timeCycle = 0;
    }
    return timeCycle;
}

s16 func_80004EAC(void *addr, s16 offset) {
    uintptr_t *item = segmented_to_virtual(addr);
    struct stru_80004EAC *temp = (struct stru_80004EAC *) segmented_to_virtual((void *) item[offset]);

    return temp->unk8 - 1;
}
