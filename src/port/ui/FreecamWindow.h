#pragma once

#include <libultraship/libultraship.h>

namespace Freecam {
class FreecamWindow : public Ship::GuiWindow {
  public:
    using Ship::GuiWindow::GuiWindow;
    ~FreecamWindow();

  private:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
};
} // namespace GameInfo