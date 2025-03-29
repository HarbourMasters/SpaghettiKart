#include <iostream>
#include <libultraship/libultraship.h>
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
        Pos = &LightPos;
        Rot = &LightRot;
        Scale = &LightScale;

        DespawnFlag = &_despawnFlag;
        DespawnValue = -1;

        Direction = direction;

        Collision = CollisionType::BOUNDING_BOX;
        BoundingBoxSize = 4.0f;

        NumLights += 1;
    }

    void LightObject::Load() {
    }

    void LightObject::Tick() {
        SetDirectionFromRotator(*Rot, Direction);
    }
    void LightObject::Draw() {
        Mat4 mtx;

        ApplyMatrixTransformations(mtx, LightPos, LightRot, LightScale);
        Editor_AddMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, sun_LightModel_mesh);
        gSPDisplayList(gDisplayListHead++, handle_Cylinder_mesh);
    }
}
