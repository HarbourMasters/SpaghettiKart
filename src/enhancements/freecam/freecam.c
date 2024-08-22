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
#include <SDL2/SDL.h>
#include "freecam_engine.h"
#include "n64_freecam.h"

typedef struct {
    Vec3f pos;
    Vec3f lookAt;
    Vec3s rot;
} freecamSaveState;

freecamSaveState fState;

u32 fRankIndex = -1;
u32 fTargetPlayer = false;
u32 fMode; // freecam mode should probably be an enum
u32 fModeInit = false;

int rightMouseButtonDown = 0; // Track if right mouse button is held down

u32 gFreecamControllerType = 0;

// void freecam_n64_calculate_forward_vector(Camera* camera, Vec3f forwardVector);
// void freecam_n64_move_camera_forward(Camera* camera, struct Controller *controller, f32 distance);
// void freecam_calculate_forward_vector(Camera* camera, Vec3f forwardVector);
// void freecam_move_camera_forward(Camera* camera, f32 distance);

// void freecam_update(Camera* camera, struct Controller *controller);
// void freecam_n64_controller_manager(Camera *camera, struct Controller *controller, Player *player);
// void freecam_target_player(Camera *camera, u32 playerIndex);
// void freecam_move_camera_up(Camera* camera, struct Controller *controller, f32 distance);
void freecam_save_state(Camera *camera);
void freecam_load_state(Camera *camera);
void freecam_mouse_manager(Camera *camera);
void freecam_keyboard_manager(Camera* camera, Vec3f forwardVector);

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
 * Camera mode 1: Enter freecam at the player's position
 * Camera mode 2: Enter freecam at previous freecam spot
 *
*/

void freecam(Camera *camera, Player *player, s8 index) {
    struct Controller *controller = &gControllers[0];
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 length;

    if (controller->buttonPressed & L_TRIG) {
        // Toggle freecam
        CVarSetInteger("gFreecam", !CVarGetInteger("gFreecam", 0));

        // Don't use `bool = !bool` here as the game code can swap these on you.
        // Which will confuse the code. This forces it to always be correct
        if (CVarGetInteger("gFreecam", 0) == 1) {
            player->type |= PLAYER_KART_AI;
        } else {
            player->type &= PLAYER_KART_AI;
        }

        gIsHUDVisible = !CVarGetInteger("gFreecam", 0);

        if (CVarGetInteger("gFreecam", 0) == 1) {

            if (fMode && fModeInit) {
                freecam_load_state(camera);
            } else {
                // !fMode or fMode not initialized
                freecam_target_player(camera, get_player_index_for_player(player));
            }

            return;
        } else {
            if(fMode) {
                freecam_save_state(camera);
            }
        }
    }

    // Driving mode
    if (CVarGetInteger("gFreecam", 0) == 0) {
        // Use normal camera code
        func_8001E45C(camera, &gPlayers[fRankIndex], index);
        return;
    }

    //if (player == gPlayerOne) { return; }

    //player->type &= ~PLAYER_HUMAN;
    //player->type |= PLAYER_HUMAN;

    // if ((player->type & PLAYER_START_SEQUENCE)) { return; }


    // Mouse/Keyboard
    if (gFreecamControllerType == 0) {
        freecam_calculate_forward_vector_allow_rotation(camera, freeCam.forwardVector);
        freecam_mouse_manager(camera, freeCam.forwardVector);
        freecam_keyboard_manager(camera, freeCam.forwardVector);
    } else { // Stock N64 controller
        freecam_n64_controller_manager(camera, controller, player);
    }
    if (!fTargetPlayer) {
        freecam_update(camera, freeCam.forwardVector);
    }
}

