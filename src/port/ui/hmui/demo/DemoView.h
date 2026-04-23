#pragma once

#include <utility>
#include <iostream>

#include "hmui/widgets/AppContext.h"
#include "hmui/widgets/GestureDetector.h"
#include "hmui/widgets/InternalDrawable.h"
#include "hmui/widgets/Column.h"
#include "hmui/widgets/Container.h"
#include "hmui/widgets/Scrollable.h"
#include "hmui/widgets/Drawable.h"
#include "hmui/graphics/GraphicsContext.h"

std::shared_ptr<D_Container> nestedTest(std::vector<Color2D> entries, size_t index = 0) {
    if (index >= entries.size()) {
        return Container(); // base case: empty container
    }

    return Container(
        .padding = EdgeInsets::all(10.0f),
        .alignment = Alignment::Center,
        .color = entries[index],
        .child = GestureDetector(
            .onTap = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                // Handle hover event
                std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
            },
            .onHover = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                // Handle tap event
                std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                c->properties.color = Color2D(
                    rand() % 256 / 255.0f,
                    rand() % 256 / 255.0f,
                    rand() % 256 / 255.0f
                );
                std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
            },
            .child = nestedTest(entries, index + 1),
        )
    );
}

class TestView : public Drawable {
public:
    // std::vector<Color2D> entries;
    std::vector<std::shared_ptr<InternalDrawable>> entries;

    void init() override {
        for(int i = 0; i < 30; ++i) {
            entries.push_back(GestureDetector(
                .onTap = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    // Handle tap event
                    std::shared_ptr<D_Container> c = std::dynamic_pointer_cast<D_Container>(child);
                    c->properties.color = Color2D(
                        rand() % 256 / 255.0f,
                        rand() % 256 / 255.0f,
                        rand() % 256 / 255.0f
                    );
                    std::cout << "Tapped on child at (" << x << ", " << y << ")\n";
                },
                .onHover = [](std::shared_ptr<InternalDrawable>& child, float x, float y) {
                    // Handle hover event
                    std::cout << "Hovered over child at (" << x << ", " << y << ")\n";
                },
                .child = Container(
                    .width = 200.0f,
                    .height = 100.0f,
                    .color = Color2D(
                        rand() % 256 / 255.0f,
                        rand() % 256 / 255.0f,
                        rand() % 256 / 255.0f
                    ),
                )
            ));
        }
        Drawable::init();
    }

    std::shared_ptr<InternalDrawable> build() override {
        return Container(
            .child = Container(
                .width = 200.0f,
                .height = 300.0f,
                .padding = EdgeInsets::all(5.0f),
                .clipToBounds = true,
                .color = Color2D(0.0f, 0.0f, 0.0f, 0.3f),
                .child = Scrollable(
                    .direction = Direction::Vertical,
                    .child = Column(
                        .children = entries
                    )
                )
            )
        );
    }

    ~TestView() override = default;
};

class DemoSubView : public Drawable {
protected:
    std::shared_ptr<InternalDrawable> child;
public:

    explicit DemoSubView(std::shared_ptr<InternalDrawable> _child): child(std::move(_child)) {}

    std::shared_ptr<InternalDrawable> build() override {
        // Render TestView
        return this->child;
    }

    ~DemoSubView() override = default;
};



class DemoView : public Drawable {
public:
    std::shared_ptr<InternalDrawable> build() override {
        return AppContext(
            .routes = {
                { "/", []() { return std::make_shared<TestView>(); }}
            },
            .initialRoute = "/"
        );
    }
    ~DemoView() override = default;
};

