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
    for (auto actor = gWorldInstance.Actors.begin(); actor != gWorldInstance.Actors.end();) {
        AActor* act = *actor; // Get a mutable copy
        if (act->bPendingDestroy) {
            if (act->IsMod()) { // C++ actor
                delete act;
                actor = gWorldInstance.Actors.erase(actor); // Remove from container
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
    for (auto actor = gWorldInstance.StaticMeshActors.begin(); actor != gWorldInstance.StaticMeshActors.end();) {
        StaticMeshActor* act = *actor; // Get a mutable copy
        if (act->bPendingDestroy) {
            delete act;
            actor = gWorldInstance.StaticMeshActors.erase(actor); // Remove from container
            continue;
        } else {
            actor++;
        }
    }
}

void CleanObjects() {
    for (auto object = gWorldInstance.Objects.begin(); object != gWorldInstance.Objects.end();) {
        OObject* obj = *object; // Get a mutable copy
        if (obj->bPendingDestroy) {
            delete obj;
            object = gWorldInstance.Objects.erase(object); // Remove from container
            continue;
        }
        object++;
    }
}
