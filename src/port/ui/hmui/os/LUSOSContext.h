#pragma once

#include <unordered_map>
#include "hmui/src/hmui/os/OSContext.h"

class LUSOSContext : public OSContext {
public:
    void init() override;
    void update() override;
    void dispose() override;
    Coord getMouseDelta() override;
    Coord getMousePosition() override;
    void setMousePosition(Coord& pos) override;
    Coord getMouseWheel() override;
    bool isMouseButtonPressed(int button) override;
    bool isMouseButtonReleased(int button) override;
    bool isMouseButtonDown(int button) override;
    void setMouseCursor(int cursor) override;
    bool isTouchDevice() override;
    bool isTouchActive() override;
    void setClipboardText(const char* text) override;
    const char* getClipboardText() override;
    void showCursor(bool show) override;
    bool isGamepadAvailable(int id) override;
    bool isGamepadButtonPressed(int id, ControllerButton button) override;
    float getGamepadAxis(int id, ControllerAxis axis) override;
    bool isKeyboardButtonPressed(int virtualKey) override;
    uint16_t getButtons(int id) override;
    bool isBackButtonPressed(int id) override;
    ~LUSOSContext() override = default;
private:
    std::unordered_map<int, bool> m_LastMouseState;
    std::unordered_map<int, bool> m_CurrentMouseState;
};
