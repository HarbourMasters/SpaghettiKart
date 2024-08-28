#pragma once

#include <cstdint>
#include <Resource.h>

namespace SF64 {
class Script : public Ship::Resource<uint16_t*> {
  public:
    using Resource::Resource;

    uint16_t** GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<uint16_t*> mScripts;
};

class ScriptCMDs : public Ship::Resource<uint16_t> {
  public:
    using Resource::Resource;

    uint16_t* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<uint16_t> mCommands;
};
} // namespace SF64
