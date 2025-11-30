#include <libultraship.h>
#include <window/Window.h>
#include "port/Engine.h"
#include "port/Game.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>
#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"

extern "C" {
#include <macros.h>
#include <defines.h>
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
#include "math_util.h"
#include "skybox_and_splitscreen.h"
#include "freecam.h"
}

#include "engine/courses/Course.h"

typedef struct {
    Vec3f pos;
    Vec3f lookAt;
    Vec3s rot;
} freecamSaveState;

freecamSaveState fState;

Controller fController;

u32 fRankIndex = 0;
u32 fTargetPlayer = false;
u32 fMode; // freecam mode should probably be an enum
u32 fModeInit = false;

u32 bFreecamUseController = false;

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

// This function is no longer used because FreeCamera.cpp
void freecam(Camera* camera, Player* player, s8 index) {
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 length;
    static bool enabled = false; // Tracks previous activation state
    bool freecamEnabled = CVarGetInteger("gFreecam", 0);

    // Freecam can only activate in single player mode
    if (gActiveScreenMode != SCREEN_MODE_1P) {
        enabled = false;
        freecamEnabled = false;
        CVarSetInteger("gFreecam", false);
    }

    if (freecamEnabled && !enabled) {
        enabled = true; // Mark that freecam was activated
        on_freecam();
    } else if (!freecamEnabled && enabled) {
        off_freecam();
        enabled = false; // Reset when freecam is turned off
    }

    // Freecam mode is enabled
    if (enabled && (player == gPlayerOne)) {
        freecam_loop(camera);
    } else {
        // func_8001E45C(camera, player, index);
        // Required if freecam were to use its own camera instead of borrowing the player camera
        // func_8001EE98(gPlayerOne, camera, index);
    }
}

void on_freecam(void) {
    gIsHUDVisible = false;
   // gPlayerOne->type |= PLAYER_CPU;
}

void off_freecam(void) {
    gIsHUDVisible = true;
    gPlayerOne->type &= ~PLAYER_CPU;
}

void freecam_loop(Camera* camera) {
    if ((fController.buttonPressed & L_TRIG) && (fController.buttonPressed & R_TRIG)) {
        // Toggle freecam
        CVarSetInteger("gFreecam", !CVarGetInteger("gFreecam", 0));
    }
    // Calculate forward direction
    freecam_calculate_forward_vector_allow_rotation(camera, freeCam.forwardVector);

    // Adjust camera rotation
    if (fTargetPlayer) {
        freecam_target_player(camera, freeCam.forwardVector);
    } else {
        freecam_mouse_manager(camera, freeCam.forwardVector);
    }

    // Adjust camera position
    freecam_keyboard_manager(camera, freeCam.forwardVector);

    // Apply final position, velocity, and lookAt
    //if (0) {
        //Tour_Tick(camera);
    //} else {
    freecam_tick(camera, freeCam.forwardVector);
    //}
}

void freecam_mouse_manager(Camera* camera, Vec3f forwardVector) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mouse = wnd->GetMouseDelta();
    wnd->SetMouseCapture(false);

    f32 yawChange = 0.0f;
    f32 pitchChange = 0.0f;

    if (bFreecamUseController) {
        // Controller controls
            f32 stickX = ((f32)fController.rightRawStickX);
            f32 stickY = ((f32)fController.rightRawStickY);

            // Sensitivity multipliers
            float controllerSensitivityX = 5.0f; // Adjust as needed
            float controllerSensitivityY = 3.0f;

            // Deadzone handling (ignore tiny stick movements)
            const float deadzone = 0.1f;
            if (fabs(stickX) < deadzone) stickX = 0.0f;
            if (fabs(stickY) < deadzone) stickY = 0.0f;

            // Instead of adding, directly set rotation velocity (so holding gives a steady rotation)
            freeCam.rotVelocity[1] += stickX * controllerSensitivityX;  // Yaw (left/right)
            freeCam.rotVelocity[2] += stickY * controllerSensitivityY;  // Pitch (up/down)
    } else { // Mouse controls
        // Calculate yaw (left/right) and pitch (up/down) changes
        if (wnd->GetMouseState(Ship::LUS_MOUSE_BTN_RIGHT)) {
            yawChange = -mouse.x * MOUSE_SENSITIVITY_X;
            pitchChange = mouse.y * MOUSE_SENSITIVITY_Y;
        }
        // Update rotational velocity based on mouse movement
        freeCam.rotVelocity[1] += yawChange * 65535.0f / (2 * M_PI);  // Yaw (left/right)
        freeCam.rotVelocity[2] += pitchChange * 65535.0f / (2 * M_PI); // Pitch (up/down)
    }
}

