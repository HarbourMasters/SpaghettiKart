#include "KartAI.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
KartAI::KartAI() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

KartAIBehaviour* KartAI::GetPointer() {
    return KartAIList.data();
}

size_t KartAI::GetPointerSize() {
    return KartAIList.size() * sizeof(KartAIBehaviour);
}
} // namespace MK64
