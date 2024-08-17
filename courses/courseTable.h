#ifndef COURSE_TABLE_H
#define COURSE_TABLE_H

#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>

// dlRomStart, vertexRomStart, & vertexStart, are u8* because mio0 compressed.
struct CourseTable {
    u8 *dlRomStart;     // 0x00 ROM start for segment 6 DL data
    u8 *dlRomEnd;       // 0x04 ROM end for segment 6 DL data
    u8 *vertexRomStart; // 0x08 ROM start for segment 4 vertex data
    u8 *vertexRomEnd;   // 0x0C ROM end for segment 7?
    u8 *offsetRomStart; // 0x10 ROM start for uncompressed segment 9 texture and DL addresses
    u8 *offsetRomEnd;   // 0x14 ROM end for uncompressed segment 9 texture and DL addresses
    CourseVtx *vertexStart;    // 0x18 segmented address of vertex data
    u32 vertexCount;    // 0x1C number of vertices in vertex data
    u8 *packedStart;    // 0x20 packed display list start address
    u8 *finalDisplaylistOffset;        // 0x24
    u32 *textures;       // 0x20 segmented address of textures table
    u16 unknown1;       // 0x2C
    u16 padding;        // 0x2E
};

extern struct CourseTable gCourseTable[];

//! @todo These should probably be ptr's instead of array's?
extern u8 _course_mario_raceway_dl_mio0SegmentRomStart[];
extern u8 _course_mario_raceway_dl_mio0SegmentRomEnd[];
extern u8 _mario_raceway_vertexSegmentRomStart[];
extern u8 _mario_raceway_vertexSegmentRomEnd[];
extern u8 _course_mario_raceway_offsetsSegmentRomStart[];
extern u8 _course_mario_raceway_offsetsSegmentRomEnd[];
extern u8 d_course_mario_raceway_packed[];

extern u8 _course_choco_mountain_dl_mio0SegmentRomStart[];
extern u8 _course_choco_mountain_dl_mio0SegmentRomEnd[];
extern u8 _choco_mountain_vertexSegmentRomStart[];
extern u8 _choco_mountain_vertexSegmentRomEnd[];
extern u8 _course_choco_mountain_offsetsSegmentRomStart[];
extern u8 _course_choco_mountain_offsetsSegmentRomEnd[];
extern u8 d_course_choco_mountain_packed[];

extern u8 _course_bowsers_castle_dl_mio0SegmentRomStart[];
extern u8 _course_bowsers_castle_dl_mio0SegmentRomEnd[];
extern u8 _bowsers_castle_vertexSegmentRomStart[];
extern u8 _bowsers_castle_vertexSegmentRomEnd[];
extern u8 _course_bowsers_castle_offsetsSegmentRomStart[];
extern u8 _course_bowsers_castle_offsetsSegmentRomEnd[];
extern u8 d_course_bowsers_castle_packed[];

extern u8 _course_banshee_boardwalk_dl_mio0SegmentRomStart[];
extern u8 _course_banshee_boardwalk_dl_mio0SegmentRomEnd[];
extern u8 _banshee_boardwalk_vertexSegmentRomStart[];
extern u8 _banshee_boardwalk_vertexSegmentRomEnd[];
extern u8 _course_banshee_boardwalk_offsetsSegmentRomStart[];
extern u8 _course_banshee_boardwalk_offsetsSegmentRomEnd[];
extern u8 d_course_banshee_boardwalk_packed[];

extern u8 _course_yoshi_valley_dl_mio0SegmentRomStart[];
extern u8 _course_yoshi_valley_dl_mio0SegmentRomEnd[];
extern u8 _yoshi_valley_vertexSegmentRomStart[];
extern u8 _yoshi_valley_vertexSegmentRomEnd[];
extern u8 _course_yoshi_valley_offsetsSegmentRomStart[];
extern u8 _course_yoshi_valley_offsetsSegmentRomEnd[];
extern u8 d_course_yoshi_valley_packed[];

