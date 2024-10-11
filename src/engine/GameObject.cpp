#include "GameObject.h"
#include <libultraship.h>

#include "World.h"

extern "C" {
    #include "camera.h"
}


    //GameActor()

GameObject::GameObject() {}

    // Virtual functions to be overridden by derived classes
void GameObject::Init() {  }
void GameObject::Update() {  }
void GameObject::Render(Camera* camera) { }
void GameObject::Collision() {}
void GameObject::Expire() { }
void GameObject::Destroy() { }
