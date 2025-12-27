#include "RaceManager.h"

#include "AllActors.h"
#include "World.h"
#include "port/Game.h"
#include "engine/editor/Editor.h"
#include "engine/editor/SceneManager.h"
#include "engine/RandomItemTable.h"

extern "C" {
#include "render_courses.h"
}

RaceManager::RaceManager(World& world) : WorldContext(world) {
}

std::unordered_map<uintptr_t, std::shared_ptr<Vtx>> mirroredVtxCache;

// Populates a collision mesh for mirror mode
extern "C" void add_triangle_to_collision_mesh(Vtx* vtx1, Vtx* vtx2, Vtx* vtx3, Vtx** outVtx1, Vtx** outVtx2, Vtx** outVtx3) {
    if (gIsMirrorMode != 0) {
        auto getOrCreateMirrored = [](Vtx* original) -> Vtx* {
            uintptr_t key = reinterpret_cast<uintptr_t>(original);
            
            auto it = mirroredVtxCache.find(key);
            if (it != mirroredVtxCache.end()) {
                return it->second.get();
            }

            auto newVtx = std::make_shared<Vtx>(*original);
            newVtx->v.ob[0] = -newVtx->v.ob[0];

            mirroredVtxCache[key] = newVtx;

            return newVtx.get();
        };

        Vtx* m1 = getOrCreateMirrored(vtx1);
        Vtx* m2 = getOrCreateMirrored(vtx2);
        Vtx* m3 = getOrCreateMirrored(vtx3);

        // don't invert winding here, already done in the gfx
        *outVtx1 = m1;
        *outVtx2 = m2;
        *outVtx3 = m3;

    } else {
        // Pas de miroir, on passe les originaux
        *outVtx1 = vtx1;
        *outVtx2 = vtx2;
        *outVtx3 = vtx3;
    }
}

void RaceManager::Load() {
    auto* track = GetWorld()->GetTrack();
    if (track) {
        mirroredVtxCache.clear();
        track->Load();
    } else {
        printf("[RaceManager] [Load] Track was nullptr\n");
    }
}

void RaceManager::PreInit() {
    // Ruleset options
    if (CVarGetInteger("gDisableItemboxes", false) == true) {
        gPlaceItemBoxes = false;
    } else {
        gPlaceItemBoxes = true;
    }

    RaceManager::SetItemTables();
}

void RaceManager::BeginPlay() {
    auto track = WorldContext.GetTrack();

    if (track) {
        // Do not spawn finishline in credits or battle mode. And if bSpawnFinishline.
        if ((gGamestate != CREDITS_SEQUENCE) && (gModeSelection != BATTLE)) {
            if (track->bSpawnFinishline) {
                if (track->FinishlineSpawnPoint.has_value()) {
                    AFinishline::Spawn(track->FinishlineSpawnPoint.value(), IRotator(0, 0, 0));
                } else {
                    AFinishline::Spawn();
                }
            
            }
        }
        gEditor.AddLight("Sun", nullptr, D_800DC610[1].l->l.dir);

        track->BeginPlay();
    }
}

void RaceManager::PostInit() {
    // Ruleset options
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
}

void RaceManager::SetItemTables() {
    std::optional<std::string> humanTableName;
    std::optional<std::string> cpuTableName;

    switch(gModeSelection) {
        case GRAND_PRIX:
            if (CVarGetInteger("gHarderCPU", false) == true) {
                humanTableName = "mk:hard_cpu_grand_prix";
                cpuTableName = "mk:hard_cpu_grand_prix";
            } else { // normal gameplay
                humanTableName = "mk:human_grand_prix";
                cpuTableName = "mk:cpu_grand_prix";
            }
            break;
        case VERSUS:
            switch (gPlayerCountSelection1) {
                case TWO_PLAYERS_SELECTED:
                    humanTableName = "mk:versus_2p";
                    break;
                case THREE_PLAYERS_SELECTED:
                    humanTableName = "mk:versus_3p";
                    break;
                case FOUR_PLAYERS_SELECTED:
                    humanTableName = "mk:versus_4p";
                    break;
            }
            break;
        case BATTLE:
            humanTableName = "mk:battle";
            break;
    }
    if (humanTableName.has_value()) {
        mHumanItemTable = gItemTableRegistry.Get(humanTableName.value());
    } else {
        mHumanItemTable = nullptr;
    }
    if (cpuTableName.has_value()) {
        mCPUItemTable = gItemTableRegistry.Get(cpuTableName.value());
    } else {
        mCPUItemTable = nullptr;
    }
    printf("[RaceManager] Selected human item probability table %s\n", humanTableName.value_or("none").c_str());
    printf("[RaceManager] Selected cpu item probability table %s\n", cpuTableName.value_or("none").c_str());
}

extern "C" int16_t RaceManager_GetRandomHumanItem(uint32_t rank) {
    auto& raceManager = GetWorld()->GetRaceManager();

    auto* table = raceManager.GetHumanItemTable();
    if (nullptr == table) {
        printf("[RaceManager_GetRandomHumanItem] Item table nullptr, giving player a none item\n");
        return ITEM_NONE;
    }

    return table->Roll(rank);
}

extern "C" int16_t RaceManager_GetRandomCPUItem(uint32_t rank) {
    auto& raceManager = GetWorld()->GetRaceManager();
    auto* table = raceManager.GetCPUItemTable();
    if (nullptr == table) {
        printf("[RaceManager_GetRandomCPUItem] Item table nullptr, giving player a none item\n");
        return ITEM_NONE;
    }

    return table->Roll(rank);
}
