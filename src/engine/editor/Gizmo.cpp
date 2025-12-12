#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "EditorMath.h"
#include "Gizmo.h"
#include "Collision.h"
#include "port/Engine.h"
#include <ship/controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <ship/window/Window.h>
#include "engine/Matrix.h"
#include "engine/Actor.h"
#include "engine/objects/Object.h"
#include "engine/editor/GameObject.h"


#include "engine/actors/Ship.h"
#include "port/Game.h"
#include "Handles.h"

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "actors.h"
#include "camera.h"
#include "src/racing/collision.h"
#include "math_util.h"
}

namespace Editor {

void Gizmo::Load() {
    /* Translate handle collision */
    RedCollision.Pos = Pos;
    RedCollision.Model = "__OTR__editor/gizmo/translate_handle_red";

    GreenCollision.Pos = Pos;
    GreenCollision.Model = "__OTR__editor/gizmo/translate_handle_green";

    BlueCollision.Pos = Pos;
    BlueCollision.Model = "__OTR__editor/gizmo/translate_handle_blue";

    /* Rotate handle collision */
    RedRotateCollision.Pos = Pos;
    RedRotateCollision.Model = "__OTR__editor/gizmo/rot_handle_red";

    GreenRotateCollision.Pos = Pos;
    GreenRotateCollision.Model = "__OTR__editor/gizmo/rot_handle_green";

    BlueRotateCollision.Pos = Pos;
    BlueRotateCollision.Model = "__OTR__editor/gizmo/rot_handle_blue";

    /* Scale handle collision */
    RedScaleCollision.Pos = Pos;
    RedScaleCollision.Model = "__OTR__editor/gizmo/scale_handle_red";

    GreenScaleCollision.Pos = Pos;
    GreenScaleCollision.Model = "__OTR__editor/gizmo/scale_handle_green";

    BlueScaleCollision.Pos = Pos;
    BlueScaleCollision.Model = "__OTR__editor/gizmo/scale_handle_blue";

    GenerateCollisionMesh(&RedCollision, (Gfx*)LOAD_ASSET_RAW(RedCollision.Model), 1.0f);
    GenerateCollisionMesh(&GreenCollision, (Gfx*)LOAD_ASSET_RAW(GreenCollision.Model), 1.0f);
    GenerateCollisionMesh(&BlueCollision, (Gfx*)LOAD_ASSET_RAW(BlueCollision.Model), 1.0f);

    GenerateCollisionMesh(&RedRotateCollision, (Gfx*)LOAD_ASSET_RAW(RedRotateCollision.Model), 1.0f);
    GenerateCollisionMesh(&GreenRotateCollision, (Gfx*)LOAD_ASSET_RAW(GreenRotateCollision.Model), 1.0f);
    GenerateCollisionMesh(&BlueRotateCollision, (Gfx*)LOAD_ASSET_RAW(BlueRotateCollision.Model), 1.0f);

    GenerateCollisionMesh(&RedScaleCollision, (Gfx*)LOAD_ASSET_RAW(RedScaleCollision.Model), 1.0f);
    GenerateCollisionMesh(&GreenScaleCollision, (Gfx*)LOAD_ASSET_RAW(GreenScaleCollision.Model), 1.0f);
    GenerateCollisionMesh(&BlueScaleCollision, (Gfx*)LOAD_ASSET_RAW(BlueScaleCollision.Model), 1.0f);
}

void Gizmo::Tick() {
    if (Enabled) {
        TranslationMode mode = static_cast<TranslationMode>(CVarGetInteger("eGizmoMode", 0));
        switch(mode) {
            case TranslationMode::Move:
                Gizmo::Translate();
                break;
            case TranslationMode::Rotate:
                Gizmo::Rotate();
                break;
            case TranslationMode::Scale:
                Gizmo::Scale();
                break;
        }
    }
}

// Makes the gizmo visible
void Gizmo::SetGizmo(const std::variant<AActor*, OObject*, GameObject*>& object, Ray ray) {
    _ray = ray.Direction;
    std::visit([this](auto* obj) {
        _selected = obj;
        this->Pos = obj->GetLocation();
    }, object);
}

void Gizmo::SetGizmoNoCursor(const std::variant<AActor*, OObject*, GameObject*>& object) {
    std::visit([this](auto* obj) {
        _selected = obj;
        Pos = obj->GetLocation();
    }, object);
}

void Gizmo::Translate() {
    static float length = 180.0f; // Default value

    std::visit([this](auto* obj) {
        Camera* camera = gScreenOneCtx->camera;
        float x, y, z = 0;
        if (nullptr == obj) {
            return;
        }

        const FVector location = obj->GetLocation();

        length = sqrt(
            pow(location.x - camera->pos[0], 2) +
            pow(location.y - camera->pos[1], 2) +
            pow(location.z - camera->pos[2], 2)
        );

        switch(this->SelectedHandle) {
            case GizmoHandle::All_Axis:
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    y = SnapToSurface(location);
                } else {
                    y = ((camera->pos[1] + _ray.y * PickDistance) + _cursorOffset.y);
                }

                obj->Translate(
                    FVector(
                        ((camera->pos[0] + _ray.x * PickDistance) + _cursorOffset.x),
                        y,
                        ((camera->pos[2] + _ray.z * PickDistance) + _cursorOffset.z)
                    )
                );
                break;
            case GizmoHandle::X_Axis:
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    y = SnapToSurface(location);
                } else {
                    y = location.y;  // Preserve Y
                }

                obj->Translate(
                    FVector(
                        ((camera->pos[0] + _ray.x * length) + _cursorOffset.x),
                        y,
                        location.z  // Preserve Z
                    )
                );
                break;
            case GizmoHandle::Y_Axis:
                obj->Translate(
                    FVector(
                        location.x, // Preserve X
                        ((camera->pos[1] + _ray.y * length) + _cursorOffset.y),
                        location.z  // Preserve Z
                    )
                );
                break;
            case GizmoHandle::Z_Axis:
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    y = SnapToSurface(location);
                } else {
                    y = location.y; // Preserve Y
                }
                obj->Translate(
                    FVector(
                        location.x, // Preserve X
                        y,
                        ((camera->pos[2] + _ray.z * length) + _cursorOffset.z)
                    )
                );
                break;
        }

