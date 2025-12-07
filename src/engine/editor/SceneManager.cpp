#include "SceneManager.h"

#include "port/Game.h"
#include "engine/CoreMath.h"
#include "World.h"
#include "GameObject.h"

#include <iostream>
#include <fstream>
#include <optional> // Must be before json.hpp
#include <nlohmann/json.hpp>
#include "port/Engine.h"
#include <libultraship/src/resource/type/Json.h>
#include "port/resource/type/Minimap.h"
#include <libultraship/src/resource/File.h>
#include "port/resource/type/ResourceType.h"

#include "engine/vehicles/Train.h"

#include "engine/objects/Object.h"
#include "engine/objects/Thwomp.h"
#include "engine/objects/Snowman.h"
#include "engine/cameras/TourCamera.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
}

namespace Editor {
    std::shared_ptr<Ship::Archive> CurrentArchive;
    std::string SceneFile = "";

    void SaveLevel() {
        if ((!CurrentArchive) || (SceneFile.empty())) {
            SPDLOG_INFO("[SceneManager] [SaveLevel] Could not save scene file, SceneFile or CurrentArchive not set");
            return;
        }
        nlohmann::json data;

        /**
         * Save track properties, static mesh actors, actors, and tour camera
         */
        data["Props"] = gWorldInstance.GetCurrentCourse()->Props.to_json();

        nlohmann::json staticMesh;
        SaveStaticMeshActors(staticMesh);
        data["StaticMeshActors"] = staticMesh;


        nlohmann::json actors;
        SaveActors(actors);
        data["Actors"] = actors;


        if (gWorldInstance.GetCurrentCourse()->TourShots.size() != 0) {
            nlohmann::json tour;
            SaveTour(tour);
            data["Tour"] = tour;
        }

        /**
         * Write data to file
         */
        try {
            std::string jsonStr = data.dump(2);
            std::vector<uint8_t> bytes; // Turn the str into raw data
            bytes.assign(jsonStr.begin(), jsonStr.end());

            // Write file to disk
            bool wrote = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->WriteFile(CurrentArchive, SceneFile, bytes);
            if (wrote) {
                // Tell the cache this file needs to be reloaded
                auto resource = GameEngine::Instance->context->GetResourceManager()->GetCachedResource(SceneFile);
                if (resource) {
                    resource->Dirty();
                }
            } else {
                SPDLOG_INFO("[SceneManager] [SaveLevel] Failed to write scene file!");
            }
        } catch (const nlohmann::json::exception& e) {
            SPDLOG_INFO("[SceneManager] [SaveLevel] JSON error during dump: {}", e.what());
        }
    }

    /** Do not use gWorldInstance.CurrentCourse during loading! The current track is not guaranteed! **/
    void LoadLevel(Course* course, std::string sceneFile) {
        SceneFile = sceneFile;
        if ((nullptr == course) || (nullptr == course->RootArchive)) {
            SPDLOG_INFO("[SceneManager] [LoadLevel] Failed to load scenefile, course or rootarchive were null");
            return;
        }

        /* 
         * Manually loading a custom asset file (scene.json) with no extractor class means that
         * the init data needs to be manually populated
         */
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Parent = course->RootArchive;
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->ByteOrder = Ship::Endianness::Little;
        initData->Type = static_cast<uint32_t>(Ship::ResourceType::Json);
        initData->ResourceVersion = 0;

        // Load the scene file and return the json data
        nlohmann::json data = std::static_pointer_cast<Ship::Json>(
            GameEngine::Instance->context->GetResourceManager()->LoadResource(sceneFile, true, initData))->Data;

        // Check that the data is valid
        if (data.is_null() || !data.is_object() || data.empty()) {
            SPDLOG_INFO("[SceneManager] [LoadLevel] Scenefile corrupted or contained no data");
            return;
        }

        SPDLOG_INFO("[SceneManager] [LoadLevel] Loading track scenefile...");

        // Load the Props, and populate actors
        LoadProps(course, data);
        LoadActors(course, data);
        LoadStaticMeshActors(course, data);
        LoadTour(course, data);
        SPDLOG_INFO("[SceneManager] [LoadLevel] Scene File Loaded!");
    }

