#include "HotAirBalloon.h"
#include "World.h"
#include "port/Game.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/luigi_raceway/luigi_raceway_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
#include "actors.h"
}

OHotAirBalloon::OHotAirBalloon(const SpawnParams& params) : OObject(params) {
    Name = "Hot Air Balloon";
    ResourceName = "mk:hot_air_balloon";

    SpawnPos = params.Location.value_or(FVector{0.0f, 0.0f, 0.0f});

    D_80165898 = 0;

    // Spawn balloon on second lap.
    if (IsLuigiRaceway()) {
        _visible = (bool*)&D_80165898;
    } else { // Spawn balloon on race start
        bool mod = true;
        _visible = &mod;
    }

    find_unused_obj_index(&_objectIndex);


    init_object(_objectIndex, 0);
}

void OHotAirBalloon::SetSpawnParams(SpawnParams& params) {
    OObject::SetSpawnParams(params);
}

void OHotAirBalloon::Tick() {
    s32 objectIndex = _objectIndex;
    
    if (*_visible) {
        if (gObjectList[objectIndex].state != 0) {
            OHotAirBalloon::func_80085768(objectIndex);
            OHotAirBalloon::func_80085534(objectIndex);
            object_calculate_new_pos_offset(objectIndex);
            if (gObjectList[objectIndex].state >= 2) {
                gActorHotAirBalloonItemBox->pos[0] = gObjectList[objectIndex].pos[0];
                gActorHotAirBalloonItemBox->pos[1] = gObjectList[objectIndex].pos[1] - 10.0;
                gActorHotAirBalloonItemBox->pos[2] = gObjectList[objectIndex].pos[2];
            }
        }
    }
}

void OHotAirBalloon::Draw(s32 cameraId) {
    s32 objectIndex;
    objectIndex = _objectIndex;
    if (*_visible) {
        if (gGamestate != CREDITS_SEQUENCE) {
            func_8008A1D0(objectIndex, cameraId, 0x000005DC, 0x00000BB8);
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                OHotAirBalloon::func_80055CCC(objectIndex, cameraId);
            }
        } else {
            clear_object_flag(objectIndex, 0x00100000);
            OHotAirBalloon::func_80055CCC(objectIndex, cameraId);
        }
    }
}

void OHotAirBalloon::func_80055CCC(s32 objectIndex, s32 cameraId) {
    UNUSED s32 pad;
    f32 test;
    Camera* camera;

    camera = &camera1[cameraId];
    if (gObjectList[objectIndex].state >= 2) {
        func_8008A454(objectIndex, cameraId, 0x0000012C);
        test = gObjectList[objectIndex].pos[1] - gObjectList[objectIndex].surfaceHeight;
        func_8004A6EC(objectIndex, (20.0 / test) + 0.5);
        if (is_obj_index_flag_status_inactive(objectIndex, 0x00100000) != 0) {
            func_80043328(gObjectList[objectIndex].pos, (u16*) gObjectList[objectIndex].direction_angle,
                          gObjectList[objectIndex].sizeScaling, (Gfx*)d_course_luigi_raceway_dl_F960);
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_luigi_raceway_dl_F650);
        } else {
            D_80183E80[0] = (s16) gObjectList[objectIndex].direction_angle[0];
            D_80183E80[1] =
                (s16) (func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos) +
                       0x8000);
            D_80183E80[2] = (u16) gObjectList[objectIndex].direction_angle[2];
            func_80043328(gObjectList[objectIndex].pos, D_80183E80, gObjectList[objectIndex].sizeScaling,
                          (Gfx*)d_course_luigi_raceway_dl_FBE0);
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_luigi_raceway_dl_FA20);
            if (gPlayerCountSelection1 == 1) {
                gObjectList[objectIndex].direction_angle[1] = 0;
            }
        }
    }
}

void OHotAirBalloon::init_hot_air_balloon(s32 objectIndex) {
    gObjectList[objectIndex].sizeScaling = 1.0f;
    gObjectList[objectIndex].model = (Gfx*)d_course_luigi_raceway_dl_F960;
    if (gGamestate != CREDITS_SEQUENCE) {
        set_obj_origin_pos(objectIndex, xOrientation * SpawnPos.x, SpawnPos.y, SpawnPos.z);
        set_obj_origin_offset(objectIndex, 0.0f, 300.0f, 0.0f);
    } else {
        set_obj_origin_pos(objectIndex, xOrientation * SpawnPos.x, SpawnPos.y, SpawnPos.z);
        set_obj_origin_offset(objectIndex, 0.0f, 300.0f, 0.0f);
    }
    func_8008B844(objectIndex);
    func_800886F4(objectIndex);
    func_80086EF0(objectIndex);
    gObjectList[objectIndex].velocity[1] = -2.0f;
    init_actor_hot_air_balloon_item_box(0.0f, 0.0f, 0.0f);
    object_next_state(objectIndex);
}

void OHotAirBalloon::func_80085534(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            if (gObjectList[objectIndex].offset[1] <= 18.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
            if (gObjectList[objectIndex].velocity[1] == 0.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 3:
            func_800871AC(objectIndex, 1);
            break;
        case 4:
            f32_step_towards(&gObjectList[objectIndex].velocity[1], 1.0f, 0.05f);
            if (gObjectList[objectIndex].velocity[1] == 1.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            func_800871AC(objectIndex, 90);
            break;
        case 6:
            f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
            if (gObjectList[objectIndex].velocity[1] == 0.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 7:
            f32_step_towards(&gObjectList[objectIndex].velocity[1], -1.0f, 0.05f);
            if (gObjectList[objectIndex].velocity[1] == -1.0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 8:
            func_800871AC(objectIndex, 90);
            break;
        case 9:
            f32_step_towards(&gObjectList[objectIndex].velocity[1], 0.0f, 0.05f);
            if (func_80087060(objectIndex, 90) != 0) {
                func_8008701C(objectIndex, 3);
            }
            break;
    }
    object_add_velocity_offset_y(objectIndex);
    gObjectList[objectIndex].direction_angle[1] += 0x100;
}

void OHotAirBalloon::func_80085768(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 1:
            OHotAirBalloon::init_hot_air_balloon(objectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}