        FVector newLoc = obj->GetLocation();
        x = newLoc.x;
        y = newLoc.y;
        z = newLoc.z;


        if (CVarGetInteger("gEditorBoundary", 0) == true) {
#define EDITOR_CLAMP(value, min, max) ((value) < (min) ? min : (value) > (max) ? max : value)
            x = EDITOR_CLAMP(newLoc.x, dimensions.MinX, dimensions.MaxX);
            y = EDITOR_CLAMP(newLoc.y, dimensions.MinY, dimensions.MaxY);
            z = EDITOR_CLAMP(newLoc.z, dimensions.MinZ, dimensions.MaxZ);
            obj->Translate(FVector(x, y, z));
#undef EDITOR_CLAMP
        }

        // Update the gizmo position
        Pos = FVector(x, y, z);

    // Pass the _selected object into this lambda function
    }, _selected);
}

f32 Gizmo::SnapToSurface(const FVector pos) {
    float y;
    y = spawn_actor_on_surface(pos.x, 2000.0f, pos.z);

    if (y == 3000.0f || y == -3000.0f) {
        y = pos.y;
    }

    return y;
}

void Gizmo::Rotate() {
    std::visit([this](auto* obj) {
        Camera* camera = gScreenOneCtx->camera;
        FVector cam = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);
        IRotator rot;

        if (nullptr == obj) {
            return;
        }

        // Store initial scale at the beginning of the drag
        if (ManipulationStart) {
            ManipulationStart = false;
            InitialRotation = obj->GetRotation();  // Store initial rotation
        }

        // Initial click position
        FVector clickPos = obj->GetLocation() - _cursorOffset;

        // Calculate difference
        FVector diff = (cam + _ray * PickDistance) - clickPos;

        // Set rotation sensitivity
        diff = diff * 100.0f;
        switch (SelectedHandle) {
            case GizmoHandle::X_Axis:
                rot.Set(
                (uint16_t)(InitialRotation.pitch + diff.x),
                InitialRotation.yaw,
                InitialRotation.roll
            );
            break;
            case GizmoHandle::Y_Axis:
                rot.Set(
                    InitialRotation.pitch,
                    (uint16_t)(InitialRotation.yaw + diff.y),
                    InitialRotation.roll
                );
                break;
            case GizmoHandle::Z_Axis:
                rot.Set(
                    InitialRotation.pitch,
                    InitialRotation.yaw,
                    (uint16_t)(InitialRotation.roll + diff.z)
                );
                break;
        }
        obj->Rotate(rot);
    // Pass the _selected object into this lambda function
    }, _selected);
}

