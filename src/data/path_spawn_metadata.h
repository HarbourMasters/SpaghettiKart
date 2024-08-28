#ifndef PATH_SPAWN_METADATA_H
#define PATH_SPAWN_METADATA_H

#include "waypoints.h"
#include <common_structs.h>
#include <assets/common_data.h>
#include "include/bomb_kart.h"

struct _struct_gCoursePathSizes_0x10 {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ char padA[6];
}; // size 0x10

extern KartAIBehaviour* gKartAIBehaviourLUT[];
extern TrackWaypoint nullPath;
extern TrackWaypoint* gCoursePathTable[][4];
extern TrackWaypoint* gCoursePathTable2[][4];
extern s16 gKartAISteeringSensitivity[];
extern f32 gKartAICourseMaximumSeparation[];
extern f32 gKartAICourseMinimumSeparation[];
extern s16 D_800DCAF4[];
extern s16 D_800DCB34[];
extern s16* D_800DCBB4[];
extern BombKartSpawn gBombKartSpawns[][NUM_BOMB_KARTS_MAX];
extern struct _struct_gCoursePathSizes_0x10 gCoursePathSizes[];
extern s32 D_800DDB20;
extern s32 D_800DDB24;

#endif
