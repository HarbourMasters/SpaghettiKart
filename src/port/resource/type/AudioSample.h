#pragma once

#include <cstdint>
#include "resource/Resource.h"

struct AdpcmLoop {
    uint32_t start;
    uint32_t end;
    uint32_t count;
    uint32_t pad;
    int16_t* state; // only exists if count != 0. 8-byte aligned
};

struct AdpcmBook {
    int32_t order;
    int32_t npredictors;
    int16_t* book; // size 8 * order * npredictors. 8-byte aligned
};

struct AudioBankSample {
    uint8_t unused;
    uint8_t loaded;
    uint8_t* sampleAddr;
    AdpcmLoop* loop;
    AdpcmBook* book;
    uint32_t sampleSize; // never read. either 0 or 1 mod 9, depending on padding
};

namespace SM64 {

class AudioSample : public Ship::Resource<AudioBankSample> {
  public:
    using Resource::Resource;

    AudioSample() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    AudioBankSample* GetPointer();
    size_t GetPointerSize();

    AudioBankSample mData;

    AdpcmLoop loop;
    AdpcmBook book;
    std::vector<uint8_t> sampleAddr;
};
} // namespace SM64
