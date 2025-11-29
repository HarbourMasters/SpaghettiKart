#pragma once

#include <libultraship/libultraship.h>

extern "C" {
#include "sounds.h"
}

namespace Editor {
class TrackPropertiesWindow : public Ship::GuiWindow {
public:
    using Ship::GuiWindow::GuiWindow;

    ~TrackPropertiesWindow();
protected:
    void InitElement() override {};
    void DrawElement() override;
    void DrawMusic();
    void DrawLight();
    void UpdateElement() override {};
    void RGB8ToFloat(const u8* src, float* dst);
    void FloatToRGB8(const float* src, u8* dst);
    const char* MusicSeqToString(MusicSeq seq);
    void DrawTourCamera();
      static int32_t SelectedShot;
      static int32_t SelectedKeyframe;
};
}
