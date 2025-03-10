#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Gizmo.h"
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
#include "courses/harbour/ship_model.h"
#include "src/racing/collision.h"
}

namespace EditorNamespace {

Gizmo::Gizmo() {
}

void Gizmo::Load() {
    RedCollision.Pos = &Pos;
    RedCollision.Model = handle_Cylinder_mesh;

    GreenCollision.Pos = &Pos;
    GreenCollision.Model = handle_Cylinder_mesh;

    BlueCollision.Pos = &Pos;
    BlueCollision.Model = handle_Cylinder_mesh;

    GenerateCollisionMesh(RedCollision, RedCollision.Model, 0.05f);
    GenerateCollisionMesh(GreenCollision, GreenCollision.Model, 0.05f);
    GenerateCollisionMesh(BlueCollision, BlueCollision.Model, 0.05f);
}

void Gizmo::Tick() {
    if (Enabled) {
        Gizmo::Translate();
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

// void Gizmo::Rotate() {

// }

// void Gizmo::Scale() {
    
// }

void Gizmo::Draw() {
    if (Enabled) {
        DrawHandles();
    }
}

void Gizmo::DrawHandles() {
    Mat4 mainMtx;
    
    Vec3s rot = {0, 0, 0};
    mtxf_pos_rotation_xyz(mainMtx, &Pos.x, rot);
    //mtxf_scale(mainMtx, 0.05f);
    if (render_set_position(mainMtx, 0) != 0) {
        //gSPDisplayList(gDisplayListHead++, wheels_Spaghetti_002_mesh);
        //gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }

    handle_f3dlite_material_lights = gdSPDefLights1(
        0x7F, 0x7F, 0x7F,
        0xFF, 0, 0, 0x49, 0x49, 0x49);
        
    Mat4 RedXMtx;
    Vec3s rot1 = {0, 0, 0};
    Vec3f pos1 = {Pos.x, Pos.y, Pos.z - _gizmoOffset};
    mtxf_pos_rotation_xyz(RedXMtx, pos1, rot1);
    mtxf_scale(RedXMtx, 0.05);
    if (render_set_position(RedXMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }

    Vec3s rot2 = {0, 0x4000, 0};

    handle_f3dlite_material_lights = gdSPDefLights1(
        0x7F, 0x7F, 0x7F,
        0, 0xFF, 0, 0x49, 0x49, 0x49);

    Mat4 GreenYMtx;
    Vec3f pos2 = {Pos.x - _gizmoOffset, Pos.y, Pos.z};
    mtxf_pos_rotation_xyz(GreenYMtx, pos2, rot2);
    mtxf_scale(GreenYMtx, 0.05);
    if (render_set_position(GreenYMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }

    Vec3s rot3 = {0x4000, 0, 0};

    handle_f3dlite_material_lights = gdSPDefLights1(
        0x7F, 0x7F, 0x7F,
        0, 0, 0xFF, 0x49, 0x49, 0x49);

    Mat4 BlueZMtx;
    Vec3f pos3 = {Pos.x, Pos.y + _gizmoOffset, Pos.z};
    mtxf_pos_rotation_xyz(BlueZMtx, pos3, rot3);
    mtxf_scale(BlueZMtx, 0.05);
    if (render_set_position(BlueZMtx, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }
}
}
