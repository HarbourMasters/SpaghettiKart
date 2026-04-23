#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"

// singleplayer, split-screen, online
class MultiplayerViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;
    bool useCont[4] = {false, false, false, false};
    void init() override {

        std::string modes[] = {"1P", "2P", "3P", "4P"};

        for(int i = 0; i < ARRAY_COUNT(modes); ++i) {
            entries.push_back(GestureDetector(
                .focusable = true,
                .focusDecorator = FocusDecorator {
                    .color = Color2D(1.0f, 1.0f, 1.0f, 0.8f),
                    .thickness = 2.0f
                },
                .onButton = [this, modes, i](std::shared_ptr<InternalDrawable> child, int controllerId, uint16_t btn) {

                    // Get text element
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    auto text = std::dynamic_pointer_cast<D_Text>(c->properties.child); //

                    if (controllerId != i) {
                        return;
                    }

                    if (btn & BTN_START) {
                        View_SetPlayerCount(i);
                        Navigator::push("/gamemode");
                        this->useCont[controllerId] = true;
                        return;
                    }

                    if (btn & A_BUTTON) {
                        useCont[controllerId] = true;
                        text->properties.text = modes[i] + " Connected!";
                    }

                    if (btn & B_BUTTON) {
                        text->properties.text = modes[i] + " Disconnected!";
                    }

                },
                .child = Container(
                    .width = MULTIPLAYER_SELECT_BUTTON_WIDTH,
                    .height = MULTIPLAYER_SELECT_BUTTON_HEIGHT,
                    .alignment = Alignment::Center(),
                    .color = MULTIPLAYER_SELECT_BUTTON_COLOUR,
                    .child = Text(
                        .text = std::string(modes[i] + " " +  (useCont[i] ? "Connected!" : "Disconnected!")),
                        .scale = MENU_BUTTON_TEXT_SCALE,
                        .color = MENU_BUTTON_TEXT_COLOUR
                    ),
                )
            ));
        }
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {

        std::vector<std::shared_ptr<InternalDrawable>> stuff = {
            Positioned(
                .child = BuildMenuBackground(),
                .left = 0,
                .top = 0,
                .right = 0,
                .bottom = 0,
            ),
            Positioned(
                .child = BuildMenuInfoBar(),
                .left = 0,
                .bottom = 0
            ),
            Positioned(
                .child = Container(
                    .alignment = Alignment::Center(),
                    .child = Row(
                        .spacing = MULTIPLAYER_SELECT_BUTTON_SPACING,
                        .mainAxisAlignment = MainAxisAlignment::SPACE_BETWEEN,
                        .children = entries,
                    )
                ),
                .left = 0,
                .top = 0,
                .right = 0,
                .bottom = 0
            ),
        };
        return BuildMenuStack(stuff);
    }

    virtual bool onBack(int controllerId) override {
        if (useCont[controllerId] == true) {
            useCont[controllerId] = false;
            return true;
        }

        for (size_t i = 0; i < 4; i++) {
            if (useCont[i] == true) {
                return true;
            }
        }

        Navigator::pop();
        return true;
    }

    ~MultiplayerViewElements() override = default;
};

class MultiplayerView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render MultiplayerViewElements
        return std::make_shared<MultiplayerViewElements>();
    }

    ~MultiplayerView() override = default;
};