    void Load_AddStaticMeshActor(const nlohmann::json& actorJson) {
        gWorldInstance.StaticMeshActors.push_back(new StaticMeshActor("", FVector(0, 0, 0), IRotator(0, 0, 0), FVector(1, 1, 1), "", nullptr));
        auto actor = gWorldInstance.StaticMeshActors.back();
        actor->from_json(actorJson);

        printf("After from_json: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
        actor->Pos.x, actor->Pos.y, actor->Pos.z, actor->Name.c_str(), actor->Model.c_str());
    }

    void SetSceneFile(std::shared_ptr<Ship::Archive> archive, std::string sceneFile) {
        CurrentArchive = archive;
        SceneFile = sceneFile;
    }

    // Called from ContentBrowser.cpp
    void LoadMinimap(Course* course, std::string filePath) {
        SPDLOG_INFO("  Loading {} minimap...", filePath);
        if (nullptr == course->RootArchive) {
            SPDLOG_INFO("[SceneManager] [LoadMinimap] Root archive is nullptr");
            SetDefaultMinimap(course);
            return;
        }

        /* 
        * Manually loading a custom asset file with no extractor class means that
        * the init data needs to be manually populated
        */
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Parent = course->RootArchive;
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->ByteOrder = Ship::Endianness::Little;
        initData->Type = static_cast<uint32_t>(MK64::ResourceType::Minimap);
        initData->ResourceVersion = 0;

        std::shared_ptr<MK64::Minimap> ptr = std::static_pointer_cast<MK64::Minimap>(
            GameEngine::Instance->context->GetResourceManager()->LoadResource(filePath, true, initData));

        if (ptr) {
            SPDLOG_INFO("  Minimap Loaded!");
            MK64::MinimapTexture texture = ptr->Texture;
            course->Props.Minimap.Texture = (const char*)texture.Data;
            course->Props.Minimap.Width = texture.Width;
            course->Props.Minimap.Height = texture.Height;
        } else { // Fallback
            SetDefaultMinimap(course);
        }
    }

    // Sets the default minimap if none has been set
    void SetDefaultMinimap(Course* course) {
        course->Props.Minimap.Texture = minimap_mario_raceway;
        course->Props.Minimap.Width = ResourceGetTexWidthByName(course->Props.Minimap.Texture);
        course->Props.Minimap.Height = ResourceGetTexHeightByName(course->Props.Minimap.Texture);
        SPDLOG_INFO("  No minimap found! Falling back to default minimap");
    }

    void SaveActors(nlohmann::json& actorList) {
        for (const auto& actor : gWorldInstance.Actors) {
            SpawnParams params{};
            bool alreadyProcessed = false;

            // Only some actors are supported for saving.
            // Bananas and stuff don't make sense to be saved.
            switch(actor->Type) {
                case ACTOR_ITEM_BOX:
                case ACTOR_FAKE_ITEM_BOX:
                case ACTOR_TREE_MARIO_RACEWAY:
                case ACTOR_TREE_YOSHI_VALLEY:
                case ACTOR_TREE_ROYAL_RACEWAY:
                case ACTOR_TREE_MOO_MOO_FARM:
                case ACTOR_PALM_TREE:
                case ACTOR_TREE_LUIGI_RACEWAY: // A plant?
                case ACTOR_UNKNOWN_0x1B:
                case ACTOR_TREE_PEACH_CASTLE:
                case ACTOR_TREE_FRAPPE_SNOWLAND:
                case ACTOR_CACTUS1_KALAMARI_DESERT:
                case ACTOR_CACTUS2_KALAMARI_DESERT:
                case ACTOR_CACTUS3_KALAMARI_DESERT:
                case ACTOR_BUSH_BOWSERS_CASTLE:
                    params.Name = get_actor_resource_location_name(actor->Type);
                    params.Location = FVector(actor->Pos[0], actor->Pos[1], actor->Pos[2]);
                    if (!params.Name.empty()) {
                        actorList.push_back(params.to_json());
                    }
                    alreadyProcessed = true;
                    break;
                case ACTOR_PIRANHA_PLANT:
                    params.Name = get_actor_resource_location_name(actor->Type);
                    params.Location = FVector(actor->Pos[0], actor->Pos[1], actor->Pos[2]);
                    // params.Type = // Need this to use royal raceway version
                    actorList.push_back(params.to_json());
                    alreadyProcessed = true;
                    break;
                case ACTOR_YOSHI_EGG:
                    params.Name = get_actor_resource_location_name(actor->Type);
                    params.Location = FVector(actor->Velocity[0], actor->Pos[1], actor->Velocity[2]); // Velocity is pathCenter
                    if (!params.Name.empty()) {
                        actorList.push_back(params.to_json());
                    }
                    alreadyProcessed = true;
                    break;
            }

            if (!alreadyProcessed) {
                actor->SetSpawnParams(params);
                if (!params.Name.empty()) {
                    actorList.push_back(params.to_json());
                }
            }
        }

        for (const auto& object : gWorldInstance.Objects) {
            SpawnParams params;
            object->SetSpawnParams(params);

            // Unimplemented objects should not be added to the SpawnList
            // The name field is required. If not set, then its not implemented yet.
            if (!params.Name.empty()) {
                actorList.push_back(params.to_json());
            }
        }
    }

