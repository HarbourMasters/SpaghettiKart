#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "ObjectPicker.h"
#include "port/Engine.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>

#include "engine/actors/Ship.h"
#include "port/Game.h"
#include "Gizmo.h"

#include "EditorMath.h"

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "defines.h"
#include "actors.h"
#include "math_util.h"
#include "math_util_2.h"
#include "camera.h"
}

ObjectPicker::ObjectPicker() {

}

void ObjectPicker::SelectObject() {
    FVector ray = ScreenRayTrace();

    ObjectPicker::FindObject(ray);

    if (_selected != nullptr) {
        eGizmo.Enable(&_selected->Pos, ray);
        eGizmo.Enabled = true;
    } else {
        //eGizmo.Disable();
        eGizmo.Enabled = false;
    }
}

void ObjectPicker::Draw() {
    if (_selected != nullptr) {
        eGizmo.Draw();
    }
}

void ObjectPicker::FindObject(FVector ray) {
    // Is the gizmo being clicked?
    if (eGizmo.Enabled) {
        Gizmo::GizmoHandle handle = Gizmo::GizmoHandle::None;
        for (size_t i = 0; i < 4; i++) {
            float t;
            auto [boxMin, boxMax] = eGizmo.GetBoundingBox((Gizmo::GizmoHandle) i);

            if (QueryCollisionRayActor(cameras[0].pos, &ray.x, &boxMin.x, &boxMax.x, &t)) {
                handle = (Gizmo::GizmoHandle) i;
                break;
            }
        }
        
        if (handle != Gizmo::GizmoHandle::None) {
            eGizmo.StartManipulation(handle);
            return; // Stop checking objects if we selected a Gizmo handle
        }
    }
    
    s32 type = 0;
    bool found = false;
    for (auto& actor : gWorldInstance.Actors) {
        float boundingBox = actor->BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }
        float t;
        float max = 2.0f;
        float min = -2.0f;
        Vec3f boxMin = { actor->Pos[0] + boundingBox * min, 
            actor->Pos[1] + boundingBox * min,
            actor->Pos[2] + boundingBox * min };

            Vec3f boxMax = { actor->Pos[0] + actor->BoundingBoxSize * max, 
                            actor->Pos[1] + actor->BoundingBoxSize * max, 
                            actor->Pos[2] + actor->BoundingBoxSize * max };

        if (QueryCollisionRayActor(cameras[0].pos, &ray.x, boxMin, boxMax, &t)) {
            // if (actor == _selected) {
            //     _selected = nullptr;
            //     break;
            // }
            found = true;
            //foundActor = &actor;
            type = actor->Type;
            _selected = actor;
            break;
        }
    }
    if (found) {
       // printf("FOUND COLLISION %d\n", type);
    } else {
       // printf("NO COLLISION\n");
        _selected = nullptr;
    }
}
