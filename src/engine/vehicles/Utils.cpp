#include <libultraship.h>
#include "Utils.h"

extern "C" {
#include "macros.h"
#include "defines.h"
#include "code_80005FD0.h"
}

uint32_t CalculateWaypointDistribution(size_t i, uint32_t numVehicles, size_t numWaypoints, uint32_t centerWaypoint) {
    return (uint32_t)(((i * numWaypoints) / numVehicles) + centerWaypoint) % numWaypoints;
}

uint32_t GetVehiclePathPointDistributed(std::vector<uint32_t>& existingTrains, uint32_t numWaypoints) {
    if (existingTrains.empty()) {
        return 0; // first train at start
    }

    // Sort trains along path
    std::sort(existingTrains.begin(), existingTrains.end());

    if (existingTrains.size() == 1) {
        // Place train halfway around the path
        return (existingTrains[0] + numWaypoints / 2) % numWaypoints;
    }

    uint32_t bestGap = 0;
    uint32_t bestPos = 0;

    for (size_t i = 0; i < existingTrains.size(); i++) {
        uint32_t start = existingTrains[i];
        uint32_t end = existingTrains[(i + 1) % existingTrains.size()];
        uint32_t gap = (end + numWaypoints - start) % numWaypoints;

        if (gap > bestGap) {
            bestGap = gap;
            bestPos = (start + gap / 2) % numWaypoints;
        }
    }

    return bestPos;
}
