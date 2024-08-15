#ifndef ALL_COURSE_MODEL_H
#define ALL_COURSE_MODEL_H

#include "macros.h"
#include "common_structs.h"

/** WARNING!!!
 * This header file has the sizes of the vertex array's hard coded.
 * If those arrays ever have their sizes changed that MUST be reflected
 * in this file or problems will arise in other parts of the code
 **/

#include <assets/yoshi_valley_vertices.h>
#include <assets/toads_turnpike_vertices.h>
#include <assets/mario_raceway_vertices.h>
#include <assets/luigi_raceway_vertices.h>
#include <assets/royal_raceway_vertices.h>
#include <assets/kalimari_desert_vertices.h>
#include <assets/frappe_snowland_vertices.h>
#include <assets/frappe_snowland_data.h>
#include <assets/wario_stadium_vertices.h>
#include <assets/moo_moo_farm_vertices.h>
#include <assets/big_donut_vertices.h>
#include <assets/choco_mountain_vertices.h>
#include <assets/bowsers_castle_vertices.h>
#include <assets/skyscraper_vertices.h>
#include <assets/block_fort_vertices.h>
#include <assets/double_deck_vertices.h>
#include <assets/banshee_boardwalk_vertices.h>
#include <assets/dks_jungle_parkway_vertices.h>
#include <assets/koopa_troopa_beach_vertices.h>
#include <assets/sherbet_land_vertices.h>
#include <assets/rainbow_road_vertices.h>

#define d_course_big_donut_vertex_count          ARRAY_COUNT(d_course_big_donut_vertex)
#define d_course_block_fort_vertex_count         ARRAY_COUNT(d_course_block_fort_vertex)
#define d_course_double_deck_vertex_count        ARRAY_COUNT(d_course_double_deck_vertex)
#define d_course_skyscraper_vertex_count         ARRAY_COUNT(d_course_skyscraper_vertex)
#define d_course_choco_mountain_vertex_count     ARRAY_COUNT(d_course_choco_mountain_vertex)
#define d_course_frappe_snowland_vertex_count    ARRAY_COUNT(d_course_frappe_snowland_vertex)
#define d_course_toads_turnpike_vertex_count     ARRAY_COUNT(d_course_toads_turnpike_vertex)
#define d_course_kalimari_desert_vertex_count    ARRAY_COUNT(d_course_kalimari_desert_vertex)
#define d_course_koopa_troopa_beach_vertex_count ARRAY_COUNT(d_course_koopa_troopa_beach_vertex)
#define d_course_luigi_raceway_vertex_count      ARRAY_COUNT(d_course_luigi_raceway_vertex)
#define d_course_moo_moo_farm_vertex_count       ARRAY_COUNT(d_course_moo_moo_farm_vertex)
#define d_course_banshee_boardwalk_vertex_count  ARRAY_COUNT(d_course_banshee_boardwalk_vertex)
#define d_course_dks_jungle_parkway_vertex_count ARRAY_COUNT(d_course_dks_jungle_parkway_vertex)
#define d_course_rainbow_road_vertex_count       ARRAY_COUNT(d_course_rainbow_road_vertex)
#define d_course_yoshi_valley_vertex_count       ARRAY_COUNT(d_course_yoshi_valley_vertex)
#define d_course_bowsers_castle_vertex_count     ARRAY_COUNT(d_course_bowsers_castle_vertex)
#define d_course_royal_raceway_vertex_count      ARRAY_COUNT(d_course_royal_raceway_vertex)
#define d_course_sherbet_land_vertex_count       ARRAY_COUNT(d_course_sherbet_land_vertex)
#define d_course_wario_stadium_vertex_count      ARRAY_COUNT(d_course_wario_stadium_vertex)

#endif // ALL_COURSE_MODEL_H
