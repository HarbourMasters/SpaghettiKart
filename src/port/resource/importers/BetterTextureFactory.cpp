#include "BetterTextureFactory.h"
#include "resource/type/Texture.h"
#include "spdlog/spdlog.h"
#include <stb_image.h>
#include <Context.h>
#include "resource/archive/ArchiveManager.h"
#include "resource/ResourceManager.h"

namespace MK64 {

std::shared_ptr<Ship::IResource> loadPngTexture(std::shared_ptr<Ship::File> filePng, std::shared_ptr<Ship::ResourceInitData> initData) {
    auto texture = std::make_shared<Fast::Texture>(initData);

    int height, width = 0;
    texture->ImageData = stbi_load_from_memory((const stbi_uc*)filePng->Buffer.get()->data(),
                                               filePng->Buffer.get()->size(), &width, &height, nullptr, 4);
    texture->Width = width;
    texture->Height = height;
    texture->Type = Fast::TextureType::RGBA32bpp;
    texture->ImageDataSize = texture->Width * texture->Height * 4;
    texture->Flags = TEX_FLAG_LOAD_AS_IMG;
    return texture;
}

std::vector<std::string> extension = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG", ".bmp", ".BMP"};

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTextureV0::ReadResource(std::shared_ptr<Ship::File> file,
                                             std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    for (const auto& ext : extension) {
        auto filePng = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(
        initData->Path + ext);

        if (filePng != nullptr) {
            return loadPngTexture(filePng, initData);
        }
    }

    auto texture = std::make_shared<Fast::Texture>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    texture->Type = (Fast::TextureType)reader->ReadUInt32();
    texture->Width = reader->ReadUInt32();
    texture->Height = reader->ReadUInt32();
    texture->ImageDataSize = reader->ReadUInt32();
    texture->ImageData = new uint8_t[texture->ImageDataSize];

    reader->Read((char*)texture->ImageData, texture->ImageDataSize);

    return texture;
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTextureV1::ReadResource(std::shared_ptr<Ship::File> file,
                                             std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    for (const auto& ext : extension) {
        auto filePng = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(
        initData->Path + ext);

        if (filePng != nullptr) {
            return loadPngTexture(filePng, initData);
        }
    }

    auto texture = std::make_shared<Fast::Texture>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    texture->Type = (Fast::TextureType)reader->ReadUInt32();
    texture->Width = reader->ReadUInt32();
    texture->Height = reader->ReadUInt32();
    texture->Flags = reader->ReadUInt32();
    texture->HByteScale = reader->ReadFloat();
    texture->VPixelScale = reader->ReadFloat();
    texture->ImageDataSize = reader->ReadUInt32();
    texture->ImageData = new uint8_t[texture->ImageDataSize];

    reader->Read((char*)texture->ImageData, texture->ImageDataSize);

    return texture;
}
} // namespace Fast
