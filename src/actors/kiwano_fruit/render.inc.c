#include <actors.h>
#include <libultra/gbi.h>
#include <main.h>
#include "assets/textures/other_textures.h"
// #include <assets/dks_jungle_parkway_data.h>

/**
 * @brief Renders the kiwano fruit actor.
 * Actor used in DK's Jungle Parkway.
 *
 * @param camera
 * @param arg1
 * @param actor
 */
void render_actor_kiwano_fruit(UNUSED Camera* camera, Mat4 arg1, struct Actor* actor) {
    uintptr_t addr;
    s32 maxObjectsReached;

    if (actor->state == 0) {
        return;
    }

    arg1[3][0] = actor->pos[0];
    arg1[3][1] = actor->pos[1];
    arg1[3][2] = actor->pos[2];

    maxObjectsReached = render_set_position(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    // Animate actor by creating an index from rot[0]. Divide by texture size to get whole number 0-2.
    // All three animated textures need to be the same size for this to work.
    size_t idx = (actor->rot[0] << 0xA) / ResourceGetTexSizeByName(gTextureDksJungleParkwayKiwanoFruit1);
    switch(idx) {
        case 0:
            addr = gTextureDksJungleParkwayKiwanoFruit1;
            break;
        case 1:
            addr = gTextureDksJungleParkwayKiwanoFruit2;
            break;
        case 2:
            addr = gTextureDksJungleParkwayKiwanoFruit3;
            break;
    }

    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(addr), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_kiwano_fruit);
}
