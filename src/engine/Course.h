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
    #include "Engine.h"
void CourseManager_LoadCourse(const char* courseVtx, 
                      course_texture* textures, const char* displaylists, size_t dlSize);

CProperties *CourseManager_GetProperties();
struct _struct_gCoursePathSizes_0x10 *CourseManager_GetPathSizes();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class Course {

public:

    typedef struct {
        const char* Name;
        const char* DebugName;
        const char* CourseLength;
        uint32_t Cup;
        uint32_t CupIndex;
        const char* AIBehaviour;
        float AIMaximumSeparation;
        float AIMinimumSeparation;
        int16_t *SomePtr;
        uint32_t AISteeringSensitivity;
        std::vector<BombKartSpawn> BombKartSpawns;
        struct _struct_gCoursePathSizes_0x10 PathSizes;
        Vec4f D_0D009418;
        Vec4f D_0D009568;
        Vec4f D_0D0096B8;
        Vec4f D_0D009808;
        const char* PathTable[4];
        CloudData *Clouds;
        CloudData *CloudList;
        int32_t MinimapFinishlineX;
        int32_t MinimapFinishlineY;
    } Properties;

    Properties Props;

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

};

#endif

#endif // COURSE_H