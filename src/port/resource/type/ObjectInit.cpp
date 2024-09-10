#include "ObjectInit.h"

namespace SF64 {
ObjectInitData* ObjectInit::GetPointer() {
    return mObjects.data();
}

size_t ObjectInit::GetPointerSize() {
    return sizeof(ObjectInitData) * mObjects.size();
}
} // namespace SF64
