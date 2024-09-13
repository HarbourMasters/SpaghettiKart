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
    virtual void Collision() override;
    virtual void Destroy() override;

    CloudData *clouds = gKalimariDesertClouds;
    CloudData *cloudList = gLuigiRacewayClouds;
    int32_t MinimapFinishlineX = 0;
    int32_t MinimapFinishlineY = 0;
};
