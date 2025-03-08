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
}

void Editor::Load() {
    printf("Editor: Loading Editor...\n");
    eObjectPicker.Load();;
    for (auto& object : eGameObjects) {
        GenerateCollisionMesh(object, object.Model, 1.0f);
    }
    printf("Editor: Loading Complete!\n");
}

void Editor::Tick() {
    auto wnd = GameEngine::Instance->context->GetWindow();

    static bool wasMouseDown = false;
    static bool isDragging = false;
    static Ship::Coords mouseStartPos;

    Ship::Coords mousePos = wnd->GetMousePos();
    bool isMouseDown = wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT);

    eGameObjects.erase(
        std::remove_if(eGameObjects.begin(), eGameObjects.end(),
                    [](const auto& object) { return (*object.DespawnFlag) == object.DespawnValue; printf("DELETED OBJ\n"); }),
        eGameObjects.end());

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
}

void Editor::AddObject(const char* name, FVector* pos, Gfx* model, float scale, CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue) {
    if (model != NULL) {
        eGameObjects.push_back({name, pos, model, {}, collision, boundingBoxSize, despawnFlag, despawnValue});
        GenerateCollisionMesh(eGameObjects.back(), model, scale);
    } else { // to bounding box or sphere collision
        eGameObjects.push_back({name, pos, model, {}, CollisionType::BOUNDING_BOX, 22.0f, despawnFlag, despawnValue});
    }
}

void Editor::ClearObjects() {
    eGameObjects.clear();
}

void Editor::SelectObjectFromSceneExplorer(GameObject* object) {
    eObjectPicker._selected = object;
    eObjectPicker.eGizmo.Enabled = true;
    eObjectPicker.eGizmo.SetGizmoNoCursor(object);
}
