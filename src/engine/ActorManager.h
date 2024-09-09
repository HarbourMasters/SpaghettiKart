#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H

//#include <vector>
//#include <memory>

#ifdef __cplusplus
#include "GameActor.h"
#include "structs.h"
extern "C" {
#endif

typedef struct ActorManager ActorManager_t;

void ActorManager_AddActor();
void ActorManager_UpdateActors();
void ActorManager_RenderActors(Camera*);
void ActorManager_RemoveExpiredActors();
struct BananaActor *ActorManager_SpawnBanana(uint16_t playerId, const float startingPos[3], const s16 startingRot[3], const float startingVelocity[3]);
struct Actor* ActorManager_GetActorByIndex(size_t index);


#ifdef __cplusplus
}
#endif

#endif // ACTOR_MANAGER_H