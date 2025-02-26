#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Editor.h"
#include "port/Engine.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>

#include "engine/actors/Ship.h"
#include "port/Game.h"

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "defines.h"
#include "actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "camera.h"
}

int gfx_create_framebuffer(uint32_t width, uint32_t height, uint32_t native_width, uint32_t native_height,
    uint8_t resize);

Editor::Editor() {
    s32 test = 1;
   // gsSPSetFB(gDisplayListHead++, &test);
}

void Editor::Tick() {
    auto wnd = GameEngine::Instance->context->GetWindow();


    // GetMouseState
    if (wnd->MouseClick(Ship::LUS_MOUSE_BTN_LEFT)) {
        eObjectPicker.SelectObject();
    }
}

void Editor::Draw() {
    eObjectPicker.Draw();
}

void Editor::DrawObj(float length) {
    Mat4 mtx;
    Vec3f pos2 = { _ray[0], _ray[1], _ray[2] };
    mtxf_translate(mtx, pos2);
    mtxf_scale(mtx, 0.03);
    render_set_position(mtx, 0);
    gSPDisplayList(gDisplayListHead++, Editor::box_Cube_mesh);
}
