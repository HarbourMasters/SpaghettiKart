#include <libultraship.h>

#include "ACoin.h"
#include "engine/Actor.h"
#include "World.h"

extern "C" {
#include "macros.h"
#include "common_structs.h"
#include "math_util.h"
#include "actor_types.h"
#include "actors.h"
#include "engine/courses/ACoinData.h"

}
ACoin::ACoin(const float pos[3]) {
    // Initialize the actor's position
    std::copy(pos, pos + 3, Pos);


    Flags = -0x8000 | 0x4000;

    BoundingBoxSize = 2.0f;
}

void ACoin::Tick() {
    Rot[1] += 90;

    if (PickedUp) {
        Timer++;
    }

    if (Timer > 500) {
        PickedUp = false;
        Timer = 0;
    }
}

void ACoin::Draw(Camera *camera) {
    Mat4 mtx;

    if (PickedUp) {
        return;
    }

    mtxf_pos_rotation_xyz(mtx, Pos, Rot);
    if (render_set_position(mtx, 0) != 0) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

        gSPDisplayList(gDisplayListHead++, (Gfx*)mario_Cylinder_mesh);
    }
}

void ACoin::Collision(Player* player, AActor* actor) {
    if (!PickedUp) {
        if (query_collision_player_vs_actor_item(player, gWorldInstance.ConvertAActorToActor(actor))) {
            PickedUp = true;
            player->effects |= BOOST_EFFECT;
        }
    }
}

bool ACoin::IsMod() { return true; }
