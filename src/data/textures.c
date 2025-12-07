/**
 * @file textures.c
 * @brief Texture data.
 * start in ram 0x801978D0
 * look like link to menu_items
 */
#include <assets/textures/texture_tkmk00.h>
#include <assets/textures/texture_data_2.h>
#include <assets/textures/player_selection.h>
#include "textures.h"

/**
 * @brief MenuTexture of a tiny font texture of number 0
 *
 */
MenuTexture D_02000000[2] = {
    { 3, gTextureTinyFont0, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 1
 *
 */
MenuTexture D_02000028[2] = {
    { 3, gTextureTinyFont1, 8, 8, 0, 0, 0x49 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 2
 *
 */
MenuTexture D_02000050[2] = {
    { 3, gTextureTinyFont2, 8, 8, 0, 0, 0x73 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 3
 *
 */
MenuTexture D_02000078[2] = {
    { 3, gTextureTinyFont3, 8, 8, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 4
 *
 */
MenuTexture D_020000A0[2] = {
    { 3, gTextureTinyFont4, 8, 8, 0, 0, 0x5a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 5
 *
 */
MenuTexture D_020000C8[2] = {
    { 3, gTextureTinyFont5, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 6
 *
 */
MenuTexture D_020000F0[2] = {
    { 3, gTextureTinyFont6, 8, 8, 0, 0, 0x74 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 7
 *
 */
MenuTexture D_02000118[2] = {
    { 3, gTextureTinyFont7, 8, 8, 0, 0, 0x5e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 8
 *
 */
MenuTexture D_02000140[2] = {
    { 3, gTextureTinyFont8, 8, 8, 0, 0, 0x77 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of number 9
 *
 */
MenuTexture D_02000168[2] = {
    { 3, gTextureTinyFont9, 8, 8, 0, 0, 0x74 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter A
 *
 */
MenuTexture D_02000190[2] = {
    { 3, gTextureTinyFontA, 8, 8, 0, 0, 0x6b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter B
 *
 */
MenuTexture D_020001B8[2] = {
    { 3, gTextureTinyFontB, 8, 8, 0, 0, 0x7d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter C
 *
 */
MenuTexture D_020001E0[2] = {
    { 3, gTextureTinyFontC, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter D
 *
 */
MenuTexture D_02000208[2] = {
    { 3, gTextureTinyFontD, 8, 8, 0, 0, 0x71 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter E
 *
 */
MenuTexture D_02000230[2] = {
    { 3, gTextureTinyFontE, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter F
 *
 */
MenuTexture D_02000258[2] = {
    { 3, gTextureTinyFontF, 8, 8, 0, 0, 0x63 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter G
 *
 */
MenuTexture D_02000280[2] = {
    { 3, gTextureTinyFontG, 8, 8, 0, 0, 0x73 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter H
 *
 */
MenuTexture D_020002A8[2] = {
    { 3, gTextureTinyFontH, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter I
 *
 */
MenuTexture D_020002D0[2] = {
    { 3, gTextureTinyFontI, 8, 8, 0, 0, 0x48 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter J
 *
 */
MenuTexture D_020002F8[2] = {
    { 3, gTextureTinyFontJ, 8, 8, 0, 0, 0x52 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter K
 *
 */
MenuTexture D_02000320[2] = {
    { 3, gTextureTinyFontK, 8, 8, 0, 0, 0x6f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter L
 *
 */
MenuTexture D_02000348[2] = {
    { 3, gTextureTinyFontL, 8, 8, 0, 0, 0x4b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter M
 *
 */
MenuTexture D_02000370[2] = {
    { 3, gTextureTinyFontM, 8, 8, 0, 0, 0x7c },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter N
 *
 */
MenuTexture D_02000398[2] = {
    { 3, gTextureTinyFontN, 8, 8, 0, 0, 0x7b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter O
 *
 */
MenuTexture D_020003C0[2] = {
    { 3, gTextureTinyFontO, 8, 8, 0, 0, 0x76 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter P
 *
 */
MenuTexture D_020003E8[2] = {
    { 3, gTextureTinyFontP, 8, 8, 0, 0, 0x6a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter Q
 *
 */
MenuTexture D_02000410[2] = {
    { 3, gTextureTinyFontQ, 8, 8, 0, 0, 0x77 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter R
 *
 */
MenuTexture D_02000438[2] = {
    { 3, gTextureTinyFontR, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter S
 *
 */
MenuTexture D_02000460[2] = {
    { 3, gTextureTinyFontS, 8, 8, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief  MenuTexture of a tiny font texture of letter T
 *
 */
MenuTexture D_02000488[2] = {
    { 3, gTextureTinyFontT, 8, 8, 0, 0, 0x54 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter U
 *
 */
MenuTexture D_020004B0[2] = {
    { 3, gTextureTinyFontU, 8, 8, 0, 0, 0x62 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter V
 *
 */
MenuTexture D_020004D8[2] = {
    { 3, gTextureTinyFontV, 8, 8, 0, 0, 0x5e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter W
 *
 */
MenuTexture D_02000500[2] = {
    { 3, gTextureTinyFontW, 8, 8, 0, 0, 0x7c },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter X
 *
 */
MenuTexture D_02000528[2] = {
    { 3, gTextureTinyFontX, 8, 8, 0, 0, 0x6b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter Y
 *
 */
MenuTexture D_02000550[2] = {
    { 3, gTextureTinyFontY, 8, 8, 0, 0, 0x51 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of letter Z
 *
 */
MenuTexture D_02000578[2] = {
    { 3, gTextureTinyFontZ, 8, 8, 0, 0, 0x68 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '!'
 *
 */
MenuTexture D_020005A0[2] = {
    { 3, gTextureTinyFontExclamationMark, 8, 8, 0, 0, 0x46 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '"'
 *
 */
MenuTexture D_020005C8[2] = {
    { 3, gTextureTinyFontDoubleQuote, 8, 8, 0, 0, 0x37 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '#'
 *
 */
MenuTexture D_020005F0[2] = {
    { 3, gTextureTinyFontHash, 8, 8, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of "'"
 *
 */
MenuTexture D_02000618[2] = {
    { 3, gTextureTinyFontSingleQuote, 8, 8, 0, 0, 0x2f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '*'
 *
 */
MenuTexture D_02000640[2] = {
    { 3, gTextureTinyFontAsterisk, 8, 8, 0, 0, 0x6a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '+'
 *
 */
MenuTexture D_02000668[2] = {
    { 3, gTextureTinyFontPlus, 8, 8, 0, 0, 0x51 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000690[2] = {
    { 3, gTexture7EEB18, 8, 8, 0, 0, 0x30 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '-'
 *
 */
MenuTexture D_020006B8[2] = {
    { 3, gTextureTinyFontMinus, 8, 8, 0, 0, 0x35 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of ','
 *
 */
MenuTexture D_020006E0[2] = {
    { 3, gTextureTinyFontComma, 8, 8, 0, 0, 0x31 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '/'
 *
 */
MenuTexture D_02000708[2] = {
    { 3, gTextureTinyFontForwardSlash, 8, 8, 0, 0, 0x4f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of ';'
 *
 */
MenuTexture D_02000730[2] = {
    { 3, gTextureTinyFontColon, 8, 8, 0, 0, 0x38 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000758[2] = {
    { 3, gTexture7EEC34, 8, 8, 0, 0, 0x51 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a tiny font texture of '?'
 *
 */
MenuTexture D_02000780[2] = {
    { 3, gTextureTinyFontQuestion, 8, 8, 0, 0, 0x60 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020007A8[2] = {
    { 3, gTexture7EECE4, 8, 8, 0, 0, 0x8f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020007D0[2] = {
    { 3, gTexture7EED74, 8, 8, 0, 0, 0x3d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020007F8[2] = {
    { 3, gTexture7EEDB0, 8, 8, 0, 0, 0x37 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000820[2] = {
    { 3, gTexture7EEDE8, 8, 8, 0, 0, 0x39 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000848[2] = {
    { 3, gTexture7EEE20, 8, 8, 0, 0, 0x5b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000870[2] = {
    { 3, gTexture7EEE7C, 8, 8, 0, 0, 0x55 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000898[2] = {
    { 3, gTexture7EEED0, 8, 8, 0, 0, 0x6a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020008C0[2] = {
    { 3, gTexture7EEF3C, 8, 8, 0, 0, 0x55 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020008E8[2] = {
    { 3, gTexture7EEF90, 8, 8, 0, 0, 0x62 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000910[2] = {
    { 3, gTexture7EEFF4, 8, 8, 0, 0, 0x64 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000938[2] = {
    { 3, gTexture7EF058, 8, 8, 0, 0, 0x5b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000960[2] = {
    { 3, gTexture7EF0B4, 8, 8, 0, 0, 0x5d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000988[2] = {
    { 3, gTexture7EF110, 8, 8, 0, 0, 0x6d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020009B0[2] = {
    { 3, gTexture7EF17C, 8, 8, 0, 0, 0x73 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020009D8[2] = {
    { 3, gTexture7EF1F0, 8, 8, 0, 0, 0x5e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000A00[2] = {
    { 3, gTexture7EF250, 8, 8, 0, 0, 0x69 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000A28[2] = {
    { 3, gTexture7EF2B8, 8, 8, 0, 0, 0x5b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000A50[2] = {
    { 3, gTexture7EF314, 8, 8, 0, 0, 0x73 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000A78[2] = {
    { 3, gTexture7EF388, 8, 8, 0, 0, 0x6f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000AA0[2] = {
    { 3, gTexture7EF3F8, 8, 8, 0, 0, 0x79 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000AC8[2] = {
    { 3, gTexture7EF470, 8, 8, 0, 0, 0x74 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000AF0[2] = {
    { 3, gTexture7EF4E4, 8, 8, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000B18[2] = {
    { 3, gTexture7EF554, 8, 8, 0, 0, 0x6c },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000B40[2] = {
    { 3, gTexture7EF5C0, 8, 8, 0, 0, 0x61 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000B68[2] = {
    { 3, gTexture7EF620, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000B90[2] = {
    { 3, gTexture7EF694, 8, 8, 0, 0, 0x74 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000BB8[2] = {
    { 3, gTexture7EF708, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000BE0[2] = {
    { 3, gTexture7EF77C, 8, 8, 0, 0, 0x6b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000C08[2] = {
    { 3, gTexture7EF7E8, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000C30[2] = {
    { 3, gTexture7EF85C, 8, 8, 0, 0, 0x6b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000C58[2] = {
    { 3, gTexture7EF8C8, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000C80[2] = {
    { 3, gTexture7EF93C, 8, 8, 0, 0, 0x76 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000CA8[2] = {
    { 3, gTexture7EF9B4, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000CD0[2] = {
    { 3, gTexture7EFA2C, 8, 8, 0, 0, 0x7b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000CF8[2] = {
    { 3, gTexture7EFAA8, 8, 8, 0, 0, 0x57 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000D20[2] = {
    { 3, gTexture7EFB00, 8, 8, 0, 0, 0x63 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000D48[2] = {
    { 3, gTexture7EFB64, 8, 8, 0, 0, 0x57 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000D70[2] = {
    { 3, gTexture7EFBBC, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000D98[2] = {
    { 3, gTexture7EFC30, 8, 8, 0, 0, 0x7b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000DC0[2] = {
    { 3, gTexture7EFCAC, 8, 8, 0, 0, 0x53 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000DE8[2] = {
    { 3, gTexture7EFD00, 8, 8, 0, 0, 0x60 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000E10[2] = {
    { 3, gTexture7EFD60, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000E38[2] = {
    { 3, gTexture7EFDD4, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000E60[2] = {
    { 3, gTexture7EFE48, 8, 8, 0, 0, 0x57 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000E88[2] = {
    { 3, gTexture7EFEA0, 8, 8, 0, 0, 0x8d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000EB0[2] = {
    { 3, gTexture7EFF2C, 8, 8, 0, 0, 0x63 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000ED8[2] = {
    { 3, gTexture7EFF90, 8, 8, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000F00[2] = {
    { 3, gTexture7F0000, 8, 8, 0, 0, 0x6d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000F28[2] = {
    { 3, gTexture7F006C, 8, 8, 0, 0, 0x6a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000F50[2] = {
    { 3, gTexture7F00D8, 8, 8, 0, 0, 0x80 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000F78[2] = {
    { 3, gTexture7F0158, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000FA0[2] = {
    { 3, gTexture7F01D0, 8, 8, 0, 0, 0x6a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000FC8[2] = {
    { 3, gTexture7F023C, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02000FF0[2] = {
    { 3, gTexture7F02B4, 8, 8, 0, 0, 0x79 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001018[2] = {
    { 3, gTexture7F032C, 8, 8, 0, 0, 0x63 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001040[2] = {
    { 3, gTexture7F0390, 8, 8, 0, 0, 0x72 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001068[2] = {
    { 3, gTexture7F0404, 8, 8, 0, 0, 0x6d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001090[2] = {
    { 3, gTexture7F0470, 8, 8, 0, 0, 0x7d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020010B8[2] = {
    { 3, gTexture7F04EC, 8, 8, 0, 0, 0x6e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020010E0[2] = {
    { 3, gTexture7F055C, 8, 8, 0, 0, 0x93 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001108[2] = {
    { 3, gTexture7F05F0, 8, 8, 0, 0, 0x7f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001130[2] = {
    { 3, gTexture7F0670, 8, 8, 0, 0, 0x7b },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001158[2] = {
    { 3, gTexture7F06EC, 8, 8, 0, 0, 0x7a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001180[2] = {
    { 3, gTexture7F0768, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020011A8[2] = {
    { 3, gTexture7F07E0, 8, 8, 0, 0, 0x77 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020011D0[2] = {
    { 3, gTexture7F0858, 8, 8, 0, 0, 0x7f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020011F8[2] = {
    { 3, gTexture7F08D8, 8, 8, 0, 0, 0x71 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001220[2] = {
    { 3, gTexture7F0948, 8, 8, 0, 0, 0x87 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001248[2] = {
    { 3, gTexture7F09D0, 8, 8, 0, 0, 0x6d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001270[2] = {
    { 3, gTexture7F0A3C, 8, 8, 0, 0, 0x7e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001298[2] = {
    { 3, gTexture7F0ABC, 8, 8, 0, 0, 0x77 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020012C0[2] = {
    { 3, gTexture7F0B34, 8, 8, 0, 0, 0x7d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020012E8[2] = {
    { 3, gTexture7F0BB0, 8, 8, 0, 0, 0x75 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001310[2] = {
    { 3, gTexture7F0C24, 8, 8, 0, 0, 0x71 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001338[2] = {
    { 3, gTexture7F0C94, 8, 8, 0, 0, 0x77 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001360[2] = {
    { 3, gTexture7F0D0C, 8, 8, 0, 0, 0x7f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001388[2] = {
    { 3, gTexture7F0D8C, 8, 8, 0, 0, 0x61 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020013B0[2] = {
    { 3, gTexture7F0DEC, 8, 8, 0, 0, 0x6e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020013D8[2] = {
    { 3, gTexture7F0E5C, 8, 8, 0, 0, 0x87 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001400[2] = {
    { 3, gTexture7F0EE4, 8, 8, 0, 0, 0x78 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001428[2] = {
    { 3, gTexture7F0F5C, 8, 8, 0, 0, 0x7d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001450[2] = {
    { 3, gTexture7F0FD8, 8, 8, 0, 0, 0x60 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001478[2] = {
    { 3, gTexture7F1038, 8, 8, 0, 0, 0x65 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020014A0[2] = {
    { 3, gTexture7F109C, 8, 8, 0, 0, 0x87 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of bar ?
 *
 */
MenuTexture D_020014C8[6] = {
    { 0, gTextureGoldBar, 256, 40, 32, 25, 0xd4c },  { 0, gTexturePinkBar, 90, 16, 65, 70, 0x468 },
    { 0, gTexturePinkBar, 90, 16, 164, 70, 0x468 },  { 0, gTexturePinkBar, 90, 16, 65, 200, 0x468 },
    { 0, gTexturePinkBar, 90, 16, 164, 200, 0x468 }, { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of gold ?
 *
 */
MenuTexture D_02001540[3] = {
    { 0, gTextureGreenGoldStripe, 256, 29, 32, 91, 0xbec },
    { 0, gTextureGoldStripe, 256, 15, 32, 180, 0x6f8 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

MenuTexture D_0200157C[2] = {
    { 0, gTextureWhiteStripe, 256, 10, 0, 0, 0xe4 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020015A4[2] = {
    { 3, gTexture7ED50C, 256, 5, 32, 120, 0x196 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of select record ?
 *
 */
MenuTexture D_020015CC[5] = {
    { 3, gTextureSelectRecord, 68, 10, 75, 74, 0x378 },
    { 3, gTextureTextEnd, 20, 10, 200, 74, 0x12f },
    { 3, gTextureTextErase, 28, 10, 96, 204, 0x19e },
    { 3, gTextureTextQuit, 24, 10, 198, 204, 0x149 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of select record ?
 *
 */
MenuTexture D_02001630[2] = {
    { 3, gTextureTableOfContents, 88, 10, 116, 94, 0x42d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of select record ?
 *
 */
MenuTexture D_02001658[5] = {
    { 3, gTextureTextHash, 8, 10, 48, 108, 0x07e },
    { 3, gTextureTextGameData, 56, 10, 132, 108, 0x2c4 },
    { 3, gTextureTextPages, 32, 10, 252, 108, 0x1c1 },
    { 3, gTextureTextPagesFree, 56, 10, 132, 184, 0x2c2 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 0
 *
 */
MenuTexture D_020016BC[2] = {
    { 3, gTextureSmallFont0, 8, 9, 0, 0, 0x8e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 1
 *
 */
MenuTexture D_020016E4[2] = {
    { 3, gTextureSmallFont1, 8, 9, 0, 0, 0x59 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 2
 *
 */
MenuTexture D_0200170C[2] = {
    { 3, gTextureSmallFont2, 8, 9, 0, 0, 0x97 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 3
 *
 */
MenuTexture D_02001734[2] = {
    { 3, gTextureSmallFont3, 8, 9, 0, 0, 0x95 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 4
 *
 */
MenuTexture D_0200175C[2] = {
    { 3, gTextureSmallFont4, 8, 9, 0, 0, 0x87 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 5
 *
 */
MenuTexture D_02001784[2] = {
    { 3, gTextureSmallFont5, 8, 9, 0, 0, 0x99 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 6
 *
 */
MenuTexture D_020017AC[2] = {
    { 3, gTextureSmallFont6, 8, 9, 0, 0, 0x9a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 7
 *
 */
MenuTexture D_020017D4[2] = {
    { 3, gTextureSmallFont7, 8, 9, 0, 0, 0x70 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 8
 *
 */
MenuTexture D_020017FC[2] = {
    { 3, gTextureSmallFont8, 8, 9, 0, 0, 0x90 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a small font texture of number 9
 *
 */
MenuTexture D_02001824[2] = {
    { 3, gTextureSmallFont9, 8, 9, 0, 0, 0x9f },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200184C[2] = {
    { 3, gTexture7ED058, 96, 22, 0, 0, 0x238 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001874[2] = {
    { 3, gTexture7ED290, 248, 10, 0, 0, 0x27d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of a n64 controller pak Data Select
 *
 */
MenuTexture D_0200189C[2] = {
    { 3, gTextureN64ControllerPakDataSelect, 180, 32, 70, 29, 0x1128 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of text Erase Data Record Confirmation
 *
 */
MenuTexture D_020018C4[2] = {
    { 3, gTextureEraseDataRecordConfirmation, 180, 32, 70, 29, 0xff4 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of text Record Not Erased
 *
 */
MenuTexture D_020018EC[2] = {
    { 3, gTextureRecordNotErased, 180, 32, 70, 29, 0x8d1 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of text Please Insert N64 Controller Pak
 *
 */
MenuTexture D_02001914[2] = {
    { 3, gTexturePlaceN64ControllerPakIntoController1, 180, 32, 70, 29, 0xc4a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of text Please Reinsert Original N64 Controller Pak
 *
 */
MenuTexture D_0200193C[2] = {
    { 3, gTexturePleaseReinsertOriginalN64ControllerPak, 180, 32, 70, 29, 0xcd6 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of text Erasing Selected Record
 *
 */
MenuTexture D_02001964[2] = {
    { 3, gTextureErasingSelectedRecord, 180, 32, 70, 29, 0xc02 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_0200198C[2] = {
    { D_0200189C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_0200199C[2] = {
    { D_020018C4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020019AC[2] = {
    { D_020018EC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020019BC[2] = {
    { D_02001914, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020019CC[2] = {
    { D_0200193C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020019DC[2] = {
    { D_02001964, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief MenuTexture of Name DK
 *
 */
MenuTexture D_020019EC[2] = {
    { 0, texture_name_dk, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Toad
 *
 */
MenuTexture D_02001A14[2] = {
    { 0, texture_name_toad, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Bowser
 *
 */
MenuTexture D_02001A3C[2] = {
    { 0, texture_name_bowser, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Luigi
 *
 */
MenuTexture D_02001A64[2] = {
    { 0, texture_name_luigi, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Mario
 *
 */
MenuTexture D_02001A8C[2] = {
    { 0, texture_name_mario, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Peach
 *
 */
MenuTexture D_02001AB4[2] = {
    { 0, texture_name_peach, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Wario
 *
 */
MenuTexture D_02001ADC[2] = {
    { 0, texture_name_wario, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Name Yoshi
 *
 */
MenuTexture D_02001B04[2] = {
    { 0, texture_name_yoshi, 64, 12, 0, 64, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Mario Raceway
 *
 */
MenuTexture seg2_mario_raceway_preview_texture[2] = {
    { 0, gTextureCoursePreviewMarioRaceway, 128, 78, 0, 0, 0x3157 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Choco Mountain
 *
 */
MenuTexture D_02001B54[2] = {
    { 0, gTextureCoursePreviewChocoMountain, 128, 78, 0, 0, 0x2a24 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Bowser Castle
 *
 */
MenuTexture D_02001B7C[2] = {
    { 0, gTextureCoursePreviewBowsersCastle, 128, 78, 0, 0, 0x30de },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Banshee Boardwalk
 *
 */
MenuTexture D_02001BA4[2] = {
    { 0, gTextureCoursePreviewBansheeBoardwalk, 128, 78, 0, 0, 0x26ad },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Yoshi Valley
 *
 */
MenuTexture D_02001BCC[2] = {
    { 0, gTextureCoursePreviewYoshiValley, 128, 78, 0, 0, 0x3a41 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Frappe Snowland
 *
 */
MenuTexture D_02001BF4[2] = {
    { 0, gTextureCoursePreviewFrappeSnowland, 128, 78, 0, 0, 0x2409 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Koopa Troopa Beach
 *
 */
MenuTexture D_02001C1C[2] = {
    { 0, gTextureCoursePreviewKoopaTroopaBeach, 128, 78, 0, 0, 0x2323 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Royal Raceway
 *
 */
MenuTexture D_02001C44[2] = {
    { 0, gTextureCoursePreviewRoyalRaceway, 128, 78, 0, 0, 0x2821 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Luigi Raceway
 *
 */
MenuTexture D_02001C6C[2] = {
    { 0, gTextureCoursePreviewLuigiRaceway, 128, 78, 0, 0, 0x2d74 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Moo Moo Farm
 *
 */
MenuTexture D_02001C94[2] = {
    { 0, gTextureCoursePreviewMooMooFarm, 128, 78, 0, 0, 0x25c7 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Toads Turnpike
 *
 */
MenuTexture D_02001CBC[2] = {
    { 0, gTextureCoursePreviewToadsTurnpike, 128, 78, 0, 0, 0x299a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Kalimari Desert
 *
 */
MenuTexture D_02001CE4[2] = {
    { 0, gTextureCoursePreviewKalimariDesert, 128, 78, 0, 0, 0x280a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Sherbet Land
 *
 */
MenuTexture D_02001D0C[2] = {
    { 0, gTextureCoursePreviewSherbetLand, 128, 78, 0, 0, 0x300e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Rainbow Road
 *
 */
MenuTexture D_02001D34[2] = {
    { 0, gTextureCoursePreviewRainbowRoad, 128, 78, 0, 0, 0x2824 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Wario Stadium
 *
 */
MenuTexture D_02001D5C[2] = {
    { 0, gTextureCoursePreviewWarioStadium, 128, 78, 0, 0, 0x32d3 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Block Fort
 *
 */
MenuTexture D_02001D84[2] = {
    { 0, gTextureCoursePreviewBlockFort, 128, 78, 0, 0, 0x2528 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Skyscraper
 *
 */
MenuTexture D_02001DAC[2] = {
    { 0, gTextureCoursePreviewSkyscraper, 128, 78, 0, 0, 0x26f0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Double Deck
 *
 */
MenuTexture D_02001DD4[2] = {
    { 0, gTextureCoursePreviewDoubleDeck, 128, 78, 0, 0, 0x28f7 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview DKs Jungle Parkway
 *
 */
MenuTexture D_02001DFC[2] = {
    { 0, gTextureCoursePreviewDksJungleParkway, 128, 78, 0, 0, 0x3ee4 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of Course Preview Big Donut
 *
 */
MenuTexture D_02001E24[2] = {
    { 0, gTextureCoursePreviewBigDonut, 128, 78, 0, 0, 0x2f5c },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001E4C[3] = {
    { seg2_mario_raceway_preview_texture, 0x0000003c },
    { seg2_mario_raceway_preview_texture, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001E64[2] = {
    { seg2_mario_raceway_preview_texture, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001E74[2] = {
    { D_02001B54, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001E84[2] = {
    { D_02001B7C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001E94[2] = {
    { D_02001BA4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001EA4[2] = {
    { D_02001BCC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001EB4[2] = {
    { D_02001BF4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001EC4[2] = {
    { D_02001C1C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001ED4[2] = {
    { D_02001C44, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001EE4[2] = {
    { D_02001C6C, 0x0000003c },
    { NULL, 0x00000000 },
};

MkAnimation D_02001EF4[2] = {
    { D_02001C94, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F04[2] = {
    { D_02001CBC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F14[2] = {
    { D_02001CE4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F24[2] = {
    { D_02001D0C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F34[2] = {
    { D_02001D34, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F44[2] = {
    { D_02001D5C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F54[2] = {
    { D_02001D84, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F64[2] = {
    { D_02001DAC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F74[2] = {
    { D_02001DD4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F84[2] = {
    { D_02001DFC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_02001F94[2] = {
    { D_02001E24, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02001FA4[2] = {
    { 3, gTexture7ED6A4, 68, 51, 0, 0, 0x38e },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of trophy gold
 *
 */
MenuTexture D_02001FCC[2] = {
    { 1, gTextureTrophyGold, 45, 45, 0, 0, 0x633 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of trophy silver
 *
 */
MenuTexture D_02001FF4[2] = {
    { 1, gTextureTrophySilver, 45, 45, 0, 0, 0x629 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of trophy bronze
 *
 */
MenuTexture D_0200201C[2] = {
    { 1, gTextureTrophyBronze, 45, 45, 0, 0, 0x5ec },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of cup gold
 *
 */
MenuTexture D_02002044[2] = {
    { 1, gTextureCupGold, 45, 45, 0, 0, 0x98a },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of cup silver
 *
 */
MenuTexture D_0200206C[2] = {
    { 1, gTextureCupSilver, 45, 45, 0, 0, 0x985 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture of cup bronze
 *
 */
MenuTexture D_02002094[2] = {
    { 1, gTextureCupBronze, 45, 45, 0, 0, 0x8a2 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020020BC[2] = {
    { D_02001FCC, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020020CC[2] = {
    { D_02001FF4, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020020DC[2] = {
    { D_0200201C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020020EC[2] = {
    { D_02002044, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_020020FC[2] = {
    { D_0200206C, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 *
 */
MkAnimation D_0200210C[2] = {
    { D_02002094, 0x0000003c },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterA[2] = {
    { 5, font_letter_A, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterB[2] = {
    { 5, font_letter_B, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterC[2] = {
    { 5, font_letter_C, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterD[2] = {
    { 5, font_letter_D, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterE[2] = {
    { 5, font_letter_E, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterF[2] = {
    { 5, font_letter_F, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterG[2] = {
    { 5, font_letter_G, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterH[2] = {
    { 5, font_letter_H, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterI[2] = {
    { 5, font_letter_I, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterJ[2] = {
    { 5, font_letter_J, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterK[2] = {
    { 5, font_letter_K, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterL[2] = {
    { 5, font_letter_L, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterM[2] = {
    { 5, font_letter_M, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterN[2] = {
    { 5, font_letter_N, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterO[2] = {
    { 5, font_letter_O, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterP[2] = {
    { 5, font_letter_P, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterQ[2] = {
    { 5, font_letter_Q, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterR[2] = {
    { 5, font_letter_R, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterS[2] = {
    { 5, font_letter_S, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterT[2] = {
    { 5, font_letter_T, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterU[2] = {
    { 5, font_letter_U, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterV[2] = {
    { 5, font_letter_V, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterW[2] = {
    { 5, font_letter_W, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterX[2] = {
    { 5, font_letter_X, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterY[2] = {
    { 5, font_letter_Y, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture seg2_textureFontLetterZ[2] = {
    { 5, font_letter_Z, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontExclamationMark[2] = {
    { 5, font_exclamation_mark, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontMinus[2] = {
    { 5, font_minus, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontInterogationMark[2] = {
    { 5, font_interogation_mark, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontSimpleQuote[2] = {
    { 5, font_simple_quote, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontDoubleQuote[2] = {
    { 5, font_double_quote, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontPlus[2] = {
    { 5, font_plus, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontDot[2] = {
    { 5, font_dot, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontFourDote[2] = {
    { 5, font_four_dote, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontCC[2] = {
    { 5, font_cc, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberZero[2] = {
    { 5, font_number_zero, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberOne[2] = {
    { 5, font_number_one, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberTwo[2] = {
    { 5, font_number_two, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberThree[2] = {
    { 5, font_number_three, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberFour[2] = {
    { 5, font_number_four, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberFive[2] = {
    { 5, font_number_five, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberSix[2] = {
    { 5, font_number_six, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberSeven[2] = {
    { 5, font_number_seven, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberEight[2] = {
    { 5, font_number_eight, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontNumberNine[2] = {
    { 5, font_number_nine, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002824[2] = {
    { 5, gTexture7F35B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200284C[2] = {
    { 5, gTexture7F3634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002874[2] = {
    { 5, gTexture7F36B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200289C[2] = {
    { 5, gTexture7F3734, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020028C4[2] = {
    { 5, gTexture7F37B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020028EC[2] = {
    { 5, gTexture7F3AB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002914[2] = {
    { 5, gTexture7F3B34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200293C[2] = {
    { 5, gTexture7F3BB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002964[2] = {
    { 5, gTexture7F3C34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200298C[2] = {
    { 5, gTexture7F3CB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020029B4[2] = {
    { 5, gTexture7F3D34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020029DC[2] = {
    { 5, gTexture7F3DB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002A04[2] = {
    { 5, gTexture7F3E34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002A2C[2] = {
    { 5, gTexture7F3EB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002A54[2] = {
    { 5, gTexture7F3F34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002A7C[2] = {
    { 5, gTexture7F3FB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002AA4[2] = {
    { 5, gTexture7F4034, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002ACC[2] = {
    { 5, gTexture7F40B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002AF4[2] = {
    { 5, gTexture7F41B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002B1C[2] = {
    { 5, gTexture7F4234, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002B44[2] = {
    { 5, gTexture7F42B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002B6C[2] = {
    { 5, gTexture7F4334, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002B94[2] = {
    { 5, gTexture7F43B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002BBC[2] = {
    { 5, gTexture7F4434, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002BE4[2] = {
    { 5, gTexture7F44B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002C0C[2] = {
    { 5, gTexture7F4534, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002C34[2] = {
    { 5, gTexture7F45B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002C5C[2] = {
    { 5, gTexture7F4634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002C84[2] = {
    { 5, gTexture7F46B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002CAC[2] = {
    { 5, gTexture7F4734, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002CD4[2] = {
    { 5, gTexture7F47B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002CFC[2] = {
    { 5, gTexture7F4834, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002D24[2] = {
    { 5, gTexture7F48B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002D4C[2] = {
    { 5, gTexture7F4934, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002D74[2] = {
    { 5, gTexture7F49B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002D9C[2] = {
    { 5, gTexture7F4A34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002DC4[2] = {
    { 5, gTexture7F4AB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002DEC[2] = {
    { 5, gTexture7F4B34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002E14[2] = {
    { 5, gTexture7F4D34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002E3C[2] = {
    { 5, gTexture7F4DB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002E64[2] = {
    { 5, gTexture7F4E34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002E8C[2] = {
    { 5, gTexture7F4EB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002EB4[2] = {
    { 5, gTexture7F4F34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002EDC[2] = {
    { 5, gTexture7F4FB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002F04[2] = {
    { 5, gTexture7F5034, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002F2C[2] = {
    { 5, gTexture7F50B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002F54[2] = {
    { 5, gTexture7F5134, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002F7C[2] = {
    { 5, gTexture7F51B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002FA4[2] = {
    { 5, gTexture7F5234, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002FCC[2] = {
    { 5, gTexture7F52B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02002FF4[2] = {
    { 5, gTexture7F5334, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200301C[2] = {
    { 5, gTexture7F53B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003044[2] = {
    { 5, gTexture7F5434, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200306C[2] = {
    { 5, gTexture7F54B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003094[2] = {
    { 5, gTexture7F5534, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020030BC[2] = {
    { 5, gTexture7F55B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020030E4[2] = {
    { 5, gTexture7F5634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200310C[2] = {
    { 5, gTexture7F56B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003134[2] = {
    { 5, gTexture7F5734, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200315C[2] = {
    { 5, gTexture7F57B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003184[2] = {
    { 5, gTexture7F5834, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020031AC[2] = {
    { 5, gTexture7F58B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020031D4[2] = {
    { 5, gTexture7F5934, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020031FC[2] = {
    { 5, gTexture7F59B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003224[2] = {
    { 5, gTexture7F5A34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200324C[2] = {
    { 5, gTexture7F5AB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003274[2] = {
    { 5, gTexture7F5B34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200329C[2] = {
    { 5, gTexture7F5BB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020032C4[2] = {
    { 5, gTexture7F5C34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020032EC[2] = {
    { 5, gTexture7F5CB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003314[2] = {
    { 5, gTexture7F5D34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200333C[2] = {
    { 5, gTexture7F4BB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003364[2] = {
    { 5, gTexture7F4C34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200338C[2] = {
    { 5, gTexture7F4CB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020033B4[2] = {
    { 5, gTexture7F4134, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020033DC[2] = {
    { 5, gTexture7F3834, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003404[2] = {
    { 5, gTexture7F38B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200342C[2] = {
    { 5, gTexture7F3934, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003454[2] = {
    { 5, gTexture7F39B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200347C[2] = {
    { 5, gTexture7F3A34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020034A4[2] = {
    { 5, gTexture7F5DB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020034CC[2] = {
    { 5, gTexture7F5E34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020034F4[2] = {
    { 5, gTexture7F5EB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200351C[2] = {
    { 5, gTexture7F5F34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003544[2] = {
    { 5, gTexture7F5FB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200356C[2] = {
    { 5, gTexture7F62B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003594[2] = {
    { 5, gTexture7F6334, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020035BC[2] = {
    { 5, gTexture7F63B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020035E4[2] = {
    { 5, gTexture7F6434, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200360C[2] = {
    { 5, gTexture7F64B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003634[2] = {
    { 5, gTexture7F6534, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200365C[2] = {
    { 5, gTexture7F65B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003684[2] = {
    { 5, gTexture7F6634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020036AC[2] = {
    { 5, gTexture7F66B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020036D4[2] = {
    { 5, gTexture7F6734, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020036FC[2] = {
    { 5, gTexture7F67B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003724[2] = {
    { 5, gTexture7F6834, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200374C[2] = {
    { 5, gTexture7F68B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003774[2] = {
    { 5, gTexture7F69B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200379C[2] = {
    { 5, gTexture7F6A34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020037C4[2] = {
    { 5, gTexture7F6AB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020037EC[2] = {
    { 5, gTexture7F6B34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003814[2] = {
    { 5, gTexture7F6BB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200383C[2] = {
    { 5, gTexture7F6C34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003864[2] = {
    { 5, gTexture7F6CB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200388C[2] = {
    { 5, gTexture7F6D34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020038B4[2] = {
    { 5, gTexture7F6DB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020038DC[2] = {
    { 5, gTexture7F6E34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003904[2] = {
    { 5, gTexture7F6EB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200392C[2] = {
    { 5, gTexture7F6F34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003954[2] = {
    { 5, gTexture7F6FB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200397C[2] = {
    { 5, gTexture7F7034, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020039A4[2] = {
    { 5, gTexture7F70B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020039CC[2] = {
    { 5, gTexture7F7134, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020039F4[2] = {
    { 5, gTexture7F71B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003A1C[2] = {
    { 5, gTexture7F7234, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003A44[2] = {
    { 5, gTexture7F72B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003A6C[2] = {
    { 5, gTexture7F7334, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003A94[2] = {
    { 5, gTexture7F7534, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003ABC[2] = {
    { 5, gTexture7F75B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003AE4[2] = {
    { 5, gTexture7F7634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003B0C[2] = {
    { 5, gTexture7F76B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003B34[2] = {
    { 5, gTexture7F7734, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003B5C[2] = {
    { 5, gTexture7F77B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003B84[2] = {
    { 5, gTexture7F7834, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003BAC[2] = {
    { 5, gTexture7F78B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003BD4[2] = {
    { 5, gTexture7F7934, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003BFC[2] = {
    { 5, gTexture7F79B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003C24[2] = {
    { 5, gTexture7F7A34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003C4C[2] = {
    { 5, gTexture7F7AB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003C74[2] = {
    { 5, gTexture7F7B34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003C9C[2] = {
    { 5, gTexture7F7BB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003CC4[2] = {
    { 5, gTexture7F7C34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003CEC[2] = {
    { 5, gTexture7F7CB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003D14[2] = {
    { 5, gTexture7F7D34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003D3C[2] = {
    { 5, gTexture7F7DB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003D64[2] = {
    { 5, gTexture7F7E34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003D8C[2] = {
    { 5, gTexture7F7EB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003DB4[2] = {
    { 5, gTexture7F7F34, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003DDC[2] = {
    { 5, gTexture7F7FB4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003E04[2] = {
    { 5, gTexture7F8034, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003E2C[2] = {
    { 5, gTexture7F80B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003E54[2] = {
    { 5, gTexture7F8134, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003E7C[2] = {
    { 5, gTexture7F81B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003EA4[2] = {
    { 5, gTexture7F8234, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003ECC[2] = {
    { 5, gTexture7F82B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003EF4[2] = {
    { 5, gTexture7F8334, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003F1C[2] = {
    { 5, gTexture7F83B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003F44[2] = {
    { 5, gTexture7F8434, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003F6C[2] = {
    { 5, gTexture7F84B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003F94[2] = {
    { 5, gTexture7F8534, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003FBC[2] = {
    { 5, gTexture7F73B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02003FE4[2] = {
    { 5, gTexture7F7434, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200400C[2] = {
    { 5, gTexture7F74B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004034[2] = {
    { 5, gTexture7F6934, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200405C[2] = {
    { 5, gTexture7F6034, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004084[2] = {
    { 5, gTexture7F60B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020040AC[2] = {
    { 5, gTexture7F6134, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020040D4[2] = {
    { 5, gTexture7F61B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020040FC[2] = {
    { 5, gTexture7F6234, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004124[2] = {
    { 5, gTexture7F9BF4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200414C[2] = {
    { 5, gTexture7F8734, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004174[2] = {
    { 5, gTexture7F8914, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200419C[2] = {
    { 5, gTexture7F8AF4, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020041C4[2] = {
    { 5, gTexture7F8CD4, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020041EC[2] = {
    { 5, gTexture7F8EB4, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004214[2] = {
    { 5, gTexture7F9094, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200423C[2] = {
    { 5, gTexture7F9274, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004264[2] = {
    { 5, gTexture7F9454, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200428C[2] = {
    { 5, gTexture7F9634, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020042B4[2] = {
    { 5, gTexture7F9814, 30, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020042DC[2] = {
    { 5, gTexture7F85B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004304[2] = {
    { 5, gTexture7F8634, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200432C[2] = {
    { 5, gTexture7F86B4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004354[2] = {
    { 5, gTexture7F99F4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200437C[2] = {
    { 5, gTexture7F9A74, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020043A4[2] = {
    { 5, gTexture7F9CF4, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020043CC[2] = {
    { 5, gTexture7F9DC4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020043F4[2] = {
    { 5, gTexture7F9E44, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200441C[2] = {
    { 5, gTexture7F9EC4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004444[2] = {
    { 5, font_apostrophe, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200446C[2] = {
    { 5, gTexture7FA0C4, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004494[2] = {
    { 5, gTexture7FA194, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020044BC[2] = {
    { 5, gTexture7FA264, 26, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020044E4[2] = {
    { 5, gTexture7F9AF4, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200450C[2] = {
    { 5, gTexture7F9B74, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture gTextureFontComma[2] = {
    { 5, font_comma, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200455C[2] = {
    { 5, gTexture7F9C74, 16, 16, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown empty MenuTexture
 *
 */
MenuTexture gTextureFontEmpty[1] = {
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the blue sky background
 *
 */
MenuTexture seg2_blue_sky_background_texture[2] = {
    { 0, background_blue_sky, 320, 240, 0, 0, 0xcd60 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the sunset background
 *
 */
MenuTexture seg2_sunset_background_texture[2] = {
    { 0, background_sunset, 320, 240, 0, 0, 0x93c4 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for copyright
 *
 */
MenuTexture seg2_copyright_1996_texture[2] = {
    { 1, copyright_1996, 124, 17, 98, 199, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for press start
 *
 */
MenuTexture seg2_push_start_button_texture[2] = {
    { 1, push_start_button, 159, 16, 81, 179, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the options menu
 *
 */
MenuTexture D_02004638[2] = {
    { 0, texture_option, 130, 32, 95, 16, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the game select menu
 *
 */
MenuTexture seg2_game_select_texture[2] = {
    { 0, texture_game_select, 200, 32, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the first colone one player
 *
 */
MenuTexture seg2_menu_1p_column[4] = {
    { 1, texture_menu_1p_game, 64, 54, 0, 0, 0x0 },
    { 1, texture_mode_mario_gp, 64, 18, 0, 65, 0x0 },
    { 1, texture_mode_time_trials, 64, 18, 0, 83, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the first colone two player
 *
 */
MenuTexture seg2_menu_2p_column[5] = {
    { 1, texture_menu_2p_game, 64, 54, 0, 0, 0x0 },
    { 1, texture_mode_mario_gp, 64, 18, 0, 65, 0x0 },
    { 1, texture_mode_vs, 64, 18, 0, 83, 0x0 },
    { 1, texture_mode_battle, 64, 18, 0, 101, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the first colone three player
 *
 */
MenuTexture seg2_menu_3p_column[4] = {
    { 1, texture_menu_3p_game, 64, 54, 0, 0, 0x0 },
    { 1, texture_mode_vs, 64, 18, 0, 65, 0x0 },
    { 1, texture_mode_battle, 64, 18, 0, 83, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for the first colone four player
 *
 */
MenuTexture seg2_menu_4p_column[4] = {
    { 1, texture_menu_4p_game, 64, 54, 0, 0, 0x0 },
    { 1, texture_mode_vs, 64, 18, 0, 65, 0x0 },
    { 1, texture_mode_battle, 64, 18, 0, 83, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_020047DC[2] = {
    { 1, texture_small_green_triangle, 12, 7, 27, 56, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004804[2] = {
    { 1, texture_small_green_triangle, 12, 7, 27, 56, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200482C[2] = {
    { 1, texture_small_green_triangle, 12, 7, 27, 56, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_02004854[2] = {
    { 1, texture_small_green_triangle, 12, 7, 27, 56, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MenuTexture
 *
 */
MenuTexture D_0200487C[2] = {
    { 1, texture_ok, 31, 19, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu L Option
 *
 */
MenuTexture seg2_textureMenuLOption[2] = {
    { 1, texture_l_option, 58, 19, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu R Option
 *
 */
MenuTexture seg2_textureMenuRData[2] = {
    { 1, texture_r_data, 58, 19, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu 50cc
 *
 */
MenuTexture seg2_50_CC_texture[2] = {
    { 1, texture_50cc, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu 100cc
 *
 */
MenuTexture seg2_100_CC_texture[2] = {
    { 1, texture_100cc, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu 150cc
 *
 */
MenuTexture seg2_150_CC_texture[2] = {
    { 1, texture_150cc, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture for Menu Extra
 *
 */
MenuTexture seg2_extra_CC_texture[2] = {
    { 1, texture_extra, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Without Item
 *
 */
MenuTexture seg2_menu_no_item_texture[2] = {
    { 1, gTextureMenuWithoutItem, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu With Item
 *
 */
MenuTexture D_020049BC[2] = {
    { 1, gTextureMenuWithItem, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Begin
 *
 */
MenuTexture D_020049E4[2] = {
    { 1, texture_begin, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Ghost
 *
 */
MenuTexture D_02004A0C[2] = {
    { 1, texture_menu_ghost, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Data
 *
 */
MenuTexture seg2_data_texture[2] = {
    { 1, texture_data, 64, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures P1 Border ?
 *
 */
MenuTexture seg2_P1_border_texture[3] = {
    { 3, gTexture7E56E4, 64, 64, 0, 0, 0x116c },
    { 4, gTextureP1BorderBlue, 64, 64, 0, 0, 0x0000 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures P2 Border ?
 *
 */
MenuTexture seg2_P2_border_texture[3] = {
    { 3, gTexture7E56E4, 64, 64, 0, 0, 0x116c },
    { 4, gTextureP2BorderRed, 64, 64, 0, 0, 0x0000 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures P3 Border ?
 *
 */
MenuTexture seg2_P3_border_texture[3] = {
    { 3, gTexture7E56E4, 64, 64, 0, 0, 0x116c },
    { 4, gTextureP3BorderOrange, 64, 64, 0, 0, 0x0000 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures P4 Border ?
 *
 */
MenuTexture seg2_P4_border_texture[3] = {
    { 3, gTexture7E56E4, 64, 64, 0, 0, 0x116c },
    { 4, gTextureP4BorderGreen, 64, 64, 0, 0, 0x0000 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Player Select
 *
 */
MenuTexture D_02004B4C[2] = {
    { 0, texture_player_select, 220, 32, 51, 16, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu OK
 *
 */
MenuTexture D_02004B74[2] = {
    { 1, texture_ok, 31, 19, 264, 202, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Select
 *
 */
MenuTexture seg2_menu_select_texture[2] = {
    { 0, gTextureMapSelect, 190, 32, 65, 18, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Mushroom Cup
 *
 */
MenuTexture seg2_mushroom_cup_texture[2] = {
    { 1, gTextureMenuMushroomCup, 65, 40, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Flower Cup
 *
 */
MenuTexture seg2_flower_cup_texture[2] = {
    { 1, gTextureMenuFlowerCup, 65, 40, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Star Cup
 *
 */
MenuTexture seg2_star_cup_texture[2] = {
    { 1, gTextureMenuStarCup, 65, 40, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Menu Special Cup
 *
 */
MenuTexture seg2_special_cup_texture[2] = {
    { 1, gTextureMenuSpecialCup, 65, 40, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for the course preview mario raceway
 *
 */
MenuTexture seg2_mario_raceway_preview_small_texture[2] = {
    { 0, gTextureCoursePreviewMarioRaceway, 128, 78, 23, 112, 0x1e3d },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture title for mushroom cup
 *
 */
MenuTexture seg2_mushroom_cup_title_texture[5] = {
    { 1, gTextureTitleLuigiRaceway, 140, 18, 157, 112, 0x0 },
    { 1, gTextureTitleMooMooFarm, 140, 18, 157, 136, 0x0 },
    { 1, gTextureTitleKoopaTroopaBeach, 140, 18, 157, 160, 0x0 },
    { 1, gTextureTitleKalimariDesert, 140, 18, 157, 184, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture title for flower cup
 *
 */
MenuTexture seg2_flower_cup_title_texture[5] = {
    { 1, gTextureTitleToadsTurnpike, 140, 18, 157, 112, 0x0 },
    { 1, gTextureTitleFrappeSnowland, 140, 18, 157, 136, 0x0 },
    { 1, gTextureTitleChocoMountain, 140, 18, 157, 160, 0x0 },
    { 1, gTextureTitleMarioRaceway, 140, 18, 157, 184, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture title for star cup
 *
 */
MenuTexture seg2_star_cup_title_texture[5] = {
    { 1, gTextureTitleWarioStadium, 140, 18, 157, 112, 0x0 },
    { 1, gTextureTitleSherbetLand, 140, 18, 157, 136, 0x0 },
    { 1, gTextureTitleRoyalRaceway, 140, 18, 157, 160, 0x0 },
    { 1, gTextureTitleBowsersCastle, 140, 18, 157, 184, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTexture title for special cup
 *
 */
MenuTexture seg2_special_cup_title_texture[5] = {
    { 1, gTextureTitleDKsJungleParkway, 140, 18, 157, 112, 0x0 },
    { 1, gTextureTitleYoshiValley, 140, 18, 157, 136, 0x0 },
    { 1, gTextureTitleBansheeBoardwalk, 140, 18, 157, 160, 0x0 },
    { 1, gTextureTitleRainbowRoad, 140, 18, 157, 184, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title for battle mode
 *
 */
MenuTexture seg2_battle_title_texture[5] = {
    { 1, gTextureTitleBigDonut, 140, 18, 157, 112, 0x0 },
    { 1, gTextureTitleBlockFort, 140, 18, 157, 136, 0x0 },
    { 1, gTextureTitleDoubleDeck, 140, 18, 157, 160, 0x0 },
    { 1, gTextureTitleSkyscraper, 140, 18, 157, 184, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures Menu OK
 *
 */
MenuTexture D_02004E80[2] = {
    { 1, texture_ok, 31, 19, 265, 208, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Mario Raceway
 *
 */
MenuTexture seg2_mario_raceway_title_texture[2] = {
    { 1, gTextureTitleMarioRaceway, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Choco Mountain
 *
 */
MenuTexture seg2_choco_mountain_title_texture[2] = {
    { 1, gTextureTitleChocoMountain, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief  MenuTextures title Bowser's Castle
 *
 */
MenuTexture D_02004EF8[2] = {
    { 1, gTextureTitleBowsersCastle, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Banshee Boardwalk
 *
 */
MenuTexture D_02004F20[2] = {
    { 1, gTextureTitleBansheeBoardwalk, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Yoshi Valley
 *
 */
MenuTexture D_02004F48[2] = {
    { 1, gTextureTitleYoshiValley, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Frappe Snowland
 *
 */
MenuTexture D_02004F70[2] = {
    { 1, gTextureTitleFrappeSnowland, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief  MenuTextures title Koopa Troopa Beach
 *
 */
MenuTexture D_02004F98[2] = {
    { 1, gTextureTitleKoopaTroopaBeach, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Royal Raceway
 *
 */
MenuTexture D_02004FC0[2] = {
    { 1, gTextureTitleRoyalRaceway, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Luigi Raceway
 *
 */
MenuTexture D_02004FE8[2] = {
    { 1, gTextureTitleLuigiRaceway, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Moo Moo Farm
 *
 */
MenuTexture D_02005010[2] = {
    { 1, gTextureTitleMooMooFarm, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Toad's Turnpike
 *
 */
MenuTexture D_02005038[2] = {
    { 1, gTextureTitleToadsTurnpike, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Kalimari Desert
 *
 */
MenuTexture D_02005060[2] = {
    { 1, gTextureTitleKalimariDesert, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Sherbet Land
 *
 */
MenuTexture D_02005088[2] = {
    { 1, gTextureTitleSherbetLand, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Rainbow Road
 *
 */
MenuTexture D_020050B0[2] = {
    { 1, gTextureTitleRainbowRoad, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Wario Stadium
 *
 */
MenuTexture D_020050D8[2] = {
    { 1, gTextureTitleWarioStadium, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Block Fort
 *
 */
MenuTexture D_02005100[2] = {
    { 1, gTextureTitleBlockFort, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Skyscraper
 *
 */
MenuTexture D_02005128[2] = {
    { 1, gTextureTitleSkyscraper, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Double Deck
 *
 */
MenuTexture D_02005150[2] = {
    { 1, gTextureTitleDoubleDeck, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title DK's Jungle Parkway
 *
 */
MenuTexture D_02005178[2] = {
    { 1, gTextureTitleDKsJungleParkway, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures title Big Donut
 *
 */
MenuTexture D_020051A0[2] = {
    { 1, gTextureTitleBigDonut, 140, 18, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 00
 *
 */
MenuTexture D_020051C8[2] = {
    { 0, gTextureMarioFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 01
 *
 */
MenuTexture D_020051F0[2] = {
    { 0, gTextureMarioFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 02
 *
 */
MenuTexture D_02005218[2] = {
    { 0, gTextureMarioFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 03
 *
 */
MenuTexture D_02005240[2] = {
    { 0, gTextureMarioFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 04
 *
 */
MenuTexture D_02005268[2] = {
    { 0, gTextureMarioFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 05
 *
 */
MenuTexture D_02005290[2] = {
    { 0, gTextureMarioFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 06
 *
 */
MenuTexture D_020052B8[2] = {
    { 0, gTextureMarioFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 07
 *
 */
MenuTexture D_020052E0[2] = {
    { 0, gTextureMarioFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 08
 *
 */
MenuTexture D_02005308[2] = {
    { 0, gTextureMarioFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 09
 *
 */
MenuTexture D_02005330[2] = {
    { 0, gTextureMarioFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 10
 *
 */
MenuTexture D_02005358[2] = {
    { 0, gTextureMarioFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 11
 *
 */
MenuTexture D_02005380[2] = {
    { 0, gTextureMarioFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 12
 *
 */
MenuTexture D_020053A8[2] = {
    { 0, gTextureMarioFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 13
 *
 */
MenuTexture D_020053D0[2] = {
    { 0, gTextureMarioFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 14
 *
 */
MenuTexture D_020053F8[2] = {
    { 0, gTextureMarioFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 15
 *
 */
MenuTexture D_02005420[2] = {
    { 0, gTextureMarioFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Mario Face 16
 *
 */
MenuTexture D_02005448[2] = {
    { 0, gTextureMarioFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 00
 *
 */
MenuTexture D_02005470[2] = {
    { 0, gTextureLuigiFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 01
 *
 */
MenuTexture D_02005498[2] = {
    { 0, gTextureLuigiFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 02
 *
 */
MenuTexture D_020054C0[2] = {
    { 0, gTextureLuigiFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 03
 *
 */
MenuTexture D_020054E8[2] = {
    { 0, gTextureLuigiFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 04
 *
 */
MenuTexture D_02005510[2] = {
    { 0, gTextureLuigiFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 05
 *
 */
MenuTexture D_02005538[2] = {
    { 0, gTextureLuigiFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 06
 *
 */
MenuTexture D_02005560[2] = {
    { 0, gTextureLuigiFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 07
 *
 */
MenuTexture D_02005588[2] = {
    { 0, gTextureLuigiFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 08
 *
 */
MenuTexture D_020055B0[2] = {
    { 0, gTextureLuigiFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 09
 *
 */
MenuTexture D_020055D8[2] = {
    { 0, gTextureLuigiFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 10
 *
 */
MenuTexture D_02005600[2] = {
    { 0, gTextureLuigiFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 11
 *
 */
MenuTexture D_02005628[2] = {
    { 0, gTextureLuigiFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 12
 *
 */
MenuTexture D_02005650[2] = {
    { 0, gTextureLuigiFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 13
 *
 */
MenuTexture D_02005678[2] = {
    { 0, gTextureLuigiFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 14
 *
 */
MenuTexture D_020056A0[2] = {
    { 0, gTextureLuigiFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 15
 *
 */
MenuTexture D_020056C8[2] = {
    { 0, gTextureLuigiFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Luigi Face 16
 *
 */
MenuTexture D_020056F0[2] = {
    { 0, gTextureLuigiFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 00
 *
 */
MenuTexture D_02005718[2] = {
    { 0, gTextureToadFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 01
 *
 */
MenuTexture D_02005740[2] = {
    { 0, gTextureToadFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 02
 *
 */
MenuTexture D_02005768[2] = {
    { 0, gTextureToadFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 03
 *
 */
MenuTexture D_02005790[2] = {
    { 0, gTextureToadFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 04
 *
 */
MenuTexture D_020057B8[2] = {
    { 0, gTextureToadFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 05
 *
 */
MenuTexture D_020057E0[2] = {
    { 0, gTextureToadFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 06
 *
 */
MenuTexture D_02005808[2] = {
    { 0, gTextureToadFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 07
 *
 */
MenuTexture D_02005830[2] = {
    { 0, gTextureToadFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 08
 *
 */
MenuTexture D_02005858[2] = {
    { 0, gTextureToadFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 09
 *
 */
MenuTexture D_02005880[2] = {
    { 0, gTextureToadFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 10
 *
 */
MenuTexture D_020058A8[2] = {
    { 0, gTextureToadFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 11
 *
 */
MenuTexture D_020058D0[2] = {
    { 0, gTextureToadFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 12
 *
 */
MenuTexture D_020058F8[2] = {
    { 0, gTextureToadFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 13
 *
 */
MenuTexture D_02005920[2] = {
    { 0, gTextureToadFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 14
 *
 */
MenuTexture D_02005948[2] = {
    { 0, gTextureToadFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 15
 *
 */
MenuTexture D_02005970[2] = {
    { 0, gTextureToadFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Toad Face 16
 *
 */
MenuTexture D_02005998[2] = {
    { 0, gTextureToadFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 00
 *
 */
MenuTexture D_020059C0[2] = {
    { 0, gTexturePeachFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 01
 *
 */
MenuTexture D_020059E8[2] = {
    { 0, gTexturePeachFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 02
 *
 */
MenuTexture D_02005A10[2] = {
    { 0, gTexturePeachFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 03
 *
 */
MenuTexture D_02005A38[2] = {
    { 0, gTexturePeachFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 04
 *
 */
MenuTexture D_02005A60[2] = {
    { 0, gTexturePeachFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 05
 *
 */
MenuTexture D_02005A88[2] = {
    { 0, gTexturePeachFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 06
 *
 */
MenuTexture D_02005AB0[2] = {
    { 0, gTexturePeachFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 07
 *
 */
MenuTexture D_02005AD8[2] = {
    { 0, gTexturePeachFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 08
 *
 */
MenuTexture D_02005B00[2] = {
    { 0, gTexturePeachFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 09
 *
 */
MenuTexture D_02005B28[2] = {
    { 0, gTexturePeachFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 10
 *
 */
MenuTexture D_02005B50[2] = {
    { 0, gTexturePeachFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 11
 *
 */
MenuTexture D_02005B78[2] = {
    { 0, gTexturePeachFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 12
 *
 */
MenuTexture D_02005BA0[2] = {
    { 0, gTexturePeachFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 13
 *
 */
MenuTexture D_02005BC8[2] = {
    { 0, gTexturePeachFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 14
 *
 */
MenuTexture D_02005BF0[2] = {
    { 0, gTexturePeachFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 15
 *
 */
MenuTexture D_02005C18[2] = {
    { 0, gTexturePeachFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Peach Face 16
 *
 */
MenuTexture D_02005C40[2] = {
    { 0, gTexturePeachFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 00
 *
 */
MenuTexture D_02005C68[2] = {
    { 0, gTextureYoshiFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 01
 *
 */
MenuTexture D_02005C90[2] = {
    { 0, gTextureYoshiFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 02
 *
 */
MenuTexture D_02005CB8[2] = {
    { 0, gTextureYoshiFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 03
 *
 */

MenuTexture D_02005CE0[2] = {
    { 0, gTextureYoshiFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 04
 *
 */
MenuTexture D_02005D08[2] = {
    { 0, gTextureYoshiFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 05
 *
 */
MenuTexture D_02005D30[2] = {
    { 0, gTextureYoshiFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 06
 *
 */
MenuTexture D_02005D58[2] = {
    { 0, gTextureYoshiFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 07
 *
 */
MenuTexture D_02005D80[2] = {
    { 0, gTextureYoshiFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 08
 *
 */
MenuTexture D_02005DA8[2] = {
    { 0, gTextureYoshiFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 09
 *
 */
MenuTexture D_02005DD0[2] = {
    { 0, gTextureYoshiFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 10
 *
 */
MenuTexture D_02005DF8[2] = {
    { 0, gTextureYoshiFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 11
 *
 */
MenuTexture D_02005E20[2] = {
    { 0, gTextureYoshiFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 12
 *
 */
MenuTexture D_02005E48[2] = {
    { 0, gTextureYoshiFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 13
 *
 */
MenuTexture D_02005E70[2] = {
    { 0, gTextureYoshiFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 14
 *
 */
MenuTexture D_02005E98[2] = {
    { 0, gTextureYoshiFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 15
 *
 */
MenuTexture D_02005EC0[2] = {
    { 0, gTextureYoshiFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Yoshi Face 16
 *
 */
MenuTexture D_02005EE8[2] = {
    { 0, gTextureYoshiFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 00
 *
 */
MenuTexture D_02005F10[2] = {
    { 0, gTextureDkFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 01
 *
 */
MenuTexture D_02005F38[2] = {
    { 0, gTextureDkFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 02
 *
 */
MenuTexture D_02005F60[2] = {
    { 0, gTextureDkFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 03
 *
 */
MenuTexture D_02005F88[2] = {
    { 0, gTextureDkFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 04
 *
 */
MenuTexture D_02005FB0[2] = {
    { 0, gTextureDkFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 05
 *
 */
MenuTexture D_02005FD8[2] = {
    { 0, gTextureDkFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 06
 *
 */
MenuTexture D_02006000[2] = {
    { 0, gTextureDkFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 07
 *
 */
MenuTexture D_02006028[2] = {
    { 0, gTextureDkFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 08
 *
 */
MenuTexture D_02006050[2] = {
    { 0, gTextureDkFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 09
 *
 */
MenuTexture D_02006078[2] = {
    { 0, gTextureDkFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 10
 *
 */
MenuTexture D_020060A0[2] = {
    { 0, gTextureDkFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 11
 *
 */
MenuTexture D_020060C8[2] = {
    { 0, gTextureDkFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 12
 *
 */
MenuTexture D_020060F0[2] = {
    { 0, gTextureDkFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 13
 *
 */
MenuTexture D_02006118[2] = {
    { 0, gTextureDkFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 14
 *
 */
MenuTexture D_02006140[2] = {
    { 0, gTextureDkFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 15
 *
 */
MenuTexture D_02006168[2] = {
    { 0, gTextureDkFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for DK Face 16
 *
 */
MenuTexture D_02006190[2] = {
    { 0, gTextureDkFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 00
 *
 */
MenuTexture D_020061B8[2] = {
    { 0, gTextureWarioFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 01
 *
 */
MenuTexture D_020061E0[2] = {
    { 0, gTextureWarioFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 02
 *
 */
MenuTexture D_02006208[2] = {
    { 0, gTextureWarioFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 03
 *
 */
MenuTexture D_02006230[2] = {
    { 0, gTextureWarioFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 04
 *
 */
MenuTexture D_02006258[2] = {
    { 0, gTextureWarioFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 05
 *
 */
MenuTexture D_02006280[2] = {
    { 0, gTextureWarioFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 06
 *
 */
MenuTexture D_020062A8[2] = {
    { 0, gTextureWarioFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 07
 *
 */
MenuTexture D_020062D0[2] = {
    { 0, gTextureWarioFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 08
 *
 */
MenuTexture D_020062F8[2] = {
    { 0, gTextureWarioFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 09
 *
 */
MenuTexture D_02006320[2] = {
    { 0, gTextureWarioFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 10
 *
 */
MenuTexture D_02006348[2] = {
    { 0, gTextureWarioFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 11
 *
 */
MenuTexture D_02006370[2] = {
    { 0, gTextureWarioFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 12
 *
 */
MenuTexture D_02006398[2] = {
    { 0, gTextureWarioFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 13
 *
 */
MenuTexture D_020063C0[2] = {
    { 0, gTextureWarioFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 14
 *
 */
MenuTexture D_020063E8[2] = {
    { 0, gTextureWarioFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 15
 *
 */
MenuTexture D_02006410[2] = {
    { 0, gTextureWarioFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Wario Face 16
 *
 */
MenuTexture D_02006438[2] = {
    { 0, gTextureWarioFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 00
 *
 */
MenuTexture D_02006460[2] = {
    { 0, gTextureBowserFace00, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 01
 *
 */
MenuTexture D_02006488[2] = {
    { 0, gTextureBowserFace01, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 02
 *
 */
MenuTexture D_020064B0[2] = {
    { 0, gTextureBowserFace02, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 03
 *
 */
MenuTexture D_020064D8[2] = {
    { 0, gTextureBowserFace03, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 04
 *
 */
MenuTexture D_02006500[2] = {
    { 0, gTextureBowserFace04, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 05
 *
 */
MenuTexture D_02006528[2] = {
    { 0, gTextureBowserFace05, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 06
 *
 */
MenuTexture D_02006550[2] = {
    { 0, gTextureBowserFace06, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 07
 *
 */
MenuTexture D_02006578[2] = {
    { 0, gTextureBowserFace07, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 08
 *
 */
MenuTexture D_020065A0[2] = {
    { 0, gTextureBowserFace08, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 09
 *
 */
MenuTexture D_020065C8[2] = {
    { 0, gTextureBowserFace09, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 10
 *
 */
MenuTexture D_020065F0[2] = {
    { 0, gTextureBowserFace10, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 11
 *
 */
MenuTexture D_02006618[2] = {
    { 0, gTextureBowserFace11, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 12
 *
 */
MenuTexture D_02006640[2] = {
    { 0, gTextureBowserFace12, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 13
 *
 */
MenuTexture D_02006668[2] = {
    { 0, gTextureBowserFace13, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 14
 *
 */
MenuTexture D_02006690[2] = {
    { 0, gTextureBowserFace14, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 15
 *
 */
MenuTexture D_020066B8[2] = {
    { 0, gTextureBowserFace15, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief MenuTextures for Bowser Face 16
 *
 */
MenuTexture D_020066E0[2] = {
    { 0, gTextureBowserFace16, 64, 64, 0, 0, 0x0 },
    { 0, NULL, 0, 0, 0, 0, 0 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_02006708[2] = {
    { D_020051C8, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_02006718[12] = {
    { D_020052B8, 0x00000001 }, { D_020052E0, 0x00000001 }, { D_02005308, 0x00000001 }, { D_02005330, 0x00000001 },
    { D_02005358, 0x00000001 }, { D_02005380, 0x00000001 }, { D_020053A8, 0x00000001 }, { D_020053D0, 0x00000001 },
    { D_020053F8, 0x00000001 }, { D_02005420, 0x00000005 }, { D_02005420, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_02006778[2] = {
    { D_02005420, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_02006788[11] = {
    { D_02005420, 0x00000002 }, { D_020053F8, 0x00000002 }, { D_020053D0, 0x00000002 }, { D_020053A8, 0x00000002 },
    { D_02005380, 0x00000002 }, { D_02005358, 0x00000002 }, { D_02005330, 0x00000002 }, { D_02005308, 0x00000002 },
    { D_020052E0, 0x00000002 }, { D_020052B8, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_020067E0[11] = {
    { D_020051F0, 0x00000001 }, { D_02005218, 0x00000001 }, { D_02005240, 0x00000001 }, { D_02005268, 0x00000001 },
    { D_02005290, 0x00000001 }, { D_02005268, 0x00000001 }, { D_02005240, 0x00000001 }, { D_02005218, 0x00000001 },
    { D_020051F0, 0x00000001 }, { D_020051C8, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_02006838[21] = {
    { D_020051F0, 0x00000001 }, { D_02005218, 0x00000001 }, { D_02005240, 0x00000001 }, { D_02005268, 0x00000001 },
    { D_02005290, 0x00000001 }, { D_02005268, 0x00000001 }, { D_02005240, 0x00000001 }, { D_02005218, 0x00000001 },
    { D_020051F0, 0x00000001 }, { D_020051C8, 0x00000001 }, { D_020051F0, 0x00000001 }, { D_02005218, 0x00000001 },
    { D_02005240, 0x00000001 }, { D_02005268, 0x00000001 }, { D_02005290, 0x00000001 }, { D_02005268, 0x00000001 },
    { D_02005240, 0x00000001 }, { D_02005218, 0x00000001 }, { D_020051F0, 0x00000001 }, { D_020051C8, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Mario face ?
 */
MkAnimation D_020068E0[2] = {
    { D_02005448, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_020068F0[2] = {
    { D_02005470, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_02006900[12] = {
    { D_02005560, 0x00000001 }, { D_02005588, 0x00000001 }, { D_020055B0, 0x00000001 }, { D_020055D8, 0x00000001 },
    { D_02005600, 0x00000001 }, { D_02005628, 0x00000001 }, { D_02005650, 0x00000001 }, { D_02005678, 0x00000001 },
    { D_020056A0, 0x00000001 }, { D_020056C8, 0x00000005 }, { D_020056C8, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_02006960[2] = {
    { D_020056C8, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_02006970[11] = {
    { D_020056C8, 0x00000002 }, { D_020056A0, 0x00000002 }, { D_02005678, 0x00000002 }, { D_02005650, 0x00000002 },
    { D_02005628, 0x00000002 }, { D_02005600, 0x00000002 }, { D_020055D8, 0x00000002 }, { D_020055B0, 0x00000002 },
    { D_02005588, 0x00000002 }, { D_02005560, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_020069C8[11] = {
    { D_02005498, 0x00000001 }, { D_020054C0, 0x00000001 }, { D_020054E8, 0x00000001 }, { D_02005510, 0x00000001 },
    { D_02005538, 0x00000001 }, { D_02005510, 0x00000001 }, { D_020054E8, 0x00000001 }, { D_020054C0, 0x00000001 },
    { D_02005498, 0x00000001 }, { D_02005470, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_02006A20[21] = {
    { D_02005498, 0x00000001 }, { D_020054C0, 0x00000001 }, { D_020054E8, 0x00000001 }, { D_02005510, 0x00000001 },
    { D_02005538, 0x00000001 }, { D_02005510, 0x00000001 }, { D_020054E8, 0x00000001 }, { D_020054C0, 0x00000001 },
    { D_02005498, 0x00000001 }, { D_02005470, 0x00000001 }, { D_02005498, 0x00000001 }, { D_020054C0, 0x00000001 },
    { D_020054E8, 0x00000001 }, { D_02005510, 0x00000001 }, { D_02005538, 0x00000001 }, { D_02005510, 0x00000001 },
    { D_020054E8, 0x00000001 }, { D_020054C0, 0x00000001 }, { D_02005498, 0x00000001 }, { D_02005470, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Luigi face ?
 */
MkAnimation D_02006AC8[2] = {
    { D_020056F0, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006AD8[2] = {
    { D_02006460, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006AE8[12] = {
    { D_02006550, 0x00000001 }, { D_02006578, 0x00000001 }, { D_020065A0, 0x00000001 }, { D_020065C8, 0x00000001 },
    { D_020065F0, 0x00000001 }, { D_02006618, 0x00000001 }, { D_02006640, 0x00000001 }, { D_02006668, 0x00000001 },
    { D_02006690, 0x00000001 }, { D_020066B8, 0x00000005 }, { D_020066B8, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006B48[2] = {
    { D_020066B8, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006B58[11] = {
    { D_020066B8, 0x00000002 }, { D_02006690, 0x00000002 }, { D_02006668, 0x00000002 }, { D_02006640, 0x00000002 },
    { D_02006618, 0x00000002 }, { D_020065F0, 0x00000002 }, { D_020065C8, 0x00000002 }, { D_020065A0, 0x00000002 },
    { D_02006578, 0x00000002 }, { D_02006550, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006BB0[11] = {
    { D_02006488, 0x00000001 }, { D_020064B0, 0x00000001 }, { D_020064D8, 0x00000001 }, { D_02006500, 0x00000001 },
    { D_02006528, 0x00000001 }, { D_02006500, 0x00000001 }, { D_020064D8, 0x00000001 }, { D_020064B0, 0x00000001 },
    { D_02006488, 0x00000001 }, { D_02006460, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006C08[21] = {
    { D_02006488, 0x00000001 }, { D_020064B0, 0x00000001 }, { D_020064D8, 0x00000001 }, { D_02006500, 0x00000001 },
    { D_02006528, 0x00000001 }, { D_02006500, 0x00000001 }, { D_020064D8, 0x00000001 }, { D_020064B0, 0x00000001 },
    { D_02006488, 0x00000001 }, { D_02006460, 0x00000001 }, { D_02006488, 0x00000001 }, { D_020064B0, 0x00000001 },
    { D_020064D8, 0x00000001 }, { D_02006500, 0x00000001 }, { D_02006528, 0x00000001 }, { D_02006500, 0x00000001 },
    { D_020064D8, 0x00000001 }, { D_020064B0, 0x00000001 }, { D_02006488, 0x00000001 }, { D_02006460, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Bowser face ?
 */
MkAnimation D_02006CB0[2] = {
    { D_020066E0, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006CC0[2] = {
    { D_020059C0, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006CD0[12] = {
    { D_02005AB0, 0x00000001 }, { D_02005AD8, 0x00000001 }, { D_02005B00, 0x00000001 }, { D_02005B28, 0x00000001 },
    { D_02005B50, 0x00000001 }, { D_02005B78, 0x00000001 }, { D_02005BA0, 0x00000001 }, { D_02005BC8, 0x00000001 },
    { D_02005BF0, 0x00000001 }, { D_02005C18, 0x00000005 }, { D_02005C18, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006D30[2] = {
    { D_02005C18, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006D40[11] = {
    { D_02005C18, 0x00000002 }, { D_02005BF0, 0x00000002 }, { D_02005BC8, 0x00000002 }, { D_02005BA0, 0x00000002 },
    { D_02005B78, 0x00000002 }, { D_02005B50, 0x00000002 }, { D_02005B28, 0x00000002 }, { D_02005B00, 0x00000002 },
    { D_02005AD8, 0x00000002 }, { D_02005AB0, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006D98[11] = {
    { D_020059E8, 0x00000001 }, { D_02005A10, 0x00000001 }, { D_02005A38, 0x00000001 }, { D_02005A60, 0x00000001 },
    { D_02005A88, 0x00000001 }, { D_02005A60, 0x00000001 }, { D_02005A38, 0x00000001 }, { D_02005A10, 0x00000001 },
    { D_020059E8, 0x00000001 }, { D_020059C0, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006DF0[21] = {
    { D_020059E8, 0x00000001 }, { D_02005A10, 0x00000001 }, { D_02005A38, 0x00000001 }, { D_02005A60, 0x00000001 },
    { D_02005A88, 0x00000001 }, { D_02005A60, 0x00000001 }, { D_02005A38, 0x00000001 }, { D_02005A10, 0x00000001 },
    { D_020059E8, 0x00000001 }, { D_020059C0, 0x00000001 }, { D_020059E8, 0x00000001 }, { D_02005A10, 0x00000001 },
    { D_02005A38, 0x00000001 }, { D_02005A60, 0x00000001 }, { D_02005A88, 0x00000001 }, { D_02005A60, 0x00000001 },
    { D_02005A38, 0x00000001 }, { D_02005A10, 0x00000001 }, { D_020059E8, 0x00000001 }, { D_020059C0, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Peach face ?
 */
MkAnimation D_02006E98[2] = {
    { D_02005C40, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006EA8[2] = {
    { D_02005718, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006EB8[12] = {
    { D_02005808, 0x00000001 }, { D_02005830, 0x00000001 }, { D_02005858, 0x00000001 }, { D_02005880, 0x00000001 },
    { D_020058A8, 0x00000001 }, { D_020058D0, 0x00000001 }, { D_020058F8, 0x00000001 }, { D_02005920, 0x00000001 },
    { D_02005948, 0x00000001 }, { D_02005970, 0x00000005 }, { D_02005970, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006F18[2] = {
    { D_02005970, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006F28[11] = {
    { D_02005970, 0x00000002 }, { D_02005948, 0x00000002 }, { D_02005920, 0x00000002 }, { D_020058F8, 0x00000002 },
    { D_020058D0, 0x00000002 }, { D_020058A8, 0x00000002 }, { D_02005880, 0x00000002 }, { D_02005858, 0x00000002 },
    { D_02005830, 0x00000002 }, { D_02005808, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006F80[11] = {
    { D_02005740, 0x00000001 }, { D_02005768, 0x00000001 }, { D_02005790, 0x00000001 }, { D_020057B8, 0x00000001 },
    { D_020057E0, 0x00000001 }, { D_020057B8, 0x00000001 }, { D_02005790, 0x00000001 }, { D_02005768, 0x00000001 },
    { D_02005740, 0x00000001 }, { D_02005718, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02006FD8[21] = {
    { D_02005740, 0x00000001 }, { D_02005768, 0x00000001 }, { D_02005790, 0x00000001 }, { D_020057B8, 0x00000001 },
    { D_020057E0, 0x00000001 }, { D_020057B8, 0x00000001 }, { D_02005790, 0x00000001 }, { D_02005768, 0x00000001 },
    { D_02005740, 0x00000001 }, { D_02005718, 0x00000001 }, { D_02005740, 0x00000001 }, { D_02005768, 0x00000001 },
    { D_02005790, 0x00000001 }, { D_020057B8, 0x00000001 }, { D_020057E0, 0x00000001 }, { D_020057B8, 0x00000001 },
    { D_02005790, 0x00000001 }, { D_02005768, 0x00000001 }, { D_02005740, 0x00000001 }, { D_02005718, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Toad face ?
 */
MkAnimation D_02007080[2] = {
    { D_02005998, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_02007090[2] = {
    { D_02005C68, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_020070A0[12] = {
    { D_02005D58, 0x00000001 }, { D_02005D80, 0x00000001 }, { D_02005DA8, 0x00000001 }, { D_02005DD0, 0x00000001 },
    { D_02005DF8, 0x00000001 }, { D_02005E20, 0x00000001 }, { D_02005E48, 0x00000001 }, { D_02005E70, 0x00000001 },
    { D_02005E98, 0x00000001 }, { D_02005EC0, 0x00000005 }, { D_02005EC0, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_02007100[2] = {
    { D_02005EC0, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_02007110[12] = {
    { D_02005EC0, 0x00000002 }, { D_02005E98, 0x00000002 }, { D_02005E70, 0x00000002 }, { D_02005E48, 0x00000002 },
    { D_02005E20, 0x00000002 }, { D_02005DF8, 0x00000002 }, { D_02005DD0, 0x00000002 }, { D_02005DA8, 0x00000002 },
    { D_02005D80, 0x00000002 }, { D_02005D58, 0x00000002 }, { D_02005D58, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_02007170[11] = {
    { D_02005C90, 0x00000001 }, { D_02005CB8, 0x00000001 }, { D_02005CE0, 0x00000001 }, { D_02005D08, 0x00000001 },
    { D_02005D30, 0x00000001 }, { D_02005D08, 0x00000001 }, { D_02005CE0, 0x00000001 }, { D_02005CB8, 0x00000001 },
    { D_02005C90, 0x00000001 }, { D_02005C68, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_020071C8[21] = {
    { D_02005C90, 0x00000001 }, { D_02005CB8, 0x00000001 }, { D_02005CE0, 0x00000001 }, { D_02005D08, 0x00000001 },
    { D_02005D30, 0x00000001 }, { D_02005D08, 0x00000001 }, { D_02005CE0, 0x00000001 }, { D_02005CB8, 0x00000001 },
    { D_02005C90, 0x00000001 }, { D_02005C68, 0x00000001 }, { D_02005C90, 0x00000001 }, { D_02005CB8, 0x00000001 },
    { D_02005CE0, 0x00000001 }, { D_02005D08, 0x00000001 }, { D_02005D30, 0x00000001 }, { D_02005D08, 0x00000001 },
    { D_02005CE0, 0x00000001 }, { D_02005CB8, 0x00000001 }, { D_02005C90, 0x00000001 }, { D_02005C68, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Yoshi face ?
 */
MkAnimation D_02007270[2] = {
    { D_02005EE8, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_02007280[2] = {
    { D_02005F10, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_02007290[12] = {
    { D_02006000, 0x00000001 }, { D_02006028, 0x00000001 }, { D_02006050, 0x00000001 }, { D_02006078, 0x00000001 },
    { D_020060A0, 0x00000001 }, { D_020060C8, 0x00000001 }, { D_020060F0, 0x00000001 }, { D_02006118, 0x00000001 },
    { D_02006140, 0x00000001 }, { D_02006168, 0x00000005 }, { D_02006168, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_020072F0[2] = {
    { D_02006168, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_02007300[11] = {
    { D_02006168, 0x00000002 }, { D_02006140, 0x00000002 }, { D_02006118, 0x00000002 }, { D_020060F0, 0x00000002 },
    { D_020060C8, 0x00000002 }, { D_020060A0, 0x00000002 }, { D_02006078, 0x00000002 }, { D_02006050, 0x00000002 },
    { D_02006028, 0x00000002 }, { D_02006000, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_02007358[11] = {
    { D_02005F38, 0x00000001 }, { D_02005F60, 0x00000001 }, { D_02005F88, 0x00000001 }, { D_02005FB0, 0x00000001 },
    { D_02005FD8, 0x00000001 }, { D_02005FB0, 0x00000001 }, { D_02005F88, 0x00000001 }, { D_02005F60, 0x00000001 },
    { D_02005F38, 0x00000001 }, { D_02005F10, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_020073B0[21] = {
    { D_02005F38, 0x00000001 }, { D_02005F60, 0x00000001 }, { D_02005F88, 0x00000001 }, { D_02005FB0, 0x00000001 },
    { D_02005FD8, 0x00000001 }, { D_02005FB0, 0x00000001 }, { D_02005F88, 0x00000001 }, { D_02005F60, 0x00000001 },
    { D_02005F38, 0x00000001 }, { D_02005F10, 0x00000001 }, { D_02005F38, 0x00000001 }, { D_02005F60, 0x00000001 },
    { D_02005F88, 0x00000001 }, { D_02005FB0, 0x00000001 }, { D_02005FD8, 0x00000001 }, { D_02005FB0, 0x00000001 },
    { D_02005F88, 0x00000001 }, { D_02005F60, 0x00000001 }, { D_02005F38, 0x00000001 }, { D_02005F10, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * DK face ?
 */
MkAnimation D_02007458[2] = {
    { D_02006190, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_02007468[2] = {
    { D_020061B8, 0x00000032 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_02007478[12] = {
    { D_020062A8, 0x00000001 }, { D_020062D0, 0x00000001 }, { D_020062F8, 0x00000001 }, { D_02006320, 0x00000001 },
    { D_02006348, 0x00000001 }, { D_02006370, 0x00000001 }, { D_02006398, 0x00000001 }, { D_020063C0, 0x00000001 },
    { D_020063E8, 0x00000001 }, { D_02006410, 0x00000005 }, { D_02006410, 0x00000001 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_020074D8[2] = {
    { D_02006410, 0x00000005 },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_020074E8[11] = {
    { D_02006410, 0x00000002 }, { D_020063E8, 0x00000002 }, { D_020063C0, 0x00000002 }, { D_02006398, 0x00000002 },
    { D_02006370, 0x00000002 }, { D_02006348, 0x00000002 }, { D_02006320, 0x00000002 }, { D_020062F8, 0x00000002 },
    { D_020062D0, 0x00000002 }, { D_020062A8, 0x00000002 }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_02007540[11] = {
    { D_020061E0, 0x00000001 }, { D_02006208, 0x00000001 }, { D_02006230, 0x00000001 }, { D_02006258, 0x00000001 },
    { D_02006280, 0x00000001 }, { D_02006258, 0x00000001 }, { D_02006230, 0x00000001 }, { D_02006208, 0x00000001 },
    { D_020061E0, 0x00000001 }, { D_020061B8, 0x0000000a }, { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_02007598[21] = {
    { D_020061E0, 0x00000001 }, { D_02006208, 0x00000001 }, { D_02006230, 0x00000001 }, { D_02006258, 0x00000001 },
    { D_02006280, 0x00000001 }, { D_02006258, 0x00000001 }, { D_02006230, 0x00000001 }, { D_02006208, 0x00000001 },
    { D_020061E0, 0x00000001 }, { D_020061B8, 0x00000001 }, { D_020061E0, 0x00000001 }, { D_02006208, 0x00000001 },
    { D_02006230, 0x00000001 }, { D_02006258, 0x00000001 }, { D_02006280, 0x00000001 }, { D_02006258, 0x00000001 },
    { D_02006230, 0x00000001 }, { D_02006208, 0x00000001 }, { D_020061E0, 0x00000001 }, { D_020061B8, 0x0000000a },
    { NULL, 0x00000000 },
};

/**
 * @brief unknown MkAnimation
 * Wario face ?
 */
MkAnimation D_02007640[2] = {
    { D_02006438, 0x00000032 },
    { NULL, 0x00000000 },
};
