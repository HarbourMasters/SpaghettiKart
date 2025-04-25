#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Editor.h"
#include "Collision.h"
#include "Light.h"

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
#include "camera.h"
}

namespace Editor {
    int gfx_create_framebuffer(uint32_t width, uint32_t height, uint32_t native_width, uint32_t native_height,
        uint8_t resize);

    Editor::Editor() {
    }

    void Editor::Load() {
        printf("Editor: Loading Editor...\n");
        eObjectPicker.Load();
        for (auto& object : eGameObjects) {
            GenerateCollisionMesh(object, object->Model, 1.0f);
            object->Load();
        }
        printf("Editor: Loading Complete!\n");
    }

    void Editor::Tick() {

        if (CVarGetInteger("gEditorEnabled", 0) == true) {
            bEditorEnabled = true;
        } else {
            bEditorEnabled = false;
            return;
        }

        auto wnd = GameEngine::Instance->context->GetWindow();

        static bool wasMouseDown = false;
        static bool isDragging = false;
        static Ship::Coords mouseStartPos;

        Ship::Coords mousePos = wnd->GetMousePos();
        bool isMouseDown = wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT);

        eGameObjects.erase(
            std::remove_if(eGameObjects.begin(), eGameObjects.end(),
                        [](const auto& object) { return (*object->DespawnFlag) == object->DespawnValue; }),
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
            eObjectPicker.eGizmo.ManipulationStart = true;
            if (!isDragging) {
                eObjectPicker.SelectObject(eGameObjects);
            }

        }

        wasMouseDown = isMouseDown;  // Update previous state

        eObjectPicker.Tick();

        for (auto& object : eGameObjects) {
            object->Tick();
        }
    }

    void Editor::Draw() {
        if (!bEditorEnabled) {
            return;
        } 
        eObjectPicker.Draw();
        for (auto& object : eGameObjects) {
            object->Draw();
        }
    }

    GameObject* Editor::AddObject(const char* name, FVector* pos, IRotator* rot, FVector* scale, Gfx* model, float collScale, GameObject::CollisionType collision, float boundingBoxSize, int32_t* despawnFlag, int32_t despawnValue) {
        //printf("After AddObj: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
        // pos->x, pos->y, pos->z, name, model);
        if (model != nullptr) {
            eGameObjects.push_back(new GameObject(name, pos, rot, scale, model, {}, collision, boundingBoxSize, despawnFlag, despawnValue));
            GenerateCollisionMesh(eGameObjects.back(), model, collScale);
        } else { // to bounding box or sphere collision
            eGameObjects.push_back(new GameObject(name, pos, rot, scale, model, {}, GameObject::CollisionType::BOUNDING_BOX,
                                                10.0f, despawnFlag, despawnValue));
        }
        return eGameObjects.back();
    }

    void Editor::AddLight(const char* name, FVector* pos, s8* rot) {
        eGameObjects.push_back(new LightObject(name, pos, rot));
    }

    void Editor::ClearObjects() {
        for (auto& obj : eGameObjects) {
            delete obj;
        }
        eGameObjects.clear();
    }

    void Editor::DeleteObject() {
        GameObject* obj = eObjectPicker.eGizmo._selected;

        if (obj) {
            *obj->DespawnFlag = obj->DespawnValue;
            obj = nullptr;
            eObjectPicker._selected = nullptr;
        }
    }

    void Editor::ClearMatrixPool() {
        EditorMatrix.clear();
    }

    void Editor::SelectObjectFromSceneExplorer(GameObject* object) {
        eObjectPicker._selected = object;
        eObjectPicker.eGizmo.Enabled = true;
        eObjectPicker.eGizmo.SetGizmoNoCursor(object);
    }

    void Editor::SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY) {
        eObjectPicker.eGizmo.dimensions.MinX = minX + -1000;
        eObjectPicker.eGizmo.dimensions.MaxX = maxX + 1000;
        eObjectPicker.eGizmo.dimensions.MinY = minY + -100;
        eObjectPicker.eGizmo.dimensions.MaxY = maxY + 500;
        eObjectPicker.eGizmo.dimensions.MinZ = minZ + -1000;
        eObjectPicker.eGizmo.dimensions.MaxZ = maxZ + 1000;
    }
}
