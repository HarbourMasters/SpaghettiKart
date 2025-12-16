#pragma once

#include "Object.h"
#include "Registry.h"
#include "SpawnParams.h"
#include "port/Game.h"
#include <memory>

template<class T> static OObject* AddObjectToWorld(const SpawnParams& params) {
    return GetWorld()->AddObject(std::make_unique<T>(params));
}

template<class T> static AActor* AddActorToWorld(const SpawnParams& params) {
    return GetWorld()->AddActor(std::make_unique<T>(params));
}

void RegisterActors(Registry<ActorInfo, const SpawnParams&>& r);
void RegisterTracks(Registry<TrackInfo>& r);
