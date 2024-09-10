#include "CourseVtxFactory.h"
#include "../type/CourseVtx.h"
#include "spdlog/spdlog.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryCourseVtxV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto cvtx = std::make_shared<CourseVtxClass>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint32_t count = reader->ReadUInt32();
    cvtx->CourseVtxList.reserve(count);

    for (uint32_t i = 0; i < count; i++) {
        CourseVtxData data;
        data.ob[0] = reader->ReadInt16();
        data.ob[1] = reader->ReadInt16();
        data.ob[2] = reader->ReadInt16();
        data.tc[0] = reader->ReadInt16();
        data.tc[1] = reader->ReadInt16();
        data.ca[0] = reader->ReadUByte();
        data.ca[1] = reader->ReadUByte();
        data.ca[2] = reader->ReadUByte();
        data.ca[3] = reader->ReadUByte();

        cvtx->CourseVtxList.push_back(data);
    }

    return cvtx;
}
} // namespace MK64
