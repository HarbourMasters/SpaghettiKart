#include "TrackSectionsFactory.h"
#include "../type/TrackSections.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"
#include "tinyxml2.h"

extern "C" {
//#include "memory.h" // Removed to prevent C linkage errors likely related with #include common_structs.h
void* segmented_uintptr_t_to_virtual(uintptr_t);
}

namespace MK64 {
std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTrackSectionsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                   std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto section = std::make_shared<TrackSectionsClass>(initData);
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

std::shared_ptr<Ship::IResource>
ResourceFactoryXMLTrackSectionsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                   std::shared_ptr<Ship::ResourceInitData> initData) {

    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto section = std::make_shared<TrackSectionsO2RClass>(initData);
    auto child =
        std::get<std::shared_ptr<tinyxml2::XMLDocument>>(file->Reader)->FirstChildElement()->FirstChildElement();

    while (child != nullptr) {
        std::string childName = std::string(child->Name());

        if (childName == "Section") {
            TrackSectionsO2R data;
            //                      Convert n64 addr to native addr
            data.addr = std::string(child->Attribute("gfx_path"));
            data.surfaceType = child->IntAttribute("surface");
            data.sectionId = child->IntAttribute("section");
            data.flags = child->IntAttribute("flags");

            section->TrackSectionsList.push_back(data);
        }
        child = child->NextSiblingElement();
    }

    return section;
}
} // namespace MK64
