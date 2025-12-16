#include <libultraship.h>

#include "Banana.h"
#include "engine/Actor.h"

extern "C" {
#include "macros.h"
#include "actor_types.h"
void update_actor_banana(struct BananaActor*);
void render_actor_banana(Camera*, float[4][4], struct BananaActor*);
}

ABanana::ABanana(const SpawnParams& params) : AActor(params) {
    Name = "Banana";
    ResourceName = "mk:banana";

    FVector pos = params.Location.value_or(FVector(0, 0, 0));
    Pos[0] = pos.x; Pos[1] = pos.y; Pos[2] = pos.z;

    FVector vel = params.Velocity.value_or(FVector(0, 0, 0));
    Velocity[0] = vel.x; Velocity[1] = vel.y; Velocity[2] = vel.z;

    Type = 6; // ACTOR_BANANA
    Flags = -0x8000;
    Unk_04 = 20;
    State = HELD_BANANA;

    PlayerId = 0; // Don't remember why this is here
    
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
void ABanana::Collision(Player* player, AActor*) { }
void ABanana::Destroy() { }

//void ABanana::Held() {}
