#include "Animation.h"

namespace SF64 {
AnimationData* Animation::GetPointer() {
    return &mData;
}

size_t Animation::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SF64
