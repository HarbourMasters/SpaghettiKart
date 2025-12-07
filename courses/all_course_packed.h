#ifndef ALL_COURSE_PACKED_H
#define ALL_COURSE_PACKED_H

/** WARNING!!!
 * Each course #defines a token that looks like `d_course_<course_name_here>_packed_end`.
 * These macros only work if the the last Gfx array in each packed.inc.c file has its length hardcoded
 * in the accompanying header file. If that array is resized or moved the #defines will become
 * incorrect
 **/

#include <assets/models/tracks/rainbow_road/rainbow_road_displaylists.h>
#include <assets/models/tracks/sherbet_land/sherbet_land_displaylists.h>
#include <assets/models/tracks/koopa_troopa_beach/koopa_troopa_beach_displaylists.h>
#include <assets/models/tracks/yoshi_valley/yoshi_valley_displaylists.h>
#include <assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_displaylists.h>
#include <assets/models/tracks/toads_turnpike/toads_turnpike_displaylists.h>
#include <assets/models/tracks/banshee_boardwalk/banshee_boardwalk_displaylists.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_displaylists.h>
#include <assets/models/tracks/luigi_raceway/luigi_raceway_displaylists.h>
#include <assets/models/tracks/royal_raceway/royal_raceway_displaylists.h>
#include <assets/models/tracks/kalimari_desert/kalimari_desert_displaylists.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_displaylists.h>
#include <assets/models/tracks/wario_stadium/wario_stadium_displaylists.h>
#include <assets/models/tracks/moo_moo_farm/moo_moo_farm_displaylists.h>
#include <assets/models/tracks/big_donut/big_donut_displaylists.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_displaylists.h>
#include <assets/models/tracks/bowsers_castle/bowsers_castle_displaylists.h>
#include <assets/models/tracks/skyscraper/skyscraper_displaylists.h>
#include <assets/models/tracks/block_fort/block_fort_displaylists.h>
#include <assets/models/tracks/double_deck/double_deck_displaylists.h>

#endif // ALL_COURSE_PACKED_H
