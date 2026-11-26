#pragma once

#include <string>

void InitModsSystem();
void UnloadMods();
// True if the mk64.o2r at this path holds the mk64-assets version this build needs: the same check that stops the
// game with "Dependency Issues" at startup. An old or unreadable archive is false.
bool IsMK64O2RCurrent(const std::string& path);
