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
}

Gizmo::Gizmo() {
}

void Gizmo::Tick() {
    
}

std::pair<FVector, FVector> Gizmo::GetBoundingBox(GizmoHandle handle) {
    FVector min = Pos + HandleOffsets[(int)handle] - FVector(HandleSize, HandleSize, HandleSize);
    FVector max = Pos + HandleOffsets[(int)handle] + FVector(HandleSize, HandleSize, HandleSize);
    return {min, max};
}

void Gizmo::StartManipulation(GizmoHandle handle) {
    printf("HANDLE SELECTED %d\n", (s32)handle);
    auto wnd = GameEngine::Instance->context->GetWindow();

    if (wnd->GetMouseState(Ship::LUS_MOUSE_BTN_LEFT)) {
        switch(handle) {
            case GizmoHandle::None:
                break;
            case GizmoHandle::Center:
                break;
            case GizmoHandle::X_Axis:
                break;
            case GizmoHandle::Y_Axis:
            Gizmo::Translate();
                break;
            case GizmoHandle::Z_Axis:
                break;
        }
    }
}

void Gizmo::Enable(Vec3f* object, FVector ray) {
    static float length = 180.0f; // Default value

    // static AActor* lastSelected = nullptr;
    // if (object != lastSelected) {
    //     length = sqrt(
    //         pow(object->Pos[0] - cameras[0].pos[0], 2) +
    //         pow(object->Pos[1] - cameras[0].pos[1], 2) +
    //         pow(object->Pos[2] - cameras[0].pos[2], 2)
    //     );
    //     lastSelected = object;
    // }

    //_selected->Pos[0] = cameras[0].pos[0] + ray.x * length;
    //object->Pos[1] = cameras[0].pos[1] + ray.y * length;
    //_selected->Pos[2] = cameras[0].pos[2] + ray.z * length;
    _selected = &object[0];
    _ray = ray;
    Pos = FVector(
        object[0][0],
        object[0][1],
        object[0][2]
    );
}

void Gizmo::Translate() {
    static float length = 180.0f; // Default value

    if (Enabled) {
        //FVector ray = ScreenRayTrace();

        length = sqrt(
            pow(_selected[0][0] - cameras[0].pos[0], 2) +
            pow(_selected[0][1] - cameras[0].pos[1], 2) +
            pow(_selected[0][2] - cameras[0].pos[2], 2)
        );

        //_selected->Pos[0] = cameras[0].pos[0] + ray.x * length;
        _selected[0][0] = cameras[0].pos[1] + _ray.y * length;
        //_selected->Pos[2] = cameras[0].pos[2] + ray.z * length;
        Pos = FVector(
            _selected[0][0],
            _selected[0][1],
            _selected[0][2]
        );
    }
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
