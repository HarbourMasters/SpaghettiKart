#pragma once

#include "resource/Resource.h"
#include <vector>

struct KartAIBehaviour {
    /* 0x0 */ int16_t waypointStart;
    /* 0x2 */ int16_t waypointEnd;
    /* 0x4 */ int32_t type;
}; // size = 0x8

namespace MK64 {
class KartAI : public Ship::Resource<KartAIBehaviour> {
  public:
    using Resource::Resource;

    KartAI();

    KartAIBehaviour* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<KartAIBehaviour> KartAIList;
};
} // namespace MK64
