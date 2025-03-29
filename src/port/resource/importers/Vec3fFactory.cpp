#include "Vec3fFactory.h"
#include "../type/Vec3fArray.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryVec3fV0::ReadResource(std::shared_ptr<Ship::File> file,
                                           std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto vec = std::make_shared<Vec3fArray>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    auto vecCount = reader->ReadUInt32();

    SPDLOG_INFO("Vec3f Count: {}", vecCount);

    for (uint32_t i = 0; i < vecCount; i++) {
        auto x = reader->ReadFloat();
        auto y = reader->ReadFloat();
        auto z = reader->ReadFloat();
        vec->mData.emplace_back(x, y, z);
    }

    return vec;
}
} // namespace SF64
