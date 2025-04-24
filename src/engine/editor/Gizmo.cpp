#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "EditorMath.h"
#include "Gizmo.h"
#include "Collision.h"
#include "port/Engine.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>


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
    RedCollision.Pos = &Pos;
    RedCollision.Model = (Gfx*)"__OTR__editor/gizmo/translate_handle_red";

    GreenCollision.Pos = &Pos;
    GreenCollision.Model = (Gfx*)"__OTR__editor/gizmo/translate_handle_green";

    BlueCollision.Pos = &Pos;
    BlueCollision.Model = (Gfx*)"__OTR__editor/gizmo/translate_handle_blue";

    /* Rotate handle collision */
    RedRotateCollision.Pos = &Pos;
    RedRotateCollision.Model = (Gfx*)"__OTR__editor/gizmo/rot_handle_red";

    GreenRotateCollision.Pos = &Pos;
    GreenRotateCollision.Model = (Gfx*)"__OTR__editor/gizmo/rot_handle_green";

    BlueRotateCollision.Pos = &Pos;
    BlueRotateCollision.Model = (Gfx*)"__OTR__editor/gizmo/rot_handle_blue";

    /* Scale handle collision */
    RedScaleCollision.Pos = &Pos;
    RedScaleCollision.Model = (Gfx*)"__OTR__editor/gizmo/scale_handle_red";

    GreenScaleCollision.Pos = &Pos;
    GreenScaleCollision.Model = (Gfx*)"__OTR__editor/gizmo/scale_handle_green";

    BlueScaleCollision.Pos = &Pos;
    BlueScaleCollision.Model = (Gfx*)"__OTR__editor/gizmo/scale_handle_blue";

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
void Gizmo::SetGizmo(GameObject* object, Ray ray) {
    _selected = object;
    _ray = ray.Direction;
    Pos = FVector(
        object->Pos->x,
        object->Pos->y,
        object->Pos->z
    );
}

void Gizmo::SetGizmoNoCursor(GameObject* object) {
    _selected = object;
    Pos = FVector(
        object->Pos->x,
        object->Pos->y,
        object->Pos->z
    );
}

void Gizmo::Translate() {
    static float length = 180.0f; // Default value

    // Prevent nullptr exceptions
    if (_selected == NULL || _selected->Pos == NULL) {
        return;
    }

    if (Enabled) {
        length = sqrt(
            pow(_selected->Pos->x - cameras[0].pos[0], 2) +
            pow(_selected->Pos->y - cameras[0].pos[1], 2) +
            pow(_selected->Pos->z - cameras[0].pos[2], 2)
        );

        switch(SelectedHandle) {
            case GizmoHandle::All_Axis:
                _selected->Pos->x = (cameras[0].pos[0] + _ray.x * PickDistance) + _cursorOffset.x;
                _selected->Pos->y = (cameras[0].pos[1] + _ray.y * PickDistance) + _cursorOffset.y;
                _selected->Pos->z = (cameras[0].pos[2] + _ray.z * PickDistance) + _cursorOffset.z;
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    _selected->Pos->y = SnapToSurface(_selected->Pos);
                }
                break;
            case GizmoHandle::X_Axis:
                _selected->Pos->x = (cameras[0].pos[0] + _ray.x * length) + _cursorOffset.x;
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    _selected->Pos->y = SnapToSurface(_selected->Pos);
                }
                break;
            case GizmoHandle::Y_Axis:
                _selected->Pos->y = (cameras[0].pos[1] + _ray.y * length) + _cursorOffset.y;
                break;
            case GizmoHandle::Z_Axis:
                _selected->Pos->z = (cameras[0].pos[2] + _ray.z * length) + _cursorOffset.z;
                if (CVarGetInteger("gEditorSnapToGround", 0) == true) {
                    _selected->Pos->y = SnapToSurface(_selected->Pos);
                }
                break;
        }

        if (CVarGetInteger("gEditorBoundary", 0) == true) {
            _selected->Pos->x = MAX(_selected->Pos->x, dimensions.MinX);
            _selected->Pos->x = MIN(_selected->Pos->x, dimensions.MaxX);

            _selected->Pos->y = MAX(_selected->Pos->y, dimensions.MinY);
            _selected->Pos->y = MIN(_selected->Pos->y, dimensions.MaxY);

            _selected->Pos->z = MAX(_selected->Pos->z, dimensions.MinZ);
            _selected->Pos->z = MIN(_selected->Pos->z, dimensions.MaxZ);
        }

        Pos = FVector(
            _selected->Pos->x,
            _selected->Pos->y,
            _selected->Pos->z
        );
    }
}

