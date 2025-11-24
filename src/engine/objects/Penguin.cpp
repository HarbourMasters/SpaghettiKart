#include <libultraship.h>
#include <libultra/gbi.h>
#include "Penguin.h"
#include <vector>

#include "port/Game.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "actors.h"
#include "math_util.h"
#include "sounds.h"
#include "update_objects.h"
#include "render_player.h"
#include "external.h"
#include "bomb_kart.h"
#include "collision.h"
#include "code_80086E70.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "math_util_2.h"
#include "collision.h"
#include "assets/bowsers_castle_data.h"
#include "ceremony_and_credits.h"
#include "objects.h"
#include "update_objects.h"
#include "render_objects.h"
#include "course_offsets.h"
#include "data/some_data.h"
extern s8 gPlayerCount;
}

OPenguin::OPenguin(const SpawnParams& params) : OObject(params) {
    Name = "Penguin";
    ResourceName = "mk:penguin";
    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    Speed = params.Speed.value_or(0);
    find_unused_obj_index(&_objectIndex);

    init_object(_objectIndex, 0);

    Object *object = &gObjectList[_objectIndex];
    object->origin_pos[0] = pos.x * xOrientation;
    object->origin_pos[1] = pos.y;
    object->origin_pos[2] = pos.z;
    object->unk_0C6 = params.Rotation.value_or(IRotator(0, 0, 0)).yaw;

    Type = static_cast<PenguinType>(params.Type.value_or(0));
    SpawnBhv = static_cast<OPenguin::Behaviour>(params.Behaviour.value_or(0));

    switch(Type) {
        case PenguinType::CHICK:
            object->surfaceHeight = 5.0f;
            object->sizeScaling = 0.04f;
            object->boundingBoxSize = 4;
            break;
        case PenguinType::ADULT:
            object->surfaceHeight = -80.0f;
            object->sizeScaling = 0.08f;
            object->boundingBoxSize = 4;
            break;
        case PenguinType::CREDITS:
            object->surfaceHeight = -80.0f;
            object->sizeScaling = 0.08f;
            object->sizeScaling = 0.15f;
            break;
        case PenguinType::EMPEROR:
            object->sizeScaling = 0.2f;
            object->boundingBoxSize = 0x000C;
            break;
    }
}

void OPenguin::Tick(void) {
    s32 objectIndex = _objectIndex;

    if (gObjectList[objectIndex].state != 0) {
        if (Type == PenguinType::EMPEROR) {
            OPenguin::EmperorPenguin(objectIndex);
        } else {
            OPenguin::OtherPenguin(objectIndex);
        }

        OPenguin::Behaviours(objectIndex);
    }
    if (func_80072320(objectIndex, 1) != 0) {
        func_80089820(objectIndex, 1.75f, 1.5f, 0x1900A046U);
    } else if (func_80072320(objectIndex, 8) != 0) {
        func_80089820(objectIndex, 1.3f, 1.0f, 0x1900A046U);
    } else {
        func_80089820(objectIndex, 1.5f, 1.25f, 0x1900A046U);
    }
    if ((is_obj_flag_status_active(objectIndex, 0x02000000) != 0) &&
        (func_80072354(objectIndex, 0x00000020) != 0)) {
        func_800722A4(objectIndex, 0x00000060);
        clear_object_flag(objectIndex, 0x02000000);
    }
}

void OPenguin::Draw(s32 cameraId) {
    s32 objectIndex = _objectIndex;
    s32 temp_s1;
    s32 drawDistance;
    u16 var_s1;
    u32 var_s3;

    if (gPlayerCountSelection1 == 1) {
        var_s3 = 0x0003D090;
    } else if (gPlayerCountSelection1 == 2) {
        var_s3 = 0x00027100;
    } else {
        var_s3 = 0x00015F90;
    }


    if (gObjectList[objectIndex].state >= 2) {

        // This code decreased draw dist if there's more players.
        // The var_s1 = 0x4000 for everything is just a guess.
        // It seems to check the angle of the camera.
        // These changes were so instance indices are not required.

        //if (gPlayerCountSelection1 == 1) {
            var_s1 = 0x4000;
            drawDistance = 1500;
        //} else {
        //    if (func_80072320(objectIndex, 8) != 0) {
        //        drawDistance = 500;
        //        var_s1 = 0x4000;
        //    } else {
        //        drawDistance = 600;
        //        var_s1 = 0x5555;
        //    }
        //}
        temp_s1 = func_8008A364(objectIndex, cameraId, var_s1, drawDistance);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
            func_800557B4(objectIndex, (u32) temp_s1, var_s3);
        }
    }
}

