#include "ContentBrowser.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"
#include "port/Engine.h"

#include <imgui.h>
#include <map>
#include <filesystem>
#include <iostream>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>
#include "CoreMath.h"
#include "World.h"
#include "AllActors.h"

namespace Editor {

    ContentBrowserWindow::~ContentBrowserWindow() {
        SPDLOG_TRACE("destruct content browser window");
    }

    void ContentBrowserWindow::DrawElement() {
        static bool refresh = true;
        static bool actorContent = false;
        static bool objectContent = false;
        static bool customContent = false;

        // List the available mods/o2r files
        // for (size_t i = 0; i < GameEngine::Instance->archiveFiles.size(); i++) {
        //     auto str = GameEngine::Instance->archiveFiles[i];
        //     if (str.starts_with("./mods/")) {
        //         ImGui::Text(GameEngine::Instance->archiveFiles[i].c_str());
        //     }
        // }

        if (ImGui::Button(ICON_FA_REFRESH)) {
            refresh = true;
        }

        // Query content in o2r and add them to Content
        if (refresh) {
            refresh = false;
            Content.clear();
            FindContent();
            return;
        }


        std::string actorText = fmt::format("{0} {1}", actorContent ? ICON_FA_FOLDER_OPEN_O : ICON_FA_FOLDER_O, "Actors");
        if (ImGui::Button(actorText.c_str(), ImVec2(80, 32))) {
            actorContent = !actorContent;
            objectContent = false;
            customContent = false;
        }

        std::string objectText = fmt::format("{0} {1}", objectContent ? ICON_FA_FOLDER_OPEN_O : ICON_FA_FOLDER_O, "Objects");
        if (ImGui::Button(objectText.c_str(), ImVec2(80, 32))) {
            objectContent = !objectContent;
            actorContent = false;
            customContent = false;
        }

        std::string customText = fmt::format("{0} {1}", customContent ? ICON_FA_FOLDER_OPEN_O : ICON_FA_FOLDER_O, "Custom");
        if (ImGui::Button(customText.c_str(), ImVec2(80, 32))) {
            customContent = !customContent;
            actorContent = false;
            objectContent = false;
        }

        if (actorContent) {
            AddActorContent();
        }

        if (objectContent) {
            AddObjectContent();
        }

        if (customContent) {
            AddCustomContent();
        }
    }

    std::unordered_map<std::string, std::function<AActor*(const FVector&)>> ActorList = {
        { "Mario Sign", [](const FVector& pos) { return new AMarioSign(pos); } },
        { "Wario Sign", [](const FVector& pos) { return new AWarioSign(pos); } },
        { "Cloud", [](const FVector& pos) { return new ACloud(pos); } },
        { "Finishline", [](const FVector& pos) { return new AFinishline(pos); } },
        { "Ghostship", [](const FVector& pos) { return new AShip(pos, AShip::Skin::GHOSTSHIP); } },
        { "Ship_1", [](const FVector& pos) { return new AShip(pos, AShip::Skin::SHIP2); } },
        { "Ship_2", [](const FVector& pos) { return new AShip(pos, AShip::Skin::SHIP3); } },
        { "SpaghettiShip", [](const FVector& pos) { return new ASpaghettiShip(pos); } },
        { "Starship", [](const FVector& pos) { return new AStarship(pos); } },
        { "Train", [](const FVector& pos) { return new ATrain(ATrain::TenderStatus::HAS_TENDER, 4, 2.5f, 0); } },
        { "Boat", [](const FVector& pos) { return new ABoat((0.6666666f)/4, 0); } },
        { "Bus", [](const FVector& pos) { return new ABus(2.0f, 2.5f, &D_80164550[0][0], 0); } },
        { "Car", [](const FVector& pos) { return new ACar(2.0f, 2.5f, &D_80164550[0][0], 0); } },
        { "Truck", [](const FVector& pos) { return new ATruck(2.0f, 2.5f, &D_80164550[0][0], 0); } },
        { "Tanker Truck", [](const FVector& pos) { return new ATankerTruck(2.0f, 2.5f, &D_80164550[0][0], 0); } },
    };

