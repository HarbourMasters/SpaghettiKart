#include <libultra/types.h>
#include "course_offsets.h"
#include <assets/models/tracks/rainbow_road/rainbow_road_data.h>

Gfx* rainbow_road_dls[] = {
    d_course_rainbow_road_dl_D8,   d_course_rainbow_road_dl_210,  d_course_rainbow_road_dl_178,
    d_course_rainbow_road_dl_288,  d_course_rainbow_road_dl_338,  d_course_rainbow_road_dl_4A0,
    d_course_rainbow_road_dl_3D0,  d_course_rainbow_road_dl_528,  d_course_rainbow_road_dl_5F8,
    d_course_rainbow_road_dl_6E0,  d_course_rainbow_road_dl_658,  d_course_rainbow_road_dl_730,
    d_course_rainbow_road_dl_7A8,  d_course_rainbow_road_dl_880,  d_course_rainbow_road_dl_7F8,
    d_course_rainbow_road_dl_8E0,  d_course_rainbow_road_dl_958,  d_course_rainbow_road_dl_A70,
    d_course_rainbow_road_dl_9C8,  d_course_rainbow_road_dl_B08,  d_course_rainbow_road_dl_B70,
    d_course_rainbow_road_dl_C70,  d_course_rainbow_road_dl_BF0,  d_course_rainbow_road_dl_D10,
    d_course_rainbow_road_dl_D80,  d_course_rainbow_road_dl_E98,  d_course_rainbow_road_dl_E08,
    d_course_rainbow_road_dl_F50,  d_course_rainbow_road_dl_FB0,  d_course_rainbow_road_dl_10A8,
    d_course_rainbow_road_dl_1030, d_course_rainbow_road_dl_1150, d_course_rainbow_road_dl_1198,
    d_course_rainbow_road_dl_12A0, d_course_rainbow_road_dl_1228, d_course_rainbow_road_dl_1340,
    d_course_rainbow_road_dl_13F0, d_course_rainbow_road_dl_14E8, d_course_rainbow_road_dl_1488,
    d_course_rainbow_road_dl_1530, d_course_rainbow_road_dl_15D0, d_course_rainbow_road_dl_16C0,
    d_course_rainbow_road_dl_1678, d_course_rainbow_road_dl_1738, d_course_rainbow_road_dl_17D0,
    d_course_rainbow_road_dl_18D0, d_course_rainbow_road_dl_1878, d_course_rainbow_road_dl_1948,
};

u64 d_course_rainbow_road_unk_double = 1;
u64 d_course_rainbow_road_unk_double2 = 1;

s16 d_rainbow_road_chomp_jaw_animation[] = {
    0x0000, 0x3fff, 0x9602, 0x9483, 0x90d2, 0x8c1b, 0x878e, 0x845a, 0x83ac, 0x88ae, 0x9264, 0x9bc5, 0x9fc9,
    0x9c7c, 0x950f, 0x8c27, 0x846d, 0x808a, 0x8348, 0x8abb, 0x9273, 0x9602, 0x6fb2, 0x70c6, 0x7369, 0x76ba,
    0x79d6, 0x7bdd, 0x7beb, 0x7752, 0x6ed7, 0x66ca, 0x6379, 0x6692, 0x6d54, 0x756c, 0x7c87, 0x8054, 0x7e7c,
    0x78b0, 0x728d, 0x6fb2, 0x9602, 0x9483, 0x90d2, 0x8c1b, 0x878e, 0x845a, 0x83ac, 0x88ae, 0x9264, 0x9bc5,
    0x9fc9, 0x9c7c, 0x950f, 0x8c27, 0x846d, 0x808a, 0x8348, 0x8abb, 0x9273, 0x9602, 0x6fb2, 0x70c6, 0x7369,
    0x76ba, 0x79d6, 0x7bdd, 0x7beb, 0x7752, 0x6ed7, 0x66ca, 0x6379, 0x6692, 0x6d54, 0x756c, 0x7c87, 0x8054,
    0x7e7c, 0x78b0, 0x728d, 0x6fb2, 0x6fb2, 0x70c6, 0x7369, 0x76ba, 0x79d6, 0x7bdd, 0x7beb, 0x7752, 0x6ed7,
    0x66ca, 0x6379, 0x6692, 0x6d54, 0x756c, 0x7c87, 0x8054, 0x7e7c, 0x78b0, 0x728d, 0x6fb2, 0x3fff, 0x0000,
};

// 0x06016098
AnimationLimbVector d_rainbow_road_unk1[] = {
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0001 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0014, 0x002a },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0014, 0x0002 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0066 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0014, 0x0052 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0014, 0x003e },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0014, 0x0016 },
    },
};

// 0x060160F8
Animation d_rainbow_road_unk2 = {
    0x00010000, 0x00000000, 0x0014, 0x0007, d_rainbow_road_chomp_jaw_animation, d_rainbow_road_unk1,
};

// 0x0601610C
Animation* d_rainbow_road_unk3[] = {
    &d_rainbow_road_unk2,
};

// 0x06016110
uintptr_t d_rainbow_road_unk4[] = {
    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_ADD_POS(0x00000000, 0x0000023a, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_rainbow_road_dl_15550),

    ANIMATION_RENDER_MODEL(d_course_rainbow_road_dl_151A8),

    ANIMATION_POP_MATRIX,

    ANIMATION_ADD_POS(0x00000000, 0x0000023a, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_rainbow_road_dl_15C68),

    ANIMATION_RENDER_MODEL(d_course_rainbow_road_dl_158C0),

    ANIMATION_RENDER_MODEL(d_course_rainbow_road_dl_15F18),

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_STOP,
};

// No idea what this stuff is
u32 d_rainbow_road_unk5[] = {
    0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000002,
};
