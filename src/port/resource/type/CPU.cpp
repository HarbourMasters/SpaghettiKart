#include "CPU.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
CPU::CPU() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

CPUBehaviour* CPU::GetPointer() {
    return CPUList.data();
}

size_t CPU::GetPointerSize() {
    return CPUList.size() * sizeof(CPUBehaviour);
}
} // namespace MK64
