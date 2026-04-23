#pragma once

#include <string>
#include <vector>
#include "hmui/graphics/GraphicsContext.h"

class D_ImageProvider : public ImageProvider {
public:
    explicit D_ImageProvider(const std::string& path) : imagePath(path) {}

    ImageHandle* load() override;
    void dispose() override;

private:
    std::string imagePath;
    ImageHandle* texture;
};

#define AssetImage(path) std::dynamic_pointer_cast<ImageProvider>(std::make_shared<D_ImageProvider>(path))
