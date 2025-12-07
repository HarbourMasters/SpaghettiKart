#include <libultraship.h>
#include <libultra/gbi.h>
#include "Seagull.h"
#include <vector>
#include "World.h"

#include "port/Game.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "defines.h"
#include "camera.h"
#include "update_objects.h"
#include "render_objects.h"
#include "actors.h"
#include "code_80057C60.h"
#include "code_80086E70.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80005FD0.h"
#include "some_data.h"
#include "ceremony_and_credits.h"
#include "assets/models/common_data.h"
extern SplineData D_800E6034;
extern SplineData D_800E60F0;
extern SplineData D_800E61B4;
extern SplineData D_800E6280;
}

SplineData* D_800E633C[] = { &D_800E6034, &D_800E60F0, &D_800E61B4, &D_800E6280 };

size_t OSeagull::_count = 0;

OSeagull::OSeagull(const SpawnParams& params) : OObject(params) {
    Name = "Seagull";
    ResourceName = "mk:seagull";
    _idx = _count;
    FVector pos = params.Location.value_or(FVector(0, 0, 0));

    s16 randZ;
    s16 randX;
    s16 randY;
    randX = random_int(200) + -100.0;
    randY = random_int(20);
    randZ = random_int(200) + -100.0;

    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);

    Object* object = &gObjectList[_objectIndex];

    object->pos[0] = pos.x;
    object->pos[1] = pos.y;
    object->pos[2] = pos.z;

    set_obj_origin_pos(_objectIndex, pos.x, pos.y, pos.z);
    if (_idx < (NUM_SEAGULLS / 2)) {
        gObjectList[_objectIndex].unk_0D5 = 0;
    } else {
        gObjectList[_objectIndex].unk_0D5 = 1;
    }

    _count++;
}

void OSeagull::Tick() {
    Object* object;
    s32 objectIndex = _objectIndex;

        object = &gObjectList[objectIndex];
        if (object->state == 0) {
            return;
        }

        OSeagull::func_80082714(objectIndex, _idx);
        OSeagull::func_8008275C(objectIndex);
        if (func_80072320(objectIndex, 2) != 0) {
            func_800722CC(objectIndex, 2);
            if (D_80165A90 != 0) {
                D_80165A90 = 0;
                D_80183E40[0] = 0.0f;
                D_80183E40[1] = 0.0f;
                D_80183E40[2] = 0.0f;
                if (gGamestate != CREDITS_SEQUENCE) {
                    func_800C98B8(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                } else {
                    if (_idx == 1) {
                        if (gCutsceneShotTimer <= 150) {
                            object = &gObjectList[_objectIndex];
                            func_800C98B8(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                        }
                    }
                }
            }
        }
    //}
    if (D_80165900 != 0) {
        D_80165900 -= 1;
    } else {
        if (gGamestate != CREDITS_SEQUENCE) {
            D_80165900 = 60;
        } else {
            D_80165900 = 15;
        }
        if ((D_80165908 != 0) && (D_80165A90 == 0)) {
            D_80165A90 = 1;
        }
    }
    D_80165908 = 0;
}

void OSeagull::Draw(s32 cameraId) { // render_object_seagulls
    s32 objectIndex = _objectIndex;

    if (func_8008A364(objectIndex, cameraId, 0x5555U, 0x000005DC) < 0x9C401 && CVarGetInteger("gNoCulling", 0) == 0) {
        D_80165908 = 1;
        _toggle = true;
    }
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        OSeagull::func_800552BC(objectIndex);
    }
}

void OSeagull::func_800552BC(s32 objectIndex) {
    if (gObjectList[objectIndex].state >= 2) {
        rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].direction_angle,
                                      gObjectList[objectIndex].sizeScaling);
        gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D0077D0);
        if (gIsGamePaused == 0) {
            gObjectList[objectIndex].unk_0A2 = render_animated_model((Armature*) gObjectList[objectIndex].model,
                                                                     (Animation**) gObjectList[objectIndex].vertex, 0,
                                                                     gObjectList[objectIndex].unk_0A2);
        } else {
            render_animated_model((Armature*) gObjectList[objectIndex].model,
                                  (Animation**) gObjectList[objectIndex].vertex, 0, gObjectList[objectIndex].unk_0A2);
        }
    }
}

void OSeagull::func_8008275C(s32 objectIndex) {
    UNUSED s32 stackPadding;
    switch (gObjectList[objectIndex].unk_0DD) {
        case 1:
            func_8008B78C(objectIndex);
            object_calculate_new_pos_offset(objectIndex);
            break;
        case 2:
            func_8008B78C(objectIndex);
            vec3f_copy(gObjectList[objectIndex].unk_01C, gObjectList[objectIndex].pos);
            func_8000D940(gObjectList[objectIndex].origin_pos, (s16*) &gObjectList[objectIndex].unk_0C6,
                          gObjectList[objectIndex].unk_034, 0.0f, 0);
            gObjectList[objectIndex].offset[0] *= 2.0;
            gObjectList[objectIndex].offset[1] *= 2.5;
            gObjectList[objectIndex].offset[2] *= 2.0;
            object_calculate_new_pos_offset(objectIndex);
            gObjectList[objectIndex].direction_angle[1] =
                get_angle_between_two_vectors(gObjectList[objectIndex].unk_01C, gObjectList[objectIndex].pos);
            break;
    }
    func_800873F4(objectIndex);
}

void OSeagull::func_8008241C(s32 objectIndex, s32 arg1) {
    s16 randZ;
    s16 randX;
    s16 randY;

    gObjectList[objectIndex].unk_0D8 = 1;
    gObjectList[objectIndex].model = (Gfx*) d_course_koopa_troopa_beach_unk4;
    gObjectList[objectIndex].vertex = (Vtx*) d_course_koopa_troopa_beach_unk_data5;
    gObjectList[objectIndex].sizeScaling = 0.2f;
    gObjectList[objectIndex].unk_0DD = 1;
    randX = random_int(0x00C8) + -100.0;
    randY = random_int(0x0014);
    randZ = random_int(0x00C8) + -100.0;

    FVector pos = SpawnPos;
    set_obj_origin_pos(objectIndex, (randX + pos.x) * xOrientation, randY + pos.y, randZ + pos.z);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    gObjectList[objectIndex].unk_034 = 1.0f;
    func_80086EF0(objectIndex);
    gObjectList[objectIndex].spline = D_800E633C[arg1 % 4];
    set_object_flag(objectIndex, 0x00000800);
    object_next_state(objectIndex);
}


void OSeagull::func_80082714(s32 objectIndex, s32 arg1) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            OSeagull::func_8008241C(objectIndex, arg1);
            break;
        case 0:
        default:
            break;
    }
}