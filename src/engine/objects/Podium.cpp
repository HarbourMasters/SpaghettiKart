#include "Podium.h"
#include "assets/ceremony_data.h"

extern "C" {
#include "main.h"
#include "defines.h"
#include "update_objects.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "podium_ceremony_actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "assets/common_data.h"
#include "some_data.h"
#include "code_80091440.h"
#include "code_80086E70.h"
#include "code_80086E70.h"
extern Vec3s D_800E634C[];
}

// Might be Cheep Cheep related?
// Vec3s D_800E634C[] = {
//     { 0xf37e, 0x0013, 0xfe22 },
//     { 0xf37b, 0x0013, 0xfe31 },
//     { 0xf380, 0x0013, 0xfe14 },
// };

OPodium::OPodium(const FVector& pos) {

    _pos = pos;

    for (size_t i = 0; i < NUM_PODIUMS; i++) {
        s32 objectIndex = indexObjectList1[i];
        // init_object(objectIndex, 0);
        // set_obj_origin_pos(objectIndex, pos.x - 1.5, pos.y, pos.z);
    }
}

void OPodium::Tick() { // func_80086604
    s32 objectIndex;
    if ((D_8016347C != 0) && (D_802874D8.unk1D < 3)) {
        if (D_801658C6 == 0) {
            for (size_t i = 0; i < 3; i++) {
                objectIndex = indexObjectList1[i];
                init_object(objectIndex, 0);
            }
            D_801658C6 = 1;
        }
    }
    for (size_t i = 0; i != NUM_PODIUMS; i++) {
        objectIndex = indexObjectList1[i];
        if (gObjectList[objectIndex].state != 0) {
            OPodium::func_80086528(objectIndex, i);
            OPodium::func_80086424(objectIndex);
        }
    }
}

void OPodium::Draw(s32 cameraId) { // func_80055F48
    for (size_t i = 0; i < NUM_PODIUMS; i++) {
        Object* object;

        object = &gObjectList[indexObjectList1[i]];
        if (object->state >= 2) {
            // func_80043220(object->pos, object->direction_angle, object->sizeScaling, object->model);
            rsp_set_matrix_transformation(object->pos, object->direction_angle, object->sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0077A0);
            gSPDisplayList(gDisplayListHead++, object->model);
        }
    }
}

void OPodium::func_8008629C(s32 objectIndex, s32 arg1) {
    switch (arg1) { /* irregular */
        case 0:
            gObjectList[objectIndex].model = (Gfx*) podium_dl3;
            gObjectList[objectIndex].unk_04C = 0x00000038;
            break;
        case 1:
            gObjectList[objectIndex].model = (Gfx*) podium2_dl3;
            gObjectList[objectIndex].unk_04C = 0x0000002B;
            break;
        case 2:
            gObjectList[objectIndex].model = (Gfx*) podium3_dl3;
            gObjectList[objectIndex].unk_04C = 0x0000001E;
            break;
        default:
            break;
    }
    gObjectList[objectIndex].sizeScaling = 1.0f;
    set_obj_origin_pos(objectIndex, _pos.x - 1.5, _pos.y, _pos.z);
    set_obj_origin_offset(objectIndex, 0.0f, -10.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0xF8E4U, 0U);
    gObjectList[objectIndex].unk_048 = 0;
    object_next_state(objectIndex);
}

void OPodium::func_80086424(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 0:
            break;
        case 1:
            gObjectList[objectIndex].velocity[1] = 0.75f;
            func_80086FD4(objectIndex);
            break;
        case 2:
            if (gObjectList[objectIndex].offset[1] >= -2.0) {
                gObjectList[objectIndex].velocity[1] -= 0.1;
            }
            object_add_velocity_offset_y(objectIndex);
            if (gObjectList[objectIndex].offset[1] >= 0.0) {
                gObjectList[objectIndex].offset[1] = 0.0f;
                gObjectList[objectIndex].velocity[1] = 0.0f;
                func_80086F60(objectIndex);
            }
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
}

void OPodium::func_80086528(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 1:
            func_8008629C(objectIndex, arg1);
            break;
        case 2:
            if (set_and_run_timer_object(objectIndex, gObjectList[objectIndex].unk_04C) != 0) {
                func_80091440(arg1);
                func_80086E70(objectIndex);
                object_next_state(objectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            if (gObjectList[objectIndex].unk_0AE == 0) {
                gObjectList[objectIndex].unk_048 = 1;
                object_next_state(objectIndex);
            }
            break;
    }
}
