#pragma once

#include <libultraship/libultraship.h>

namespace GameInfo {
class GameInfoWindow : public Ship::GuiWindow {
  public:
    using Ship::GuiWindow::GuiWindow;
    ~GameInfoWindow();

  private:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
};
} // namespace GameInfo
