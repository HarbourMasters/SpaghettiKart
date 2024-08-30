#ifndef AUDIO_HEAP_H
#define AUDIO_HEAP_H

#include <libultra/types.h>

#include "internal.h"

#define SOUND_LOAD_STATUS_NOT_LOADED 0
#define SOUND_LOAD_STATUS_IN_PROGRESS 1
#define SOUND_LOAD_STATUS_COMPLETE 2
#define SOUND_LOAD_STATUS_DISCARDABLE 3
#define SOUND_LOAD_STATUS_4 4
#define SOUND_LOAD_STATUS_5 5

#define IS_BANK_LOAD_COMPLETE(bankId) (gBankLoadStatus[bankId] >= SOUND_LOAD_STATUS_COMPLETE)
#define IS_SEQ_LOAD_COMPLETE(seqId) (gSeqLoadStatus[seqId] >= SOUND_LOAD_STATUS_COMPLETE)

struct SoundAllocPool {
    u8* start;
    u8* cur;
    u32 size;
    s32 numAllocatedEntries;
}; // size = 0x10

struct SeqOrBankEntry {
    u8* ptr;
    u32 size;
    s16 poolIndex;
    s16 id;
}; // size = 0xC

struct PersistentPool {
    /*0x00*/ u32 numEntries;
    /*0x04*/ struct SoundAllocPool pool;
    /*0x14*/ struct SeqOrBankEntry entries[32];
}; // size = 0x194

struct TemporaryPool {
    /*EU,   SH*/
    /*0x00, 0x00*/ u32 nextSide;
    /*0x04,     */ struct SoundAllocPool pool;
    /*0x04,        pool.start     */
    /*0x08,        pool.cur       */
    /*0x0C, 0x0C   pool.size      */
    /*0x10, 0x10   pool.numAllocatedEntries */
    /*0x14,     */ struct SeqOrBankEntry entries[2];
    /*0x14, 0x14   entries[0].ptr */
    /*0x18,        entries[0].size*/
    /*0x1C, 0x1E   entries[0].id  */
    /*0x20, 0x20   entries[1].ptr */
    /*0x24,        entries[1].size*/
    /*0x28, 0x2A   entries[1].id  */
}; // size = 0x2C

struct SoundMultiPool {
    /*0x000*/ struct PersistentPool persistent;
    /*0x194*/ struct TemporaryPool temporary;
    /*     */ u32 pad2[4];
}; // size = 0x1D0

struct Unk1Pool {
    struct SoundAllocPool pool;
    struct SeqOrBankEntry entries[32];
};

struct UnkEntry {
    s8 used;
    s8 medium;
    s8 bankId;
    u32 pad;
    u8* srcAddr;
    u8* dstAddr;
    u32 size;
};

struct UnkPool {
    /*0x00*/ struct SoundAllocPool pool;
    /*0x10*/ struct UnkEntry entries[64];
    /*0x510*/ s32 numEntries;
    /*0x514*/ u32 unk514;
};

struct PoolSplit {
    u32 wantSeq;
    u32 wantBank;
    u32 wantUnused;
    u32 wantCustom;
}; // size = 0x10

struct PoolSplit2 {
    u32 wantPersistent;
    u32 wantTemporary;
}; // size = 0x8

void reset_bank_and_seq_load_status(void);
void discard_bank(s32 bankId);
void discard_sequence(s32 seqId);
void* soundAlloc(struct SoundAllocPool* pool, u32 size);
void sound_alloc_pool_init(struct SoundAllocPool* pool, void* memAddr, u32 size);
void persistent_pool_clear(struct PersistentPool* persistent);
void temporary_pool_clear(struct TemporaryPool* temporary);
void func_800B90E0(struct SoundAllocPool* pool);
void sound_init_main_pools(s32);
void func_800B914C(struct PoolSplit*);
void seq_and_bank_pool_init(struct PoolSplit2* a);
void persistent_pools_init(struct PoolSplit* a);
void temporary_pools_init(struct PoolSplit* a);
void* alloc_bank_or_seq(struct SoundMultiPool*, s32, s32, s32, s32);
void* get_bank_or_seq(s32 poolIdx, s32 arg1, s32 id);
void* get_bank_or_seq_inner(s32 poolIdx, s32 arg1, s32 bankId);
void func_800B9BE4(f32, f32, u16*);
void decrease_reverb_gain(void);
s32 audio_shut_down_and_reset_step(void);
void audio_reset_session(void);
void* unk_pool1_lookup(s32 poolIdx, s32 id);
void func_800BA8B0(s32, s32);

extern s32 D_800EA5D0;

extern s32 gMaxAudioCmds;
extern s16 gTempoInternalToExternal;
extern f32 D_803B7178;
extern s32 gRefreshRate;
extern u32 gSampleDmaNumListItems;
extern struct AudioSessionSettingsEU gAudioSessionPresets[];

// Note: In some .asm files D_803AFBC8 has been replaced with gLeftVolRampings
// That is almost certainly incorrect, but I don't know how to fix it at this point
extern s16 gVolume; // D_803AFBC0
extern s8 gUseReverb;
extern s8 gNumSynthesisReverbs; // D_803AFBC3
extern struct NoteSubEu* gNoteSubsEu;
extern struct SoundAllocPool gAudioSessionPool; // D_803AFBC8
extern struct SoundAllocPool gAudioInitPool;    // D_803AFBD8
extern struct SoundAllocPool gNotesAndBuffersPool;
extern struct SoundAllocPool gPersistentCommonPool; // D_803AFC28
extern struct SoundAllocPool gTemporaryCommonPool;  // D_803AFC38
extern struct SoundMultiPool gSeqLoadedPool;        // D_803AFC48
extern struct SoundMultiPool gBankLoadedPool;       // D_803AFE18
extern struct SoundMultiPool gUnusedLoadedPool;
extern struct Unk1Pool gUnkPool1;                   // D_803B01B8
extern struct PoolSplit sSessionPoolSplit;          // D_803B0348
extern struct PoolSplit2 sSeqAndBankPoolSplit;      // D_803B0358
extern struct PoolSplit sPersistentCommonPoolSplit; // D_803B0360;
extern struct PoolSplit sTemporaryCommonPoolSplit;  // D_803B0370;
extern struct SoundMultiPool gUnusedLoadedPool;     // D_803AFFE8
extern struct SoundAllocPool gSeqAndBankPool;       // D_803AFC18
extern u8 gBankLoadStatus[64];                      // D_803B03C0
extern u8 gUnkLoadStatus[64];
extern u8 gSeqLoadStatus[256];        // D_803B0400
extern volatile u8 gAudioResetStatus; // D_803B0500
extern u8 gAudioResetPresetIdToLoad;
extern s32 gAudioResetFadeOutFramesLeft; // D_803B0504
extern struct Note* gNotes;

#endif // AUDIO_HEAP_H
