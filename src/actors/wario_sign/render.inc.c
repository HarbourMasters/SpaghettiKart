#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include <assets/models/tracks/wario_stadium/wario_stadium_data.h>

/**
 * @brief Renders the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @param camera
 * @param arg1
 */
void render_actor_wario_sign(Camera* camera, struct Actor* arg1) {
    Mat4 sp38;
    f32 unk =
        is_within_render_distance(camera->pos, arg1->pos, camera->rot[1], 0, camera->fieldOfView, 16000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        unk = MAX(unk, 0.0f);
    }

    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

        mtxf_pos_rotation_xyz(sp38, arg1->pos, arg1->rot);
        if (render_set_position(sp38, 0) != 0) {

            gSPDisplayList(gDisplayListHead++, d_course_wario_stadium_dl_sign);
        }
    }
}
