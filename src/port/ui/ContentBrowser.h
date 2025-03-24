#pragma once

#include <libultraship/libultraship.h>

namespace Editor {
class ContentBrowserWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;
    ~ContentBrowserWindow();

    std::vector<std::string> Content;
    std::vector<std::string> Tracks; // Contains modded archives in mods/
    std::unordered_map<std::string, std::vector<std::string>> TrackAssetMap;
    std::unordered_map<std::string, std::string> TrackPath;

    bool Refresh = true;
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
    void AddTrackContent();
    void AddActorContent();
    void AddObjectContent();
    void AddCustomContent();
    void FindTracks();
    void FindContent();
    void FolderButton(const char* label, bool& contentFlag, const ImVec2& size = ImVec2(80, 32));
};
}
