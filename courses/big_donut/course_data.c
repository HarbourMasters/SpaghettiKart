//! @todo Replace addresses with variables
#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <actor_types.h>

#include "course_displaylists.h"

Gfx d_course_big_donut_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(d_course_big_donut_packed_dl_210),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(d_course_big_donut_packed_dl_D20),
    gsSPDisplayList(d_course_big_donut_packed_dl_AC0),
    gsSPDisplayList(d_course_big_donut_packed_dl_450),
    gsSPEndDisplayList(),
};

// Item box spawn locations
struct ActorSpawnData d_course_big_donut_item_box_spawns[] = {
    {{ -40, 222, 824 }, {0} },
    {{ 125, 200, 573 }, {0} },
    {{ -302, 251, 889 }, {0} },
    {{ 38, 203, 654 }, {0} },
    {{ 22, 207, 716 }, {0} },
    {{ 110, 205, 682 }, {0} },
    {{ 681, 205, 53 }, {1} },
    {{ 759, 218, -253 }, {1} },
    {{ 798, 221, 172 }, {1} },
    {{ 557, 200, -111 }, {1} },
    {{ 629, 201, -126 }, {1} },
    {{ 704, 207, -137 }, {1} },
    {{ -111, 200, -554 }, {2} },
    {{ 72, 250, -933 }, {2} },
    {{ -25, 207, -705 }, {2} },
    {{ -21, 225, -843 }, {2} },
    {{ -83, 220, -807 }, {2} },
    {{ -49, 214, -775 }, {2} },
    {{ -807, 220, 53 }, {3} },
    {{ -859, 242, 300 }, {3} },
    {{ -672, 207, -219 }, {3} },
    {{ -885, 108, -59 }, {3} },
    {{ -793, 103, -33 }, {3} },
    {{ -757, 214, 166 }, {3} },
    {{ -32768, 0, 0 }, {0} },
};
