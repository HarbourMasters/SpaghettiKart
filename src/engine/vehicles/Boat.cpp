#include <libultraship.h>
#include "Boat.h"
#include <vector>

extern "C" {
#include "macros.h"
#include "main.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "actors.h"
#include "math_util.h"
#include "sounds.h"
#include "update_objects.h"
extern s8 gPlayerCount;
}

size_t ABoat::_count = 0;

ABoat::ABoat(f32 speed, u32 waypoint) {
    Path2D* temp_a2;
    u16 waypointOffset;
    Index = _count;
    Speed = speed;

    // Set to the default value
    std::fill(SmokeParticles, SmokeParticles + 128, NULL_OBJECT_ID);

    waypointOffset = waypoint;
    temp_a2 = &gVehicle2DWaypoint[waypointOffset];
    Position[0] = temp_a2->x;
    Position[1] = D_80162EB2;
    Position[2] = temp_a2->z;
    WaypointIndex = Index * 0xB4;
    ActorIndex = -1;

    IsActive = 1;

    Velocity[0] = 0.0f;
    Velocity[1] = 0.0f;
    Velocity[2] = 0.0f;

    Vec3s paddleWheelBoatRot;
    if (IsActive == 1) {
        f32 origXPos = Position[0];
        f32 origZPos = Position[2];
        RotY = update_vehicle_following_waypoint(Position, (s16*) &WaypointIndex, Speed);
        Velocity[0] = Position[0] - origXPos;
        Velocity[2] = Position[2] - origZPos;
        vec3s_set(paddleWheelBoatRot, 0, RotY, 0);
        ActorIndex = add_actor_to_empty_slot(Position, paddleWheelBoatRot, Velocity, ACTOR_PADDLE_BOAT);
    }

    _count++;
}

void ABoat::Draw(Camera* camera) {
}

bool ABoat::IsMod() {
    return true;
}

void ABoat::Tick() {
    Path2D* waypoint;
    struct Actor* paddleBoatActor;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f30;
    s16 temp_a1;
    s32 temp;
    s16 var_v1;
    Vec3f sp94;
    Vec3f sp88;
    UNUSED s32 pad;
    Vec3f smokePos;
    UNUSED s32 pad2;
    AnotherSmokeTimer += 1;
    if (IsActive == 1) {
        temp_f26 = Position[0];
        temp_f28 = Position[1];
        temp_f30 = Position[2];
        update_vehicle_following_waypoint(Position, (s16*) &WaypointIndex, Speed);
        SomeFlags = set_vehicle_render_distance_flags(Position, BOAT_SMOKE_RENDER_DISTANCE, SomeFlags);
        if ((((s16) AnotherSmokeTimer % 10) == 0) && (SomeFlags != 0)) {
            smokePos[0] = (f32) ((f64) Position[0] - 30.0);
            smokePos[1] = (f32) ((f64) Position[1] + 180.0);
            smokePos[2] = (f32) ((f64) Position[2] + 45.0);
            adjust_position_by_angle(smokePos, Position, RotY);
            // spawn_ferry_smoke(Index, smokePos, 1.1f);
            AddSmoke(Index, smokePos, 1.1f);
            smokePos[0] = (f32) ((f64) Position[0] + 30.0);
            smokePos[1] = (f32) ((f64) Position[1] + 180.0);
            smokePos[2] = (f32) ((f64) Position[2] + 45.0);
            adjust_position_by_angle(smokePos, Position, RotY);
            // spawn_ferry_smoke(Index, smokePos, 1.1f);
            AddSmoke(Index, smokePos, 1.1f);
        }
        if (random_int(100) == 0) {
            if (random_int(2) == 0) {
                func_800C98B8(Position, Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x47));
            } else {
                func_800C98B8(Position, Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x48));
            }
        }
        sp94[0] = temp_f26;
        sp94[1] = temp_f28;
        sp94[2] = temp_f30;
        waypoint = &gVehicle2DWaypoint[(WaypointIndex + 5) % gVehicle2DWaypointLength];
        sp88[0] = (f32) waypoint->x;
        sp88[1] = (f32) D_80162EB0;
        sp88[2] = (f32) waypoint->z;
        temp_a1 = get_angle_between_waypoints(sp94, sp88);
        temp = temp_a1 - RotY;
        var_v1 = temp;
        if (var_v1 < 0) {
            var_v1 = -var_v1;
        }
        if (var_v1 >= 0x1771) {
            if (Speed > 0.2) {
                Speed -= 0.04;
            }
            if (var_v1 >= 0x3D) {
                var_v1 = 0x003C;
            }
        } else {
            if (Speed < 2.0) {
                Speed += 0.02;
            }
            if (var_v1 >= 0x1F) {
                var_v1 = 0x001E;
            }
        }
        if (temp >= 0x8000) {
            RotY -= var_v1;
        } else if (temp > 0) {
            RotY += var_v1;
        } else if (temp < -0x7FFF) {
            RotY += var_v1;
        } else if (temp < 0) {
            RotY -= var_v1;
        }
        Velocity[0] = Position[0] - temp_f26;
        Velocity[1] = Position[1] - temp_f28;
        Velocity[2] = Position[2] - temp_f30;
        paddleBoatActor = GET_ACTOR(ActorIndex);
        paddleBoatActor->pos[0] = Position[0];
        paddleBoatActor->pos[1] = Position[1];
        paddleBoatActor->pos[2] = Position[2];
        if (gIsMirrorMode != 0) {
            paddleBoatActor->rot[1] = -RotY;
        } else {
            paddleBoatActor->rot[1] = RotY;
        }
        Velocity[0] = Velocity[0];
        Velocity[1] = Velocity[1];
        Velocity[2] = Velocity[2];
    }
}

void ABoat::VehicleCollision(s32 playerId, Player* player) {
    f32 x_diff;
    f32 y_diff;
    f32 z_diff;
    f32 playerX;
    f32 playerZ;
    f32 playerY;

    if (!((player->effects & 0x01000000)) && (!(player->effects & HIT_BY_ITEM_EFFECT))) {
        playerX = player->pos[0];
        playerY = player->pos[1];
        playerZ = player->pos[2];
        if (IsActive == 1) {
            x_diff = playerX - Position[0];
            y_diff = playerY - Position[1];
            z_diff = playerZ - Position[2];
            if ((x_diff > -300.0) && (x_diff < 300.0)) {
                if ((z_diff > -300.0) && (z_diff < 300.0)) {
                    if ((func_80006018(Position[0], Position[2], Velocity[0], Velocity[2], 200.0f, 60.0f, playerX,
                                       playerZ) == 1) &&
                        (y_diff < 60.0)) {
                        player->soundEffects |= 0x80000;
                    }
                }
            }
        }
    }
}

s32 ABoat::AddSmoke(size_t ferryIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex = add_unused_obj_index(SmokeParticles, &NextParticlePtr, gObjectParticle2_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_ferry_smoke(objectIndex, pos, velocity);
    }

    return objectIndex;
}
