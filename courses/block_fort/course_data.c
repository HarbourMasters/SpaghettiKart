#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <actor_types.h>

#include "course_displaylists.h"

Gfx d_course_block_fort_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(d_course_block_fort_packed_dl_14A0),
    gsSPDisplayList(d_course_block_fort_packed_dl_9C0),
    gsSPDisplayList(d_course_block_fort_packed_dl_688),
    gsSPDisplayList(d_course_block_fort_packed_dl_3F0),
    gsSPEndDisplayList(),
};

// Item box spawn locations
struct ActorSpawnData d_course_block_fort_item_box_spawns[] = {
    {{ -38, 0, 744 }, {0} },
    {{ 29, 0, 710 }, {0} },
    {{ -13, 0, 685 }, {0} },
    {{ -553, 47, 543 }, {0} },
    {{ -527, 47, 526 }, {0} },
    {{ -141, 47, 153 }, {0} },
    {{ -185, 47, 179 }, {0} },
    {{ -388, 92, 366 }, {0} },
    {{ -428, 92, 424 }, {0} },
    {{ -724, 0, -50 }, {1} },
    {{ -679, 0, -4 }, {1} },
    {{ -736, 0, 37 }, {1} },
    {{ -554, 47, -550 }, {1} },
    {{ -530, 47, -536 }, {1} },
    {{ -170, 47, -172 }, {1} },
    {{ -138, 47, -133 }, {1} },
    {{ -433, 92, -429 }, {1} },
    {{ -379, 92, -390 }, {1} },
    {{ -61, 0, -728 }, {2} },
    {{ -17, 0, -670 }, {2} },
    {{ 26, 0, -708 }, {2} },
    {{ 549, 47, -548 }, {2} },
    {{ 527, 47, -532 }, {2} },
    {{ 148, 47, -146 }, {2} },
    {{ 192, 47, -181 }, {2} },
    {{ 471, 92, -455 }, {2} },
    {{ 414, 92, -426 }, {2} },
    {{ 728, 0, -36 }, {3} },
    {{ 676, 0, -4 }, {3} },
    {{ 700, 0, 31 }, {3} },
    {{ 140, 47, 163 }, {3} },
    {{ 178, 47, 201 }, {3} },
    {{ 529, 47, 532 }, {3} },
    {{ 554, 47, 557 }, {3} },
    {{ 470, 92, 482 }, {3} },
    {{ 438, 92, 442 }, {3} },
    {{ -32768, 0, 0 }, {0} },
};
