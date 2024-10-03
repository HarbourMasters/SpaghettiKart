#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>


// C-compatible function declaration
#ifdef __cplusplus
//#include "World.h"
extern "C" {
#endif

    #include "camera.h"
    #include "course_offsets.h"
    #include "data/some_data.h"
    #include "defines.h"
    #include "bomb_kart.h"
    #include "path_spawn_metadata.h"
    #include "Engine.h"

CProperties *CourseManager_GetProps();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class World; // <-- Forward declare


class Course {

public:
    typedef struct {
        uint8_t r, g, b;
    } RGB8;

    typedef struct {
        RGB8 TopRight;
        RGB8 BottomRight;
        RGB8 BottomLeft;
        RGB8 TopLeft;
        RGB8 FloorTopRight;
        RGB8 FloorBottomRight;
        RGB8 FloorBottomLeft;
        RGB8 FloorTopLeft;
    } SkyboxColours;


    typedef struct {
        const char* Id;
        const char* Name;
        const char* DebugName;
        const char* CourseLength;
        const char* AIBehaviour;
        float AIMaximumSeparation;
        float AIMinimumSeparation;
        float NearPersp;
        float FarPersp;
        int16_t *SomePtr;
        uint32_t AISteeringSensitivity;
        _struct_gCoursePathSizes_0x10 PathSizes;
        Vec4f D_0D009418;
        Vec4f D_0D009568;
        Vec4f D_0D0096B8;
        Vec4f D_0D009808;
        const char* PathTable[4];
        const char* PathTable2[4];
        CloudData *Clouds;
        CloudData *CloudList;
        int32_t MinimapFinishlineX;
        int32_t MinimapFinishlineY;
        SkyboxColours Skybox;
    } Properties;

    Properties Props;

    const char* vtx = nullptr;
    const char* gfx = nullptr;
    size_t gfxSize = 0;
    const course_texture* textures = nullptr;

    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load();
    virtual void Load(Vtx* vtx, Gfx *gfx);
    // This function may not be needed due to otr system.
    virtual void LoadTextures();
    virtual void SpawnActors();
    virtual void Init();
    virtual void InitClouds();
    virtual void UpdateClouds(s32, Camera*);
    virtual void SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7);
    virtual void MinimapSettings();
    virtual void InitCourseObjects();
    virtual void UpdateCourseObjects();
    virtual void RenderCourseObjects(s32 cameraId);
    virtual void SomeSounds();
    virtual void SetCourseVtxColours();
    virtual void WhatDoesThisDo(Player*, int8_t);
    virtual void WhatDoesThisDoAI(Player*, int8_t);
    virtual void MinimapFinishlinePosition();
    virtual void SetStaffGhost();
    virtual void SpawnBombKarts();
    virtual void BeginPlay();
    virtual void Render(struct UnkStruct_800DC5EC*);
    virtual void RenderCredits();
    virtual void SpawnVehicles();
    virtual void UpdateVehicles();
    virtual void Waypoints(Player* player, int8_t playerId);
    virtual void Collision();
    virtual void GenerateCollision();
    virtual void Water();
    virtual void Destroy();
    virtual World* GetWorld();

};

#endif

#endif // COURSE_H