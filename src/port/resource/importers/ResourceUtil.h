#pragma once

#include "resourcebridge.h"
#include "Context.h"

namespace SF64 {
template <typename T> T LoadChild(uint64_t crc) {
    auto path = ResourceGetNameByCrc(crc);
    if (path == nullptr) {
        return nullptr;
    }
    auto asset = Ship::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(path);
    return asset ? static_cast<T>(asset->GetRawPointer()) : nullptr;
}
} // namespace SF64