f32 gFreecamSpeed = 3.0f;
f32 gFreecamSpeedMultiplier = 2.0f;
bool prevPrev = false;

#define MAX_KEYS 256
static bool prevKeyState[MAX_KEYS] = { false };

// KeyDown function
bool FreecamKeyDown(int virtualKey) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    static bool prevKeyState[256] = { false }; // Store previous key states
    bool isDownNow = false;

    if (wnd->GetWindowBackend() == Ship::WindowBackend::FAST3D_SDL_OPENGL) {
        // Use SDL to check key states
        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        isDownNow = keystate[virtualKey] != 0;
    }
#ifdef _WIN32
    else if (wnd->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
        // Use Windows GetKeyState for DirectX
        SHORT keyState = GetKeyState(virtualKey);
        isDownNow = (keyState & 0x8000) != 0;
    }
#endif

    // Determine if this is a new key press
    bool isKeyDownEvent = isDownNow && !prevKeyState[virtualKey];

    // Update the previous state for this key
    prevKeyState[virtualKey] = isDownNow;

    return isKeyDownEvent;
}

void freecam_keyboard_manager(Camera* camera, Vec3f forwardVector) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    float moveSpeed = gFreecamSpeed;

    // Determine movement direction based on keys pressed
    Vec3f totalMove = { 0.0f, 0.0f, 0.0f };

    // if (keystate[SDL_SCANCODE_F]) {
    //     fMode = true;
    // }

    // // Target a player
    // if (keystate[SDL_SCANCODE_G]) {
    //     fTargetPlayer = false;
    // }
    bool TargetNextPlayer = false, TargetPreviousPlayer = false;
    bool prevNext;
    bool Forward = false, PanLeft = false, Backward = false, PanRight = false;
    bool Up = false, Down = false;
    bool FastMove = false;

    // Use n64 controls for use with a controller
    //! @todo configure this properly
    if (bFreecamUseController) {
        if (fController.buttonDepressed & R_TRIG) {
            fTargetPlayer = true;
        }
        if (fController.buttonDepressed & L_TRIG) {
            fTargetPlayer = false;
        }
        if (fController.buttonPressed & L_JPAD) {
            TargetPreviousPlayer = true;
        }
        if (fController.buttonPressed & R_JPAD) {
            TargetNextPlayer = true;
        }
        if (fController.rawStickY > 10) {
            Forward = true;
        }
        if (fController.rawStickY < -10) {
            Backward = true;
        }
        if (fController.rawStickX > 10) {
            PanRight = true;
        }
        if (fController.rawStickX < -10) {
            PanLeft = true;
        }
        if (fController.button & B_BUTTON) {
            Down = true;
        }
        if (fController.button & A_BUTTON) {
            Up = true;
        }
        if (fController.button & Z_TRIG) {
           FastMove = true;
        }
    }
    // Keyboard and mouse DX
