#include <libultraship.h>
#include <macros.h>
#include <stubs.h>

#include "audio/data.h"
#include "audio/effects.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/synthesis.h"
#include "audio/playback.h"
#include "audio/seqplayer.h"

s16 gVolume;
s8 gUseReverb;
s8 gNumSynthesisReverbs;
struct NoteSubEu* gNoteSubsEu;

struct SoundAllocPool gAudioSessionPool;
struct SoundAllocPool gAudioInitPool;
struct SoundAllocPool gNotesAndBuffersPool;
u8 sAudioHeapPad[0x20]; // probably two unused pools
struct SoundAllocPool gSeqAndBankPool;
struct SoundAllocPool gPersistentCommonPool;
struct SoundAllocPool gTemporaryCommonPool;

struct SoundMultiPool gSeqLoadedPool;
struct SoundMultiPool gBankLoadedPool;
struct SoundMultiPool gUnusedLoadedPool;

struct Unk1Pool gUnkPool1;

struct PoolSplit sSessionPoolSplit;
struct PoolSplit2 sSeqAndBankPoolSplit;
struct PoolSplit sPersistentCommonPoolSplit;
struct PoolSplit sTemporaryCommonPoolSplit;

u8 gUnkLoadStatus[0x40];
u8 gBankLoadStatus[0x40];
u8 gSeqLoadStatus[0x100];

volatile u8 gAudioResetStatus;
u8 gAudioResetPresetIdToLoad;
s32 gAudioResetFadeOutFramesLeft;

u8 gAudioUnusedBuffer[0x1000];

struct Note* gNotes;

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be used in some sort of printf variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char heapAudioString00[] = "Warning:Kill Note  %x \n";
char heapAudioString01[] = "Kill Voice %d (ID %d) %d\n";
char heapAudioString02[] = "Warning: Running Sequence's data disappear!\n";
char heapAudioString03[] = "Audio:Memory:Heap OverFlow : Not Allocate %d!\n";
char heapAudioString04[] = "Audio:Memory:DataHeap Not Allocate \n";
char heapAudioString05[] = "StayHeap Not Allocate %d\n";
char heapAudioString06[] = "AutoHeap Not Allocate %d\n";
char heapAudioString07[] = "Status ID0 : %d  ID1 : %d\n";
char heapAudioString08[] = "id 0 is Stopping\n";
char heapAudioString09[] = "id 0 is Stop\n";
char heapAudioString10[] = "id 1 is Stopping\n";
char heapAudioString11[] = "id 1 is Stop\n";
char heapAudioString12[] = "WARNING: NO FREE AUTOSEQ AREA.\n";
char heapAudioString13[] = "WARNING: NO STOP AUTO AREA.\n";
char heapAudioString14[] = "         AND TRY FORCE TO STOP SIDE \n";
char heapAudioString15[] = "Check ID0  (seq ID %d) Useing ...\n";
char heapAudioString16[] = "Check ID1  (seq ID %d) Useing ...\n";
char heapAudioString17[] = "No Free Seq area.\n";
char heapAudioString18[] = "CH %d: ID %d\n";
char heapAudioString19[] = "TWO SIDES ARE LOADING... ALLOC CANCELED.\n";
char heapAudioString20[] = "WARNING: Before Area Overlaid After.";
char heapAudioString21[] = "WARNING: After Area Overlaid Before.";
char heapAudioString22[] = "MEMORY:SzHeapAlloc ERROR: sza->side %d\n";
char heapAudioString23[] = "Audio:MEMORY:SzHeap Overflow error. (%d bytes)\n";
char heapAudioString24[] = "Auto Heap Unhit for ID %d\n";
char heapAudioString25[] = "Heap Reconstruct Start %x\n";
char heapAudioString26[] = "AHPBASE %x\n";
char heapAudioString27[] = "AHPCUR  %x\n";
char heapAudioString28[] = "HeapTop %x\n";
char heapAudioString29[] = "SynoutRate %d / %d \n";
char heapAudioString30[] = "FXSIZE %d\n";
char heapAudioString31[] = "FXCOMP %d\n";
char heapAudioString32[] = "FXDOWN %d\n";
char heapAudioString33[] = "WaveCacheLen: %d\n";
char heapAudioString34[] = "SpecChange Finished\n";
char heapAudioString35[] = "Fbank Seq %x\n";
char heapAudioString36[] = "Already Load Type %d,ID %d\n";
char heapAudioString37[] = "Warning:Emem Over,not alloc %d\n";
char heapAudioString38[] = "Write %d\n";

