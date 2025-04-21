#pragma once

#include <libultraship/libultraship.h>
#include "engine/courses/Course.h"

namespace Editor {
class ContentBrowserWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~ContentBrowserWindow();

    struct Tracks {
        Course* course;
        std::string SceneFile;
        std::string Name;
        std::string Dir; // Directory
        std::shared_ptr<Ship::Archive> Archive;
    };

    std::vector<Tracks> Tracks;

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
    void RemoveCustomTracksFromTrackList(); // Prevents duplicate courses being added to World->Courses array
    void AddActorContent();
    void AddObjectContent();
    void AddCustomContent();
    void FindTracks();
    void FindContent();
    void FolderButton(const char* label, bool& contentFlag, const ImVec2& size = ImVec2(80, 32));
};
}