extern u8 _course_frappe_snowland_dl_mio0SegmentRomStart[];
extern u8 _course_frappe_snowland_dl_mio0SegmentRomEnd[];
extern u8 _frappe_snowland_vertexSegmentRomStart[];
extern u8 _frappe_snowland_vertexSegmentRomEnd[];
extern u8 _course_frappe_snowland_offsetsSegmentRomStart[];
extern u8 _course_frappe_snowland_offsetsSegmentRomEnd[];
extern u8 d_course_frappe_snowland_packed[];

extern u8 _course_koopa_troopa_beach_dl_mio0SegmentRomStart[];
extern u8 _course_koopa_troopa_beach_dl_mio0SegmentRomEnd[];
extern u8 _koopa_troopa_beach_vertexSegmentRomStart[];
extern u8 _koopa_troopa_beach_vertexSegmentRomEnd[];
extern u8 _course_koopa_troopa_beach_offsetsSegmentRomStart[];
extern u8 _course_koopa_troopa_beach_offsetsSegmentRomEnd[];
extern u8 d_course_koopa_troopa_beach_packed[];

extern u8 _course_royal_raceway_dl_mio0SegmentRomStart[];
extern u8 _course_royal_raceway_dl_mio0SegmentRomEnd[];
extern u8 _royal_raceway_vertexSegmentRomStart[];
extern u8 _royal_raceway_vertexSegmentRomEnd[];
extern u8 _course_royal_raceway_offsetsSegmentRomStart[];
extern u8 _course_royal_raceway_offsetsSegmentRomEnd[];
extern u8 d_course_royal_raceway_packed[];

extern u8 _course_luigi_raceway_dl_mio0SegmentRomStart[];
extern u8 _course_luigi_raceway_dl_mio0SegmentRomEnd[];
extern u8 _luigi_raceway_vertexSegmentRomStart[];
extern u8 _luigi_raceway_vertexSegmentRomEnd[];
extern u8 _course_luigi_raceway_offsetsSegmentRomStart[];
extern u8 _course_luigi_raceway_offsetsSegmentRomEnd[];
extern u8 d_course_luigi_raceway_packed[];

extern u8 _course_moo_moo_farm_dl_mio0SegmentRomStart[];
extern u8 _course_moo_moo_farm_dl_mio0SegmentRomEnd[];
extern u8 _moo_moo_farm_vertexSegmentRomStart[];
extern u8 _moo_moo_farm_vertexSegmentRomEnd[];
extern u8 _course_moo_moo_farm_offsetsSegmentRomStart[];
extern u8 _course_moo_moo_farm_offsetsSegmentRomEnd[];
extern u8 d_course_moo_moo_farm_packed[];

extern u8 _course_toads_turnpike_dl_mio0SegmentRomStart[];
extern u8 _course_toads_turnpike_dl_mio0SegmentRomEnd[];
extern u8 _toads_turnpike_vertexSegmentRomStart[];
extern u8 _toads_turnpike_vertexSegmentRomEnd[];
extern u8 _course_toads_turnpike_offsetsSegmentRomStart[];
extern u8 _course_toads_turnpike_offsetsSegmentRomEnd[];
extern u8 d_course_toads_turnpike_packed[];

extern u8 _course_kalimari_desert_dl_mio0SegmentRomStart[];
extern u8 _course_kalimari_desert_dl_mio0SegmentRomEnd[];
extern u8 _kalimari_desert_vertexSegmentRomStart[];
extern u8 _kalimari_desert_vertexSegmentRomEnd[];
extern u8 _course_kalimari_desert_offsetsSegmentRomStart[];
extern u8 _course_kalimari_desert_offsetsSegmentRomEnd[];
extern u8 d_course_kalimari_desert_packed[];

