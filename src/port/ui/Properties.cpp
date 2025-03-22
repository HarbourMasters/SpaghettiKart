#include "Properties.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "engine/editor/Editor.h"
#include "port/Game.h"

namespace Editor {

    PropertiesWindow::~PropertiesWindow() {
        SPDLOG_TRACE("destruct scene explorer window");
    }

    void PropertiesWindow::DrawElement() {
    }
}
