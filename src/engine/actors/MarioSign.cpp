#include "MarioSign.h"

#include <libultra/gbi.h>
#include <assets/mario_raceway_data.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "actor_types.h"
}

AMarioSign::AMarioSign(FVector pos) {
    Type = ACTOR_MARIO_SIGN;
    Name = "Mario Sign";
    Pos[0] = pos.x;
    Pos[1] = pos.y;
    Pos[2] = pos.z;
    Flags |= 0x4000;
}

void AMarioSign::Tick() {
    if ((Flags & 0x800) == 0) {
        if ((Flags & 0x400) != 0) {
            Pos[1] += 4.0f;
            if (Pos[1] > 800.0f) {
                Flags |= 0x800;
                Rot[1] += 1820;
            }
        } else {
            Rot[1] += 182;
        }
    }
}

void AMarioSign::Draw(Camera *camera) {
    Mat4 sp40;
    f32 unk;

    if (Flags & 0x800) {
        return;
    }

    unk = is_within_render_distance(camera->pos, Pos, camera->rot[1], 0, gCameraZoom[camera - camera1], 16000000.0f);
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
