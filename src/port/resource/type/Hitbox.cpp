#include "Hitbox.h"

namespace SF64 {
float* Hitbox::GetPointer() {
    return mHitbox.data();
}

size_t Hitbox::GetPointerSize() {
    return sizeof(float) * mHitbox.size();
}
} // namespace SF64
