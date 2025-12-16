#ifndef ALL_COURSE_MODEL_H
#define ALL_COURSE_MODEL_H

#include "macros.h"
#include <common_structs.h>

/** WARNING!!!
 * This header file has the sizes of the vertex array's hard coded.
 * If those arrays ever have their sizes changed that MUST be reflected
 * in this file or problems will arise in other parts of the code
 **/

#include <assets/models/tracks/yoshi_valley/yoshi_valley_vertices.h>
#include <assets/models/tracks/toads_turnpike/toads_turnpike_vertices.h>
#include <assets/models/tracks/mario_raceway/mario_raceway_vertices.h>
#include <assets/models/tracks/luigi_raceway/luigi_raceway_vertices.h>
#include <assets/models/tracks/royal_raceway/royal_raceway_vertices.h>
#include <assets/models/tracks/kalimari_desert/kalimari_desert_vertices.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_vertices.h>
#include <assets/models/tracks/frappe_snowland/frappe_snowland_data.h>
#include <assets/models/tracks/wario_stadium/wario_stadium_vertices.h>
#include <assets/models/tracks/moo_moo_farm/moo_moo_farm_vertices.h>
#include <assets/models/tracks/big_donut/big_donut_vertices.h>
#include <assets/models/tracks/choco_mountain/choco_mountain_vertices.h>
#include <assets/models/tracks/bowsers_castle/bowsers_castle_vertices.h>
#include <assets/models/tracks/skyscraper/skyscraper_vertices.h>
#include <assets/models/tracks/block_fort/block_fort_vertices.h>
#include <assets/models/tracks/double_deck/double_deck_vertices.h>
#include <assets/models/tracks/banshee_boardwalk/banshee_boardwalk_vertices.h>
#include <assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_vertices.h>
#include <assets/models/tracks/koopa_troopa_beach/koopa_troopa_beach_vertices.h>
#include <assets/models/tracks/sherbet_land/sherbet_land_vertices.h>
#include <assets/models/tracks/rainbow_road/rainbow_road_vertices.h>

#define d_course_big_donut_vertex_count ARRAY_COUNT(d_course_big_donut_vertex)
#define d_course_block_fort_vertex_count ARRAY_COUNT(d_course_block_fort_vertex)
#define d_course_double_deck_vertex_count ARRAY_COUNT(d_course_double_deck_vertex)
#define d_course_skyscraper_vertex_count ARRAY_COUNT(d_course_skyscraper_vertex)
#define d_course_choco_mountain_vertex_count ARRAY_COUNT(d_course_choco_mountain_vertex)
#define d_course_frappe_snowland_vertex_count ARRAY_COUNT(d_course_frappe_snowland_vertex)
#define d_course_mario_raceway_vertex_count ARRAY_COUNT(d_course_mario_raceway_vertex)
#define d_course_toads_turnpike_vertex_count ARRAY_COUNT(d_course_toads_turnpike_vertex)
#define d_course_kalimari_desert_vertex_count ARRAY_COUNT(d_course_kalimari_desert_vertex)
#define d_course_koopa_troopa_beach_vertex_count ARRAY_COUNT(d_course_koopa_troopa_beach_vertex)
#define d_course_luigi_raceway_vertex_count ARRAY_COUNT(d_course_luigi_raceway_vertex)
#define d_course_moo_moo_farm_vertex_count ARRAY_COUNT(d_course_moo_moo_farm_vertex)
#define d_course_banshee_boardwalk_vertex_count ARRAY_COUNT(d_course_banshee_boardwalk_vertex)
#define d_course_dks_jungle_parkway_vertex_count ARRAY_COUNT(d_course_dks_jungle_parkway_vertex)
#define d_course_rainbow_road_vertex_count ARRAY_COUNT(d_course_rainbow_road_vertex)
#define d_course_yoshi_valley_vertex_count ARRAY_COUNT(d_course_yoshi_valley_vertex)
#define d_course_bowsers_castle_vertex_count ARRAY_COUNT(d_course_bowsers_castle_vertex)
#define d_course_royal_raceway_vertex_count ARRAY_COUNT(d_course_royal_raceway_vertex)
#define d_course_sherbet_land_vertex_count ARRAY_COUNT(d_course_sherbet_land_vertex)
#define d_course_wario_stadium_vertex_count ARRAY_COUNT(d_course_wario_stadium_vertex)

#endif // ALL_COURSE_MODEL_H
