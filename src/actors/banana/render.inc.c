#include <actors.h>
#include <code_800029B0.h>
#include <libultra/gbi.h>

/**
 * @brief Render the banana actor
 *
 * @param camera
 * @param arg1
 * @param banana
 */
void render_actor_banana(Camera* camera, UNUSED Mat4 arg1, struct BananaActor* banana) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3s sp7C;
    Mat4 sp3C;

    f32 temp = is_within_render_distance(camera->pos, banana->pos, camera->rot[1], 0, gCameraFOV[camera - camera1],
                                         490000.0f);
    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp = MAX(temp, 0.0f);
    }
    if (temp < 0.0f) {
        actor_not_rendered(camera, (struct Actor*) banana);
        return;
    }

    if ((banana->pos[1] > gCourseMaxY + 800.0f)) {
        actor_not_rendered(camera, (struct Actor*) banana);
        return;
    }
    if (banana->pos[1] < (gCourseMinY - 800.0f)) {
        actor_not_rendered(camera, (struct Actor*) banana);
        return;
    }

    actor_rendered(camera, (struct Actor*) banana);

    if (banana->state == 5) {
        mtxf_pos_rotation_xyz(sp3C, banana->pos, banana->rot);
    } else {
        sp7C[0] = 0;
        sp7C[1] = 0;
        sp7C[2] = 0;
        mtxf_pos_rotation_xyz(sp3C, banana->pos, sp7C);
    }

    maxObjectsReached = render_set_position(sp3C, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (banana->state != 5) {
        gSPDisplayList(gDisplayListHead++, &common_model_banana);
    } else {
        gSPDisplayList(gDisplayListHead++, &common_model_flat_banana);
    }
}
