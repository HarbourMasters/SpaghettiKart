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
f32 gRotDampValue = 0.96f;

// Update FreeCam state
void freecam_tick(Camera* camera, Vec3f forwardVector) {

    // Apply camera movement
    camera->pos[0] += freeCam.velocity[0] * gDeltaTime;
    camera->pos[1] += freeCam.velocity[1] * gDeltaTime;
    camera->pos[2] += freeCam.velocity[2] * gDeltaTime;

    // Damp the velocity back to zero over time (camera slowly comes to a stop)
    freeCam.velocity[0] *= gDampValue;
    freeCam.velocity[1] *= gDampValue;
    freeCam.velocity[2] *= gDampValue;

    // Apply camera rotation
    camera->rot[0] += freeCam.rotVelocity[0] * gDeltaTime;
    camera->rot[1] += freeCam.rotVelocity[1] * gDeltaTime;
    camera->rot[2] += freeCam.rotVelocity[2] * gDeltaTime;

    // Damp the velocity back to zero over time (camera rot slowly comes to a stop)
    freeCam.rotVelocity[0] *= gRotDampValue;
    freeCam.rotVelocity[1] *= gRotDampValue;
    freeCam.rotVelocity[2] *= gRotDampValue;

    // Update lookAt
    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[1] = camera->pos[1] + forwardVector[1];
    camera->lookAt[2] = camera->pos[2] + forwardVector[2];
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
    forwardVector[1] = -sinf(pitch); // Keep the height unchanged
    forwardVector[2] = cosf(yaw);
}

f32 gFreecamFollowFactor = 0.7;

void freecam_target_player(Camera* camera, Vec3f forwardVector) {
    // Apply damping to velocity
    freeCam.velocity[0] *= gDampValue;
    freeCam.velocity[1] *= gDampValue;
    freeCam.velocity[2] *= gDampValue;

    // Calculate the direction from the player to the camera
    f32 dirX = gPlayers[fRankIndex].pos[0] - camera->pos[0];
    f32 dirY = gPlayers[fRankIndex].pos[1] - camera->pos[1];
    f32 dirZ = gPlayers[fRankIndex].pos[2] - camera->pos[2];

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

    // Calculate the forward vector based on the updated look-at direction
    forwardVector[0] = camera->lookAt[0] - camera->pos[0];
    forwardVector[1] = camera->lookAt[1] - camera->pos[1];
    forwardVector[2] = camera->lookAt[2] - camera->pos[2];

    // Normalize the forward vector
    f32 forwardLength = sqrtf(forwardVector[0] * forwardVector[0] + forwardVector[1] * forwardVector[1] +
                              forwardVector[2] * forwardVector[2]);
    if (forwardLength > 0.0f) {
        forwardVector[0] /= forwardLength;
        forwardVector[1] /= forwardLength;
        forwardVector[2] /= forwardLength;
    }

    // Store or return the forward vector as needed for subsequent movement calculations
    freeCam.forwardVector[0] = forwardVector[0];
    freeCam.forwardVector[1] = forwardVector[1];
    freeCam.forwardVector[2] = forwardVector[2];

    // Movement
    freeCam.velocity[0] += gPlayers[fRankIndex].velocity[0] * gFreecamFollowFactor;
    freeCam.velocity[1] += gPlayers[fRankIndex].velocity[1] * gFreecamFollowFactor;
    freeCam.velocity[2] += gPlayers[fRankIndex].velocity[2] * gFreecamFollowFactor;
}

void freecam_get_player_from_character(s32 characterId) {
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        if (gPlayers[i].characterId == characterId) {
            fRankIndex = i;
        }
    }
}