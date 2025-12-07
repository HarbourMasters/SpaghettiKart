#include "ChainChomp.h"
#include "World.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/rainbow_road/rainbow_road_data.h"
#include "assets/textures/tracks/rainbow_road/rainbow_road_data.h"
#include "assets/other/tracks/rainbow_road/rainbow_road_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
#include "code_80005FD0.h"
#include "external.h"
}

size_t OChainChomp::_count = 0;

OChainChomp::OChainChomp() {
    Name = "Chain Chomp";
    ResourceName = "mk:chain_chomp";
    _idx = _count;
    init_object(indexObjectList2[_idx], 0);
    _objectIndex = indexObjectList2[_idx];

    _count++;
}

void OChainChomp::Tick() {
    s32 objectIndex;
    Object* object;

    objectIndex = indexObjectList2[_idx];
    object = &gObjectList[objectIndex];
    if (object->state != 0) {
        OChainChomp::func_800859C8(objectIndex, _idx);
        vec3f_copy(object->unk_01C, object->offset);
        // This should be spawn pos or follow path
        func_8000D940(object->offset, &object->unk_084[8], object->unk_034, object->surfaceHeight, 0);
        object->direction_angle[1] = get_angle_between_two_vectors(object->unk_01C, object->offset);
        object_calculate_new_pos_offset(objectIndex);
        func_80089CBC(objectIndex, 30.0f);
    }
}

void OChainChomp::Draw(s32 cameraId) {
    s32 objectIndex;

    objectIndex = indexObjectList2[_idx];
    func_8008A1D0(objectIndex, cameraId, 0x000005DC, 0x000009C4);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        func_80055AB8(objectIndex, cameraId);
    }
}

void OChainChomp::func_80055AB8(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (gObjectList[objectIndex].state >= 2) {
        if (is_obj_flag_status_active(objectIndex, 0x00100000) != 0) {
            D_80183E40[0] = gObjectList[objectIndex].pos[0];
            D_80183E40[1] = gObjectList[objectIndex].pos[1] + 16.0;
            D_80183E40[2] = gObjectList[objectIndex].pos[2];
            D_80183E80[0] = 0;
            D_80183E80[1] =
                func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
            D_80183E80[2] = 0x8000;
            func_800468E0(D_80183E40, D_80183E80, 0.54f, (u8*) d_course_rainbow_road_sphere, (Vtx*) D_0D0062B0,
                          0x00000020, 0x00000040, 0x00000020, 0x00000040, 5);
        } else {
            rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].direction_angle,
                                          gObjectList[objectIndex].sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D0077D0);
            render_animated_model((Armature*) gObjectList[objectIndex].model,
                                  (Animation**) gObjectList[objectIndex].vertex, 0,
                                  (s16) gObjectList[objectIndex].textureListIndex);
        }
    }
}

void OChainChomp::func_80085878(s32 objectIndex, s32 arg1) {
    TrackPathPoint* temp_v0;
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 1;
    object->model = (Gfx*) d_rainbow_road_unk4;
    object->vertex = (Vtx*) d_rainbow_road_unk3;
    object->sizeScaling = 0.03f;
    object->boundingBoxSize = 0x000A;
    set_object_flag(objectIndex, 0x04000200);
    object->unk_084[8] = (arg1 * 0x12C) + 0x1F4;
    set_obj_origin_pos(objectIndex, 0.0f, -15.0f, 0.0f);
    temp_v0 = &gCurrentTrackPath[(u16) object->unk_084[8]];
    set_obj_origin_offset(objectIndex, temp_v0->x, temp_v0->y, temp_v0->z);
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->unk_034 = 4.0f;
    object->type = get_animation_length(d_rainbow_road_unk3, 0);
    object_next_state(objectIndex);
}

void OChainChomp::func_800859C8(s32 objectIndex, s32 arg1) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) { /* irregular */
        case 0:
            break;
        case 1:
            OChainChomp::func_80085878(objectIndex, arg1);
            break;
        case 2:
            func_80072E54(objectIndex, 0, (s32) object->type, 1, 0, -1);
            break;
    }
    if (D_8018D40C == 0) {
        func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x57));
    }
    func_80074344(objectIndex, &object->surfaceHeight, -0.8f, 0.8f, 0.03f, 0, -1);
}
