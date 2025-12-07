#include "Flagpole.h"
#include "World.h"

extern "C" {
#include "code_800029B0.h"
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/yoshi_valley/yoshi_valley_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
}

size_t OFlagpole::_count = 0;

OFlagpole::OFlagpole(const SpawnParams& params) : OObject(params) {
    Name = "Flagpole";
    ResourceName = "mk:flagpole";
    _idx = _count;

    find_unused_obj_index(&_objectIndex);
    init_object(_objectIndex, 0);

    SpawnPos = params.Location.value_or(FVector(0, 0, 0));
    gObjectList[_objectIndex].pos[0] = SpawnPos.x;
    gObjectList[_objectIndex].pos[1] = SpawnPos.y;
    gObjectList[_objectIndex].pos[2] = SpawnPos.z;

    SpawnRot = params.Rotation.value_or(IRotator(0, 0, 0));
    gObjectList[_objectIndex].orientation[0] = SpawnRot.pitch;
    gObjectList[_objectIndex].orientation[1] = SpawnRot.yaw;
    gObjectList[_objectIndex].orientation[2] = SpawnRot.roll;

    _count++;
}

void OFlagpole::Tick() { // func_80083080
    s32 objectIndex = _objectIndex;

    if (gObjectList[objectIndex].state != 0) {
        OFlagpole::func_80083018(objectIndex);
        OFlagpole::func_80083060(objectIndex);
    }
}

void OFlagpole::Draw(s32 cameraId) { // func_80055228
    s32 objectIndex = _objectIndex;

    func_8008A364(objectIndex, cameraId, 0x4000U, 0x000005DC);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        OFlagpole::func_80055164(objectIndex);
    }
}

void OFlagpole::func_80055164(s32 objectIndex) { // func_80055164
    if (gObjectList[objectIndex].state >= 2) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)D_0D0077A0);
        rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                                      gObjectList[objectIndex].sizeScaling);
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

void OFlagpole::func_80082F1C(s32 objectIndex) {
    gObjectList[objectIndex].model = (Gfx*) d_course_yoshi_valley_unk5;
    gObjectList[objectIndex].vertex = (Vtx*) d_course_yoshi_valley_unk4;
    gObjectList[objectIndex].sizeScaling = 0.027f;
    object_next_state(objectIndex);
    FVector pos = SpawnPos;
    set_obj_origin_pos(objectIndex, pos.x * xOrientation, pos.y, pos.z);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    IRotator rot = SpawnRot;
    set_obj_orientation(objectIndex, rot.pitch, rot.yaw, rot.roll); // changed from directional_angle to orientation for editor support
}

void OFlagpole::func_80083018(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 1:
            OFlagpole::func_80082F1C(objectIndex);
            break;
        case 0:
        default:
            break;
    }
}

void OFlagpole::func_80083060(s32 objectIndex) {
    object_calculate_new_pos_offset(objectIndex);
}
