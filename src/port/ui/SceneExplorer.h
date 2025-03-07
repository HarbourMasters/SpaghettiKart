#pragma once

#include <libultraship/libultraship.h>

namespace Editor {
class SceneExplorerWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~SceneExplorerWindow();
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
};
}