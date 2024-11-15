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
