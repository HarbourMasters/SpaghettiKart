#include "TrackWaypointFactory.h"
#include "../type/TrackWaypoint.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTrackWaypointsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                    std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto section = std::make_shared<TrackWaypoints>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    section->TrackWaypointList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        TrackWaypointData data;
        data.posX = reader->ReadInt16();
        data.posY = reader->ReadInt16();
        data.posZ = reader->ReadInt16();
        data.trackSegment = reader->ReadUInt16();

        section->TrackWaypointList.push_back(data);
    }

    return section;
}

std::shared_ptr<Ship::IResource>
ResourceFactoryXMLTrackWaypointsV0::ReadResource(std::shared_ptr<Ship::File> file,
                                         std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto paths = std::make_shared<Paths>(initData);

    auto root = std::get<std::shared_ptr<tinyxml2::XMLDocument>>(file->Reader)->FirstChildElement();

    auto path = root->FirstChildElement("TrackWaypoint");

    while (path != nullptr) {

        std::vector<TrackWaypointData> waypointPath; // Temporary container for this path

        auto pointElem = path->FirstChildElement("Point");

        while (pointElem != nullptr) {
            TrackWaypointData point;
            point.posX = pointElem->IntAttribute("X");
            point.posY = pointElem->IntAttribute("Y");
            point.posZ = pointElem->IntAttribute("Z");
            point.trackSegment = pointElem->IntAttribute("ID");

            waypointPath.push_back(point); // Push to temp vector

            pointElem = pointElem->NextSiblingElement("Point");
        }

        paths->PathList.push_back(waypointPath); // Push full path to outer list

        path = path->NextSiblingElement("TrackWaypoint");
    }
    return paths;
}

} // namespace MK64
