#pragma once

#include "hmui/widgets/AppContext.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/widgets/InternalDrawable.h"
#include "MainView.h"
#include "MultiplayerView.h"
#include "GamemodeView.h"
#include "PlayerSelectView.h"
#include "TrackSelectView.h"
#include "OnlineMainView.h"
#include "OnlineRaceView.h"
#include "OnlineBattleView.h"
#include "CCView.h"

class RouterView : public Drawable {
public:
    s32 menuOptions;

    std::shared_ptr<InternalDrawable> build() override {
        return AppContext(
            .routes = {
                { "/", []() { return std::make_shared<MainView>(); }},
                { "/multiplayer", []() { return std::make_shared<MultiplayerView>(); }},
                { "/gamemode", []() { return std::make_shared<GamemodeView>(); }},
                { "/cc", []() { return std::make_shared<CCView>(); }},
                { "/player_select", []() { return std::make_shared<PlayerSelectView>(); }},
                { "/track_select", []() { return std::make_shared<TrackSelectView>(); }},
                { "/online_main", []() { return std::make_shared<OnlineMainView>(); }},
                { "/online_race", []() { return std::make_shared<OnlineRaceView>(); }},
                { "/online_battle", []() { return std::make_shared<OnlineBattleView>(); }},
            },
            .initialRoute = "/"
        );
    }

    ~RouterView() override = default;
};
