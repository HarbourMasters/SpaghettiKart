#pragma once

#include "resource/Resource.h"
#include <vector>

struct CPUBehaviour {
    /* 0x0 */ int16_t pathPointStart;
    /* 0x2 */ int16_t pathPointEnd;
    /* 0x4 */ int32_t type;
}; // size = 0x8

namespace MK64 {
class CPU : public Ship::Resource<CPUBehaviour> {
  public:
    using Resource::Resource;

    CPU();

    CPUBehaviour* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<CPUBehaviour> CPUList;
};
} // namespace MK64
