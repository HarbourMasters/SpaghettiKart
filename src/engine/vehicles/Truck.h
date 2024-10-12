#pragma once

#include <libultraship.h>
#include "Vehicle.h"
#include <vector>

extern "C" {
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "sounds.h"
}

class AVehicle; // Forward declare

class ATruck : public AVehicle {
    public:

    const char* Type;
    size_t Index;
    f32 Speed;
    s16 ActorIndex;
    Vec3f Position;
    Vec3f Velocity;
    Vec3s Rotation;
    f32 SomeMultiplierTheSequel;
    s8 SomeFlagsTheSequel = 0;
    u16 WaypointIndex;
    s8 SomeFlags = 0;
    s16 SomeType;

    f32 SomeArg3 = 55.0f;
    f32 SomeArg4 = 12.5f;
    u32 SoundBits = SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03);

    explicit ATruck(size_t idx, f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);

    virtual void Spawn() override;
    virtual void BeginPlay() override;
    virtual void Tick() override;
    virtual void Draw(s32 playerId) override;
    virtual void Collision(s32 playerId, Player* player) override;
};