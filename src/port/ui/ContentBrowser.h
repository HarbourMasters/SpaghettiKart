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
    bool CustomContent = false;
    bool TrackContent = false;
protected:
    void InitElement() override {};
    void DrawElement() override;
    void UpdateElement() override {};
    void AddTrackContent(std::string search);
    void AddActorContent(std::string search);
    void AddCustomContent(std::string search);
    void FindContent();
    void FolderButton(const char* label, bool& contentFlag, const ImVec2& size = ImVec2(80, 32));
    ATrain* TrainWindow();

private:
    char mSearchBuffer[128] = "";  // Search bar for all tabs
    static std::string ToLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c){ return std::tolower(c); });
        return result;
    }
};
}
