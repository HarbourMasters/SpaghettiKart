#pragma once

#include <libultraship.h>
#include "Actor.h"
#include <vector>

extern "C" {
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "sounds.h"
}

class ACar : public AActor {
  public:
    explicit ACar(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);

    ~ACar() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

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

    f32 SomeArg3 = 11.5f;
    f32 SomeArg4 = 8.5f;
    u32 SoundBits = SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual bool IsMod() override;

  private:
    static size_t _count;
};
