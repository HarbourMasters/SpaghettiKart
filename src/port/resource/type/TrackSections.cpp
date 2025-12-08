#include "TrackSections.h"
#include "libultraship/libultra/gbi.h"
#include "src/engine/tracks/Track.h"

namespace MK64 {
TrackSectionsClass::TrackSectionsClass() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackSections* TrackSectionsClass::GetPointer() {
    return TrackSectionsList.data();
}

size_t TrackSectionsClass::GetPointerSize() {
    return TrackSectionsList.size() * sizeof(TrackSections);
}

TrackSectionsO2RClass::TrackSectionsO2RClass() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackSections* TrackSectionsO2RClass::GetPointer() {
    return TrackSectionsList.data();
}

size_t TrackSectionsO2RClass::GetPointerSize() {
    return TrackSectionsList.size() * sizeof(TrackSections);
}
} // namespace MK64
