#include "FallingRock.h"

#include <libultra/gbi.h>
#include "CoreMath.h"
#include <assets/textures/tracks/choco_mountain/choco_mountain_data.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_data.h>
#include "port/interpolation/FrameInterpolation.h"
#include "port/Game.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "actor_types.h"
#include "code_800029B0.h"
#include "collision.h"
#include "code_800029B0.h"
#include "external.h"
}

size_t AFallingRock::_count = 0;

AFallingRock::AFallingRock(SpawnParams params) : AActor(params) {
    Type = ACTOR_FALLING_ROCK;
    Name = "Falling Rock";
    ResourceName = "mk:falling_rock";

    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    TimerLength = params.Behaviour.value_or(80);
    Pos[0] = pos.x * gCourseDirection;
    Pos[1] = pos.y + 10.0f;
    Pos[2] = pos.z;
    State = _count;
    func_802AAAAC(&Unk30);

    Flags = -0x8000;
    Flags |= 0x4000;
    BoundingBoxSize = 10.0f;
    Model = d_course_choco_mountain_dl_falling_rock;

    _count += 1;
}

void AFallingRock::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Behaviour = TimerLength;
}

void AFallingRock::Reset() {
    RespawnTimer = TimerLength;
    FVector pos = SpawnPos;
    Pos[0] = (f32) pos.x * gCourseDirection;
    Pos[1] = (f32) pos.y + 10.0f;
    Pos[2] = (f32) pos.z;
    vec3f_set(Velocity, 0, 0, 0);
    vec3s_set(Rot, 0, 0, 0);
}

bool AFallingRock::IsMod() {
    return true;
}

/**
 * @brief Updates the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param rock
 */
void AFallingRock::Tick() {
    Vec3f unkVec;
    f32 pad0;
    f32 pad1;

    if (RespawnTimer != 0) {
        RespawnTimer -= 1;
        return;
    }
    if (Pos[1] < CM_GetWaterLevel(Pos, NULL)) {
        AFallingRock::Reset();
    }
    Rot[0] += (s16) ((Velocity[2] * 5461.0f) / 20.0f);
    Rot[2] += (s16) ((Velocity[0] * 5461.0f) / 20.0f);
    Velocity[1] -= 0.1;
    if (Velocity[1] < (-2.0f)) {
        Velocity[1] = -2.0f;
    }
    Pos[0] += Velocity[0];
    Pos[1] += Velocity[1];
    Pos[2] += Velocity[2];
    pad1 = Velocity[1];
    check_bounding_collision(&Unk30, 10.0f, Pos[0], Pos[1], Pos[2]);
    pad0 = Unk30.surfaceDistance[2];
    if (pad0 < 0.0f) {
        unkVec[0] = -Unk30.orientationVector[0];
        unkVec[1] = -Unk30.orientationVector[1];
        unkVec[2] = -Unk30.orientationVector[2];
        Pos[0] += unkVec[0] * Unk30.surfaceDistance[2];
        Pos[1] += unkVec[1] * Unk30.surfaceDistance[2];
        Pos[2] += unkVec[2] * Unk30.surfaceDistance[2];
        adjust_pos_orthogonally(unkVec, pad0, Velocity, 2.0f);
        Velocity[1] = -1.2f * pad1;
        func_800C98B8(Pos, Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
    }
    pad0 = Unk30.surfaceDistance[0];
    if (pad0 < 0.0f) {
        unkVec[1] = -Unk30.unk48[1];
        if (unkVec[1] == 0.0f) {
            Velocity[1] *= -1.2f;
            return;
        } else {
            unkVec[0] = -Unk30.unk48[0];
            unkVec[2] = -Unk30.unk48[2];
            Pos[0] += unkVec[0] * Unk30.surfaceDistance[0];
            Pos[1] += unkVec[1] * Unk30.surfaceDistance[0];
            Pos[2] += unkVec[2] * Unk30.surfaceDistance[0];
            adjust_pos_orthogonally(unkVec, pad0, Velocity, 2.0f);
            Velocity[1] = -1.2f * pad1;
            func_800C98B8(Pos, Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
    pad0 = Unk30.surfaceDistance[1];
    if (pad0 < 0.0f) {
        unkVec[1] = -Unk30.unk54[1];
        if (unkVec[1] == 0.0f) {
            Velocity[1] *= -1.2f;
        } else {
            unkVec[0] = -Unk30.unk54[0];
            unkVec[2] = -Unk30.unk54[2];
            Pos[0] += unkVec[0] * Unk30.surfaceDistance[1];
            Pos[1] += unkVec[1] * Unk30.surfaceDistance[1];
            Pos[2] += unkVec[2] * Unk30.surfaceDistance[1];
            pad1 = Velocity[1];
            adjust_pos_orthogonally(unkVec, pad0, Velocity, 2.0f);
            Velocity[1] = -1.2f * pad1;
            func_800C98B8(Pos, Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}



/**
 * @brief Renders the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param camera
 * @param rock
 */
void AFallingRock::Draw(Camera* camera) {
    Vec3s sp98;
    Vec3f sp8C;
    Mat4 mtx;
    f32 height;
    UNUSED s32 pad[4];

    if (RespawnTimer != 0) {
        return;
    }

    height = is_within_render_distance(camera->pos, Pos, camera->rot[1], 400.0f, gCameraFOV[camera - camera1],
                                       4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        height = CLAMP(height, 0.0f, 250000.0f);
    }

    if (height < 0.0f) {
        return;
    }

    if (height < 250000.0f) {

        if (Unk30.unk34 == 1) {
            sp8C[0] = Pos[0];
            sp8C[2] = Pos[2];
            height = calculate_surface_height(sp8C[0], Pos[1], sp8C[2], Unk30.meshIndexZX);
            sp98[0] = 0;
            sp98[1] = 0;
            sp98[2] = 0;
            sp8C[1] = height + 2.0f;

            FrameInterpolation_RecordOpenChild("rock_shadow", (uintptr_t) this);
            mtxf_pos_rotation_xyz(mtx, sp8C, sp98);
            if (render_set_position(mtx, 0) == 0) {
                FrameInterpolation_RecordCloseChild();
                return;
            }
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_choco_mountain_dl_6F88);
            FrameInterpolation_RecordCloseChild();
        }
    }

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("rock", (uintptr_t) this);

    mtxf_pos_rotation_xyz(mtx, Pos, Rot);
    if (render_set_position(mtx, 0) == 0) {
        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
        return;
    }
    gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_choco_mountain_dl_falling_rock);

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}

void AFallingRock::DrawEditorProperties() {
    ImGui::Text("Location");
    ImGui::SameLine();
    FVector location = SpawnPos;
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

    ImGui::Text("Respawn Timer");
    ImGui::SameLine();

    int32_t behaviour = static_cast<int32_t>(TimerLength);

    if (ImGui::InputInt("##Behaviour", &behaviour)) {
            TimerLength = static_cast<uint32_t>(behaviour);
    }
}
