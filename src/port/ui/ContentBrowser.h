#pragma once

#include <libultraship/libultraship.h>
#include "engine/tracks/Track.h"
#include "AllActors.h"

namespace Editor {
class ContentBrowserWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~ContentBrowserWindow();

    std::vector<std::string> Content;

    bool Refresh = true;

    bool ActorContent = false;
    bool ObjectContent = false;
    bool CustomContent = false;
    bool TrackContent = false;
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
    void AddTrackContent();
    void AddActorContent();
    void AddObjectContent();
    void AddCustomContent();
    void FindContent();
    void FolderButton(const char* label, bool& contentFlag, const ImVec2& size = ImVec2(80, 32));
    ATrain* TrainWindow();

};
}
