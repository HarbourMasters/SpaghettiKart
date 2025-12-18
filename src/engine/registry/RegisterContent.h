#pragma once

#include "Registry.h"
#include "DataRegistry.h"
#include "src/port/Game.h"
#include "engine/objects/Object.h"
#include "engine/Actor.h"
#include "engine/SpawnParams.h"

template<class T> static OObject* AddObjectToWorld(const SpawnParams& params) {
    return GetWorld()->AddObject(std::make_unique<T>(params));
}

template<class T> static AActor* AddActorToWorld(const SpawnParams& params) {
    return GetWorld()->AddActor(std::make_unique<T>(params));
}

void RegisterTracks(Registry<TrackInfo>& r);
void RegisterActors(Registry<ActorInfo, const SpawnParams&>& r);
void RegisterItems(Registry<ItemInfo>& r);
void RegisterItemTables(DataRegistry<RandomItemTable>& r);
