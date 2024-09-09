#include <libultraship.h>

#include "ABanana.h"
#include "GameActor.h"
#include "structs.h"

extern "C" {
    void update_actor_banana(struct BananaActor*);
    void render_actor_banana(Camera*, float[4][4], struct BananaActor*);
}

ABanana::ABanana(const float pos[3], const s16 rot[3], const float velocity[3]) {
    // Initialize the BananaActor's position, rotation, and velocity
    std::copy(pos, pos + 3, this->pos);
    std::copy(rot, rot + 3, this->rot);
    std::copy(velocity, velocity + 3, this->velocity);
}

//Collision unk30;  // Use the globally defined Collision type here

//GameActor()

// Virtual functions to be overridden by derived classes
void ABanana::Init() {

}

void ABanana::Update() { 
    update_actor_banana(reinterpret_cast<struct BananaActor*>(this));
}

void ABanana::Render(Camera *camera) {
    render_actor_banana(camera, NULL, reinterpret_cast<struct BananaActor*>(this));
}
void ABanana::Collision() { }
void ABanana::Expire() { }
void ABanana::Destroy() { }

//void ABanana::Held() {}
