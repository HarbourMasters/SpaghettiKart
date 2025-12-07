#include <camera.h>
#include <actors.h>
#include <main.h>
#include <libultra/gbi.h>
#include <assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h>

/**
 * @brief Renders the cow actor.
 * Actor used in Moo Moo Farm.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_cow(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    if (is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1],
                                  4000000.0f) < 0 &&
        CVarGetInteger("gNoCulling", 0) == 0) {
        return;
    }

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("render_actor_cow", TAG_OBJECT(arg2));

    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        switch (arg2->state) {
            case 0:
                gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow1);
                break;
            case 1:
                gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow2);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow3);
                break;
            case 3:
                gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow4);
                break;
            case 4:
                gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow5);
                break;
        }
    }

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}
