#include "LUSOSContext.h"
#include "libultraship.h"
#include "Engine.h"
#include "freecam/freecam.h"

#include "main.h"
#define MAX_AXIS_VALUE 85.0f

void LUSOSContext::init() {}
void LUSOSContext::update() {
    auto wnd = GameEngine::Instance->context->GetWindow();

    for (int i = 0; i < 3; i++) {
        m_LastMouseState[i] = m_CurrentMouseState[i];
        m_CurrentMouseState[i] = wnd->GetMouseState(static_cast<Ship::MouseBtn>(i));
    }
}
void LUSOSContext::dispose() {}

Coord LUSOSContext::getMouseDelta() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords delta = wnd->GetMouseDelta();
    return Coord(static_cast<float>(delta.x), static_cast<float>(delta.y));
}

Coord LUSOSContext::getMousePosition() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mouse = wnd->GetMousePos();
    return Coord(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void LUSOSContext::setMousePosition(Coord& pos) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mousePos = { static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y) };
    wnd->SetMousePos(mousePos);
}

Coord LUSOSContext::getMouseWheel() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::CoordsF wheel = wnd->GetMouseWheel();
    return Coord(wheel.x * 4.0f, wheel.y * 4.0f);
}

bool LUSOSContext::isMouseButtonPressed(int button) {
    return m_CurrentMouseState[button] && !m_LastMouseState[button];
}

bool LUSOSContext::isMouseButtonReleased(int button) {
    return !m_CurrentMouseState[button] && m_LastMouseState[button];
}

bool LUSOSContext::isMouseButtonDown(int button) {
    return m_CurrentMouseState[button];
}

void LUSOSContext::setMouseCursor(int cursor) {
    // Not implemented
}

void LUSOSContext::setClipboardText(const char* text) {
    // Not implemented
}

bool LUSOSContext::isTouchDevice() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    // Not implemented
    return false;
}

bool LUSOSContext::isTouchActive() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    // Not implemented
    return false;
}

const char* LUSOSContext::getClipboardText() {
    // Not implemented
    return "";
}

void LUSOSContext::showCursor(bool show) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    wnd->SetCursorVisibility(show);
}

bool LUSOSContext::isGamepadAvailable(int id) { 
    return true;
}

bool LUSOSContext::isGamepadButtonPressed(int id, ControllerButton button) {
    switch(button) {
        case ControllerButton::LEFT_FACE_UP:
            return gControllers[id].buttonPressed & BTN_DUP;
        case ControllerButton::LEFT_FACE_RIGHT:
            return gControllers[id].buttonPressed & BTN_DRIGHT;
        case ControllerButton::LEFT_FACE_DOWN:
            return gControllers[id].buttonPressed & BTN_DDOWN;
        case ControllerButton::LEFT_FACE_LEFT:
            return gControllers[id].buttonPressed & BTN_DLEFT;
        case ControllerButton::RIGHT_FACE_UP:
            return false;
        case ControllerButton::RIGHT_FACE_RIGHT:
            return false;
        case ControllerButton::RIGHT_FACE_DOWN:
            return gControllers[id].buttonPressed & BTN_A;
        case ControllerButton::RIGHT_FACE_LEFT:
            return gControllers[id].buttonPressed & BTN_B;
        case ControllerButton::LEFT_TRIGGER_1:
            return gControllers[id].buttonPressed & BTN_L;
        case ControllerButton::RIGHT_TRIGGER_1:
            return gControllers[id].buttonPressed & BTN_R;
        case ControllerButton::LEFT_TRIGGER_2:
            return gControllers[id].buttonPressed & BTN_Z;
        case ControllerButton::RIGHT_TRIGGER_2:
            return gControllers[id].buttonPressed & BTN_Z;
        case ControllerButton::MIDDLE_LEFT:
        case ControllerButton::MIDDLE:
            return gControllers[id].buttonPressed & BTN_START;
        case ControllerButton::MIDDLE_RIGHT:
            // Not implemented
            break;
    }

    return false;
}

uint16_t LUSOSContext::getButtons(int id) {
    return gControllers[id].buttonDepressed;
}

bool LUSOSContext::isBackButtonPressed(int id) {
    bool pressed = gControllers[id].buttonDepressed & B_BUTTON;
    if (pressed) {
        return pressed;
    }
#define SDL_SCANCODE_BACKSPACE 42
#define VK_BACK 8
#ifdef _WIN32
    if (isKeyboardButtonPressed(VK_BACK) || isMouseButtonPressed(2)) { // right click
#else
    if (isKeyboardButtonPressed(SDL_SCANCODE_BACKSPACE) || isMouseButtonPressed(2)) {
#endif
        return true;
    }
}

float LUSOSContext::getGamepadAxis(int id, ControllerAxis axis) {
    switch(axis) {
        case ControllerAxis::LEFT_X:
            return gControllers[id].rawStickX / MAX_AXIS_VALUE;
        case ControllerAxis::LEFT_Y:
            return -gControllers[id].rawStickY / MAX_AXIS_VALUE;
        case ControllerAxis::RIGHT_X:
            return gControllers[id].rightRawStickX / MAX_AXIS_VALUE;
        case ControllerAxis::RIGHT_Y:
            return -gControllers[id].rightRawStickY / MAX_AXIS_VALUE;
        case ControllerAxis::LEFT_TRIGGER:
            return (gControllers[id].button & BTN_L) ? 1.0f : 0.0f;
        case ControllerAxis::RIGHT_TRIGGER:
            return (gControllers[id].button & BTN_R) ? 1.0f : 0.0f;
    }

    return 0.0f;
}

// Usage isKeyboardButtonPressed('A') for keyboard A button
bool LUSOSContext::isKeyboardButtonPressed(int virtualKey) {
    return FreecamKeyDown(virtualKey);
}
