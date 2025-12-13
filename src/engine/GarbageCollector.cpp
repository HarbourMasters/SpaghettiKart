#include "GarbageCollector.h"
#include "World.h"

/**
 * Removes objects if they are marked for deletion
 */
void RunGarbageCollector() {
    CleanActors();
    CleanObjects();
    CleanStaticMeshActors();
}

void CleanActors() {
    for (auto actor = GetWorld()->Actors.begin(); actor != GetWorld()->Actors.end();) {
        auto* act = actor->get(); // Get a mutable copy
        if (act->bPendingDestroy) {
            if (act->IsMod()) { // C++ actor
                actor = GetWorld()->Actors.erase(actor); // Remove from container
            } else { // Old C actor
                act->Flags = 0;
                act->Type = 0;
                act->Name = "";
                act->ResourceName = "";
                actor++; // Manually advance the iterator since no deletion happens here
            }
            gNumActors -= 1;
            continue;
        }
        actor++;
    }
}

void CleanStaticMeshActors() {
    for (auto actor = GetWorld()->StaticMeshActors.begin(); actor != GetWorld()->StaticMeshActors.end();) {
        StaticMeshActor* act = actor->get(); // Get a mutable copy
        if (act->bPendingDestroy) {
            actor = GetWorld()->StaticMeshActors.erase(actor); // Remove from container
            continue;
        } else {
            actor++;
        }
    }
}

void CleanObjects() {
    for (auto object = GetWorld()->Objects.begin(); object != GetWorld()->Objects.end();) {
        OObject* obj = object->get(); // Get a mutable copy
        if (obj->bPendingDestroy) {
            object = GetWorld()->Objects.erase(object); // Remove from container
            continue;
        }
        object++;
    }
}
