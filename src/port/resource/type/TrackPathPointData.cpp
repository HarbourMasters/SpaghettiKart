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
    size_t totalSize = 0;
    // Iterate over each PathObject in the PathObject vector
    for (const auto& obj : PathObject) {
        // Add the size of the vector itself (this is the overhead of the vector container)
        totalSize += sizeof(obj.PathList);

        // Add the size of the elements in the PathList vector
        totalSize += obj.PathList.size() * sizeof(TrackPathPoint);

        // Add the size of the PathIndex (int32_t)
        totalSize += sizeof(obj.PathIndex);
    }
    return totalSize;
}

} // namespace MK64