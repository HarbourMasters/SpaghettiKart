#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "common_structs.h"
#include "bomb_kart.h"
#include "objects.h"
#include "path_spawn_metadata.h"

typedef struct {

    const char* Name;
    const char* DebugName;
    const char* CourseLength;
    u32 Cup;
    u32 CupIndex;
    const char* AIBehaviour;
    float AIMaximumSeparation;
    float AIMinimumSeparation;
    s16 *SomePtr;
    u32 AISteeringSensitivity;
    struct _struct_gCoursePathSizes_0x10 PathSizes;
    Vec4f D_0D009418;
    Vec4f D_0D009568;
    Vec4f D_0D0096B8;
    Vec4f D_0D009808;
    const char* PathTable[4];
    const char* PathTable2[4];
    CloudData *Clouds;
    CloudData *CloudList;
    s32 MinimapFinishlineX;
    s32 MinimapFinishlineY;
    Color_RGB8 Colours;
    Color_RGB8 Colours2;
    Color_RGB8 Colours3;
    Color_RGB8 Colours4;
} CProperties;

#endif // __ENGINE_H__