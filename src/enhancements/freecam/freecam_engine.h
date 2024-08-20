#ifndef FREE_CAM_ENGINE_H
#define FREE_CAM_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libultraship.h>
#include "camera.h"

// Yaw/pitch rotation sensitivity
#define MOUSE_SENSITIVITY_X 0.006f
#define MOUSE_SENSITIVITY_Y 0.006f

typedef struct {
    Vec3f velocity;
    Vec3f forwardVector;
} FreeCam;

extern FreeCam freeCam;
extern f32 gDampValue;

//void update_camera(Camera *camera, f32 deltaTime);
void freecam_pan_camera(Camera *camera, f32 moveSpeed);
void freecam_calculate_forward_vector_allow_rotation(Camera* camera, Vec3f forwardVector);
void freecam_calculate_forward_vector(Camera* camera, Vec3f forwardVector);
void freecam_move_camera_forward(Camera* camera, f32 distance, Vec3f forwardVector);
void freecam_target_player(Camera *camera, u32 playerIndex);
void freecam_update(Camera* camera, Vec3f forwardVector);
void freecam_move_camera_up(Camera* camera, f32);
void freecam_move_camera_down(Camera* camera, f32);

extern f32 gDampValue;

#ifdef __cplusplus
}
#endif

#endif // FREE_CAM_ENGINE_H