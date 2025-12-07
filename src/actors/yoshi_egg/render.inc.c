#include <camera.h>
#include <actors.h>
#include <defines.h>
#include <main.h>
#include "port/interpolation/FrameInterpolation.h"

/**
 * @brief Renders the Yoshi egg actor.
 * Actor used in Yoshi Valley.
 *
 * @param arg0
 * @param arg1
 * @param egg
 * @param arg3
 */
void render_actor_yoshi_egg(Camera* arg0, Mat4 arg1, struct YoshiValleyEgg* egg, u16 arg3) {
    Mat4 sp60;
    Vec3s sp5C;
    Vec3f sp54;
    f32 temp_f0;

    if (gGamestate != CREDITS_SEQUENCE) {
        temp_f0 = is_within_render_distance(arg0->pos, egg->pos, arg0->rot[1], 200.0f, gCameraFOV[arg0 - camera1],
                                            16000000.0f);

        if (CVarGetInteger("gNoCulling", 0) == 1) {
            temp_f0 = MAX(temp_f0, 0.0f);
        }

        if (temp_f0 < 0.0f) {
            return;
        }
    } else {
        arg3 = 15;
        temp_f0 = 0.0f;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        arg3 = 15;
        temp_f0 = 0.0f;
    }

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    if ((arg3 > 12) && (arg3 < 20)) {
        if (temp_f0 < 640000.0f) {
            sp54[0] = egg->pos[0];
            sp54[1] = 3.0f;
            sp54[2] = egg->pos[2];
            func_802976D8(sp5C);
            func_8029794C(sp54, sp5C, 10.0f);
        }
        sp5C[0] = 0;
        sp5C[1] = egg->eggRot;
        sp5C[2] = 0;

        FrameInterpolation_RecordOpenChild("yoshi_egg", TAG_OBJECT(egg));

        mtxf_pos_rotation_xyz(sp60, egg->pos, sp5C);
        if (render_set_position(sp60, 0) == 0) {
            return;
        }

        gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
        gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_16D70);
        FrameInterpolation_RecordCloseChild();
    } else {
        arg1[3][0] = egg->pos[0];
        arg1[3][1] = egg->pos[1];
        arg1[3][2] = egg->pos[2];

        FrameInterpolation_RecordOpenChild("yoshi_egg2", TAG_OBJECT(egg));

        if (render_set_position(arg1, 0) != 0) {
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_egg_lod0);
        }
        FrameInterpolation_RecordCloseChild();
    }
}