void Gizmo::Scale() {
    std::visit([this](auto* obj) {
        Camera* camera = gScreenOneCtx->camera;
        FVector cam = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);
        if (nullptr == obj) {
            return;
        }

        // Store initial scale at the beginning of the drag
        if (ManipulationStart) {
            ManipulationStart = false;
            InitialScale = obj->GetScale();
        }

        FVector scale = obj->GetScale();

        // Initial click position
        FVector clickPos = obj->GetLocation() - _cursorOffset;

        // Calculate difference
        FVector diff = (cam + _ray * PickDistance) - clickPos;

        // Lower scaling sensitivity
        diff = diff * 0.01f;

        switch (SelectedHandle) {
            case GizmoHandle::X_Axis:
                obj->SetScale(
                    FVector(
                        (InitialScale.x + -diff.x),
                        scale.y,
                        scale.z
                    )
                );
                break;
            case GizmoHandle::Y_Axis:
               obj->SetScale(
                    FVector(
                        scale.x,
                        (InitialScale.y + diff.y),
                        scale.z
                    )
                );
                break;
            case GizmoHandle::Z_Axis:
                obj->SetScale(
                    FVector(
                        scale.x,
                        scale.y,
                        (InitialScale.z + -diff.z)
                    )
                );
                break;
            case GizmoHandle::All_Axis:
                float uniformScale = (diff.x - diff.y - diff.z) / 3.0f;
                uniformScale *= 1.8; // Increased sensitivity
                obj->SetScale(
                    FVector(
                        uniformScale,
                        uniformScale,
                        uniformScale
                    )
                );
                break;
        }
    // Pass the _selected object into this lambda function
    }, _selected);
}

void Gizmo::Draw() {
    if (Enabled) {
        DrawHandles();
        // DebugCollision(&RedCollision, Pos, {0, 0, 0}, {0.05f, 0.05f, 0.05f}, RedCollision.Triangles);
        // DebugCollision(&BlueCollision, Pos, {90, 0, 0}, {0.05f, 0.05f, 0.05f}, BlueCollision.Triangles);
        // DebugCollision(&GreenCollision, Pos, {0, 90, 0}, {0.05f, 0.05f, 0.05f}, GreenCollision.Triangles);
        // DebugCollision(&RedRotateCollision, Pos, {0, 0, 0}, {0.15f, 0.15f, 0.15f}, RedRotateCollision.Triangles);
        //DebugCollision((uintptr_t)_selected, Pos, BlueRotateCollision.Triangles);
        //DebugCollision((uintptr_t)_selected, Pos, GreenRotateCollision.Triangles);
    }
}

