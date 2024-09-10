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
} // namespace MK64
