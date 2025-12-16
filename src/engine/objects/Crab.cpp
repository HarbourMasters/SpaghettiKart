#include <libultraship.h>
#include <libultra/gbi.h>
#include "Crab.h"
#include <vector>
#include "CoreMath.h"

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
#include "assets/models/tracks/koopa_troopa_beach/koopa_troopa_beach_data.h"
#include "assets/textures/tracks/koopa_troopa_beach/koopa_troopa_beach_data.h"
#include "assets/models/common_data.h"
}

size_t OCrab::_count = 0;

OCrab::OCrab(const SpawnParams& params) : OObject(params) {
    Name = "Crab";
    ResourceName = "mk:crab";
    _idx = _count;
    _start = params.PatrolStart.value_or(FVector2D(0, 0));
    _end = params.PatrolEnd.value_or(FVector2D(0, 0));

    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);
    gObjectList[_objectIndex].pos[0] = gObjectList[_objectIndex].origin_pos[0] = _start.x * xOrientation;
    gObjectList[_objectIndex].pos[2] = gObjectList[_objectIndex].origin_pos[2] = _start.z;

    gObjectList[_objectIndex].unk_01C[0] = _end.x * xOrientation;
    gObjectList[_objectIndex].unk_01C[2] = _end.z;

    _count++;
}

void OCrab::SetSpawnParams(SpawnParams& params) {
    params.Name = std::string(ResourceName);
    params.PatrolStart = _start;
    params.PatrolEnd = _end;
}

void OCrab::Tick(void) {
    s32 objectIndex = _objectIndex;
    if (gObjectList[objectIndex].state != 0) {
        OCrab::func_80082B34(objectIndex);
        func_8008A6DC(objectIndex, 500.0f);
        OCrab::func_80082C30(objectIndex);
        OCrab::func_80082E18(objectIndex);
    }
}

Vtx common_vtx_crab[] = {
    { { { -32, -31, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, -31, 0 }, 0, { 4032, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, 31, 0 }, 0, { 4032, 3968 }, { 255, 255, 255, 255 } } },
    { { { -32, 31, 0 }, 0, { 0, 3968 }, { 255, 255, 255, 255 } } },
};

void OCrab::Draw(s32 cameraId) {
    Camera* camera;
    s32 objectIndex = _objectIndex;
    if (gObjectList[objectIndex].state >= 2) {
        camera = &camera1[cameraId];
        func_8004A6EC(objectIndex, 0.5f);
        gObjectList[objectIndex].orientation[1] =
            func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
        rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                                      gObjectList[objectIndex].sizeScaling);
        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
        gDPLoadTLUT_pal256(gDisplayListHead++, gObjectList[objectIndex].activeTLUT);
        rsp_load_texture((u8*) gObjectList[objectIndex].activeTexture, 64, 64);
        gSPVertex(gDisplayListHead++, (uintptr_t) common_vtx_crab, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
        gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    }
}

void OCrab::DrawModel(s32 cameraId) {
    s32 someIndex;
    s32 objectIndex = _objectIndex;
    func_8008A364(objectIndex, cameraId, 0x2AABU, 800);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        Camera* camera;
        s32 objectIndex;

        if (gObjectList[objectIndex].state >= 2) {
            camera = &camera1[cameraId];
            func_8004A6EC(objectIndex, 0.5f);
            gObjectList[objectIndex].orientation[1] =
                func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
            rsp_set_matrix_transformation(gObjectList[objectIndex].pos, gObjectList[objectIndex].orientation,
                                          gObjectList[objectIndex].sizeScaling);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007D78);
            gDPLoadTLUT_pal256(gDisplayListHead++, gObjectList[objectIndex].activeTLUT);
            rsp_load_texture((u8*) gObjectList[objectIndex].activeTexture, 64, 64);
            gSPVertex(gDisplayListHead++, (uintptr_t) common_vtx_crab, 4, 0);
            gSPDisplayList(gDisplayListHead++, (Gfx*) common_rectangle_display);
            gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
        }
    }
}