extern u8 gUnkLoadStatus[];

void reset_bank_and_seq_load_status(void) {
    s32 i;
    for (i = 0; i < 64; i++) {
        if (gBankLoadStatus[i] != 5) {
            gBankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 64; i++) {
        if (gUnkLoadStatus[i] != 5) {
            gUnkLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 256; i++) {
        if (gSeqLoadStatus[i] != 5) {
            gSeqLoadStatus[i] = 0;
        }
    }
}

void discard_bank(s32 bankId) {
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        struct Note* note = &gNotes[i];

        if (note->noteSubEu.bankId == bankId) {
            // (These prints are unclear. Arguments are picked semi-randomly.)
            // eu_stubbed_printf_1("Warning:Kill Note  %x \n", i);
            if (note->priority >= NOTE_PRIORITY_MIN) {
                // eu_stubbed_printf_3("Kill Voice %d (ID %d) %d\n", note->waveId, bankId, note->priority);
                // eu_stubbed_printf_0("Warning: Running Sequence's data disappear!\n");
                note->parentLayer->enabled = false; // is 0x48, should be 0x44
                note->parentLayer->finished = true;
            }
            note_disable(note);
            audio_list_remove(&note->listItem);
            audio_list_push_back(&gNoteFreeLists.disabled, &note->listItem);
        }
    }
}

void discard_sequence(s32 seqId) {
    s32 i;

    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        if (gSequencePlayers[i].enabled && gSequencePlayers[i].seqId == seqId) {
            sequence_player_disable(&gSequencePlayers[i]);
        }
    }
}

void* soundAlloc(struct SoundAllocPool* pool, u32 size) {
    u8* start;
    u8* pos;
    u32 alignedSize = ALIGN16(size);

    start = pool->cur;
    if (start + alignedSize <= pool->start + pool->size) {
        pool->cur += alignedSize;
        for (pos = start; pos < pool->cur; pos++) {
            *pos = 0;
        }
    } else {
        // eu_stubbed_printf_1("Heap OverFlow : Not Allocate %d!\n", size);
        return NULL;
    }
    pool->numAllocatedEntries++;
    return start;
}

void sound_alloc_pool_init(struct SoundAllocPool* pool, void* memAddr, u32 size) {
    pool->cur = pool->start = (u8*) ALIGN16((uintptr_t) memAddr);
    pool->size = size;
    pool->numAllocatedEntries = 0;
}

void persistent_pool_clear(struct PersistentPool* persistent) {
    persistent->pool.numAllocatedEntries = 0;
    persistent->pool.cur = persistent->pool.start;
    persistent->numEntries = 0;
}

void temporary_pool_clear(struct TemporaryPool* temporary) {
    temporary->pool.numAllocatedEntries = 0;
    temporary->pool.cur = temporary->pool.start;
    temporary->nextSide = 0;
    temporary->entries[0].ptr = temporary->pool.start;
    temporary->entries[1].ptr = temporary->pool.start + temporary->pool.size;
    temporary->entries[0].id = -1; // should be at 1e not 1c
    temporary->entries[1].id = -1;
}

void func_800B90E0(struct SoundAllocPool* pool) {
    pool->numAllocatedEntries = 0;
    pool->cur = pool->start;
}

// inspired by sound_init_main_pools in sm64
void sound_init_main_pools(s32 arg0) {
    sound_alloc_pool_init(&gAudioInitPool, &gAudioHeap, arg0);
    sound_alloc_pool_init(&gAudioSessionPool, gAudioHeap + arg0, gAudioHeapSize - arg0);
}

// inspired by session_pools_init in sm64
void func_800B914C(struct PoolSplit* arg0) {
    gAudioSessionPool.cur = gAudioSessionPool.start;
    sound_alloc_pool_init(&gNotesAndBuffersPool, soundAlloc(&gAudioSessionPool, arg0->wantSeq), arg0->wantSeq);
    sound_alloc_pool_init(&gSeqAndBankPool, soundAlloc(&gAudioSessionPool, arg0->wantCustom), arg0->wantCustom);
}

