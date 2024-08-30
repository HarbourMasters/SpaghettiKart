#include "Vec3fArray.h"

namespace SF64 {
Vec3fData* Vec3fArray::GetPointer() {
    return mData.data();
}

size_t Vec3fArray::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SF64
