#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include <assets/kalimari_desert_data.h>

/**
 * @brief Renders the train engine actor.
 * Actor used in Kalimari Desert.
 * His update position are made in vehicle.
 *
 * @param camera
 * @param actor
 */
void render_actor_train_engine(Camera *camera, struct TrainCar *actor) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3f sp160;
    Mat4 sp120;
    Mat4 spE0;
    Mat4 spA0;

    f32 distance = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2500.0f, gCameraZoom[camera - camera1], 9000000.0f);

    if (distance < 0.0f) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);
    maxObjectsReached = render_set_position(sp120, 0) == 0;
    if (maxObjectsReached) { return; }

    if (distance < 122500.0f) {

        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1C0F0));
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1B978));

    } else if (distance < 640000.0f) {

        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1D670));
       gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1D160));
    } else {
       gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1E910));
       gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1E480));
    }
    if (1440000.0f < distance) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D28));

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 32.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 32.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, 16.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, 16.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 12.0f, -12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D70));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 12.0f, -12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D70));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, 17.0f, 12.0f, -34.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D70));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, -17.0f, 12.0f, -34.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D70));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void render_actor_train_tender(Camera *camera, struct TrainCar *actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 625.0f, gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);
    if (render_set_position(sp120, 0) == 0) { return; }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1F228));

    } else if (temp_f0 < 1000000.0f) {
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1F708));
    } else {
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x1FAF8));
    }
    if (1440000.0f < temp_f0) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D28));

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void render_actor_train_passenger_car(Camera *camera, struct TrainCar *actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2025.0f, gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);

    if (render_set_position(sp120, 0) == 0) { return; }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x20A20));
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x20A08));

    } else if (temp_f0 < 1000000.0f) {

        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x21550));
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x21220));
    } else {
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x21C90));
        gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x21A80));
    }
    if (1440000.0f < temp_f0) { return; }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22D28));

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, 17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, -17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    if (render_set_position(spA0, 3) == 0) { return; }

    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual(0x07000000+0x22DB8));
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
