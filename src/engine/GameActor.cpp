#include <libultraship.h>

#include "GameActor.h"
#include "structs.h"

    //GameActor()

GameActor::GameActor() {}

    // Virtual functions to be overridden by derived classes
void GameActor::Init() {  }
void GameActor::Update() {  }
void GameActor::Render(Camera *camera) { }
void GameActor::Collision() {}
void GameActor::Expire() { }
void GameActor::Destroy() { }