void seq_and_bank_pool_init(struct PoolSplit2* a) {
    gSeqAndBankPool.cur = gSeqAndBankPool.start;
    sound_alloc_pool_init(&gPersistentCommonPool, soundAlloc(&gSeqAndBankPool, a->wantPersistent), a->wantPersistent);
    sound_alloc_pool_init(&gTemporaryCommonPool, soundAlloc(&gSeqAndBankPool, a->wantTemporary), a->wantTemporary);
}

void persistent_pools_init(struct PoolSplit* a) {
    gPersistentCommonPool.cur = gPersistentCommonPool.start;
    sound_alloc_pool_init(&gSeqLoadedPool.persistent.pool, soundAlloc(&gPersistentCommonPool, a->wantSeq), a->wantSeq);
    sound_alloc_pool_init(&gBankLoadedPool.persistent.pool, soundAlloc(&gPersistentCommonPool, a->wantBank),
                          a->wantBank);
    sound_alloc_pool_init(&gUnusedLoadedPool.persistent.pool, soundAlloc(&gPersistentCommonPool, a->wantUnused),
                          a->wantUnused);
    persistent_pool_clear(&gSeqLoadedPool.persistent);
    persistent_pool_clear(&gBankLoadedPool.persistent);
    persistent_pool_clear(&gUnusedLoadedPool.persistent);
}

void temporary_pools_init(struct PoolSplit* a) {
    gTemporaryCommonPool.cur = gTemporaryCommonPool.start;
    sound_alloc_pool_init(&gSeqLoadedPool.temporary.pool, soundAlloc(&gTemporaryCommonPool, a->wantSeq), a->wantSeq);
    sound_alloc_pool_init(&gBankLoadedPool.temporary.pool, soundAlloc(&gTemporaryCommonPool, a->wantBank), a->wantBank);
    sound_alloc_pool_init(&gUnusedLoadedPool.temporary.pool, soundAlloc(&gTemporaryCommonPool, a->wantUnused),
                          a->wantUnused);
    temporary_pool_clear(&gSeqLoadedPool.temporary);
    temporary_pool_clear(&gBankLoadedPool.temporary);
    temporary_pool_clear(&gUnusedLoadedPool.temporary);
}

