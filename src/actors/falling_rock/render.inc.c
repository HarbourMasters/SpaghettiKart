#include <actors.h>
#include <main.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_data.h>
#include "port/interpolation/FrameInterpolation.h"

/**
 * @brief Renders the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param camera
 * @param rock
 */
void render_actor_falling_rock(Camera* camera, struct FallingRock* rock) {
    Vec3s sp98;
    Vec3f sp8C;
    Mat4 mtx;
    f32 height;
    UNUSED s32 pad[4];

    if (rock->respawnTimer != 0) {
        return;
    }

    height = is_within_render_distance(camera->pos, rock->pos, camera->rot[1], 400.0f, gCameraFOV[camera - camera1],
                                       4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        height = CLAMP(height, 0.0f, 250000.0f);
    }

    if (height < 0.0f) {
        return;
    }

    if (height < 250000.0f) {

        if (rock->unk30.unk34 == 1) {
            sp8C[0] = rock->pos[0];
            sp8C[2] = rock->pos[2];
            height = calculate_surface_height(sp8C[0], rock->pos[1], sp8C[2], rock->unk30.meshIndexZX);
            sp98[0] = 0;
            sp98[1] = 0;
            sp98[2] = 0;
            sp8C[1] = height + 2.0f;

            FrameInterpolation_RecordOpenChild("rock_shadow", (uintptr_t) rock);
            mtxf_pos_rotation_xyz(mtx, sp8C, sp98);
            if (render_set_position(mtx, 0) == 0) {
                FrameInterpolation_RecordCloseChild();
                return;
            }
            gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_6F88);
            FrameInterpolation_RecordCloseChild();
        }
    }

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("rock", (uintptr_t) rock);

    mtxf_pos_rotation_xyz(mtx, rock->pos, rock->rot);
    if (render_set_position(mtx, 0) == 0) {
        // @port Pop the transform id.
        FrameInterpolation_RecordCloseChild();
        return;
    }
    gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_falling_rock);

    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}
