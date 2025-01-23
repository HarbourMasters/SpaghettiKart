#include <libultraship.h>
#include <window/Window.h>
#include "port/Engine.h"
#include "port/Game.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>

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

u32 fRankIndex = 0;
u32 fTargetPlayer = false;
u32 fMode; // freecam mode should probably be an enum
u32 fModeInit = false;

int rightMouseButtonDown = 0; // Track if right mouse button is held down

u32 gFreecamControllerType = 0;

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
void freecam(Camera* camera, Player* player, s8 index) {
    struct Controller* controller = &gControllers[0];
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
    freecam_tick(camera, freeCam.forwardVector);
}

f32 gFreecamRotateSmoothingFactor = 0.85f;

void freecam_mouse_manager(Camera* camera, Vec3f forwardVector) {
    static int prevMouseX = 0, prevMouseY = 0;
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mouse = wnd->GetMouseDelta();

    // Uint32 mouseState = SDL_GetRelativeMouseState(&mouse.x, &mouse.y);

    // printf("MOUSE %d %d\n", mouse.x, mouse.y);

    mouse.x = (mouse.x + prevMouseX) / 2;
    mouse.y = (mouse.y + prevMouseY) / 2;
    prevMouseX = mouse.x;
    prevMouseY = mouse.y;

    if (wnd->GetMouseState(Ship::LUS_MOUSE_BTN_RIGHT)) {
        // Calculate yaw (left/right) and pitch (up/down) changes
        f32 yawChange = mouse.x * MOUSE_SENSITIVITY_X;
        f32 pitchChange = mouse.y * MOUSE_SENSITIVITY_Y;

        // Smoothly update yaw and pitch
        camera->rot[1] += (short) (yawChange * 65535.0f / (2 * M_PI));   // Yaw (left/right)
        camera->rot[2] += (short) (pitchChange * 65535.0f / (2 * M_PI)); // Pitch (up/down)

        // Clamp pitch to avoid extreme values
        if (camera->rot[2] > 15999) {
            camera->rot[2] = 15999;
        }
        if (camera->rot[2] < -15999) {
            camera->rot[2] = -15999;
        }

        // Smoothly interpolate the lookAt position
        Vec3f targetLookAt = { camera->pos[0] + forwardVector[0], camera->pos[1] + forwardVector[1],
                               camera->pos[2] + forwardVector[2] };

        // Smoothing
        camera->lookAt[0] += (targetLookAt[0] - camera->lookAt[0]) * gFreecamRotateSmoothingFactor;
        camera->lookAt[1] += (targetLookAt[1] - camera->lookAt[1]) * gFreecamRotateSmoothingFactor;
        camera->lookAt[2] += (targetLookAt[2] - camera->lookAt[2]) * gFreecamRotateSmoothingFactor;
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
    Controller* controller = &gControllers[0];

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
    if (gFreecamControllerType == 1) {
        // Targeting /fMode is broken
        // if (controller->buttonPressed & U_JPAD) {
        //     fMode = !fMode;
        // }
        // Target a player
        if (controller->buttonPressed & R_TRIG) {
            fTargetPlayer = !fTargetPlayer;
        }
        if (controller->buttonPressed & L_CBUTTONS) {
            TargetPreviousPlayer = true;
        }
        if (controller->buttonPressed & R_CBUTTONS) {
            TargetNextPlayer = true;
        }
        if (controller->button & A_BUTTON) {
            Forward = true;
        }
        if (controller->button & B_BUTTON) {
            Backward = true;
        }
        if (controller->button & L_JPAD) {
            PanLeft = true;
        }
        if (controller->button & R_JPAD) {
            PanLeft = true;
        }
        if (controller->button & U_CBUTTONS) {
            Down = true;
        }
        if (controller->button & U_CBUTTONS) {
            Up = true;
        }
        // if (controller->button ??) {
        //    FastMove = true;
        // }
        // Keyboard and mouse DX
    }
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

void freecam_render_setup(void) {
    u16 perspNorm;
    Mat4 matrix;
    init_rdp();
    func_802A53A4();
    init_rdp();
    func_80057FC4(0);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_CULL_BOTH | G_CULL_FRONT);
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect,
                  CM_GetProps()->NearPersp, CM_GetProps()->FarPersp, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, (&gGfxPool->mtxPersp[0]), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera1->pos[0], camera1->pos[1], camera1->pos[2], camera1->lookAt[0],
             camera1->lookAt[1], camera1->lookAt[2], camera1->up[0], camera1->up[1], camera1->up[2]);
    gSPMatrix(gDisplayListHead++, (&gGfxPool->mtxLookAt[0]), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    mtxf_identity(matrix);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    render_set_position(matrix, 0);
    init_rdp();
}
