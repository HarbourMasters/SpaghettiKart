#include <libultraship.h>

#include "GameObject.hpp"
#include "World.hpp"

extern "C" {
    #include "camera.h"
}

extern World gActorManager;

    //GameActor()

GameObject::GameObject() {}

    // Virtual functions to be overridden by derived classes
void GameObject::Init() {  }
void GameObject::Update() {  }
void GameObject::Render() { }
void GameObject::Collision() {}
void GameObject::Expire() { }
void GameObject::Destroy() { }
// World* GameObject::GetWorld() {
//   //  return &gWorldInstance;
// }