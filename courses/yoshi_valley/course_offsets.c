#include <libultra/types.h>
#include "course_offsets.h"
#include <assets/textures/other_textures.h>
#include <assets/models/tracks/yoshi_valley/yoshi_valley_data.h>

#ifdef TARGET_N64
extern u8 gTexture66EBF0[];
extern u8 gTextureWoodBridgeSlats[];
extern u8 gTexture65E2EC[];
extern u8 gTexture6846DC[];
extern u8 gTextureFenceRope[];
extern u8 gTexture685108[];
extern u8 gTexture64CC20[];
extern u8 gTextureGrass4[];
extern u8 gTexture6775EC[];
extern u8 gTexture68E2D0[];
extern u8 gTextureCheckerboardBlackWhite[];
extern u8 gTexture643B3C[];
extern u8 gTextureSignWoodRedArrow[];
extern u8 gTexture68DEC0[];
#endif

Gfx* d_course_yoshi_valley_dl_list[] = {
    d_course_yoshi_valley_dl_40,   d_course_yoshi_valley_dl_3F0,  d_course_yoshi_valley_dl_290,
    d_course_yoshi_valley_dl_4E8,  d_course_yoshi_valley_dl_7A0,  d_course_yoshi_valley_dl_BC0,
    d_course_yoshi_valley_dl_9C8,  d_course_yoshi_valley_dl_CC0, // <- No idea what this is supposed to be a pointer to
    d_course_yoshi_valley_dl_FA0,  d_course_yoshi_valley_dl_13F8, d_course_yoshi_valley_dl_1178,
    d_course_yoshi_valley_dl_14D0, d_course_yoshi_valley_dl_17A8, d_course_yoshi_valley_dl_1C38,
    d_course_yoshi_valley_dl_1968, d_course_yoshi_valley_dl_1D48, d_course_yoshi_valley_dl_2000,
    d_course_yoshi_valley_dl_2408, d_course_yoshi_valley_dl_21B0, d_course_yoshi_valley_dl_25E8,
    d_course_yoshi_valley_dl_2840, d_course_yoshi_valley_dl_2BC0, d_course_yoshi_valley_dl_2978,
    d_course_yoshi_valley_dl_2D70, d_course_yoshi_valley_dl_2F48, d_course_yoshi_valley_dl_3258,
    d_course_yoshi_valley_dl_3078, d_course_yoshi_valley_dl_3420, d_course_yoshi_valley_dl_3560,
    d_course_yoshi_valley_dl_3880, d_course_yoshi_valley_dl_36D0, d_course_yoshi_valley_dl_3A38,
    d_course_yoshi_valley_dl_3BA0, d_course_yoshi_valley_dl_3E30, d_course_yoshi_valley_dl_3D20,
    d_course_yoshi_valley_dl_3F88, d_course_yoshi_valley_dl_40B8, d_course_yoshi_valley_dl_44C8,
    d_course_yoshi_valley_dl_4320, d_course_yoshi_valley_dl_4718, d_course_yoshi_valley_dl_48C0,
    d_course_yoshi_valley_dl_4C28, d_course_yoshi_valley_dl_4A98, d_course_yoshi_valley_dl_4E60,
    d_course_yoshi_valley_dl_5018, d_course_yoshi_valley_dl_53E0, d_course_yoshi_valley_dl_51D0,
    d_course_yoshi_valley_dl_5638, d_course_yoshi_valley_dl_5778, d_course_yoshi_valley_dl_5A80,
    d_course_yoshi_valley_dl_5890, d_course_yoshi_valley_dl_5C70, d_course_yoshi_valley_dl_5D60,
    d_course_yoshi_valley_dl_6070, d_course_yoshi_valley_dl_5ED8, d_course_yoshi_valley_dl_6268,
    d_course_yoshi_valley_dl_6358, d_course_yoshi_valley_dl_66B0, d_course_yoshi_valley_dl_6530,
    d_course_yoshi_valley_dl_68C8, d_course_yoshi_valley_dl_69C0, d_course_yoshi_valley_dl_6D78,
    d_course_yoshi_valley_dl_6BF0, d_course_yoshi_valley_dl_6FB8, d_course_yoshi_valley_dl_7100,
    d_course_yoshi_valley_dl_7400, d_course_yoshi_valley_dl_7310, d_course_yoshi_valley_dl_75C8,
    d_course_yoshi_valley_dl_7770, d_course_yoshi_valley_dl_7A08, d_course_yoshi_valley_dl_7938,
    d_course_yoshi_valley_dl_7BB8, d_course_yoshi_valley_dl_7CE8, d_course_yoshi_valley_dl_8030,
    d_course_yoshi_valley_dl_7F80, d_course_yoshi_valley_dl_8218, d_course_yoshi_valley_dl_8420,
    d_course_yoshi_valley_dl_8830, d_course_yoshi_valley_dl_8710, d_course_yoshi_valley_dl_8958,
    d_course_yoshi_valley_dl_8C30, d_course_yoshi_valley_dl_8FD8, d_course_yoshi_valley_dl_8D58,
    d_course_yoshi_valley_dl_91D8, d_course_yoshi_valley_dl_9340, d_course_yoshi_valley_dl_9780,
    d_course_yoshi_valley_dl_9548, d_course_yoshi_valley_dl_9A00, d_course_yoshi_valley_dl_9BB0,
    d_course_yoshi_valley_dl_9F70, d_course_yoshi_valley_dl_9DB8, d_course_yoshi_valley_dl_A188,
    d_course_yoshi_valley_dl_A328, d_course_yoshi_valley_dl_A6B8, d_course_yoshi_valley_dl_A518,
    d_course_yoshi_valley_dl_A8A0, d_course_yoshi_valley_dl_AAC0, d_course_yoshi_valley_dl_AE78,
    d_course_yoshi_valley_dl_ACF0, d_course_yoshi_valley_dl_AFF8, d_course_yoshi_valley_dl_B220,
    d_course_yoshi_valley_dl_B578, d_course_yoshi_valley_dl_B428, d_course_yoshi_valley_dl_B778,
    d_course_yoshi_valley_dl_B970, d_course_yoshi_valley_dl_BC98, d_course_yoshi_valley_dl_BAF8,
    d_course_yoshi_valley_dl_BE18, d_course_yoshi_valley_dl_BFC8, d_course_yoshi_valley_dl_C2D8,
    d_course_yoshi_valley_dl_C140, d_course_yoshi_valley_dl_C470, d_course_yoshi_valley_dl_C580,
    d_course_yoshi_valley_dl_C728, d_course_yoshi_valley_dl_C668, d_course_yoshi_valley_dl_C838,
    d_course_yoshi_valley_dl_C8F8, d_course_yoshi_valley_dl_CC80, d_course_yoshi_valley_dl_CAD8,
    d_course_yoshi_valley_dl_CEC8, d_course_yoshi_valley_dl_D018, d_course_yoshi_valley_dl_D3D0,
    d_course_yoshi_valley_dl_D1E8, d_course_yoshi_valley_dl_D540,
};