void* alloc_bank_or_seq(struct SoundMultiPool* arg0, s32 arg1, s32 size, s32 arg3, s32 id) {
    struct TemporaryPool* tp;
    struct SoundAllocPool* pool;
    void* ret;
    u16 UNUSED _firstVal;
    u16 UNUSED _secondVal;
    u16 firstVal;
    u16 secondVal;
    s32 var_v1_2;
    u8* table;
    u8 isSound;

    if (arg3 == 0) {
        tp = &arg0->temporary;
        // clang-format off
        if (arg0 == &gSeqLoadedPool) {
            table = gSeqLoadStatus; isSound = 0;
        } else {
            // wtf?
            firstVal += 0;
            if (arg0 == &gBankLoadedPool) {
                isSound = 1; table = gBankLoadStatus;
            } else if (arg0 == &gUnusedLoadedPool) {
                table = gUnkLoadStatus; isSound = 2;
            }
        }
        // clang-format on
        if (tp->entries[0].id == -1) {
            firstVal = 0;
        } else {
            firstVal = table[tp->entries[0].id];
        }
        if (tp->entries[1].id == -1) {
            secondVal = 0;
        } else {
            secondVal = table[tp->entries[1].id];
        }
        if (isSound == 1) {
            if (firstVal == 4) {
                for (var_v1_2 = 0; var_v1_2 < gMaxSimultaneousNotes; var_v1_2++) {
                    if (gNotes[var_v1_2].noteSubEu.bankId == tp->entries[0].id && gNotes[var_v1_2].noteSubEu.enabled) {
                        break;
                    }
                }
                if (var_v1_2 == gMaxSimultaneousNotes) {
                    if (gBankLoadStatus[tp->entries[0].id] != SOUND_LOAD_STATUS_5) {
                        gBankLoadStatus[tp->entries[0].id] = SOUND_LOAD_STATUS_DISCARDABLE;
                    }
                    firstVal = SOUND_LOAD_STATUS_DISCARDABLE;
                }
            }
            if (secondVal == 4) {
                for (var_v1_2 = 0; var_v1_2 < gMaxSimultaneousNotes; var_v1_2++) {
                    if (gNotes[var_v1_2].noteSubEu.bankId == tp->entries[1].id && gNotes[var_v1_2].noteSubEu.enabled) {
                        break;
                    }
                }
                if (var_v1_2 == gMaxSimultaneousNotes) {
                    if (gBankLoadStatus[tp->entries[1].id] != SOUND_LOAD_STATUS_5) {
                        gBankLoadStatus[tp->entries[1].id] = SOUND_LOAD_STATUS_DISCARDABLE;
                    }
                    secondVal = SOUND_LOAD_STATUS_DISCARDABLE;
                }
            }
        }
        if (firstVal == 0) {
            tp->nextSide = 0;
        } else if (secondVal == 0) {
            tp->nextSide = 1;
        } else {
            if ((firstVal == 3) && (secondVal == 3)) {
                // ?
            } else if (firstVal == 3) {
                tp->nextSide = 0;
            } else if (secondVal == 3) {
                tp->nextSide = 1;
            } else {
                if (isSound == 0) {
                    if (firstVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < SEQUENCE_PLAYERS; var_v1_2++) {
                            if (gSequencePlayers[var_v1_2].enabled &&
                                gSequencePlayers[var_v1_2].seqId == tp->entries[0].id) {
                                break;
                            }
                        }
                        if (var_v1_2 == SEQUENCE_PLAYERS) {
                            tp->nextSide = 0;
                            goto out;
                        }
                    }
                    if (secondVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < SEQUENCE_PLAYERS; var_v1_2++) {
                            if (gSequencePlayers[var_v1_2].enabled &&
                                gSequencePlayers[var_v1_2].seqId == tp->entries[1].id) {
                                break;
                            }
                        }
                        if (var_v1_2 == SEQUENCE_PLAYERS) {
                            tp->nextSide = 1;
                            goto out;
                        }
                    }
                } else if (isSound == 1) {
                    if (firstVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < gMaxSimultaneousNotes; var_v1_2++) {
                            if (gNotes[var_v1_2].noteSubEu.bankId == tp->entries[0].id &&
                                gNotes[var_v1_2].noteSubEu.enabled) {
                                break;
                            }
                        }
                        if (var_v1_2 == gMaxSimultaneousNotes) {
                            tp->nextSide = 0;
                            goto out;
                        }
                    }
                    if (secondVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < gMaxSimultaneousNotes; var_v1_2++) {
                            if (gNotes[var_v1_2].noteSubEu.bankId == tp->entries[1].id &&
                                gNotes[var_v1_2].noteSubEu.enabled) {
                                break;
                            }
                        }
                        if (var_v1_2 == gMaxSimultaneousNotes) {
                            tp->nextSide = 1;
                            goto out;
                        }
                    }
                }
                if (tp->nextSide == 0) {
                    if (firstVal == SOUND_LOAD_STATUS_IN_PROGRESS) {
                        if (secondVal != SOUND_LOAD_STATUS_IN_PROGRESS) {
                            tp->nextSide = 1;
                            goto out;
                        }
                    } else {
                        goto out;
                    }
                } else {
                    if (secondVal == SOUND_LOAD_STATUS_IN_PROGRESS) {
                        if (firstVal != SOUND_LOAD_STATUS_IN_PROGRESS) {
                            tp->nextSide = 0;
                            goto out;
                        }
                    } else {
                        goto out;
                    }
                }
                return NULL;
            out:;
            }
        }

        pool = &arg0->temporary.pool;
        if (tp->entries[tp->nextSide].id != (s8) -1) {
            table[tp->entries[tp->nextSide].id] = SOUND_LOAD_STATUS_NOT_LOADED;
            if (isSound == true) {
                discard_bank(tp->entries[tp->nextSide].id);
            }
        }
        switch (tp->nextSide) {
            case 0:
                tp->entries[0].ptr = pool->start;
                tp->entries[0].id = (s16) id;
                tp->entries[0].size = (u32) size;
                pool->cur = pool->start + size;
                if (tp->entries[1].ptr < pool->cur) {
                    table[tp->entries[1].id] = 0;
                    switch (isSound) { /* irregular */
                        case 0:
                            discard_sequence((s32) tp->entries[1].id);
                            break;
                        case 1:
                            discard_bank((s32) tp->entries[1].id);
                            break;
                    }
                    tp->entries[1].id = -1;
                    tp->entries[1].ptr = pool->start + pool->size;
                }
                ret = tp->entries[0].ptr;
                break;
            case 1:
                tp->entries[1].ptr = pool->start + pool->size - size - 0x10;
                tp->entries[1].id = (s16) id;
                tp->entries[1].size = (u32) size;
                if ((u32) tp->entries[1].ptr < (u32) pool->cur) {
                    table[tp->entries[0].id] = 0;
                    switch (isSound) { /* switch 1; irregular */
                        case 0:        /* switch 1 */
                            discard_sequence((s32) tp->entries[0].id);
                            break;
                        case 1: /* switch 1 */
                            discard_bank((s32) tp->entries[0].id);
                            break;
                    }
                    tp->entries[0].id = -1;
                    pool->cur = pool->start;
                }
                ret = tp->entries[1].ptr;
                break;
            default:
                return NULL;
        }
        tp->nextSide ^= 1;
        return ret;
    }
    ret = soundAlloc(&arg0->persistent.pool, arg1 * size);
    arg0->persistent.entries[arg0->persistent.numEntries].ptr = ret;
    if (ret == NULL) {
        switch (arg3) {
            case 2:
                return alloc_bank_or_seq(arg0, arg1, size, 0, id);
            case 1:
            case 0:
                return NULL;
        }
    }
    arg0->persistent.entries[arg0->persistent.numEntries].id = (s16) id;
    arg0->persistent.entries[arg0->persistent.numEntries].size = (u32) size;
    return arg0->persistent.entries[arg0->persistent.numEntries++].ptr;
}

