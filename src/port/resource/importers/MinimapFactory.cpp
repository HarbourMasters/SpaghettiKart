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

    texture->Texture.Size = file->Buffer->size();
    texture->Texture.Width = 0;
    texture->Texture.Height = 0;
    texture->Texture.Channels = 4; // Always force 4 channels (RGBA)
    texture->Texture.Data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(file->Buffer->data()), texture->Texture.Size,
                                               &texture->Texture.Width, &texture->Texture.Height, nullptr, 4);

    if (nullptr == texture->Texture.Data) {
        SPDLOG_ERROR("MinimapFactory.cpp: Error loading minimap texture {}", stbi_failure_reason());
        return nullptr;
    }


   // stbi_image_free(imageData);

    return texture;
}

}
