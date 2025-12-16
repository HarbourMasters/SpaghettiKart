#pragma once

#include <libultraship.h>
#include "Actor.h"
#include <vector>
#include "RegisterContent.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"
#include "engine/World.h"

extern "C" {
#include "main.h"
#include "vehicles.h"
}

class ABoat : public AActor {
    public:

    enum SpawnMode : uint16_t {
        POINT, // Spawn boat at a specific path point
        AUTO,  // Automatically distribute boats based on a specific path point
    };

    const char* Type = "mk:paddle_boat";
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

    explicit ABoat(const SpawnParams& params);

    ~ABoat() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    // This is simply a helper function to keep Spawning code clean
    static inline ABoat* Spawn(f32 speed, uint32_t pathIndex, uint32_t pathPoint, ABoat::SpawnMode spawnMode) {
        SpawnParams params = {
            .Name = "mk:paddle_boat",
            .Type = static_cast<uint16_t>(spawnMode),
            .PathIndex = pathIndex,
            .PathPoint = pathPoint,
            .Speed = speed,
        };
        return static_cast<ABoat*>(AddActorToWorld<ABoat>(params));
    }

    ABoat::SpawnMode SpawnType = ABoat::SpawnMode::AUTO;
    uint32_t PathIndex = 0;
    uint32_t PathPoint = 0;

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual s32 AddSmoke(size_t, Vec3f, f32);
    virtual bool IsMod() override;
    virtual void DrawEditorProperties() override;
private:
    static size_t _count;
    static std::map<uint32_t, std::vector<uint32_t>> BoatCounts;
};
