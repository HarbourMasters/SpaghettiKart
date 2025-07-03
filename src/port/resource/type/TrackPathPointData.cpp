#include "TrackPathPointData.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
TrackPathPointData::TrackPathPointData() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackPathPoint* TrackPathPointData::GetPointer() {
    return TrackPathPointList.data();
}

size_t TrackPathPointData::GetPointerSize() {
    return TrackPathPointList.size() * sizeof(TrackPathPoint);
}


Paths::Paths() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

// I don't know how to return this properly
TrackPathPoint* Paths::GetPointer() {
    printf("Do not LOAD_ASSET an XML track waypoint/path, you need to use dynamic_cast<MK64::Paths>(ResourceLoad())");
    return nullptr;
}

size_t Paths::GetPointerSize() {
    size_t totalWaypoints = 0;
    for (const auto& path : PathList) {
        totalWaypoints += path.size();
    }
    return totalWaypoints * sizeof(TrackPathPoint);
}

} // namespace MK64