    std::unordered_map<std::string, std::function<OObject*(const FVector&)>> ObjectList = {
        { "Bat", [](const FVector& pos) { return new OBat(pos, IRotator(0, 0, 0)); } },
        { "Bomb Kart", [](const FVector& pos) { return new OBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f); } },
        // { "Boos", [](const FVector& pos) { return new OBoos(pos, &D_80164550[0][0], 0, 0, 0.8333333f); } },
        { "CheepCheep", [](const FVector& pos) { return new OCheepCheep(pos, OCheepCheep::CheepType::RACE, IPathSpan(0, 10)); } },
        { "Crab", [](const FVector& pos) { return new OCrab(FVector2D(0, 10), FVector2D(20, 10)); } },
        { "ChainChomp", [](const FVector& pos) { return new OChainChomp(); } },
        { "Flagpole", [](const FVector& pos) { return new OFlagpole(pos, 0); } },
        { "Hedgehog", [](const FVector& pos) { return new OHedgehog(pos, FVector2D(0, 10), 0); } },
        { "HotAirBalloon", [](const FVector& pos) { return new OHotAirBalloon(pos); } },
        { "Lakitu", [](const FVector& pos) { return new OLakitu(0, OLakitu::LakituType::STARTER); } },
        // { "Mole", [](const FVector& pos) { return new OMole(pos, ); } }, // <-- Needs a group
        { "Chick Penguin", [](const FVector& pos) { return new OPenguin(pos, 0, OPenguin::PenguinType::CHICK, OPenguin::Behaviour::SLIDE3); } },
        { "Penguin", [](const FVector& pos) { return new OPenguin(pos, 0, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE); } },
        { "Emperor Penguin", [](const FVector& pos) { return new OPenguin(pos, 0, OPenguin::PenguinType::EMPEROR, OPenguin::Behaviour::STRUT); } },
        { "Seagull", [](const FVector& pos) { return new OSeagull(pos); } },
        { "Thwomp", [](const FVector& pos) { return new OThwomp(pos.x, pos.z, 0, 1.0f, 0, 0, 2.0f); } },
        { "Trashbin", [](const FVector& pos) { return new OTrashBin(pos, IRotator(0, 0, 0), 1.0f, OTrashBin::Behaviour::MUNCHING); } },
        { "Trophy", [](const FVector& pos) { return new OTrophy(pos, OTrophy::TrophyType::GOLD_150, OTrophy::Behaviour::ROTATE2); } },
        { "Snowman", [](const FVector& pos) { return new OSnowman(pos); } },
        { "Podium", [](const FVector& pos) { return new OPodium(pos); } },
    };

    void ContentBrowserWindow::AddActorContent() {
        FVector pos = GetPositionAheadOfCamera(300.0f);

        size_t i_actor = 0;
        for (const auto& actor : ActorList) {
            if ((i_actor != 0) && (i_actor % 10 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", actor.first, i_actor);
            if (ImGui::Button(label.c_str())) {
                gWorldInstance.AddActor(actor.second(pos));  // Pass pos to the lambda
            }
            i_actor += 1;
        }
    }

    void ContentBrowserWindow::AddObjectContent() {
        FVector pos = GetPositionAheadOfCamera(300.0f);

        size_t i_object = 0;
        for (auto& object : ObjectList) {
            if ((i_object != 0) && (i_object % 10 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", object.first, i_object);
            if (ImGui::Button(label.c_str())) {
                gWorldInstance.AddObject(object.second(pos));  // Pass pos to the lambda
            }
            i_object += 1;
        }
    }

    void ContentBrowserWindow::AddCustomContent() {
        size_t i_custom = 0;
        for (const auto& file : Content) {
            if ((i_custom != 0) && (i_custom % 10 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", file, i_custom);
            if (ImGui::Button(label.c_str())) {
                int coll;
                //printf("ContentBrowser.cpp: name: %s\n", test.c_str());
                std::string name = file.substr(file.find_last_of('/') + 1);
                auto actor = gWorldInstance.AddStaticMeshActor(name, FVector(50, 100, 0), IRotator(0, 90, 0), FVector(1, 1, 1), "__OTR__" + file, &coll);
                // This is required because ptr gets cleaned up.
                actor->Model = "__OTR__" + file;

            }
            i_custom += 1;
        }
    }

    void ContentBrowserWindow::FindContent() {
        std::list<std::string> myList = {"tracks/*", "actors/*", "objects/*"};
        std::list<std::string> myList2 = {""};


        auto ptr = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->ListFiles(myList, myList2);
        if (ptr) {
            auto files = *ptr;
            for (const auto& file : files) {
                if (file.find("/mat_") != std::string::npos) {
                    continue;
                } else if (file.size() >= 6 && file.substr(file.size() - 6, 5) == "_tri_" && isdigit(file.back())) {
                    // ends with _tri_#
                    continue;
                } else if (file.find("_vtx_") != std::string::npos) {
                    // Has _vtx_
                    continue;
                } else if (file.find('.') != std::string::npos) {
                    // File has an extension
                    continue;
                }

                Content.push_back(file);
            }
        }
    }
}
