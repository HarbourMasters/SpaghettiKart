#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Editor.h"
#include "Collision.h"

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

void Editor::Load() {
    eObjectPicker.Load();
    for (auto& object : eGameObjects) {
        //GenerateCollisionMesh(object, object.Model, 1.0f);
    }
}

void Editor::Tick() {
    auto wnd = GameEngine::Instance->context->GetWindow();

    static bool wasMouseDown = false;
    static bool isDragging = false;
    static Ship::Coords mouseStartPos;

    Ship::Coords mousePos = wnd->GetMousePos();
    bool isMouseDown = wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT);

    if (isMouseDown && !wasMouseDown) {  
        // Mouse just pressed (Pressed state)
        mouseStartPos = mousePos;
        isDragging = false;
    }

    if (isMouseDown) {  
        // Mouse is being held (Held state)
        int dx = mousePos.x - mouseStartPos.x;
        int dy = mousePos.y - mouseStartPos.y;
        int dragThreshold = 5;  // Adjust as needed

        if ((dx * dx + dy * dy) > (dragThreshold * dragThreshold)) {  
            // Squared distance check to avoid unnecessary sqrt()
            isDragging = true;
            eObjectPicker.DragHandle();  // Call drag logic
        }
    }

    if (!isMouseDown && wasMouseDown) {  
        // Mouse just released (Released state)
        eObjectPicker.eGizmo.SelectedHandle = Gizmo::GizmoHandle::None;
        if (!isDragging) {
            eObjectPicker.SelectObject(eGameObjects);
        }

    }

    wasMouseDown = isMouseDown;  // Update previous state

    eObjectPicker.Tick();
}
 
void Editor::Draw() {
    eObjectPicker.Draw();
    DrawObj();
}

void Editor::AddObject(FVector* pos, Gfx* model, float scale, CollisionType collision, float boundingBoxSize) {
    
    if (model != NULL) {
        eGameObjects.push_back({pos, model, {}, collision, boundingBoxSize});
        GenerateCollisionMesh(eGameObjects.back(), model, scale);
        printf("\n");
    } else { // to bounding box or sphere collision
        eGameObjects.push_back({pos, model, {}, CollisionType::BOUNDING_BOX, 2.0f});
    }
}
