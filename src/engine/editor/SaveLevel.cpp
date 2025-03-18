#include "SaveLevel.h"
#include "port/Game.h"
#include "CoreMath.h"
#include "World.h"
#include "GameObject.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Editor {

   // SaveLevel::SaveLevel() {}

    void SaveLevel() {
        auto props = gWorldInstance.CurrentCourse->Props;

        nlohmann::json data;

        data["Props"] = props.to_json();

        nlohmann::json actors;

      /*  for (const auto& actor : gWorldInstance.StaticMeshActors) {
            actors.push_back(actor->to_json());
        }

        data["Actors"] = actors;*/

        std::ofstream file("track.json");
        file << data.dump(4); // Pretty print with indent
    }

    void LoadLevel() {
        // Open the JSON file
        std::ifstream file("track.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open track.json for reading!" << std::endl;
            return;
        }

        // Check if level data file is empty
        if (file.peek() == std::ifstream::traits_type::eof()) {
            return;
        }

        // Parse the JSON file into a nlohmann::json object
        nlohmann::json data;
        file >> data;


        // Load the Props (deserialize it)
        if (data.contains("Props")) {
            auto& propsJson = data["Props"];
            gWorldInstance.CurrentCourse->Props.from_json(propsJson);  // Assuming you have a `from_json` function
        } else {
            std::cerr << "Props data not found in the JSON file!" << std::endl;
        }

        // Load the Actors (deserialize them)
        if (data.contains("Actors")) {
            auto& actorsJson = data["Actors"];
            gWorldInstance.StaticMeshActors.clear();  // Clear existing actors, if any

            for (const auto& actorJson : actorsJson) {
                Load_AddStaticMeshActor(actorJson);
            }
        } else {
            std::cerr << "Actors data not found in the JSON file!" << std::endl;
        }
        // Close the file after loading
        file.close();
    }

    void Load_AddStaticMeshActor(const nlohmann::json& actorJson) {
        gWorldInstance.StaticMeshActors.push_back(new StaticMeshActor("", FVector(0, 0, 0), IRotator(0, 0, 0), FVector(1, 1, 1), "", nullptr));
        auto actor = gWorldInstance.StaticMeshActors.back();
        actor->from_json(actorJson);
        
printf("After from_json: Pos(%f, %f, %f), Name: %s, Model: %s\n", 
       actor->Pos.x, actor->Pos.y, actor->Pos.z, actor->Name.c_str(), actor->Model.c_str());
        gEditor.AddObject(actor->Name.c_str(), &actor->Pos, (Vec3s*)&actor->Rot, &actor->Scale, (Gfx*) nullptr, 1.0f,
                        GameObject::CollisionType::BOUNDING_BOX, 20.0f, (int32_t*) &actor->bPendingDestroy, (int32_t) 1);
    }
}
