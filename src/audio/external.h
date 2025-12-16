#ifndef AUDIO_EXTERNAL_H
#define AUDIO_EXTERNAL_H

#include <libultraship.h>
#include <common_structs.h>
#include "camera.h"

#define ADSR_STATE_DISABLED 0
#define ADSR_STATE_INITIAL 1
#define ADSR_STATE_START_LOOP 2
#define ADSR_STATE_LOOP 3
#define ADSR_STATE_FADE 4
#define ADSR_STATE_HANG 5
#define ADSR_STATE_DECAY 6
#define ADSR_STATE_RELEASE 7
#define ADSR_STATE_SUSTAIN 8

#define ADSR_ACTION_RELEASE 0x10
#define ADSR_ACTION_DECAY 0x20
#define ADSR_ACTION_HANG 0x40

#define ADSR_DISABLE 0
#define ADSR_HANG -1
#define ADSR_GOTO -2
#define ADSR_RESTART -3

// Sequence arguments, passed to seq_player_play_sequence. seqId may be bit-OR'ed with
// SEQ_VARIATION; this will load the same sequence, but set a variation
// bit which may be read by the sequence script.
#define SEQUENCE_ARGS(priority, seqId) ((priority << 8) | seqId)

#define SOUND_MODE_STEREO 0
#define SOUND_MODE_MONO 3
#define SOUND_MODE_HEADSET 1

#define SEQ_PLAYER_LEVEL 0 // Level background music
#define SEQ_PLAYER_ENV 1   // Misc music like the puzzle jingle
#define SEQ_PLAYER_SFX 2   // Sound effects

#define SOUND_BANK_UNK0 0
#define SOUND_BANK_UNK1 1
#define SOUND_BANK_UNK2 2
#define SOUND_BANK_UNK3 3
#define SOUND_BANK_UNK4 4
#define SOUND_BANK_UNK5 5
#define SOUND_BANK_COUNT 6

// Almost certainly an expanded version of struct Sound from SM64
struct Sound {
    /* 0x00 */ u32 soundBits;
    /* 0x04 */ Vec3f* position;
    /* 0x08 */ u8 cameraId; // playerId? some indicator of "who" the sound is desitned for
    /* 0x09 */              // u8  compilerPadding0[3];
    /* 0x0C */ f32* unk0C;
    /* 0x10 */ f32* unk10;
    /* 0x14 */ s8* unk14;
}; // size = 0x18

struct Unk_800E9F7C {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ s8 unk_14;
    /* 0x15 */ // s8 comilerPadding[3];
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ f32 unk_30;
    /* 0x34 */ f32 unk_34;
    /* 0x38 */ f32 unk_38;
}; // size = 0x3C

// I'm speculating that this struct has something to do with
// the doppler effect that happens to some sounds
struct Unk_8018EFD8 {
    // Pointers to a position
    /* 0x00 */ f32* posX;
    /* 0x04 */ f32* posY;
    /* 0x08 */ f32* posZ;
    // Pointers to a velocity
    /* 0x0C */ f32* velX;
    /* 0x10 */ f32* velY;
    /* 0x14 */ f32* velZ;
    /* 0x18 */ Vec3f unk18;
    /* 0x24 */ f32* unk24;  // Some type of multiplier/scalar
                            // Indexes in D_8018EFD8, not really sure what they're for though
    /* 0x28 */ u8 cameraId; // playerId? some indicator of "who" the sound is desitned for
    /* 0x29 */ u8 prev;
    /* 0x2A */ u8 next;
    /* 0x2B */ // u8 compilerPadding00;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ // u8 compilerPadding01[3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u32 soundBits;
}; // size = 0x3C

// This is almost certainly an expanded version of SoundCharacteristics from SM64
struct SoundCharacteristics {
    // These f32 pointers appear to point to the `Vec3f unk18` members in a struct Unk_8018EFD8
    /* 0x00 */ Vec3f* unk00;
    /* 0x04 */ f32* unk04;
    /* 0x08 */ f32* unk08;
    /* 0x0C */ u8 cameraId; // playerId? some indicator of "who" the sound is desitned for
    /* 0x0D */              // u8 compilerPadding0[3];
    /* 0x10 */ f32* unk10;
    /* 0x14 */ f32* unk14;
    /* 0x18 */ s8* unk18;
    /* 0x1C */ f32 distance; // XZ distance from the camera
    /* 0x20 */ u32 priority;
    /* 0x24 */ u32 soundBits;
    /* 0x28 */ u8 soundStatus;
    /* 0x29 */ u8 freshness;
    /* 0x2A */ u8 prev;
    /* 0x2B */ u8 next;
    /* 0x2C */ u8 unk2C;
}; // size = 0x30

