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
        Mat4 mtx_sun;
        Editor_MatrixIdentity(mtx_sun);
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);


        // Calculate camera-to-object distance
        FVector cameraDir = FVector(LightPos.x - cameras[0].pos[0], LightPos.y - cameras[0].pos[1], LightPos.z - cameras[0].pos[2]);
        cameraDir = cameraDir.Normalize();

        IRotator centerRot;
        SetRotatorFromDirection(cameraDir, &centerRot);

        // Account for object not facing the correct direction when exported
        centerRot.yaw += 0x4000;
        ApplyMatrixTransformations(mtx_sun, LightPos, centerRot, LightScale);
        Editor_AddMatrix(mtx_sun, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, sun_LightModel_mesh);

        // Draw Arrow
        Mat4 mtx_arrow;
        IRotator rot = LightRot;
        rot.yaw += 0x4000;
        ApplyMatrixTransformations(mtx_arrow, LightPos, rot, LightScale);
        Editor_AddMatrix(mtx_arrow, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__editor/light/sun_arrow");
    }
}
