#pragma once

#include <libultraship.h>

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"

class ParticleEmitter {
  public:
    virtual ~ParticleEmitter() = default; // Virtual destructor for proper cleanup in derived classes

    explicit ParticleEmitter();

    virtual void Tick();
    virtual void Draw(s32 cameraId);
    virtual bool IsMod();
};
}
