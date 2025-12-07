#include "ContentBrowser.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"
#include "port/Engine.h"
#include "SpawnParams.h"

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
#include "port/Game.h"
#include "src/engine/editor/SceneManager.h"

#include "World.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "collision.h"
}

namespace Editor {
    bool bIsTrainWindowOpen = false; // Global because member variables do not work in lambdas

    ContentBrowserWindow::~ContentBrowserWindow() {
        SPDLOG_TRACE("destruct content browser window");
    }

    void ContentBrowserWindow::DrawElement() {
        if (ImGui::Button(ICON_FA_REFRESH)) {
            Refresh = true;
        }

        // Query content in o2r and add them to Content
        if (Refresh) {
            Refresh = false;
            RemoveCustomTracksFromTrackList();
            Tracks.clear();
            Content.clear();
            FindTracks();
            FindContent();
            return;
        }

        ImGui::BeginChild("LeftPanel", ImVec2(120, 0), true, ImGuiWindowFlags_None);

        ContentBrowserWindow::FolderButton("Tracks", TrackContent);
        ContentBrowserWindow::FolderButton("Actors", ActorContent);
        ContentBrowserWindow::FolderButton("Objects", ObjectContent);
        ContentBrowserWindow::FolderButton("Custom", CustomContent);
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true, ImGuiWindowFlags_None);
        if (TrackContent) {
            AddTrackContent();
        }

        if (ActorContent) {
            AddActorContent();
        }

        if (ObjectContent) {
            AddObjectContent();
        }

