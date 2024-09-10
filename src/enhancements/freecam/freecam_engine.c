#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include "player_controller.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "racing/collision.h"
#include "main.h"
#include <camera.h>
#include "freecam_engine.h"

FreeCam freeCam;

#include <math.h>

f32 gDampValue = 0.99f;

void freecam_update(Camera *camera, Vec3f forwardVector) {
    // Update FreeCam state
    camera->pos[0] += freeCam.velocity[0] * gDeltaTime;
    camera->pos[1] += freeCam.velocity[1] * gDeltaTime;
    camera->pos[2] += freeCam.velocity[2] * gDeltaTime;

    // Apply damping to velocity
    freeCam.velocity[0] *= gDampValue;
    freeCam.velocity[1] *= gDampValue;
    freeCam.velocity[2] *= gDampValue;

    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[1] = camera->pos[1] + forwardVector[1];
    camera->lookAt[2] = camera->pos[2] + forwardVector[2];
}

// Function to move the camera forward or backward without changing its height
void freecam_move_camera_forward(Camera *camera, f32 moveSpeed, Vec3f forwardVector) {
    // Calculate the forward vector
    forwardVector[0] = camera->lookAt[0] - camera->pos[0];
    //forwardVector[1] = 0;  // Don't change the height
    forwardVector[2] = camera->lookAt[2] - camera->pos[2];

    // Normalize the forward vector
    f32 length = sqrtf(forwardVector[0] * forwardVector[0] + forwardVector[2] * forwardVector[2]);
    if (length != 0) {
        forwardVector[0] /= length;
        forwardVector[2] /= length;
    }

    // Update FreeCam velocity
    freeCam.velocity[0] = forwardVector[0] * moveSpeed;
    freeCam.velocity[2] = forwardVector[2] * moveSpeed;

    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[2] = camera->pos[0] + forwardVector[2];

}


void freecam_pan_camera(Camera *camera, f32 moveSpeed) {
    Vec3f rightVector;
    f32 length;

    // Calculate the right vector for pan movement
    rightVector[0] = camera->lookAt[2] - camera->pos[2];
   // rightVector[1] = 0; // Maintain the same height
    rightVector[2] = -(camera->lookAt[0] - camera->pos[0]);

    // Normalize the right vector
    length = sqrtf(rightVector[0] * rightVector[0] + rightVector[2] * rightVector[2]);
    if (length != 0) {
        rightVector[0] /= length;
        rightVector[2] /= length;
    }

    // Update camera's position
    freeCam.velocity[0] = rightVector[0] * moveSpeed;
    freeCam.velocity[2] = rightVector[2] * moveSpeed;

}




void freecam_calculate_forward_vector_allow_rotation(Camera* camera, Vec3f forwardVector) {
    // Convert yaw from 0-65535 to degrees
    f32 pitch = (camera->rot[2] / 65535.0f) * 360.0f;
    f32 yaw = (camera->rot[1] / 65535.0f) * 360.0f;

    // Convert degrees to radians
    pitch = pitch * M_PI / 180.0f;
    yaw = yaw * M_PI / 180.0f;

    // Calculate the forward vector based on yaw, ignoring pitch to keep height unchanged
    forwardVector[0] = -sinf(yaw);
    forwardVector[1] = -sinf(pitch);  // Keep the height unchanged
    forwardVector[2] = cosf(yaw);
}

// Calculates the forward direction vector based on camera orientation
void freecam_calculate_forward_vector(Camera *camera, Vec3f forwardVector) {
    // Convert yaw from 0-65535 to degrees
    f32 yaw = (camera->rot[0] / 65535.0f) * 360.0f;
    f32 pitch = (camera->rot[2] / 65535.0f) * 360.0f;
    
    // Convert degrees to radians
    yaw = yaw * M_PI / 180.0f;
    pitch = pitch * M_PI / 180.0f;

    // Calculate the forward vector based on yaw and pitch
    forwardVector[0] = -sinf(yaw) * cosf(pitch); // x-component
    forwardVector[1] = sinf(pitch);              // y-component (vertical)
    forwardVector[2] = cosf(yaw) * cosf(pitch);  // z-component
}

// Function to move the camera forward
void freecam_move_camera_up(Camera* camera, f32 distance) {
    // Update the velocity of the camera in the Y direction
    freeCam.velocity[1] += distance;
    camera->lookAt[1] += distance;
}
void freecam_move_camera_down(Camera* camera, f32 distance) {
    // Update the velocity of the camera in the Y direction
    freeCam.velocity[1] += distance;
    camera->lookAt[1] += distance;
}

void freecam_target_player(Camera *camera, u32 playerIndex) {
    Vec3f forwardVector;// = 2.0f;
    Player *player = &gPlayers[playerIndex];

    // Calculate the direction from the player to the camera
    f32 dirX = player->pos[0] - camera->pos[0];
    f32 dirY = player->pos[1] - camera->pos[1];
    f32 dirZ = player->pos[2] - camera->pos[2];

    // Normalize the direction vector (if needed)
    f32 length = sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
    if (length > 0) {
        dirX /= length;
        dirY /= length;
        dirZ /= length;
    }

    // Update the camera's look-at direction
    camera->lookAt[0] = camera->pos[0] + dirX;
    camera->lookAt[1] = camera->pos[1] + dirY;
    camera->lookAt[2] = camera->pos[2] + dirZ;
}