struct Unk_800EA06C {
    /* 0x00 */ Vec3f unk00;
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ // u8 compilerPadding0[3];
}; // size = 0x10

typedef struct {
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ f32 unk_04;
    /* 0x06 */ // u16 compilerPadding;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ // u16 compilerPadding;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14;
    /* 0x16 */ // u16 compilerPadding;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ u16 unk_1C;
    /* 0x1E */             // u16 compilerPadding;
} struct_D_801930D0_inner; // size = 0x20

typedef struct {
    // This will need lots of expanding/documenting
    // Its 147 words of space being used
    /* 0x000 */ f32 unk_000;
    /* 0x004 */ f32 unk_004;
    /* 0x008 */ f32 unk_008;
    /* 0x00C */ u16 unk_00C;
    /* 0x00E */ u8 unk_00E[3]; // Array size guessed
    /* 0x011 */ u8 unk_011;
    /* 0x012 */ u8 unk_012;
    /* 0x013 */ u8 unk_013;
    /* 0x014 */ u32 unk_014;
    /* 0x018 */ u16 unk_018;
    /* 0x01A */ u16 unk_01A;
    /* 0x01C */ f32 unk_01C;
    /* 0x020 */ f32 unk_020;
    /* 0x024 */ f32 unk_024;
    /* 0x028 */ u16 unk_028;
    /* 0x02A */ u16 unk_02A;
    /* 0x02C */ u32 unk_02C[5]; // Array size guessed
    /* 0x040 */ u8 unk_040;
    /* 0x041 */ u8 unk_041;
    /* 0x042 */ u8 unk_042;
    /* 0x043 */ u8 unk_043;
    /* 0x044 */ struct_D_801930D0_inner unk_044[16];
    /* 0x244 */ u16 unk_244;
    /* 0x246 */ u16 unk_246;
    /* 0x248 */ u16 unk_248;
    /* 0x24A */ u16 unk_24A;
} struct_D_801930D0_entry; // size = 0x24C

// Not quite the same as the struct found in SM64,
// not even sure that the struct or its members are even
// correct in the MK64 context
struct ChannelVolumeScaleFade {
    /* 0x000 */ f32 current;
    /* 0x004 */ f32 target;
    /* 0x008 */ f32 velocity;
    /* 0x00C */ u16 remainingFrames;
    /* 0x00A */ // u16 compilerPadding;
}; // size = 0x10

typedef struct {
    f32 unk0;
    f32 unk4;
    u8 unk8;
    u8 unk9;
} UnkStruct8018EF18; // size = 0xC

typedef struct {
    u8 thing0;
    u8 thing1;
} struct_D_80192CA8_entry;

/** @cond */

void func_800C94A4(u8);
void func_800CADD0(u8, f32);
void func_800C13F0(void);
void audio_reset_session_eu(OSMesg);
f32 func_800C1480(u8, u8);
s8 func_800C15D0(u8, u8, u8);
s8 func_800C16E8(f32, f32, u8);
f32 func_800C1934(u8, u8);
void func_800C19D0(u8, u8, u8);
struct Unk_8018EFD8* func_800C1C88(u8, Vec3f, Vec3f, f32*, u8, u32);
void func_800C1DA4(Camera*, Vec3s, struct Unk_8018EFD8*);
void func_800C1E2C(Camera*, Vec3f, struct Unk_8018EFD8*);
void func_800C1F8C(void);

Vec3f* func_800C21E8(Vec3f, u32);
void func_800C2274(u8);
void func_800C2474(void);
void func_800C284C(u8, u8, u8, u16);
void func_800C29B4(u8, u16);

