#include <actors.h>

/**
 * @brief Updates the Yoshi egg actor.
 *
 * @param egg
 */
void update_actor_yoshi_egg(struct YoshiValleyEgg* egg) {
    egg->pathRot += 0x5B;
    egg->pos[0] = egg->pathCenter[0] + (sins(egg->pathRot) * egg->pathRadius);
    egg->pos[2] = egg->pathCenter[2] + (coss(egg->pathRot) * egg->pathRadius);
    if ((egg->flags & 0x400) != 0) {
        egg->pathCenter[1] -= 0.12;
        if (egg->pathCenter[1] < -3.0f) {
            egg->pathCenter[1] = -3.0f;
        }
        egg->pos[1] += egg->pathCenter[1];
        if (egg->pos[1] < 0.0f) {
            egg->pos[1] = 0.0f;
            egg->pathCenter[1] = 0.0f;
            egg->flags &= ~(1 << 10);
        }
        egg->eggRot -= 0x4FA;
    }
    egg->eggRot -= 0x222;
}
