#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>


// C-compatible function declaration
#ifdef __cplusplus
extern "C" {
#endif

    #include "camera.h"
    #include "course_offsets.h"
    #include "data/some_data.h"
void CourseManager_LoadCourse(const char* courseVtx, 
                      course_texture* textures, const char* displaylists, size_t dlSize);

#ifdef __cplusplus
}
#endif


class Course {

public:
    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load(const char* vtx, const char *gfx);
    virtual void LoadTextures();
    virtual void SpawnActors();
    virtual void Init();
    virtual void InitClouds();
    virtual void UpdateClouds(s32, Camera*);
    virtual void SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7);
    virtual void MinimapSettings();
    virtual void InitCourseObjects();
    virtual void UpdateCourseObjects();
    virtual void RenderCourseObjects();
    virtual void SomeSounds();
    virtual void SetCourseVtxColours();
    virtual void WhatDoesThisDo(Player*, int8_t);
    virtual void WhatDoesThisDoAI(Player*, int8_t);
    virtual void MinimapFinishlinePosition();
    virtual void SetStaffGhost();
    virtual void BeginPlay();
    virtual void Render(struct UnkStruct_800DC5EC*);
    virtual void Collision();
    virtual void Destroy();

    CloudData *clouds = gKalimariDesertClouds;
    CloudData *cloudList = gLuigiRacewayClouds;
    int32_t MinimapFinishlineX = 0;
    int32_t MinimapFinishlineY = 0;
};

#endif // COURSE_H