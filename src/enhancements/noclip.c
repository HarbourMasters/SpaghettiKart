#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <camera.h>
#include "main.h"
#include <libc/math.h>
#include <common_structs.h>
#include "racing/collision.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"

// Yaw/pitch rotation sensitivity
#define SENSITIVITY_X 0.0003f
#define SENSITIVITY_Y 0.0003f

u32 fRankIndex = 0;
u32 fTargetPlayer = false;
u32 fMode; // noclip mode should probably be an enum
u32 fModeInit = false;

typedef struct {
    Vec3f pos;
    Vec3f lookAt;
    Vec3s rot;
} NoclipSaveState;

NoclipSaveState fState;

void noclip_calculate_forward_vector(Camera* camera, Vec3f forwardVector);
void noclip_move_camera_forward(Camera* camera, struct Controller *controller, f32 distance);
void noclip_update(Camera* camera, struct Controller *controller);
void noclip_controller_manager(Camera *camera, struct Controller *controller, Player *player);
void noclip_target_player(Camera *camera, u32 playerIndex);
void noclip_move_camera_up(Camera* camera, struct Controller *controller, f32 distance);
void noclip_save_state(Camera *camera);
void noclip_load_state(Camera *camera);


/**
 * Controls
 *
 * Forward: A
 * Backward: B
 *
 * Go faster: Z
 *
 * Up:       C-up
 * Down:     C-down
 *
 * Targets players based on rank position
 *
 * Target player: R-trig
 * Target next player: C-right
 * Target previous player: C-left
 *
 * Switch camera modes:   D-pad left
 *
 * Camera mode 1: Enter noclip at the player's position
 * Camera mode 2: Enter noclip at previous noclip spot
 *
*/

void noclip(Camera *camera, Player *player, s8 index) {
    struct Controller *controller = &gControllers[0];
    Vec3f forwardVector;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 length;

    if (controller->buttonPressed & L_TRIG) {
        // Toggle noclip
        CVarSetInteger("gNoclip", !CVarGetInteger("gNoclip", 0));

        // Don't use `bool = !bool` here as the game code can swap these on you.
        // Which will confuse the code. This forces it to always be correct
        if (CVarGetInteger("gNoclip", 0) == 1) {
            player->type |= PLAYER_KART_AI;
        } else {
            player->type &= PLAYER_KART_AI;
        }

        gIsHUDVisible = !CVarGetInteger("gNoclip", 0);

        if (CVarGetInteger("gNoclip", 0) == 1) {

            if (fMode && fModeInit) {
                noclip_load_state(camera);
            } else {
                // !fMode or fMode not initialized
                noclip_target_player(camera, get_player_index_for_player(player));
            }

            return;
        } else {
            if(fMode) {
                noclip_save_state(camera);
            }
        }
    }

    // Driving mode
    if (CVarGetInteger("gNoclip", 0) == 0) {
        // Use normal camera code
        func_8001E45C(camera, &gPlayers[fRankIndex], index);
        return;
    }


    //if (player == gPlayerOne) { return; }

    //player->type &= ~PLAYER_HUMAN;
    //player->type |= PLAYER_HUMAN;

    if ((player->type & PLAYER_START_SEQUENCE)) { return; }


    noclip_controller_manager(camera, controller, player);




}

void noclip_save_state(Camera *camera) {
    fState.pos[0] = camera->pos[0];
    fState.pos[1] = camera->pos[1];
    fState.pos[2] = camera->pos[2];

    fState.lookAt[0] = camera->lookAt[0];
    fState.lookAt[1] = camera->lookAt[1];
    fState.lookAt[2] = camera->lookAt[2];

    fState.rot[0] = camera->rot[0];
    fState.rot[1] = camera->rot[1];
    fState.rot[2] = camera->rot[2];
    fModeInit = true;
}

void noclip_load_state(Camera *camera) {
    camera->pos[0] = fState.pos[0];
    camera->pos[1] = fState.pos[1];
    camera->pos[2] = fState.pos[2];

    camera->lookAt[0] = fState.lookAt[0];
    camera->lookAt[1] = fState.lookAt[1];
    camera->lookAt[2] = fState.lookAt[2];

    camera->rot[0] = fState.rot[0];
    camera->rot[1] = fState.rot[1];
    camera->rot[2] = fState.rot[2];
}

void noclip_controller_manager(Camera *camera, struct Controller *controller, Player *player) {

    if (controller->buttonPressed & U_JPAD) {
        fMode = !fMode;
    }

    // Target a player
    if (controller->buttonPressed & R_TRIG) {
        fTargetPlayer = !fTargetPlayer;
    }

    // Target next player
    if (controller->buttonPressed & L_CBUTTONS) {
        if (fRankIndex > 0) {
            fRankIndex--;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    // Target previous player
    if (controller->buttonPressed & R_CBUTTONS) {
        if (fRankIndex < 7) {
            fRankIndex++;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    // Target camera at chosen player
    if (fTargetPlayer) {
        noclip_target_player(camera, gGPCurrentRacePlayerIdByRank[fRankIndex]);
        // Don't run the other camera code.
        return;
    }

    // Rotation
    if (!fTargetPlayer) {
        if (controller->stickDirection != 0) {
            noclip_update(camera, controller);
        }
    }

    // Forward
    if (controller->button & A_BUTTON)  {
        noclip_move_camera_forward(camera, controller, 3.0f);
    }

    // Backward     B button but not A button.
    if (controller->button & B_BUTTON && !(controller->button & A_BUTTON))  {
        noclip_move_camera_forward(camera, controller, -3.0f);
    }

    // Up
    if (controller->button & U_CBUTTONS) {
        noclip_move_camera_up(camera, controller, 2.0f);
    }
    // Up
    if (controller->button & D_CBUTTONS) {
        noclip_move_camera_up(camera, controller, -2.0f);
    }
}

// Calculates the forward direction vector based on camera orientation
void noclip_calculate_forward_vector(Camera* camera, Vec3f forwardVector) {
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
void noclip_move_camera_forward(Camera* camera, struct Controller *controller, f32 distance) {
    Vec3f forwardVector;
    Vec3f rightVector;
    f32 length;
    noclip_calculate_forward_vector(camera, forwardVector);

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
void noclip_move_camera_up(Camera* camera, struct Controller *controller, f32 distance) {
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
void noclip_update(Camera* camera, struct Controller *controller) {
    // Calculate yaw (horizontal movement)
    f32 yawChange = controller->rawStickX * SENSITIVITY_X;
    f32 pitchChange = controller->rawStickY * SENSITIVITY_Y;
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
    noclip_calculate_forward_vector(camera, forwardVector);
    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[1] = camera->pos[1] + forwardVector[1];
    camera->lookAt[2] = camera->pos[2] + forwardVector[2];
}

void noclip_target_player(Camera *camera, u32 playerIndex) {
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