void OCrab::init_ktb_crab(s32 objectIndex) {
    Object* object;

    init_texture_object(objectIndex, (uint8_t*) d_course_koopa_troopa_beach_crab_tlut,
                        (const char**) d_course_koopa_troopa_beach_crab_frames, 64, (u16) 64);
    object = &gObjectList[objectIndex];
    object->sizeScaling = 0.15f;
    object->textureListIndex = 0;
    object_next_state(objectIndex);
    object->boundingBoxSize = 1;
    set_object_flag(objectIndex, 0x04000420);
    func_80086EAC(objectIndex, 0, 1);
    set_obj_origin_offset(objectIndex, 0.0f, 0.0f, 0.0f);
    set_obj_orientation(objectIndex, 0U, 0U, 0x8000U);
    object->unk_034 = 1.5f;
    set_object_flag(objectIndex, 0x00000200);
}

void OCrab::func_80082B34(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) { /* irregular */
        case 0:
            break;
        case 1:
            OCrab::init_ktb_crab(objectIndex);
            break;
        case 2:
            func_80072E54(objectIndex, 0, 3, 1, 2, -1);
            break;
        case 3:
            func_80072E54(objectIndex, 4, 6, 1, 2, -1);
            break;
    }
    if (gObjectList[objectIndex].state >= 2) {
        func_80073514(objectIndex);
    }
}

void OCrab::func_80082C30(s32 objectIndex) {
    switch (gObjectList[objectIndex].unk_0AE) {
        case 1:
            if (func_80087A0C(objectIndex, gObjectList[objectIndex].origin_pos[0], gObjectList[objectIndex].unk_01C[0],
                              gObjectList[objectIndex].origin_pos[2], gObjectList[objectIndex].unk_01C[2]) != 0) {
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            if (func_80087104(objectIndex, 0x003CU) != 0) {
                gObjectList[objectIndex].unk_034 = 0.8f;
                func_800726CC(objectIndex, 2);
                func_80086FD4(objectIndex);
            }
            break;
        case 3:
            if (func_80087954(objectIndex, 0x0000003C) != 0) {
                func_80086FD4(objectIndex);
                func_800726CC(objectIndex, 3);
            }
            break;
        case 4:
            if (func_80087104(objectIndex, 0x003CU) != 0) {
                func_800726CC(objectIndex, 2);
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            if (func_8008789C(objectIndex, 0x0000003C) != 0) {
                func_800726CC(objectIndex, 3);
                func_8008701C(objectIndex, 2);
            }
            break;
    }
    object_calculate_new_pos_offset(objectIndex);
    if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
        func_80088538(objectIndex);
        gObjectList[objectIndex].pos[1] = (f32) (gObjectList[objectIndex].surfaceHeight + 2.5);
    }
}

void OCrab::func_80082E18(s32 objectIndex) {
    if (gObjectList[objectIndex].state >= 2) {
        func_80089F24(objectIndex);
    }
}

void OCrab::DrawEditorProperties() {
    ImGui::Text("Start Location");
    ImGui::SameLine();

    if (ImGui::DragFloat2("##PathSpan", (float*)&_start)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPathSpan")) {
        _start = FVector2D(0.0f, 0.0f);
        gObjectList[_objectIndex].pos[0] = gObjectList[_objectIndex].origin_pos[0] = _start.x * xOrientation;
        gObjectList[_objectIndex].pos[2] = gObjectList[_objectIndex].origin_pos[2] = _start.z;
    }

    ImGui::Text("Patrol Location");
    ImGui::SameLine();

    if (ImGui::DragFloat2("##PatrolLoc", (float*)&_end)) {
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPatrolLoc")) {
        _end = FVector2D(0.0f, 0.0f);
        gObjectList[_objectIndex].unk_01C[0] = _end.x * xOrientation;
        gObjectList[_objectIndex].unk_01C[2] = _end.z;
    }
}
