#ifndef MENUS_H
#define MENUS_H

#include <common_structs.h>
#include <defines.h>

// D_8018EDEC is position on options screen?
enum MenuOptionsCursorPositions { MENU_OPTIONS_CSRPOS_SOUNDMODE = 0x16 };

// needs to be a union (or array...?) to go into rodata as a const
// terrible for endianness... Best guess as to what this is for..
union GameModePack {
    u8 modes[4];
    s32 word;
};

// This really, really shouldn't be in this header file, but I don't know where else to put it
void rmonPrintf(const char*, ...);

/* functions */
void update_menus(void);
void options_menu_act(struct Controller*, u16);
void data_menu_act(struct Controller*, u16);
void course_data_menu_act(struct Controller*, u16);
void logo_intro_menu_act(struct Controller*, u16);
void controller_pak_menu_act(struct Controller*, u16);
void splash_menu_act(struct Controller*, u16);
void func_800B28C8(void);
void main_menu_act(struct Controller*, u16);
bool is_character_spot_free(s32);
void player_select_menu_act(struct Controller*, u16);
void course_select_menu_act(struct Controller*, u16);
void func_800B3F74(s32);
void func_800B44AC(void);
void func_800B44BC(void);
bool func_800B4520(void);

/* data */
extern s32 D_8018EDC0;
extern f32 D_8018EDC4;
extern f32 D_8018EDC8;
extern f32 D_8018EDCC;
extern f32 D_8018EDD0;
extern f32 D_8018EDD4;
extern f32 D_8018EDD8;
extern f32 D_8018EDDC;
extern s32 D_8018EDE0;

extern s8 gCharacterGridSelections[];
extern s8 D_8018EDE8[];
extern s8 D_8018EDEC;
extern s8 gMainMenuSelectionDepth;
extern s8 D_8018EDEE;
extern s8 gDebugMenuSelection;
extern s8 gControllerPakMenuSelection;
extern s8 D_8018EDF1;
extern u8 gSoundMode;
extern s8 gPlayerCount;
extern s8 D_8018EDF4;
extern s8 D_8018EDF5;
extern s8 D_8018EDF6;
extern s8 gTimeTrialDataCourseIndex;
extern s8 gCourseRecordsMenuSelection;
extern s8 D_8018EDF9;
extern s8 gDebugGotoScene;
extern s8 D_8018EDFB;
extern s8 D_8018EDFC;
extern s32 gMenuTimingCounter;
extern s32 gMenuDelayTimer;
extern s8 D_8018EE08;
extern s8 gCupSelection;
extern s8 D_8018EE0A;
extern s8 gCupCourseSelection;
extern s8 D_8018EE0C;
extern struct_8018EE10_entry D_8018EE10[];

extern s32 gMenuSelection; // D_800E86A0
extern s32 D_800E86A4;
extern s8 gCharacterSelections[];

extern s8 D_800E86AC[];
extern s8 D_800E86B0[4][3];
extern s8 gNextDemoId;
extern s8 gControllerPakSelectedTableRow;
extern s8 D_800E86C4[];
extern s8 D_800E86D0[];
extern s8 unref_800E86E0[];
extern u32 sVIGammaOffDitherOn;

extern const s8 D_800F2B50[5];
extern const s8 D_800F2B58[5];
extern const s8 D_800F2B60[5][3];
extern const s32 gGameModeFromNumPlayersAndRowSelection[5][3];
extern const s8 D_800F2BAC[8];
extern const s16 gCupCourseOrder[NUM_CUPS][NUM_COURSES_PER_CUP];

extern const s8 D_800F2BDC[8];

extern const union GameModePack D_800F2BE4;

// end of menus.c variables

#endif /* MENUS_H */
