#include "ActorSpawnDataFactory.h"
#include "../type/SpawnData.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"
#include <common_structs.h>

namespace MK64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryActorSpawnDataV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto section = std::make_shared<ActorSpawn>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    section->ActorSpawnDataList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        ActorSpawnData data;
        data.pos[0] = reader->ReadInt16();
        data.pos[1] = reader->ReadInt16();
        data.pos[2] = reader->ReadInt16();
        data.signedSomeId = reader->ReadInt16();

        section->ActorSpawnDataList.push_back(data);
    }

    return section;
}
} // namespace MK64
