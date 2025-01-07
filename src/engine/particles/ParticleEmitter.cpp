#include <libultraship.h>

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
}

// Virtual functions to be overridden by derived classes
void ParticleEmitter::Tick() {
}
void ParticleEmitter::Draw(s32 cameraId) {
}

bool ParticleEmitter::IsMod() {
    return false;
}
