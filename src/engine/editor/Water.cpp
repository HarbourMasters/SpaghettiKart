#include <iostream>
#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "Water.h"
#include "../CoreMath.h"
#include <libultra/types.h>
#include "../World.h"

#include "port/Engine.h"

#include "engine/actors/Ship.h"
#include "port/Game.h"
#include "Gizmo.h"

#include "EditorMath.h"

extern "C" {
#include "main.h"
#include "common_structs.h"
#include "defines.h"
#include "actors.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
}

namespace Editor {

    size_t WaterVolumeObject::NumWaterVolumes = 0;

    WaterVolumeObject::WaterVolumeObject(const char* name, FVector* pos) : GameObject() {
        Name = name;
        Pos = &WaterPos;
        Rot = &WaterRot;
        Scale = &WaterScale;

        DespawnFlag = &_despawnFlag;
        DespawnValue = -1;

        Collision = CollisionType::VTX_INTERSECT;

        NumWaterVolumes += 1;
    }

    void WaterVolumeObject::Load() {
    }

    void WaterVolumeObject::Tick() {
    }
    void WaterVolumeObject::Draw() {
        Mat4 mtx;
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        IRotator rot = WaterRot;
        rot.yaw += 0x4000;
        ApplyMatrixTransformations(mtx, WaterPos, rot, WaterScale);
        Editor_AddMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*)"__OTR__editor/water/water_plane");
    }
}