f32 Gizmo::SnapToSurface(FVector* pos) {
    float y;
    y = spawn_actor_on_surface(pos->x, 2000.0f, pos->z);

    if (y == 3000.0f || y == -3000.0f) {
        y = pos->y;
    }

    return y;
}

void Gizmo::Rotate() {
    FVector cam = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);

    if (_selected == nullptr || _selected->Rot == nullptr) {
        return;
    }

    // Store initial scale at the beginning of the drag
    if (ManipulationStart) {
        ManipulationStart = false;
        InitialRotation = *_selected->Rot;  // Store initial rotation
    }

    // Initial click position
    FVector clickPos = *_selected->Pos - _cursorOffset;

    // Calculate difference
    FVector diff = (cam + _ray * PickDistance) - clickPos;

    // Set rotation sensitivity
    diff = diff * 100.0f;
    switch (SelectedHandle) {
        case GizmoHandle::X_Axis:
            _selected->Rot->pitch = (uint16_t)InitialRotation.pitch + diff.x;
            break;
        case GizmoHandle::Y_Axis:
            _selected->Rot->yaw = (uint16_t)InitialRotation.yaw + diff.y;
            break;
        case GizmoHandle::Z_Axis:
            _selected->Rot->roll = (uint16_t)InitialRotation.roll + diff.z;
            break;
    }
}

void Gizmo::Scale() {
    FVector cam = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);
    if (_selected == nullptr || _selected->Scale == nullptr) {
        return;
    }

    // Store initial scale at the beginning of the drag
    if (ManipulationStart) {
        ManipulationStart = false;
        InitialScale = *_selected->Scale;
    }

    // Initial click position
    FVector clickPos = *_selected->Pos - _cursorOffset;

    // Calculate difference
    FVector diff = (cam + _ray * PickDistance) - clickPos;

    // Lower scaling sensitivity
    diff = diff * 0.01f;

    switch (SelectedHandle) {
        case GizmoHandle::X_Axis:
            _selected->Scale->x = InitialScale.x + -diff.x;
            break;
        case GizmoHandle::Y_Axis:
            _selected->Scale->y = InitialScale.y + diff.y;
            break;
        case GizmoHandle::Z_Axis:
            _selected->Scale->z = InitialScale.z + -diff.z;
            break;
        case GizmoHandle::All_Axis:
            float uniformScale = (diff.x - diff.y - diff.z) / 3.0f;
            uniformScale *= 1.8; // Increased sensitivity
            _selected->Scale->x = uniformScale;
            _selected->Scale->y = uniformScale;
            _selected->Scale->z = uniformScale;
            break;
    }
}

void Gizmo::Draw() {
    if (Enabled) {
        DrawHandles();
        //DebugCollision(&RedCollision, Pos, {0, 0, 0}, {0.05f, 0.05f, 0.05f}, RedCollision.Triangles);
        //DebugCollision(&BlueCollision, Pos, {90, 0, 0}, {0.05f, 0.05f, 0.05f}, BlueCollision.Triangles);
        //DebugCollision(&GreenCollision, Pos, {0, 90, 0}, {0.05f, 0.05f, 0.05f}, GreenCollision.Triangles);
        //DebugCollision(&RedRotateCollision, Pos, {0, 0, 0}, {0.15f, 0.15f, 0.15f}, RedRotateCollision.Triangles);
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
        Mat4 CenterMtx;
        Editor_MatrixIdentity(CenterMtx);

        // Calculate camera-to-object distance
        FVector cameraDir = FVector(Pos.x - cameras[0].pos[0], Pos.y - cameras[0].pos[1], Pos.z - cameras[0].pos[2]);
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
