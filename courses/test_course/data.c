#include "waypoints.h"
#include "course.h"
#include <libultra/gbi.h>

extern Gfx mario_Plane_mesh[];

Gfx test_course_data_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(mario_Plane_mesh),
};

TrackWaypoint test_course_path[] = {
    { 92,   6,    38,  1 }, // Point at 337.5 degrees
    { 71,   6,    71,  1 }, // Point at 315 degrees
    { 38,   6,    92,  1 }, // Point at 292.5 degrees
    { 0,    6,   100, 1 },  // Point at 270 degrees
    { -38,  6,    92,  1 }, // Point at 247.5 degrees
    { -71,  6,    71,  1 }, // Point at 225 degrees
    { -92,  6,    38,  1 }, // Point at 202.5 degrees
    { -100, 6,    0,   1 }, // Point at 180 degrees
    { -92,  6,   -38,  1 }, // Point at 157.5 degrees
    { -71,  6,   -71,  1 }, // Point at 135 degrees
    { -38,  6,   -92,  1 }, // Point at 112.5 degrees
    { 0,    6,   -100, 1 }, // Point at 90 degrees
    { 38,   6,   -92,  1 }, // Point at 67.5 degrees
    { 71,   6,   -71,  1 }, // Point at 45 degrees
    { 92,   6,   -38,  1 }, // Point at 22.5 degrees
    { 100,  6,    0,   1 }, // Point at 0 degrees
    { -32768, -32768, -32768, 0 }
};

TrackSections test_course_addr[] = {
    { mario_Plane_mesh, 255,  255, 0x0000 },
    { 0x00000000, 0, 0, 0x00000 },
};
