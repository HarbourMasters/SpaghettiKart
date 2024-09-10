#include "GenericArray.h"

namespace SF64 {
uint8_t* GenericArray::GetPointer() {
    return mData.data();
}

size_t GenericArray::GetPointerSize() {
    return mData.size();
}
} // namespace SF64
