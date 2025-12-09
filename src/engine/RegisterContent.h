#pragma once

#include "Registry.h"
#include "SpawnParams.h"

void RegisterActors(Registry<const SpawnParams&>& r);
void RegisterTracks(Registry<>& r);
