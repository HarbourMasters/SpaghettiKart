#include "Boos.h"
#include "World.h"
#include "CoreMath.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/textures/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/models/common_data.h"
#include "assets/textures/boo_frames.h"
#include "assets/textures/other_textures.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "menus.h"
#include "race_logic.h"
#include "external.h"
}

size_t OBoos::_count = 0;

OBoos::OBoos(const SpawnParams& params) : OObject(params) {
    Name = "Boos";
    ResourceName = "mk:boos";

    size_t numBoos = params.Count.value_or(5);

    ActiveZone = params.TriggerSpan.value_or(IPathSpan(30, 50));
    LeftTrigger = params.LeftExitSpan.value_or(IPathSpan(0, 10));
    RightTrigger = params.RightExitSpan.value_or(IPathSpan(80, 100));

    // Max five boos allowed due to limited splines
    // D_800E5D9C
    if (numBoos > 10) {
        printf("[Boos.cpp] Only 10 boos allowed.\n");
        numBoos = 10;
    }

    _indices.resize(numBoos, 0);

    // Generate objects
    for (size_t i = 0; i < numBoos; i++) {
        find_unused_obj_index(&_indices[i]);
    }

    _numBoos = numBoos;
}

void OBoos::SetSpawnParams(SpawnParams& params) {
    OObject::SetSpawnParams(params);
    params.Count = _numBoos;
    params.LeftExitSpan = LeftTrigger;
    params.TriggerSpan = ActiveZone;
    params.RightExitSpan = RightTrigger;
}

void OBoos::Tick() {
    u16 temp_t4;
    s32 objectIndex;
    Player* player;
    Camera* camera;
    Object* object;

    OBoos::func_8007CA70();
    for (size_t i = 0; i < _numBoos; i++) {
        objectIndex = _indices[i]; // indexObjectList3[i];
        object = &gObjectList[objectIndex];
        if (object->state != 0) {
            func_8007C684(objectIndex);
            func_8008B78C(objectIndex);
            player = &gPlayerOne[object->unk_0D1];
            camera = &camera1[object->unk_0D1];
            temp_t4 = (0x8000 - camera->rot[1]);
            object->pos[0] = player->pos[0] + (coss(temp_t4) * (object->origin_pos[0] + object->offset[0])) -
                             (sins(temp_t4) * (object->origin_pos[2] + object->offset[2]));
            object->pos[1] = 6.5 + player->unk_074 + object->origin_pos[1] + object->offset[1];
            object->pos[2] = player->pos[2] + (sins(temp_t4) * (object->origin_pos[0] + object->offset[0])) +
                             (coss(temp_t4) * (object->origin_pos[2] + object->offset[2]));
            OBoos::func_8007C550(objectIndex);
            if (is_obj_flag_status_active(objectIndex, 0x00000080) != 0) {
                object->vertex = D_800E44B0;
            } else {
                object->vertex = D_800E4470;
            }
        }
    }
}

void OBoos::Draw(s32 cameraId) {
    u32 temp_s2;
    s32 objectIndex;

    for (size_t i = 0; i < _numBoos; i++) {
        objectIndex = _indices[i]; // indexObjectList3[i];
        if (gObjectList[objectIndex].state >= 2) {
            temp_s2 = func_8008A364(objectIndex, cameraId, 0x4000U, 0x00000320);
            if (CVarGetInteger("gNoCulling", 0) == 1) {
                temp_s2 = MIN(temp_s2, 0x15F91U);
            }

            // @port: Tag the transform.
            FrameInterpolation_RecordOpenChild("Boo", (uintptr_t)&gObjectList[objectIndex]);
            
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                func_800523B8(objectIndex, cameraId, temp_s2);
            }

            // @port Pop the transform id.
            FrameInterpolation_RecordCloseChild();
        }
    }
}

void OBoos::func_800523B8(s32 objectIndex, s32 arg1, u32 arg2) {
    UNUSED s32 pad[2];
    Object* object;
    Camera* camera = &camera1[arg1];

    object = &gObjectList[objectIndex];
    object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
    func_800484BC(object->pos, object->orientation, object->sizeScaling, object->primAlpha, (u8*) object->activeTLUT,
                  (u8*) object->activeTexture, object->vertex, 0x00000030, 0x00000028, 0x00000030, 0x00000028);
    if ((is_obj_flag_status_active(objectIndex, 0x00000020) != 0) && (arg2 < 0x15F91U)) {
        func_8004A630(&D_8018C830, object->pos, 0.4f);
    }
}

void OBoos::func_8007CA70(void) {
    // wtf is up with this pointer? Its such a dumb way of going about things
    u16* point;

    if (_isActive == false) {
        _playerId = OBoos::func_8007C9F8();
        point = &gNearestPathPointByPlayerId[_playerId];

        if ((*point > ActiveZone.Start) && (*point < ActiveZone.End)) {
            // First group entrance
            OBoos::BooStart(0, _playerId);
        }
    }
    if (_isActive == true) {
        point = &gNearestPathPointByPlayerId[_playerId];

        // Left boundary
        if ((*point > LeftTrigger.Start) && (*point < LeftTrigger.End)) {
            // First group exit reverse direction
            OBoos::BooExit(0);
        }

        // Right boundary
        if ((*point > RightTrigger.Start) && (*point < RightTrigger.End)) {
            // First group exit
            OBoos::BooExit(0);
        }
    }
}

