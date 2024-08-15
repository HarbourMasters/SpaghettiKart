#ifndef COURSE_OFFSETS_H
#define COURSE_OFFSETS_H

#include <libultraship.h>
#include <libultra/types.h>
#include <libultra/gbi.h>
#include <assets/other_textures.h>
#include "code_80004740.h"

typedef struct
{
    u8 *addr;      // segmented address texture file
    u32 file_size; // compressed file size
    u32 data_size; // uncompressed texture size
    u32 padding;   // always zero
} course_texture;

extern Gfx toads_turnpike_dl_0[];
extern Gfx toads_turnpike_dl_1[];
extern Gfx toads_turnpike_dl_2[];
extern Gfx toads_turnpike_dl_3[];
extern Gfx toads_turnpike_dl_4[];
extern Gfx toads_turnpike_dl_5[];
extern Gfx toads_turnpike_dl_6[];
extern Gfx toads_turnpike_dl_7[];
extern Gfx toads_turnpike_dl_8[];
extern Gfx toads_turnpike_dl_9[];
extern Gfx toads_turnpike_dl_10[];
extern Gfx toads_turnpike_dl_11[];

extern u32 d_course_sherbet_land_unk_data1[];
extern s16 d_course_sherbet_land_unk_data2[];
extern animation_type_3_triplet d_course_sherbet_land_unk_data3[];
extern animation_type_2 d_course_sherbet_land_unk_data4;
extern s16 d_course_sherbet_land_unk_data5[];
extern animation_type_3_triplet d_course_sherbet_land_unk_data6[];
extern animation_type_2 d_course_sherbet_land_unk_data7;
extern s16 d_course_sherbet_land_unk_data8[];
extern animation_type_3_triplet d_course_sherbet_land_unk_data9[];
extern animation_type_2 d_course_sherbet_land_unk_data10;
extern animation_type_2 *d_course_sherbet_land_unk_data11[];
extern u32 d_course_sherbet_land_unk_data12[];

extern s16 d_rainbow_road_chomp_jaw_animation[];
extern animation_type_3_triplet d_rainbow_road_unk1[];
extern animation_type_2 d_rainbow_road_unk2;
extern animation_type_2 *d_rainbow_road_unk3[];
extern u32 d_rainbow_road_unk4[];
extern u32 d_rainbow_road_unk5[];

extern Gfx *koopa_troopa_beach_dls1[];
extern Gfx *koopa_troopa_beach_dls2[];
extern u32 d_course_koopa_troopa_beach_unk_data1[];
extern s16 d_course_koopa_troopa_beach_unk_data2[];
extern animation_type_3_triplet d_course_koopa_troopa_beach_unk_data3[];
extern animation_type_2 d_course_koopa_troopa_beach_unk_data4;
extern u32 d_course_koopa_troopa_beach_unk4[];
extern animation_type_2 *d_course_koopa_troopa_beach_unk_data5[];
extern u32 d_course_koopa_troopa_beach_unk_data6[];

extern s16 d_course_yoshi_valley_unk1[];
extern animation_type_3_triplet d_course_yoshi_valley_unk2[];
extern animation_type_2 d_course_yoshi_valley_unk3;
extern animation_type_2 *d_course_yoshi_valley_unk4[];
extern u32 d_course_yoshi_valley_unk5[];
extern u32 d_course_yoshi_valley_unk6[];

#endif // COURSE_OFFSETS_H