void* get_bank_or_seq(s32 poolIdx, s32 arg1, s32 id) {
    void* ret;

    ret = unk_pool1_lookup(poolIdx, id);
    if (ret != NULL) {
        return ret;
    }
    return get_bank_or_seq_inner(poolIdx, arg1, id);
}

void* get_bank_or_seq_inner(s32 poolIdx, s32 arg1, s32 bankId) {
    u32 i;
    struct SoundMultiPool* loadedPool;
    struct TemporaryPool* temporary;
    struct PersistentPool* persistent;

    switch (poolIdx) {
        case 0:
            loadedPool = &gSeqLoadedPool;
            break;
        case 1:
            loadedPool = &gBankLoadedPool;
            break;
        case 2:
            loadedPool = &gUnusedLoadedPool;
            break;
    }

    temporary = &loadedPool->temporary;
    if (arg1 == 0) {
        if (temporary->entries[0].id == bankId) {
            temporary->nextSide = 1;
            return temporary->entries[0].ptr;
        } else if (temporary->entries[1].id == bankId) {
            temporary->nextSide = 0;
            return temporary->entries[1].ptr;
        } else {
            return NULL;
        }
    }

    persistent = &loadedPool->persistent;
    for (i = 0; i < persistent->numEntries; i++) {
        if (persistent->entries[i].id == bankId) {
            return persistent->entries[i].ptr;
        }
    }

    if (arg1 == 2) {
        return get_bank_or_seq(poolIdx, 0, bankId);
    }
    return NULL;
}

void func_800B9BE4(f32 arg0, f32 arg1, u16* arg2) {
    s32 i;
    f32 tmp[16];

    tmp[0] = arg1 * 262159.0f;
    tmp[8] = arg0 * 262159.0f;
    tmp[1] = (arg1 * arg0) * 262159.0f;
    tmp[9] = ((arg0 * arg0) + arg1) * 262159.0f;

    for (i = 2; i < 8; i++) {
        //! @bug they probably meant to store the value to tmp[i] and tmp[8 + i]
        arg2[i] = arg1 * tmp[i - 2] + arg0 * tmp[i - 1];
        arg2[8 + i] = arg1 * tmp[6 + i] + arg0 * tmp[7 + i];
    }

    for (i = 0; i < 16; i++) {
        arg2[i] = tmp[i];
    }
}

