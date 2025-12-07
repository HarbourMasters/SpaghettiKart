#ifndef CODE_80091750_H
#define CODE_80091750_H

#include <common_structs.h>
#include "textures.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern u32 _course_mario_raceway_dl_mio0SegmentRomStart[];

/* File specific types */

/*
Known `types` for `MenuItem`
0x53: "Mushroom Cup" box on the cup selection screen
0x54: "Flower Cup" box on the cup selection screen
0x55: "Star Cup" box on the cup selection screen
0x56: "Special Cup" box on the cup selection screen
0x5E: A box of static over the course images as the cup selection screen loads in.
      It is near unnoticeable though as in practice it doesn't last long enough to be seen.
      Try locking the word at `8018DC80` to see something like 0x20 just before confirming character selection to make
it last longer See `func_80096CD8` for the actual drawing of the static
*/

typedef struct {
    /* 0x00 */ s32 type;     // id maybe?
    /* 0x04 */ s32 state;    // sound mode, maybe some other stuff
    /* 0x08 */ s32 subState; // This is used but I can't tell what for
    /* 0x0C */ s32 column;
    /* 0x10 */ s32 row;
    /* 0x14 */ u8 priority; // priority/depth/z-level. Higher values are drawn on top of lower values
                            // If equal, later entries in gMenuItems are on top
    /* 0x15 */ u8 visible;  // active? If 1 its displayed, if 0 its not
    // These seem to be generic space available for use by the struct, no 1 purpose for any given member
    /* 0x16 */ s16 unk16;            // Potentially unused
    /* 0x18 */ s32 D_8018DEE0_index; // Index in D_8018DEE0, an array of some other struct type
    /* 0x1C */ s32 param1;           // Multi use. Sometimes cup selection, sometimes course index.
    /* 0x20 */ s32 param2; // Multi use, hard to tell what for though. Sometimes a random number, sometimes GP points
    /* 0x24 */ f32 paramf; // Multi use, x scaling for some things, rotation multiplier for the question box in some
                           // menus, probably some other things
} MenuItem;                // size = 0x28

typedef struct {
    /* 0x00 */ MkAnimation* textureSequence;
    /* 0x04 */ s32 sequenceIndex;    // Index in textureSequence that the animation is currently on
    /* 0x08 */ s32 frameCountDown;   // Frames left for the given animation part
    /* 0x0C */ u32 visible;          // visbile if 0x80000000, otherwise invisbile AND paused
    /* 0x10 */ s32 menuTextureIndex; // Don't know what sMenuTextureMap tracks
    /* 0x14 */ s32 unk14;            // Flip flops between 0 and 1, use unknown
} struct_8018DEE0_entry;             // size = 0x18

typedef struct {
    /* 0x0 */ MenuTexture* texture;
    /* 0x4 */ s32 texNum;
} struct_8018E060_entry; // size = 0x8

typedef struct {
    /* 0x0 */ MenuTexture* mk64Texture;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
} struct_8018E0E8_entry; // size = 0x8

typedef struct {
    /* 0x00 */ u64* textureData; // This should be interpreted as a segmented address
                                 /**
                                  * Its hard to tell what exactly what this is meant to be,
                                  * but it appears to be used as some sort of offset/index from the address stored in sMenuTextureList.
                                  * This value is (roughly) the sum of (width * height) of the
                                  * textures in all the previous entries in sMenuTextureMap
                                  */
    /* 0x04 */ s32 offset;
} TextureMap; // size = 0x08

typedef struct {
    /* 0x00 */ MenuTexture* textures;
    /* 0x04 */ Gfx* displayList;
} struct_8018E768_entry; // size = 0x08

struct UnkStruct_8018E7E8 {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 unk1;
    /* 0x6 */ s16 unk2;
}; // size = 0x8

typedef struct {
    /* 0x00 */ s16 column;
    /* 0x02 */ s16 row;
    /* 0x04 */ s16 pad0;
    /* 0x06 */ s16 pad1;
} Unk_D_800E70A0; // size = 0x08

enum MENU_ITEM_PRIORITY {
    MENU_ITEM_PRIORITY_0,
    MENU_ITEM_PRIORITY_1,
    MENU_ITEM_PRIORITY_2,
    MENU_ITEM_PRIORITY_3,
    MENU_ITEM_PRIORITY_4,
    MENU_ITEM_PRIORITY_5,
    MENU_ITEM_PRIORITY_6,
    MENU_ITEM_PRIORITY_7,
    MENU_ITEM_PRIORITY_8,
    MENU_ITEM_PRIORITY_9,
    MENU_ITEM_PRIORITY_A,
    MENU_ITEM_PRIORITY_B,
    MENU_ITEM_PRIORITY_C,
    MENU_ITEM_PRIORITY_D,
    MENU_ITEM_PRIORITY_E,
    MENU_ITEM_PRIORITY_F,
    MENU_ITEM_PRIORITY_MAX // 0x10
};

enum CenterText { LEFT_TEXT = 1, CENTER_TEXT_MODE_1, RIGHT_TEXT, CENTER_TEXT_MODE_2 };

