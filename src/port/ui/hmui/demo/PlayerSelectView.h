#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"
#include "hmui/widgets/InternalDrawable.h"

extern "C" {
#include "menu_items.h"
}

// singleplayer, split-screen, online
class PlayerSelectViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {

        for(int i = 0; i < 8; ++i) {
            entries.push_back(GestureDetector(
                .focusable = true,
                .focusDecorator = FocusDecorator {
                    .color = Color2D(1.0f, 1.0f, 1.0f, 0.8f),
                    .thickness = 2.0f
                },
                .onTap = [i](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_TAP_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                    Navigator::push("/track_select");
                    View_SelectPlayer(0, i);
                },
                .onTapRelease = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = MENU_BUTTON_COLOUR;
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onHover = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    // Handle hover event
                    std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = BUTTON_ON_HOVER_COLOUR;
                },
                .onHoverEnd = [](std::shared_ptr<InternalDrawable> child, float x, float y) {
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = MENU_BUTTON_COLOUR;
                },
                .child = Container(
                    .width = PLAYER_SELECT_BUTTON_WIDTH,
                    .height = PLAYER_SELECT_BUTTON_HEIGHT,
                    .color = MENU_BUTTON_COLOUR,
                    .child = Stack(
                        .children = {
                            Positioned(
                                .child = Container(
                                    .width = PLAYER_SELECT_BUTTON_WIDTH,
                                  //  .height = PLAYER_SELECT_BUTTON_HEIGHT - 80,
                                    .alignment = Alignment::Center(),
                                    .child = Text(.text = "Preview")
                                ),
                                .top = 0,
                                .bottom = PLAYER_SELECT_LABEL_HEIGHT,
                            ),
                            Positioned(
                                .child = Container(
                                    .width = PLAYER_SELECT_BUTTON_WIDTH,
                                    .height = PLAYER_SELECT_LABEL_HEIGHT,
                                    .alignment = Alignment::Center(),
                                    .child = Text(
                                        .text = D_800E76A8[i], // names
                                        .scale = MENU_BUTTON_TEXT_SCALE,
                                        .color = MENU_BUTTON_TEXT_COLOUR
                                    ),
                                ),
                                .bottom = 0,
                            ),
                        },
                        .fit = StackFit::Expand,
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
                    .width = (PLAYER_SELECT_BUTTON_WIDTH + PLAYER_SELECT_BUTTON_SPACING) * PLAYER_SELECT_ROWS,
                    .height = entries.size() * PLAYER_SELECT_BUTTON_HEIGHT,
                    .alignment = Alignment::Center(),
                    .child = Wrap(
                        .direction = Direction::Horizontal,
                        .spacing = PLAYER_SELECT_BUTTON_SPACING,
                        .runSpacing = PLAYER_SELECT_BUTTON_RUN_SPACING,
                        .alignment = MainAxisAlignment::CENTER,
                        .children = entries
                    ),
                ),
                .left = 0,
                .top = 0,
                .right = 0,
                .bottom = 0
            ),
        };
    return BuildMenuStack(stuff);
    }

    ~PlayerSelectViewElements() override = default;
};

class PlayerSelectView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render PlayerSelectView
        return std::make_shared<PlayerSelectViewElements>();
    }

    ~PlayerSelectView() override = default;
};
