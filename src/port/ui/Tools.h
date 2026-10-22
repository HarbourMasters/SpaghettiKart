#pragma once

#include <libultraship/libultraship.h>

namespace TrackEditor {
class ToolsWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;

    ~ToolsWindow() override;

  protected:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override {};
    void GameSpeed();
};
}