void decrease_reverb_gain(void) {
    s32 i;
    for (i = 0; i < gNumSynthesisReverbs; i++) {
        gSynthesisReverbs[i].reverbGain -= gSynthesisReverbs[i].reverbGain / 4;
    }
}

s32 audio_shut_down_and_reset_step(void) {
    s32 i;
    s32 j;

    switch (gAudioResetStatus) {
        case 5:
            for (i = 0; i < SEQUENCE_PLAYERS; i++) {
                sequence_player_disable(&gSequencePlayers[i]);
            }
            gAudioResetFadeOutFramesLeft = 4;
            gAudioResetStatus--;
            break;
        case 4:
            if (gAudioResetFadeOutFramesLeft != 0) {
                gAudioResetFadeOutFramesLeft--;
                decrease_reverb_gain();
            } else {
                for (i = 0; i < gMaxSimultaneousNotes; i++) {
                    if (gNotes[i].noteSubEu.enabled && gNotes[i].adsr.state != ADSR_STATE_DISABLED) {
                        gNotes[i].adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
                        gNotes[i].adsr.action |= ADSR_ACTION_RELEASE;
                    }
                }
                gAudioResetFadeOutFramesLeft = 0x00000010;
                gAudioResetStatus--;
            }
            break;
        case 3:
            if (gAudioResetFadeOutFramesLeft != 0) {
                gAudioResetFadeOutFramesLeft--;
                decrease_reverb_gain();
            } else {
                for (i = 0; i < NUMAIBUFFERS; i++) {
                    for (j = 0; j < (s32) (AIBUFFER_LEN / sizeof(s16)); j++) {
                        gAiBuffers[i][j] = 0;
                    }
                }
                gAudioResetFadeOutFramesLeft = 4;
                gAudioResetStatus--;
            }
            break;
        case 2:
            if (gAudioResetFadeOutFramesLeft != 0) {
                gAudioResetFadeOutFramesLeft--;
            } else {
                gAudioResetStatus--;
            }
            break;
        case 1:
            audio_reset_session();
            gAudioResetStatus = 0;
            break;
    }
    if (gAudioResetStatus < 3) {
        return 0;
    }
    return 1;
}

