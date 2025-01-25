#pragma once

#include <libultraship.h>
#include "Actor.h"
#include <vector>

extern "C" {
#include "main.h"
#include "vehicles.h"
}

class ABoat : public AActor {
  public:
    const char* Type = "mk:boat";
    size_t Index;
    bool IsActive; // The paddle wheel boat only shows up if the number of players is < 3
    Vec3f Position;
    Vec3f Velocity;
    u16 WaypointIndex;
    s16 ActorIndex;
    f32 Speed;
    s16 RotY = 0;
    s32 SomeFlags;

    int32_t SmokeParticles[128];
    int32_t NextParticlePtr = 0;
    int16_t AnotherSmokeTimer = 0;
    int16_t SmokeTimer = 0;

    explicit ABoat(f32 speed, uint32_t waypoint);

    ~ABoat() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual s32 AddSmoke(size_t, Vec3f, f32);
    virtual bool IsMod() override;

  private:
    static size_t _count;
};