void OPenguin::Behaviours(s32 objectIndex) { // func_800850B0
    Object* object;

    object = &gObjectList[objectIndex];
    switch (SpawnBhv) {
        case 1: // emperor
            OPenguin::func_80085080(objectIndex);
            break;
        case 2:
            OPenguin::func_8008502C(objectIndex);
            break;
        case 3:
            OPenguin::func_80084D2C(objectIndex, 0);
            break;
        case 4:
            OPenguin::func_80084D2C(objectIndex, 1);
            break;
        case 5:
            OPenguin::func_80084D2C(objectIndex, 2);
            break;
        case 6:
            OPenguin::func_80084D2C(objectIndex, 3);
            break;
    }
    if (func_80072320(objectIndex, 0x00000020) != 0) {
        if (func_80072320(objectIndex, 0x00000040) != 0) {
            func_800722CC(objectIndex, 0x00000040);
            object->unk_084[6] = 0;
            object->unk_084[7] = 0x0096;
        }
        if (object->unk_084[7] == 0) {
            func_800722CC(objectIndex, 0x00000020);
        } else {
            object->unk_084[7]--;
            object->orientation[0] = object->direction_angle[0];
            object->orientation[1] += 0x2000;
            object->orientation[2] = object->direction_angle[2];
        }
    } else {
        object->orientation[0] = object->direction_angle[0];
        object->orientation[1] = object->direction_angle[1];
        object->orientation[2] = object->direction_angle[2];
    }
}

