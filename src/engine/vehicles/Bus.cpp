#include <libultraship.h>
#include "Bus.h"
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
#include "render_player.h"
#include "external.h"
extern s8 gPlayerCount;
}

size_t ABus::_count = 0;

ABus::ABus(f32 speedA, f32 speedB, TrackPathPoint* path, uint32_t waypoint) {
    Name = "Bus";
    TrackPathPoint* temp_v0;
    u16 waypointOffset;
    s32 numWaypoints = gPathCountByPathIndex[0];

    Index = _count;

    waypointOffset = waypoint;
    temp_v0 = &path[waypointOffset];
    Position[0] = (f32) temp_v0->posX;
    Position[1] = (f32) temp_v0->posY;
    Position[2] = (f32) temp_v0->posZ;
    ActorIndex = -1;
    WaypointIndex = waypointOffset;
    Velocity[0] = 0.0f;
    Velocity[1] = 0.0f;
    Velocity[2] = 0.0f;
    if (gModeSelection == TIME_TRIALS) {
        SomeType = ((Index - 1) % 3);
    } else {
        SomeType = random_int(3);
    }
    SomeMultiplierTheSequel = (f32) ((f64) (f32) (SomeType - 1) * 0.6);
    if (((gCCSelection > CC_50) || (gModeSelection == TIME_TRIALS)) && (SomeType == 2)) {
        Speed = speedA;
    } else {
        Speed = speedB;
    }
    Rotation[0] = 0;
    Rotation[2] = 0;
    if (gIsInExtra == 0) {
        Rotation[1] = func_8000D6D0(Position, (s16*) &WaypointIndex, Speed, SomeMultiplierTheSequel, 0, 3);
    } else {
        Rotation[1] = func_8000D940(Position, (s16*) &WaypointIndex, Speed, SomeMultiplierTheSequel, 0);
    }
    sVehicleSoundRenderCounter = 10;

    spawn_vehicle_on_road(Position, Rotation, Velocity, WaypointIndex, SomeMultiplierTheSequel, Speed);
    ActorIndex = add_actor_to_empty_slot(Position, Rotation, Velocity, ACTOR_SCHOOL_BUS);

    _count++;
}

bool ABus::IsMod() {
    return true;
}

void ABus::Draw(Camera* camera) {
    s32 var_v0;
    s32 var_s2;
    s32 waypointCount;
    u16 temp_a1;

    waypointCount = gPathCountByPathIndex[0];
    if (!(gPlayers[camera->playerId].speed < 1.6666666666666667)) {
        temp_a1 = WaypointIndex;
        for (var_v0 = 0; var_v0 < 0x18; var_v0 += 3) {
            if (((sSomeNearestPathPoint + var_v0) % waypointCount) == temp_a1) {
                gPlayerTrackPositionFactorInstruction[camera->playerId].target =
                    player_track_position_factor_vehicle(SomeType, gTrackPositionFactor[camera->playerId], temp_a1);
                return;
            }
        }
    }
}

void ABus::Tick() {
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 temp_f2_2;
    s16 var_a1;
    s16 thing;
    Vec3f sp40;
    Vec3f sp34;

    sp5C = Position[0];
    sp58 = Position[1];
    sp54 = Position[2];
    sp40[0] = sp58;
    sp40[1] = 0.0f;
    sp40[2] = 0.0f;
    temp_f0_2 = func_80013C74(SomeType, WaypointIndex);
    if (SomeMultiplierTheSequel < temp_f0_2) {
        SomeMultiplierTheSequel = SomeMultiplierTheSequel + 0.06;
        if (temp_f0_2 < SomeMultiplierTheSequel) {
            SomeMultiplierTheSequel = temp_f0_2;
        }
    }
    if (temp_f0_2 < SomeMultiplierTheSequel) {
        SomeMultiplierTheSequel = SomeMultiplierTheSequel - 0.06;
        if (SomeMultiplierTheSequel < temp_f0_2) {
            SomeMultiplierTheSequel = temp_f0_2;
        }
    }
    if (gIsInExtra == 0) {
        var_a1 = func_8000D6D0(Position, (s16*) &WaypointIndex, Speed, SomeMultiplierTheSequel, 0, 3);
    } else {
        var_a1 = func_8000D940(Position, (s16*) &WaypointIndex, Speed, SomeMultiplierTheSequel, 0);
    }
    adjust_angle(&Rotation[1], var_a1, 100);
    temp_f0_3 = Position[0] - sp5C;
    temp_f2_2 = Position[2] - sp54;
    sp34[0] = Position[1];
    sp34[1] = 0.0f;
    sp34[2] = sqrtf((temp_f0_3 * temp_f0_3) + (temp_f2_2 * temp_f2_2));
    thing = get_angle_between_two_vectors(sp40, sp34);
    adjust_angle(&Rotation[0], -thing, 100);
    Velocity[0] = Position[0] - sp5C;
    Velocity[1] = Position[1] - sp58;
    Velocity[2] = Position[2] - sp54;
    struct Actor* vehicleActor = GET_ACTOR(ActorIndex);
    vehicleActor->pos[0] = Position[0];
    vehicleActor->pos[1] = Position[1];
    vehicleActor->pos[2] = Position[2];
    vehicleActor->rot[0] = Rotation[0];
    if (gIsMirrorMode != 0) {
        vehicleActor->rot[1] = -Rotation[1];
    } else {
        vehicleActor->rot[1] = Rotation[1];
    }
    vehicleActor->rot[2] = Rotation[2];
    vehicleActor->velocity[0] = Velocity[0];
    vehicleActor->velocity[1] = Velocity[1];
    vehicleActor->velocity[2] = Velocity[2];
}

