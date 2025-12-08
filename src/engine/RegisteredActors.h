#pragma once

#include "Registry.h"
#include "SpawnParams.h"

void RegisterGameActors(Registry<SpawnParams&>& r);
void RegisterTracks(Registry<>& r);
