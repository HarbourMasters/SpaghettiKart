#include <libultraship.h>
#include <actor_types.h>

/**
 * @brief Updates the paddle boat actor.
 * Actor used in DK's Jungle Parkway.
 * Rotate only the paddle wheel, for position and rotation of the boat it's in vehicle.
 *
 * @param boat
 */
void update_actor_paddle_boat(struct PaddleWheelBoat* boat) {
    boat->wheelRot += 0x38E;
}