void OPenguin::func_80084D2C(s32 objectIndex, s32 arg1) {
    f32 sp24;

    switch (gObjectList[objectIndex].unk_0AE) {
        case 0:
            break;
        case 1:
            gObjectList[objectIndex].direction_angle[1] =
                func_800417B4(gObjectList[objectIndex].direction_angle[1], gObjectList[objectIndex].unk_0C6);
            if (gObjectList[objectIndex].direction_angle[1] == gObjectList[objectIndex].unk_0C6) {
                gObjectList[objectIndex].unk_09C = 4;
                gObjectList[objectIndex].unk_034 = 0.4f;
                func_80086FD4(objectIndex);
            }
            break;
        case 2:
            f32_step_towards(&gObjectList[objectIndex].unk_034, 0.8f, 0.02f);
            if (func_80087060(objectIndex, 0x0000000F) != 0) {
                func_800722A4(objectIndex, 1);
                func_800722A4(objectIndex, 2);
                gObjectList[objectIndex].unk_09C = 1;
                gObjectList[objectIndex].unk_0D8 = 1;
                gObjectList[objectIndex].textureListIndex = 0;
                gObjectList[objectIndex].type =
                    get_animation_length(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
                if (func_80072354(objectIndex, 0x00000020) != 0) {
                    func_800722A4(objectIndex, 0x00000080);
                }
            }
            break;
        case 3:
            switch (arg1) {
                case 0:
                    sp24 = 1.0f;
                    break;
                case 1:
                    sp24 = 1.5f;
                    break;
                case 2:
                    sp24 = 2.0f;
                    break;
                case 3:
                    sp24 = 2.5f;
                    break;
            }
            f32_step_towards(&gObjectList[objectIndex].unk_034, sp24, 0.15f);
            if ((func_80072354(objectIndex, 2) != 0) && (sp24 == gObjectList[objectIndex].unk_034)) {
                func_80086FD4(objectIndex);
            }
            break;
        case 4:
            if (func_80087060(objectIndex, 0x0000001E) != 0) {
                func_800722CC(objectIndex, 1);
                func_80086FD4(objectIndex);
            }
            break;
        case 5:
            f32_step_towards(&gObjectList[objectIndex].unk_034, 0.4f, 0.2f);
            if (func_80087060(objectIndex, 0x0000000A) != 0) {
                func_800722A4(objectIndex, 2);
                gObjectList[objectIndex].unk_0D8 = 2;
                gObjectList[objectIndex].textureListIndex = 0;
                gObjectList[objectIndex].type =
                    get_animation_length(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
                func_800726CC(objectIndex, 3);
                func_80086FD4(objectIndex);
            }
            break;
        case 6:
            if (func_80072354(objectIndex, 2) != 0) {
                gObjectList[objectIndex].unk_0D8 = 0;
                gObjectList[objectIndex].textureListIndex = 0;
                gObjectList[objectIndex].type =
                    get_animation_length(d_course_sherbet_land_unk_data11, gObjectList[objectIndex].unk_0D8);
                gObjectList[objectIndex].unk_0C6 += 0x8000;
                func_800726CC(objectIndex, 2);
                func_8008701C(objectIndex, 1);
            }
            break;
    }
    func_8008781C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
}

void OPenguin::func_80085080(s32 objectIndex) {
    func_8008B78C(objectIndex);
    object_calculate_new_pos_offset(objectIndex);
    func_800873F4(objectIndex);
}

void OPenguin::func_8008502C(s32 objectIndex) {
    func_80088038(objectIndex, gObjectList[objectIndex].unk_01C[1], gObjectList[objectIndex].unk_0C6);
    object_calculate_new_pos_offset(objectIndex);
    func_800873F4(objectIndex);
}


void OPenguin::EmperorPenguin(s32 objectIndex) {
    switch (gObjectList[objectIndex].state) {
        case 0:
            break;
        case 1:
            OPenguin::InitEmperorPenguin(objectIndex);
            break;
        case 2:
            func_80072E54(objectIndex, 0, gObjectList[objectIndex].type, 1, 0, -1);
            break;
    }
}

void OPenguin::InitEmperorPenguin(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    object->model = (Gfx*) d_course_sherbet_land_unk_data1;
    object->vertex = (Vtx*) d_course_sherbet_land_unk_data11;
    //object->sizeScaling = 0.2f;
    //object->boundingBoxSize = 0x000C;
    object->unk_09C = 1;
    set_obj_direction_angle(objectIndex, 0U, 0U, 0U);
    object->unk_0DD = 1;
    func_80086EF0(objectIndex);
    object->spline = D_800E672C[0];
    set_object_flag(objectIndex, 0x04000800);
    object->type = get_animation_length(d_course_sherbet_land_unk_data11, 0);
    object_next_state(objectIndex);
}

void OPenguin::OtherPenguin(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    switch (object->state) {
        case 0:
            break;
        case 1:
            OPenguin::InitOtherPenguin(objectIndex);
            break;
        case 2:
            func_80072E54(objectIndex, 0, (s32) object->type, (s32) object->unk_09C, 0, -1);
            if (func_80072354(objectIndex, 0x00000020) != 0) {
                if (object->unk_084[6] == 0) {
                    object->unk_084[6] = random_int(0x005AU) + 0x5A;
                    func_800722A4(objectIndex, 0x00000080);
                } else {
                    object->unk_084[6]--;
                }
            }
            break;
        case 3:
            func_80072E54(objectIndex, 0, object->type, 1, 0, 0);
            break;
        case 4:
            func_800722CC(objectIndex, 2);
            object_next_state(objectIndex);
            break;
    }
    if (func_80072320(objectIndex, 0x00000020) != 0) {
        if (object->unk_084[6] == 0) {
            func_800722A4(objectIndex, 0x00000080);
            object->unk_084[6] = 0x0010;
        } else {
            object->unk_084[6]--;
        }
    }
    if (func_80072320(objectIndex, 0x00000080) != 0) {
        func_800722CC(objectIndex, 0x00000080);
        if (func_80072320(objectIndex, 0x00000010) != 0) {
            func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x49));
        } else {
            func_800C98B8(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x17));
        }
    }
}

bool OPenguin::_toggle = false;

