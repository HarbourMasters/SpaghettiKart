#pragma once

#ifdef __cplusplus
#include <cstdint>
#endif

struct AudioSequenceData {
    uint32_t bankCount;
    uint8_t* banks;
    uint8_t* data;
    uint8_t id;
};

#ifdef __cplusplus
#include "resource/Resource.h"
namespace SM64 {

class AudioSequence : public Ship::Resource<AudioSequenceData> {
  public:
    using Resource::Resource;

    AudioSequence() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    AudioSequenceData* GetPointer();
    size_t GetPointerSize();

    AudioSequenceData mData;
    std::vector<uint8_t> banks;
    std::vector<uint8_t> sampleData;
};
} // namespace SM64
#endif
