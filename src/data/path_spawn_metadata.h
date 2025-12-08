#ifndef PATH_SPAWN_METADATA_H
#define PATH_SPAWN_METADATA_H

#include "waypoints.h"
#include <common_structs.h>
#include <assets/other/common_data.h>
#include "include/bomb_kart.h"

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ char padA[6];
} TrackPathSizes; // size 0x10

extern TrackPathPoint nullPath;
extern s16 gAIDistances[];
extern s16 gMarioRacewayAIDistances[];
extern s32 D_800DDB20;
extern s32 D_800DDB24;

#endif
