#ifndef AUDIO_PORT_EU_H
#define AUDIO_PORT_EU_H

#include "audio/internal.h"

#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x40

void eu_process_audio_cmd(struct EuAudioCmd*);
void seq_player_fade_to_zero_volume(s32 arg0, s32 fadeOutTime);
void func_800CBA64(s32 playerIndex, s32 fadeInTime);
void port_eu_init_queues(void);
void func_800CBB48(s32, s32*);
void func_800CBB88(u32, f32);
void func_800CBBB8(u32, u32);
void func_800CBBE8(u32, s8);
void func_800CBC24(void);
void func_800CBCB0(u32 arg0);
void port_eu_init(void);

extern OSMesgQueue D_801937C0;
extern OSMesgQueue D_801937D8;
extern OSMesgQueue D_801937F0;
extern OSMesgQueue D_80193808;

extern struct EuAudioCmd sAudioCmd[0x100];

extern OSMesg D_80194020[];
extern OSMesg D_80194028[];
extern OSMesg D_80194038[];
extern OSMesg D_8019403C[];

extern OSMesgQueue* D_800EA3A8;
extern OSMesgQueue* D_800EA3AC;
extern OSMesgQueue* D_800EA3B0;
extern OSMesgQueue* D_800EA3B4;
extern s32 D_800EA484;
extern s32 D_800EA4A4;

#endif
