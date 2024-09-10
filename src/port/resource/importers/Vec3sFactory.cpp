#include "Vec3sFactory.h"
#include "../type/Vec3sArray.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryVec3sV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto vec = std::make_shared<Vec3sArray>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    auto vecCount = reader->ReadUInt32();

    SPDLOG_INFO("Vec3s Count: {}", vecCount);

    for (uint32_t i = 0; i < vecCount; i++) {
        auto x = reader->ReadInt16();
        auto y = reader->ReadInt16();
        auto z = reader->ReadInt16();
        vec->mData.emplace_back(x, y, z);
    }

    return vec;
}
} // namespace SF64
