#pragma once

#include "Registry.h"
#include "SpawnParams.h"

void RegisterActors(Registry<const ActorInfo, const SpawnParams&>& r);
void RegisterTracks(Registry<const TrackInfo>& r);
