#pragma once

#include <libultraship.h>
#include "Actor.h"
#include <vector>
#include "RegisterContent.h"
#include "engine/SpawnParams.h"

extern "C" {
#include "sounds.h"
}

class ATruck : public AActor {
  public:
    enum SpawnMode : uint16_t {
        POINT, // Spawn car at a specific path point
        AUTO,  // Automatically distribute cars based on a specific path point
    };

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

    float SpeedB = 0.0f;
    ATruck::SpawnMode SpawnType = ATruck::SpawnMode::AUTO;
    uint32_t PathIndex = 0;
    uint32_t PathPoint = 0;

    // This is simply a helper function to keep Spawning code clean
    static ATruck* Spawn(f32 speedA, f32 speedB, uint32_t pathIndex, uint32_t pathPoint, ATruck::SpawnMode spawnMode) {
        SpawnParams params = {
            .Name = "mk:truck",
            .Type = static_cast<uint16_t>(spawnMode),
            .PathIndex = pathIndex,
            .PathPoint = pathPoint,
            .Speed = speedA,
            .SpeedB = speedB
        };
        return dynamic_cast<ATruck*>(AddActorToWorld<ATruck>(params));
    }

    explicit ATruck(const SpawnParams& params);

    ~ATruck() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual bool IsMod() override;
    virtual void DrawEditorProperties() override;

  private:
    static size_t _count;
    static std::map<uint32_t, std::vector<uint32_t>> TruckCounts;
};