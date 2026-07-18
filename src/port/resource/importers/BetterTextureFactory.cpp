#include "BetterTextureFactory.h"
#include "fast/resource/type/Texture.h"
#include "spdlog/spdlog.h"
#include <stb_image.h>
#include <ship/Context.h>
#include "ship/resource/archive/ArchiveManager.h"
#include "ship/resource/ResourceManager.h"

namespace MK64 {

std::shared_ptr<Ship::IResource> loadPngTexture(std::shared_ptr<Ship::File> filePng, std::shared_ptr<Ship::ResourceInitData> initData,
                                                uint16_t origWidth, uint16_t origHeight) {
    auto texture = std::make_shared<Fast::Texture>(initData);

    int height, width = 0;
    texture->ImageData = stbi_load_from_memory((const stbi_uc*)filePng->Buffer.get()->data(),
                                               filePng->Buffer.get()->size(), &width, &height, nullptr, 4);
    texture->Width = width;
    texture->Height = height;
    // Record the dimensions of the texture this image replaces. The N64 game
    // often samples a TILE slightly larger than the texture (clamp repeats
    // the edge); UVs must be normalized in the ORIGINAL texture's texel
    // space or the tile overhang crops the replacement art (visible as text
    // glyphs cut off at the bottom/right with 4K packs).
    texture->OrigWidth = origWidth;
    texture->OrigHeight = origHeight;
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

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);
    auto binType = (Fast::TextureType)reader->ReadUInt32();
    uint32_t binWidth = reader->ReadUInt32();
    uint32_t binHeight = reader->ReadUInt32();

    for (const auto& ext : extension) {
        auto filePng = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(
        initData->Path + ext);

        if (filePng != nullptr) {
            return loadPngTexture(filePng, initData, binWidth, binHeight);
        }
    }

    auto texture = std::make_shared<Fast::Texture>(initData);

    texture->Type = binType;
    texture->Width = binWidth;
    texture->Height = binHeight;
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

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);
    auto binType = (Fast::TextureType)reader->ReadUInt32();
    uint32_t binWidth = reader->ReadUInt32();
    uint32_t binHeight = reader->ReadUInt32();

    for (const auto& ext : extension) {
        auto filePng = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(
        initData->Path + ext);

        if (filePng != nullptr) {
            return loadPngTexture(filePng, initData, binWidth, binHeight);
        }
    }

    auto texture = std::make_shared<Fast::Texture>(initData);

    texture->Type = binType;
    texture->Width = binWidth;
    texture->Height = binHeight;
    texture->Flags = reader->ReadUInt32();
    texture->HByteScale = reader->ReadFloat();
    texture->VPixelScale = reader->ReadFloat();
    texture->ImageDataSize = reader->ReadUInt32();
    texture->ImageData = new uint8_t[texture->ImageDataSize];

    reader->Read((char*)texture->ImageData, texture->ImageDataSize);

    return texture;
}
} // namespace Fast
