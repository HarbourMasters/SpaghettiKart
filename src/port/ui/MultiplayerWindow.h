#pragma once

#include <libultraship/libultraship.h>

namespace Multiplayer {
class MultiplayerWindow : public Ship::GuiWindow {
  public:
    using Ship::GuiWindow::GuiWindow;
    ~MultiplayerWindow();

  private:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
};
} // namespace Multiplayer
