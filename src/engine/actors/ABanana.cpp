#include <libultraship.h>

#include "ABanana.h"
#include "engine/Actor.h"

extern "C" {
#include "macros.h"
#include "actor_types.h"
void update_actor_banana(struct BananaActor*);
void render_actor_banana(Camera*, float[4][4], struct BananaActor*);
}

ABanana::ABanana(uint16_t playerId, const float pos[3], const s16 rot[3], const float velocity[3]) {
    // Initialize the BananaActor's position, rotation, and velocity
    std::copy(pos, pos + 3, Pos);
    //std::copy(rot, rot + 3, this->a.rot);
    std::copy(velocity, velocity + 3, Velocity);

    Type = 6; // ACTOR_BANANA
    Flags = -0x8000;
    Unk_04 = 20;
    State = HELD_BANANA;

    PlayerId = playerId;
    
    //this->a.unk_08 = 0.0f;
    Flags |= 0x4000 | 0x1000;
    BoundingBoxSize = 2.0f;

    Unk30.meshIndexYX = 5000;
    Unk30.meshIndexZX = 5000;
    Unk30.meshIndexZY = 5000;
    Unk30.unk30 = 0;
    Unk30.unk32 = 0;
    Unk30.unk34 = 0;
    Unk30.surfaceDistance[0] = 0;
    Unk30.surfaceDistance[1] = 0;
    Unk30.surfaceDistance[2] = 0;
    Unk30.unk48[0] = 0;
    Unk30.unk48[1] = 0;
    Unk30.unk48[2] = 1.0f;
    Unk30.unk54[0] = 1.0f;
    Unk30.unk54[1] = 0.0f;
    Unk30.unk54[2] = 0.0f;
    Unk30.orientationVector[0] = 0.0f;
    Unk30.orientationVector[1] = 1.0f;
    Unk30.orientationVector[2] = 0.0f;
}

void ABanana::Tick() { 
    update_actor_banana((BananaActor*)this);
}

void ABanana::Draw(Camera *camera) {
    render_actor_banana(camera, NULL, (BananaActor*)this);
}
void ABanana::Collision() { }
void ABanana::Destroy() { }

//void ABanana::Held() {}