enum MENU_ITEM_TYPE {
    MENU_ITEM_UI_NONE,
    MENU_ITEM_UI_START_BACKGROUND,
    MENU_ITEM_UI_LOGO_AND_COPYRIGHT,
    MENU_ITEM_UI_PUSH_START_BUTTON,
    MENU_ITEM_UI_NO_CONTROLLER,
    MENU_ITEM_UI_START_RECORD_TIME,
    MENU_ITEM_UI_GAME_SELECT = 0xA,
    MENU_ITEM_UI_1P_GAME,
    MENU_ITEM_UI_2P_GAME,
    MENU_ITEM_UI_3P_GAME,
    MENU_ITEM_UI_4P_GAME,
    MENU_ITEM_UI_OK,
    MAIN_MENU_OPTION_GFX,
    MAIN_MENU_DATA_GFX,
    MAIN_MENU_50CC,
    MAIN_MENU_100CC,
    MAIN_MENU_150CC,
    MAIN_MENU_EXTRA_CC,
    MENU_ITEM_TYPE_016,
    MENU_ITEM_TYPE_017,
    MAIN_MENU_TIME_TRIALS_BEGIN,
    MAIN_MENU_TIME_TRIALS_DATA,
    MENU_ITEM_TYPE_01B = 0x1B,
    MAIN_MENU_BACKGROUND = 0x23,
    CHARACTER_SELECT_BACKGROUND,
    COURSE_SELECT_BACKGROUND,
    CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER = 0x2A,
    CHARACTER_SELECT_MENU_MARIO,
    CHARACTER_SELECT_MENU_LUIGI,
    CHARACTER_SELECT_MENU_TOAD,
    CHARACTER_SELECT_MENU_PEACH,
    CHARACTER_SELECT_MENU_YOSHI,
    CHARACTER_SELECT_MENU_DK,
    CHARACTER_SELECT_MENU_WARIO,
    CHARACTER_SELECT_MENU_BOWSER,
    CHARACTER_SELECT_MENU_OK,
    CHARACTER_SELECT_MENU_1P_CURSOR,
    CHARACTER_SELECT_MENU_2P_CURSOR,
    CHARACTER_SELECT_MENU_3P_CURSOR,
    CHARACTER_SELECT_MENU_4P_CURSOR,
    MENU_ITEM_TYPE_043 = 0x43,
    MENU_ITEM_TYPE_044,
    MENU_ITEM_TYPE_045,
    MENU_ITEM_TYPE_046,
    MENU_ITEM_TYPE_047,
    MENU_ITEM_TYPE_048,
    MENU_ITEM_TYPE_049,
    MENU_ITEM_TYPE_050 = 0x50,
    COURSE_SELECT_MAP_SELECT = 0x52,
    COURSE_SELECT_MUSHROOM_CUP,
    COURSE_SELECT_FLOWER_CUP,
    COURSE_SELECT_STAR_CUP,
    COURSE_SELECT_SPECIAL_CUP,
    MENU_ITEM_TYPE_058 = 0x58,
    COURSE_SELECT_COURSE_NAMES,
    MENU_ITEM_TYPE_05A,
    MENU_ITEM_TYPE_05B,
    COURSE_SELECT_BATTLE_NAMES,
    COURSE_SELECT_OK,
    MENU_ITEM_TYPE_05E,
    MENU_ITEM_TYPE_05F,
    MENU_ITEM_TYPE_060,
    MENU_ITEM_TYPE_061,
    MENU_ITEM_TYPE_062,
    MENU_ITEM_TYPE_064 = 0x64,
    MENU_ITEM_TYPE_065,
    MENU_ITEM_TYPE_066,
    MENU_ITEM_TYPE_067,
    MENU_ITEM_TYPE_068,
    MENU_ITEM_TYPE_069,
    MENU_ITEM_TYPE_06E = 0x6E,
    MENU_ITEM_TYPE_078 = 0x78,
    MENU_ITEM_TYPE_079,
    MENU_ITEM_TYPE_07A,
    MENU_ITEM_TYPE_07B,
    MENU_ITEM_TYPE_07C, // Courses for data menu
    MENU_ITEM_TYPE_07D,
    MENU_ITEM_TYPE_07E,
    MENU_ITEM_TYPE_07F,
    MENU_ITEM_TYPE_080,
    MENU_ITEM_TYPE_081,
    MENU_ITEM_TYPE_082,
    MENU_ITEM_TYPE_083,
    MENU_ITEM_TYPE_084,
    MENU_ITEM_TYPE_085,
    MENU_ITEM_TYPE_086,
    MENU_ITEM_TYPE_087,
    MENU_ITEM_TYPE_088,
    MENU_ITEM_TYPE_089,
    MENU_ITEM_TYPE_08A,
    MENU_ITEM_TYPE_08B, // Courses for data menu end
    MENU_ITEM_TYPE_08C,
    MENU_ITEM_TYPE_08D,
    MENU_ITEM_TYPE_096 = 0x96,
    MENU_ITEM_TYPE_097,
    MENU_ITEM_TYPE_098,
    MENU_ITEM_TYPE_0A0 = 0xA0,
    MENU_ITEM_TYPE_0A1,
    MENU_ITEM_TYPE_0AA = 0xAA,
    MENU_ITEM_TYPE_0AB,
    MENU_ITEM_TYPE_0AC,
    MENU_ITEM_TYPE_0AF = 0XAF,
    MENU_ITEM_TYPE_0B0,
    MENU_ITEM_TYPE_0B1,
    MENU_ITEM_TYPE_0B2,
    MENU_ITEM_TYPE_0B3,
    MENU_ITEM_TYPE_0B4,
    MENU_ITEM_TYPE_0B9 = 0xB9,
    MENU_ITEM_TYPE_0BA,
    MENU_ITEM_TYPE_0BB,
    MENU_ITEM_ANNOUNCE_GHOST,
    MENU_ITEM_END_COURSE_OPTION,
    MENU_ITEM_TYPE_0BE,
    MENU_ITEM_PAUSE = 0xC7,
    MENU_ITEM_TYPE_0D2 = 0xD2,
    MENU_ITEM_TYPE_0D3,
    MENU_ITEM_TYPE_0D4,
    MENU_ITEM_TYPE_0D5,
    MENU_ITEM_TYPE_0D6,
    MENU_ITEM_TYPE_0D7,
    MENU_ITEM_TYPE_0D8,
    MENU_ITEM_TYPE_0D9,
    MENU_ITEM_TYPE_0DA,
    MENU_ITEM_DATA_COURSE_IMAGE = 0xE6,
    MENU_ITEM_DATA_COURSE_INFO,
    MENU_ITEM_DATA_COURSE_SELECTABLE,
    MENU_ITEM_TYPE_0E9,
    MENU_ITEM_TYPE_0EA,
    MENU_ITEM_TYPE_0F0 = 0xF0,
    MENU_ITEM_TYPE_0F1,
    MENU_ITEM_UI_LOGO_INTRO = 0xFA,
    START_MENU_FLAG,
    MENU_ITEM_TYPE_10E = 0x10E,
    MENU_ITEM_TYPE_12B = 0X12B,
    MENU_ITEM_TYPE_12C,
    MENU_ITEM_TYPE_12D,
    MENU_ITEM_TYPE_12E,
    MENU_ITEM_TYPE_12F,
    MENU_ITEM_TYPE_130,
    MENU_ITEM_TYPE_190 = 0x190,
    MENU_ITEM_TYPE_191,
    MENU_ITEM_TYPE_192,
    MENU_ITEM_TYPE_193,
    MENU_ITEM_TYPE_194,
    MENU_ITEM_TYPE_195,
    MENU_ITEM_TYPE_196,
    MENU_ITEM_TYPE_197,
    MENU_ITEM_TYPE_198,
    MENU_ITEM_TYPE_199,
    MENU_ITEM_TYPE_19A,
    MENU_ITEM_TYPE_19B,
    MENU_ITEM_TYPE_19C,
    MENU_ITEM_TYPE_19D,
    MENU_ITEM_TYPE_19E,
    MENU_ITEM_TYPE_19F,
    MENU_ITEM_TYPE_1A0,
    MENU_ITEM_TYPE_1A1,
    MENU_ITEM_TYPE_1A2,
    MENU_ITEM_TYPE_1A3,
    MENU_ITEM_TYPE_1A4,
    MENU_ITEM_TYPE_1A5,
    MENU_ITEM_TYPE_1A6,
    MENU_ITEM_TYPE_1A7,
    MENU_ITEM_TYPE_1A8,
    MENU_ITEM_TYPE_1A9,
    MENU_ITEM_TYPE_1AA,
    MENU_ITEM_TYPE_1AB,
    MENU_ITEM_TYPE_1AC,
    MENU_ITEM_TYPE_1AD,
    MENU_ITEM_TYPE_1AE,
    MENU_ITEM_TYPE_1AF,
    MENU_ITEM_TYPE_1B0,
    MENU_ITEM_TYPE_1B1,
    MENU_ITEM_TYPE_1B2,
    MENU_ITEM_TYPE_1B3,
    MENU_ITEM_TYPE_1B4,
    MENU_ITEM_TYPE_1B5,
    MENU_ITEM_TYPE_1B6,
    MENU_ITEM_TYPE_1B7,
    MENU_ITEM_TYPE_1B8,
    MENU_ITEM_TYPE_1B9,
    MENU_ITEM_TYPE_1BA,
    MENU_ITEM_TYPE_1BB,
    MENU_ITEM_TYPE_1BC,
    MENU_ITEM_TYPE_1BD,
    MENU_ITEM_TYPE_1BE,
    MENU_ITEM_TYPE_1BF,
    MENU_ITEM_TYPE_1C0,
    MENU_ITEM_TYPE_1C1,
    MENU_ITEM_TYPE_1C2,
    MENU_ITEM_TYPE_1C3,
    MENU_ITEM_TYPE_1C4,
    MENU_ITEM_TYPE_1C5,
    MENU_ITEM_TYPE_1C6,
    MENU_ITEM_TYPE_1C7,
    MENU_ITEM_TYPE_1C8,
    MENU_ITEM_TYPE_1C9,
    MENU_ITEM_TYPE_1CA,
    MENU_ITEM_TYPE_1CB,
    MENU_ITEM_TYPE_1CC,
    MENU_ITEM_TYPE_1CD,
    MENU_ITEM_TYPE_1CE,
    MENU_ITEM_UI_HARBOUR_MASTERS
};

