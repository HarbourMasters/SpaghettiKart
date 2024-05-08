#pragma once
#include <libultraship/libultraship.h>

namespace GameUI {
    void SetupGuiElements();
    void Destroy();
}

class GameMenuBar : public Ship::GuiMenuBar {
  public:
    using Ship::GuiMenuBar::GuiMenuBar;
  protected:
    void DrawElement() override;
    void InitElement() override {};
    void UpdateElement() override {};
};