extern u8 _course_sherbet_land_dl_mio0SegmentRomStart[];
extern u8 _course_sherbet_land_dl_mio0SegmentRomEnd[];
extern u8 _sherbet_land_vertexSegmentRomStart[];
extern u8 _sherbet_land_vertexSegmentRomEnd[];
extern u8 _course_sherbet_land_offsetsSegmentRomStart[];
extern u8 _course_sherbet_land_offsetsSegmentRomEnd[];
extern u8 d_course_sherbet_land_packed[];

extern u8 _course_rainbow_road_dl_mio0SegmentRomStart[];
extern u8 _course_rainbow_road_dl_mio0SegmentRomEnd[];
extern u8 _rainbow_road_vertexSegmentRomStart[];
extern u8 _rainbow_road_vertexSegmentRomEnd[];
extern u8 _course_rainbow_road_offsetsSegmentRomStart[];
extern u8 _course_rainbow_road_offsetsSegmentRomEnd[];
extern u8 d_course_rainbow_road_packed[];

extern u8 _course_wario_stadium_dl_mio0SegmentRomStart[];
extern u8 _course_wario_stadium_dl_mio0SegmentRomEnd[];
extern u8 _wario_stadium_vertexSegmentRomStart[];
extern u8 _wario_stadium_vertexSegmentRomEnd[];
extern u8 _course_wario_stadium_offsetsSegmentRomStart[];
extern u8 _course_wario_stadium_offsetsSegmentRomEnd[];
extern u8 d_course_wario_stadium_packed[];

extern u8 _course_block_fort_dl_mio0SegmentRomStart[];
extern u8 _course_block_fort_dl_mio0SegmentRomEnd[];
extern u8 _block_fort_vertexSegmentRomStart[];
extern u8 _block_fort_vertexSegmentRomEnd[];
extern u8 _course_block_fort_offsetsSegmentRomStart[];
extern u8 _course_block_fort_offsetsSegmentRomEnd[];
extern u8 d_course_block_fort_packed[];

extern u8 _course_skyscraper_dl_mio0SegmentRomStart[];
extern u8 _course_skyscraper_dl_mio0SegmentRomEnd[];
extern u8 _skyscraper_vertexSegmentRomStart[];
extern u8 _skyscraper_vertexSegmentRomEnd[];
extern u8 _course_skyscraper_offsetsSegmentRomStart[];
extern u8 _course_skyscraper_offsetsSegmentRomEnd[];
extern u8 d_course_skyscraper_packed[];

extern u8 _course_double_deck_dl_mio0SegmentRomStart[];
extern u8 _course_double_deck_dl_mio0SegmentRomEnd[];
extern u8 _double_deck_vertexSegmentRomStart[];
extern u8 _double_deck_vertexSegmentRomEnd[];
extern u8 _course_double_deck_offsetsSegmentRomStart[];
extern u8 _course_double_deck_offsetsSegmentRomEnd[];
extern u8 d_course_double_deck_packed[];

extern u8 _course_dks_jungle_parkway_dl_mio0SegmentRomStart[];
extern u8 _course_dks_jungle_parkway_dl_mio0SegmentRomEnd[];
extern u8 _dks_jungle_parkway_vertexSegmentRomStart[];
extern u8 _dks_jungle_parkway_vertexSegmentRomEnd[];
extern u8 _course_dks_jungle_parkway_offsetsSegmentRomStart[];
extern u8 _course_dks_jungle_parkway_offsetsSegmentRomEnd[];
extern u8 d_course_dks_jungle_parkway_packed[];

extern u8 _course_big_donut_dl_mio0SegmentRomStart[];
extern u8 _course_big_donut_dl_mio0SegmentRomEnd[];
extern u8 _big_donut_vertexSegmentRomStart[];
extern u8 _big_donut_vertexSegmentRomEnd[];
extern u8 _course_big_donut_offsetsSegmentRomStart[];
extern u8 _course_big_donut_offsetsSegmentRomEnd[];
extern u8 d_course_big_donut_packed[];

#endif // COURSE_TABLE_H
