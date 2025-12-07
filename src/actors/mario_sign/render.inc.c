#include <actors.h>
#include <main.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include "port/interpolation/FrameInterpolation.h"

/**
 * @brief Renders the Mario sign actor.
 * Actor used in Mario Raceway.
 *
 * @param arg0
 * @param arg1
 * @param arg2
 */
void render_actor_mario_sign(Camera* arg0, UNUSED Mat4 arg1, struct Actor* arg2) {
    Mat4 mtx;
    f32 unk;
    s16 temp = arg2->flags;
    if (temp & 0x800) {
        return;
    }

    unk = is_within_render_distance(arg0->pos, arg2->pos, arg0->rot[1], 0, gCameraFOV[arg0 - camera1], 16000000.0f);
    if (CVarGetInteger("gNoCulling", 0) == 1) {
        unk = MAX(unk, 0.0f);
    }
    if (!(unk < 0.0f)) {

        FrameInterpolation_RecordOpenChild("mario_sign", TAG_OBJECT(arg2));

        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        mtxf_pos_rotation_xyz(mtx, arg2->pos, arg2->rot);
        if (render_set_position(mtx, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, d_course_mario_raceway_dl_sign);
        }
        FrameInterpolation_RecordCloseChild();
    }
}
