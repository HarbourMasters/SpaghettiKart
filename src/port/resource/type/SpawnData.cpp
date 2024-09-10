#include "SpawnData.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
ActorSpawn::ActorSpawn() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

ActorSpawnData* ActorSpawn::GetPointer() {
    return ActorSpawnDataList.data();
}

size_t ActorSpawn::GetPointerSize() {
    return ActorSpawnDataList.size() * sizeof(ActorSpawnData);
}
} // namespace MK64