void Gizmo::DrawHandles() {
    Mat4 mainMtx;
    Editor_MatrixIdentity((float(*)[4])&Mtx_RedX);
    Editor_MatrixIdentity((float(*)[4])&Mtx_GreenY);
    Editor_MatrixIdentity((float(*)[4])&Mtx_BlueZ);

    const char* blueHandle;
    const char* greenHandle;
    const char* redHandle;
    const char* center = nullptr;
    IRotator blueRot;
    IRotator greenRot;
    IRotator redRot;
    FVector scale = {0.05f, 0.05f, 0.05f};

    switch(static_cast<TranslationMode>(CVarGetInteger("eGizmoMode", 0))) {
        case TranslationMode::Move:
            center = "__OTR__editor/gizmo/center_handle";
            blueHandle = "__OTR__editor/gizmo/translate_handle_blue";
            greenHandle = "__OTR__editor/gizmo/translate_handle_green";
            redHandle = "__OTR__editor/gizmo/translate_handle_red";
            _gizmoOffset = 8.0f;
            greenRot = {0, 90, 0};
            blueRot = {90, 0, 0};
            scale = {0.3, 0.3, 0.3};
            break;
        case TranslationMode::Rotate:
            center = nullptr; // No All_Axis drag button for Rotation
            blueHandle = "__OTR__editor/gizmo/rot_handle_blue";
            greenHandle = "__OTR__editor/gizmo/rot_handle_green";
            redHandle = "__OTR__editor/gizmo/rot_handle_red";
            _gizmoOffset = 0.0f;
            scale = {0.15f, 0.15f, 0.15f};
            break;
        case TranslationMode::Scale:
            center = "__OTR__editor/gizmo/center_handle";
            blueHandle = "__OTR__editor/gizmo/scale_handle_blue";
            greenHandle = "__OTR__editor/gizmo/scale_handle_green";
            redHandle = "__OTR__editor/gizmo/scale_handle_red";
            _gizmoOffset = 8.0f;
            greenRot = {0, 90, 0};
            blueRot = {90, 0, 0};
            scale = {0.05f, 0.05f, 0.05f};
            break;
    }

    ApplyMatrixTransformations(mainMtx, Pos, Rot, {1, 1, 1});
    Editor_AddMatrix(mainMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (center) {
        Camera* camera = gScreenOneCtx->camera;
        Mat4 CenterMtx;
        Editor_MatrixIdentity(CenterMtx);

        // Calculate camera-to-object distance
        FVector cameraDir = FVector(Pos.x - camera->pos[0], Pos.y - camera->pos[1], Pos.z - camera->pos[2]);
        cameraDir = cameraDir.Normalize();

        IRotator centerRot;
        SetRotatorFromDirection(cameraDir, &centerRot);
        centerRot.pitch += 0x4000; // Align mesh to face camera since it was not exported facing the correct direction
        centerRot.yaw += 0x4000;

        ApplyMatrixTransformations(CenterMtx, Pos, centerRot, FVector(0.06f, 0.06f, 0.06f));
        Editor_AddMatrix(CenterMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)center);
    }

    ApplyMatrixTransformations((float(*)[4])&Mtx_RedX, FVector(Pos.x, Pos.y, Pos.z - _gizmoOffset), Rot, scale);
    Editor_AddMatrix((float(*)[4])&Mtx_RedX, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*)redHandle);

    ApplyMatrixTransformations((float(*)[4])&Mtx_GreenY, FVector(Pos.x - _gizmoOffset, Pos.y, Pos.z), greenRot, scale);
    Editor_AddMatrix((float(*)[4])&Mtx_GreenY, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*)greenHandle);

    ApplyMatrixTransformations((float(*)[4])&Mtx_BlueZ, FVector(Pos.x, Pos.y + _gizmoOffset, Pos.z), blueRot, scale);
    Editor_AddMatrix((float(*)[4])&Mtx_BlueZ, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, (Gfx*)blueHandle);
}
}
