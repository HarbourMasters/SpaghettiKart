#include "TrackSectionsFactory.h"
#include "../type/TrackSections.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"

extern "C" {
    #include "memory.h"
}

namespace MK64 {
    std::shared_ptr<Ship::IResource> ResourceFactoryBinaryTrackSectionsV0::ReadResource(std::shared_ptr<Ship::File> file) {
        if (!FileHasValidFormatAndReader(file)) {
            return nullptr;
        }

        auto section = std::make_shared<TrackSectionsClass>(file->InitData);
        auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

        uint32_t count = reader->ReadUInt32();
        section->TrackSectionsList.reserve(count);

        for (uint32_t i = 0; i < count; i++) {
            TrackSectionsI data;
            //                      Convert n64 addr to native addr
            data.addr = (uintptr_t) segmented_uintptr_t_to_virtual(reader->ReadUInt32());
            data.surfaceType = reader->ReadUByte();
            data.sectionId = reader->ReadUByte();
            data.flags = reader->ReadUInt16();

            section->TrackSectionsList.push_back(data);
        }

        return section;
    }
}