#include "Hedgehog.h"
#include "engine/World.h"
#include "port/Game.h"
#include "port/interpolation/FrameInterpolation.h"

extern "C" {
#include "render_objects.h"
#include "update_objects.h"
#include "assets/models/tracks/yoshi_valley/yoshi_valley_data.h"
#include "assets/textures/tracks/yoshi_valley/yoshi_valley_data.h"
#include "assets/models/common_data.h"
#include "math_util.h"
#include "math_util_2.h"
#include "code_80086E70.h"
#include "code_80057C60.h"
}

size_t OHedgehog::_count = 0;

OHedgehog::OHedgehog(const SpawnParams& params) : OObject(params) {
    Name = "Hedgehog";
    ResourceName = "mk:hedgehog";
    _idx = _count;
    SpawnPos = params.Location.value_or(FVector(0, 0, 0));
    PatrolEnd = params.PatrolEnd.value_or(FVector2D(0, 0));

    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);
    gObjectList[_objectIndex].pos[0] = gObjectList[_objectIndex].origin_pos[0] = SpawnPos.x * xOrientation;
    gObjectList[_objectIndex].pos[1] = gObjectList[_objectIndex].surfaceHeight = SpawnPos.y + 6.0;
    gObjectList[_objectIndex].pos[2] = gObjectList[_objectIndex].origin_pos[2] = SpawnPos.z;
    gObjectList[_objectIndex].unk_0D5 = (u8) params.Behaviour.value_or(9);
    gObjectList[_objectIndex].unk_09C = PatrolEnd.x * xOrientation;
    gObjectList[_objectIndex].unk_09E = PatrolEnd.z;

    _count++;
}

void OHedgehog::SetSpawnParams(SpawnParams& params) {
    params.Name = std::string(ResourceName);
    params.Location = SpawnPos;
    params.PatrolEnd = PatrolEnd;
}

void OHedgehog::Tick() {
    OHedgehog::func_800833D0(_objectIndex, _idx);
    OHedgehog::func_80083248(_objectIndex);
    OHedgehog::func_80083474(_objectIndex);

    // This func clears a bit from all hedgehogs. This results in setting the height of all hedgehogs to zero.
    // The solution is to only clear the bit from the current instance; `self` or `this`
    // func_80072120(indexObjectList2, NUM_HEDGEHOGS);
    clear_object_flag(_objectIndex, 0x00600000); // The fix
}

void OHedgehog::Draw(s32 cameraId) {
    u32 something = func_8008A364(_objectIndex, cameraId, 0x4000U, 0x000003E8);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        something = MIN(something, 0x52211U - 1);
    }
    if (is_obj_flag_status_active(_objectIndex, VISIBLE) != 0) {
        set_object_flag(_objectIndex, 0x00200000);
        if (something < 0x2711U) {
            set_object_flag(_objectIndex, 0x00000020);
        } else {
            clear_object_flag(_objectIndex, 0x00000020);
        }
        if (something < 0x57E41U) {
            set_object_flag(_objectIndex, 0x00400000);
        }
        if (something < 0x52211U) {
            OHedgehog::func_800555BC(_objectIndex, cameraId);
        }
    }
}

void OHedgehog::func_800555BC(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    if (gObjectList[objectIndex].state >= 2) {
        camera = &camera1[cameraId];
        OHedgehog::func_8004A870(objectIndex, 0.7f);
        gObjectList[objectIndex].orientation[1] =
            func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
        draw_2d_texture_at(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                           gObjectList[objectIndex].sizeScaling, (u8*) gObjectList[objectIndex].activeTLUT,
                           (u8*) gObjectList[objectIndex].activeTexture, gObjectList[objectIndex].vertex, 64, 64, 64,
                           32);
    }
}

