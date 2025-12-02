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
#include <ship/resource/type/Json.h>
#include "port/resource/type/Minimap.h"
#include <ship/resource/File.h>
#include "port/resource/type/ResourceType.h"

#include "engine/vehicles/Train.h"

#include "engine/objects/Object.h"
#include "engine/objects/Thwomp.h"
#include "engine/objects/Snowman.h"
#include <iostream>

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
}

namespace Editor {

    std::shared_ptr<Ship::Archive> CurrentArchive;
    std::string SceneFile = "";

    void SaveLevel() {
        auto props = gWorldInstance.CurrentCourse->Props;

        if ((CurrentArchive) && (!SceneFile.empty())) {
            nlohmann::json data;
    
            data["Props"] = props.to_json();

            nlohmann::json staticMesh;

            for (const auto& mesh : gWorldInstance.StaticMeshActors) {
                staticMesh.push_back(mesh->to_json());
            }
            data["StaticMeshActors"] = staticMesh;

            nlohmann::json actors;

            SaveActors(actors);

            data["Actors"] = actors;

            try {
                auto dat = data.dump(2);
                std::vector<uint8_t> stringify;
                stringify.assign(dat.begin(), dat.end());

                bool wrote = GameEngine::Instance->context->GetResourceManager()->GetArchiveManager()->WriteFile(CurrentArchive, SceneFile, stringify);
                if (wrote) {
                    // Tell the cache this needs to be reloaded
                    auto resource = GameEngine::Instance->context->GetResourceManager()->GetCachedResource(SceneFile);
                    if (resource) {
                        resource->Dirty();
                    }
                } else {
                    printf("[SceneManager::SaveLevel] Failed to write scene file!\n");
                }
            } catch (const nlohmann::json::exception& e) {
                printf("[SceneManager::SaveLevel]\n  JSON error during dump: %s\n", e.what());
            }
        } else {
            printf("Could not save scene file, SceneFile or CurrentArchive not set\n");
        }
    }


    /** Do not use gWorldInstance.CurrentCourse during loading! The current track is not guaranteed! **/
    void LoadLevel(Course* course, std::string sceneFile) {
        SceneFile = sceneFile;
        if ((nullptr != course) && (nullptr != course->RootArchive)) {
            auto initData = std::make_shared<Ship::ResourceInitData>();
            initData->Parent = course->RootArchive;
            initData->Format = RESOURCE_FORMAT_BINARY;
            initData->ByteOrder = Ship::Endianness::Little;
            initData->Type = static_cast<uint32_t>(Ship::ResourceType::Json);
            initData->ResourceVersion = 0;

            nlohmann::json data = std::static_pointer_cast<Ship::Json>(
                GameEngine::Instance->context->GetResourceManager()->LoadResource(sceneFile, true, initData))->Data;

            if (data.is_null() || data.empty()) {
                return;
            }

            // Load the Props (deserialize it)
            if (data.contains("Props")) {
                auto& propsJson = data["Props"];
                try {
                    course->Props.from_json(propsJson);
                } catch(const std::exception& e) {
                    std::cerr << "SceneManager::LoadLevel() Error parsing track properties: " << e.what() << std::endl;
                    std::cerr << "  Is your scene.json file out of date?" << std::endl;
                }
            } else {
                std::cerr << "Props data not found in the JSON file!" << std::endl;
            }

            /** Populate Track SpawnParams for spawning actors **/
            if (data.contains("Actors")) {
                auto & actorsJson = data["Actors"];
                course->SpawnList.clear();
                for (const auto& actor : actorsJson) {
                    SpawnParams params;
                    params.from_json(actor); //<SpawnParams>();
                    if (!params.Name.empty()) {
                        course->SpawnList.push_back(params);
                    }
                }
                SPDLOG_INFO("[SceneManager] Loaded Scene File!");
            }

            // Load the Actors (deserialize them)
            if (data.contains("StaticMeshActors")) {
                auto& actorsJson = data["StaticMeshActors"];
                gWorldInstance.StaticMeshActors.clear();  // Clear existing actors, if any
                
                for (const auto& actorJson : actorsJson) {
                    Load_AddStaticMeshActor(actorJson);
                }
            } else {
                SPDLOG_INFO("[SceneManager::LoadLevel] [scene.json] This track contains no StaticMeshActors!");
            }
        }
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

    void LoadMinimap(Course* course, std::string filePath) {
        printf("LOADING MINIMAP %s\n", filePath.c_str());
        if ((nullptr != course) && (nullptr != course->RootArchive)) {
            auto initData = std::make_shared<Ship::ResourceInitData>();
            initData->Parent = course->RootArchive;
            initData->Format = RESOURCE_FORMAT_BINARY;
            initData->ByteOrder = Ship::Endianness::Little;
            initData->Type = static_cast<uint32_t>(MK64::ResourceType::Minimap);
            initData->ResourceVersion = 0;

            std::shared_ptr<MK64::Minimap> ptr = std::static_pointer_cast<MK64::Minimap>(
                GameEngine::Instance->context->GetResourceManager()->LoadResource(filePath, true, initData));

            if (ptr) {
                printf("LOADED MINIMAP!\n");
                MK64::MinimapTexture texture = ptr->Texture;
                course->Props.Minimap.Texture = (const char*)texture.Data;
                course->Props.Minimap.Width = texture.Width;
                course->Props.Minimap.Height = texture.Height;
            } else { // Fallback
                course->Props.Minimap.Texture = minimap_mario_raceway;
                course->Props.Minimap.Width = ResourceGetTexWidthByName(course->Props.Minimap.Texture);
                course->Props.Minimap.Height = ResourceGetTexHeightByName(course->Props.Minimap.Texture);
            }
        }
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
}
