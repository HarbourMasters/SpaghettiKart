#include <libultraship.h>
#include "Object.h"

#include "World.h"

extern "C" {
    #include "camera.h"
}


    //GameActor()

OObject::OObject() {}

    // Virtual functions to be overridden by derived classes
void OObject::Tick() { }
void OObject::Tick60fps() {}
void OObject::Draw(s32 cameraId) { }
void OObject::Expire() { }
void OObject::Destroy() {
    PendingDestroy = true;
}
void OObject::Reset() { }
