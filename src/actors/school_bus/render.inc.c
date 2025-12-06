#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include <defines.h>

/**
 * @brief Renders the school bus actor.
 * Actor used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @param arg0
 * @param arg1
 */
void render_actor_school_bus(Camera* arg0, struct Actor* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 pad2[32];
    f32 temp_f0;

    temp_f0 =
        is_within_render_distance(arg0->pos, arg1->pos, arg0->rot[1], 2500.0f, gCameraFOV[arg0 - camera1], 9000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {
        return;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        temp_f0 = 0.0f;
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(spC8, arg1->pos, arg1->rot);
    if (render_set_position(spC8, 0) != 0) {

        if (gActiveScreenMode == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_3);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_4);
            } else {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_5);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_4);
        } else {
            gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_5);
        }
    }
}
