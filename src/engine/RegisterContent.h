#pragma once

#include "Registry.h"
#include "SpawnParams.h"

void RegisterActors(Registry<ActorInfo, const SpawnParams&>& r);
void RegisterTracks(Registry<TrackInfo>& r);
