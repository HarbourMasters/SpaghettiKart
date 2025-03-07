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

void ObjectPicker::Load() {
    eGizmo.Load();
}

void ObjectPicker::Tick() {
    eGizmo.Tick();
}

void ObjectPicker::SelectObject(std::vector<GameObject>& objects) {
    Ray ray;
    ray.Origin = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);

    // This allows selection of objects in the scene explorer.
    // Otherwise this would still run when selecting buttons in editor windows.
    if (IsInGameScreen()) {
        ray.Direction = ScreenRayTrace();

        ObjectPicker::FindObject(ray, objects);

        if (_selected != nullptr) {
            eGizmo.Enable(_selected, ray);
            eGizmo.Enabled = true;
        } else {
            //eGizmo.Disable();
            eGizmo.Enabled = false;
            eGizmo._selected = nullptr;
        }
    }
}

void ObjectPicker::DragHandle() {
    Ray ray;
    ray.Origin = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);
    ray.Direction = ScreenRayTrace();

    // Skip if a drag is already in progress
    if (eGizmo.SelectedHandle != Gizmo::GizmoHandle::None) {
        eGizmo._ray = ray.Direction;
        return;
    }

    // Is the gizmo being dragged?
    if (eGizmo.Enabled) {
        float t;
        if (IntersectRaySphere(ray, eGizmo.Pos, eGizmo.AllAxisRadius, t)) {
            eGizmo.SelectedHandle = Gizmo::GizmoHandle::All_Axis;
            eGizmo._ray = ray.Direction;
            FVector clickPosition = ray.Origin + ray.Direction * t;
            eGizmo._cursorOffset = eGizmo.Pos - clickPosition;
            eGizmo.PickDistance = t;
            return;
        }

        for (auto tri = eGizmo.RedCollision.Triangles.begin(); tri < eGizmo.RedCollision.Triangles.end(); tri++) {
            float t;
            FVector pos = FVector(eGizmo.Pos.x, eGizmo.Pos.y, eGizmo.Pos.z - eGizmo._gizmoOffset);
            if (IntersectRayTriangle(ray, *tri, pos, t)) {
                eGizmo.SelectedHandle = Gizmo::GizmoHandle::Z_Axis;
                eGizmo._ray = ray.Direction;
                FVector clickPosition = ray.Origin + ray.Direction * t;
                eGizmo._cursorOffset = eGizmo.Pos - clickPosition;
                return; // Stop checking objects if we selected a Gizmo handle
            }
        }

        for (auto tri = eGizmo.GreenCollision.Triangles.begin(); tri < eGizmo.GreenCollision.Triangles.end(); tri++) {
            float t;
            FVector pos = FVector(eGizmo.Pos.x - eGizmo._gizmoOffset, eGizmo.Pos.y, eGizmo.Pos.z);
            if (IntersectRayTriangle(ray, *tri, pos, t)) {
                eGizmo.SelectedHandle = Gizmo::GizmoHandle::X_Axis;
                eGizmo._ray = ray.Direction;
                FVector clickPosition = ray.Origin + ray.Direction * t;
                eGizmo._cursorOffset = eGizmo.Pos - clickPosition;
                return; // Stop checking objects if we selected a Gizmo handle
            }
        }

        for (auto tri = eGizmo.BlueCollision.Triangles.begin(); tri < eGizmo.BlueCollision.Triangles.end(); tri++) {
            float t;
            FVector pos = FVector(eGizmo.Pos.x, eGizmo.Pos.y + eGizmo._gizmoOffset, eGizmo.Pos.z);
            if (IntersectRayTriangle(ray, *tri, pos, t)) {
                eGizmo.SelectedHandle = Gizmo::GizmoHandle::Y_Axis;
                eGizmo._ray = ray.Direction;
                FVector clickPosition = ray.Origin + ray.Direction * t;
                eGizmo._cursorOffset = eGizmo.Pos - clickPosition;
                return; // Stop checking objects if we selected a Gizmo handle
            }
        }
    }
}

void ObjectPicker::Draw() {
    if (_selected != NULL) {
        eGizmo.Draw();
    }
}

void ObjectPicker::FindObject(Ray ray, std::vector<GameObject>& objects) {
    bool found = false;
    for (auto& object : objects) {
        float boundingBox = object.BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }

        switch(object.Collision) {
            case CollisionType::VTX_INTERSECT:
                for (const auto& tri : object.Triangles) {
                    float t;
                    if (IntersectRayTriangle(ray, tri, *object.Pos, t)) {
                        printf("\nSELECTED OBJECT\n\n");
                        _selected = &object;
                        found = true;
                    }
                }
                break;
            case CollisionType::BOUNDING_BOX: {
                float max = 2.0f;
                float min = -2.0f;
                Vec3f boxMin = { object.Pos->x + boundingBox * min, 
                                 object.Pos->y + boundingBox * min,
                                 object.Pos->z + boundingBox * min };

                Vec3f boxMax = { object.Pos->x + boundingBox * max, 
                                 object.Pos->y + boundingBox * max, 
                                 object.Pos->z + boundingBox * max };
                float t;
                if (QueryCollisionRayActor(&ray.Origin.x, &ray.Direction.x, boxMin, boxMax, &t)) {
                    // if (actor == _selected) {
                    //     _selected = nullptr;
                    //     break;
                    // }
                    printf("FOUND BOUNDING BOX OBJECT\n");
                    found = true;
                    //foundActor = &actor;
                    //type = object.Type;
                    _selected = &object;
                    break;
                }
                break;
            }
            case CollisionType::BOUNDING_SPHERE:
                printf("Editor::ObjectPicker.cpp Bounding sphere collision type not yet supported\n");
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
