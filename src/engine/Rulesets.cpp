#include "Rulesets.h"
#include "objects/Thwomp.h"
#include "objects/Trophy.h"

extern "C" {
#include "code_800029B0.h"
#include "memory.h"
}

// Before the game is loaded, and before vertices and displaylists are unpacked.
// Only runs a single time at the beginning of a track.
void Rulesets::PreLoad() {
}

// Just before BeginPlay() (used to spawn actors) is ran.
// Only runs a single time at the beginning of a track.
void Rulesets::PreInit() {
    if (CVarGetInteger("gDisableItemboxes", false) == true) {
        gPlaceItemBoxes = false;
    } else {
        gPlaceItemBoxes = true;
    }
}

// Just after BeginPlay() is ran.
// Only runs a single time at the beginning of a track.
void Rulesets::PostInit() {
    if (CVarGetInteger("gAllThwompsAreMarty", false) == true) {
        for (auto object : gWorldInstance.Objects) {
            if (OThwomp* thwomp = dynamic_cast<OThwomp*>(object)) {
                gObjectList[thwomp->_objectIndex].unk_0D5 = OThwomp::States::JAILED; // Sets all the thwomp behaviour flags to marty
                thwomp->State =  OThwomp::States::JAILED;
            }
        }
    }

    if (CVarGetInteger("gAllBombKartsChase", false) == true) {
        for (auto object : gWorldInstance.Objects) {
            if (OBombKart* kart = dynamic_cast<OBombKart*>(object)) {
                kart->State = OBombKart::States::CHASE;
            }
        }
    }

    if (CVarGetInteger("gGoFish", false) == true) {
        gWorldInstance.AddObject(new OTrophy(FVector(0,0,0), OTrophy::TrophyType::GOLD, OTrophy::Behaviour::GO_FISH));
    }
}
