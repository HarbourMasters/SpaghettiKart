#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H

//#include <vector>
//#include <memory>

#ifdef __cplusplus
#include "GameActor.h"
extern "C" {
#endif

typedef struct ActorManager ActorManager_t;

void ActorManager_AddActor();
void ActorManager_UpdateActors();
void ActorManager_RenderActors(Camera*);
void ActorManager_RemoveExpiredActors();


#ifdef __cplusplus
}
#endif

#endif // ACTOR_MANAGER_H