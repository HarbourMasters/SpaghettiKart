#include "data/path_spawn_metadata.h"
#include "courses/all_course_data.h"
#include <assets/ceremony_data.h>
#include <assets/mario_raceway_data.h>
#include <assets/luigi_raceway_data.h>
#include <assets/royal_raceway_data.h>
#include <assets/kalimari_desert_data.h>

// @warning Array contains an extra zero element at the end.
KartAIBehaviour *gKartAIBehaviourLUT[] = {
    #include "course_metadata/gKartAIBehaviourLUT.inc.c"

};

TrackWaypoint nullPath = { 0x8000, 0x0000, 0x0000, 0x0000 };

TrackWaypoint *gCoursePathTable[][4] = {
    #include "course_metadata/gCoursePathTableUnknown.inc.c"
};

TrackWaypoint *gCoursePathTable2[][4] = {
    #include "course_metadata/gCoursePathTable.inc.c"
};

// @warning Array contains an extra zero element at the end.
s16 gKartAISteeringSensitivity[] = {
    #include "course_metadata/gCPUSteeringSensitivity.inc.c"
};

// Possibly maximum cpu separation
f32 gKartAICourseMaximumSeparation[] = {
    #include "course_metadata/gKartAICourseMaximumSeparation.inc.c"
};

// Possibly minimum cpu separation
f32 gKartAICourseMinimumSeparation[] = {
    #include "course_metadata/gKartAICourseMinimumSeparation.inc.c"
};

// I think the types for D_800DCAF4, D_800DCB34, and D_800DCBB4 are all
// wrong in some way based on their usage in func_800088D8
// But I cannot be bothered to figure it out
// Used as a ptr in D_800DCBB4
s16 D_800DCAF4[] = {
    0x0014, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x001e, 0x0019, 0x0032, 0x004b, 0x0064, 0x007d, 0x0096, 0x00af,
    0x0028, 0x001e, 0x003c, 0x005a, 0x0078, 0x0096, 0x00b4, 0x00d2,
    0x0032, 0x0028, 0x0050, 0x0078, 0x00a0, 0x00c8, 0x00f0, 0x0118,
};

// Used as a ptr in D_800DCBB4
s16 D_800DCB34[] = {
    0x0014, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x001e, 0x0019, 0x002d, 0x0041, 0x005a, 0x0073, 0x008c, 0x00a5,
    0x0028, 0x0003, 0x0006, 0x0010, 0x002e, 0x0031, 0x003b, 0x0059,
    0x0032, 0x001e, 0x003c, 0x003f, 0x0049, 0x004e, 0x006c, 0x008a,
    0x000a, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
};

s16 *D_800DCBB4[] = {
    #include "course_metadata/D_800DCBB4.inc.c"
};

BombKartSpawn gBombKartSpawns[][NUM_BOMB_KARTS_MAX] = {
    #include "course_metadata/gBombKartSpawns.inc.c"
};

struct _struct_gCoursePathSizes_0x10 gCoursePathSizes[] = {
    #include "course_metadata/gCoursePathSizes.inc.c"
};

s32 D_800DDB20 = 0x00000000;

s32 D_800DDB24 = 0x00000001;
