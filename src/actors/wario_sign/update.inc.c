#include <actors.h>

/**
 * @brief Updates the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @param arg0
 */
void update_actor_wario_sign(struct Actor* arg0) {
    arg0->rot[1] += 0xB6;
}
