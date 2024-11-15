#pragma once

#include <libultraship.h>
#include "Vehicle.h"
#include <vector>

extern "C" {
#include "main.h"
#include "vehicles.h"
}

class AVehicle; // Forward declare

class ATrain : public AVehicle {
    public:

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

    explicit ATrain(size_t idx, size_t numCarriages, f32 speed, uint32_t waypoint);

    virtual void Spawn() override;
    virtual void BeginPlay() override;
    virtual void Tick() override;
    virtual void Draw(s32 playerId) override;
    virtual void Collision(s32 playerId, Player* player) override;
    s32 AddSmoke(s32 trainIndex, Vec3f pos, f32 velocity);
    void SyncComponents(TrainCarStuff* trainCar, s16 orientationY);
};