void ABus::VehicleCollision(s32 playerId, Player* player) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f22;

    f32 spC4;
    f32 spC0;
    f32 spBC;

    if (((D_801631E0[playerId] != 1) || ((((player->type & PLAYER_HUMAN) != 0)) && !(player->type & PLAYER_CPU))) &&
        !(player->effects & 0x01000000)) {

        spC4 = player->pos[0];
        spC0 = player->pos[1];
        spBC = player->pos[2];

        temp_f12 = spC4 - Position[0];
        temp_f22 = spC0 - Position[1];
        temp_f14 = spBC - Position[2];

        if (((temp_f12) > -100.0) && ((temp_f12) < 100.0)) {
            if ((temp_f22 > -20.0) && (temp_f22 < 20.0)) {

                if (((temp_f14) > -100.0) && ((temp_f14) < 100.0)) {
                    if (is_collide_with_vehicle(Position[0], Position[2], Velocity[0], Velocity[2], SomeArg3, SomeArg4,
                                                spC4, spBC) == (s32) 1) {
                        player->soundEffects |= REVERSE_SOUND_EFFECT;
                    }
                }
            }
        }
        if ((player->type & PLAYER_HUMAN) && !(player->type & PLAYER_CPU)) {
            if (((temp_f12) > -300.0) && ((temp_f12) < 300.0) && ((temp_f22 > -20.0)) && (temp_f22 < 20.0) &&
                (((temp_f14) > -300.0)) && ((temp_f14) < 300.0)) {
                if ((sVehicleSoundRenderCounter > 0) && (SomeFlags == 0)) {
                    sVehicleSoundRenderCounter -= 1;
                    SomeFlags |= (RENDER_VEHICLE << playerId);
                    func_800C9D80(Position, Velocity, SoundBits);
                }
            } else {
                if (SomeFlags != 0) {
                    SomeFlags &= ~(RENDER_VEHICLE << playerId);
                    if (SomeFlags == 0) {
                        sVehicleSoundRenderCounter += 1;
                        func_800C9EF4(Position, SoundBits);
                    }
                }
            }

            if (((temp_f12) > -200.0) && ((temp_f12) < 200.0) && ((temp_f22 > -20.0)) && (temp_f22 < 20.0) &&
                (((temp_f14) > -200.0)) && ((temp_f14) < 200.0)) {
                if (!(SomeFlagsTheSequel & ((1 << playerId)))) {

                    s32 var_s1 = 0;
                    u16 path = gPathCountByPathIndex[0];
                    s32 t1;
                    s32 t2;

                    switch (gIsInExtra) {
                        case 0:
                            t1 = is_path_point_in_range(WaypointIndex, gNearestPathPointByPlayerId[playerId], 10, 0,
                                                        path);
                            if ((gIsPlayerWrongDirection[playerId] == 0) && (t1 > 0) && (player->speed < Speed)) {
                                var_s1 = 1;
                            }
                            if ((gIsPlayerWrongDirection[playerId] == 1) && (t1 > 0)) {
                                var_s1 = 1;
                            }
                            break;
                        case 1:
                            t2 = is_path_point_in_range(WaypointIndex, gNearestPathPointByPlayerId[playerId], 0, 10,
                                                        path);
                            if (t2 > 0) {
                                if (random_int(2) == 0) {
                                    // temp_v1_2 = gIsPlayerWrongDirection[playerId];
                                    if (gIsPlayerWrongDirection[playerId] == 0) {
                                        var_s1 = 1;
                                    }
                                    if ((gIsPlayerWrongDirection[playerId] == 1) && (player->speed < Speed)) {
                                        var_s1 = 1;
                                    }
                                } else {
                                    SomeFlagsTheSequel |= ((1 << playerId));
                                }
                            }
                            break;
                    }
                    if (var_s1 == 1) {

                        u32 soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);

                        switch (SoundBits) {
                            case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05):
                                soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);
                                if (random_int(4) == 0) {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3C);
                                }
                                break;
                            case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02):
                                if (random_int(2) != 0) {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3D);
                                } else {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3E);
                                }
                                break;
                            case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03):
                                if (random_int(2) != 0) {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3F);
                                } else {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x40);
                                }
                                break;
                            case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04):
                                if (random_int(2) != 0) {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x41);
                                } else {
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x42);
                                }
                                break;
                        }
                        SomeFlagsTheSequel |= ((1 << playerId));
                        func_800C98B8(Position, Velocity, soundBits2);
                    }
                }
            } else {
                if (SomeFlagsTheSequel & ((1 << playerId))) {
                    SomeFlagsTheSequel &= ~((1 << playerId));
                }
            }
        }
    }
}
