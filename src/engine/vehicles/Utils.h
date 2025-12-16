#pragma once

#include <libultraship.h>

uint32_t CalculateWaypointDistribution(size_t i, uint32_t numVehicles, size_t numWaypoints, uint32_t centerWaypoint);
uint32_t GetVehiclePathPointDistributed(std::vector<uint32_t>& existingTrains, uint32_t numWaypoints);
