#include "Rulesets.h"
#include "objects/Thwomp.h"
#include "objects/Trophy.h"
#include "objects/BombKart.h"
#include "actors/Text.h"

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
        for (auto& object : GetWorld()->Objects) {
            if (OThwomp* thwomp = dynamic_cast<OThwomp*>(object.get())) {
                gObjectList[thwomp->_objectIndex].unk_0D5 = OThwomp::States::JAILED; // Sets all the thwomp behaviour flags to marty
                thwomp->Behaviour = OThwomp::States::JAILED;
            }
        }
    }

    if (CVarGetInteger("gAllBombKartsChase", false) == true) {
        for (auto& object : GetWorld()->Objects) {
            if (OBombKart* kart = dynamic_cast<OBombKart*>(object.get())) {
                kart->Behaviour = OBombKart::States::CHASE;
            }
        }
    }

    if (CVarGetInteger("gGoFish", false) == true) {
        OTrophy::Spawn(FVector(0,0,0), OTrophy::TrophyType::GOLD, OTrophy::Behaviour::GO_FISH);
    }

    if (CVarGetInteger("gPlayerNames", false) == true) {

        std::string playerNames[NUM_PLAYERS] = {
            "Player 1", "Player 2", "Player 3",
            "Player 4", "Player 5", "Player 6",
            "Player 7", "Player 8"
        };

        for (size_t i = 0; i < NUM_PLAYERS; i++) {
            //                         text,               pos,                 scale,                           mode,            playerIndex
            AText* text = AText::Spawn(playerNames[i], FVector(0, 0, 0), FVector(0.15f, 0.15f, 0.15f), AText::TextMode::FOLLOW_PLAYER, i);
            text->ScaleX = 1.0f;
            text->Scale.x = 0.15f;
            text->Scale.y = 0.15f;
            text->Scale.z = 0.15f;
            text->Animate = false; // Cycle between colours similar to grand prix title text

            // White
            for (size_t j = 0; j < 4; j++) {
                text->TextColour[j] = {255, 255, 255, 255};
            }

        }
    }
}
