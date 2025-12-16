#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"
#include <nlohmann/json.hpp>

// todo: Make this class AStaticMeshActor : public AActor
class StaticMeshActor {
public:
    std::string Name;
    std::string ResourceName;
    FVector Pos;
    IRotator Rot;
    FVector Scale;
    std::string Model;
    int32_t* CollisionMesh;
    bool bPendingDestroy = false;
    StaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision);

    nlohmann::json to_json() const {
        nlohmann::json j;

        // Serialize each field of the class
        j["Name"] = Name;
        j["Position"] = {Pos.x, Pos.y, Pos.z};  // Assuming FVector has x, y, z fields
        j["Rotation"] = {Rot.pitch, Rot.yaw, Rot.roll};  // Assuming IRotator has pitch, yaw, roll fields
        j["Scale"] = {Scale.x, Scale.y, Scale.z};  // Assuming FVector has x, y, z fields
        j["Model"] = Model;

        // If Collision is not null, serialize it
        if (CollisionMesh != nullptr) {
            j["Collision"] = *CollisionMesh;  // Serialize the value that Collision points to
        } else {
            j["Collision"] = nullptr;  // Handle the case where Collision is nullptr
        }

        return j;
    }

    void from_json(const nlohmann::json& j) {
        Name = j.at("Name").get<std::string>();
        Pos = FVector(j.at("Position")[0].get<float>(), j.at("Position")[1].get<float>(), j.at("Position")[2].get<float>());
        Rot.Set(j.at("Rotation")[0].get<uint16_t>(), j.at("Rotation")[1].get<uint16_t>(), j.at("Rotation")[2].get<uint16_t>());
        Scale = FVector(j.at("Scale")[0].get<float>(), j.at("Scale")[1].get<float>(), j.at("Scale")[2].get<float>());

        // Deserialize the Model string
        Model = j.at("Model").get<std::string>();

        // Check if Collision is present in the JSON and deserialize it
        //if (j.contains("Collision") && !j["Collision"].is_null()) {
            // If Collision is a valid value, allocate memory for it and assign the value
        //    Collision = new int32_t(j.at("Collision").get<int32_t>());
        //} else {
            // If Collision is not present or is null, set it to nullptr
            CollisionMesh = nullptr;
        //}
    }

    virtual void Draw();
    virtual void Destroy();
};
