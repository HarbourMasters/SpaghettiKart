#pragma once

#include <utility>
#include <iostream>
#include "LayoutSettings.h"

#include "hmui/Navigator.h"

// singleplayer, split-screen, online
class OnlineBattleViewElements : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {
        std::string modes[] = {"8P", "4P"};

        for(int i = 0; i < 2; ++i) {
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
                    switch(i) {
                        default:
                            Navigator::push("/player_select");
                            break;
                    }

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
                    .width = MENU_BUTTON_WIDTH,
                    .height = MENU_BUTTON_HEIGHT,
                    .alignment = Alignment::Center(),
                    .color = MENU_BUTTON_COLOUR,
                    .child = Text(
                        .text = modes[i],
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
                .child = BuildMenuContent(entries),
                .left = 0,
                .top = 0,
                .right = 0,
                .bottom = 0
            ),
        };
    return BuildMenuStack(stuff);
    }

    ~OnlineBattleViewElements() override = default;
};

class OnlineBattleView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        // Render OnlineBattleView
        return std::make_shared<OnlineBattleViewElements>();
    }

    ~OnlineBattleView() override = default;
};
