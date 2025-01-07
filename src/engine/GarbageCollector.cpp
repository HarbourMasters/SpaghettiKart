#include "GarbageCollector.h"
#include "World.h"

void RunGarbageCollector() {
    // CleanActors();
    CleanObjects();
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

void CleanObjects() {
    for (auto object = gWorldInstance.Objects.begin(); object != gWorldInstance.Objects.end();) {
        OObject* obj = *object; // Get a mutable copy
        if (obj->PendingDestroy) {
            delete obj;
            object = gWorldInstance.Objects.erase(object); // Remove from container
            continue;
        }
        object++;
    }
}
