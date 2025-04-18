#include "TrackSections.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
TrackSectionsClass::TrackSectionsClass() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackSectionsI* TrackSectionsClass::GetPointer() {
    return TrackSectionsList.data();
}

size_t TrackSectionsClass::GetPointerSize() {
    return TrackSectionsList.size() * sizeof(TrackSectionsI);
}

TrackSectionsO2RClass::TrackSectionsO2RClass() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackSectionsO2R* TrackSectionsO2RClass::GetPointer() {
    return TrackSectionsList.data();
}

size_t TrackSectionsO2RClass::GetPointerSize() {
    return TrackSectionsList.size() * sizeof(TrackSectionsO2R);
}
} // namespace MK64
