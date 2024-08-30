#include "Limb.h"

namespace SF64 {
LimbData* Limb::GetPointer() {
    return &mData;
}

size_t Limb::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SF64
