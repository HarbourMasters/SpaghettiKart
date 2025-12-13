#include <libultraship.h>
#include "TrainCrossing.h"
#include "World.h"
#include "engine/Actor.h"
#include "vehicles/Train.h"
#include <memory>

extern "C" {
#include "macros.h"
#include "main.h"
#include "vehicles.h"
#include "code_80005FD0.h"
#include "code_80057C60.h"
#include "common_structs.h"
}

TrainCrossing::TrainCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius) {
    Position[0] = position[0] * xOrientation;
    Position[1] = position[1];
    Position[2] = position[2];
    ApproachRadius = approachRadius;
    ExitRadius = exitRadius;
    WaypointMin = waypointMin;
    WaypointMax = waypointMax;
}

void TrainCrossing::CrossingTrigger() {
    f32 waypoint;
    s32 i;
    OnTriggered = 0;

    for (const auto& actor : GetWorld()->Actors) {
        if (auto* train = dynamic_cast<ATrain*>(actor.get())) {
            f32 radius = DynamicRadius(train->Locomotive.position, train->Locomotive.velocity, Position);

            if (Distance(train->Locomotive.position, Position) < radius) {
                OnTriggered = true; // Trigger within range
            }
        }
    }

    if (OnTriggered) {
        Timer += 1;
    } else {
        Timer = 0;
    }
}

void TrainCrossing::AICrossingBehaviour(s32 playerId) {
    bStopAICrossing[playerId] = 0;

    if ((!(D_801631E0[playerId] != 0)) ||
        (set_vehicle_render_distance_flags(gPlayers[playerId].pos, TRAIN_CROSSING_AI_DISTANCE, 0))) {
        if ((OnTriggered == 1) && ((Timer) > FRAMES_SINCE_CROSSING_ACTIVATED)) {
            if ((sSomeNearestPathPoint > WaypointMin) && (sSomeNearestPathPoint < WaypointMax)) {
                bStopAICrossing[playerId] = 1;
            }
        }
    }
}

f32 TrainCrossing::Distance(Vec3f a, Vec3f b) {
    float dx = b[0] - a[0];                   // Difference in x-coordinates
    float dy = b[1] - a[1];                   // Difference in y-coordinates
    float dz = b[2] - a[2];                   // Difference in z-coordinates
    return sqrt(dx * dx + dy * dy + dz * dz); // Return the distance
}

f32 TrainCrossing::DynamicRadius(Vec3f trainPos, Vec3f trainVelocity, Vec3f crossingPos) {
    // Calculate direction of train (this assumes velocity is a normalized direction vector)
    Vec3f trainToCrossing;
    trainToCrossing[0] = crossingPos[0] - trainPos[0];
    trainToCrossing[1] = crossingPos[1] - trainPos[1];
    trainToCrossing[2] = crossingPos[2] - trainPos[2];

    // Dot product to check if the train is approaching or moving away from the crossing
    f32 dotProduct = trainToCrossing[0] * trainVelocity[0] + trainToCrossing[1] * trainVelocity[1] +
                     trainToCrossing[2] * trainVelocity[2];

    if (dotProduct > 0) {
        // Train is approaching the crossing
        return ApproachRadius;
    } else {
        // Train has passed the crossing
        return ExitRadius;
    }
}
