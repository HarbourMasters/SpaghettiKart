#pragma once

#include <libultraship.h>
#include "libultraship/src/window/gui/Gui.h"
#include "libultraship/src/window/Window.h"

class Gui; // <-- forward declare
//class Window;

namespace Ship {
    class SpaghettiGui : public Gui {
      public:
        SpaghettiGui() : Gui() {}
        SpaghettiGui(std::vector<std::shared_ptr<GuiWindow>> guiWindows) : Gui(guiWindows) {}

      protected:
        virtual void DrawMenu() override;
    };
}