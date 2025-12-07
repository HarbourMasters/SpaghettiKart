#pragma once

#include <libultraship/libultraship.h>
#include "CoreMath.h"
#include "engine/courses/Course.h"
#include <optional>
#include <nlohmann/json.hpp>

namespace Editor {
    void SaveLevel();
    void LoadLevel(Course* course, std::string sceneFile);
    void Load_AddStaticMeshActor(const nlohmann::json& actorJson);
    void SetSceneFile(std::shared_ptr<Ship::Archive> archive, std::string sceneFile);
    void LoadMinimap(Course* course, std::string filePath);
    void SetDefaultMinimap(Course* course);

    void SaveActors(nlohmann::json& actorList);
    void SaveStaticMeshActors(nlohmann::json& actorList);
    void SaveTour(nlohmann::json& tour);

    void LoadProps(Course* course, nlohmann::json& data);
    void LoadActors(Course* course, nlohmann::json& data);
    void LoadStaticMeshActors(Course* course, nlohmann::json& data);
    void LoadTour(Course* course, nlohmann::json& data);

    void SpawnActors(std::vector<std::pair<std::string, SpawnParams>> spawnList);

    extern std::shared_ptr<Ship::Archive> CurrentArchive; // This is used to retrieve and write the scene data file
    extern std::string SceneFile;


    inline nlohmann::json ToJson(const FVector& v) {
        return {
            {"x", v.x},
            {"y", v.y},
            {"z", v.z}
        };
    }

    inline nlohmann::json ToJson(const TourCamera::KeyFrame& kf) {
        return {
            {"Pos", ToJson(kf.Pos)},
            {"LookAt", ToJson(kf.LookAt)},
            {"Duration", kf.Duration}
        };
    }

    inline nlohmann::json ToJson(const TourCamera::CameraShot& shot) {
        nlohmann::json j;
        j["StartPos"]   = ToJson(shot.Pos);
        j["StartLookAt"] = ToJson(shot.LookAt);

        // KeyFrames
        nlohmann::json keyframes = nlohmann::json::array();
        for (const auto& kf : shot.Frames) {
            keyframes.push_back(ToJson(kf));
        }
        j["KeyFrames"] = keyframes;

        return j;
    }

    inline FVector FromJsonVec(const nlohmann::json& j) {
        FVector v{};
        if (j.contains("x")) v.x = j["x"].get<float>();
        if (j.contains("y")) v.y = j["y"].get<float>();
        if (j.contains("z")) v.z = j["z"].get<float>();
        return v;
    }

    inline TourCamera::KeyFrame FromJsonKeyFrame(const nlohmann::json& j) {
        TourCamera::KeyFrame kf{};
        if (j.contains("Pos"))     kf.Pos = FromJsonVec(j["Pos"]);
        if (j.contains("LookAt"))  kf.LookAt = FromJsonVec(j["LookAt"]);
        if (j.contains("Duration")) kf.Duration = j["Duration"].get<float>();
        return kf;
    }

    inline TourCamera::CameraShot FromJsonCameraShot(const nlohmann::json& j) {
        TourCamera::CameraShot shot{};
        if (j.contains("StartPos"))    shot.Pos = FromJsonVec(j["StartPos"]);
        if (j.contains("StartLookAt")) shot.LookAt = FromJsonVec(j["StartLookAt"]);

        if (j.contains("KeyFrames") && j["KeyFrames"].is_array()) {
            for (const auto& kfJson : j["KeyFrames"]) {
                shot.Frames.push_back(FromJsonKeyFrame(kfJson));
            }
        }

        return shot;
    }
}
