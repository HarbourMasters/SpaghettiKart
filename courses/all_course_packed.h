#ifndef ALL_COURSE_PACKED_H
#define ALL_COURSE_PACKED_H

/** WARNING!!!
 * Each course #defines a token that looks like `d_course_<course_name_here>_packed_end`.
 * These macros only work if the the last Gfx array in each packed.inc.c file has its length hardcoded
 * in the accompanying header file. If that array is resized or moved the #defines will become
 * incorrect
 **/

#include "courses/mario_raceway/course_displaylists.h"
#include "courses/koopa_troopa_beach/course_displaylists.h"
#include "courses/luigi_raceway/course_displaylists.h"
#include "courses/dks_jungle_parkway/course_displaylists.h"
#include "courses/rainbow_road/course_displaylists.h"
#include "courses/yoshi_valley/course_displaylists.h"
#include "courses/sherbet_land/course_displaylists.h"

#include <assets/toads_turnpike_displaylists.h>
#include <assets/banshee_boardwalk_displaylists.h>
#include <assets/mario_raceway_displaylists.h>
#include <assets/luigi_raceway_displaylists.h>
#include <assets/royal_raceway_displaylists.h>
#include <assets/kalimari_desert_displaylists.h>
#include <assets/frappe_snowland_displaylists.h>
#include <assets/wario_stadium_displaylists.h>
#include <assets/moo_moo_farm_displaylists.h>
#include <assets/big_donut_displaylists.h>
#include <assets/choco_mountain_displaylists.h>
#include <assets/bowsers_castle_displaylists.h>
#include <assets/skyscraper_displaylists.h>
#include <assets/block_fort_displaylists.h>
#include <assets/double_deck_displaylists.h>

#endif // ALL_COURSE_PACKED_H
