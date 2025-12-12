#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "engine/World.h"
#include "engine/Actor.h"
#include "engine/objects/Object.h"

#include "ObjectPicker.h"
#include "port/Engine.h"
#include <ship/controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <ship/window/Window.h>

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
    Camera* camera = gScreenOneCtx->camera;
    Ray ray;
    ray.Origin = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);

    // This allows selection of objects in the scene explorer.
    // Otherwise this would still run when selecting buttons in editor windows.
    if (IsInGameScreen()) {
        ray.Direction = ScreenRayTrace();

        ObjectPicker::FindObject(ray, objects);

        std::visit([this, ray](auto* obj) {
            if (obj) {
                eGizmo.SetGizmo(_selected, ray);
                eGizmo.Enabled = true;
            } else {
                eGizmo.Enabled = false;
                _selected = static_cast<GameObject*>(nullptr);
            }
        }, _selected);
    }
}

void ObjectPicker::DragHandle() {
    Camera* camera = gScreenOneCtx->camera;
    Ray ray;
    ray.Origin = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);
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
    std::visit([](auto* obj) {
       if (obj) {
           gEditor.eObjectPicker.eGizmo.Draw();
       }
    }, _selected);

    if (Debug) {
        Camera* camera = gScreenOneCtx->camera;
        Mat4 CursorMtx;
        IRotator rot = IRotator(0,0,0);
        FVector scale = FVector(1, 1, 1);
        FVector ray = ScreenRayTrace();

        float x = (camera->pos[0] + ray.x * 800);
        float y = (camera->pos[1] + ray.y * 800);
        float z = (camera->pos[2] + ray.z * 800);

        ApplyMatrixTransformations((float(*)[4])&CursorMtx, FVector(x, y, z), rot, scale);
        Editor_AddMatrix((float(*)[4])&CursorMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__gizmo/gizmo_center_button");
    }
}

void ObjectPicker::FindObject(Ray ray, std::vector<GameObject*> objects) {
    float distance = FLT_MAX;
    std::variant<AActor*, OObject*, GameObject*> object;

    _selected = static_cast<GameObject*>(nullptr);

    auto [hitActor, hitActorDist] = ObjectPicker::CheckAActorRay(ray);
    if (hitActor) {
        object = hitActor;
        distance = hitActorDist;
    }

    // OObjects
    auto [hitObject, hitObjectDist] = ObjectPicker::CheckOObjectRay(ray);
    if (hitObject && (hitObjectDist < distance)) {
        object = hitObject;
        distance = hitObjectDist;
    }

    // Editor objects
    auto [hitEditorObject, hitEditorObjectDist] = ObjectPicker::CheckEditorObjectRay(ray);
    if (hitEditorObject && (hitEditorObjectDist < distance)) {
        object = hitEditorObject;
        distance = hitEditorObjectDist;
    }

    // Set _selected from object variant
    _selected = object;
    std::visit([this](auto* obj) {
        if (obj) {
        }
    }, object);
}

std::pair<GameObject*, float> ObjectPicker::CheckEditorObjectRay(Ray ray) {
    GameObject* hitObject = nullptr;
    float hitDistance = FLT_MAX;

    for (auto& object : gEditor.eGameObjects) {
        float boundingBox = object->BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }

        switch(object->Collision) {
            case GameObject::CollisionType::VTX_INTERSECT:
                for (const auto& tri : object->Triangles) {
                    float t;
                    if (IntersectRayTriangleAndTransform(ray, object->Pos, tri, t)) {
                        if (t < hitDistance) {
                            hitDistance = t;
                            hitObject = object;
                        }
                    }
                }
                break;
            case GameObject::CollisionType::BOUNDING_BOX: {
                float max = 2.0f;
                float min = -2.0f;
                Vec3f boxMin = { object->Pos.x + boundingBox * min, 
                                 object->Pos.y + boundingBox * min,
                                 object->Pos.z + boundingBox * min };

                Vec3f boxMax = { object->Pos.x + boundingBox * max, 
                                 object->Pos.y + boundingBox * max, 
                                 object->Pos.z + boundingBox * max };
                float t;
                if (QueryCollisionRayActor(&ray.Origin.x, &ray.Direction.x, boxMin, boxMax, &t)) {
                    if (t < hitDistance) {
                        hitDistance = t;
                        hitObject = object;
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
    return std::pair(hitObject, hitDistance);
}

std::pair<OObject*, float> ObjectPicker::CheckOObjectRay(Ray ray) {
    OObject* hitObject = nullptr;
    float hitDistance = FLT_MAX;


    return std::pair(hitObject, hitDistance);
}

std::pair<AActor*, float> ObjectPicker::CheckAActorRay(Ray ray) {
    AActor* hitActor = nullptr;
    float hitDistance = FLT_MAX;

    for (const auto& actor : GetWorld()->Actors) {
        if ((actor->bPendingDestroy) && (!actor->IsMod())) {
            continue;
        }

        float boundingBox = actor->BoundingBoxSize;
        if (boundingBox == 0.0f) {
            boundingBox = 2.0f;
        }

        if (actor->Triangles.size()) {
            for (const auto& tri : actor->Triangles) {
                float t;
                if (IntersectRayTriangleAndTransform(ray, FVector(actor->Pos[0], actor->Pos[1], actor->Pos[2]), tri, t)) {
                    if (t < hitDistance) {
                        hitDistance = t;
                        hitActor = actor.get();
                    }
                }
            }
        } else {
            float max = 1.2f;
            float min = -1.2f;
            Vec3f boxMin = { actor->Pos[0] + boundingBox * min, 
                             actor->Pos[1] + boundingBox * min,
                             actor->Pos[2] + boundingBox * min };

            Vec3f boxMax = { actor->Pos[0] + boundingBox * max, 
                             actor->Pos[1] + boundingBox * max, 
                             actor->Pos[2] + boundingBox * max };
            float t;
            if (QueryCollisionRayActor(&ray.Origin.x, &ray.Direction.x, boxMin, boxMax, &t)) {
                if (t < hitDistance) {
                    hitDistance = t;
                    hitActor = actor.get();
                }
            }
        }
    }

    return std::pair(hitActor, hitDistance);
}

}
