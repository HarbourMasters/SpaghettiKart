#include "MarioSign.h"

#include <libultra/gbi.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "actor_types.h"
#include "code_800029B0.h"
#include "collision.h"
}

AMarioSign::AMarioSign(const SpawnParams& params) : AActor(params) {
    Name = "Mario Sign";
    ResourceName = "mk:mario_sign";
    Model = d_course_mario_raceway_dl_sign;

    Speed = params.Speed.value_or(182);

    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    Pos[0] = pos.x * gCourseDirection;
    Pos[1] = pos.y;
    Pos[2] = pos.z;

    IRotator rot = params.Rotation.value_or(IRotator(0, 0, 0));
    Rot[0] = rot.pitch;
    Rot[1] = rot.yaw;
    Rot[2] = rot.roll;

    Scale = params.Scale.value_or(FVector(1.0f, 1.0f, 1.0f));

    func_802AAAAC(&Unk30);
    Flags = -0x8000;
    Flags |= 0x4000;
}

bool AMarioSign::IsMod() {
    return true;
}

void AMarioSign::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
}

void AMarioSign::Tick() {
    if ((Flags & 0x800) == 0) {
        if ((Flags & 0x400) != 0) {
            Pos[1] += 4.0f;
            if (Pos[1] > 800.0f) {
                Flags |= 0x800;
                Rot[1] += Speed * 10; // Originally 1820
            }
        } else {
            Rot[1] += Speed; // Originally 182
        }
    }
}

void AMarioSign::Draw(Camera *camera) {
    Mat4 sp40;
    f32 unk;

    if (Flags & 0x800) {
        return;
    }

    unk = is_within_render_distance(camera->pos, Pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 16000000.0f);
    if (CVarGetInteger("gNoCulling", 0) == 1) {
        unk = MAX(unk, 0.0f);
    }
    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        mtxf_pos_rotation_xyz(sp40, Pos, Rot);

        if (render_set_position(sp40, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_mario_raceway_dl_sign);
        }
    }
}
