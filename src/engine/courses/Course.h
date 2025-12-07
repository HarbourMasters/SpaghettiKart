#ifndef ENGINE_COURSE_H
#define ENGINE_COURSE_H

#include <libultraship/libultraship.h>
#include "CoreMath.h"

#ifdef __cplusplus
#include "engine/SpawnParams.h"
#include <optional>
#include <nlohmann/json.hpp>
#include "engine/objects/Lakitu.h"
#include "engine/cameras/TourCamera.h"
#include "port/resource/type/TrackSections.h"
extern "C" {
#endif

#include "camera.h"
#include "course_offsets.h"
#include "data/some_data.h"
#include "defines.h"
#include "bomb_kart.h"
#include "path_spawn_metadata.h"
#include "waypoints.h"
#include "sounds.h"
#include "common_structs.h"

#ifdef __cplusplus
}
#endif

typedef struct SkyboxColours {
    RGB8 TopRight;
    RGB8 BottomRight;
    RGB8 BottomLeft;
    RGB8 TopLeft;
    RGB8 FloorTopRight;
    RGB8 FloorBottomRight;
    RGB8 FloorBottomLeft;
    RGB8 FloorTopLeft;
} SkyboxColours;

// Extends infinitely in the Y direction
// If a player is overtop of a water volume then it should use its height
// Recommend using the new water surface type. This is here to support the stock tracks.
// Albeit, there's no reason you cannot use this so long as you input a square.
// How to use: WaterVolumes.push_back({0, -100, 100, -100, 100});
struct WaterVolume {
    float Height; // Y coordinate of the Water level
    float MinX;
    float MaxX;
    float MinZ;
    float MaxZ;
};

typedef struct MinimapProps {
    const char* Texture;
    int16_t Width;
    int16_t Height;
    IVector2D Pos[2]; // Minimap position for players 1 and 2. 3/4 player mode is hard-coded to the center.
    int32_t PlayerX; // The offset to place the player markers
    int32_t PlayerY;
    float PlayerScaleFactor; // Scale factor of the player markers
    float FinishlineX; // The offset to place the finishline texture on the minimap
    float FinishlineY;
    RGB8 Colour; // Colour of the visible pixels (the track path)
} MinimapProps;

void ResizeMinimap(MinimapProps* minimap);
void ReverseGfx(Gfx* gfx);

void InvertTriangleWinding(Gfx* gfx);
void InvertTriangleWindingByName(const char* name);
void RestoreTriangleWinding();
bool IsTriangleWindingInverted();

