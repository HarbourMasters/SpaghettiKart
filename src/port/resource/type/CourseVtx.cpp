#include "CourseVtx.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
CourseVtxClass::CourseVtxClass() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

CourseVtxData* CourseVtxClass::GetPointer() {
    return CourseVtxList.data();
}

size_t CourseVtxClass::GetPointerSize() {
    return CourseVtxList.size() * sizeof(CourseVtxData);
}
} // namespace MK64
