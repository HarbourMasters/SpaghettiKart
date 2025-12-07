#ifndef MENU_H
#define MENU_H

#include <libultraship/libultraship.h>
#include "UIWidgets.h"
#include "MenuTypes.h"
#include "src/port/Game.h"
#include "src/port/audio/HMAS.h"

extern "C" {
#include "defines.h"
#include "main.h"
#include "menus.h"
#include "code_800029B0.h"
#include "external.h"
}

namespace Ship {
uint32_t GetVectorIndexOf(std::vector<std::string>& vector, std::string value);
class Menu : public GuiWindow {
  public:
    using GuiWindow::GuiWindow;

    Menu(const std::string& cVar, const std::string& name, uint8_t searchSidebarIndex_ = 0,
         UIWidgets::Colors menuThemeIndex_ = UIWidgets::Colors::LightBlue);
    virtual ~Menu() {}

    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
    void Draw() override;
    void InsertSidebarSearch();
    void RemoveSidebarSearch();
    void UpdateWindowBackendObjects();

    void MenuDrawItem(WidgetInfo& widget, uint32_t width, UIWidgets::Colors menuThemeIndex);
    void AddMenuEntry(std::string entryName, const char* entryCvar);
    std::unordered_map<uint32_t, disabledInfo>& GetDisabledMap();

  protected:
    ImVec2 mOriginalSize;
    std::string mName;
    uint32_t mWindowFlags;
    std::unordered_map<std::string, MainMenuEntry> menuEntries;
    std::vector<std::string> menuOrder;
    uint32_t DrawSearchResults(std::string& menuSearchText);
    ImGuiTextFilter menuSearch;
    uint8_t searchSidebarIndex;
    UIWidgets::Colors defaultThemeIndex;
    std::shared_ptr<std::vector<Ship::WindowBackend>> availableWindowBackends;
    std::unordered_map<Ship::WindowBackend, const char*> availableWindowBackendsMap;
    Ship::WindowBackend configWindowBackend;

    std::unordered_map<uint32_t, disabledInfo> disabledMap;
    std::vector<disabledInfo> disabledVector;
    const SidebarEntry searchSidebarEntry = {
        .columnCount = 1,
        .columnWidgets = { { { .name = "Sidebar Search",
                               .type = WIDGET_SEARCH,
                               .options = std::make_shared<UIWidgets::WidgetOptions>(UIWidgets::WidgetOptions{}.Tooltip(
                                   "Searches all menus for the given text, including tooltips.")) } } }
    };
    virtual void ProcessReset() {
      gGamestateNext = MAIN_MENU_FROM_QUIT;
      gIsGamePaused = 0;
      // Reset credits
      D_800DC5E4 = 0;
      gTourComplete = false;
      SetMarioRaceway();
      memset(&gGameModeMenuColumn, 0, sizeof(s8) * NUM_ROWS_GAME_MODE_MENU);
      memset(&gGameModeSubMenuColumn, 0, sizeof(s8) * NUM_COLUMN_GAME_MODE_SUB_MENU * NUM_ROWS_GAME_MODE_SUB_MENU);

      CM_ResetAudio();

      switch(CVarGetInteger("gSkipIntro", 0)) {
          case 0:
              gMenuSelection = HARBOUR_MASTERS_MENU;
              break;
          case 1:
              gMenuSelection = LOGO_INTRO_MENU;
              break;
          case 2:
              gMenuSelection = START_MENU;
              break;
          case 3:
              gMenuSelection = MAIN_MENU;
              break;
      }

      // Debug mode override gSkipIntro
      if (CVarGetInteger("gEnableDebugMode", 0) == true) {
          gMenuSelection = START_MENU;
      } else {
          gMenuSelection = LOGO_INTRO_MENU;
      }
    }

  private:
    bool allowPopout = true; // PortNote: should be set to false on small screen ports
    bool popped;
    ImVec2 poppedSize;
    ImVec2 poppedPos;
    float uiScale = 1.0f;
    float windowHeight;
    float windowWidth;
};
} // namespace Ship

#endif // MENU_H
