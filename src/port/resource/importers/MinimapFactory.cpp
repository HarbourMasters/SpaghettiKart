#include "MinimapFactory.h"
#include "port/resource/type/Minimap.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"

namespace MK64 {

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryMinimapV0::ReadResource(std::shared_ptr<Ship::File> file,
                                         std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto texture = std::make_shared<MK64::Minimap>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    int width = 0, height = 0;
    stbi_uc* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(file->Buffer->data()), file->Buffer->size(), &width, &height, nullptr, 1);

    if (data == nullptr) {
        SPDLOG_ERROR("MinimapFactory.cpp: Error loading minimap texture {}", stbi_failure_reason());
        return nullptr;
    }

    // Calculate new size for 4bpp (two pixels per byte)
    size_t pixelCount = width * height;
    size_t packedSize = (pixelCount + 1) / 2; // Round up if odd

    uint8_t* packedData = new uint8_t[packedSize];

    for (size_t i = 0; i < pixelCount; i += 2) {
        uint8_t first = data[i] >> 4; // 8-bit -> 4-bit
        uint8_t second = 0;
        if (i + 1 < pixelCount) {
            second = data[i + 1] >> 4;
        }
        packedData[i / 2] = (first << 4) | second;
    }

    // Done with original data
    stbi_image_free(data);

    // Store packed data
    texture->Texture.Width = width;
    texture->Texture.Height = height;
    texture->Texture.Channels = 1;
    texture->Texture.Size = packedSize;
    texture->Texture.Data = packedData;

    return texture;
}

}
