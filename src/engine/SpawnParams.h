#pragma once

#include <vector>
#include <optional>
#include <string>
#include <nlohmann/json.hpp>
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
}

// Helper function to handle std::optional deserialization
template <typename T>
void get_optional_to(const nlohmann::json& j, const char* key, std::optional<T>& opt_val) {
    if (j.contains(key) && !j.at(key).is_null()) {
        opt_val = j.at(key).get<T>();
    }
}

// Helper function to handle std::optional serialization
template <typename T>
void set_optional_from(nlohmann::json& j, const char* key, const std::optional<T>& opt_val) {
    if (opt_val.has_value()) {
        j[key] = opt_val.value();
    }
}

// Used to save and load all game actors to the scene file
struct SpawnParams {
    std::string Name; // Must use format mk:actor_name for stock game, mymodname:myactorname for mods
    std::optional<int16_t> Type; // OObject type (ex. Emperor penguin, sliding penguin) or literal actor type for AActors
    std::optional<int16_t> Behaviour;
    std::optional<std::string> Skin;

    std::optional<FVector> Location;
    std::optional<IRotator> Rotation;   // int16_t
    std::optional<FVector> Scale;
    std::optional<FVector> Velocity; // Used by some AActors
    std::optional<FVector2D> PatrolStart; // OCrab
    std::optional<FVector2D> PatrolEnd;   // OCrab & Hedgehog
    std::optional<IPathSpan> PathSpan; // Cheep Cheep

    // Thwomps
    std::optional<int16_t> PrimAlpha; // Thwomp
    std::optional<uint16_t> BoundingBoxSize;

    // Boos
    std::optional<uint32_t> Count; // vehicles
    std::optional<IPathSpan> LeftExitSpan;  // Disable boo
    std::optional<IPathSpan> TriggerSpan;   // Activate boos
    std::optional<IPathSpan> RightExitSpan; // Disable boo


    // Vehicles
    std::optional<uint32_t> PathIndex; // 0-3 Place vehicle this path
    std::optional<uint32_t> PathPoint; // Path point index
    std::optional<bool> Bool; // train tender
    std::optional<bool> Bool2;
    std::optional<float> Speed; // Train
    std::optional<float> SpeedB; // cars, trucks, buses, etc.
    std::optional<FVector> FVec2;

    std::optional<RGBA8> Colour;
    std::optional<RGBA8> Colour2;
    std::optional<RGBA8> Colour3;
    std::optional<RGBA8> Colour4;

    void from_json(const nlohmann::json& j) {
        j.at("Name").get_to(Name);
        get_optional_to(j, "Type", Type);
        get_optional_to(j, "Behaviour", Behaviour);
        get_optional_to(j, "Skin", Skin);
        get_optional_to(j, "Location", Location);
        get_optional_to(j, "Rotation", Rotation);
        get_optional_to(j, "Scale", Scale);
        get_optional_to(j, "Velocity", Velocity);
        get_optional_to(j, "PatrolStart", PatrolStart);
        get_optional_to(j, "PatrolEnd", PatrolEnd);
        get_optional_to(j, "PathSpan", PathSpan);
        get_optional_to(j, "PrimAlpha", PrimAlpha);
        get_optional_to(j, "BoundingBoxSize", BoundingBoxSize);
        get_optional_to(j, "Count", Count);
        get_optional_to(j, "LeftExitSpan", LeftExitSpan);
        get_optional_to(j, "TriggerSpan", TriggerSpan);
        get_optional_to(j, "RightExitSpan", RightExitSpan);
        get_optional_to(j, "PathIndex", PathIndex);
        get_optional_to(j, "PathPoint", PathPoint);
        get_optional_to(j, "Bool", Bool);
        get_optional_to(j, "Bool2", Bool2);
        get_optional_to(j, "Speed", Speed);
        get_optional_to(j, "SpeedB", SpeedB);
        get_optional_to(j, "FVec2", FVec2);
        get_optional_to(j, "Colour", Colour);
        get_optional_to(j, "Colour2", Colour2);
        get_optional_to(j, "Colour3", Colour3);
        get_optional_to(j, "Colour4", Colour4);
    }

    nlohmann::json to_json() const {
        nlohmann::json j;
        j["Name"] = Name;
        set_optional_from(j, "Type", Type);
        set_optional_from(j, "Behaviour", Behaviour);
        set_optional_from(j, "Skin", Skin);
        set_optional_from(j, "Location", Location);
        set_optional_from(j, "Rotation", Rotation);
        set_optional_from(j, "Scale", Scale);
        set_optional_from(j, "Velocity", Velocity);
        set_optional_from(j, "PatrolStart", PatrolStart);
        set_optional_from(j, "PatrolEnd", PatrolEnd);
        set_optional_from(j, "PathSpan", PathSpan);
        set_optional_from(j, "PrimAlpha", PrimAlpha);
        set_optional_from(j, "BoundingBoxSize", BoundingBoxSize);
        set_optional_from(j, "Count", Count);
        set_optional_from(j, "LeftExitSpan", LeftExitSpan);
        set_optional_from(j, "TriggerSpan", TriggerSpan);
        set_optional_from(j, "RightExitSpan", RightExitSpan);
        set_optional_from(j, "PathIndex", PathIndex);
        set_optional_from(j, "PathPoint", PathPoint);
        set_optional_from(j, "Bool", Bool);
        set_optional_from(j, "Bool2", Bool2);
        set_optional_from(j, "Speed", Speed);
        set_optional_from(j, "SpeedB", SpeedB);
        set_optional_from(j, "FVec2", FVec2);
        set_optional_from(j, "Colour", Colour);
        set_optional_from(j, "Colour2", Colour2);
        set_optional_from(j, "Colour3", Colour3);
        set_optional_from(j, "Colour4", Colour4);
        return j;
    }
};
