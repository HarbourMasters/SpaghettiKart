#include "BetterTextureFactory.h"
#include "fast/resource/type/Texture.h"
#include "spdlog/spdlog.h"
#include <stb_image.h>
#include <ship/Context.h>
#include "ship/resource/archive/ArchiveManager.h"
#include "ship/resource/ResourceManager.h"
#include "port/resource/AsyncTextureUpgrader.h"
#include <cstring>

namespace MK64 {

std::vector<std::string> extension = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG", ".bmp", ".BMP"};

// Replacement images at or below this (compressed) size decode inline: they
// take ~1-3 ms, and deferring them makes rapidly-cycling textures (kart
// animation frames — ~90% of a 4K pack) visibly flicker between original and
// HD art. Larger images (skies, banner sheets: 30-80 ms decodes, the actual
// stutter source) stream in asynchronously with a brief original-art pop-in.
static constexpr size_t kSyncDecodeMaxBytes = 200 * 1024;

// Image replacements (texture packs) are the port's "alternate assets";
// gating the lookup here is what makes the alt-assets toggle work.
static std::shared_ptr<Ship::File> findReplacementImage(const std::shared_ptr<Ship::ResourceInitData>& initData) {
    if (!Ship::Context::GetRawInstance()->GetResourceManager()->IsAltAssetsEnabled()) {
        return nullptr;
    }
    for (const auto& ext : extension) {
        auto filePng = Ship::Context::GetRawInstance()->GetResourceManager()->LoadFileProcess(initData->Path + ext);
        if (filePng != nullptr) {
            return filePng;
        }
    }
    return nullptr;
}

// Decode a replacement image and apply it to a just-created texture (which has
// never been drawn, so no GPU-cache invalidation is needed). Field semantics
// match AsyncTextureUpgrader::ApplyCompleted.
static void upgradeTextureInline(const std::shared_ptr<Fast::Texture>& texture,
                                 const std::shared_ptr<Ship::File>& fileImg, uint16_t origWidth, uint16_t origHeight) {
    int width = 0, height = 0;
    stbi_uc* decoded = stbi_load_from_memory((const stbi_uc*)fileImg->Buffer.get()->data(),
                                             fileImg->Buffer.get()->size(), &width, &height, nullptr, 4);
    if (decoded == nullptr || width <= 0 || height <= 0) {
        SPDLOG_WARN("BetterTextureFactory: failed to decode replacement image for {}", texture->GetInitData()->Path);
        if (decoded != nullptr) {
            stbi_image_free(decoded);
        }
        return; // keep the original binary art
    }

    const size_t size = (size_t)width * height * 4;
    uint8_t* pixels = new uint8_t[size];
    std::memcpy(pixels, decoded, size);
    stbi_image_free(decoded);

    delete[] texture->ImageData;
    texture->ImageData = pixels;
    texture->ImageDataSize = (uint32_t)size;
    texture->Width = (uint16_t)width;
    texture->Height = (uint16_t)height;
    texture->OrigWidth = origWidth;
    texture->OrigHeight = origHeight;
    texture->Type = Fast::TextureType::RGBA32bpp;
    texture->Flags = TEX_FLAG_LOAD_AS_IMG;
    texture->HByteScale = 1.0f;
    texture->VPixelScale = 1.0f;
}

static void applyReplacement(const std::shared_ptr<Fast::Texture>& texture,
                             const std::shared_ptr<Ship::ResourceInitData>& initData) {
    auto fileImg = findReplacementImage(initData);
    if (fileImg == nullptr) {
        return;
    }
    const uint16_t origWidth = texture->Width;
    const uint16_t origHeight = texture->Height;
    if (fileImg->Buffer.get()->size() <= kSyncDecodeMaxBytes) {
        upgradeTextureInline(texture, fileImg, origWidth, origHeight);
    } else {
        AsyncTextureUpgrader::Instance().Enqueue(texture, fileImg, origWidth, origHeight);
    }
    // Kick off background decoding for the rest of this texture's directory
    // (kart animation frames, font sheets, ...) so siblings are already HD
    // when they first appear — the render thread never pays for them.
    AsyncTextureUpgrader::Instance().PrefetchSiblings(initData->Path);
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTextureV0::ReadResource(std::shared_ptr<Ship::File> file,
                                             std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto texture = std::make_shared<Fast::Texture>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    texture->Type = (Fast::TextureType)reader->ReadUInt32();
    texture->Width = reader->ReadUInt32();
    texture->Height = reader->ReadUInt32();
    texture->ImageDataSize = reader->ReadUInt32();
    texture->ImageData = new uint8_t[texture->ImageDataSize];

    reader->Read((char*)texture->ImageData, texture->ImageDataSize);

    // Texture packs replace art at the same path via image siblings: small
    // ones decode inline, large ones stream in asynchronously (see
    // kSyncDecodeMaxBytes).
    applyReplacement(texture, initData);

    return texture;
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryTextureV1::ReadResource(std::shared_ptr<Ship::File> file,
                                             std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
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

    applyReplacement(texture, initData);

    return texture;
}
} // namespace MK64
