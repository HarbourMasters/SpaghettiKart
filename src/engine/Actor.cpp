#include <libultraship.h>

#include "Actor.h"

    //GameActor()

AActor::AActor() {}

    // Virtual functions to be overridden by derived classes
void AActor::Tick() {  }
void AActor::Draw(Camera *camera) { }
void AActor::Collision() {}
void AActor::Destroy() { }