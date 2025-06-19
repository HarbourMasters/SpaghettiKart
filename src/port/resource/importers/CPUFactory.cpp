#include "CPUFactory.h"
#include "../type/CPU.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryCPUV0::ReadResource(std::shared_ptr<Ship::File> file, std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto ai = std::make_shared<CPU>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    ai->CPUList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        CPUBehaviour data;
        data.pathPointStart = reader->ReadInt16();
        data.pathPointEnd = reader->ReadInt16();
        data.type = reader->ReadInt32();

        ai->CPUList.push_back(data);
    }

    return ai;
}
} // namespace MK64