typedef struct Properties {
    char Name[128];
    char DebugName[128];
    char CourseLength[128];
    int32_t LakituTowType;
    MinimapProps Minimap;
    const char* AIBehaviour;
    float AIMaximumSeparation;
    float AIMinimumSeparation;
    float NearPersp;
    float FarPersp;
    int16_t* AIDistance;
    uint32_t AISteeringSensitivity;
    _struct_gCoursePathSizes_0x10 PathSizes;
    Vec4f CurveTargetSpeed;
    Vec4f NormalTargetSpeed;
    Vec4f D_0D0096B8;
    Vec4f OffTrackTargetSpeed;
    TrackPathPoint* PathTable[4]; // Only used for podium ceremony
    TrackPathPoint* PathTable2[5]; // The fifth entry is for vehicles
    uint8_t* CloudTexture;
    CloudData *Clouds;
    CloudData *CloudList;
    SkyboxColours Skybox;
    enum MusicSeq Sequence;
    float WaterLevel; // Used for effects, and Lakitu pick up height. Not necessarily the visual water model height.

#ifdef __cplusplus
    nlohmann::json to_json() const {
        nlohmann::json j;
       // j["Id"] = Id ? Id : "";
        j["Name"] = Name ? Name : "";
        j["DebugName"] = DebugName ? DebugName : "";
        j["CourseLength"] = CourseLength ? CourseLength : "";
        //j["AIBehaviour"] = AIBehaviour ? AIBehaviour : "";
        j["LakituTowType"] = LakituTowType;
        j["AIMaximumSeparation"] = AIMaximumSeparation;
        j["AIMinimumSeparation"] = AIMinimumSeparation;
        j["NearPersp"] = NearPersp;
        j["FarPersp"] = FarPersp;

        // AIDistance as a JSON array
        j["AIDistance"] = std::vector<int16_t>(AIDistance, AIDistance + 32); // gAIDistances array size of 32

        j["AISteeringSensitivity"] = AISteeringSensitivity;

        // PathSizes - Assuming _struct_gCoursePathSizes_0x10 can be serialized similarly
        // j["PathSizes"] = PathSizes; // Implement your serialization logic here

        j["CurveTargetSpeed"] = { CurveTargetSpeed[0], CurveTargetSpeed[1], CurveTargetSpeed[2], CurveTargetSpeed[3] };
        j["NormalTargetSpeed"] = { NormalTargetSpeed[0], NormalTargetSpeed[1], NormalTargetSpeed[2], NormalTargetSpeed[3] };
        j["D_0D0096B8"] = { D_0D0096B8[0], D_0D0096B8[1], D_0D0096B8[2], D_0D0096B8[3] };
        j["OffTrackTargetSpeed"] = { OffTrackTargetSpeed[0], OffTrackTargetSpeed[1], OffTrackTargetSpeed[2], OffTrackTargetSpeed[3] };

        // Serialize arrays PathTable and PathTable2 (convert pointers into a JSON array if possible)
        //j["PathTable"] = {{}};
        //j["PathTable2"] = {{}};
        // Populate PathTable and PathTable2

        //j["Clouds"] = Clouds ? nlohmann::json{{"x", Clouds->x, "y", Clouds->y, "z", Clouds->z}} : nullptr;
        //j["CloudList"] = CloudList ? nlohmann::json{{"x", CloudList->x, "y", CloudList->y, "z", CloudList->z}} : nullptr;
        
        j["MinimapPosition"] = {Minimap.Pos[0].X, Minimap.Pos[0].Y};
        j["MinimapPosition2P"] = {Minimap.Pos[1].X, Minimap.Pos[1].Y};
        j["MinimapPlayerX"] = Minimap.PlayerX;
        j["MinimapPlayerY"] = Minimap.PlayerY;
        j["MinimapPlayerScaleFactor"] = Minimap.PlayerScaleFactor;
        j["MinimapFinishlineX"] = Minimap.FinishlineX;
        j["MinimapFinishlineY"] = Minimap.FinishlineY;
        j["MinimapColour"] = {static_cast<int>(Minimap.Colour.r), static_cast<int>(Minimap.Colour.g), static_cast<int>(Minimap.Colour.b)};
        // SkyboxColors - assuming SkyboxColors can be serialized similarly

        #define TO_INT(value) static_cast<int>(value)
        j["Skybox"] = {
            TO_INT(Skybox.TopRight.r), TO_INT(Skybox.TopRight.g), TO_INT(Skybox.TopRight.b),
            TO_INT(Skybox.BottomRight.r), TO_INT(Skybox.BottomRight.g), TO_INT(Skybox.BottomRight.b),
            TO_INT(Skybox.BottomLeft.r), TO_INT(Skybox.BottomLeft.g), TO_INT(Skybox.BottomLeft.b),
            TO_INT(Skybox.TopLeft.r), TO_INT(Skybox.TopLeft.g), TO_INT(Skybox.TopLeft.b),
            TO_INT(Skybox.FloorTopRight.r), TO_INT(Skybox.FloorTopRight.g), TO_INT(Skybox.FloorTopRight.b),
            TO_INT(Skybox.FloorBottomRight.r), TO_INT(Skybox.FloorBottomRight.g), TO_INT(Skybox.FloorBottomRight.b),
            TO_INT(Skybox.FloorBottomLeft.r), TO_INT(Skybox.FloorBottomLeft.g), TO_INT(Skybox.FloorBottomLeft.b),
            TO_INT(Skybox.FloorTopLeft.r), TO_INT(Skybox.FloorTopLeft.g), TO_INT(Skybox.FloorTopLeft.b)
        };
        j["Sequence"] = static_cast<int>(Sequence);

        j["WaterLevel"] = static_cast<float>(WaterLevel);
        #undef CAST_TO_INT

        return j;
    }

    // Function to load struct from JSON
    void from_json(const nlohmann::json& j) {
        //Id = j.at("Id").get<std::string>().c_str();
//        Name = j.at("Name").get<std::string>().c_str();
        strncpy(Name, j.at("Name").get<std::string>().c_str(), sizeof(Name) - 1);
        Name[sizeof(Name) - 1] = '\0'; // Ensure null termination

//        DebugName = j.at("DebugName").get<std::string>().c_str();
        strncpy(DebugName, j.at("DebugName").get<std::string>().c_str(), sizeof(DebugName) - 1);
        DebugName[sizeof(DebugName) - 1] = '\0'; // Ensure null termination

  //      CourseLength = j.at("CourseLength").get<std::string>().c_str();
        strncpy(CourseLength, j.at("CourseLength").get<std::string>().c_str(), sizeof(CourseLength) - 1);
        CourseLength[sizeof(CourseLength) - 1] = '\0'; // Ensure null termination

        //AIBehaviour = j.at("AIBehaviour").get<std::string>().c_str();
        LakituTowType = j.at("LakituTowType").get<int>();

        AIMaximumSeparation = j.at("AIMaximumSeparation").get<float>();
        AIMinimumSeparation = j.at("AIMinimumSeparation").get<float>();
        NearPersp = j.at("NearPersp").get<float>();
        FarPersp = j.at("FarPersp").get<float>();

        const auto temp = j.at("AIDistance").get<std::vector<int16_t>>();

        // Ensure the vector has 32 entries
        if (temp.size() == 32) {
            // Copy the data into the existing AIDistances array
            std::copy(temp.begin(), temp.end(), AIDistance);
        } else {
            printf("Course::from_json() AIDistance array not size of 32\n");
        }

        AISteeringSensitivity = j.at("AISteeringSensitivity").get<uint32_t>();

        // Deserialize PathSizes and other custom structs if needed

        CurveTargetSpeed[0] = j.at("CurveTargetSpeed")[0].get<float>();
        CurveTargetSpeed[1] = j.at("CurveTargetSpeed")[1].get<float>();
        CurveTargetSpeed[2] = j.at("CurveTargetSpeed")[2].get<float>();
        CurveTargetSpeed[3] = j.at("CurveTargetSpeed")[3].get<float>();

        NormalTargetSpeed[0] = j.at("NormalTargetSpeed")[0].get<float>();
        NormalTargetSpeed[1] = j.at("NormalTargetSpeed")[1].get<float>();
        NormalTargetSpeed[2] = j.at("NormalTargetSpeed")[2].get<float>();
        NormalTargetSpeed[3] = j.at("NormalTargetSpeed")[3].get<float>();

        D_0D0096B8[0] = j.at("D_0D0096B8")[0].get<float>();
        D_0D0096B8[1] = j.at("D_0D0096B8")[1].get<float>();
        D_0D0096B8[2] = j.at("D_0D0096B8")[2].get<float>();
        D_0D0096B8[3] = j.at("D_0D0096B8")[3].get<float>();

        OffTrackTargetSpeed[0] = j.at("OffTrackTargetSpeed")[0].get<float>();
        OffTrackTargetSpeed[1] = j.at("OffTrackTargetSpeed")[1].get<float>();
        OffTrackTargetSpeed[2] = j.at("OffTrackTargetSpeed")[2].get<float>();
        OffTrackTargetSpeed[3] = j.at("OffTrackTargetSpeed")[3].get<float>();
        
        // Deserialize arrays PathTable and PathTable2 similarly
        
        //Clouds = nullptr; // Deserialize if data is present
        //CloudList = nullptr; // Deserialize if data is present
        Minimap.Pos[0].X = j.at("MinimapPosition")[0].get<int32_t>();
        Minimap.Pos[0].Y = j.at("MinimapPosition")[1].get<int32_t>();
        Minimap.Pos[1].X = j.at("MinimapPosition2P")[0].get<int32_t>();
        Minimap.Pos[1].Y = j.at("MinimapPosition2P")[1].get<int32_t>();
        Minimap.PlayerX = j.at("MinimapPlayerX").get<int32_t>();
        Minimap.PlayerY = j.at("MinimapPlayerY").get<int32_t>();
        Minimap.PlayerScaleFactor = j.at("MinimapPlayerScaleFactor").get<float>();
        Minimap.FinishlineX = j.at("MinimapFinishlineX").get<float>();
        Minimap.FinishlineY = j.at("MinimapFinishlineY").get<float>();
        Minimap.Colour.r = j.at("MinimapColour")[0].get<uint8_t>();
        Minimap.Colour.g = j.at("MinimapColour")[1].get<uint8_t>();
        Minimap.Colour.b = j.at("MinimapColour")[2].get<uint8_t>();
        //textures = nullptr; // Deserialize textures if present

        Skybox.TopRight.r = j.at("Skybox")[0].get<uint8_t>();
        Skybox.TopRight.g = j.at("Skybox")[1].get<uint8_t>();
        Skybox.TopRight.b = j.at("Skybox")[2].get<uint8_t>();

        Skybox.BottomRight.r = j.at("Skybox")[3].get<uint8_t>();
        Skybox.BottomRight.g = j.at("Skybox")[4].get<uint8_t>();
        Skybox.BottomRight.b = j.at("Skybox")[5].get<uint8_t>();

        Skybox.BottomLeft.r = j.at("Skybox")[6].get<uint8_t>();
        Skybox.BottomLeft.g = j.at("Skybox")[7].get<uint8_t>();
        Skybox.BottomLeft.b = j.at("Skybox")[8].get<uint8_t>();
        
        Skybox.TopLeft.r = j.at("Skybox")[9].get<uint8_t>();
        Skybox.TopLeft.g = j.at("Skybox")[10].get<uint8_t>();
        Skybox.TopLeft.b = j.at("Skybox")[11].get<uint8_t>();

        Skybox.FloorTopRight.r = j.at("Skybox")[12].get<uint8_t>();
        Skybox.FloorTopRight.g = j.at("Skybox")[13].get<uint8_t>();
        Skybox.FloorTopRight.b = j.at("Skybox")[14].get<uint8_t>();

        Skybox.FloorBottomRight.r = j.at("Skybox")[15].get<uint8_t>();
        Skybox.FloorBottomRight.g = j.at("Skybox")[16].get<uint8_t>();
        Skybox.FloorBottomRight.b = j.at("Skybox")[17].get<uint8_t>();

        Skybox.FloorBottomLeft.r = j.at("Skybox")[18].get<uint8_t>();
        Skybox.FloorBottomLeft.g = j.at("Skybox")[19].get<uint8_t>();
        Skybox.FloorBottomLeft.b = j.at("Skybox")[20].get<uint8_t>();

        Skybox.FloorTopLeft.r = j.at("Skybox")[21].get<uint8_t>();
        Skybox.FloorTopLeft.g = j.at("Skybox")[22].get<uint8_t>();
        Skybox.FloorTopLeft.b = j.at("Skybox")[23].get<uint8_t>();

        Sequence = static_cast<MusicSeq>(j.at("Sequence").get<int>());
        WaterLevel = j.at("WaterLevel").get<float>();
    }
    void SetText(char* name, const char* title, size_t bufferSize) {
        // Copy the title into the name buffer, ensuring it's null-terminated and within bounds
        std::strncpy(name, title, bufferSize - 1);
        name[bufferSize - 1] = '\0';  // Ensure the string is null-terminated
    }

    const char* GetName() {
        return Name;
    }

    void New() {
        SetText(Name, "", sizeof(Name));
        SetText(DebugName, "", sizeof(DebugName));
        SetText(CourseLength, "", sizeof(CourseLength));
    }
#endif

} Properties;

