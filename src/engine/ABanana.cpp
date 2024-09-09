#include <libultraship.h>

#include "ABanana.h"
#include "GameActor.h"
#include "structs.h"

extern "C" {
    void update_actor_banana(struct BananaActor*);
    void render_actor_banana(Camera*, float[4][4], struct BananaActor*);
}

ABanana::ABanana(uint16_t playerId, const float pos[3], const s16 rot[3], const float velocity[3]) {
    // Initialize the BananaActor's position, rotation, and velocity
    std::copy(pos, pos + 3, this->a.pos);
    //std::copy(rot, rot + 3, this->a.rot);
    std::copy(velocity, velocity + 3, this->a.velocity);

    this->a.type = 6; // ACTOR_BANANA
    this->a.flags = -0x8000;
    this->a.unk_04 = 0;
    this->a.state = 0;
    //this->a.unk_08 = 0.0f;
    this->a.flags |= 0x4000 | 0x1000;
    this->a.boundingBoxSize = 2.0f;

    //Collision *collision = &this->a.unk30;
    this->a.unk30.meshIndexYX = 5000;
    this->a.unk30.meshIndexZX = 5000;
    this->a.unk30.meshIndexZY = 5000;
    this->a.unk30.unk30 = 0;
    this->a.unk30.unk32 = 0;
    this->a.unk30.unk34 = 0;
    this->a.unk30.surfaceDistance[0] = 0;
    this->a.unk30.surfaceDistance[1] = 0;
    this->a.unk30.surfaceDistance[2] = 0;
    this->a.unk30.unk48[0] = 0;
    this->a.unk30.unk48[1] = 0;
    this->a.unk30.unk48[2] = 1.0f;
    this->a.unk30.unk54[0] = 1.0f;
    this->a.unk30.unk54[1] = 0.0f;
    this->a.unk30.unk54[2] = 0.0f;
    this->a.unk30.orientationVector[0] = 0.0f;
    this->a.unk30.orientationVector[1] = 1.0f;
    this->a.unk30.orientationVector[2] = 0.0f;
    this->a.playerId = playerId;
    this->a.state = 0; // HELD_BANANA
    this->a.unk_04 = 20;

}

//Collision unk30;  // Use the globally defined Collision type here

//GameActor()

// Virtual functions to be overridden by derived classes
void ABanana::Init() {

}

void ABanana::Update() { 
    update_actor_banana(&this->a);
}

void ABanana::Render(Camera *camera) {
    render_actor_banana(camera, NULL, &this->a);
}
void ABanana::Collision() { }
void ABanana::Expire() { }
void ABanana::Destroy() { }

//void ABanana::Held() {}