#ifdef _WIN32
    else if (wnd->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
        if (FreecamKeyDown('F')) {
            fTargetPlayer = !fTargetPlayer;
        }
        if (FreecamKeyDown('N')) {
            TargetPreviousPlayer = true;
        }
        if (FreecamKeyDown('M')) {
            TargetNextPlayer = true;
        }
        if (GetKeyState('W') & 0x8000) {
            Forward = true;
        }
        if (GetKeyState('S') & 0x8000) {
            Backward = true;
        }
        if (GetKeyState('D') & 0x8000) {
            PanRight = true;
        }
        if (GetKeyState('A') & 0x8000) {
            PanLeft = true;
        }
        if (GetKeyState(VK_SPACE) & 0x8000) {
            Up = true;
        }
        if (GetKeyState(VK_LSHIFT) & 0x8000 || GetKeyState(VK_RSHIFT) & 0x8000) {
            Down = true;
        }
        // Fast movement with Ctrl
        if (GetKeyState(VK_LCONTROL) || GetKeyState(VK_RCONTROL)) {
            FastMove = true;
        }
        // Keyboard/mouse OpenGL/SDL
    }
#endif
    else if (wnd->GetWindowBackend() == Ship::WindowBackend::FAST3D_SDL_OPENGL) {
        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        if (FreecamKeyDown(SDL_SCANCODE_F)) {
            fTargetPlayer = !fTargetPlayer;
        }
        if (FreecamKeyDown(SDL_SCANCODE_N)) {
            TargetPreviousPlayer = true;
        }
        if (FreecamKeyDown(SDL_SCANCODE_M)) {
            TargetNextPlayer = true;
        }
        if (keystate[SDL_SCANCODE_W]) {
            Forward = true;
        }
        if (keystate[SDL_SCANCODE_S]) {
            Backward = true;
        }
        if (keystate[SDL_SCANCODE_D]) {
            PanRight = true;
        }
        if (keystate[SDL_SCANCODE_A]) {
            PanLeft = true;
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
            Up = true;
        }
        if (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
            Down = true;
        }
        if (keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) {
            FastMove = true;
        }
    }

    // Target previous player
    if (TargetPreviousPlayer) {
        if (fRankIndex > 0) {
            fRankIndex--;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    // Target next player
    if (TargetNextPlayer) {
        if (fRankIndex < 7) {
            fRankIndex++;
            camera->playerId = fRankIndex;
            D_800DC5EC->player = &gPlayers[fRankIndex];
        }
    }

    if (FastMove) {
        moveSpeed *= gFreecamSpeedMultiplier;
    }

    if (Forward) {
        totalMove[0] += forwardVector[0] * moveSpeed;
        totalMove[2] += forwardVector[2] * moveSpeed;
    }
    if (Backward) {
        totalMove[0] -= forwardVector[0] * moveSpeed;
        totalMove[2] -= forwardVector[2] * moveSpeed;
    }
    if (PanRight) {
        totalMove[0] -= forwardVector[2] * moveSpeed; // Pan right
        totalMove[2] += forwardVector[0] * moveSpeed;
    }
    if (PanLeft) {
        totalMove[0] += forwardVector[2] * moveSpeed; // Pan left
        totalMove[2] -= forwardVector[0] * moveSpeed;
    }
    if (Up) {
        totalMove[1] += moveSpeed; // Move up
    }
    if (Down) {
        totalMove[1] -= moveSpeed; // Move down
    }

    freeCam.velocity[0] += totalMove[0];
    freeCam.velocity[1] += totalMove[1];
    freeCam.velocity[2] += totalMove[2];
}

void freecam_update_controller(void) {
    fController.rawStickX = gControllerPads[0].stick_x;
    fController.rawStickY = gControllerPads[0].stick_y;

    fController.rightRawStickX = gControllerPads[0].right_stick_x;
    fController.rightRawStickY = gControllerPads[0].right_stick_y;

    if ((gControllerPads[0].button & 4) != 0) {
        gControllerPads[0].button |= Z_TRIG;
    }
    fController.buttonPressed = gControllerPads[0].button & (gControllerPads[0].button ^ fController.button);
    fController.buttonDepressed = fController.button & (gControllerPads[0].button ^ fController.button);
    fController.button = gControllerPads[0].button;

    // Note that D Pad as stick code has been removed. So if it's needed, it needs to be put back in.
}

