#include "ContentBrowser.h"
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

namespace EditorNamespace {

    ContentBrowserWindow::~ContentBrowserWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void ContentBrowserWindow::DrawElement() {
        ImGui::Text("This is your Content browser Lab editor window!");
        if (ImGui::Button("Click Me")) {
            // Handle button click (example)
        }
    }
}
