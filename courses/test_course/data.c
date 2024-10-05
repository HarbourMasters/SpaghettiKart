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
    {    0, 0, 0 , 0},
    {    0, 0, -90 , 0},
    {    0, 0, -180 , 0},
    {    0, 0, -270 , 0},
    {    0, 0, -360 , {0}},
    {   150, 0, -360 , {0}},
    {   300, 0, -360 , {0}},
    {   450, 0, -360 , {0}},
    {   600, 0, -360 , {0}},
    {   750, 0, -360 , {0}},
    {   900, 0, -320 , {0}},  // Start curving to the right
    {   950, 0, -250 , {0}},
    {   950, 0, -180 , {0}},
    {   950, 0, -90 , {0}},
    {   950, 0, 0 , 0},       // Right of the starting point
    {   950, 0, 90 , {0}},
    {   950, 0, 180 , {0}},
    {   900, 0, 250 , {0}},
    {   850, 0, 300 , {0}},   // Heading back left
    {   750, 0, 360 , 0},     // Mirroring the curve on the left
    {   600, 0, 360 , 0},
    {   450, 0, 360 , 0},
    {   300, 0, 360 , 0},
    {   150, 0, 360 , 0},
    {    0, 0, 360 , 0},      // Returning to x=0
    {    0, 0, 270 , 0},
    {    0, 0, 180 , 0},
    {    0, 0, 90 , 0},
    { -32768, -32768, -32768, 0 } // Terminator
};

TrackSections test_course_addr[] = {
    { mario_Plane_mesh, 255,  255, 0x0000 },
    { 0x00000000, 0, 0, 0x00000 },
};
