#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include <assets/models/common_data.h>
#include <assets/textures/common_data.h>
#include "courses/all_course_data.h"
#include <assets/models/tracks/kalimari_desert/kalimari_desert_data.h>
#include <assets/models/tracks/bowsers_castle/bowsers_castle_data.h>

/**
 * @brief Renders the tree actor in Mario rawceay.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_mario_raceway(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 = is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1],
                                        16000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 3.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_mario_raceway_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Yoshi Valley.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_yoshi_valley(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Royal Raceway.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_royal_raceway(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_royal_raceway_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Moo Moo Farm.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_moo_moo_farm(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 6250000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 600.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 5.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_tree);
    }
}

void render_actor_tree_luigi_raceway(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        // Why is a TLUT being used a DL here? That makes no sense
        // Based on the TLUT being loaded above, this ought to be be another
        // tree related DL, presumably one found in a course other than Moo Moo farm
        //                                 0x0600FC70
        gSPDisplayList(gDisplayListHead++, d_course_luigi_raceway_dl_FC70);
    }
}

/**
 * @brief Renders Peach's Castle trees in Royal Raceway.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_peach_castle(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_royal_raceway_dl_castle_tree);
    }
}

/**
 * @brief Renders the bush actor in Bowser's Castle.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_bush_bowser_castle(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 640000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_trees_import);
        gSPDisplayList(gDisplayListHead++, d_course_bowsers_castle_dl_bush);
    }
}

/**
 * @brief Renders the tree actor in Frappe Snowland.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_frappe_snowland(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 2.79999995f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, d_course_frappe_snowland_dl_tree);
    }
}

/**
 * @brief Renders the a first variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_cactus1_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 1.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_cactus1);
    }
}

/**
 * @brief Renders the a second variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_cactus2_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 1.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_cactus2);
    }
}

/**
 * @brief Renders the a third variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
void render_actor_tree_cactus3_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraFOV[camera - camera1], 4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 0.80000001f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_cactus3);
    }
}
