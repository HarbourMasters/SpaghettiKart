#include <iostream>
#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"
#include "engine/Matrix.h"

#include "Light.h"
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
#include "math_util.h"
#include "math_util_2.h"
}

namespace Editor {

size_t LightObject::NumLights = 0;

    LightObject::LightObject(const char* name, FVector* pos, s8* direction) {
        Name = name;
        ResourceName = "editor:light";

        Pos = FVector(0, 100, 0);
        Rot = IRotator(0, 0, 0);
        Scale = FVector(0.1, 0.1, 0.1);

        SpawnPos = Pos;
        SpawnRot = Rot;

        Direction = direction;

        Collision = CollisionType::BOUNDING_BOX;
        BoundingBoxSize = 4.0f;

        NumLights += 1;
    }

    void LightObject::Load() {
    }

    void LightObject::Tick() {
        SetDirectionFromRotator(Rot, Direction);
    }
    void LightObject::Draw() {
        Camera* camera = gScreenOneCtx->camera;
        Mat4 mtx_sun;
        Editor_MatrixIdentity(mtx_sun);
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);


        // Calculate camera-to-object distance
        FVector cameraDir = FVector(Pos.x - camera->pos[0], Pos.y - camera->pos[1], Pos.z - camera->pos[2]);
        cameraDir = cameraDir.Normalize();

        IRotator centerRot;
        SetRotatorFromDirection(cameraDir, &centerRot);

        // The sun was exported facing the wrong direction.
        // Thus, force the sun texture to face the camera.
        centerRot.yaw += 0x4000;
        ApplyMatrixTransformations(mtx_sun, Pos, centerRot, Scale);
        Editor_AddMatrix(mtx_sun, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, sun_LightModel_mesh);

        // Draw Arrow
        Mat4 mtx_arrow;
        ApplyMatrixTransformations(mtx_arrow, Pos, Rot, Scale);
        Editor_AddMatrix(mtx_arrow, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__editor/light/sun_arrow");
    }
}
