#include <actors.h>
#include <main.h>
#include <libultra/gbi.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include <assets/models/tracks/royal_raceway/royal_raceway_data.h>

const char* sPiranhaPlantTextures[] = {
    gTexturePiranhaPlant1,
    gTexturePiranhaPlant2,
    gTexturePiranhaPlant3,
    gTexturePiranhaPlant4,
    gTexturePiranhaPlant5,
    gTexturePiranhaPlant6,
    gTexturePiranhaPlant7,
    gTexturePiranhaPlant8,
    gTexturePiranhaPlant9
};

/**
 * @brief Renders the piranha plant actor.
 * Actor used in Mario Raceway and Royal Raceway.
 *
 * @param arg0
 * @param arg1
 * @param arg2
 */
void render_actor_piranha_plant(Camera* arg0, Mat4 arg1, struct PiranhaPlant* arg2) {
    UNUSED s32 pad;
    u8* addr;
    s16 temp_lo = arg0 - camera1;
    s16 animationFrame; // unconfirmed
    s16 temp = arg2->flags;
    f32 temp_f0;
    s32 maxObjectsReached;

    if (temp & 0x800) {
        return;
    }

    temp_f0 = is_within_render_distance(arg0->pos, arg2->pos, arg0->rot[1], 0, gCameraFOV[arg0 - camera1], 1000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        temp_f0 = MAX(temp_f0, 0.0f);
    }

    if (temp_f0 < 0.0f) {

        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = -1;
                break;
            case 1:
                arg2->visibilityStates[1] = -1;
                break;
            case 2:
                arg2->visibilityStates[2] = -1;
                break;
            case 3:
                arg2->visibilityStates[3] = -1;
                break;
        }
        return;
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];
    maxObjectsReached = render_set_position(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (CVarGetInteger("gDisableLod", 1) == 1) {
        temp_f0 = 0.0f;
    }

    if (90000.0f < temp_f0) {

        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = 0;
                break;
            case 1:
                arg2->visibilityStates[1] = 0;
                break;
            case 2:
                arg2->visibilityStates[2] = 0;
                break;
            case 3:
                arg2->visibilityStates[3] = 0;
                break;
        }
        animationFrame = 0;

    } else {
        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = 1;
                break;
            case 1:
                arg2->visibilityStates[1] = 1;
                break;
            case 2:
                arg2->visibilityStates[2] = 1;
                break;
            case 3:
                arg2->visibilityStates[3] = 1;
                break;
        }

        switch (temp_lo) {
            case 0:
                animationFrame = arg2->timers[0];
                break;
            case 1:
                animationFrame = arg2->timers[1];
                break;
            case 2:
                animationFrame = arg2->timers[2];
                break;
            case 3:
                animationFrame = arg2->timers[3];
                break;
        }
    }
    animationFrame /= 6;

    if (animationFrame > 8) {
        animationFrame = 8;
    }
    gDPLoadTextureBlock(gDisplayListHead++, sPiranhaPlantTextures[animationFrame], G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (IsMarioRaceway()) {
        gSPDisplayList(gDisplayListHead++, &d_course_mario_raceway_dl_piranha_plant);
    } else {
        gSPDisplayList(gDisplayListHead++, &d_course_royal_raceway_dl_piranha_plant);
    }
}
