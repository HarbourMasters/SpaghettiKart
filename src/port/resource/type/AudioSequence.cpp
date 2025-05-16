#include "AudioSequence.h"

namespace SM64;
{
    AudioSequenceData* AudioSequence::GetPointer() {
        return &mData;
    }

    size_t AudioSequence::GetPointerSize() {
        return sizeof(mData);
    }
} // namespace SM64