void audio_reset_session(void) {
    s32 var_s1;
    s32 var_s5;
    s32 temp;
    u32 totalMem;
    u32 temporaryMem;
    u32 persistentMem;
    s16* mem;
    struct SynthesisReverb* reverb;
    struct ReverbSettingsEU* reverbSettings;
    struct AudioSessionSettingsEU* temp_s6 = &gAudioSessionPresets[gAudioResetPresetIdToLoad];

    gSampleDmaNumListItems = 0;
    gAudioBufferParameters.frequency = temp_s6->frequency;
    gAudioBufferParameters.aiFrequency = osAiSetFrequency(gAudioBufferParameters.frequency);
    gAudioBufferParameters.samplesPerFrameTarget = ALIGN16(gAudioBufferParameters.frequency / gRefreshRate);
    gAudioBufferParameters.minAiBufferLength = gAudioBufferParameters.samplesPerFrameTarget - 0x10;
    gAudioBufferParameters.maxAiBufferLength = gAudioBufferParameters.samplesPerFrameTarget + 0x10;
    gAudioBufferParameters.updatesPerFrame = ((gAudioBufferParameters.samplesPerFrameTarget + 0x10) / 192) + 1;
    gAudioBufferParameters.samplesPerUpdate =
        (gAudioBufferParameters.samplesPerFrameTarget / gAudioBufferParameters.updatesPerFrame) & ~0x0007;
    gAudioBufferParameters.samplesPerUpdateMax = gAudioBufferParameters.samplesPerUpdate + 8;
    gAudioBufferParameters.samplesPerUpdateMin = gAudioBufferParameters.samplesPerUpdate - 8;
    gAudioBufferParameters.resampleRate = 32000.0f / ((f32) (s32) gAudioBufferParameters.frequency);
    gAudioBufferParameters.unkUpdatesPerFrameScaled = 0.001171875f / gAudioBufferParameters.updatesPerFrame;
    gAudioBufferParameters.updatesPerFrameInv = 1.0f / gAudioBufferParameters.updatesPerFrame;
    gMaxSimultaneousNotes = temp_s6->maxSimultaneousNotes;
    gVolume = temp_s6->volume;
    gTempoInternalToExternal =
        (u32) (((gAudioBufferParameters.updatesPerFrame * 2880000.0f) / gTatumsPerBeat) / D_803B7178);
    gAudioBufferParameters.presetUnk4 = temp_s6->unk1;
    gAudioBufferParameters.samplesPerFrameTarget *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.maxAiBufferLength *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.minAiBufferLength *= gAudioBufferParameters.presetUnk4;
    gAudioBufferParameters.updatesPerFrame *= gAudioBufferParameters.presetUnk4;
    gMaxAudioCmds =
        (gMaxSimultaneousNotes * 0x14 * gAudioBufferParameters.updatesPerFrame) + (temp_s6->numReverbs * 0x20) + 0x1E0;
    persistentMem = temp_s6->persistentSeqMem + temp_s6->persistentBankMem + temp_s6->unk_18;
    temporaryMem = temp_s6->temporarySeqMem + temp_s6->temporaryBankMem + temp_s6->unk_24;
    totalMem = persistentMem + temporaryMem;
    temp = (gAudioSessionPool.size - totalMem) - 0x100;
    sSessionPoolSplit.wantSeq = temp;
    sSessionPoolSplit.wantCustom = totalMem;
    func_800B914C(&sSessionPoolSplit);
    sSeqAndBankPoolSplit.wantPersistent = persistentMem;
    sSeqAndBankPoolSplit.wantTemporary = temporaryMem;
    seq_and_bank_pool_init(&sSeqAndBankPoolSplit);
    sPersistentCommonPoolSplit.wantSeq = temp_s6->persistentSeqMem;
    sPersistentCommonPoolSplit.wantBank = temp_s6->persistentBankMem;
    sPersistentCommonPoolSplit.wantUnused = temp_s6->unk_18;
    persistent_pools_init(&sPersistentCommonPoolSplit);
    sTemporaryCommonPoolSplit.wantSeq = temp_s6->temporarySeqMem;
    sTemporaryCommonPoolSplit.wantBank = temp_s6->temporaryBankMem;
    sTemporaryCommonPoolSplit.wantUnused = temp_s6->unk_24;
    temporary_pools_init(&sTemporaryCommonPoolSplit);
    reset_bank_and_seq_load_status();
    gNotes = soundAlloc(&gNotesAndBuffersPool, gMaxSimultaneousNotes * sizeof(struct Note));
    note_init_all();
    init_note_free_list();
    gNoteSubsEu =
        soundAlloc(&gNotesAndBuffersPool, gAudioBufferParameters.updatesPerFrame * gMaxSimultaneousNotes * 0x10);
    for (var_s5 = 0; var_s5 != 2; var_s5++) {
        gAudioCmdBuffers[var_s5] = soundAlloc(&gNotesAndBuffersPool, gMaxAudioCmds * sizeof(u64));
    }
    for (var_s5 = 0; var_s5 < 4; var_s5++) {
        gSynthesisReverbs[var_s5].useReverb = 0;
    }

    gNumSynthesisReverbs = temp_s6->numReverbs;
    for (var_s5 = 0; var_s5 < gNumSynthesisReverbs; var_s5++) {
        reverb = &gSynthesisReverbs[var_s5];
        reverbSettings = &temp_s6->reverbSettings[var_s5];
        reverb->windowSize = reverbSettings->windowSize * 64;
        reverb->downsampleRate = reverbSettings->downsampleRate;
        reverb->reverbGain = reverbSettings->gain;
        reverb->useReverb = 8;
        reverb->ringBuffer.left = soundAlloc(&gNotesAndBuffersPool, reverb->windowSize * 2);
        reverb->ringBuffer.right = soundAlloc(&gNotesAndBuffersPool, reverb->windowSize * 2);
        reverb->nextRingBufferPos = 0;
        reverb->unkC = 0;
        reverb->curFrame = 0;
        reverb->bufSizePerChannel = reverb->windowSize;
        reverb->framesLeftToIgnore = 2;
        if (reverb->downsampleRate != 1) {
            reverb->resampleFlags = 1;
            reverb->resampleRate = 0x8000 / reverb->downsampleRate;
            reverb->resampleStateLeft = soundAlloc(&gNotesAndBuffersPool, 0x00000020U);
            reverb->resampleStateRight = soundAlloc(&gNotesAndBuffersPool, 0x00000020U);
            reverb->unk24 = soundAlloc(&gNotesAndBuffersPool, 0x00000020U);
            reverb->unk28 = soundAlloc(&gNotesAndBuffersPool, 0x00000020U);
            for (var_s1 = 0; var_s1 < gAudioBufferParameters.updatesPerFrame; var_s1++) {
                mem = soundAlloc(&gNotesAndBuffersPool, 0x00000300U);
                reverb->items[0][var_s1].toDownsampleLeft = mem;
                reverb->items[0][var_s1].toDownsampleRight = mem + (DEFAULT_LEN_1CH / sizeof(s16));
                mem = soundAlloc(&gNotesAndBuffersPool, 0x00000300U);
                reverb->items[1][var_s1].toDownsampleLeft = mem;
                reverb->items[1][var_s1].toDownsampleRight = mem + (DEFAULT_LEN_1CH / sizeof(s16));
            }
        }
    }

    func_800BB030(gMaxSimultaneousNotes);
    osWritebackDCacheAll();
}

