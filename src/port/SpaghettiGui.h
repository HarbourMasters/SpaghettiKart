#pragma once

#include <libultraship.h>
#include "ship/window/gui/Gui.h"
#include "fast/Fast3dGui.h"
#include "ship/window/Window.h"

namespace Ship {
    class SpaghettiGui : public Fast::Fast3dGui {
      public:
        SpaghettiGui() : Fast::Fast3dGui() {}
        SpaghettiGui(std::vector<std::shared_ptr<GuiWindow>> guiWindows) : Fast::Fast3dGui(guiWindows) {}

      protected:
        void DrawMenu() override;
    };
}