void freecam_save_state(Camera *camera) {
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

void freecam_load_state(Camera *camera) {
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

f32 gFreecamRotateSmoothingFactor = 0.85f;

void freecam_mouse_manager(Camera* camera, Vec3f forwardVector) {
    int mouseX, mouseY;
    static int prevMouseX = 0, prevMouseY = 0;
    Uint32 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

    mouseX = (mouseX + prevMouseX) / 2;
    mouseY = (mouseY + prevMouseY) / 2;
    prevMouseX = mouseX;
    prevMouseY = mouseY;

    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        // Calculate yaw (left/right) and pitch (up/down) changes
        f32 yawChange = mouseX * MOUSE_SENSITIVITY_X;
        f32 pitchChange = mouseY * MOUSE_SENSITIVITY_Y;

        // Smoothly update yaw and pitch
        camera->rot[1] += (short)(yawChange * 65535.0f / (2 * M_PI)); // Yaw (left/right)
        camera->rot[2] += (short)(pitchChange * 65535.0f / (2 * M_PI)); // Pitch (up/down)

        // Clamp pitch to avoid extreme values
        if (camera->rot[2] > 15999) camera->rot[2] = 15999;
        if (camera->rot[2] < -15999) camera->rot[2] = -15999;

        // Calculate the forward vector based on the new yaw and pitch
        //freecam_calculate_forward_vector_allow_rotation(camera, forwardVector);

        // Smoothly interpolate the lookAt position
        Vec3f targetLookAt = {
            camera->pos[0] + forwardVector[0],
            camera->pos[1] + forwardVector[1],
            camera->pos[2] + forwardVector[2]
        };

        // Smoothing
        camera->lookAt[0] += (targetLookAt[0] - camera->lookAt[0]) * gFreecamRotateSmoothingFactor;
        camera->lookAt[1] += (targetLookAt[1] - camera->lookAt[1]) * gFreecamRotateSmoothingFactor;
        camera->lookAt[2] += (targetLookAt[2] - camera->lookAt[2]) * gFreecamRotateSmoothingFactor;
    }
}


f32 gFreecamSpeed = 3.0f;
f32 gFreecamSpeedMultiplier = 2.0f;

void freecam_keyboard_manager(Camera* camera, Vec3f forwardVector) {
    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    float moveSpeed = gFreecamSpeed;

    // Fast movement with Ctrl
    if (keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) {
        moveSpeed *= gFreecamSpeedMultiplier;
    }

    // Determine movement direction based on keys pressed
    Vec3f totalMove = {0.0f, 0.0f, 0.0f};


    // if (keystate[SDL_SCANCODE_F]) {
    //     fMode = true;
    // }

    // // Target a player
    // if (keystate[SDL_SCANCODE_G]) {
    //     fTargetPlayer = false;
    // }

    // Target next player
    if (keystate[SDL_SCANCODE_N]) {
        if (fRankIndex > 0) {
            fRankIndex--;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    // Target previous player
    if (keystate[SDL_SCANCODE_M]) {
        if (fRankIndex < 7) {
            fRankIndex++;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    // Target camera at chosen player
    if (fRankIndex != -1) {
        freecam_target_player(camera, gGPCurrentRacePlayerIdByRank[fRankIndex]);
        // Don't run the other camera code.
        return;
    }


    if (keystate[SDL_SCANCODE_W]) {
        totalMove[0] += forwardVector[0] * moveSpeed;
        totalMove[2] += forwardVector[2] * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_S]) {
        totalMove[0] -= forwardVector[0] * moveSpeed;
        totalMove[2] -= forwardVector[2] * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_D]) {
        totalMove[0] -= forwardVector[2] * moveSpeed; // Pan right
        totalMove[2] += forwardVector[0] * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_A]) {
        totalMove[0] += forwardVector[2] * moveSpeed; // Pan left
        totalMove[2] -= forwardVector[0] * moveSpeed;
    }
    if (keystate[SDL_SCANCODE_SPACE]) {
        totalMove[1] += moveSpeed; // Move up
    }
    if (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
        totalMove[1] -= moveSpeed; // Move down
    }
    freeCam.velocity[0] += totalMove[0];
    freeCam.velocity[1] += totalMove[1];
    freeCam.velocity[2] += totalMove[2];

    // Update camera's lookAt position
    camera->lookAt[0] = camera->pos[0] + forwardVector[0];
    camera->lookAt[2] = camera->pos[2] + forwardVector[2];
}

// Control the camera using the n64 controller
void freecam_n64_controller_manager(Camera *camera, struct Controller *controller, Player *player) {

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
        freecam_target_player(camera, gGPCurrentRacePlayerIdByRank[fRankIndex]);
        // Don't run the other camera code.
        return;
    }

    // Rotation
    if (!fTargetPlayer) {
        if (controller->stickDirection != 0) {
            freecam_n64_update(camera, controller);
        }
    }

    // Forward
    if (controller->button & A_BUTTON)  {
        freecam_n64_move_camera_forward(camera, controller, 3.0f);
    }

    // Backward     B button but not A button.
    if (controller->button & B_BUTTON && !(controller->button & A_BUTTON))  {
        freecam_n64_move_camera_forward(camera, controller, -3.0f);
    }

    // Up
    if (controller->button & U_CBUTTONS) {
        freecam_n64_move_camera_up(camera, controller, 2.0f);
    }
    // Up
    if (controller->button & D_CBUTTONS) {
        freecam_n64_move_camera_up(camera, controller, -2.0f);
    }
}

void freecam_render_setup(void) {
        u16 perspNorm;
        Mat4 matrix;
        init_rdp();
        func_802A53A4();
        init_rdp();
        func_80057FC4(0);
        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_CULL_BOTH | G_CULL_FRONT);
        guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, D_80150150, D_8015014C, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, perspNorm);
        gSPMatrix(gDisplayListHead++, (&gGfxPool->mtxPersp[0]), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        guLookAt(&gGfxPool->mtxLookAt[0], camera1->pos[0], camera1->pos[1], camera1->pos[2], camera1->lookAt[0], camera1->lookAt[1], camera1->lookAt[2], camera1->up[0], camera1->up[1], camera1->up[2]);
        gSPMatrix(gDisplayListHead++, (&gGfxPool->mtxLookAt[0]), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
        render_set_position(matrix, 0);
        init_rdp();
}
