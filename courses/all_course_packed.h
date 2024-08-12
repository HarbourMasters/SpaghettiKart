#ifndef ALL_COURSE_PACKED_H
#define ALL_COURSE_PACKED_H

/** WARNING!!!
 * Each course #defines a token that looks like `d_course_<course_name_here>_packed_end`.
 * These macros only work if the the last Gfx array in each packed.inc.c file has its length hardcoded
 * in the accompanying header file. If that array is resized or moved the #defines will become
 * incorrect
 **/

#include "courses/big_donut/course_displaylists.h"
#include "courses/block_fort/course_displaylists.h"
#include "courses/double_deck/course_displaylists.h"
#include "courses/skyscraper/course_displaylists.h"
#include "courses/choco_mountain/course_displaylists.h"
#include "courses/frappe_snowland/course_displaylists.h"
#include "courses/mario_raceway/course_displaylists.h"
#include "courses/toads_turnpike/course_displaylists.h"
#include "courses/koopa_troopa_beach/course_displaylists.h"
#include "courses/luigi_raceway/course_displaylists.h"
#include "courses/moo_moo_farm/course_displaylists.h"
#include "courses/banshee_boardwalk/course_displaylists.h"
#include "courses/dks_jungle_parkway/course_displaylists.h"
#include "courses/rainbow_road/course_displaylists.h"
#include "courses/yoshi_valley/course_displaylists.h"
#include "courses/bowsers_castle/course_displaylists.h"
#include "courses/sherbet_land/course_displaylists.h"
#include "courses/wario_stadium/course_displaylists.h"

#endif // ALL_COURSE_PACKED_H