    void SaveStaticMeshActors(nlohmann::json& actorList) {
        for (const auto& mesh : gWorldInstance.StaticMeshActors) {
            actorList.push_back(mesh->to_json());
        }
    }

    void SaveTour(nlohmann::json& tour) {
        tour["Enabled"] = gWorldInstance.GetCurrentCourse()->bTourEnabled;

        // Camera shots
        tour["Shots"] = nlohmann::json::array();
        for (const auto& shot : gWorldInstance.GetCurrentCourse()->TourShots) {
            tour["Shots"].push_back(ToJson(shot));
        }
    }

    void LoadProps(Course* course, nlohmann::json& data) {
        if (!data.contains("Props") || !data["Props"].is_object()) {
            SPDLOG_INFO("Track is missing props data. Is the scene.json file corrupt?");
            return;
        }

        try {
            course->Props.from_json(data["Props"]);
        } catch(const std::exception& e) {
            std::cerr << "  Error parsing track properties: " << e.what() << std::endl;
            std::cerr << "    Is your scene.json file out of date?" << std::endl;
        }
    }

    void LoadActors(Course* course, nlohmann::json& data) {
        if (!data.contains("Actors") || !data["Actors"].is_object()) {
            SPDLOG_INFO("  This track contains no actors");
            return;
        }

        course->SpawnList.clear(); // Clear existing actors, if any

        for (const auto& actor : data["Actors"]) {
            SpawnParams params;
            params.from_json(actor); //<SpawnParams>();
            if (!params.Name.empty()) {
                course->SpawnList.push_back(params);
            }
        }
    }

    void LoadStaticMeshActors(Course* course, nlohmann::json& data) {
        if (!data.contains("StaticMeshActors") || !data["StaticMeshActors"].is_object()) {
            SPDLOG_INFO("  This track contains no StaticMeshActors!");
            return;
        }

        gWorldInstance.StaticMeshActors.clear(); // Clear existing actors, if any
        
        for (const auto& actorJson : data["StaticMeshActors"]) {
            Load_AddStaticMeshActor(actorJson);
        }
    }

    void LoadTour(Course* course, nlohmann::json& data) {
        if (!data.contains("Tour") || !data["Tour"].is_object()) {
            SPDLOG_INFO(" This track does not contain a camera tour");
            return;
        }

        nlohmann::json& tours = data["Tour"];

        // Enable flag
        if (tours.contains("Enabled")) {
            course->bTourEnabled = tours["Enabled"].get<bool>();
        } else {
            course->bTourEnabled = false;
        }

        // Camera shots
        if (tours.contains("Shots") && tours["Shots"].is_array()) {
            course->TourShots.clear();

            for (const auto& shotJson : tours["Shots"]) {
                course->TourShots.push_back(FromJsonCameraShot(shotJson));
            }
        }
    }
}
