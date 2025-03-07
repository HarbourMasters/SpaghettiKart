#include "EditorWindow.h"
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

namespace GameUI {
    
    extern std::shared_ptr<PortMenu> mPortMenu;
    
    void RegisterEditorWidgets() {
        mPortMenu->AddSidebarEntry("Enhancements", "HM64 Lab", 4);
        WidgetPath path = { "Enhancements", "HM64 Lab", SECTION_COLUMN_1 };
        mPortMenu->AddWidget(path, "Enable HM64 Labs", WIDGET_CVAR_CHECKBOX)
        .CVar("gEditor")
        .Options(UIWidgets::CheckboxOptions({{ .tooltip = "Edit the universe!"}}));
    }
    
static RegisterMenuInitFunc initFunc(RegisterEditorWidgets);

} // namespace GameUI

namespace EditorWin {

    EditorWindow::~EditorWindow() {
        SPDLOG_TRACE("destruct editor window");
    }

    void EditorWindow::DrawElement() {
        auto viewport = ImGui::GetMainViewport();
        ImGuiID mainDock = ImGui::GetID("main_dock");

        EditorWindow::SetupLayout();
        EditorWindow::DrawContentBrowser(viewport, mainDock);
        EditorWindow::DrawSceneExplorer(viewport, mainDock);


    }

    void EditorWindow::SetupLayout() {

    }

    void EditorWindow::DrawContentBrowser(ImGuiViewport* viewport, ImGuiID mainDock) {
        ImGuiID bottomId = 0;

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {

            if (!ImGui::DockBuilderGetNode(mainDock)) {                
                bottomId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.25f, nullptr, nullptr);
                ImGui::DockBuilderSetNodeSize(bottomId, ImVec2(viewport->Size.x * 0.2f, viewport->Size.y));





                ImGui::DockBuilderDockWindow("Content Browser", bottomId);
                ImGui::DockBuilderFinish(mainDock);
            }
        }

        if (ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
    
            ImGui::Text("This is your Content browser Lab editor window!");
            if (ImGui::Button("Click Me")) {
                // Handle button click (example)
            }
            ImGui::End();
        }
    }

    void EditorWindow::DrawSceneExplorer(ImGuiViewport* viewport, ImGuiID mainDock) {
        ImGuiID rightId = 0;

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {

            if (!ImGui::DockBuilderGetNode(mainDock)) {                
                rightId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Right, 0.25f, nullptr, nullptr);
                ImGui::DockBuilderSetNodeSize(rightId, ImVec2(viewport->Size.x * 0.2f, viewport->Size.y));





                ImGui::DockBuilderDockWindow("HM64 Lab", rightId);
                ImGui::DockBuilderFinish(mainDock);
            }
        }

        if (ImGui::Begin("HM64 Lab", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
    
            ImGui::Text("This is your HM64 Lab editor window!");
            if (ImGui::Button("Click Me")) {
                // Handle button click (example)
            }
            ImGui::End();
        }
    }
}
