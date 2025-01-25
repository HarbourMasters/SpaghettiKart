#include "AudioSample.h"

namespace SM64 {
AudioBankSample* AudioSample::GetPointer() {
    return &mData;
}

size_t AudioSample::GetPointerSize() {
    return sizeof(mData);
}
} // namespace SM64
