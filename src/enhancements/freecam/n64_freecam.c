#include <libultraship.h>
#include <defines.h>
#include <macros.h>
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "camera.h"
#include <common_structs.h>
#include "player_controller.h"

#define N64_SENSITIVITY_X 0.0003f
#define N64_SENSITIVITY_Y 0.0003f

// Calculates the forward direction vector based on camera orientation
void freecam_n64_calculate_forward_vector(Camera* camera, Vec3f forwardVector) {
    f32 pitch = (camera->rot[2] / 65535.0f) * 360.0f; // Convert pitch from 0-65535 to degrees
    f32 yaw = (camera->rot[1] / 65535.0f) * 360.0f;   // Convert yaw from 0-65535 to degrees

    // Convert degrees to radians
    pitch = pitch * M_PI / 180.0f;
    yaw = yaw * M_PI / 180.0f;

    forwardVector[0] = -sinf(yaw) * cosf(pitch);
    forwardVector[1] = -sinf(pitch);
    forwardVector[2] = cosf(yaw) * cosf(pitch);
}

// Function to move the camera forward
void freecam_n64_move_camera_forward(Camera* camera, struct Controller *controller, f32 distance) {
    Vec3f forwardVector;
    Vec3f rightVector;
    f32 length;
    freecam_n64_calculate_forward_vector(camera, forwardVector);

    if (controller->button & Z_TRIG) {
        distance *= 3;
    }

    // Normalize the forward vector
    length = sqrtf(forwardVector[0] * forwardVector[0] + forwardVector[1] * forwardVector[1] + forwardVector[2] * forwardVector[2]);
    forwardVector[0] /= length;
    forwardVector[1] /= length;
    forwardVector[2] /= length;

    // Calculate the right vector by taking the cross product of forward and up
    rightVector[0] = forwardVector[1] * camera->up[2] - forwardVector[2] * camera->up[1];
    rightVector[1] = forwardVector[2] * camera->up[0] - forwardVector[0] * camera->up[2];
    rightVector[2] = forwardVector[0] * camera->up[1] - forwardVector[1] * camera->up[0];

    // Move the camera's position along the forward vector while considering its up vector
    camera->pos[0] += forwardVector[0] * distance;
    camera->pos[1] += forwardVector[1] * distance;
    camera->pos[2] += forwardVector[2] * distance;

    // Optionally, you can also adjust the lookAt point to maintain the same relative position
    camera->lookAt[0] += forwardVector[0] * distance;
    camera->lookAt[1] += forwardVector[1] * distance;
    camera->lookAt[2] += forwardVector[2] * distance;
}

// Function to move the camera forward
void freecam_n64_move_camera_up(Camera* camera, struct Controller *controller, f32 distance) {
    // Check if the Z button is pressed (for faster movement)
    if (controller->button & Z_TRIG) {
        distance *= 3;
    }

    // Move the camera's position along its up vector (Y-axis)
    camera->pos[1] += distance;

    // Optionally, adjust the lookAt point to maintain the same relative position
    camera->lookAt[1] += distance;
}

// Update camera rotation and lookAt point based on input
void freecam_n64_update(Camera* camera, struct Controller *controller) {
    // Calculate yaw (horizontal movement)
    f32 yawChange = controller->rawStickX * N64_SENSITIVITY_X;
    f32 pitchChange = controller->rawStickY * N64_SENSITIVITY_Y;
    Vec3f forwardVector;

    check_bounding_collision(&camera->collision, 50, camera->pos[0], camera->pos[1], camera->pos[2]);

    camera->rot[1] += (short)(yawChange * 65535.0f / (2 * M_PI)); // Convert radians to 0-65535 range

    camera->rot[2] += (short)(-pitchChange * 65535.0f / (2 * M_PI)); // Convert radians to 0-65535 range

    if (camera->rot[2] > 15999) {
        camera->rot[2] = 15999;
    } else if (camera->rot[2] < -15999) {
        camera->rot[2] = -15999;
    }

    // Update the lookAt point based on the new orientation
    freecam_calculate_forward_vector(camera, forwardVector);
    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[1] = camera->pos[1] + forwardVector[1];
    camera->lookAt[2] = camera->pos[2] + forwardVector[2];
}