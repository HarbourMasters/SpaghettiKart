#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Editor.h"
#include "Collision.h"
#include "Light.h"

#include "port/Engine.h"
#include <ship/controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <ship/window/Window.h>

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
    Editor::Editor() {
    }

    Editor::~Editor() {
        ClearObjects();
        ClearMatrixPool();
    }

    void Editor::Load() {
        printf("Editor: Loading Editor...\n");
        eObjectPicker.Load();
        for (auto& object : eGameObjects) {
            GenerateCollisionMesh(object, (Gfx*)object->Model, 1.0f);
            object->Load();
        }

        printf("Editor: Loading Complete!\n");
    }

    void Editor::GenerateCollision() {
        // for (auto& actor : gWorldInstance.Actors) {
        //     GenerateCollisionMesh(actor, (Gfx*)actor->Model, 1.0f);
        // }
    }

    void Editor::Tick() {
        if (CVarGetInteger("gEditorEnabled", 0) == true) {
            bEditorEnabled = true;
        } else {
            bEditorEnabled = false;
            gIsEditorPaused = false; // Prevents game being paused with the editor closed.
            return;
        }

        // Set camera
        if (CVarGetInteger("gFreecam", 0) == true) {
            eCamera = &cameras[CAMERA_FREECAM];
        } else {
            eCamera = &cameras[0];
        }

        auto wnd = GameEngine::Instance->context->GetWindow();

        static bool wasMouseDown = false;
        static bool isDragging = false;
        static Ship::Coords mouseStartPos;

        Ship::Coords mousePos = wnd->GetMousePos();
        bool isMouseDown = wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT);

        //auto it = std::remove_if(eGameObjects.begin(), eGameObjects.end(),
        //    [](auto& object) {
        //        if (*object->DespawnFlag == object->DespawnValue) {
        //            delete object; // Free the pointed-to memory
        //            return true;   // Remove the pointer from the vector
        //        }
        //        return false;
        //    });

        //eGameObjects.erase(it, eGameObjects.end());

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

    GameObject* Editor::AddObject(FVector pos, IRotator rot, FVector scale, const char* model, float collScale, GameObject::CollisionType collision, float boundingBoxSize) {
        //printf("After AddObj: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
        // pos->x, pos->y, pos->z, name, model);

        if (nullptr != model && model[0] != '\0') {
            eGameObjects.push_back(new GameObject(pos, rot, scale, model, {}, collision, boundingBoxSize));
            GenerateCollisionMesh(eGameObjects.back(), (Gfx*)LOAD_ASSET_RAW(model), collScale);
        } else { // to bounding box or sphere collision
            eGameObjects.push_back(new GameObject(pos, rot, scale, model, {}, GameObject::CollisionType::BOUNDING_BOX,
                                                10.0f));
        }
        return eGameObjects.back();
    }

    void Editor::AddLight(const char* name, FVector* pos, s8* rot) {
        eGameObjects.push_back(new LightObject(name, pos, rot));
    }

    void Editor::ClearObjects() {
        ResetGizmo();

        for (auto& obj : eGameObjects) {
            delete obj;
        }
        eGameObjects.clear();
    }

    // Reset the gizmo
    void Editor::ResetGizmo() {
        eObjectPicker.eGizmo._selected = static_cast<GameObject*>(nullptr);
        eObjectPicker._selected = static_cast<GameObject*>(nullptr);
        eObjectPicker.eGizmo.Pos = FVector(0, 0, 0);
        eObjectPicker.eGizmo.Enabled = false;
    }

    void Editor::DeleteObject() {

        std::visit([this](auto* obj) {
            if (nullptr != obj) {
                gEditor.ResetGizmo(); // Unselect the object to prevent crashes
                obj->Destroy();
            }
        }, eObjectPicker.eGizmo._selected);
    }

    void Editor::ClearMatrixPool() {
        EditorMatrix.clear();
    }

    void Editor::SelectObjectFromSceneExplorer(std::variant<AActor*, OObject*, GameObject*> object) {
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
