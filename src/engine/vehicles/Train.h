#pragma once

#include <libultraship.h>
#include <vector>
#include "RegisterContent.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"
#include "engine/World.h"

#include "Actor.h"

extern "C" {
#include "main.h"
#include "vehicles.h"
}

/**
 * Note that you can only remove the tender if there are no carriages
 * @arg waypoint initial waypoint to spawn at.
 */
class ATrain : public AActor {
    public:

    enum SpawnMode : uint16_t {
        POINT, // Spawn train at a specific path point
        AUTO,  // Automatically distribute trains based on a specific path point
    };

    enum TenderStatus {
        NO_TENDER,
        HAS_TENDER,
    };

    TrainCarStuff Locomotive;
    TrainCarStuff Tender;
    std::vector<TrainCarStuff> PassengerCars;
    size_t PassengerCarsCount = 0;
    ATrain::SpawnMode SpawnType = ATrain::SpawnMode::AUTO;
    uint32_t PathIndex = 0;
    uint32_t PathPoint = 0;
    TenderStatus HasTender = TenderStatus::NO_TENDER;
    s32 SomeFlags;
    f32 SomeMultiplier;

    size_t Index; // Spawns the train in halves of the train path

    int32_t SmokeParticles[128];
    int32_t NextParticlePtr = 0;
    int16_t AnotherSmokeTimer = 0;
    int16_t SmokeTimer = 0;

    explicit ATrain(const SpawnParams& params);

    ~ATrain() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    // This is simply a helper function to keep Spawning code clean
    static inline ATrain* Spawn(ATrain::TenderStatus tender, size_t numCarriages, f32 speed, uint32_t pathIndex, uint32_t pathPoint, ATrain::SpawnMode spawnMode) {
        SpawnParams params = {
            .Name = "mk:train",
            .Type = static_cast<int16_t>(spawnMode),
            .Count = numCarriages,
            .PathIndex = pathIndex,
            .PathPoint = pathPoint,
            .Bool = tender,
            .Speed = speed, // 120.0f is about the maximum usable value
        };
        return static_cast<ATrain*>(AddActorToWorld<ATrain>(params));
    }

    virtual void SetSpawnParams(SpawnParams& params);
    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual bool IsMod() override;
    s32 AddSmoke(s32 trainIndex, Vec3f pos, f32 velocity);
    void SyncComponents(TrainCarStuff* trainCar, s16 orientationY);
    virtual void DrawEditorProperties() override;

private:
    static size_t _count; // Total number of spawned trains
//                  pathIndex, array of spawn points
    static std::map<uint32_t, std::vector<uint32_t>> TrainCounts;
};
