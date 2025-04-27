#include "Minimap.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
Minimap::Minimap() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

MinimapTexture* Minimap::GetPointer() {
    return &Texture;
}

size_t Minimap::GetPointerSize() {
    return sizeof(Texture);
}

} // namespace MK64
