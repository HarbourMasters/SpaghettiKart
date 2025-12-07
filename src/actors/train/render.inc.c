#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include <assets/models/tracks/kalimari_desert/kalimari_desert_data.h>

/**
 * @brief Renders the train engine actor.
 * Actor used in Kalimari Desert.
 * His update position are made in vehicle.
 *
 * @param camera
 * @param actor
 */
void render_actor_train_engine(Camera* camera, struct TrainCar* actor) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3f sp160;
    Mat4 mainMtx;
    Mat4 objectMtx;
    Mat4 resultMtx;

    f32 distance = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2500.0f,
                                             gCameraFOV[camera - camera1], 9000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        distance = MAX(distance, 0.0f);
    }

    if (distance < 0.0f) {
        return;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        distance = 0.0f;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(mainMtx, actor->pos, actor->rot);
    maxObjectsReached = render_set_position(mainMtx, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (distance < 122500.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1C0F0);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1B978);

    } else if (distance < 640000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1D670);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1D160);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1E910);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1E480);
    }
    if (1440000.0f < distance) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    mtxf_rotate_x(mainMtx, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 32.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 32.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, 16.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, 16.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 12.0f, -12.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 12.0f, -12.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, 17.0f, 12.0f, -34.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(mainMtx, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, -17.0f, 12.0f, -34.0f);
    mtxf_translate(objectMtx, sp160);
    mtxf_multiplication(resultMtx, mainMtx, objectMtx);

    maxObjectsReached = render_set_position(resultMtx, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void render_actor_train_tender(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 625.0f,
                                            gCameraFOV[camera - camera1], 9000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        temp_f0 = 0.0f;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);
    if (render_set_position(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1F228);

    } else if (temp_f0 < 1000000.0f) {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1F708);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1FAF8);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void render_actor_train_passenger_car(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2025.0f,
                                            gCameraFOV[camera - camera1], 9000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        temp_f0 = 0.0f;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);

    if (render_set_position(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_20A20);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_20A08);

    } else if (temp_f0 < 1000000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21550);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21220);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21C90);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21A80);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, 17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, -17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
