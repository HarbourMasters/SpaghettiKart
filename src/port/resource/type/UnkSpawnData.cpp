#include "UnkSpawnData.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
UnkActorSpawn::UnkActorSpawn() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

UnkActorSpawnData* UnkActorSpawn::GetPointer() {
    return UnkActorSpawnDataList.data();
}

size_t UnkActorSpawn::GetPointerSize() {
    return UnkActorSpawnDataList.size() * sizeof(UnkActorSpawnData);
}
} // namespace LUS