enum LoadImgCompType {
    LOAD_MENU_IMG_MIO0_ONCE = -1,
    LOAD_MENU_IMG_TKMK00_ONCE,
    // Force load image even if the data is loaded before
    LOAD_MENU_IMG_FORCE = LOAD_MENU_IMG_TKMK00_ONCE, // 0
    LOAD_MENU_IMG_MIO0_FORCE,
    LOAD_MENU_IMG_TKMK00_FORCE
};

enum TEXT_MENU_ID { CONTINUE_GAME, RETRY, COURSE_CHANGE, DRIVER_CHANGE, QUIT_TEXT_MENU, REPLAY, SAVE_GHOST };

/* Function Prototypes */

f64 exponent_by_squaring(f64, s32);
f64 func_800917B0(f64, f64);
f64 func_8009186C(f64);
f64 func_8009195C(f64);
f64 func_80091A6C(f64, s32);
f64 func_80091AC0(f64, s32*);
void swap_values(s32*, s32*);
s32 func_80091D74(void);
void func_80091EE4(void);
void func_80091FA4(void);
void func_80092148(void);
void func_800921B4(void);
void text_rainbow_effect(s32, s32, s32);
void set_text_color_rainbow_if_selected(s32, s32, s32);
void func_80092258(void);
void func_80092290(s32, s32*, s32*);
void func_80092500(void);
void func_80092564(void);
void func_800925A0(void);
void func_800925CC(void);
void func_80092604(void);
void func_80092630(void);
void func_8009265C(void);
void func_80092688(void);
void func_80092C80(void);
s32 char_to_glyph_index(char*);
s32 func_80092DF8(char*);
s32 func_80092E1C(char*);
s32 func_80092EE4(char*);
s32 get_string_width(char*);
void set_text_color(s32);
void func_800930E4(s32, s32, char*);
void print_text0(s32, s32, char*, s32, f32, f32, s32);
void print_text0_wide_right(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode);
void print_text_mode_1(s32, s32, char*, s32, f32, f32);
void print_text_mode_2(s32, s32, char*, s32, f32, f32);
void print_text_mode_2_wide_right(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY);
void print_text1(s32, s32, char*, s32, f32, f32, s32);
void print_text1_left(s32, s32, char*, s32, f32, f32);
void print_text1_center_mode_1(s32, s32, char*, s32, f32, f32);
void print_text1_right(s32, s32, char*, s32, f32, f32);
void print_text1_center_mode_2(s32, s32, char*, s32, f32, f32);
void print_text2(s32, s32, char*, s32, f32, f32, s32);
void print_text2_wide(s32, s32, char*, s32, f32, f32, s32);
void func_800939C8(s32, s32, char*, s32, f32, f32);
void text_draw(s32, s32, char*, s32, f32, f32);
void text_draw_wide(s32, s32, char*, s32, f32, f32);
void func_80093A30(s32);
void func_80093A5C(u32);
void func_80093B70(u32);
void func_80093C1C(s32);
void func_80093C88(void);
void func_80093C90(void);
void func_80093C98(s32);
void func_80093E20(void);
void func_80093E40(void);
void func_80093E60(void);
void func_80093F10(void);
void func_800940EC(s32);
void func_800942D0(void);
void func_80094660(struct GfxPool*, s32);
void render_checkered_flag(struct GfxPool*, s32);
void func_80094A64(struct GfxPool*);
void setup_menus(void);
void func_80095574(void);
Gfx* draw_flash_select_case(Gfx*, s32, s32, s32, s32, s32);
Gfx* draw_flash_select_case_slow(Gfx*, s32, s32, s32, s32);
Gfx* draw_flash_select_case_fast(Gfx*, s32, s32, s32, s32);
Gfx* func_800959F8(Gfx*, Vtx*);
Gfx* func_80095BD0(Gfx*, u8*, f32, f32, u32, u32, f32, f32);
Gfx* func_80095E10(Gfx*, s8, s32, s32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* func_800963F0(Gfx*, s8, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* func_80096CD8(Gfx*, s32, s32, u32, u32);
Gfx* func_80097274(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u16* argA, u32 argB, u32 argC, s32 argD);
Gfx* func_80097A14(Gfx*, s8, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* func_80097AE4(Gfx*, s8, s32, s32, u8*, s32);
Gfx* func_80097E58(Gfx* displayListHead, s8 fmt, u32 arg2, u32 arg3, u32 arg4, u32 arg5, s32 arg6, s32 arg7,
                   u8* someTexture, u32 arg9, u32 argA, s32 width);
Gfx* func_80098558(Gfx*, u32, u32, u32, u32, u32, u32, s32, s32);
Gfx* func_800987D0(Gfx*, u32, u32, u32, u32, s32, s32, u8*, u32, s32);
Gfx* draw_box_fill(Gfx*, s32, s32, s32, s32, s32, s32, s32, s32);
Gfx* draw_box(Gfx*, s32, s32, s32, s32, u32, u32, u32, u32);
Gfx* draw_box_wide(Gfx*, s32, s32, s32, s32, u32, u32, u32, u32);
Gfx* func_80098FC8(Gfx*, s32, s32, s32, s32);
void clear_menu_textures(void);
void load_menu_img(MenuTexture*);
void load_menu_img_mio0_forced(MenuTexture*);
void load_menu_img_comp_type(MenuTexture*, s32);
void func_80099958(MenuTexture*, s32, s32);
void func_80099E54(void);
void func_80099E60(MenuTexture*, s32, s32);
void func_80099EC4(void);
void func_80099A70(void);
void func_80099A94(MenuTexture*, s32);
void func_80099AEC(void);
void func_8009A238(MenuTexture*, s32);
void func_8009A2F0(struct_8018E0E8_entry*);
void func_8009A344(void);
s32 animate_character_select_menu(MkAnimation*);
s32 func_8009A478(MkAnimation*, s32);
void func_8009A594(s32, s32, MkAnimation*);
void func_8009A640(s32, s32, s32, MkAnimation*);
void func_8009A6D4(void);
void func_8009A76C(s32, s32, s32, s32);
void func_8009A7EC(s32, s32, s32, s32, s32);
MenuTexture* func_8009A878(struct_8018DEE0_entry*);
MenuTexture* func_8009A944(struct_8018DEE0_entry*, s32);
void func_8009A9FC(s32, s32, u32, s32);
void func_8009AB7C(s32);
void func_8009AD78(s32, s32);
u16* func_8009B8C4(u64*);
void func_8009B938(void);
void func_8009B954(MenuTexture*);
void func_8009B998(void);
Gfx* func_8009B9D0(Gfx*, MenuTexture*);
Gfx* render_menu_textures(Gfx*, MenuTexture*, s32, s32);
Gfx* func_8009BC9C(Gfx*, MenuTexture*, s32, s32, s32, s32);
Gfx* print_letter(Gfx*, MenuTexture*, f32, f32, s32, f32, f32);
Gfx* print_letter_wide_right(Gfx* arg0, MenuTexture* glyphTexture, f32 arg2, f32 arg3, s32 mode, f32 scaleX,
                             f32 scaleY);
Gfx* func_8009C204(Gfx*, MenuTexture*, s32, s32, s32);
Gfx* func_8009C434(Gfx*, struct_8018DEE0_entry*, s32, s32, s32);
Gfx* func_8009C434_alt(Gfx*, struct_8018DEE0_entry*, s32, s32);
Gfx* func_8009C708(Gfx*, struct_8018DEE0_entry*, s32, s32, s32, s32);
void func_8009C918(void);
void func_8009CA2C(void);
void func_8009CA6C(s32);
void draw_fade_in(s32, s32, s32);
void draw_black_fade_in(s32, s32);
void func_8009CDFC(s32, s32);
void func_8009CE1C(void);
void func_8009CE64(s32);
void func_8009D77C(s32, s32, s32);
void func_8009D958(s32, s32);
void func_8009D978(s32, s32);
void func_8009D998(s32);
void func_8009DAA8(void);
void func_8009DB8C(void);
void func_8009DEF8(u32, u32);
void func_8009DF4C(s32);
void func_8009DF6C(s32);
void func_8009DF8C(u32, u32);
void func_8009DFE0(s32);
void func_8009E000(s32);
void func_8009E020(s32, s32);
void func_8009E088(s32, s32);
void func_8009E0F0(s32);
void func_8009E1C0(void);
void func_8009E1E4(void);
void func_8009E208(void);
void func_8009E230(void);
void func_8009E258(void);
void func_8009E280(void);
void func_8009E2A8(s32);
void func_8009E2F0(s32);
void func_8009E5BC(void);
void func_8009E5FC(s32);
void clear_menus(void);
void add_menu_item(s32, s32, s32, s8);
void render_menus(MenuItem*);
void func_800A08D8(u8, s32, s32);
s32 func_800A095C(char*, s32, s32, s32);
void func_800A09E0(MenuItem*);
void func_800A0AD0(MenuItem*);
void func_800A0B80(MenuItem*);
void func_800A0DFC(void);
void func_800A0EB8(MenuItem*, s32);
void func_800A0FA4(MenuItem*, s32);
void func_800A10CC(MenuItem*);
void render_cursor_player(MenuItem*, s32, s32);
void func_800A12BC(MenuItem*, MenuTexture*);
void func_800A1350(MenuItem*);
void func_800A143C(MenuItem*, s32);
void func_800A1500(MenuItem*);
void func_800A15EC(MenuItem*);
void func_800A1780(MenuItem*);
void render_menu_item_data_course_image(MenuItem*);
void render_menu_item_data_course_info(MenuItem*);
void menu_item_data_course_selectable(MenuItem*);
void func_800A1DE0(MenuItem*);
void func_800A1F30(MenuItem*);
void func_800A1FB0(MenuItem*);
void func_800A2D1C(MenuItem*);
void func_800A2EB8(MenuItem*);
void func_800A32B4(s32, s32, s32, s32);
void func_800A34A8(MenuItem*);
void func_800A3A10(s8*);
void func_800A3ADC(MenuItem*, s32, s32, s32, s32, s8*);
void time_trials_finish_text_render(MenuItem*);
void func_800A3E60(MenuItem*);
void render_lap_time(s32, s32, s32);
void render_lap_times(s32, s32, s32);
void render_menu_item_announce_ghost(MenuItem*);
void render_pause_menu(MenuItem*);
void render_pause_menu_time_trials(MenuItem*);
void render_pause_menu_versus(MenuItem*);
void render_pause_grand_prix(MenuItem*);
void render_pause_battle(MenuItem*);
void func_800A54EC(void);
void render_menu_item_end_course_option(MenuItem*);
void func_800A6034(MenuItem*);
void func_800A6154(MenuItem*);
void func_800A638C(MenuItem*);
void pause_menu_item_box_cursor(MenuItem*, Unk_D_800E70A0*);
void func_800A69C8(MenuItem*);
void func_800A6BEC(MenuItem*);
void func_800A6CC0(MenuItem*);
void func_800A6D94(s32, s32, u8*);
void func_800A6E94(s32, s32, u8*);
void func_800A70E8(MenuItem*);
void func_800A7258(MenuItem*);
void func_800A72FC(MenuItem*);
void func_800A7448(MenuItem*);
void func_800A75A0(MenuItem*);
void func_800A761C(MenuItem*);
void menu_item_credits_render(MenuItem*);
void convert_number_to_ascii(s32, char*);
void write_dashes(char*);
void get_time_record_minutes(s32, char*);
void get_time_record_seconds(s32, char*);
void get_time_record_centiseconds(s32, char*);
void func_800A79F4(s32, char*);
void handle_menus_with_pri_arg(s32);
void handle_menus_default(void);
void handle_menus_special(void);
void func_800A8270(s32, MenuItem*);
void func_800A8564(MenuItem*);
void func_800A86E8(MenuItem*);
void func_800A874C(MenuItem*);
void func_800A890C(s32, MenuItem*);
void func_800A8A98(MenuItem*);
void func_800A8CA4(MenuItem*);
void render_battle_introduction(MenuItem*);
void func_800A8EC0(MenuItem*);
void func_800A8F48(MenuItem*);
void func_800A90D4(s32, MenuItem*);
void func_800A91D8(MenuItem*, s32, s32);
void func_800A9208(MenuItem*, s32);
void func_800A9278(MenuItem*, s32);
void func_800A92E8(MenuItem*, s32);
void func_800A939C(MenuItem*, s32);
void func_800A940C(MenuItem*, s32);
void func_800A94C8(MenuItem*, s32, s32);
void func_800A954C(MenuItem*);
void func_800A9710(MenuItem*);
void func_800A97BC(MenuItem*);
void update_ok_menu_item(MenuItem*);
void func_800A9B9C(MenuItem*);
void func_800A9C40(MenuItem*);
void func_800A9D5C(MenuItem*);
void func_800A9E58(MenuItem*);
void func_800AA280(MenuItem*);
void func_800AA2EC(MenuItem*);
void func_800AA5C8(MenuItem*, s8);
void func_800AA69C(MenuItem*);
void func_800AAA9C(MenuItem*);
void func_800AAB90(MenuItem*);
void func_800AAC18(MenuItem*);
void update_cursor(MenuItem*);
void func_800AAE18(MenuItem*);
MenuItem* get_menu_item_player_count(void);
MenuItem* get_menu_item_character(s32);
MenuItem* find_menu_items_dupe(s32);
MenuItem* find_menu_items(s32);
s32 get_character_menu_state(s32);
void hover_cursor_over_character_portrait(MenuItem*, s32);
s32 func_800AAFCC(s32);
void func_800AB020(MenuItem*);
void func_800AB098(MenuItem*);
void func_800AB164(MenuItem*);
void func_800AB260(MenuItem*);
void func_800AB290(MenuItem*);
void func_800AB314(MenuItem*);
void func_800AB904(MenuItem*);
void func_800AB9B0(MenuItem*);
void func_800ABAE8(MenuItem*);
void func_800ABB24(MenuItem*);
void func_800ABBCC(MenuItem*);
void func_800ABC38(MenuItem*);
void func_800ABCF4(MenuItem*);
void func_800ABEAC(MenuItem*);
void func_800ABF68(MenuItem*);
void func_800AC128(MenuItem*);
void func_800AC300(MenuItem*);
void func_800AC324(MenuItem*);
void func_800AC458(MenuItem*);
void func_800AC978(MenuItem*);
void func_800ACA14(MenuItem*);
void func_800ACC50(MenuItem*);
void func_800ACF40(MenuItem*);
void func_800AD1A4(MenuItem*);
void func_800AD2E8(MenuItem*);
void func_800ADF48(MenuItem*);
void func_800AE218(MenuItem*);
void func_800AEC54(MenuItem*);
void func_800AEDBC(MenuItem*);
void func_800AEE90(MenuItem*);
void func_800AEEBC(MenuItem*);
void func_800AEEE8(MenuItem*);
void func_800AEF14(MenuItem*);
void func_800AEF74(MenuItem*);
void func_800AF004(MenuItem*);
void func_800AF1AC(MenuItem*);
void func_800AF270(MenuItem*);
void func_800AF480(MenuItem*);
void func_800AF4DC(MenuItem*);
void func_800AF740(MenuItem*);

// This really, really shouldn't be in this header file, but I don't know where else to put it
void rmonPrintf(const char*, ...);

/* File specific defines */

#define MENU_ITEMS_MAX 0x20
#define D_8018DEE0_SIZE 0x10
#define D_8018E060_SIZE 0x10
#define D_8018E0E8_SIZE 0x05
#define TEXTURE_MAP_MAX 0xC8
#define D_8018E768_SIZE 0x08
#define D_8018E7E8_SIZE 0x05
#define D_8018E810_SIZE 0x05
#define MENU_TEXTURE_BUFFER_MAX 500

/* This is where I'd put my static data, if I had any */

extern s32 D_800DDB24;
extern s16 gGetPlayerByCharacterId[];

/**
 * Old name: sMenuTextureBuffer
 * This array has been modified. Instead of holding a buffer of texture data
 * It now holds a list of pointers to asset strings.
 * use `load_texture()` and `replace_texture()` to add textures to the list for rendering.
 */
extern const char* sMenuTextureList[MENU_TEXTURE_BUFFER_MAX];
extern u8* sTKMK00_LowResBuffer;
extern u8* sGPPointsCopy;
extern void* gSomeDLBuffer;
extern s8 gGPPointsByCharacterId[8];
extern s8 gCharacterIdByGPOverallRank[];
extern s8 D_8018D9D8;
extern s8 D_8018D9D9;
extern MenuItem gMenuItems[MENU_ITEMS_MAX];
extern struct_8018DEE0_entry D_8018DEE0[D_8018DEE0_SIZE];
extern struct_8018E060_entry D_8018E060[];
extern struct_8018E0E8_entry D_8018E0E8[D_8018E0E8_SIZE];
extern s32 sMenuTextureListIndex; // Old name: sMenuTextureBufferIndex, index into sMenuTextureList
extern TextureMap sMenuTextureMap[TEXTURE_MAP_MAX];
extern s32 sMenuTextureEntries;
extern Gfx* sGfxPtr;
extern s32 gNumD_8018E768Entries;
extern struct_8018E768_entry D_8018E768[D_8018E768_SIZE];
extern s32 gCycleFlashMenu;
extern s8 gTransitionType[];
extern u32 gTransitionDuration[];
extern u32 gCurrentTransitionTime[];
extern struct UnkStruct_8018E7E8 D_8018E7E8[D_8018E7E8_SIZE];
extern struct UnkStruct_8018E7E8 D_8018E810[D_8018E810_SIZE];
extern s8 gTextColor;
extern u8 D_8018ED90;
extern u8 D_8018ED91;
extern s8 D_8018E838[];
extern s32 D_8018E840[];
extern s32 D_8018E850[];
extern s32 D_8018E854;
extern s32 D_8018E858[];
extern s32 D_8018E85C;

// extern u8 _textures_0aSegmentRomStart[];
// extern u8 _textures_0bSegmentRomStart[];

extern Unk_D_800E70A0 D_800E70A0[];
extern Unk_D_800E70A0 D_800E70E8[];
extern Unk_D_800E70A0 D_800E7108[][4];
extern Unk_D_800E70A0 D_800E7148[];
extern Unk_D_800E70A0 D_800E7168[];
extern Unk_D_800E70A0 D_800E7188[];
extern Unk_D_800E70A0 D_800E7208[][2];
extern Unk_D_800E70A0 D_800E7248[];
extern Unk_D_800E70A0 D_800E7258[];
extern Unk_D_800E70A0 D_800E7268[];
extern Unk_D_800E70A0 D_800E7278[];
extern Unk_D_800E70A0 D_800E72F8;
extern Unk_D_800E70A0 D_800E7300[];
extern Unk_D_800E70A0 D_800E7360[];
extern Unk_D_800E70A0 D_800E7380[];
extern Unk_D_800E70A0 D_800E7390[];
extern Unk_D_800E70A0 D_800E73C0[];
extern Unk_D_800E70A0 D_800E73D0[];
extern Unk_D_800E70A0 D_800E73E0[];
extern Unk_D_800E70A0 D_800E7410[];
extern Unk_D_800E70A0 D_800E7420[];
extern Unk_D_800E70A0 D_800E7430[];
extern Unk_D_800E70A0 D_800E7458[];
extern Unk_D_800E70A0 D_800E7480[];
extern RGBA16 D_800E74A8[];
extern RGBA16 D_800E74D0[];
extern RGBA16 gBackgroundColor[];
extern const s16 gGlyphDisplayWidth[];
extern char* gCupNames[];
extern const s8 D_800EFD64[];
extern char* gCupText[];
extern char* gDebugCharacterNames[];
extern char* D_800E76A8[];
extern char* D_800E76CC[];
extern char* D_800E76DC[];
extern char* gDebugScreenModeNames[];
extern char* gDebugSoundModeNames[];
extern char* gSoundModeNames[]; // D_800E7710
extern char* gWinLoseText[];
extern char* gBestTimeText[];
extern char* gLapTimeText;
extern char* gPrefixTimeText[];
extern char* D_800E7744[];
extern char* gTextPauseButton[];
extern char* D_800E7778[];
extern char gTextMenuAnnounceGhost[];
extern char* gTextNoController[];
extern char* gTextBattleIntroduction[];
extern char gTextMenuData[];
extern char gTextDistance[];
extern char* gTextMenuOption[];
extern char* D_800E7840[];
extern char* gEraseBestGhostText[];
extern char* D_800E7860[];
extern char* gTextOptionMenu[];
extern char* D_800E7878[];
extern char* D_800E7884[];
extern char* D_800E7890[];
extern char* D_800E78D0[];
extern char* D_800E7900[];
extern char* D_800E7918[];
extern char* D_800E7920[];
extern char* D_800E7928[];
extern char* D_800E7930[];
extern char* D_800E7938[];
extern char* D_800E7940[];
extern char* D_800E7980[];
extern char* D_800E798C[];
extern char* D_800E7A34[];
extern char* D_800E7A3C[];
extern char* D_800E7A44;
extern char* D_800E7A48[];
extern char* D_800E7A54[];
extern char* D_800E7A60[];
extern char* D_800E7A6C[];
extern char* D_800E7A74[];
extern char* D_800E7A80[];
extern char* D_800E7A88[];
extern char* D_800E7A98;
extern char* D_800E7A9C[];
extern char* gPlaceText[];
extern const s8 gGPPointRewards[];
extern const s8 D_800F0B1C[];
extern const s8 D_800F0B28[];
extern const s8 D_800F0B50[];
extern const s8 D_800F0B54[];
extern RGBA16 D_800E7AC8[];
extern RGBA16 D_800E7AE8[];
extern MenuTexture* D_800E7AF8[];
extern MenuTexture* D_800E7D0C[];
extern MkAnimation* D_800E7D34[];
extern MenuTexture* gMenuTexturesBackground[];
extern MenuTexture* D_800E7D54[];
extern MenuTexture* D_800E7D74[];
extern MenuTexture* D_800E7DC4[];
extern MkAnimation* D_800E7E14[];
extern MkAnimation* D_800E7E20[];
extern MkAnimation* D_800E7E34[];
extern MenuTexture* gGlyphTextureLUT[];
extern MenuTexture* D_800E7FF0[];
extern MenuTexture* D_800E80A0[];
extern MenuTexture* D_800E8114[];
extern MenuTexture* D_800E8174[];
extern MenuTexture* D_800E817C[];
extern MenuTexture* D_800E81E4[];
extern MenuTexture* D_800E822C[];
extern MenuTexture* D_800E8234[];
extern MenuTexture* D_800E8254[];
extern MenuTexture* D_800E8274[];
extern MenuTexture* D_800E8294[];
extern MenuTexture* gMenuTexturesBorderPlayer[];
extern MenuTexture* gMenuTexturesTrackSelection[];
extern MenuTexture* D_800E82F4[];
extern MkAnimation* D_800E8320[];
extern MkAnimation* D_800E8340[];
extern MkAnimation* D_800E8360[];
extern MkAnimation* gCharacterCelebrateAnimation[];
extern MkAnimation* D_800E83A0[];
extern MkAnimation* gCharacterDeselectAnimation[];
extern MkAnimation* gCharacterSingleBlinkAnimation[];
extern MkAnimation* gCharacterDoubleBlinkAnimation[];
extern MkAnimation* gCharacterDefeatAnimation[];
extern s32 D_800E8440[];
extern s32 D_800E8460[];
extern s32 D_800E8480[];
extern s32 D_800E84A0[];
extern Vtx* D_800E84C0[];
extern Gfx* D_800E84CC[];
extern Gfx* D_800E84EC[];
extern Gfx* D_800E850C[];
extern s8 D_800E852C;
extern f32 sIntroModelMotionSpeed;
extern f32 sIntroModelSpeed;
extern Unk_D_800E70A0 D_800E8538[];
extern Unk_D_800E70A0 D_800E8540[];
extern Unk_D_800E70A0 D_800E85C0[];
extern Unk_D_800E70A0 D_800E8600[];

extern s32 gControllerPak1NumPagesFree;
extern s32 gControllerPak1FileNote;
extern s32 gControllerPak2FileNote;

extern f32 D_8018ED98;
extern f32 D_8018ED9C;
extern f32 D_8018EDA0;

extern f32 D_8018EDA4;
extern f32 D_8018EDA8;
extern f32 D_8018EDAC;

#ifdef __cplusplus
}
#endif

#endif
