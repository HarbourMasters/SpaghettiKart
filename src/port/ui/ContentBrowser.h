#pragma once

#include <libultraship/libultraship.h>
#include "engine/tracks/Track.h"
#include "AllActors.h"

namespace Editor {
class ContentBrowserWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~ContentBrowserWindow();

    struct Tracks {
        std::shared_ptr<Track> invalidTrack; // If not nullptr, user needs to create a scene file for this track.
        std::shared_ptr<Track> track; // A valid custom track. Used to reset the World->Tracks array on a file system refresh.
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
    void RemoveCustomTracksFromTrackList(); // Prevents duplicate tracks being added to World->Tracks array
    void AddActorContent();
    void AddObjectContent();
    void AddCustomContent();
    void FindTracks();
    void FindContent();
    void FolderButton(const char* label, bool& contentFlag, const ImVec2& size = ImVec2(80, 32));
    ATrain* TrainWindow();

};
}
