#include "CheepCheep.h"
#include "port/Game.h"

#include "assets/models/tracks/banshee_boardwalk/banshee_boardwalk_data.h"
#include "assets/models/common_data.h"


extern "C" {
#include "math_util.h"
#include "math_util_2.h"
#include "render_objects.h"
#include "update_objects.h"
#include "code_800029B0.h"
#include "code_80086E70.h"
#include "waypoints.h"
#include "code_80057C60.h"
#include "some_data.h"
extern Vec3s D_800E634C[];
extern Lights1 D_800E45C0[];
}

OCheepCheep::OCheepCheep(const SpawnParams& params) : OObject(params) {
    Name = "Cheep Cheep";
    ResourceName = "mk:cheep_cheep";
    _behaviour = static_cast<Behaviour>(params.Behaviour.value_or(0));
}

void OCheepCheep::SetSpawnParams(SpawnParams& params) {
    OObject::SetSpawnParams(params);
    params.Behaviour = static_cast<int16_t>(_behaviour);
    params.PathSpan = ActivationPoints;
}

void OCheepCheep::Tick() { // update_cheep_cheep
    s32 objectIndex;
    switch (_behaviour) {
        case Behaviour::RACE:
            UNUSED s32 pad;

            OCheepCheep::func_8007BD04(0);
            objectIndex = indexObjectList2[0];
            OCheepCheep::func_8007BBBC(objectIndex);
            object_calculate_new_pos_offset(objectIndex);
            break;
        case Behaviour::PODIUM_CEREMONY:
            objectIndex = indexObjectList2[0];
            if (D_801658BC == 1) {
                D_801658BC = 0;
                init_object(objectIndex, 0);
            }
            if (gObjectList[objectIndex].state != 0) {
                OCheepCheep::func_8007BEC8(objectIndex);
                OCheepCheep::func_8007BFB0(objectIndex);
            }
            break;
    }
}

void OCheepCheep::Draw(s32 cameraId) { // func_8005217C
    Lights1* D_800E45C0l = (Lights1*) (D_800E45C0);
    Object* object;
    s32 temp_a3;
    temp_a3 = indexObjectList2[0];
    object = &gObjectList[temp_a3];
    if (object->state >= 2) {
        if (is_obj_flag_status_active(temp_a3, 0x10) != 0) {
            rsp_set_matrix_transformation(object->pos, object->direction_angle, object->sizeScaling);
            func_800520C0(temp_a3);

            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007828);
            gSPLight(gDisplayListHead++, &D_800E45C0l[0].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E45C0l[0].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_dl_7B38);
            gSPLight(gDisplayListHead++, &D_800E45C0l[1].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E45C0l[1].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_dl_7978);
            gSPLight(gDisplayListHead++, &D_800E45C0l[2].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E45C0l[2].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_dl_78C0);
            gSPLight(gDisplayListHead++, &D_800E45C0l[3].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E45C0l[3].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, (Gfx*) d_course_banshee_boardwalk_dl_7650);
        }
    }
}

void OCheepCheep::func_8007BBBC(s32 objectIndex) {
    f32 var_f14;
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 1:
            func_800735BC(objectIndex, (Gfx*) d_course_banshee_boardwalk_dl_cheep_cheep, 2.0f);
            set_object_flag(objectIndex, 0x00000010);
            object->unk_0D5 = 0;
            break;
        case 2:
            if (gIsMirrorMode != 0) {
                func_80087E08(objectIndex, 18.0f, 0.7f, 25.0f, (s16) -0x00005800, 0x0000012C);
            } else {
                func_80087E08(objectIndex, 18.0f, 0.7f, 25.0f, (s16) 0x00005800, 0x0000012C);
            }
            if (object->velocity[2] < 0.0f) {
                var_f14 = -object->velocity[2];
            } else {
                var_f14 = object->velocity[2];
            }
            object->direction_angle[0] = func_80041658(object->velocity[1], var_f14);
            set_and_run_timer_object(objectIndex, 0x00000046);
            break;
        case 3:
            func_80072428(objectIndex);
            break;
        case 0:
            break;
    }
}

