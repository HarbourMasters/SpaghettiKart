#include "Vec3sArray.h"

namespace SF64 {
Vec3sData* Vec3sArray::GetPointer() {
    return mData.data();
}

size_t Vec3sArray::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SF64
