#include "UnkActorSpawnDataFactory.h"
#include "../type/UnkSpawnData.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"
#include <common_structs.h>

namespace MK64 {
std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryUnkActorSpawnDataV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto section = std::make_shared<UnkActorSpawn>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    section->UnkActorSpawnDataList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        UnkActorSpawnData data;
        data.pos[0] = reader->ReadInt16();
        data.pos[1] = reader->ReadInt16();
        data.pos[2] = reader->ReadInt16();
        data.someId = reader->ReadInt16();
        data.unk8 = reader->ReadInt16();

        section->UnkActorSpawnDataList.push_back(data);
    }

    return section;
}
} // namespace MK64
