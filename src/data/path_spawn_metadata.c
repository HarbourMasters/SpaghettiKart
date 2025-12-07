#include "data/path_spawn_metadata.h"
#include "courses/all_course_data.h"
#include <assets/models/ceremony_data.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_data.h>
#include <assets/models/tracks/luigi_raceway/luigi_raceway_data.h>
#include <assets/models/tracks/royal_raceway/royal_raceway_data.h>
#include <assets/models/tracks/kalimari_desert/kalimari_desert_data.h>
#include <assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_data.h>
#include <assets/models/tracks/wario_stadium/wario_stadium_data.h>
#include <assets/models/tracks/bowsers_castle/bowsers_castle_data.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_data.h>
#include <assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h>

TrackPathPoint nullPath = { 0x8000, 0x0000, 0x0000, 0x0000 };

// I think the types for gAIDistances, gMarioRacewayAIDistances, and D_800DCBB4 are all
// wrong in some way based on their usage in func_800088D8
// But I cannot be bothered to figure it out
// Used as a ptr in D_800DCBB4
s16 gAIDistances[] = {
    0x0014, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x001e, 0x0019, 0x0032, 0x004b, 0x0064, 0x007d, 0x0096, 0x00af,
    0x0028, 0x001e, 0x003c, 0x005a, 0x0078, 0x0096, 0x00b4, 0x00d2,
    0x0032, 0x0028, 0x0050, 0x0078, 0x00a0, 0x00c8, 0x00f0, 0x0118,
};

// Used as a ptr in D_800DCBB4
s16 gMarioRacewayAIDistances[] = {
    0x0014, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x001e, 0x0019, 0x002d, 0x0041, 0x005a, 0x0073, 0x008c, 0x00a5,
    0x0028, 0x0003, 0x0006, 0x0010, 0x002e, 0x0031, 0x003b, 0x0059,
    0x0032, 0x001e, 0x003c, 0x003f, 0x0049, 0x004e, 0x006c, 0x008a,
};

s16 some_data[] = { // Not sure what this is for
    0x000a, 0x0005, 0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0023,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x000a, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
};

s32 D_800DDB20 = 0x00000000;

s32 D_800DDB24 = 0x00000001;
