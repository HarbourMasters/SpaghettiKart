#ifndef COURSE_OFFSETS_H
#define COURSE_OFFSETS_H

#include <libultraship.h>
#include <libultra/types.h>
#include <libultra/gbi.h>
#include <assets/textures/other_textures.h>
#include "animation.h"

typedef struct {
    const char* addr;      // segmented address texture file
    u32 file_size; // compressed file size
    u32 data_size; // uncompressed texture size
    u32 padding;   // always zero
} course_texture;

extern uintptr_t d_course_sherbet_land_unk_data1[];
extern s16 d_course_sherbet_land_unk_data2[];
extern AnimationLimbVector d_course_sherbet_land_unk_data3[];
extern Animation d_course_sherbet_land_unk_data4;
extern s16 d_course_sherbet_land_unk_data5[];
extern AnimationLimbVector d_course_sherbet_land_unk_data6[];
extern Animation d_course_sherbet_land_unk_data7;
extern s16 d_course_sherbet_land_unk_data8[];
extern AnimationLimbVector d_course_sherbet_land_unk_data9[];
extern Animation d_course_sherbet_land_unk_data10;
extern Animation* d_course_sherbet_land_unk_data11[];
extern u32 d_course_sherbet_land_unk_data12[];

extern s16 d_rainbow_road_chomp_jaw_animation[];
extern AnimationLimbVector d_rainbow_road_unk1[];
extern Animation d_rainbow_road_unk2;
extern Animation* d_rainbow_road_unk3[];
extern uintptr_t d_rainbow_road_unk4[];
extern u32 d_rainbow_road_unk5[];

extern s16 d_course_yoshi_valley_unk1[];
extern AnimationLimbVector d_course_yoshi_valley_unk2[];
extern Animation d_course_yoshi_valley_unk3;
extern Animation* d_course_yoshi_valley_unk4[];
extern uintptr_t d_course_yoshi_valley_unk5[];
extern u32 d_course_yoshi_valley_unk6[];

extern uintptr_t d_course_sherbet_land_unk_data1[];
extern s16 d_course_sherbet_land_unk_data2[];
extern AnimationLimbVector d_course_sherbet_land_unk_data3[];
extern Animation d_course_sherbet_land_unk_data4;
extern s16 d_course_sherbet_land_unk_data5[];
extern AnimationLimbVector d_course_sherbet_land_unk_data6[];
extern Animation d_course_sherbet_land_unk_data7;
extern s16 d_course_sherbet_land_unk_data8[];
extern AnimationLimbVector d_course_sherbet_land_unk_data9[];
extern Animation d_course_sherbet_land_unk_data10;
extern Animation* d_course_sherbet_land_unk_data11[];
extern u32 d_course_sherbet_land_unk_data12[];

extern s16 d_rainbow_road_chomp_jaw_animation[];
extern AnimationLimbVector d_rainbow_road_unk1[];
extern Animation d_rainbow_road_unk2;
extern Animation* d_rainbow_road_unk3[];
extern u32 d_rainbow_road_unk5[];

extern uintptr_t d_course_koopa_troopa_beach_unk_data1[];
extern s16 d_course_koopa_troopa_beach_unk_data2[];
extern AnimationLimbVector d_course_koopa_troopa_beach_unk_data3[];
extern Animation d_course_koopa_troopa_beach_unk_data4;
extern uintptr_t d_course_koopa_troopa_beach_unk4[];
extern Animation* d_course_koopa_troopa_beach_unk_data5[];
extern uintptr_t d_course_koopa_troopa_beach_unk_data6[];

extern s16 d_course_yoshi_valley_unk1[];
extern AnimationLimbVector d_course_yoshi_valley_unk2[];
extern Animation d_course_yoshi_valley_unk3;
extern Animation* d_course_yoshi_valley_unk4[];
extern u32 d_course_yoshi_valley_unk6[];

#endif // COURSE_OFFSETS_H
