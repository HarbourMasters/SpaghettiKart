#include <libultraship/libultraship.h>
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
#include "camera.h"
}

namespace Editor {

void ObjectPicker::Load() {
    eGizmo.Load();
}

void ObjectPicker::Tick() {
}

void ObjectPicker::SelectObject(std::vector<GameObject*> objects) {
    Ray ray;
    ray.Origin = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);

    // This allows selection of objects in the scene explorer.
    // Otherwise this would still run when selecting buttons in editor windows.
    if (IsInGameScreen()) {
        ray.Direction = ScreenRayTrace();

        ObjectPicker::FindObject(ray, objects);

        if (_selected != nullptr) {
            eGizmo.SetGizmo(_selected, ray);
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
        eGizmo.Tick();
        return;
    }

    // Is the gizmo being dragged?
    if (!eGizmo.Enabled) { return; }
    float closestDistance = FLT_MAX;
    std::optional<FVector> closestClickPos;
    Gizmo::GizmoHandle closestHandle = Gizmo::GizmoHandle::None;

    // All axis grab. Not used in rotate mode rotate
    if (static_cast<Gizmo::TranslationMode>(CVarGetInteger("eGizmoMode", 0)) != Gizmo::TranslationMode::Rotate) {
        float t;
        if (IntersectRaySphere(ray, eGizmo.Pos, eGizmo.AllAxisRadius, t)) {
            if (t < closestDistance) {
                closestDistance = t;
                closestClickPos = ray.Origin + ray.Direction * t;
                closestHandle = Gizmo::GizmoHandle::All_Axis;
            }
        }
    }

    // Arrow handles
    auto tryHandle = [&](Gizmo::GizmoHandle handle, MtxF mtx, const std::vector<Triangle>& tris) {
        for (const auto& tri : tris) {
            if (auto clickPos = QueryHandleIntersection(mtx, ray, tri)) {
                float dist = (*clickPos - ray.Origin).Magnitude();
                if (dist < closestDistance) {
                    closestDistance = dist;
                    closestClickPos = *clickPos;
                    closestHandle = handle;
                }
            }
        }
    };

    switch(static_cast<Gizmo::TranslationMode>(CVarGetInteger("eGizmoMode", 0))) {
        case Gizmo::TranslationMode::Move:
            tryHandle(Gizmo::GizmoHandle::Z_Axis, eGizmo.Mtx_RedX, eGizmo.RedCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::X_Axis, eGizmo.Mtx_GreenY, eGizmo.GreenCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::Y_Axis, eGizmo.Mtx_BlueZ, eGizmo.BlueCollision.Triangles);
            break;
        case Gizmo::TranslationMode::Rotate:
            tryHandle(Gizmo::GizmoHandle::X_Axis, eGizmo.Mtx_RedX, eGizmo.RedRotateCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::Z_Axis, eGizmo.Mtx_GreenY, eGizmo.GreenRotateCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::Y_Axis, eGizmo.Mtx_BlueZ, eGizmo.BlueRotateCollision.Triangles);
            break;
        case Gizmo::TranslationMode::Scale:
            tryHandle(Gizmo::GizmoHandle::Z_Axis, eGizmo.Mtx_RedX, eGizmo.RedScaleCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::X_Axis, eGizmo.Mtx_GreenY, eGizmo.GreenScaleCollision.Triangles);
            tryHandle(Gizmo::GizmoHandle::Y_Axis, eGizmo.Mtx_BlueZ, eGizmo.BlueScaleCollision.Triangles);
            break;

    }

    if (closestHandle != Gizmo::GizmoHandle::None && closestClickPos.has_value()) {
        eGizmo.SelectedHandle = closestHandle;
        eGizmo._ray = ray.Direction;
        eGizmo._cursorOffset = eGizmo.Pos - *closestClickPos;
        eGizmo.PickDistance = closestDistance;
    }
}

void ObjectPicker::Draw() {
    if (_selected != NULL) {
        eGizmo.Draw();
    }

    if (Debug) {
        Mat4 CursorMtx;
        IRotator rot = IRotator(0,0,0);
        FVector scale = FVector(0.1, 0.1, 0.1);
        FVector ray = ScreenRayTrace();

        float x = (cameras[0].pos[0] + ray.x * 800);
        float y = (cameras[0].pos[1] + ray.y * 800);
        float z = (cameras[0].pos[2] + ray.z * 800);

        ApplyMatrixTransformations((float(*)[4])&CursorMtx, FVector(x, y, z), rot, scale);
        Editor_AddMatrix((float(*)[4])&CursorMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__tracks/sphere");
    }
}

void ObjectPicker::FindObject(Ray ray, std::vector<GameObject*> objects) {
    bool found = false;
    GameObject* closestObject = nullptr;
    float closestDistance = FLT_MAX;

    for (auto& object : objects) {
        float boundingBox = object->BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }

        switch(object->Collision) {
            case GameObject::CollisionType::VTX_INTERSECT:
                for (const auto& tri : object->Triangles) {
                    float t;
                    if (IntersectRayTriangleAndTransform(ray, *object->Pos, tri, t)) {
                        if (t < closestDistance) {
                            closestDistance = t;
                            closestObject = object;
                            printf("SELECTED OBJECT\n");
                        }
                    }
                }
                break;
            case GameObject::CollisionType::BOUNDING_BOX: {
                float max = 2.0f;
                float min = -2.0f;
                Vec3f boxMin = { object->Pos->x + boundingBox * min, 
                                 object->Pos->y + boundingBox * min,
                                 object->Pos->z + boundingBox * min };

                Vec3f boxMax = { object->Pos->x + boundingBox * max, 
                                 object->Pos->y + boundingBox * max, 
                                 object->Pos->z + boundingBox * max };
                float t;
                if (QueryCollisionRayActor(&ray.Origin.x, &ray.Direction.x, boxMin, boxMax, &t)) {
                    if (t < closestDistance) {
                        closestDistance = t;
                        closestObject = object;
                        printf("FOUND BOUNDING BOX OBJECT\n");
                    }
                    break;
                }
                break;
            }
            case GameObject::CollisionType::BOUNDING_SPHERE:
                printf("Editor::ObjectPicker.cpp Bounding sphere collision type not yet supported\n");
                break;
        }
    }
    if (closestObject != nullptr) {
        _selected = closestObject;
       // printf("FOUND COLLISION %d\n", type);
    } else {
       // printf("NO COLLISION\n");
        _selected = nullptr;
    }
}
}
