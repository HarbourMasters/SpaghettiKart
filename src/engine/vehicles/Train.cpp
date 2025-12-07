
#include <libultraship.h>
#include <libultra/gbi.h>
#include "Train.h"
#include <vector>

#include "engine/courses/Course.h"
#include "engine/vehicles/Utils.h"
#include "engine/World.h"
#include "port/Game.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "vehicles.h"
#include "actors.h"
#include "math_util.h"
#include "sounds.h"
#include "update_objects.h"
#include "waypoints.h"
#include "code_80057C60.h"
#include "math_util_2.h"
#include "render_objects.h"
#include "assets/models/common_data.h"
//  #include "common_structs.h"
}

// The two counts are so we can spawn trains at specific points or use auto distribution
size_t ATrain::_count = 0;
//       pathIndex,     array of spawn points
std::map<uint32_t, std::vector<uint32_t>> ATrain::TrainCounts;

ATrain::ATrain(const SpawnParams& params) : AActor(params) {
    Name = "Train";
    ResourceName = "mk:train";
    BoundingBoxSize = 2.0f;
    TrainCarStuff* ptr1;
    TrackPathPoint* pos;

    Index = _count;

    PassengerCarsCount = params.Count.value_or(0);
    bool tender = params.Bool.value_or(true);

    // The path to spawn the train at
    uint32_t pathIndex = params.PathIndex.value_or(0);
    // The point along the path to spawn the train at
    uint32_t pathPoint = 0;

    SpawnType = static_cast<SpawnMode>(params.Type.value_or(SpawnMode::POINT));

    switch(SpawnType) {
        case SpawnMode::POINT: // Spawn train at a specific path point
            pathPoint = params.PathPoint.value_or(0);
            TrainCounts[pathIndex].push_back(pathPoint);
            break;
        case SpawnMode::AUTO: // Automatically distribute trains based on a specific path point
            printf("vehicle path size %d\n", gVehiclePathSize);
            pathPoint = GetVehiclePathPointDistributed(TrainCounts[pathIndex], gVehiclePathSize);
            TrainCounts[pathIndex].push_back(pathPoint);
            printf("train spawn path point: %d\n", pathPoint);
            break;
    }

    // Set to the default value
    std::fill(SmokeParticles, SmokeParticles + 128, NULL_OBJECT_ID);

    for (size_t i = 0; i < PassengerCarsCount; i++) {
        PassengerCars.push_back(TrainCarStuff());
    }

    // 120.0f is about the maximum usable value
    for (size_t i = 0; i < PassengerCars.size(); i++) {
        pathPoint += 4;
        ptr1 = &PassengerCars[i];
        pos = &gVehicle2DPathPoint[pathPoint];
        set_vehicle_pos_path_point(ptr1, pos, pathPoint);
    }
    // Smaller offset for the tender
    pathPoint += 3;
    pos = &gVehicle2DPathPoint[pathPoint];
    set_vehicle_pos_path_point(&this->Tender, pos, pathPoint);
    pathPoint += 4;
    pos = &gVehicle2DPathPoint[pathPoint];
    set_vehicle_pos_path_point(&Locomotive, pos, pathPoint);

    // Only use locomotive unless overwritten below.
    // Fall back in-case someone tries to spawn a train with carriages but no tender; not allowed.
    if (PassengerCarsCount > 0) {
        tender = HAS_TENDER;
    }

    Tender.isActive = static_cast<bool>(tender);

    for (size_t i = 0; i < PassengerCarsCount; i++) {
        PassengerCars[i].isActive = 1;
    }

    AnotherSmokeTimer = 0;

    TrainCarStuff* tempLocomotive;
    TrainCarStuff* tempTender;
    TrainCarStuff* tempPassengerCar;
    Vec3s trainCarRot;
    s16 trainCarYRot;
    f32 origXPos;
    f32 origZPos;

    tempLocomotive = &Locomotive;
    origXPos = tempLocomotive->position[0];
    origZPos = tempLocomotive->position[2];
    trainCarYRot =
        update_vehicle_following_path(tempLocomotive->position, (s16*) &tempLocomotive->waypointIndex, Speed);
    tempLocomotive->velocity[0] = tempLocomotive->position[0] - origXPos;
    tempLocomotive->velocity[2] = tempLocomotive->position[2] - origZPos;
    vec3s_set(trainCarRot, 0, trainCarYRot, 0);
    tempLocomotive->actorIndex =
        add_actor_to_empty_slot(tempLocomotive->position, trainCarRot, tempLocomotive->velocity, ACTOR_TRAIN_ENGINE);

    tempTender = &Tender;
    if (tempTender->isActive == 1) {
        origXPos = tempTender->position[0];
        origZPos = tempTender->position[2];
        trainCarYRot = update_vehicle_following_path(tempTender->position, (s16*) &tempTender->waypointIndex, Speed);
        tempTender->velocity[0] = tempTender->position[0] - origXPos;
        tempTender->velocity[2] = tempTender->position[2] - origZPos;
        vec3s_set(trainCarRot, 0, trainCarYRot, 0);
        tempTender->actorIndex =
            add_actor_to_empty_slot(tempTender->position, trainCarRot, tempTender->velocity, ACTOR_TRAIN_TENDER);
    }

    for (size_t i = 0; i < PassengerCars.size(); i++) {
        tempPassengerCar = &PassengerCars[i];
        if (tempPassengerCar->isActive == 1) {
            origXPos = tempPassengerCar->position[0];
            origZPos = tempPassengerCar->position[2];
            trainCarYRot = update_vehicle_following_path(tempPassengerCar->position,
                                                         (s16*) &tempPassengerCar->waypointIndex, Speed);
            tempPassengerCar->velocity[0] = tempPassengerCar->position[0] - origXPos;
            tempPassengerCar->velocity[2] = tempPassengerCar->position[2] - origZPos;
            vec3s_set(trainCarRot, 0, trainCarYRot, 0);
            tempPassengerCar->actorIndex = add_actor_to_empty_slot(
                tempPassengerCar->position, trainCarRot, tempPassengerCar->velocity, ACTOR_TRAIN_PASSENGER_CAR);
        }
    }

    _count++;
}

