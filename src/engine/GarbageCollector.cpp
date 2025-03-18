#include "GarbageCollector.h"
#include "World.h"

void RunGarbageCollector() {
    //CleanActors();
    CleanObjects();
    CleanStaticMeshActors();
}

void CleanActors() {
    // for (auto actor = gWorldInstance.Actors.begin(); actor != gWorldInstance.Actors.end();) {
    //     OObject* act = *actor; // Get a mutable copy
    //     if (act->PendingDestroy) {
    //         delete act;
    //         actor = gWorldInstance.Objects.erase(actor); // Remove from container
    //         continue;
    //     }
    //     actor++;
    // }
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
