#include "KartAIFactory.h"
#include "../type/KartAI.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryKartAIV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto ai = std::make_shared<KartAI>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    ai->KartAIList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        KartAIBehaviour data;
        data.waypointStart = reader->ReadInt16();
        data.waypointEnd = reader->ReadInt16();
        data.type = reader->ReadInt32();

        ai->KartAIList.push_back(data);
    }

    return ai;
}
} // namespace MK64
