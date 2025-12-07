#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include "assets/models/tracks/kalimari_desert/kalimari_desert_data.h"

/**
 * @brief Renders the railroad crossing actor.
 * Actor used in Kalimari Desert.
 *
 * @param arg0
 * @param rr_crossing
 */
void render_actor_railroad_crossing(Camera* arg0, struct RailroadCrossing* rr_crossing) {
    UNUSED Vec3s sp80 = { 0, 0, 0 };
    Mat4 sp40;
    f32 unk = is_within_render_distance(arg0->pos, rr_crossing->pos, arg0->rot[1], 0.0f, gCameraFOV[arg0 - camera1],
                                        4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        unk = MAX(unk, 0.0f);
    }

    if (!(unk < 0.0f)) {
        mtxf_pos_rotation_xyz(sp40, rr_crossing->pos, rr_crossing->rot);

        if (render_set_position(sp40, 0) != 0) {
            gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);

            if (CM_GetCrossingOnTriggered(rr_crossing->crossingTrigger)) {

                if (rr_crossing->someTimer < 20) {
                    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_crossing_right_active);
                } else {
                    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_crossing_left_active);
                }
            } else {
                gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_crossing_both_inactive);
            }
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
        }
    }
}