void OHedgehog::func_8004A870(s32 objectIndex, f32 arg1) {
    Mat4 mtx;
    Object* object;

    if ((is_obj_flag_status_active(objectIndex, 0x00000020) != 0) &&
        (is_obj_flag_status_active(objectIndex, 0x00800000) != 0)) {
        object = &gObjectList[objectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->surfaceHeight + 0.8;
        D_80183E50[2] = object->pos[2];

        // @port: Tag the transform.
        FrameInterpolation_RecordOpenChild("hedgehog", (uintptr_t) &gObjectList[objectIndex]);

        set_transform_matrix(mtx, object->unk_01C, D_80183E50, 0U, arg1);
        // convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], mtx);
        // gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
        //           G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        AddHudMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007B98);

        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
    }
}

const char* sHedgehogTexList[] = { d_course_yoshi_valley_hedgehog };

void OHedgehog::func_8008311C(s32 objectIndex, s32 id) {
    Object* object;
    Vtx* vtx = (Vtx*) LOAD_ASSET_RAW(common_vtx_hedgehog);

    init_texture_object(objectIndex, (u8*) d_course_yoshi_valley_hedgehog_tlut, sHedgehogTexList, 0x40U,
                        (u16) 0x00000040);
    object = &gObjectList[objectIndex];
    object->activeTLUT = d_course_yoshi_valley_hedgehog_tlut;
    object->activeTexture = d_course_yoshi_valley_hedgehog;
    object->vertex = vtx;
    object->sizeScaling = 0.2f;
    object->textureListIndex = 0;
    object_next_state(objectIndex);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    object->unk_034 = ((id % 6) * 0.1) + 0.5;
    func_80086E70(objectIndex);
    set_object_flag(objectIndex, 0x04000600);
    object->boundingBoxSize = 2;
}

void OHedgehog::func_80083248(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 0:
            break;
        case 1:
            if (func_80087A0C(objectIndex, gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_09C,
                              gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_09E) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            func_800871AC(objectIndex, 0x0000003C);
            break;
        case 3:
            if (func_80087A0C(objectIndex, gObjectList[objectIndex].unk_09C, gObjectList[objectIndex].origin_pos[0],
                              gObjectList[objectIndex].unk_09E, gObjectList[objectIndex].origin_pos[2]) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 4:
            if (func_80087060(objectIndex, 0x0000003C) != 0) {
                func_8008701C(objectIndex, 1);
            }
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    if (is_obj_flag_status_active(objectIndex, 0x00200000) != 0) {
        if (is_obj_flag_status_active(objectIndex, 0x00400000) != 0) {
            func_8008861C(objectIndex);
        }
        gObjectList[objectIndex].pos[1] = gObjectList[objectIndex].surfaceHeight + 6.0;
    }
}

void OHedgehog::func_800833D0(s32 objectIndex, s32 id) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OHedgehog::func_8008311C(objectIndex, id);
            break;
        case 2:
            func_80072D3C(objectIndex, 0, 1, 4, -1);
            break;
    }
    if (gObjectList[objectIndex].textureListIndex == 0) {
        Vtx* vtx = (Vtx*) LOAD_ASSET_RAW(common_vtx_hedgehog);
        gObjectList[objectIndex].vertex = vtx;
    } else {
        Vtx* vtx = (Vtx*) LOAD_ASSET_RAW(D_0D006130);
        gObjectList[objectIndex].vertex = vtx;
    }
}

void OHedgehog::func_80083474(s32 objectIndex) {
    if (gObjectList[objectIndex].state >= 2) {
        func_80089F24(objectIndex);
    }
}

void OHedgehog::DrawEditorProperties() {
    Object* obj = &gObjectList[_objectIndex];

    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = FVector(obj->pos[0], obj->pos[1], obj->pos[2]);
    if (ImGui::DragFloat3("##Location", (float*)&location)) {
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPos")) {
        FVector location = FVector(0, 0, 0);
        Translate(location);
        gEditor.eObjectPicker.eGizmo.Pos = location;
    }

    ImGui::Text("Patrol Location");
    ImGui::SameLine();

    if (ImGui::DragFloat2("##PatrolLoc", (float*)&PatrolEnd)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPatrolLoc")) {
        PatrolEnd = FVector2D(0.0f, 0.0f);
    }
}