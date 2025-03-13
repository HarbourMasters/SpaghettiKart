#include <iostream>
#include <libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "Light.h"
#include "port/Engine.h"
#include <controller/controldevice/controller/mapping/keyboard/KeyboardScancodes.h>
#include <window/Window.h>

#include "engine/actors/Ship.h"
#include "port/Game.h"
#include "Gizmo.h"

#include "EditorMath.h"
#include "Handle.h"

extern "C" {
#include "common_structs.h"
#include "main.h"
#include "defines.h"
#include "actors.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
}

namespace Editor {

size_t LightObject::NumLights = 0;

    LightObject::LightObject(const char* name, FVector* pos, s8* direction) : GameObject(nullptr, nullptr) {
        Name = name;
        // If pos is nullptr, allocate memory, otherwise assign pos
        // if (pos != nullptr) {
        //     Pos = pos;
        // }
        Pos = &LightPos;
        Rot = &LightRot;

        // Pos = &LightPos;
        DespawnFlag = &_despawnFlag;
        DespawnValue = -1;

        Direction = direction;

        NumLights += 1;
    }

    void LightObject::Load() {
        //Pos = &LightPos;
    }

    void LightObject::Tick() {
        //Pos = &LightPos;
        SetDirectionFromRotator(*Rot, Direction);
        //Direction[0] = static_cast<char>(((*Rot)[0] / 256 - 128) & 0xFF);
        //Direction[1] = static_cast<char>(((*Rot)[1] / 256 - 128) & 0xFF);
        //Direction[2] = static_cast<char>(((*Rot)[2] / 256 - 128) & 0xFF);
        //printf("Light rot %d %d %d %d %d %d\n", Direction[0], Direction[1], Direction[2], (*Rot)[0], (*Rot)[1], (*Rot)[2]);
    }
    void LightObject::Draw() {
        Mat4 mtx;

        Vec3f pos = {Pos->x, Pos->y, Pos->z};
        Vec3s rot = {(*Rot)[0], (*Rot)[1] + 0x4000, (*Rot)[2]};
        mtxf_pos_rotation_xyz(mtx, pos, rot);
        mtxf_scale(mtx, 0.3);
        Editor_AddMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, sun_LightModel_mesh);
        gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }
}