void func_800C3724(void);
void func_800C2A2C(u32);
void func_800C3448(u32);
void func_800C3478(void);
u16 func_800C3508(u8);
void func_800C3608(u8, u8);
u8 func_800C357C(s32);
void func_800C35E8(u8);
void func_800C36C4(u8, u8, u8, u8);
void func_800C3F70(void);

void func_800C400C(void);
void func_800C4084(u16);
void func_800C40F0(u8);
void play_sound(u32, Vec3f*, u8, f32*, f32*, s8*);
void func_800C41CC(u8, struct SoundCharacteristics*);
void func_800C4398(void);
void delete_sound_from_bank(u8, u8);
void func_800C4888(u8);
void func_800C4FE4(u8);

void func_800C5278(u8);
void func_800C5384(u8, Vec3f*);
void func_800C54B8(u8, Vec3f*);
void func_800C550C(Vec3f*);
void func_800C5578(Vec3f*, u32);
void func_800C56F0(u32);
void func_800C5848(void);
void fade_channel_volume_scale(u8, u8, u16);
void func_800C5968(u8);
void func_800C59C4(void);
void sound_init(void);
void func_800C5BD0(void);
void func_800C5C40(void);
void func_800C5CB8(void);
void func_800C5D04(u8);
void func_800C5E38(u8);

void func_800C6108(u8);
void func_800C64A0(u8);
void func_800C6758(u8);
void func_800C683C(u8);

void func_800C70A8(u8);
void func_800C76C0(u8);

void func_800C847C(u8);
void func_800C86D8(u8);
void func_800C8770(u8);
void func_800C8920(void);
void func_800C89E4(void);
void func_800C8AE4(void);
void func_800C8C7C(u8);
void func_800C8CCC(void);
void play_sound2(s32);
void play_sequence(u16);
void play_sequence2(u16);
void func_800C8F44(u8);
void func_800C8F80(u8, u32);

void func_800C9018(u8, u32);
void func_800C9060(u8, u32);
void func_800C90F4(u8, u32);
void func_800C9250(u8);
void func_800C92CC(u8, u32);
void func_800C94A4(u8);
void func_800C97C4(u8);
void func_800C98B8(Vec3f, Vec3f, u32);
void func_800C99E0(Vec3f, s32);
void func_800C9A88(u8);
void func_800C9D0C(u8);
void func_800C9D80(Vec3f, Vec3f, u32);
void func_800C9EF4(Vec3f, u32);
void func_800C9F90(u8);

void func_800CA008(u8, u8);
void func_800CA0A0(void);
void func_800CA0B8(void);
void func_800CA0CC(void);
void func_800CA0E4(void);
void func_800CA118(u8);
void func_800CA24C(u8);
void func_800CA270(void);
void func_800CA288(u8, s8);
void func_800CA2B8(u8);
void func_800CA2E4(u8, s8);
void func_800CA30C(u8);
void func_800CA330(u8);
void func_800CA388(u8);
void play_sequences(u16, u16);
void func_800CA49C(u8);
void func_800CA59C(u8);
void func_800CA984(u8);
void func_800CAACC(u8);
void func_800CAB4C(u8);
void func_800CAC08(void);
void func_800CAC60(s32);
void func_800CAD40(s32);
void func_800CAEC4(u8, f32);
void func_800CAFC0(u8);

void func_800CB134(void);
void func_800CB14C(void);
void func_800CB2C4(void);
void func_800CBC24(void);

extern void audio_set_player_volume(u8 player, f32 volume);

// This is some from other file, its definitely not part of audio's data/bss
extern s8 D_801657E5;

// Based on SM64, this should be part of data.c's BSS section. Somehow.
extern s32 gAudioErrorFlags;

extern s8 D_8018EF10;
extern UnkStruct8018EF18 D_8018EF18[16];
extern struct Unk_8018EFD8 D_8018EFD8[];
// These are indexes for D_8018EFD8, but their purpose is unknown
extern u8 D_8018FB90;
extern u8 D_8018FB91;
extern Camera* gCopyCamera[4];
extern Vec3f gVelocityCamera[4];
extern Vec3f gCameraLastPos[4];
extern u8 D_8018FC08;
extern s16 D_8018FC10[4][2];

