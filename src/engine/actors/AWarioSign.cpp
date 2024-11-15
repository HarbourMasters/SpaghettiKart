#include "AWarioSign.h"

#include <libultra/gbi.h>
#include <assets/wario_stadium_data.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
}

AWarioSign::AWarioSign(Vec3f pos) {
    Pos[0] = pos[0];
    Pos[1] = pos[1];
    Pos[2] = pos[2];
}

void AWarioSign::Tick() {
    Rot[1] += 0xB6;
}

void AWarioSign::Draw(Camera *camera) {
    Mat4 sp38;
    f32 unk =
        is_within_render_distance(camera->pos, Pos, camera->rot[1], 0, gCameraZoom[camera - camera1], 16000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        unk = MAX(unk, 0.0f);
    }

    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

        mtxf_pos_rotation_xyz(sp38, Pos, Rot);
        if (render_set_position(sp38, 0) != 0) {

            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_wario_stadium_dl_sign);
        }
    }
}
