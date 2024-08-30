#include "Skeleton.h"

namespace SF64 {
LimbData** Skeleton::GetPointer() {
    return mLimbs.data();
}

size_t Skeleton::GetPointerSize() {
    return mLimbs.size() * sizeof(LimbData*);
}
} // namespace SF64
