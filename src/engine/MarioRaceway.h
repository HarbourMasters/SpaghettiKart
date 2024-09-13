#pragma once

#include <libultraship.h>
#include "Course.h"
#include "mario_raceway_vertices.h"
#include "mario_raceway_displaylists.h"

extern "C" {
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    #include "mario_raceway_data.h"
    extern const course_texture mario_raceway_textures[];
}

class CourseMarioRaceway : public Course {
public:
    virtual ~CourseMarioRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit CourseMarioRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void LoadTextures() override;
    virtual void SpawnActors() override;
    virtual void Init() override;
    virtual void InitClouds() override;
    virtual void UpdateClouds(s32, Camera*) override;
    virtual void MinimapSettings() override;
    virtual void InitCourseObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void MinimapFinishlinePosition() override;
    virtual void SetStaffGhost() override;
    virtual void BeginPlay() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void Collision() override;
    virtual void GenerateCollision() override;
    virtual void Destroy() override;

    const char* name = "Mario Raceway";
    const char* debugName = "m circuit";
    const char* courseLength = "567m";
    uint32_t cup = FLOWER_CUP;
    uint32_t cupIndex = 3;
    const char* KartAIBehaviour = D_0D008F28;
    float KartAIMaximumseparation = 50.0f;
    float MinimumAISeparation = 0.3f;
    int16_t *somePtr = D_800DCB34;
    uint32_t AISteeringSensitivity = 48;
    BombKartSpawn BombKartSpawns[7] = {
        { 40, 3, 0.8333333, 0, 0, 0, 0 },
        { 100, 3, 0.8333333, 0, 0, 0, 0 },
        { 265, 3, 0.8333333, 0, 0, 0, 0 },
        { 285, 1, 0.8333333, 0, 0, 0, 0 },
        { 420, 1, 0.8333333, 0, 0, 0, 0 },
        { 0, 0, 0.8333333, 0, 0, 0, 0 },
        { 0, 0, 0.8333333, 0, 0, 0, 0 },
    };
    struct _struct_gCoursePathSizes_0x10 PathSizes = {600, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Vec4f D_0D009418 = {4.1666665f, 5.5833334f, 6.1666665f, 6.75f};
    Vec4f D_0D009568 = {3.75f, 5.1666665f, 5.75f, 6.3333334f};
    Vec4f D_0D0096B8 = {3.3333332f, 3.9166667f, 4.5f, 5.0833334f};
    Vec4f D_0D009808 = {3.75f, 5.1666665f, 5.75f, 6.3333334f};

    const char* PathTable[4] = {
        d_course_mario_raceway_unknown_waypoints,
        nullptr, nullptr, nullptr
    };

    CloudData *clouds = gKalimariDesertClouds;
    CloudData *cloudList = gLuigiRacewayClouds;
    int32_t MinimapFinishlineX = 0;
    int32_t MinimapFinishlineY = 0;
};
