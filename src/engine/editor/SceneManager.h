#pragma once

#include <libultraship/libultraship.h>
#include "CoreMath.h"
#include "engine/tracks/Track.h"
#include <optional>
#include <nlohmann/json.hpp>

namespace Editor {
    void SaveLevel(Track* track);
    void LoadTrackDataFromJson(Track* track, const std::string& trackPath);
    void LoadTrackInfo(TrackInfo& info, std::shared_ptr<Ship::Archive> archive, std::string sceneFile);
    void Load_AddStaticMeshActor(const nlohmann::json& actorJson);
    void LoadMinimap(Track* track, std::string filePath);
    void SetDefaultMinimap(Track* track);

    void SaveActors(nlohmann::json& actorList);
    void SaveStaticMeshActors(nlohmann::json& actorList);
    void SaveTour(Track* track, nlohmann::json& tour);

    void LoadProps(Track* track, nlohmann::json& data);
    void LoadPaths(Track* track, const std::string& trackPath);
    void LoadTrackInfoData(TrackInfo& info, nlohmann::json& data);
    void LoadActors(Track* track, nlohmann::json& data);
    void LoadStaticMeshActors(Track* track, nlohmann::json& data);
    void LoadTour(Track* track, nlohmann::json& data);

    void SpawnActors(std::vector<std::pair<std::string, SpawnParams>> spawnList);

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
