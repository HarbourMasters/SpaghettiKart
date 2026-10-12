#pragma once

#include <libultraship/libultraship.h>

namespace TrackEditor {
class ToolsWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;

    ~ToolsWindow();
protected:
    void OnInit(const nlohmann::json& initArgs = {}) override;
    void DrawElement() override;
    void UpdateElement() override {};
    void GameSpeed();
};
}
