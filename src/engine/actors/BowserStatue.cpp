#include "BowserStatue.h"

#include <libultra/gbi.h>
#include "engine/Matrix.h"
#include "port/interpolation/FrameInterpolation.h"
extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_displaylists.h"
}

Vtx gBowserStatueVtx[717];
Gfx gBowserStatueGfx[162];

ABowserStatue::ABowserStatue(const SpawnParams& params) {
    Name = "Bowser Statue";
    ResourceName = "mk:bowser_statue";
    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    Pos[0] = pos.x; Pos[1] = pos.y; Pos[2] = pos.z;

    IRotator rot = params.Rotation.value_or(IRotator(0, 0, 0));
    Rot[0] = rot.pitch; Rot[1] = rot.yaw; Rot[2] = rot.roll;

    Scale = params.Scale.value_or(FVector(1.0f, 1.0f, 1.0f));

    mBehaviour = static_cast<ABowserStatue::Behaviour>(params.Behaviour.value_or(0));
}

void ABowserStatue::Tick() {
    switch(mBehaviour) {
        case DEFAULT:
            break;
        case CRUSH:
            break;
    }
}

void ABowserStatue::Draw(Camera *camera) {
    Mat4 mtx;
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    
    FVector pos = FVector(Pos[0] + 76, Pos[1], Pos[2] + 1846);

    FrameInterpolation_RecordOpenChild("mk:bowser_statue", TAG_OBJECT(this));
    ApplyMatrixTransformations(mtx, pos, *(IRotator*)Rot, Scale);
    AddObjectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetCombineMode(gDisplayListHead++,G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++,G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPDisplayList(gDisplayListHead++,(Gfx*) d_course_bowsers_castle_packed_dl_2BB8);
    FrameInterpolation_RecordCloseChild();
}

bool ABowserStatue::IsMod() { return true; }