extern struct SoundCharacteristics sSoundBanks[SOUND_BANK_COUNT][20];
extern u8 sSoundBankUsedListBack[SOUND_BANK_COUNT];
extern u8 sSoundBankFreeListFront[SOUND_BANK_COUNT];
extern u8 sNumSoundsInBank[SOUND_BANK_COUNT];
extern u8 D_80192AB8[SOUND_BANK_COUNT][8][8];
extern u8 D_80192C38;
extern bool sSoundBankDisabled[SOUND_BANK_COUNT];
extern struct ChannelVolumeScaleFade D_80192C48[SOUND_BANK_COUNT];
extern struct_D_80192CA8_entry D_80192CA8[3][5];
extern u8 D_80192CC6[3];
extern u32 D_80192CD0[256];
extern struct_D_801930D0_entry D_801930D0[3];
extern u8 sNumProcessedSoundRequests;
extern u8 sSoundRequestCount;
extern struct Sound sSoundRequests[0x100];

// Data entries
extern u8 D_800E9DA0;
extern s32 D_800E9DB4[];
extern f32 D_800E9DC4[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f };
extern f32 D_800E9DD4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9DE4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9DF4[8]; // = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
extern s32 D_800E9E14[4]; // = { 0, 0, 0, 0 };
extern s32 D_800E9E24[4]; // = { 0, 0, 0, 0 };
extern s32 D_800E9E34[8];
extern f32 D_800E9E54[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f }; /* const */
extern f32 D_800E9E64[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern s32 D_800E9E74[4]; // = { 0, 0, 0, 0 };          /* const */
extern s32 D_800E9E84[4];
extern u32 D_800E9E94[4];
extern s32 D_800E9EA4[4]; // = { 0, 0, 0, 0 };
extern f32 D_800E9EB4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EC4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9ED4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EE4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EF4[4];
extern f32 D_800E9F04[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f };
extern f32 D_800E9F14[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f }; /* const */
extern u8 D_800E9F24[8];  // = { 0, 0, 0, 0, 0, 0, 0, 0 };
extern u8 D_800E9F2C[8];  // = { 0, 0, 0, 0, 0, 0, 0, 0 };
extern f32 D_800E9F34[8]; // = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; /* const */
extern f32 D_800E9F54[8]; // = { 1.0f, 1.0f, 1.0f, 1.0f }; /* const */
extern u8 D_800E9F74[4];
extern u8 D_800E9F78[4];
extern struct Unk_800E9F7C D_800E9F7C[4];
extern u8 D_800E9F90[];
extern struct Unk_800EA06C D_800EA06C[8];
extern u8 D_800EA0EC[];
extern u8 D_800EA0F0; // = 0;
extern u8 D_800EA0F4;
extern u8 D_800EA104;
extern u8 D_800EA108; // = 0;
extern u8 D_800EA10C[];
extern f32 D_800EA110[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800EA120[4];
extern f32 D_800EA130[8];
extern f32 D_800EA150; // = 1.4f;
extern u8 D_800EA154[];
extern u16 gCurrentMusicSeq;
extern u16 D_800EA160;
extern u8 D_800EA164;
extern s8 D_800EA168;
extern u8 D_800EA170[];
extern u16 D_800EA174; // = 0;
extern f32 D_800EA178;
extern f32 D_800EA17C;
extern u16 D_800EA180; // = 0;                          /* const */
extern u16 D_800EA184;
extern u8 D_800EA188[][6];
extern u8 D_800EA1A0[][6];
extern u8 D_800EA1C0; // = 0;
extern u16 D_800EA1C4;
// Most similar to gGlobalSoundSource from SM64, but I don't know if its really
// a sound source, its usage makes it look like a 0'd Vec3f for general usage
extern Vec3f D_800EA1C8; // = {0.0f, 0.0f, 0.0f}
extern f32 D_800EA1D4;
extern u8 D_800EA1DC; // = 0;
extern u8 D_800EA1E4;
extern u8 D_800EA1E8;
extern u8 D_800EA1EC;
extern u8 D_800EA1F0[];
extern u8 D_800EA1F4[];
extern u8 D_800EA244;

extern s8 D_800EA16C;

extern OSMesgQueue* D_800EA3B0;
extern OSMesgQueue* D_800EA3B4;

/** @endcond */

#endif
