#include "TrackPathPointFactory.h"
#include "../type/TrackPathPointData.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"
#include "tinyxml2.h"

namespace MK64 {
std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTrackPathPointsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                    std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto section = std::make_shared<TrackPathPointData>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    section->TrackPathPointList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        TrackPathPoint data;
        data.posX = reader->ReadInt16();
        data.posY = reader->ReadInt16();
        data.posZ = reader->ReadInt16();
        data.trackSectionId = reader->ReadUInt16();

        section->TrackPathPointList.push_back(data);
    }

    return section;
}

std::shared_ptr<Ship::IResource>
ResourceFactoryXMLTrackPathPointsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                         std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto paths = std::make_shared<Paths>(initData);

    auto root = std::get<std::shared_ptr<tinyxml2::XMLDocument>>(file->Reader)->FirstChildElement();

    auto path = root->FirstChildElement("TrackPathPoint");

    while (path != nullptr) {

        std::vector<TrackPathPoint> waypointPath; // Temporary container for this path

        auto pointElem = path->FirstChildElement("Point");

        while (pointElem != nullptr) {
            TrackPathPoint point;
            point.posX = pointElem->IntAttribute("X");
            point.posY = pointElem->IntAttribute("Y");
            point.posZ = pointElem->IntAttribute("Z");
            point.trackSectionId = pointElem->IntAttribute("ID");

            waypointPath.push_back(point); // Push to temp vector

            pointElem = pointElem->NextSiblingElement("Point");
        }

        paths->PathList.push_back(waypointPath); // Push full path to outer list

        path = path->NextSiblingElement("TrackPathPoint");
    }
    return paths;
}

} // namespace MK64
