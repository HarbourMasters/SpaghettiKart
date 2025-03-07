#pragma once

#include <libultraship/libultraship.h>

namespace EditorNamespace {
class ContentBrowserWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~ContentBrowserWindow();
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
};
}