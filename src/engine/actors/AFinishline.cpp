#include <libultraship.h>
#include <libultra/gbi.h>

#include "AFinishline.h"
#include "engine/Actor.h"
#include "World.h"
#include "assets/common_data.h"

extern "C" {
#include "macros.h"
#include "common_structs.h"
#include "math_util.h"
#include "actor_types.h"
#include "actors.h"
extern f32 gKartHopInitialVelocityTable[];
extern f32 gKartGravityTable[];
}

AFinishline::AFinishline(Vec3f pos) {
    if (pos != nullptr) { // Allow user to set their own values
        Pos[0] = pos[0];
        Pos[1] = pos[1] - 15;
        Pos[2] = pos[2];
    } else { // Default to first waypoint position
        Pos[0] = D_80164490->posX;
        Pos[1] = (f32) (D_80164490->posY - 15);
        Pos[2] = D_80164490->posZ;
    }

    Rot[0] = 0;
    Rot[1] = 0;
    Rot[2] = 0;

    Flags = -0x8000 | 0x4000;

    BoundingBoxSize = 0.0f;
}

void AFinishline::Tick() {
}

extern Gfx cloud_mesh[];

void AFinishline::Draw(Camera* camera) {
    Mat4 mtx;
    s16 temp = Pos[2];
    s32 maxObjectsReached;

    if (gGamestate == CREDITS_SEQUENCE) {
        return;
    }

    mtxf_pos_rotation_xyz(mtx, Pos, Rot);

    maxObjectsReached = render_set_position(mtx, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (temp < camera->pos[2]) {
        if (D_800DC5BC != 0) {

            gDPSetFogColor(gDisplayListHead++, D_801625EC, D_801625F4, D_801625F0, 0xFF);
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D001C20);
        } else {
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D001B90);
        }
    } else if (D_800DC5BC != 0) {
        gDPSetFogColor(gDisplayListHead++, D_801625EC, D_801625F4, D_801625F0, 0xFF);
        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D001C88);
    } else {
        gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D001BD8);
    }
}

void AFinishline::Collision(Player* player, AActor* actor) {
}

bool AFinishline::IsMod() {
    return true;
}
