#include "LUSImageProvider.h"

#include "port/Engine.h"

std::unordered_map<std::string, ImageHandle> textureCache;

ImageHandle* D_ImageProvider::load() {
    if(imagePath.find("__OTR__") != std::string::npos) {
        imagePath = imagePath.substr(7);
    }
    auto it = textureCache.find(imagePath);
    if (it != textureCache.end()) {
        return &it->second;
    }

    auto gui = GameEngine::Instance->context->GetWindow()->GetGui();
    if(!gui->HasTextureByName(imagePath)){
        throw std::runtime_error("Texture not found: " + imagePath);
    }

    auto size = gui->GetTextureSize(imagePath);
    auto tex = gui->GetTextureByName(imagePath);

    textureCache[imagePath] = {
        (int) size.x,
        (int) size.y,
        (void*) tex
    };

    return &textureCache[imagePath];
}

void D_ImageProvider::dispose() {
    auto it = textureCache.find(imagePath);
    if (it != textureCache.end()) {
        textureCache.erase(it);
    }
}