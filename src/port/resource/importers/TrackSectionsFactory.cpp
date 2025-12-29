#include "TrackSectionsFactory.h"
#include "../type/TrackSections.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"
#include "ship/utils/StrHash64.h"
#include "tinyxml2.h"
#include "src/engine/tracks/Track.h"

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
        TrackSections data;
        //                      Convert n64 addr to native addr
        data.crc = reader->ReadUInt64();
        data.surfaceType = reader->ReadUByte();
        data.sectionId = reader->ReadUByte();
        data.clip = reader->ReadUInt16();
        data.layer = 0;
        data.location[0] = 0;
        data.location[1] = 0;
        data.location[2] = 0;

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
            TrackSections data;
            //                      Convert n64 addr to native addr
            data.crc = CRC64(child->Attribute("gfx_path"));
            data.surfaceType = child->IntAttribute("surface");
            data.sectionId = child->IntAttribute("section");
            data.clip = child->IntAttribute("flags");
            data.layer = child->IntAttribute("drawlayer");
            data.location[0] = child->FloatAttribute("x");
            data.location[1] = child->FloatAttribute("y");
            data.location[2] = child->FloatAttribute("z");

            section->TrackSectionsList.push_back(data);
        }
        child = child->NextSiblingElement();
    }

    return section;
}
} // namespace MK64
