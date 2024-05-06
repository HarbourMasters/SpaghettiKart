#ifndef ALL_COURSE_MODEL_H
#define ALL_COURSE_MODEL_H

#include "macros.h"
#include "common_structs.h"

/** WARNING!!!
 * This header file has the sizes of the vertex array's hard coded.
 * If those arrays ever have their sizes changed that MUST be reflected
 * in this file or problems will arise in other parts of the code
 **/

extern CourseVtx d_course_big_donut_vertex[];
extern CourseVtx d_course_block_fort_vertex[];
extern CourseVtx d_course_double_deck_vertex[];
extern CourseVtx d_course_skyscraper_vertex[];
extern CourseVtx d_course_choco_mountain_vertex[];
extern CourseVtx d_course_frappe_snowland_vertex[];
extern CourseVtx d_course_mario_raceway_vertex[0x167D];
extern CourseVtx d_course_toads_turnpike_vertex[];
extern CourseVtx d_course_kalimari_desert_vertex[];
extern CourseVtx d_course_koopa_troopa_beach_vertex[];
extern CourseVtx d_course_luigi_raceway_vertex[];
extern CourseVtx d_course_moo_moo_farm_vertex[];
extern CourseVtx d_course_banshee_boardwalk_vertex[];
extern CourseVtx d_course_dks_jungle_parkway_vertex[];
extern CourseVtx d_course_rainbow_road_vertex[];
extern CourseVtx d_course_yoshi_valley_vertex[];
extern CourseVtx d_course_bowsers_castle_vertex[];
extern CourseVtx d_course_royal_raceway_vertex[];
extern CourseVtx d_course_sherbet_land_vertex[];
extern CourseVtx d_course_wario_stadium_vertex[];

#define d_course_big_donut_vertex_count          ARRAY_COUNT(d_course_big_donut_vertex)
#define d_course_block_fort_vertex_count         ARRAY_COUNT(d_course_block_fort_vertex)
#define d_course_double_deck_vertex_count        ARRAY_COUNT(d_course_double_deck_vertex)
#define d_course_skyscraper_vertex_count         ARRAY_COUNT(d_course_skyscraper_vertex)
#define d_course_choco_mountain_vertex_count     ARRAY_COUNT(d_course_choco_mountain_vertex)
#define d_course_frappe_snowland_vertex_count    ARRAY_COUNT(d_course_frappe_snowland_vertex)
#define d_course_mario_raceway_vertex_count      ARRAY_COUNT(d_course_mario_raceway_vertex)
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
