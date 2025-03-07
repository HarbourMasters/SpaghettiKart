#pragma once

#include <libultraship/libultraship.h>

namespace Editor {
class ToolsWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;

    ~ToolsWindow();
protected:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override {};
};
}