void ATrain::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = "mk:train";
    params.Type = static_cast<uint16_t>(SpawnType);
    params.Bool = Tender.isActive;
    params.Speed = Speed;
    params.Count = PassengerCarsCount;
    params.PathIndex = PathIndex;
    params.PathPoint = PathPoint;
}

bool ATrain::IsMod() {
    return true;
}

void ATrain::SyncComponents(TrainCarStuff* trainCar, s16 orientationY) {
    struct TrainCar* trainCarActor;

    //! @todo: Change actorIndex to ptr to TrainCar actor
    trainCarActor = (struct TrainCar*) GET_ACTOR(trainCar->actorIndex);
    trainCarActor->pos[0] = trainCar->position[0];
    trainCarActor->pos[1] = trainCar->position[1];
    trainCarActor->pos[2] = trainCar->position[2];
    if (gIsMirrorMode != 0) {
        trainCarActor->rot[1] = -orientationY;
    } else {
        trainCarActor->rot[1] = orientationY;
    }
    trainCarActor->velocity[0] = trainCar->velocity[0];
    trainCarActor->velocity[1] = trainCar->velocity[1];
    trainCarActor->velocity[2] = trainCar->velocity[2];
}

void ATrain::Tick() {
    TrainCarStuff* car;
    u16 oldWaypointIndex;
    s16 orientationYUpdate;
    s32 j;
    Vec3f smokePos;
    FVector temp_f20 = {
        Locomotive.position[0],
        Locomotive.position[1],
        Locomotive.position[2]
    };

    AnotherSmokeTimer += 1;

    oldWaypointIndex = (u16) Locomotive.waypointIndex;

    orientationYUpdate = update_vehicle_following_path(Locomotive.position, (s16*) &Locomotive.waypointIndex, Speed);

    Locomotive.velocity[0] = Locomotive.position[0] - temp_f20.x;
    Locomotive.velocity[1] = Locomotive.position[1] - temp_f20.y;
    Locomotive.velocity[2] = Locomotive.position[2] - temp_f20.z;

    sync_train_components(&Locomotive, orientationYUpdate);

    if ((oldWaypointIndex != Locomotive.waypointIndex) &&
        ((Locomotive.waypointIndex == 0x00BE) || (Locomotive.waypointIndex == 0x0140))) { // play crossing bell sound
        func_800C98B8(Locomotive.position, Locomotive.velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
    } else if (random_int(100) == 0) { // play train whistle sound
        func_800C98B8(Locomotive.position, Locomotive.velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
    }

    SomeFlags = set_vehicle_render_distance_flags(Locomotive.position, TRAIN_SMOKE_RENDER_DISTANCE, SomeFlags);
    // Renders locomotive smoke on all screens if any player is within range.
    if ((((s16) AnotherSmokeTimer % 5) == 0) && (SomeFlags != 0)) {
        smokePos[0] = Locomotive.position[0];
        smokePos[1] = (f32) ((f64) Locomotive.position[1] + 65.0);
        smokePos[2] = (f32) ((f64) Locomotive.position[2] + 25.0);
        adjust_position_by_angle(smokePos, Locomotive.position, orientationYUpdate);
        // spawn_train_smoke(Index, smokePos, 1.1f);
        AddSmoke(Index, smokePos, 1.1f);
    }

    car = &Tender;

    if (car->isActive == 1) {
        temp_f20.x = car->position[0];
        temp_f20.y = car->position[1];
        temp_f20.z = car->position[2];
        orientationYUpdate = update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, Speed);
        car->velocity[0] = car->position[0] - temp_f20.x;
        car->velocity[1] = car->position[1] - temp_f20.y;
        car->velocity[2] = car->position[2] - temp_f20.z;
        sync_train_components(car, orientationYUpdate);
    }

    for (j = 0; j < PassengerCars.size(); j++) {
        car = &PassengerCars[j];
        if (car->isActive == 1) {
            temp_f20.x = car->position[0];
            temp_f20.y = car->position[1];
            temp_f20.z = car->position[2];

            orientationYUpdate = update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, Speed);
            car->velocity[0] = car->position[0] - temp_f20.x;
            car->velocity[1] = car->position[1] - temp_f20.y;
            car->velocity[2] = car->position[2] - temp_f20.z;
            sync_train_components(car, orientationYUpdate);
        }
    }
}

