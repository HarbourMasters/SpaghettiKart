

#include "StarEmitter.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "render_objects.h"
#include "assets/models/common_data.h"
#include "code_80057C60.h"
#include "update_objects.h"
#include "code_80086E70.h"
#include "math_util.h"
#include "math_util_2.h"
}

StarEmitter::StarEmitter() {
    for (size_t i = 0; i < gObjectParticle3_SIZE; i++) {
        ObjectIndex[i] = NULL_OBJECT_ID;
    }
}

void StarEmitter::Emit(Vec3f arg1, s32 arg2) { // func_80077138
    s32 objectIndex;
    s8 temp_v0_3;
    Vec3s sp30;

    objectIndex = add_unused_obj_index(&ObjectIndex[0], &_next, gObjectParticle3_SIZE);

    if (objectIndex == NULL_OBJECT_ID) {
        return;
    }

    init_object(objectIndex, arg2);
    gObjectList[objectIndex].unk_0D5 = 0x0C;
    gObjectList[objectIndex].sizeScaling = 0.05f;
    set_obj_origin_pos(objectIndex, arg1[0], arg1[1], arg1[2]);
    set_obj_orientation(objectIndex, 0U, 0U, 0U);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    switch (arg2) {
        case 0:
            gObjectList[objectIndex].velocity[1] = -1.0f;
            gObjectList[objectIndex].unk_034 = (f32) ((random_int(0x004BU) * 0.01) + 0.25);
            gObjectList[objectIndex].direction_angle[1] = random_int(0x0040U) << 0xA;
            func_8008751C(objectIndex);
            gObjectList[objectIndex].unk_084[5] = 0x001E;
            break;
        case 1:
            gObjectList[objectIndex].velocity[1] = 1.5f;
            gObjectList[objectIndex].unk_034 = (f32) ((random_int(0x0064U) * 0.01) + 0.5);
            gObjectList[objectIndex].direction_angle[1] = random_int(0x0040U) << 0xA;
            func_8008751C(objectIndex);
            gObjectList[objectIndex].unk_084[5] = 0x0032;
            break;
    }
    temp_v0_3 = random_int(0x000CU);
    if (temp_v0_3 < 9) {
        func_8005C674(temp_v0_3, &sp30[2], &sp30[1], sp30);
        gObjectList[objectIndex].unk_048 = 0;
        gObjectList[objectIndex].unk_084[0] = sp30[2];
        gObjectList[objectIndex].unk_084[1] = sp30[1];
        gObjectList[objectIndex].unk_084[2] = sp30[0];
    } else {
        temp_v0_3 = random_int(3U);
        func_8005C6B4(temp_v0_3, &sp30[2], &sp30[1], sp30);
        gObjectList[objectIndex].unk_084[0] = sp30[2];
        gObjectList[objectIndex].unk_084[1] = sp30[1];
        gObjectList[objectIndex].unk_084[2] = sp30[0];
        gObjectList[objectIndex].unk_084[4] = temp_v0_3;
        gObjectList[objectIndex].unk_048 = 1;
    }
    gObjectList[objectIndex].primAlpha = 0x00FF;
    gObjectList[objectIndex].unk_084[3] = random_int(0x0800U) + 0x400;
    if ((gObjectList[objectIndex].direction_angle[1] < 0x3000) ||
        (gObjectList[objectIndex].direction_angle[1] >= 0xB001)) {
        gObjectList[objectIndex].unk_084[3] = -gObjectList[objectIndex].unk_084[3];
    }
}

void StarEmitter::Tick() { // func_80077640
    s32 someIndex;
    s32 objectIndex;
    Object* object;

    for (someIndex = 0; someIndex < gObjectParticle3_SIZE; someIndex++) {
        objectIndex = ObjectIndex[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state != 0) {
                StarEmitter::func_80077450(objectIndex);
                StarEmitter::func_80077584(objectIndex);
                if (object->state == 0) {
                    delete_object_wrapper(&ObjectIndex[someIndex]);
                }
            }
        }
    }
}

void StarEmitter::Draw(s32 cameraId) { // func_80054BE8
    s32 var_s0;
    s32 temp_a0;
    Camera* camera;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007AE0);
    load_texture_block_ia8_nomirror((u8*) gTexture69C80C, 0x00000020, 0x00000020);
    func_8004B35C(0x000000FF, 0x000000FF, 0, 0x000000FF);
    D_80183E80[0] = 0;
    for (var_s0 = 0; var_s0 < gObjectParticle3_SIZE; var_s0++) {
        temp_a0 = ObjectIndex[var_s0];
        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("Ceremony Stars", (uintptr_t) &ObjectIndex[var_s0]);
        if ((temp_a0 != -1) && (gObjectList[temp_a0].state >= 2)) {
            StarEmitter::func_80054AFC(temp_a0, camera->pos);
        }
        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }
}

void StarEmitter::func_80054AFC(s32 objectIndex, Vec3f arg1) {
    D_80183E80[0] = func_800418E8(gObjectList[objectIndex].pos[2], gObjectList[objectIndex].pos[1], arg1);
    D_80183E80[1] = func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], arg1);
    D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
    func_8004B138((s32) gObjectList[objectIndex].unk_084[0], (s32) gObjectList[objectIndex].unk_084[1],
                  (s32) gObjectList[objectIndex].unk_084[2], (s32) gObjectList[objectIndex].primAlpha);
    rsp_set_matrix_transformation(gObjectList[objectIndex].pos, (u16*) D_80183E80,
                                  gObjectList[objectIndex].sizeScaling);
    gSPVertex(gDisplayListHead++, (uintptr_t) D_0D005AE0, 4, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
}

void StarEmitter::func_80077428(s32 objectIndex) {
    object_next_state(objectIndex);
    func_80086E70(objectIndex);
}

void StarEmitter::func_80077584(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    if ((object->unk_0AE != 0) && (object->unk_0AE == 1) && ((u8) object->unk_0D8 != 0)) {
        if (object->velocity[1] >= -0.5) {
            object->velocity[1] -= 0.15;
        } else {
            object->velocity[2] = 0.0f;
            object->velocity[0] = 0.0f;
        }
    }
    object->orientation[2] += object->unk_084[3];
    object_add_velocity_offset_xyz(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void StarEmitter::func_80077450(s32 objectIndex) {
    UNUSED s16 stackPadding0;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;

    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            StarEmitter::func_80077428(objectIndex);
            break;
        case 2:
            f32_step_up_towards(&gObjectList[objectIndex].sizeScaling, 0.1f, 0.01f);
            if ((gObjectList[objectIndex].pos[1] <= gObjectList[objectIndex].unk_084[5]) &&
                (func_80073B00(objectIndex, &gObjectList[objectIndex].primAlpha, 0x000000FF, 0, 0x00000010, 0, 0) !=
                 0)) {
                func_80086F60(objectIndex);
                func_80072428(objectIndex);
            }
            break;
    }
    if (gObjectList[objectIndex].unk_048 != 0) {
        gObjectList[objectIndex].unk_084[4] = (s16) ((s32) (gObjectList[objectIndex].unk_084[4] + 1) % 3);
        func_8005C6B4(gObjectList[objectIndex].unk_084[4], &sp3C, &sp3A, &sp38);
        gObjectList[objectIndex].unk_084[0] = sp3C;
        gObjectList[objectIndex].unk_084[1] = sp3A;
        gObjectList[objectIndex].unk_084[2] = sp38;
    }
}