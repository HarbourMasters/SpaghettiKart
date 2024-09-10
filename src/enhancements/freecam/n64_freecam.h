#ifndef N64_FREECAM_H
#define N64_FREECAM_H

#include <libultraship.h>


void freecam_n64_move_camera_up(Camera* camera, struct Controller *controller, f32 distance);
void freecam_n64_controller_manager(Camera *camera, struct Controller *controller, Player *player);
void freecam_n64_calculate_forward_vector(Camera* camera, Vec3f forwardVector);
void freecam_n64_move_camera_forward(Camera* camera, struct Controller *controller, f32 distance);
void freecam_n64_update(Camera* camera, struct Controller *controller);

#endif // N64_FREECAM_H