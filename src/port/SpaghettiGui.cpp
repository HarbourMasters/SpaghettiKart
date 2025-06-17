#include <libultraship.h>
#include "SpaghettiGui.h"
#include <libultraship/src/window/gui/Gui.h>
#include <libultraship/src/window/Window.h>
#ifdef __SWITCH__
#include "ConfigVersion.h"
#else
#include "Config.h"
#endif

#ifdef __APPLE__
#include <SDL_hints.h>
#include <SDL_video.h>

#include <imgui_impl_metal.h>
#include <imgui_impl_sdl2.h>
#else
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_video.h>
#endif

#if defined(__ANDROID__) || defined(__IOS__)
#include "port/mobile/MobileImpl.h"
#endif

#ifdef ENABLE_OPENGL
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#endif

#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// NOLINTNEXTLINE
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif

namespace Ship {
#define TOGGLE_BTN ImGuiKey_F1
#define TOGGLE_PAD_BTN ImGuiKey_GamepadBack

    void SpaghettiGui::DrawMenu() {
        const std::shared_ptr<Window> wnd = Context::GetInstance()->GetWindow();
        const std::shared_ptr<Config> conf = Context::GetInstance()->GetConfig();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                    ImGuiWindowFlags_NoResize;

        if (GetMenuBar() && GetMenuBar()->IsVisible()) {
            windowFlags |= ImGuiWindowFlags_MenuBar;
        }

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2((int)wnd->GetWidth(), (int)wnd->GetHeight()));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::Begin("Main - Deck", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        mTemporaryWindowPos = ImGui::GetWindowPos();

        const ImGuiID dockId = ImGui::GetID("main_dock");
        if (!ImGui::DockBuilderGetNode(dockId)) {
            ImGui::DockBuilderRemoveNode(dockId);
            ImGui::DockBuilderAddNode(dockId, ImGuiDockNodeFlags_NoTabBar);
            ImGui::DockBuilderSetNodeSize(dockId, ImVec2(viewport->Size.x, viewport->Size.y));

            ImGui::DockBuilderDockWindow("Main Game", dockId);

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            const ImGuiID dockId = ImGui::GetID("main_dock");

            ImGuiID topId = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Up, 0.15f, nullptr, nullptr);
            ImGui::DockBuilderSetNodeSize(topId, ImVec2(viewport->Size.x, 40));

            ImGuiID bottomId = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Down, 0.25f, nullptr, nullptr);
            ImGui::DockBuilderSetNodeSize(bottomId, ImVec2(viewport->Size.x, viewport->Size.y * 0.1f));

            ImGuiID bottomLeftId = ImGui::DockBuilderSplitNode(bottomId, ImGuiDir_Left, 0.25f, nullptr, nullptr);
            ImGui::DockBuilderSetNodeSize(bottomId, ImVec2(viewport->Size.x, viewport->Size.y * 0.1f));

            ImGuiID rightId = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Right, 0.25f, nullptr, nullptr);
            ImGui::DockBuilderSetNodeSize(rightId, ImVec2(viewport->Size.x * 0.15f, viewport->Size.y));

            // Order of operations matters here for the properties window to be in the right spot
            ImGui::DockBuilderDockWindow("Scene Explorer", rightId);
            ImGui::DockBuilderDockWindow("Track Properties", rightId); // Attach as second tab

            ImGuiID rightBottomId = ImGui::DockBuilderSplitNode(rightId, ImGuiDir_Down, 0.25f, nullptr, nullptr);
            ImGui::DockBuilderSetNodeSize(rightBottomId, ImVec2(viewport->Size.x, viewport->Size.y * 0.25));

            ImGui::DockBuilderDockWindow("Properties", rightBottomId);
            ImGui::DockBuilderDockWindow("Tools", topId);
            ImGui::DockBuilderDockWindow("Content Browser", bottomLeftId);

            ImGui::DockBuilderFinish(dockId);
        }

        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoDockingInCentralNode);

        if (ImGui::IsKeyPressed(TOGGLE_BTN) || ImGui::IsKeyPressed(ImGuiKey_Escape) ||
            (ImGui::IsKeyPressed(TOGGLE_PAD_BTN) && CVarGetInteger(CVAR_IMGUI_CONTROLLER_NAV, 0))) {
            if ((ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(TOGGLE_PAD_BTN)) && GetMenu()) {
                GetMenu()->ToggleVisibility();
            } else if ((ImGui::IsKeyPressed(TOGGLE_BTN) || ImGui::IsKeyPressed(TOGGLE_PAD_BTN)) && GetMenuBar()) {
                Gui::GetMenuBar()->ToggleVisibility();
            }
            if (wnd->IsFullscreen()) {
                Context::GetInstance()->GetWindow()->SetMouseCapture(
                    !(GetMenuOrMenubarVisible() || wnd->ShouldForceCursorVisibility()));
            }
            if (CVarGetInteger(CVAR_IMGUI_CONTROLLER_NAV, 0) && GetMenuOrMenubarVisible()) {
                mImGuiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            } else {
                mImGuiIo->ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
            }
        }

    #if __APPLE__
        if ((ImGui::IsKeyDown(ImGuiKey_LeftSuper) || ImGui::IsKeyDown(ImGuiKey_RightSuper)) &&
            ImGui::IsKeyPressed(ImGuiKey_R, false)) {
            std::reinterpret_pointer_cast<ConsoleWindow>(
                Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Console"))
                ->Dispatch("reset");
        }
    #else
        if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
            ImGui::IsKeyPressed(ImGuiKey_R, false)) {
            std::reinterpret_pointer_cast<ConsoleWindow>(
                Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Console"))
                ->Dispatch("reset");
        }
    #endif

        if (GetMenuBar()) {
            GetMenuBar()->Update();
            GetMenuBar()->Draw();
        }

        if (GetMenu()) {
            GetMenu()->Update();
            GetMenu()->Draw();
        }

        for (auto& windowIter : mGuiWindows) {
            windowIter.second->Update();
            windowIter.second->Draw();
        }

        ImGui::End();
    }

}