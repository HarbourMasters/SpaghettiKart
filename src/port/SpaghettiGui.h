#pragma once

#include <libultraship.h>
#include "ship/window/gui/Gui.h"
#include "fast/Fast3dGui.h"
#include "ship/window/Window.h"

class Gui; // <-- forward declare
//class Window;

namespace Ship {
    class SpaghettiGui : public Fast::Fast3dGui {
      public:
        SpaghettiGui() : Fast::Fast3dGui() {}
        SpaghettiGui(std::vector<std::shared_ptr<GuiWindow>> guiWindows) : Fast::Fast3dGui(guiWindows) {}

      protected:
        virtual void DrawMenu() override;
    };
}