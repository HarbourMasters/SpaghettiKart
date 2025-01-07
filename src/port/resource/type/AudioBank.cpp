#include "AudioBank.h"

namespace SM64 {
CtlEntry* AudioBank::GetPointer() {
    return &mData;
}

size_t AudioBank::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SM64