void ATrain::VehicleCollision(s32 playerId, Player* player) {
    TrainCarStuff* trainCar;
    f32 playerPosX;
    f32 playerPosZ;
    f32 x_dist;
    f32 z_dist;
    s32 trainIndex;

    if (D_801631E0[playerId] != 1) {
        if (!(player->effects & 0x01000000)) {
            playerPosX = player->pos[0];
            playerPosZ = player->pos[2];
            trainCar = &Locomotive;
            x_dist = playerPosX - trainCar->position[0];
            z_dist = playerPosZ - trainCar->position[2];
            if ((x_dist > -100.0) && (x_dist < 100.0)) {
                if ((z_dist > -100.0) && (z_dist < 100.0)) {
                    if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                trainCar->velocity[2], 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                        player->soundEffects |= REVERSE_SOUND_EFFECT;
                    }
                    trainCar = &Tender;
                    if (trainCar->isActive == 1) {
                        if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                    trainCar->velocity[2], 30.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            player->soundEffects |= REVERSE_SOUND_EFFECT;
                        }
                    }
                }
            }

            for (size_t i = 0; i < PassengerCars.size(); i++) {
                trainCar = &PassengerCars[i];
                x_dist = playerPosX - trainCar->position[0];
                z_dist = playerPosZ - trainCar->position[2];
                if (trainCar->isActive == 1) {
                    if ((x_dist > -100.0) && (x_dist < 100.0)) {
                        if ((z_dist > -100.0) && (z_dist < 100.0)) {
                            if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2],
                                                        trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                        playerPosX, playerPosZ) == 1) {
                                player->soundEffects |= REVERSE_SOUND_EFFECT;
                            }
                        }
                    }
                }
            }
        }
    }
}

void ATrain::Draw(Camera* camera) {
}

s32 ATrain::AddSmoke(s32 trainIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex = add_unused_obj_index(SmokeParticles, &NextParticlePtr, gObjectParticle2_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_train_smoke(objectIndex, pos, velocity);
    }
    return objectIndex;
}

void ATrain::DrawEditorProperties() {
    ImGui::Text("Passenger Cars");
    ImGui::SameLine();

    int count = static_cast<int>(PassengerCarsCount);
    if (ImGui::InputInt("##Count", &count)) {
        // Clamp to uint32_t range (only lower bound needed if assuming positive values)
        if (count < 0) count = 0;
        PassengerCarsCount = static_cast<uint32_t>(count);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetCount")) {
        PassengerCarsCount = 0;
    }

    ImGui::Text("Spawn Mode");
    ImGui::SameLine();

    int32_t type = static_cast<int32_t>(SpawnType);
    const char* items[] = { "POINT", "AUTO" };

    if (ImGui::Combo("##Type", &type, items, IM_ARRAYSIZE(items))) {
        SpawnType = static_cast<ATrain::SpawnMode>(type);
    }

    if (type == ATrain::SpawnMode::POINT) {
        ImGui::Text("Path Index");
        ImGui::SameLine();

        int pathIndex = static_cast<int>(PathIndex);
        if (ImGui::InputInt("##PathIndex", &pathIndex)) {
            if (pathIndex < 0) pathIndex = 0;
            PathIndex = static_cast<uint32_t>(pathIndex);
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetPathIndex")) {
            PathIndex = 0;
        }

        ImGui::Text("Path Point");
        ImGui::SameLine();

        int pathPoint = static_cast<int>(PathPoint);
        if (ImGui::InputInt("##PathPoint", &pathPoint)) {
            if (pathPoint < 0) pathPoint = 0;
            PathPoint = static_cast<uint32_t>(pathPoint);
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_UNDO "##ResetPathPoint")) {
            PathPoint = 0;
        }
    }

    ImGui::Text("Has Tender");
    ImGui::SameLine();

    bool theBool = HasTender;
    if (ImGui::Checkbox("##Bool", &theBool)) {
        HasTender = static_cast<TenderStatus>(theBool);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetBool")) {
        HasTender = TenderStatus::NO_TENDER;
    }

    ImGui::Text("Speed");
    ImGui::SameLine();

    float speed = Speed;
    if (ImGui::DragFloat("##Speed", &speed, 0.1f)) {
        Speed = speed;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_UNDO "##ResetSpeed")) {
        Speed = 0.0f;
    }
}