void OCheepCheep::func_8007BD04(s32 playerId) {
    s32 objectIndex;

    objectIndex = indexObjectList2[0];
    if (gObjectList[objectIndex].state == 0) {
        IPathSpan span = ActivationPoints;
        if (((s32) gNearestPathPointByPlayerId[playerId] >= span.Start) &&
            ((s32) gNearestPathPointByPlayerId[playerId] <= span.End)) {
            FVector pos = SpawnPos;
            set_obj_origin_pos(objectIndex, xOrientation * pos.x, pos.y, pos.z);
            init_object(objectIndex, 1);
        }
    }
}

void OCheepCheep::init_var_cheep_cheep(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D5 = 1;
    object->status = 0;
    object->model = (Gfx*) d_course_banshee_boardwalk_dl_cheep_cheep;
    object->sizeScaling = 0.2f;
    object_next_state(objectIndex);
    set_obj_origin_pos(objectIndex, D_800E634C[0][0], D_800E634C[0][1] + 55.0, D_800E634C[0][2]);
    set_obj_origin_offset(objectIndex, 0.0f, 30.0f, 0.0f);
    set_obj_direction_angle(objectIndex, 0U, 0x3800U, 0U);
}

void OCheepCheep::func_8007BEC8(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 1:
            OCheepCheep::init_var_cheep_cheep(objectIndex);
            break;
        case 2:
            if (set_and_run_timer_object(objectIndex, 0x0000003C) != 0) {
                set_object_flag(objectIndex, 0x00000010);
                func_80086E70(objectIndex);
            }
            break;
        case 3:
            if (object->unk_0AE == 0) {
                object_next_state(objectIndex);
            }
            break;
        case 4:
            if (set_and_run_timer_object(objectIndex, 0x0000000A) != 0) {
                func_8008701C(objectIndex, 2);
            }
            break;
        case 5:
            if (object->unk_0AE == 0) {
                func_80072428(objectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void OCheepCheep::func_8007BFB0(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->unk_0AE) {
        case 0:
            break;
        case 1:
            object->velocity[1] = -0.2f;
            if ((f64) object->offset[1] <= 0.0) {
                object->offset[1] = 0.0f;
                object->velocity[1] = 0.0f;
                func_80086F60(objectIndex);
            }
            break;
        case 2:
            if (func_800871AC(objectIndex, 0x00000014) != 0) {
                object->unk_084[7] = 0x0040;
            }
            break;
        case 3:
            object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.0015);
            if ((s32) object->direction_angle[0] >= 0xA01) {
                object->unk_084[7] -= 4;
            }
            if (u16_step_up_towards(object->direction_angle, 0x0C00U, (u16) object->unk_084[7]) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 4:
            object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.0015);
            object->unk_034 = 0.001f;
            func_80086FD4(objectIndex);
            object->unk_084[7] = 0;
            break;
        case 5:
            if (object->unk_034 <= 0.004) {
                object->unk_034 += 0.0002;
            }
            object->sizeScaling += object->unk_034;
            s16_step_up_towards(&object->unk_084[7], 0x0100, 0x0010);
            object->direction_angle[0] -= object->unk_084[7];
            if (func_80087060(objectIndex, 0x00000035) != 0) {
                func_80086FD4(objectIndex);
            }
            break;
        case 6:
            if (func_80087060(objectIndex, 0x0000000F) != 0) {
                func_80086FD4(objectIndex);
                D_801658CE = 1;
            }
            break;
        case 7:
            object->sizeScaling = (f32) ((f64) object->sizeScaling - 0.05);
            if ((f64) object->sizeScaling <= 0.01) {
                clear_object_flag(objectIndex, 0x00000010);
                object->sizeScaling = 0.000001f;
                func_80086FD4(objectIndex);
            }
            break;
        case 8:
            func_80086F60(objectIndex);
            break;
    }
    if (object->unk_0AE < 0xA) {
        func_80074344(objectIndex, &object->sizeScaling, 0.2f, 0.21f, 0.001f, 0, -1);
    }
    object_add_velocity_offset_y(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void OCheepCheep::DrawEditorProperties() {
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

    IPathSpan span = ActivationPoints;

    ImGui::Text("Path Span");
    ImGui::SameLine();

    if (ImGui::DragInt2("##PathSpan", (int*)&span)) {
        ActivationPoints = span;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetPathSpan")) {
        ActivationPoints = IPathSpan(0.0f, 0.0f);
    }
}
