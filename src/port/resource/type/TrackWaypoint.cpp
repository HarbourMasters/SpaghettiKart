#include "TrackWaypoint.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
TrackWaypoints::TrackWaypoints() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackWaypoint* TrackWaypoints::GetPointer() {
    return TrackWaypointList.data();
}

size_t TrackWaypoints::GetPointerSize() {
    return TrackWaypointList.size() * sizeof(TrackWaypoint);
}
} // namespace MK64
