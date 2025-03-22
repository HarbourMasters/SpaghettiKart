#pragma once

#include <libultraship/libultraship.h>
#include "port/Game.h"


namespace Editor {
class PropertiesWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~PropertiesWindow();
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
};
}