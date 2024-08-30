#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {
typedef struct {
    s32 msgId;
    u16* msgPtr;
} MsgLookup;

class Message : public Ship::Resource<void> {
  public:
    using Resource::Resource;

    void* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<uint16_t> mMessage;
};

class MessageLookup : public Ship::Resource<MsgLookup> {
  public:
    using Resource::Resource;

    MsgLookup* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<MsgLookup> mLookupTable;
};
} // namespace SF64
