#include "waypoints.h"
#include "course.h"
#include "displaylists.h"
#include <libultra/gbi.h>

Gfx test_course_data_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(test_course_dls),
};

TrackWaypoint test_course_path[] = {
    {  100, 0,       0, 1 }, // Point at 0 degrees
    {  92,    0,    -38, 1 }, // Point at 22.5 degrees
    {  71,    0,    -71, 1 }, // Point at 45 degrees
    {  38,    0,    -92, 1 }, // Point at 67.5 degrees
    {   0,    0,   -100, 1 }, // Point at 90 degrees
    { -38,    0,    -92, 1 }, // Point at 112.5 degrees
    { -71,    0,    -71, 1 }, // Point at 135 degrees
    { -92,    0,    -38, 1 }, // Point at 157.5 degrees
    { -100, 0,       0, 1 }, // Point at 180 degrees
    { -92,    0,     38, 1 }, // Point at 202.5 degrees
    { -71,    0,     71, 1 }, // Point at 225 degrees
    { -38,    0,     92, 1 }, // Point at 247.5 degrees
    {   0,    0,    100, 1 }, // Point at 270 degrees
    {  38,    0,     92, 1 }, // Point at 292.5 degrees
    {  71,    0,     71, 1 }, // Point at 315 degrees
    {  92,    0,     38, 1 }  // Point at 337.5 degrees
};

TrackSections test_course_addr[] = {
    { test_course_dls, 1,  1, 0x0000 },
    { 0x00000000, 0, 0, 0x00000 },
};
