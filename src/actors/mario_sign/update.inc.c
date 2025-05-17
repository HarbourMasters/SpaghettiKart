#include <actors.h>

/**
 * @brief Updates the Mario sign actor.
 *
 * @param arg0
 */
void update_actor_mario_sign(struct Actor* arg0) {
    if ((arg0->flags & 0x800) == 0) {
        if ((arg0->flags & 0x400) != 0) {
            arg0->pos[1] += 4.0f;
            if (arg0->pos[1] > 800.0f) {
                arg0->flags |= 0x800;
                arg0->rot[1] += 1820;
            }
        } else {
            arg0->rot[1] += 182;
        }
    }
}
