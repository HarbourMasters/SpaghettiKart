#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include "internal.h"

#define AUDIO_LOCK_UNINITIALIZED 0
#define AUDIO_LOCK_NOT_LOADING 0x76557364
#define AUDIO_LOCK_LOADING 0x19710515

#define NUMAIBUFFERS 3
#ifdef VERSION_EU
#define AIBUFFER_LEN (0xaa * 16)
#else
#define AIBUFFER_LEN (0xa0 * 16)
#endif

// number of DMAs performed during this frame
extern s32 gCurrAudioFrameDmaCount;

extern s16 sSawtoothWaves[256];
extern s16 sTriangleWaves[256];
extern s16 sSineWaves[256];
extern s16 sSquareWaves[256];
extern s16 sUnknownWave6[256];
extern s16 gUnknownWave7[256];
extern s16* gWaveSamples[6];
extern u32 fill[2];
extern f32 gPitchBendFrequencyScale[256];
extern f32 gNoteFrequencies[128];
extern u8 gDefaultShortNoteVelocityTable[16];
extern u8 gDefaultShortNoteDurationTable[16];
extern struct AdsrEnvelope gDefaultEnvelope[];
extern u32 fill2;
extern struct NoteSubEu gZeroNoteSub;
extern struct NoteSubEu gDefaultNoteSub;
extern u16 gHeadsetPanQuantization[0x10];
extern s16 gUnknownData_800F6290[];
extern f32 gHeadsetPanVolume[128];
extern f32 gStereoPanVolume[128];
extern f32 gDefaultPanVolume[128];
extern u32 gAudioRandom;

extern s16 gTatumsPerBeat;          // D_800EA5CC
extern volatile s32 gAudioLoadLock; // D_800EA5DC

extern s32 gAudioTaskIndex;
extern s32 gCurrAiBufferIndex;
extern Acmd* gAudioCmdBuffers[2];    // probably wrong
extern Acmd* gAudioCmd;              // D_803B70D0
extern struct SPTask* gAudioTask;    // D_803B70D4
extern struct SPTask gAudioTasks[2]; // D_803B70D8
extern f32 D_803B7178;
extern s32 gRefreshRate;
extern s16* gAiBuffers[];      // D_803B7180
extern s16 gAiBufferLengths[]; // osAiSetNextBuffer nbytes
extern u16 D_803B7192;         // unconfirmed
extern u32 gAudioRandom;
extern s32 gAudioErrorFlags;
extern u64 gAudioGlobalsEndMarker; // unconfirmed
extern u8 gAudioHeap[];            // unconfirmed

extern struct ReverbSettingsEU sReverbSettings[];
extern struct AudioSessionSettingsEU gAudioSessionPresets[];
extern s8 gUnusedCount800EA5C8;
extern s16 gTatumsPerBeat;
extern s32 gAudioHeapSize;
extern s32 gAudioInitPoolSize;
extern s32 D_800EA5D8;
extern volatile s32 gAudioLoadLock;

#endif
