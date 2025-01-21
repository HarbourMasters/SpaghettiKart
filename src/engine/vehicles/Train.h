#pragma once

#include <libultraship.h>
#include "Actor.h"
#include <vector>

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

    enum TenderStatus {
        NO_TENDER,
        HAS_TENDER,
    };

    TrainCarStuff Locomotive;
    TrainCarStuff Tender;
    std::vector<TrainCarStuff> PassengerCars;
    f32 Speed; // 120.0f is about the maximum usable value
    s32 SomeFlags;
    f32 SomeMultiplier;
    size_t NumCars; // Non-locomotive car count?

    const char* Type = "mk:train";
    size_t Index; // Spawns the train in halves of the train path

    int32_t SmokeParticles[128];
    int32_t NextParticlePtr = 0;
    int16_t AnotherSmokeTimer = 0;
    int16_t SmokeTimer = 0;

    explicit ATrain(ATrain::TenderStatus tender, size_t numCarriages, f32 speed, uint32_t waypoint);

    ~ATrain() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void VehicleCollision(s32 playerId, Player* player) override;
    virtual bool IsMod() override;
    s32 AddSmoke(s32 trainIndex, Vec3f pos, f32 velocity);
    void SyncComponents(TrainCarStuff* trainCar, s16 orientationY);

private:
    static size_t _count;
};