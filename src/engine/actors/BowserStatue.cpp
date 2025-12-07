#include "BowserStatue.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
}

Vtx gBowserStatueVtx[717];
Gfx gBowserStatueGfx[162];

ABowserStatue::ABowserStatue(FVector pos, ABowserStatue::Behaviour behaviour) {
    Name = "Bowser Statue";
    ResourceName = "mk:bowser_statue";
    Pos = pos;
    ABowserStatue::Behaviour _behaviour = behaviour;
}

void ABowserStatue::Tick() {
    switch(_behaviour) {
        case DEFAULT:
            break;
        case CRUSH:
            break;
    }
}

void ABowserStatue::Draw(Camera *camera) {
    Mat4 mtx;
    Vec3f pos;
    pos[0] = Pos.x + 76;
    pos[1] = Pos.y;
    pos[2] = Pos.z + 1846;
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(mtx, pos, Rot);
    if (render_set_position(mtx, 0) != 0) {

        gSPDisplayList(gDisplayListHead++, gBowserStatueGfx);
    }
}

bool ABowserStatue::IsMod() { return true; }