void* unk_pool1_lookup(s32 poolIdx, s32 id) {
    s32 i;

    for (i = 0; i < gUnkPool1.pool.numAllocatedEntries; i++) {
        if (gUnkPool1.entries[i].poolIndex == poolIdx && gUnkPool1.entries[i].id == id) {
            return gUnkPool1.entries[i].ptr;
        }
    }
    return NULL;
}

// SM64 does not appear to have a function
// comparable to this one, not a clue what
// this one is doing.
void func_800BA8B0(s32 poolIdx, s32 id) {
    ALSeqFile* sp3C;
    s32 temp_a2;
    u32 temp_a1;
    u8* var_a3;
    UNUSED u8* temp_v0;
    UNUSED s32 pad;

    switch (poolIdx) { /* irregular */
        case 0:
            sp3C = gSeqFileHeader;
            break;
        case 1:
            sp3C = gAlCtlHeader;
            break;
        case 2:
            sp3C = gAlTbl;
            break;
    }
    if (sp3C->seqArray[id].len == 0) {
        id = (s32) sp3C->seqArray[id].offset;
    }
    if (unk_pool1_lookup(poolIdx, id) == NULL) {
        temp_a2 = gUnkPool1.pool.numAllocatedEntries;
        temp_a1 = sp3C->seqArray[id].len;
        var_a3 = sp3C->seqArray[id].offset;
        if (poolIdx == 1) {
            var_a3 += 0x10;
        }
        gUnkPool1.entries[temp_a2].ptr = soundAlloc(&gUnkPool1.pool, temp_a1);
        if (gUnkPool1.entries[temp_a2].ptr != NULL) {
            audio_dma_copy_immediate(var_a3, gUnkPool1.entries[temp_a2].ptr, temp_a1);
            gUnkPool1.entries[temp_a2].poolIndex = poolIdx;
            gUnkPool1.entries[temp_a2].id = id;
            gUnkPool1.entries[temp_a2].size = temp_a1;
            switch (poolIdx) { /* switch 1; irregular */
                case 0:        /* switch 1 */
                    if (gSeqLoadStatus[id] != 5) {
                        gSeqLoadStatus[id] = 5;
                    }
                    break;
                case 1: /* switch 1 */
                    gCtlEntries[id].instruments = (struct Instrument**) (gUnkPool1.entries[temp_a2].ptr + 4);
                    func_800BB584(id);
                    if (gBankLoadStatus[id] != 5) {
                        gBankLoadStatus[id] = 5;
                    }
                    break;
                case 2: /* switch 1 */
                    break;
            }
        }
    }
}
