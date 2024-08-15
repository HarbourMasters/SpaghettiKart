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

extern s16 d_course_yoshi_valley_unk1[];
extern animation_type_3_triplet d_course_yoshi_valley_unk2[];
extern animation_type_2 d_course_yoshi_valley_unk3;
extern animation_type_2 *d_course_yoshi_valley_unk4[];
extern u32 d_course_yoshi_valley_unk5[];
extern u32 d_course_yoshi_valley_unk6[];

#endif // COURSE_OFFSETS_H
