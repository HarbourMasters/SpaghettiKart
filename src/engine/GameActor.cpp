#include <cstdint>
#include <iostream>

extern "C" {
    #include "actor_types.h"
}

class GameActor {
public:
    explicit GameActor(Actor* actor) : myActor(actor) {}

    void Init() {
        std::cout << "Initializing actor of type " << myActor->type << "\n";
        // Additional initialization logic
    }

    void Update() {
        std::cout << "Updating actor of type " << myActor->type << "\n";
        // Update actor properties
        // myActor->pos.x += myActor->velocity.x;
        // myActor->pos.y += myActor->velocity.y;
        // myActor->pos.z += myActor->velocity.z;
    }

    void Render() {
        std::cout << "Rendering actor of type " << myActor->type << "\n";
        // Render logic
    }

    void Expire() {
        std::cout << "Expiring actor of type " << myActor->type << "\n";
        // Expire logic
    }

    void Destroy() {
        std::cout << "Destroying actor of type " << myActor->type << "\n";
        // Destroy logic
    }

private:
    Actor* myActor;
};