        if (CustomContent) {
            AddCustomContent();
        }
        ImGui::EndChild();
    }

    void ContentBrowserWindow::FolderButton(const char* label, bool& contentFlag, const ImVec2& size) {
        std::string buttonText = fmt::format("{0} {1}", contentFlag ? ICON_FA_FOLDER_OPEN_O : ICON_FA_FOLDER_O, label);
        if (ImGui::Button(buttonText.c_str(), size)) {
            TrackContent = false;
            ActorContent = false;
            ObjectContent = false;
            CustomContent = false;
            contentFlag = !contentFlag;
        }
    }

    // For C-actors
    std::unordered_map<std::string, std::function<void(const FVector&)>> CActorList = {
        { "Item Box", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_ITEM_BOX);
            s32 height = spawn_actor_on_surface(position[0], position[1] + 10.0f, position[2]);

            Actor* actor = CM_GetActor(id);
            actor->unk_08 = height;
            actor->velocity[0] = position[1];
            actor->pos[1] = height - 20.0f;

        }},
        { "Fake Item Box", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_FAKE_ITEM_BOX);

            Actor* actor = CM_GetActor(id);
            actor->state = 1;
            actor->velocity[1] = position[1];
        }},
        { "Yoshi Egg", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_YOSHI_EGG);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Piranha Plant", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_PIRANHA_PLANT);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},

        { "Tree (Mario Raceway)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Yoshi Valley)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_YOSHI_VALLEY);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Royal Raceway)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_ROYAL_RACEWAY);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Moo Moo Farm)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_MOO_MOO_FARM);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Palm Tree", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_PALM_TREE);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Luigi Raceway)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_LUIGI_RACEWAY);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Unknown Plant (0x1B)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_UNKNOWN_0x1B);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Peach's Castle)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_PEACH_CASTLE);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Tree (Frappe Snowland)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_TREE_FRAPPE_SNOWLAND);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Cactus 1 (Kalamari Desert)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_CACTUS1_KALAMARI_DESERT);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Cactus 2 (Kalamari Desert)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_CACTUS2_KALAMARI_DESERT);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Cactus 3 (Kalamari Desert)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_CACTUS3_KALAMARI_DESERT);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
        { "Bush (Bowser's Castle)", [](const FVector& pos) {
            Vec3f position = {pos.x, pos.y, pos.z};
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            s32 id = add_actor_to_empty_slot(position, rot, vel, ACTOR_BUSH_BOWSERS_CASTLE);
            s32 height = spawn_actor_on_surface(position[0], position[1], position[2]);
        }},
    };

    std::unordered_map<std::string, std::function<AActor*(const FVector&)>> ActorList = {
        // The banana gets attached to a player. This needs to be disconnected if this should be used in the editor
//        { "Banana", [](const FVector& pos) { return gWorldInstance.AddActor(new ABanana(SpawnParams{.Name = "mk:banana", .Location = pos})); } },
        { "Falling Rock", [](const FVector& pos) { return AFallingRock::Spawn(pos, 80); } },
        { "Mario Sign", [](const FVector& pos) { return AMarioSign::Spawn(pos, IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f)); } },
        { "Wario Sign", [](const FVector& pos) { return AWarioSign::Spawn(pos, IRotator(0, 0, 0), FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f)); } },
        { "Cloud", [](const FVector& pos) { return ACloud::Spawn(pos, 500, 3.0f, 200.0f); } },
        { "Finishline", [](const FVector& pos) { return AFinishline::Spawn(pos, IRotator(0, 0, 0)); } },
        { "Ghostship", [](const FVector& pos) { return AShip::Spawn(pos, IRotator(0, 0, 0), FVector(0.4f, 0.4f, 0.4f), AShip::Skin::GHOSTSHIP); } },
        { "Ship 1", [](const FVector& pos) { return AShip::Spawn(pos, IRotator(0, 0, 0), FVector(0.4f, 0.4f, 0.4f), AShip::Skin::SHIP2); } },
        { "Ship 2", [](const FVector& pos) { return AShip::Spawn(pos, IRotator(0, 0, 0), FVector(0.4f, 0.4f, 0.4f), AShip::Skin::SHIP3); } },
        { "SpaghettiShip", [](const FVector& pos) { return ASpaghettiShip::Spawn(pos, IRotator(0, 0, 0), FVector(0.4f, 0.4f, 0.4f)); } },
        { "Starship", [](const FVector& pos) { return AStarship::Spawn(pos, IRotator(0, 0, 0), FVector(1.5f, 1.5f, 1.5f), 0.01f, 150.0f); } },
        { "Train", [](const FVector& pos) { bIsTrainWindowOpen = true; return nullptr;  } },
        { "Boat", [](const FVector& pos) { return ABoat::Spawn((0.6666666f)/4, 0, 0, ABoat::SpawnMode::AUTO); } },
        { "Bus", [](const FVector& pos) { return ABus::Spawn(2.0f, 2.5f, 0, 0, ABus::SpawnMode::AUTO); } },
        { "Car", [](const FVector& pos) { return ACar::Spawn(2.0f, 2.5f, 0, 0, ACar::SpawnMode::AUTO); } },
        { "Truck", [](const FVector& pos) { return ATruck::Spawn(2.0f, 2.5f, 0, 0, ATruck::SpawnMode::AUTO); } },
        { "Tanker Truck", [](const FVector& pos) { return ATankerTruck::Spawn(2.0f, 2.5f, 0, 0, ATankerTruck::SpawnMode::AUTO); } },
        { "Text", [](const FVector& pos) { return AText::Spawn("Harbour Masters", FVector(0, 0, 0), FVector(1.0f, 1.0f, 1.0f), AText::TextMode::STATIONARY, 0); } },
    };

    std::unordered_map<std::string, std::function<OObject*(const FVector&)>> ObjectList = {
        { "Bat", [](const FVector& pos) { return OBat::Spawn(pos, IRotator(0, 0, 0)); } },
        { "Bomb Kart", [](const FVector& pos) { return OBombKart::Spawn(pos, 1, 0.8333333f); } },
        { "Boos", [](const FVector& pos) { return OBoos::Spawn(5, IPathSpan(0, 50), IPathSpan(60, 90), IPathSpan(100, 140)); } },
        { "Cheep Cheep", [](const FVector& pos) { return OCheepCheep::Spawn(pos, OCheepCheep::Behaviour::RACE, IPathSpan(0, 10)); } },
        { "Crab", [](const FVector& pos) { return OCrab::Spawn(FVector2D(pos.x, pos.z), FVector2D(pos.x + 100, pos.z + 100)); } },

        // Animation crash
        // { "Chain Chomp", [](const FVector& pos) { return gWorldInstance.AddObject(new OChainChomp()); } },
        { "Flagpole", [](const FVector& pos) { return OFlagpole::Spawn(pos, 0); } },
        { "Hedgehog", [](const FVector& pos) { return OHedgehog::Spawn(pos, FVector2D(0, 10), 0); } },
        { "Hot Air Balloon", [](const FVector& pos) { return OHotAirBalloon::Spawn(pos); } },
        { "Lakitu", [](const FVector& pos) { return new OLakitu(0, OLakitu::LakituType::STARTER); } },
        // { "Mole", [](const FVector& pos) { return new OMole(pos, ); } }, // <-- Needs a group
        { "Penguin", [](const FVector& pos) { return OPenguin::Spawn(pos, 0x150, 0, 100.0f, OPenguin::PenguinType::ADULT, OPenguin::Behaviour::CIRCLE); } },
        { "Seagull", [](const FVector& pos) { return OSeagull::Spawn(pos); } },
        { "Thwomp", [](const FVector& pos) { return OThwomp::Spawn(pos.x, pos.z, 0, 1.0f, 1, 0, 7); } },
        { "Trash Bin", [](const FVector& pos) { return OTrashBin::Spawn(pos, IRotator(0, 0, 0), 1.0f, OTrashBin::Behaviour::MUNCHING); } },
        { "Trophy", [](const FVector& pos) { return OTrophy::Spawn(pos, OTrophy::TrophyType::GOLD_150, OTrophy::Behaviour::ROTATE2); } },
        { "Snowman", [](const FVector& pos) { return OSnowman::Spawn(pos); } },
        { "Podium", [](const FVector& pos) { return OPodium::Spawn(pos); } },
        { "Balloons", [](const FVector& pos) { return OGrandPrixBalloons::Spawn(pos); } },
    };

    void ContentBrowserWindow::AddTrackContent() {
        size_t i_track = 0;
        for (auto& track : Tracks) {
            if (!track.SceneFile.empty()) { // has scene file
                std::string label = fmt::format("{}##{}", track.Name, i_track);
                if (ImGui::Button(label.c_str())) {
                    gWorldInstance.SetCurrentCourse(track.course);
                    gGamestateNext = RACING;
                    SetSceneFile(track.Archive, track.SceneFile);
                    break;
                }
            } else { // no scene file
                std::string label = fmt::format("{} {}", ICON_FA_EXCLAMATION_TRIANGLE, track.Name);
                if (ImGui::Button(label.c_str())) {
                    track.SceneFile = track.Dir + "/scene.json";
                    gWorldInstance.SetCurrentCourse(track.invalidTrack);
                    SetSceneFile(track.Archive, track.SceneFile);
                    SaveLevel();
                    Refresh = true;
                }
            }

            i_track += 1;
        }
    }

    // When resetting the known content, we need to also pop the custom courses
    // out of World::Courses vector. Otherwise, duplicate courses would show up for users.
    void ContentBrowserWindow::RemoveCustomTracksFromTrackList() {
        for (auto& track : Tracks) {
            auto it = gWorldInstance.Courses.begin();
            while (it != gWorldInstance.Courses.end()) {
                if (track.course.get() == it->get()) {
                    it = gWorldInstance.Courses.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    void ContentBrowserWindow::AddActorContent() {
        FVector pos = GetPositionAheadOfCamera(300.0f);

        size_t i_actor = 0;
        for (const auto& actor : CActorList) {
            if ((i_actor != 0) && (i_actor % 8 == 0)) {
            } else {
                ImGui::SameLine();
            }
            std::string label = fmt::format("{}##{}", actor.first, i_actor);
            if (ImGui::Button(label.c_str())) {
                actor.second(pos);
            }
            
            i_actor += 1;
        }

        for (const auto& actor : ActorList) {
            if ((i_actor != 0) && (i_actor % 8 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", actor.first, i_actor);
            if (ImGui::Button(label.c_str())) {
                //gWorldInstance.AddActor(
                actor.second(pos);
            }
            i_actor += 1;
        }

        ContentBrowserWindow::TrainWindow();
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
                object.second(pos);
            }
            i_object += 1;
        }
    }

    void ContentBrowserWindow::AddCustomContent() {
        FVector pos = GetPositionAheadOfCamera(300.0f);

        size_t i_custom = 0;
        for (const auto& file : Content) {
            if ((i_custom != 0) && (i_custom % 5 == 0)) {
            } else {
                ImGui::SameLine();
            }

            std::string label = fmt::format("{}##{}", file, i_custom);
            if (ImGui::Button(label.c_str())) {
                int coll;
                //printf("ContentBrowser.cpp: name: %s\n", test.c_str());
                std::string name = file.substr(file.find_last_of('/') + 1);
                auto actor = gWorldInstance.AddStaticMeshActor(name, FVector(pos), IRotator(0, 0, 0), FVector(1, 1, 1), "__OTR__" + file, &coll);
                // This is required because ptr gets cleaned up.
                actor->Model = "__OTR__" + file;

            }
            i_custom += 1;
        }
    }

    // Finds modded archives only. For discovering tracks
    void ContentBrowserWindow::FindTracks() {
        auto manager = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager();

        auto ptr2 = manager->ListDirectories("tracks/*");
        if (ptr2) {
            auto dirs = *ptr2;

            for (const std::string& dir : dirs) {
                std::string name = dir.substr(dir.find_last_of('/') + 1);
                std::string sceneFile = dir + "/scene.json";
                std::string minimapFile = dir + "/minimap.png";
                // The track has a valid scene file
                if (manager->HasFile(sceneFile)) {
                    auto archive = manager->GetArchiveFromFile(sceneFile);
                    
                    auto course = std::make_shared<Course>();
                    course->RootArchive = archive;
                    course->LoadO2R(dir);
                    LoadLevel(course.get(), sceneFile);
                    LoadMinimap(course.get(), minimapFile);
                    Tracks.push_back({nullptr, course, sceneFile, name, dir, archive});
                    gWorldInstance.Courses.push_back(std::move(course));
                } else { // The track does not have a valid scene file
                    const std::string file = dir + "/data_track_sections";

                    // If the track has a data_track_sections file,
                    // then it must at least be a valid track.
                    // So lets add it as an uninitialized track.
                    if (manager->HasFile(file)) {

                        auto course = std::make_shared<Course>();
                        course->Id = (std::string("mods:") + name).c_str();
                        course->Props.SetText(course->Props.Name, name.c_str(), sizeof(course->Props.Name));
                        course->Props.SetText(course->Props.DebugName, name.c_str(), sizeof(course->Props.Name));
                        auto archive = manager->GetArchiveFromFile(file);
                        Tracks.push_back({course, nullptr, "", name, dir, archive});
                    } else {
                        printf("ContentBrowser.cpp: Track '%s' missing required track files. Cannot add to game\n  Missing %s/data_track_sections file\n", name.c_str(), dir.c_str());
                    }

                }
            }
        }
    }

    void ContentBrowserWindow::FindContent() {
        auto ptr = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->ListFiles({"hmintro/*", "*tracks/*","actors/*", "objects/*"}, {""});
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

    /**  Actors that need config windows before spawning  **/

    ATrain* ContentBrowserWindow::TrainWindow() {
        if (!bIsTrainWindowOpen) {
            return nullptr;
        }

        // Setup train window size and position
        // Set window size constraints (min, max)
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));

        // Get the main viewport to find the center of the screen
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Center the window
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // Optional: auto-resize to content
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Appearing); // Initial size only

        if (ImGui::Begin("Spawn Train")) {
            static int32_t numCarriages = 4;
            static ATrain::TenderStatus tender = ATrain::TenderStatus::HAS_TENDER; // Can only disable tender if using no passenger cars
            static int32_t pathIndex = 0;
            static int32_t pathPoint = 0;
            static ATrain::SpawnMode spawnMode = ATrain::SpawnMode::AUTO;

            // Num Carriage Input
            if (ImGui::InputInt("Carriages", &numCarriages)) {
                // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                if (numCarriages < 0) numCarriages = 0;
            }

            // Setup for has tender settings
            if (numCarriages > 0) {
                ImGui::BeginDisabled();
                // Tender is required if there are any carriages
                tender = ATrain::TenderStatus::HAS_TENDER;
            }

            // Convert enum to bool
            bool hasTender = (tender == ATrain::TenderStatus::HAS_TENDER);
            if (ImGui::Checkbox("Has Tender", &hasTender)) {
                tender = hasTender ? ATrain::TenderStatus::HAS_TENDER : ATrain::TenderStatus::NO_TENDER;
            }

            if (numCarriages > 0) {
                ImGui::EndDisabled();
            }

            // Set Spawn Mode
            bool localSpawnMode = (spawnMode == ATrain::SpawnMode::AUTO);
            if (ImGui::Checkbox("Place Train Automatically", &localSpawnMode)) {
                spawnMode = localSpawnMode ? ATrain::SpawnMode::AUTO : ATrain::SpawnMode::POINT;
            }

            // Set PathIndex and PathPoint
            if (spawnMode == ATrain::SpawnMode::POINT) {
                // PathIndex and PathPoint
                if (ImGui::InputInt("Path Index", &pathIndex)) {
                    // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                    if (pathIndex < 0) pathIndex = 0;
                }

                if (ImGui::InputInt("Path Point", &pathPoint)) {
                    // Clamp to uint32_t range (only lower bound needed if assuming positive values)
                    if (pathPoint < 0) pathPoint = 0;
                }
            }

            if (ImGui::Button("Spawn")) {
                bIsTrainWindowOpen = false;
                return ATrain::Spawn(tender, numCarriages, 2.5f, (uint32_t)pathIndex, (uint32_t)pathPoint, spawnMode);
            }
        }
        ImGui::End();
    }
}