void OPenguin::InitOtherPenguin(s32 objectIndex) {
    Object* object;

    object = &gObjectList[objectIndex];
    object->unk_0D8 = 0;
    object->model = (Gfx*) d_course_sherbet_land_unk_data1;
    object->vertex = (Vtx*) d_course_sherbet_land_unk_data11;
    //object->boundingBoxSize = 4;
    object->unk_09C = 2;
    object->unk_04C = random_int(300);
    set_object_flag(objectIndex, 0x04000220);

    // This code has been significantly refactored from the original func_800845C8
    // Into a switch statement instead of checking for the index of the penguin
    switch(SpawnBhv) {
        case Behaviour::CIRCLE:
            object->unk_01C[1] = Speed;

            if (_toggle) {
                object->unk_0C4 = 0x8000;
            } else {
                object->unk_0C4 = 0;
            }

            _toggle = !_toggle;

            //object->unk_0DD = 2;
            func_800722A4(objectIndex, 8);
            break;
        case Behaviour::SLIDE3:
        case Behaviour::SLIDE4:
        case Behaviour::SLIDE6:
        case Behaviour::STRUT:

            if (gIsMirrorMode) {
                object->unk_0C6 = SpawnRot.roll; // Roll is used to save mirror mode angle offset;
            }

            set_obj_direction_angle(objectIndex, 0U, object->unk_0C6 + 0x8000, 0U);
            func_800722A4(objectIndex, 20);
            break;
    }

    func_80086EF0(objectIndex);
    object->unk_034 = 0.0f;
    object->type = get_animation_length(d_course_sherbet_land_unk_data11, 0);
    object_next_state(objectIndex);
}

void OPenguin::Reset() {
    _toggle = false;
}

void OPenguin::DrawEditorProperties() {
    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = GetLocation();
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

    ImGui::Text("Rotation");
    ImGui::SameLine();

    IRotator objRot = GetRotation();

    // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
    int rot[3] = {
        objRot.pitch,
        objRot.yaw,
        objRot.roll
    };

    if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
        for (size_t i = 0; i < 3; i++) {
            // Wrap around 0–65535
            rot[i] = (rot[i] % 65536 + 65536) % 65536;
        }
        IRotator newRot;
        newRot.Set(
            static_cast<uint16_t>(rot[0]),
            static_cast<uint16_t>(rot[1]),
            static_cast<uint16_t>(rot[2])
        );
        Rotate(newRot);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetRot")) {
        IRotator rot = IRotator(0, 0, 0);
        Rotate(rot);
    }

    ImGui::Text("Spawn Mode");
    ImGui::SameLine();

    int32_t type = static_cast<int32_t>(Type);
    const char* items[] = { "CHICK", "ADULT", "CREDITS", "EMPEROR" };

    if (ImGui::Combo("##Type", &type, items, IM_ARRAYSIZE(items))) {
        Type = static_cast<PenguinType>(type);

        // Update type values
        Object* object = &gObjectList[this->_objectIndex];
        switch(static_cast<PenguinType>(type)) {
            case PenguinType::CHICK:
                object->surfaceHeight = 5.0f;
                object->sizeScaling = 0.04f;
                object->boundingBoxSize = 4;
                break;
            case PenguinType::ADULT:
                object->surfaceHeight = -80.0f;
                object->sizeScaling = 0.08f;
                object->boundingBoxSize = 4;
                break;
            case PenguinType::CREDITS:
                object->surfaceHeight = -80.0f;
                object->sizeScaling = 0.08f;
                object->sizeScaling = 0.15f;
                break;
            case PenguinType::EMPEROR:
                object->sizeScaling = 0.2f;
                object->boundingBoxSize = 0x000C;
                break;
        }
    }

    ImGui::Text("Spawn Mode");
    ImGui::SameLine();

    int32_t behaviour = static_cast<int32_t>(SpawnBhv);
    const char* items2[] = { "DISABLED", "STRUT", "CIRCLE", "SLIDE3", "SLIDE4", "UNK", "SLIDE6" };

    if (ImGui::Combo("##Behaviour", &behaviour, items2, IM_ARRAYSIZE(items2))) {
        SpawnBhv = static_cast<Behaviour>(behaviour);
    }

    ImGui::Text("Diameter");
    ImGui::SameLine();

    float speed = Speed;
    if (ImGui::DragFloat("##Speed", &speed, 0.1f)) {
        Speed = speed;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
        Speed = 0.0f;
    }
}
