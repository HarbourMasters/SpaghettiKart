#include <libultraship.h>
#include <macros.h>
#include <libultra/gbi.h>
#include <actor_types.h>

#include "course_displaylists.h"

Gfx d_course_skyscraper_dl[] = {
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(d_course_skyscraper_packed_dl_238),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(d_course_skyscraper_packed_dl_FE8),
    gsSPDisplayList(d_course_skyscraper_packed_dl_C60),
    gsSPDisplayList(d_course_skyscraper_packed_dl_B70),
    gsSPDisplayList(d_course_skyscraper_packed_dl_6B8),
    gsSPDisplayList(d_course_skyscraper_packed_dl_570),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPDisplayList(d_course_skyscraper_packed_dl_10C8),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

// Item box spawn locations
struct ActorSpawnData d_course_skyscraper_item_box_spawns[] = {
    {{ -18, 480, 365 }, { 0 }},
    {{ -272, 480, 312 }, { 0 }},
    {{ -312, 480, 359 }, { 0 }},
    {{ -293, 480, 337 }, { 0 }},
    {{ 12, 480, 360 }, { 0 }},
    {{ 363, 480, 14 }, { 1 }},
    {{ 288, 480, 309 }, { 1 }},
    {{ 308, 480, 326 }, { 1 }},
    {{ 325, 480, 345 }, { 1 }},
    {{ 364, 480, -9 }, { 1 }},
    {{ 11, 480, -361 }, { 2 }},
    {{ 281, 480, -306 }, { 2 }},
    {{ 303, 480, -316 }, { 2 }},
    {{ 336, 480, -324 }, { 2 }},
    {{ -11, 480, -363 }, { 2 }},
    {{ -357, 480, -5 }, { 3 }},
    {{ -318, 480, -267 }, { 3 }},
    {{ -357, 480, -312 }, { 3 }},
    {{ -339, 480, -288 }, { 3 }},
    {{ -354, 480, 18 }, { 3 }},
    {{ -66, 485, 0 }, { 4 }},
    {{ 0, 485, -67 }, { 4 }},
    {{ 66, 485, 1 }, { 4 }},
    {{ 1, 485, 67 }, { 4 }},
    {{ -32768, 0, 0 }, { 0 }},
};
