#include "RaceManager.h"

#include "AllActors.h"
#include "World.h"
#include "port/Game.h"
#include "engine/editor/Editor.h"
#include "engine/editor/SceneManager.h"

extern "C" {
#include "render_courses.h"
}

RaceManager::RaceManager(World& world) : WorldContext(world) {
}

void RaceManager::Load() {
    if (WorldContext.CurrentCourse) {
        WorldContext.CurrentCourse->Load();
    }
}

void RaceManager::PreInit() {
    // Ruleset options
    if (CVarGetInteger("gDisableItemboxes", false) == true) {
        gPlaceItemBoxes = false;
    } else {
        gPlaceItemBoxes = true;
    }
}

void RaceManager::BeginPlay() {
    auto course = WorldContext.CurrentCourse;

    if (course) {
        // Do not spawn finishline in credits or battle mode. And if bSpawnFinishline.
        if ((gGamestate != CREDITS_SEQUENCE) && (gModeSelection != BATTLE)) {
            if (course->bSpawnFinishline) {
                if (course->FinishlineSpawnPoint.has_value()) {
                    AFinishline::Spawn(course->FinishlineSpawnPoint.value(), IRotator(0, 0, 0));
                } else {
                    AFinishline::Spawn();
                }
            
            }
        }
        gEditor.AddLight("Sun", nullptr, D_800DC610[1].l->l.dir);

        course->BeginPlay();
    }
}

void RaceManager::PostInit() {
    // Ruleset options
    if (CVarGetInteger("gAllThwompsAreMarty", false) == true) {
        for (auto object : gWorldInstance.Objects) {
            if (OThwomp* thwomp = dynamic_cast<OThwomp*>(object)) {
                gObjectList[thwomp->_objectIndex].unk_0D5 = OThwomp::States::JAILED; // Sets all the thwomp behaviour flags to marty
                thwomp->Behaviour = OThwomp::States::JAILED;
            }
        }
    }

    if (CVarGetInteger("gAllBombKartsChase", false) == true) {
        for (auto object : gWorldInstance.Objects) {
            if (OBombKart* kart = dynamic_cast<OBombKart*>(object)) {
                kart->Behaviour = OBombKart::States::CHASE;
            }
        }
    }

    if (CVarGetInteger("gGoFish", false) == true) {
        OTrophy::Spawn(FVector(0,0,0), OTrophy::TrophyType::GOLD, OTrophy::Behaviour::GO_FISH);
    }
}

void RaceManager::Clean() {
}
