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
    #include "defines.h"
    #include "bomb_kart.h"
    #include "path_spawn_metadata.h"
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
    virtual void RenderCredits();
    virtual void Collision();
    virtual void GenerateCollision();
    virtual void Destroy();

    const char* name = "Course Name";
    const char* debugName = "CName";
    const char* courseLength = "567m";
    uint32_t cup = FLOWER_CUP;
    uint32_t cupIndex = 3;
    const char* KartAIBehaviour = D_0D008F28;
    float KartAIMaximumseparation = 50.0f;
    float MinimumAISeparation = 0.3f;
    int16_t *somePtr = D_800DCB34;
    uint32_t AISteeringSensitivity = 48;
    BombKartSpawn gBombKartSpawns[7] = {
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

    const char *PathTable[4] = {
        nullptr, nullptr, nullptr, nullptr
    };

    CloudData *clouds = gKalimariDesertClouds;
    CloudData *cloudList = gLuigiRacewayClouds;
    int32_t MinimapFinishlineX = 0;
    int32_t MinimapFinishlineY = 0;
};

#endif // COURSE_H