void OBoos::func_8007C5B4(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, (u8*) d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 48, 40);
    object = &gObjectList[objectIndex];
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    set_object_flag(objectIndex, 0x00000020);
    object_next_state(objectIndex);
    object->primAlpha = 0;
    func_80073844(objectIndex);
    object->sizeScaling = 0.15f;
    object->unk_034 = 1.0f;
    func_80073FD4(objectIndex);
    func_80086EF0(objectIndex);
    set_object_flag(objectIndex, 0x00000800);
    object->orientation[0] = 0;
    object->orientation[2] = 0x8000;
}

void OBoos::func_8007C684(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OBoos::func_8007C5B4(objectIndex);
            break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        switch (gObjectList[objectIndex].unk_0DC) {
            case 0:
                break;
            case 1:
                func_80073998(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 0x00000050, 2, 1, 0);
                break;
            case 2:
                func_80073CB0(objectIndex, &gObjectList[objectIndex].primAlpha, 0x00000050, 0x00000078, 1, 0, -1);
                break;
            case 3:
                func_80073DC0(objectIndex, &gObjectList[objectIndex].primAlpha, 0, 2);
                break;
            case 4:
                func_80072428(objectIndex);
                func_80086F60(objectIndex);
                func_80073884(objectIndex);
                break;
        }
        func_80073514(objectIndex);
    }
}

void OBoos::func_8007C4A4(s32 objectIndex) {
    u16 var_t9;

    var_t9 = gObjectList[objectIndex].direction_angle[1] * 0x24 / 0x10000;

    if (var_t9 < 0x13) {
        clear_object_flag(objectIndex, 0x80);
        gObjectList[objectIndex].textureListIndex = var_t9;
    } else {
        set_object_flag(objectIndex, 0x80);
        gObjectList[objectIndex].textureListIndex = 0x24 - var_t9;
    }
}

void OBoos::func_8007C550(s32 objectIndex) {
    gObjectList[objectIndex].direction_angle[1] =
        func_800417B4(gObjectList[objectIndex].direction_angle[1],
                      atan2s(gObjectList[objectIndex].velocity[0], gObjectList[objectIndex].velocity[2]));
    OBoos::func_8007C4A4(objectIndex);
}

s32 OBoos::func_8007C9F8(void) {
    s32 ret;
    s32 first, second;
    if (gPlayerCount == 1) {
        ret = 0;
    } else {
        if (gPlayerCount == 2) {
            if (gModeSelection == 0) {
                first = gGPCurrentRaceRankByPlayerId[0];
                second = gGPCurrentRaceRankByPlayerId[1];
                if (first < second) {
                    ret = 0;
                } else {
                    ret = 1;
                }
            } else {
                ret = gGPCurrentRacePlayerIdByRank[0];
            }
        } else {
            ret = gGPCurrentRacePlayerIdByRank[0];
        }
    }
    return ret;
}

void OBoos::BooStart(s32 group, s32 playerId) {
    s32 objectIndex;
    s16 temp_s1_2;
    s16 temp_s4;
    s16 temp_s5;
    SplineData* spline;
    for (size_t i = 0; i < _numBoos; i++) {
        objectIndex = _indices[i]; // indexObjectList3[group + i];
        init_object(objectIndex, 1);
        gObjectList[objectIndex].unk_0D1 = playerId;
        temp_s1_2 = random_int(0x003CU) - 0x1E;
        temp_s4 = random_int(0x0014U) - 0xA;
        temp_s5 = random_int(0x0050U) - 0x28;
        random_int(0x1000U);
        spline = D_800E5D9C[i];
        gObjectList[objectIndex].spline = spline;
        gObjectList[objectIndex].origin_pos[0] = (f32) temp_s1_2;
        gObjectList[objectIndex].origin_pos[1] = (f32) temp_s4;
        gObjectList[objectIndex].origin_pos[2] = (f32) temp_s5;
    }
    func_800C9060(playerId, 0x1900705AU);

    _isActive = true;
}

void OBoos::BooExit(s32 group) {
    s32 objectIndex;
    for (size_t i = 0; i < _numBoos; i++) {
        objectIndex = _indices[i]; // indexObjectList3[group + temp_a0];
        gObjectList[objectIndex].unk_0DC += 1;
    }

    _isActive = false;
}

void OBoos::DrawEditorProperties() {
    ImGui::Text("Num Boos");
    ImGui::SameLine();

    int count = static_cast<int>(_count);
    if (ImGui::InputInt("##Count", &count)) {
        // Clamp to uint32_t range (only lower bound needed if assuming positive values)
        if (count < 0) count = 0;
        if (count > 10) count = 10;
        _count = static_cast<uint32_t>(count);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetCount")) {
        _count = 5;
    }

    ImGui::Text("Left Exit Span");
    ImGui::SameLine();

    if (ImGui::DragInt2("##LeftExitSpan", (int*)&LeftTrigger)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetLeftExitSpan")) {
        LeftTrigger = IPathSpan(0, 0);
    }

    ImGui::Text("Trigger Span");
    ImGui::SameLine();

    if (ImGui::DragInt2("##TriggerSpan", (int*)&ActiveZone)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetTriggerSpan")) {
        ActiveZone = IPathSpan(0, 0);
    }

    ImGui::Text("Right Exit Span");
    ImGui::SameLine();

    if (ImGui::DragInt2("##RightExitSpan", (int*)&RightTrigger)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetRightExitSpan")) {
        RightTrigger = IPathSpan(0, 0);
    }
}
