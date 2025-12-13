#include "SceneManager.h"

#include "port/Game.h"
#include "engine/CoreMath.h"
#include "World.h"
#include "GameObject.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <optional> // Must be before json.hpp
#include <nlohmann/json.hpp>
#include "port/Engine.h"
#include <ship/resource/type/Json.h>
#include "port/resource/type/Minimap.h"
#include <ship/resource/File.h>
#include "port/resource/type/ResourceType.h"

#include "engine/vehicles/Train.h"

#include "engine/objects/Object.h"
#include "engine/objects/Thwomp.h"
#include "engine/objects/Snowman.h"
#include "engine/cameras/TourCamera.h"
#include "port/resource/type/TrackPathPointData.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
#include "code_80005FD0.h"
}

namespace Editor {
    void SaveLevel(Track* track) {
        nlohmann::json data;

        /**
         * Save track properties, static mesh actors, actors, and tour camera
         */
        data["Props"] = track->Props.to_json();

        nlohmann::json staticMesh;
        SaveStaticMeshActors(staticMesh);
        data["StaticMeshActors"] = staticMesh;


        nlohmann::json actors;
        SaveActors(actors);
        data["Actors"] = actors;


        if (track->TourShots.size() != 0) {
            nlohmann::json tour;
            SaveTour(track, tour);
            data["Tour"] = tour;
        }

        if (nullptr == track->Archive) {
            SPDLOG_INFO("[SceneManager] [SaveLevel] Track archive nullptr");
            return;
        }

        /**
         * Write data to file
         */
        try {
            std::string jsonStr = data.dump(2);
            std::vector<uint8_t> bytes; // Turn the str into raw data
            bytes.assign(jsonStr.begin(), jsonStr.end());

            const TrackInfo* info = gTrackRegistry.GetInfo(track->ResourceName);
            std::string sceneFile = info->Path + "/scene.json";

            // Write file to disk
            bool wrote = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->WriteFile(track->Archive, sceneFile, bytes);
            if (wrote) {
                // Tell the cache this file needs to be reloaded
                auto resource = GameEngine::Instance->context->GetResourceManager()->GetCachedResource(sceneFile);
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

    /** Do not use GetWorld()->CurrentCourse during loading! The current track is not guaranteed! **/
    void LoadTrackDataFromJson(Track* track, const std::string& trackPath) {
        SPDLOG_INFO("[SceneManager] [LoadTrackDataFromJson] Loading track scenefile...");

        if (trackPath.empty()) {
            SPDLOG_INFO("  Unable to load track. trackPath empty.");
            return;
        }

        std::string sceneFile = trackPath + "/scene.json";
        if ((nullptr == track) || (nullptr == track->Archive)) {
            SPDLOG_INFO("  Failed to load scenefile, track or archive were null");
            return;
        }

        /* 
         * Manually loading a custom asset file (scene.json) with no extractor class means that
         * the init data needs to be manually populated
         */
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Parent = track->Archive;
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->ByteOrder = Ship::Endianness::Little;
        initData->Type = static_cast<uint32_t>(Ship::ResourceType::Json);
        initData->ResourceVersion = 0;

        // Load the scene file and return the json data
        nlohmann::json data = std::static_pointer_cast<Ship::Json>(
            GameEngine::Instance->context->GetResourceManager()->LoadResource(sceneFile, true, initData))->Data;

        // Check that the data is valid
        if (data.is_null() || !data.is_object() || data.empty()) {
            SPDLOG_INFO("  Scenefile corrupted or contained no data");
            return;
        }

        SPDLOG_INFO("  Loading Props, Actors, Static Mesh Actors, and Tour...");

        // Load the Props, and populate actors
        LoadProps(track, data);
        LoadPaths(track, trackPath);
        LoadMinimap(track, trackPath);
        LoadActors(track, data);
        LoadStaticMeshActors(track, data);
        LoadTour(track, data);
        SPDLOG_INFO("[SceneManager] [LoadTrackDataFromJson] Scene File Loaded!");
    }

    void LoadTrackInfo(TrackInfo& info, std::shared_ptr<Ship::Archive> archive, std::string sceneFile) {
        if (nullptr == archive) {
            SPDLOG_INFO("[SceneManager] [LoadTrackDataFromJson] Failed to load scenefile, track or rootarchive were null");
            return;
        }

        /* 
         * Manually loading a custom asset file (scene.json) with no extractor class means that
         * the init data needs to be manually populated
         */
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Parent = archive;
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->ByteOrder = Ship::Endianness::Little;
        initData->Type = static_cast<uint32_t>(Ship::ResourceType::Json);
        initData->ResourceVersion = 0;

        // Load the scene file and return the json data
        nlohmann::json data = std::static_pointer_cast<Ship::Json>(
            GameEngine::Instance->context->GetResourceManager()->LoadResource(sceneFile, true, initData))->Data;

        // Check that the data is valid
        if (data.is_null() || !data.is_object() || data.empty()) {
            SPDLOG_INFO("[SceneManager] [LoadTrackInfo] Scenefile corrupted or contained no data");
            return;
        }

        LoadTrackInfoData(info, data);

        SPDLOG_INFO("[SceneManager] [LoadTrackInfo] Loaded track info!");
    }

    void Load_AddStaticMeshActor(const nlohmann::json& actorJson) {
        GetWorld()->StaticMeshActors.push_back(std::make_unique<StaticMeshActor>("", FVector(0, 0, 0), IRotator(0, 0, 0), FVector(1, 1, 1), "", nullptr));
        auto& actor = GetWorld()->StaticMeshActors.back();
        actor->from_json(actorJson);

        printf("After from_json: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
        actor->Pos.x, actor->Pos.y, actor->Pos.z, actor->Name.c_str(), actor->Model.c_str());
    }

    void LoadMinimap(Track* track, std::string filePath) {
        std::string minimapPath = filePath + "/minimap.png";
        SPDLOG_INFO("  Loading {} minimap...", filePath);

        /* 
        * Manually loading a custom asset file with no extractor class means that
        * the init data needs to be manually populated
        */
        auto initData = std::make_shared<Ship::ResourceInitData>();
        initData->Parent = track->Archive;
        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->ByteOrder = Ship::Endianness::Little;
        initData->Type = static_cast<uint32_t>(MK64::ResourceType::Minimap);
        initData->ResourceVersion = 0;

        std::shared_ptr<MK64::Minimap> ptr = std::static_pointer_cast<MK64::Minimap>(
            GameEngine::Instance->context->GetResourceManager()->LoadResource(minimapPath, true, initData));

        if (ptr) {
            SPDLOG_INFO("  Minimap Loaded!");
            MK64::MinimapTexture texture = ptr->Texture;
            track->Props.Minimap.Texture = (const char*)texture.Data;
            track->Props.Minimap.Width = texture.Width;
            track->Props.Minimap.Height = texture.Height;
        } else { // Fallback
            SetDefaultMinimap(track);
        }
    }

    // Sets the default minimap if none has been set
    void SetDefaultMinimap(Track* track) {
        track->Props.Minimap.Texture = minimap_mario_raceway;
        track->Props.Minimap.Width = ResourceGetTexWidthByName(track->Props.Minimap.Texture);
        track->Props.Minimap.Height = ResourceGetTexHeightByName(track->Props.Minimap.Texture);
        SPDLOG_INFO("  No minimap found! Falling back to default minimap");
    }

    void SaveActors(nlohmann::json& actorList) {
        for (const auto& actor : GetWorld()->Actors) {
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

        for (const auto& object : GetWorld()->Objects) {
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
        for (const auto& mesh : GetWorld()->StaticMeshActors) {
            actorList.push_back(mesh->to_json());
        }
    }

    void SaveTour(Track* track, nlohmann::json& tour) {
        tour["Enabled"] = track->bTourEnabled;

        // Camera shots
        tour["Shots"] = nlohmann::json::array();
        for (const auto& shot : track->TourShots) {
            tour["Shots"].push_back(ToJson(shot));
        }
    }

    void LoadProps(Track* track, nlohmann::json& data) {
        if (!data.contains("Props") || !data["Props"].is_object()) {
            SPDLOG_INFO("Track is missing props data. Is the scene.json file corrupt?");
            return;
        }

        try {
            track->Props.from_json(data["Props"]);
        } catch(const std::exception& e) {
            std::cerr << "  Error parsing track properties: " << e.what() << std::endl;
            std::cerr << "    Is your scene.json file out of date?" << std::endl;
        }
    }

    void LoadPaths(Track* track, const std::string& trackPath) {
        std::string path_file = (trackPath + "/data_paths").c_str();

        auto res = std::dynamic_pointer_cast<MK64::Paths>(ResourceLoad(path_file.c_str()));

        if (res != nullptr) {
            auto& paths = res->PathList;

            size_t i = 0;
            u16* ptr = &track->Props.PathSizes.unk0;
            for (auto& path : paths) {
                if (i >= ARRAY_COUNT(track->Props.PathTable2)) {
                    printf("[Track.cpp] The game can only import 5 paths. Found more than 5. Skipping the rest\n");
                    break; // Only 5 paths allowed. 4 track, 1 vehicle
                }
                ptr[i] = path.size();
                track->Props.PathTable2[i] = (TrackPathPoint*) path.data();

                i += 1;
            }
        }
        gVehiclePathSize = track->Props.PathSizes.unk0; // This is likely incorrect.
    }

    void LoadTrackInfoData(TrackInfo& info, nlohmann::json& data) {
        if (!data.contains("Props") || !data["Props"].is_object()) {
            SPDLOG_INFO("[SceneManager] [LoadTrackInfoData] Track is missing props data. Is the scene.json file corrupt?");
            return;
        }

        try {
            nlohmann::json& j = data.at("Props");

            info.ResourceName = j.at("ResourceName").get<std::string>();
            info.Name = j.at("Name").get<std::string>();
            info.DebugName = j.at("DebugName").get<std::string>();
            info.Length = j.at("TrackLength").get<std::string>();

            printf("[SceneManager] [LoadTrackInfoData] ResourceName: %s, Name: %s, DebugName: %s, Length: %s\n",
                info.ResourceName.c_str(),
                info.Name.c_str(),
                info.DebugName.c_str(),
                info.Length.c_str()
            );
        } catch(const std::exception& e) {
            std::cerr << "  Error parsing track properties: " << e.what() << std::endl;
            std::cerr << "    Is your scene.json file out of date?" << std::endl;
        }
    }

    void LoadActors(Track* track, nlohmann::json& data) {
        if (!data.contains("Actors") || !data["Actors"].is_array()) {
            SPDLOG_INFO("  This track contains no actors");
            return;
        }

        track->SpawnList.clear(); // Clear existing actors, if any

        for (const auto& actor : data["Actors"]) {
            SpawnParams params;
            params.from_json(actor); //<SpawnParams>();
            if (!params.Name.empty()) {
                track->SpawnList.push_back(params);
            }
        }
    }

    void LoadStaticMeshActors(Track* track, nlohmann::json& data) {
        if (!data.contains("StaticMeshActors") || !data["StaticMeshActors"].is_array()) {
            SPDLOG_INFO("  This track contains no StaticMeshActors!");
            return;
        }

        GetWorld()->StaticMeshActors.clear(); // Clear existing actors, if any
        
        for (const auto& actorJson : data["StaticMeshActors"]) {
            Load_AddStaticMeshActor(actorJson);
        }
    }

    void LoadTour(Track* track, nlohmann::json& data) {
        if (!data.contains("Tour") || !data["Tour"].is_object()) {
            SPDLOG_INFO("  This track does not contain a camera tour");
            return;
        }

        nlohmann::json& tours = data["Tour"];

        // Enable flag
        if (tours.contains("Enabled")) {
            track->bTourEnabled = tours["Enabled"].get<bool>();
        } else {
            track->bTourEnabled = false;
        }

        // Camera shots
        if (tours.contains("Shots") && tours["Shots"].is_array()) {
            track->TourShots.clear();

            for (const auto& shotJson : tours["Shots"]) {
                track->TourShots.push_back(FromJsonCameraShot(shotJson));
            }
        }
    }
}
