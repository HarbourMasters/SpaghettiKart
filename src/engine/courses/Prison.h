#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    #include "courses/Prison/grass.h"
    #include "courses/Prison/track.h"
    extern const course_texture test_course_textures[];
}

class PrisonCourse : public Course {
public:
    virtual ~PrisonCourse() = default; // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit PrisonCourse();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void LoadTextures() override;
    virtual void BeginPlay() override;
    virtual void MinimapSettings() override;
    //virtual void InitCourseObjects() override;
    //virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void MinimapFinishlinePosition() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    //virtual void RenderCredits() override;    
    //virtual void Collision() override;
    //virtual void SpawnVehicles() override;
    //virtual void UpdateVehicles() override;
    //virtual void UpdateCourseObjects() override;
    //virtual void Destroy() override;
    virtual bool IsMod() override;
};
