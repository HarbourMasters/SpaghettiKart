#include "waypoints.h"
#include "course.h"
#include <libultra/gbi.h>

extern Gfx mario_Plane_001_mesh[];

Gfx test_course_data_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(mario_Plane_001_mesh),
};

TrackPathPoint test_course_path[] = {
    { 0, 0, 0, 0 },
    { 0, 0, -100, 1 },
    { 0, 0, -200, 1 },
    { 0, 0, -300, 1 },
    { 0, 0, -400, 1 },
    { 0, 0, -500, 1 },
    { 0, 0, -600, 1 },
    { 0, 0, -700, 1 },
    { 0, 0, -800, 1 },
    { 0, 0, -900, 1 },
    { 0, 0, -1000, 1 },
    { 0, 0, -1096, 1 }, // Main point 1
    { 100, 0, -1090, 2 },
    { 200, 0, -1085, 2 },
    { 300, 0, -1080, 2 },
    { 400, 0, -1075, 2 },
    { 500, 0, -1072, 2 }, // Curve begins to smooth here
    { 600, 0, -1068, 2 },
    { 700, 0, -1065, 2 },
    { 800, 0, -1063, 2 },
    { 900, 0, -1061, 2 },
    { 984, 0, -1060, 2 }, // Main point 2
    { 990, 0, -900, 3 },
    { 995, 0, -800, 3 },
    { 997, 0, -700, 3 },
    { 998, 0, -600, 3 },
    { 999, 0, -500, 3 },
    { 999, 0, -400, 3 },
    { 999, 0, -300, 3 },
    { 999, 0, -200, 3 },
    { 999, 0, -100, 3 },
    { 999, 0, 0, 3 },
    { 999, 0, 100, 3 },
    { 999, 0, 200, 3 },
    { 999, 0, 300, 3 },
    { 999, 0, 400, 3 },
    { 999, 0, 500, 3 },
    { 999, 0, 600, 3 },
    { 999, 0, 700, 3 },
    { 999, 0, 800, 3 },
    { 999, 0, 900, 3 },
    { 999, 0, 940, 3 }, // Main point 3
    { 900, 0, 945, 4 },
    { 800, 0, 945, 4 },
    { 700, 0, 947, 4 },
    { 600, 0, 948, 4 },
    { 500, 0, 949, 4 },
    { 400, 0, 949, 4 },
    { 300, 0, 949, 4 },
    { 200, 0, 950, 4 },
    { 100, 0, 950, 4 },

    // End of path
    { -32768, -32768, -32768, 0 } // Terminator
};

TrackSections test_course_addr[] = {
    { mario_Plane_001_mesh, 255, 255, 0x0000 },
    { 0x00000000, 0, 0, 0x00000 },
};
