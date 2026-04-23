#pragma once

#include "port/Engine.h"
#include "port/ui/hmui/provider/LUSImageProvider.h"

#include "hmui/Navigator.h"
#include "hmui/graphics/GraphicsContext.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/Row.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
#include "hmui/widgets/Scrollable.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/widgets/Image.h"
#include "hmui/widgets/Text.h"
#include "hmui/widgets/Wrap.h"
#include "hmui/widgets/Stack.h"
#include "hmui/widgets/Image.h"

#define DEBUG_COMPONENTS

/** Variables **/
bool view_singleplayer = false;

/** Buttons **/

#define MENU_BUTTON_WIDTH 325.0f
#define MENU_BUTTON_HEIGHT 75.0f
#define MENU_BUTTON_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.0f)

#define MENU_BUTTON_TEXT_COLOUR Color2D(1.0f, 1.0f, 1.0f, 1.0f)
#define MENU_BUTTON_TEXT_SCALE 1.8f

/** General Container Layout **/

#define BACKGROUND_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.8f)
#define MARGIN_LEFT_SPACING EdgeInsets::only(580.0f, 360.0f, 0.0f, 0.0f)
#define CONTENT_HEIGHT entries.size() * MENU_BUTTON_HEIGHT

/** Events **/

#define BUTTON_ON_TAP_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.60f)
#define BUTTON_ON_HOVER_COLOUR Color2D(0.0f, 0.0f, 0.0f, 0.40f)

/** Multiplayer Player Count Select */
#define MULTIPLAYER_SELECT_BUTTON_WIDTH 300.0f
#define MULTIPLAYER_SELECT_BUTTON_HEIGHT 300.0f
#define MULTIPLAYER_SELECT_BUTTON_SPACING 60.0f
#define MULTIPLAYER_SELECT_BUTTON_COLOUR Color2D(0.3f, 0.3f, 0.6f, 0.4f)

/** Player Select **/
#define PLAYER_SELECT_ROWS 4
#define PLAYER_SELECT_BUTTON_WIDTH 367.0f
#define PLAYER_SELECT_BUTTON_HEIGHT 368.0f
#define PLAYER_SELECT_BUTTON_SPACING 29.0f
#define PLAYER_SELECT_BUTTON_RUN_SPACING 34.0f
#define PLAYER_SELECT_LABEL_HEIGHT 69.0f

/** Track Select **/
#define TRACK_SELECT_ROWS 3
#define TRACK_SELECT_BUTTON_WIDTH 325.0f
#define TRACK_SELECT_BUTTON_HEIGHT 350.0f
#define TRACK_SELECT_BUTTON_SPACING 29.0f
#define TRACK_SELECT_BUTTON_RUN_SPACING 34.0f
#define TRACK_SELECT_LABEL_HEIGHT 69.0f

/**
 * This should be the base of your View as this
 * allows the placing of multiple elements around the screen
 */
inline std::shared_ptr<InternalDrawable> BuildMenuStack(
    const std::vector<std::shared_ptr<InternalDrawable>> entries
) {
    return Container(
        
        .child = Stack(
            .children = entries,
            .fit = StackFit::Expand,
        )
    );
}

inline std::shared_ptr<InternalDrawable> BuildMenuBackground() {
    return Container(
        .width = (float)OTRGetGameViewportWidth(),
        .height = (float)OTRGetGameViewportHeight(),
        .clipToBounds = true,
        .color = BACKGROUND_COLOUR
    );
}

inline std::shared_ptr<InternalDrawable> BuildMenuContent(
    const std::vector<std::shared_ptr<InternalDrawable>> entries
) {
    return Container(
        .margin = MARGIN_LEFT_SPACING,
        .clipToBounds = false,
        .child = Column( .children = entries )
    );
}

inline std::shared_ptr<InternalDrawable> BuildMenuInfoBar() {
    return Container(
        .width = (float)OTRGetGameViewportWidth(),
        .height = 100.0f,

        .color = Color2D(0.0f, 0.0f, 0.0f, 0.4f),
        .child = Container(
            .margin = EdgeInsets::only(25, 0, 0, 0),
            .alignment = Alignment::CenterLeft(),
            .child = Text(
                .text = "Back: B, Backspace, or Right-click",
                .scale = MENU_BUTTON_TEXT_SCALE,
                .alignV = VerticalAlign::Center,
                .color = MENU_BUTTON_TEXT_COLOUR
            )
        ),
    );
}

#include "engine/TrackBrowser.h"
extern "C" {
#include "main.h"
#include "menus.h"
#include "code_800029B0.h"
extern s8 gPlayerCount;
}

/** Menu Game Actions **/
void View_SetPlayerCount(size_t count) {
    gPlayerCountSelection1 = count;
    gPlayerCount = count;
}

void View_SetScreenMode(s32 screenMode) {
    gScreenModeSelection = screenMode;
}

void View_SetCC(s32 cc) {
    gCCSelection = cc;
}

void View_SetGamemode(s32 gamemode) {
    gModeSelection = gamemode;
}

void View_SelectPlayer(s32 playerId, s32 characterId) {
    if ((playerId >= 0) && (playerId < 4)) {
        gCharacterSelections[playerId] = characterId;
    }
}

void View_SetTrack(const std::string& resourceName) {
    TrackBrowser::Instance->SetTrack(resourceName.c_str());
    gGamestateNext = RACING;
    Navigator::pushReplacement("/");
    HMUI::Instance->setActive(false);
}