#ifdef __cplusplus


class World; // <-- Forward declare

class Course {

public:
    std::string Id;
    Properties Props;

    // This allows multiple water levels in a map.
    // Ex. DK Jungle where there's a waterfall and you can drive above and below it.
    std::vector<WaterVolume> WaterVolumes;

    bool bSpawnFinishline = true;
    std::optional<FVector> FinishlineSpawnPoint;

    // O2R Loading
    std::shared_ptr<Ship::Archive> RootArchive;
    std::string SceneFilePtr;
    std::string TrackSectionsPtr;

    bool bIsMod = false;
    std::vector<SpawnParams> SpawnList;

    bool bTourEnabled = false;
    std::vector<TourCamera::CameraShot> TourShots;


    virtual ~Course() = default;

    explicit Course();

    virtual void LoadO2R(std::string trackPath); // Load custom track from o2r
    virtual void Load(); // Decompress and load stock courses or from o2r but TrackSectionsPtr must be set.
    virtual void Load(Vtx* vtx, Gfx *gfx); // Load custom track from code. Load must be overridden and then call to this base class method impl.
    virtual void UnLoad();
    virtual void ParseCourseSections(TrackSections* sections, size_t size);

    /**
     * @brief BeginPlay This function is called once at the start of gameplay.
     * Actor spawning should go here.
     */
    virtual void BeginPlay();
    void SpawnActors();
    virtual void TestPath();
    virtual void InitClouds();
    virtual void UpdateClouds(s32, Camera*);
    virtual void SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7);
    virtual void InitCourseObjects();
    virtual void UpdateCourseObjects();
    virtual void RenderCourseObjects(s32 cameraId);
    virtual void SomeSounds();
    virtual void CreditsSpawnActors();
    virtual void WhatDoesThisDo(Player*, int8_t);
    virtual void WhatDoesThisDoAI(Player*, int8_t);
    virtual void SetStaffGhost();
    virtual void Render(struct UnkStruct_800DC5EC*);
    virtual void RenderCredits();
    virtual void Waypoints(Player* player, int8_t playerId);
    virtual f32 GetWaterLevel(FVector pos, Collision* collision);
    virtual void ScrollingTextures();
    // Draw transparent models (water, signs, arrows, etc.) 
    virtual void DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                           uint16_t playerDirection);
    virtual void Destroy();
    virtual bool IsMod();

  private:
    void Init();
};

#endif

#endif // ENGINE_COURSE_H