// 0x060145E8 Flag animation?
s16 d_course_yoshi_valley_unk1[] = {
    0x0000, 0x12e3, 0x0f43, 0x099f, 0x02d1, 0xfbb3, 0xf51b, 0xefe3, 0xecc0, 0xea97, 0xe905, 0xe817, 0xe7da, 0xe85a,
    0xe9a5, 0xebc7, 0xef2b, 0xf51f, 0xfcd4, 0x053b, 0x0d49, 0x13ee, 0x181e, 0x1a24, 0x1b1b, 0x1b28, 0x1a71, 0x191c,
    0x174f, 0x152f, 0x0dfd, 0x0b37, 0x064a, 0x006d, 0xfada, 0xf6c7, 0xf4d9, 0xf371, 0xf25d, 0xf1a3, 0xf148, 0xf152,
    0xf1c6, 0xf2aa, 0xf404, 0xf622, 0xfa11, 0xff44, 0x04fd, 0x0a7d, 0x0f08, 0x11dd, 0x132a, 0x13b1, 0x1393, 0x12f3,
    0x11f2, 0x10b2, 0x0f55, 0xf43f, 0xf376, 0xf2f2, 0xf2b7, 0xf2c3, 0xf319, 0xf3b9, 0xf4a5, 0xf64b, 0xf8e8, 0xfc2a,
    0xffc5, 0x0367, 0x06c3, 0x098b, 0x0b73, 0x0c85, 0x0ce4, 0x0ca5, 0x0bdd, 0x0aa2, 0x08f8, 0x06a5, 0x03d3, 0x00b6,
    0xfd87, 0xfa7a, 0xf7c5, 0xf59f, 0xf5bc, 0xf61a, 0xf722, 0xf8ba, 0xfac7, 0xfd30, 0xffd8, 0x02a6, 0x0580, 0x0829,
    0x0a3a, 0x0bbc, 0x0cb9, 0x0d3f, 0x0d4c, 0x0c44, 0x0a3b, 0x0785, 0x0478, 0x0169, 0xfead, 0xfc97, 0xfb3e, 0xfa01,
    0xf8db, 0xf7d5, 0xf6f8, 0xf64f, 0xf5e2, 0x04ea, 0x04b3, 0x0421, 0x034c, 0x0250, 0x0147, 0x004b, 0xff77, 0xfeaa,
    0xfdc3, 0xfcd9, 0xfc02, 0xfb55, 0xfae7, 0xfad0, 0xfb25, 0xfbdc, 0xfcd6, 0xfdf4, 0xff17, 0x0020, 0x00f2, 0x01a2,
    0x0253, 0x02fe, 0x039a, 0x0422, 0x048c, 0x04d1, 0x3fff, 0xc001, 0x3fff, 0x0000,
};

// 0x06014714
AnimationLimbVector d_course_yoshi_valley_unk2[] = {
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x0094 },
    },
    {
        { 0x0001, 0x0092 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x0093 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x001d, 0x0075 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x001d, 0x0058 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x001d, 0x003b },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x001d, 0x001e },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x001d, 0x0001 },
    },
};

// 0x06014780
Animation d_course_yoshi_valley_unk3 = {
    0x00010000, 0x00000000, 0x001d, 0x0008, d_course_yoshi_valley_unk1, d_course_yoshi_valley_unk2,
};

// 0x06014794
Animation* d_course_yoshi_valley_unk4[] = {
    &d_course_yoshi_valley_unk3,
};

//! 0x06014798
uintptr_t d_course_yoshi_valley_unk5[] = {
    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_ADD_POS(0x00000000, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_yoshi_valley_dl_142C0),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_ADD_POS(0x000007f8, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_yoshi_valley_dl_143C8),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_yoshi_valley_dl_144A8, 0x000001cc, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_yoshi_valley_dl_14500, 0x000001cc, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_yoshi_valley_dl_14558, 0x00000158, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_yoshi_valley_dl_145D0, 0x00000120, 0x00000000, 0x00000000),

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_STOP,
};

// No idea what this stuff is
u32 d_course_yoshi_valley_unk6[] = {
    0x00000000,
    0x00000001,
};
