#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "common_structs.h"
#include "bomb_kart.h"
#include "objects.h"
#include "path_spawn_metadata.h"
#include "waypoints.h"

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
    const char* MinimapTexture;
    s16 D_800E5548[2];
    float AIMaximumSeparation;
    float AIMinimumSeparation;
    float NearPersp;
    float FarPersp;
    s16 *SomePtr;
    u32 AISteeringSensitivity;
    _struct_gCoursePathSizes_0x10 PathSizes;
    Vec4f D_0D009418;
    Vec4f D_0D009568;
    Vec4f D_0D0096B8;
    Vec4f D_0D009808;
    TrackWaypoint* PathTable[4];
    TrackWaypoint* PathTable2[4];
    CloudData *Clouds;
    CloudData *CloudList;
    s32 MinimapFinishlineX;
    s32 MinimapFinishlineY;
    SkyboxColours Skybox;
} CProperties;

#endif // __ENGINE_H__