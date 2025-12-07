#include <libultraship.h>
#include <macros.h>

#include "audio/load.h"
#include "audio/data.h"
#include "audio/heap.h"
#include "audio/internal.h"
#include "audio/external.h"
#include "audio/playback.h"
#include "audio/synthesis.h"
#include "audio/seqplayer.h"
#include "audio/port_eu.h"
#include "port/Engine.h"
#include "buffers/gfx_output_buffer.h"

#include <string.h>

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct SequencePlayer gSequencePlayers[SEQUENCE_PLAYERS];
struct SequenceChannel gSequenceChannels[SEQUENCE_CHANNELS];
struct SequenceChannelLayer gSequenceLayers[SEQUENCE_LAYERS];
struct SequenceChannel gSequenceChannelNone;
// D_803B5F5C
struct AudioListItem gLayerFreeList;
struct NotePool gNoteFreeLists;
OSMesgQueue gCurrAudioFrameDmaQueue;
OSMesg gCurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSIoMesg gCurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSMesgQueue D_803B6720;
OSMesg D_803B6738;

OSIoMesg D_803B6740;
struct SharedDma sSampleDmas[0x70];
u32 gSampleDmaNumListItems;
u32 sSampleDmaListSize1;
s32 D_803B6E60;
s32 load_bss_pad;

u8 sSampleDmaReuseQueue1[256]; // sSampleDmaReuseQueue1
u8 sSampleDmaReuseQueue2[256]; // sSampleDmaReuseQueue2
u8 sSampleDmaReuseQueueTail1;  // sSampleDmaReuseQueueTail1
u8 sSampleDmaReuseQueueTail2;  // sSampleDmaReuseQueueTail2
u8 sSampleDmaReuseQueueHead1;  // sSampleDmaReuseQueueHead1
u8 sSampleDmaReuseQueueHead2;  // sSampleDmaReuseQueueHead2

ALSeqFile* gSeqFileHeader;
ALSeqFile* gAlCtlHeader;
ALSeqFile* gAlTbl;
u8* gAlBankSets;
u16 gSequenceCount;
struct CtlEntry* gCtlEntries;
struct AudioBufferParametersEU gAudioBufferParameters;
u32 D_803B70A8;
s32 gMaxAudioCmds;
s32 gMaxSimultaneousNotes;
s16 gTempoInternalToExternal;
s8 gAudioLibSoundMode;
// If we take SM64 as gospel, these should be in data.c, but that doesn't match
volatile s32 gAudioFrameCount;
s32 gCurrAudioFrameDmaCount;

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be used in some sort of printf variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char loadAudioString00[] = "Romcopy %x -> %x ,size %x\n";
char loadAudioString01[] = "Romcopyend\n";
char loadAudioString02[] = "CAUTION:WAVE CACHE FULL %d";
char loadAudioString03[] = "LOAD  Rom :%x -> Ram :%x  Len:%x\n";
char loadAudioString04[] = "BASE %x %x\n";
char loadAudioString05[] = "LOAD %x %x %x\n";
char loadAudioString06[] = "INSTTOP    %x\n";
char loadAudioString07[] = "INSTMAP[0] %x\n";
char loadAudioString08[] = "already flags %d\n";
char loadAudioString09[] = "already flags %d\n";
char loadAudioString10[] = "ERR:SLOW BANK DMA BUSY\n";
char loadAudioString11[] = "ERR:SLOW DMA BUSY\n";
char loadAudioString12[] = "Check %d  bank %d\n";
char loadAudioString13[] = "Cache Check\n";
char loadAudioString14[] = "NO BANK ERROR\n";
char loadAudioString15[] = "BANK %d LOADING START\n";
char loadAudioString16[] = "BANK %d LOAD MISS (NO MEMORY)!\n";
char loadAudioString17[] = "BANK %d ALREADY CACHED\n";
char loadAudioString18[] = "BANK LOAD MISS! FOR %d\n";
char loadAudioString19[] = "Seq %d Loading Start\n";
char loadAudioString20[] = "Heap Overflow Error\n";
char loadAudioString21[] = "SEQ  %d ALREADY CACHED\n";
char loadAudioString22[] = "Ok,one bank slow load Start \n";
char loadAudioString23[] = "Sorry,too many %d bank is none.fast load Start \n";
char loadAudioString24[] = "Seq %d:Default Load Id is %d\n";
char loadAudioString25[] = "Seq Loading Start\n";
char loadAudioString26[] = "Error:Before Sequence-SlowDma remain.\n";
char loadAudioString27[] = "      Cancel Seq Start.\n";
char loadAudioString28[] = "SEQ  %d ALREADY CACHED\n";
char loadAudioString29[] = "Clear Workarea %x -%x size %x \n";
char loadAudioString30[] = "AudioHeap is %x\n";
char loadAudioString31[] = "Heap reset.Synth Change %x \n";
char loadAudioString32[] = "Heap %x %x %x\n";
char loadAudioString33[] = "Main Heap Initialize.\n";
char loadAudioString34[] = "---------- Init Completed. ------------\n";
char loadAudioString35[] = " Syndrv    :[%6d]\n";
char loadAudioString36[] = " Seqdrv    :[%6d]\n";
char loadAudioString37[] = " audiodata :[%6d]\n";
char loadAudioString38[] = "---------------------------------------\n";

/**
 * Performs an immediate DMA copy
 */
void audio_dma_copy_immediate(u8* devAddr, void* vAddr, size_t nbytes) {
    // eu_stubbed_printf_3("Romcopy %x -> %x ,size %x\n", devAddr, vAddr, nbytes);
    osInvalDCache(vAddr, nbytes);
    osPiStartDma(&D_803B6740, OS_MESG_PRI_HIGH, OS_READ, (uintptr_t) devAddr, vAddr, nbytes, &D_803B6720);
    osRecvMesg(&D_803B6720, NULL, OS_MESG_BLOCK);
    // eu_stubbed_printf_0("Romcopyend\n");
}

/**
 * Performs an asynchronus (normal priority) DMA copy
 */
void audio_dma_copy_async(uintptr_t devAddr, void* vAddr, size_t nbytes, OSMesgQueue* queue, OSIoMesg* mesg) {
    osInvalDCache(vAddr, nbytes);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, vAddr, nbytes, queue);
}

/**
 * Performs a partial asynchronous (normal priority) DMA copy. This is limited
 * to 0x1000 bytes transfer at once.
 */
void audio_dma_partial_copy_async(uintptr_t* devAddr, u8** vAddr, size_t* remaining, OSMesgQueue* queue,
                                  OSIoMesg* mesg) {
    size_t transfer = (*remaining >= 0x1000 ? 0x1000 : *remaining);
    *remaining -= transfer;
    osInvalDCache(*vAddr, transfer);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, *devAddr, *vAddr, transfer, queue);
    *devAddr += transfer;
    *vAddr += transfer;
}

void decrease_sample_dma_ttls() {
    u32 i;

    for (i = 0; i < sSampleDmaListSize1; i++) {
        struct SharedDma* temp = &sSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->reuseIndex = sSampleDmaReuseQueueHead1;
                sSampleDmaReuseQueue1[sSampleDmaReuseQueueHead1++] = (u8) i;
            }
        }
    }

    for (i = sSampleDmaListSize1; i < gSampleDmaNumListItems; i++) {
        struct SharedDma* temp = &sSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->reuseIndex = sSampleDmaReuseQueueHead2;
                sSampleDmaReuseQueue2[sSampleDmaReuseQueueHead2++] = (u8) i;
            }
        }
    }

    D_803B6E60 = 0;
}

void* dma_sample_data(uintptr_t devAddr, u32 size, s32 arg2, u8* dmaIndexRef) {
    s32 hasDma = false;
    struct SharedDma* dma;
    uintptr_t dmaDevAddr;
    u32 transfer;
    u32 i;
    u32 dmaIndex;
    size_t bufferPos;
    UNUSED u32 pad;

    if (arg2 != 0 || *dmaIndexRef >= sSampleDmaListSize1) {
        for (i = sSampleDmaListSize1; i < gSampleDmaNumListItems; i++) {
            dma = &sSampleDmas[i];
            bufferPos = devAddr - dma->source;
            if (0 <= bufferPos && (size_t) bufferPos <= dma->bufSize - size) {
                // We already have a DMA request for this memory range.
                if (dma->ttl == 0 && sSampleDmaReuseQueueTail2 != sSampleDmaReuseQueueHead2) {
                    // Move the DMA out of the reuse queue, by swapping it with the
                    // tail, and then incrementing the tail.
                    if (dma->reuseIndex != sSampleDmaReuseQueueTail2) {
                        sSampleDmaReuseQueue2[dma->reuseIndex] = sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2];
                        sSampleDmas[sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2]].reuseIndex = dma->reuseIndex;
                    }
                    sSampleDmaReuseQueueTail2++;
                }
                dma->ttl = 60;
                *dmaIndexRef = (u8) i;
                return &dma->buffer[(devAddr - dma->source)];
            }
        }

        if ((sSampleDmaReuseQueueTail2 != sSampleDmaReuseQueueHead2) && (arg2 != 0)) {
            // Allocate a DMA from reuse queue 2. This queue can be empty, since
            // TTL 60 is pretty large.
            dmaIndex = sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2++];
            dma = &sSampleDmas[dmaIndex];
            hasDma = true;
        }
    } else {
        dma = &sSampleDmas[*dmaIndexRef];
        for (i = 0; i < sSampleDmaListSize1; dma = &sSampleDmas[i++]) {
            bufferPos = devAddr - dma->source;
            if (0 <= bufferPos && (size_t) bufferPos <= dma->bufSize - size) {
                // We already have DMA for this memory range.
                if (dma->ttl == 0) {
                    // Move the DMA out of the reuse queue, by swapping it with the
                    // tail, and then incrementing the tail.
                    if (dma->reuseIndex != sSampleDmaReuseQueueTail1) {
                        if (1) {}
                        sSampleDmaReuseQueue1[dma->reuseIndex] = sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1];
                        sSampleDmas[sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1]].reuseIndex = dma->reuseIndex;
                    }
                    sSampleDmaReuseQueueTail1++;
                }
                dma->ttl = 2;
                return dma->buffer + (devAddr - dma->source);
            }
        }
    }

    if (!hasDma) {
        // Allocate a DMA from reuse queue 1. This queue will hopefully never
        // be empty, since TTL 2 is so small.
        dmaIndex = sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1++];
        dma = &sSampleDmas[dmaIndex];
        hasDma = true;
    }

    transfer = dma->bufSize;
    dmaDevAddr = devAddr & ~0xF;
    dma->ttl = 2;
    dma->source = dmaDevAddr;
    dma->sizeUnused = transfer;
    dma->buffer = (void*) dmaDevAddr;
    // osPiStartDma(&gCurrAudioFrameDmaIoMesgBufs[gCurrAudioFrameDmaCount++], OS_MESG_PRI_NORMAL, OS_READ, dmaDevAddr,
    //              dma->buffer, transfer, &gCurrAudioFrameDmaQueue);
    *dmaIndexRef = dmaIndex;
    return (devAddr - dmaDevAddr) + dma->buffer;
}

// init_sample_dma_buffers
void func_800BB030(UNUSED s32 arg0) {
    s32 i;
#define j i

    D_803B70A8 = 0x5A0;

    for (i = 0; i < gMaxSimultaneousNotes * 3 * gAudioBufferParameters.presetUnk4; i++) {
        sSampleDmas[gSampleDmaNumListItems].buffer = soundAlloc(&gNotesAndBuffersPool, D_803B70A8);
        if (sSampleDmas[gSampleDmaNumListItems].buffer == NULL) {
            break;
        }
        sSampleDmas[gSampleDmaNumListItems].bufSize = D_803B70A8;
        sSampleDmas[gSampleDmaNumListItems].source = 0;
        sSampleDmas[gSampleDmaNumListItems].sizeUnused = 0;
        sSampleDmas[gSampleDmaNumListItems].unused2 = 0;
        sSampleDmas[gSampleDmaNumListItems].ttl = 0;
        gSampleDmaNumListItems++;
    }

    for (i = 0; (u32) i < gSampleDmaNumListItems; i++) {
        sSampleDmaReuseQueue1[i] = (u8) i;
        sSampleDmas[i].reuseIndex = (u8) i;
    }

    for (j = gSampleDmaNumListItems; j < 0x100; j++) {
        sSampleDmaReuseQueue1[j] = 0;
    }

    sSampleDmaReuseQueueTail1 = 0;
    sSampleDmaReuseQueueHead1 = (u8) gSampleDmaNumListItems;
    sSampleDmaListSize1 = gSampleDmaNumListItems;

    D_803B70A8 = 0x180;
    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        sSampleDmas[gSampleDmaNumListItems].buffer = soundAlloc(&gNotesAndBuffersPool, D_803B70A8);
        if (sSampleDmas[gSampleDmaNumListItems].buffer == NULL) {
            break;
        }
        sSampleDmas[gSampleDmaNumListItems].bufSize = D_803B70A8;
        sSampleDmas[gSampleDmaNumListItems].source = 0;
        sSampleDmas[gSampleDmaNumListItems].sizeUnused = 0;
        sSampleDmas[gSampleDmaNumListItems].unused2 = 0;
        sSampleDmas[gSampleDmaNumListItems].ttl = 0;
        gSampleDmaNumListItems++;
    }

    for (i = sSampleDmaListSize1; (u32) i < gSampleDmaNumListItems; i++) {
        sSampleDmaReuseQueue2[i - sSampleDmaListSize1] = (u8) i;
        sSampleDmas[i].reuseIndex = (u8) (i - sSampleDmaListSize1);
    }

    // This probably meant to touch the range size1..size2 as well... but it
    // doesn't matter, since these values are never read anyway.
    for (j = gSampleDmaNumListItems; j < 0x100; j++) {
        sSampleDmaReuseQueue2[j] = sSampleDmaListSize1;
    }

    sSampleDmaReuseQueueTail2 = 0;
    sSampleDmaReuseQueueHead2 = gSampleDmaNumListItems - sSampleDmaListSize1;
#undef j
}

// Similar to patch_sound, but not really
s32 func_800BB304(struct AudioBankSample* sample) {
    // struct AudioBankSample *sample = sound->sample;
    UNUSED u8* mem;

    if (sample == (void*) NULL) {
        return -1;
    }

    if (sample->loaded == 1) {
        // temp_a1 = sound->sampleAddr // unk10;
        mem = soundAlloc(&gNotesAndBuffersPool, sample->sampleSize);
        // temp_a1_2 = temp_v0;
        if (mem == (void*) NULL) {
            return -1;
        }
        audio_dma_copy_immediate(sample->sampleAddr, mem, sample->sampleSize);
        sample->loaded = 0x81;
        sample->sampleAddr = mem; // sound->unk4
    }
    return 0;
}

s32 func_800BB388(s32 bankId, s32 instId, s32 arg2) {
    struct Instrument* instr;
    struct Drum* drum;

    if (instId < 0x7F) {
        instr = get_instrument_inner(bankId, instId);
        if (instr == NULL) {
            return -1;
        }
        if (instr->normalRangeLo != 0) {
            func_800BB304(instr->lowNotesSound.sample);
        }
        func_800BB304(instr->normalNotesSound.sample);
        if (instr->normalRangeHi != 0x7F) {
            func_800BB304(instr->highNotesSound.sample);
        }
        //! @bug missing return
    } else if (instId == 0x7F) {
        drum = get_drum(bankId, arg2);
        if (drum == NULL) {
            return -1;
        }
        func_800BB304(drum->sound.sample);
        return 0;
    }
#ifdef AVOID_UB
    return 0;
#endif
}

// This appears to be a modified version of alSeqFileNew
// from src/os/alBankNew.c
// Or maybe its patch_seq_file from SM64's load_sh.c?
void func_800BB43C(ALSeqFile* f, u8* base) {
#define PATCH(SRC, BASE, TYPE) SRC = (TYPE) ((uintptr_t) SRC + (uintptr_t) BASE)
    int i;
    u8* wut = base;
    for (i = 0; i < f->seqCount; i++) {
        if (f->seqArray[i].len != 0) {
            PATCH(f->seqArray[i].offset, wut, u8*);
        }
    }
#undef PATCH
}

void patch_sound(struct AudioBankSound* sound, u8* memBase, u8* offsetBase) {
    struct AudioBankSample* sample;
    void* patched;
    u8* mem;

#define PATCH(x, base) (patched = (void*) ((uintptr_t) (x) + (uintptr_t) base))

    if (sound->sample != NULL) {
        sample = sound->sample = (struct AudioBankSample*) PATCH(sound->sample, memBase);
        if (sample->loaded == 0) {
            sample->sampleAddr = (u8*) PATCH(sample->sampleAddr, offsetBase);
            sample->loop = (struct AdpcmLoop*) PATCH(sample->loop, memBase);
            sample->book = (struct AdpcmBook*) PATCH(sample->book, memBase);
            sample->loaded = 1;
        } else if (sample->loaded == 0x80) {
            PATCH(sample->sampleAddr, offsetBase);
            mem = soundAlloc(&gNotesAndBuffersPool, sample->sampleSize);
            if (mem == NULL) {
                sample->sampleAddr = (u8*) patched;
                sample->loaded = 1;
            } else {
                audio_dma_copy_immediate((u8*) patched, mem, sample->sampleSize);
                sample->loaded = 0x81;
                sample->sampleAddr = mem;
            }
            sample->loop = (struct AdpcmLoop*) PATCH(sample->loop, memBase);
            sample->book = (struct AdpcmBook*) PATCH(sample->book, memBase);
        }
    }

#undef PATCH
}

// There does not appear to an SM64 counterpart to this function
void func_800BB584(s32 bankId) {
    u8* var_a1;

    if (gAlTbl->seqArray[bankId].len == 0) {
        var_a1 = gAlTbl->seqArray[(uintptr_t) gAlTbl->seqArray[bankId].offset].offset;
    } else {
        var_a1 = gAlTbl->seqArray[bankId].offset;
    }
    // wtf is up with the `gCtlEntries[bankId].instruments - 1` stuff?
    patch_audio_bank((struct AudioBank*) (gCtlEntries[bankId].instruments - 1), var_a1,
                     gCtlEntries[bankId].numInstruments, gCtlEntries[bankId].numDrums);
    gCtlEntries[bankId].drums = (struct Drum**) *(gCtlEntries[bankId].instruments - 1);
}

void patch_audio_bank(struct AudioBank* mem, u8* offset, u32 numInstruments, u32 numDrums) {
    struct Instrument* instrument;
    struct Instrument** itInstrs;
    struct Instrument** end;
    struct AudioBank* temp;
    u32 i;
    void* patched;
    struct Drum* drum;
    struct Drum** drums;
    u32 numDrums2;

#define BASE_OFFSET_REAL(x, base) (void*) ((uintptr_t) (x) + (uintptr_t) base)
#define PATCH(x, base) (patched = BASE_OFFSET_REAL(x, base))
#define PATCH_MEM(x) x = PATCH(x, mem)

#define BASE_OFFSET(x, base) BASE_OFFSET_REAL(base, x)

    drums = mem->drums;
    numDrums2 = numDrums;
    if (drums != NULL && numDrums2 > 0) {
        mem->drums = PATCH(drums, mem);
        for (i = 0; i < numDrums2; i++) {
            patched = mem->drums[i];
            if (patched != NULL) {
                drum = PATCH(patched, mem);
                mem->drums[i] = drum;
                if (drum->loaded == 0) {
                    patch_sound(&drum->sound, (u8*) mem, offset);
                    patched = drum->envelope;
                    drum->envelope = BASE_OFFSET(mem, patched);
                    drum->loaded = 1;
                }
            }
        }
    }

    //! Doesn't affect EU, but required for US/JP
    temp = &*mem;
    if (numInstruments > 0) {
        //! Doesn't affect EU, but required for US/JP
        struct Instrument** tempInst;
        itInstrs = temp->instruments;
        tempInst = temp->instruments;
        end = numInstruments + tempInst;

        do {
            if (*itInstrs != NULL) {
                *itInstrs = BASE_OFFSET(*itInstrs, mem);
                instrument = *itInstrs;

                if (instrument->loaded == 0) {
                    patch_sound(&instrument->lowNotesSound, (u8*) mem, offset);
                    patch_sound(&instrument->normalNotesSound, (u8*) mem, offset);
                    patch_sound(&instrument->highNotesSound, (u8*) mem, offset);
                    patched = instrument->envelope;
                    instrument->envelope = BASE_OFFSET(mem, patched);
                    instrument->loaded = 1;
                }
            }
            itInstrs++;
        } while (end != itInstrs);
    }
#undef PATCH_MEM
#undef PATCH
#undef BASE_OFFSET_REAL
#undef BASE_OFFSET
}

struct AudioBank* bank_load_immediate(s32 bankId, s32 arg1) {
    s32 alloc;
    UNUSED s32 stackPadding0[9];
    struct AudioBank* ret;
    u8* ctlData;

    alloc = gAlCtlHeader->seqArray[bankId].len + 0xf;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    ctlData = gAlCtlHeader->seqArray[bankId].offset;
    ret = alloc_bank_or_seq(&gBankLoadedPool, 1, alloc, arg1, bankId);
    if (ret == NULL) {
        return NULL;
    }
    audio_dma_copy_immediate(ctlData + 0x10, ret, (u32) alloc);
    gCtlEntries[bankId].instruments = ret->instruments;
    func_800BB584(bankId);
    if (gBankLoadStatus[bankId] != 5) {
        gBankLoadStatus[bankId] = 2;
    }
    return ret;
}

struct AudioBank* bank_load_async(s32 bankId, s32 arg1, struct SequencePlayer* seqPlayer) {
    size_t alloc;
    UNUSED s32 stackPadding0[9];
    struct AudioBank* ret;
    u8* ctlData;
    UNUSED s32 stackPadding1[2];

    alloc = gAlCtlHeader->seqArray[bankId].len + 0xF;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    ctlData = gAlCtlHeader->seqArray[bankId].offset;
    ret = alloc_bank_or_seq(&gBankLoadedPool, 1, alloc, arg1, bankId);
    if (ret == NULL) {
        return NULL;
    }
    seqPlayer->loadingBankId = bankId;
    gCtlEntries[bankId].instruments = ret->instruments;
    gCtlEntries[bankId].drums = NULL;
    seqPlayer->bankDmaCurrMemAddr = (u8*) ret;
    seqPlayer->bankDmaCurrDevAddr = (uintptr_t) (ctlData + 0x10);
    seqPlayer->bankDmaRemaining = alloc;
    if (1) {}
    osCreateMesgQueue(&seqPlayer->bankDmaMesgQueue, &seqPlayer->bankDmaMesg, 1);
    seqPlayer->bankDmaInProgress = true;
    audio_dma_partial_copy_async(&seqPlayer->bankDmaCurrDevAddr, &seqPlayer->bankDmaCurrMemAddr,
                                 &seqPlayer->bankDmaRemaining, &seqPlayer->bankDmaMesgQueue, &seqPlayer->bankDmaIoMesg);
    if (gBankLoadStatus[bankId] != 5) {
        gBankLoadStatus[bankId] = 1;
    }
    return ret;
}

void* sequence_dma_immediate(s32 seqId, s32 arg1) {
    s32 seqLength;
    void* ptr;
    u8* seqData;

    seqLength = gSeqFileHeader->seqArray[seqId].len;
    seqLength = ALIGN16(seqLength);
    seqData = gSeqFileHeader->seqArray[seqId].offset;
    ptr = alloc_bank_or_seq(&gSeqLoadedPool, 1, seqLength, arg1, seqId);
    if (ptr == NULL) {
        return NULL;
    }

    audio_dma_copy_immediate(seqData, ptr, seqLength);
    if (gSeqLoadStatus[seqId] != 5) {
        gSeqLoadStatus[seqId] = 2;
    }
    return ptr;
}

void* sequence_dma_async(s32 seqId, s32 arg1, struct SequencePlayer* seqPlayer) {
    s32 seqLength;
    void* ptr;
    u8* seqData;
    OSMesgQueue* mesgQueue;

    seqLength = gSeqFileHeader->seqArray[seqId].len;
    seqLength = ALIGN16(seqLength);
    seqData = gSeqFileHeader->seqArray[seqId].offset;
    ptr = alloc_bank_or_seq(&gSeqLoadedPool, 1, seqLength, arg1, seqId);
    if (ptr == NULL) {
        return NULL;
    }
    if (seqLength < 0x41) {
        audio_dma_copy_immediate(seqData, ptr, (u32) seqLength);
        if (1) {}
        if (gSeqLoadStatus[seqId] != 5) {
            gSeqLoadStatus[seqId] = 2;
        }
    } else {
        audio_dma_copy_immediate(seqData, ptr, 0x00000040U);
        mesgQueue = &seqPlayer->seqDmaMesgQueue;
        osCreateMesgQueue(mesgQueue, &seqPlayer->seqDmaMesg, 1);
        seqPlayer->seqDmaInProgress = true;
        audio_dma_copy_async((uintptr_t) (seqData + 0x40), (u8*) ptr + 0x40, seqLength - 0x40, mesgQueue,
                             &seqPlayer->seqDmaIoMesg);
        if (gSeqLoadStatus[seqId] != 5) {
            gSeqLoadStatus[seqId] = 1;
        }
    }
    return ptr;
}

uint8_t* load_sequence_immediate(s32 seqId, s32 arg1) {
    return GameEngine_LoadSequence(seqId)->data;
}

struct CtlEntry* load_banks_immediate(s32 seqId, u8* outDefaultBank) {
    u32 bankId;
    struct AudioSequenceData* seqData = GameEngine_LoadSequence(seqId);
    struct CtlEntry* output;
    for (size_t i = 0; i < seqData->bankCount; i++) {
        output = GameEngine_LoadBank(bankId = seqData->banks[i]);
    }
    *outDefaultBank = bankId;
    return output;
}

void preload_sequence(u32 seqId, u8 preloadMask) {
    void* sequenceData;
    u8 temp;

    if (seqId >= gSequenceCount) {
        return;
    }

    // if (gSeqFileHeader->seqArray[seqId].len == 0) {
    //     seqId = (u32) gSeqFileHeader->seqArray[seqId].offset;
    // }

    gAudioLoadLock = AUDIO_LOCK_LOADING;
    if (preloadMask & PRELOAD_BANKS) {
        load_banks_immediate(seqId, &temp);
    }

    if (preloadMask & PRELOAD_SEQUENCE) {
        //! @bug should be IS_SEQ_LOAD_COMPLETE
        if (IS_SEQ_LOAD_COMPLETE(seqId) == true) {
            sequenceData = load_sequence_immediate(seqId, 2);
        } else {
            sequenceData = NULL;
        }
        if (sequenceData == NULL && sequence_dma_immediate(seqId, 2) == NULL) {
            gAudioLoadLock = AUDIO_LOCK_NOT_LOADING;
            return;
        }
    }

    gAudioLoadLock = AUDIO_LOCK_NOT_LOADING;
}

void load_sequence(u32 player, u32 seqId, s32 loadAsync) {
    if (!loadAsync) {
        gAudioLoadLock = AUDIO_LOCK_LOADING;
    }
    load_sequence_internal(player, seqId, loadAsync);
    if (!loadAsync) {
        gAudioLoadLock = AUDIO_LOCK_NOT_LOADING;
    }
}

void load_sequence_internal(u32 player, u32 seqId, s32 loadAsync) {
    struct SequencePlayer* seqPlayer = &gSequencePlayers[player];

    if (seqId >= gSequenceCount) {
        return;
    }

    sequence_player_disable(seqPlayer);
    struct CtlEntry* bank = load_banks_immediate(seqId, &seqPlayer->defaultBank[0]);

    if (bank == NULL) {
        return;
    }

    eu_stubbed_printf_2("Seq %d:Default Load Id is %d\n", seqId, seqPlayer->defaultBank[0]);
    eu_stubbed_printf_0("Seq Loading Start\n");

    uint8_t* sequenceData = load_sequence_immediate(seqId, 2);
    if (sequenceData == NULL) {
        return;
    }

    seqPlayer->seqId = seqId;
    init_sequence_player(player);
    seqPlayer->scriptState.depth = 0;
    seqPlayer->delay = 0;
    seqPlayer->enabled = 1;
    seqPlayer->seqData = sequenceData;
    seqPlayer->scriptState.pc = sequenceData;
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/5FBUM
//  There is some wild bullshit going on in this function
//  It is an unholy cross between SM64's EU and Shindou
//  verions of this function, with the for loop towards
//  the bottom resembling stuff from bank_load_async
extern u8 _audio_banksSegmentRomStart;
extern u8 _audio_tablesSegmentRomStart;
extern u8 _instrument_setsSegmentRomStart;
extern u8 _sequencesSegmentRomStart;

void audio_init(void) {
    s32 i;
    UNUSED s32 pad[2];
    s32 j;
    UNUSED s32 pad2[6];
    u32 sp60[2];
    UNUSED s32 pad1[2];
    s32 aaa;
    s32 size;
    UNUSED u64* ptr64;
    UNUSED s32 one = 1;
    u8* test;

    gAudioLoadLock = 0;

#ifdef TARGET_N64
    for (i = 0; i < gAudioHeapSize / 8; i++) {
        ((u64*) gAudioHeap)[i] = 0;
    }

    // It seems boot.s doesn't clear the .bss area for audio, so do it here.
    ptr64 = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    for (i = ((uintptr_t) &gAudioGlobalsEndMarker -
              (uintptr_t) ((u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer)))) /
             8;
         i >= 0; i--) {
        *ptr64++ = 0;
    }
#else
    memset(gAudioHeap, 0, gAudioHeapSize);
#endif

    // UTODO: Which is the correct one?
    D_803B7178 = 16.713f;
    gRefreshRate = 60;
    port_eu_init();
    for (i = 0; i < NUMAIBUFFERS; i++) {
        gAiBufferLengths[i] = 0xa0;
    }

    gAudioTaskIndex = gAudioFrameCount = 0;
    gCurrAiBufferIndex = 0;
    gAudioLibSoundMode = 0;
    gAudioTask = NULL;
    gAudioTasks[0].task.t.data_size = 0;
    gAudioTasks[1].task.t.data_size = 0;
    osCreateMesgQueue(&D_803B6720, &D_803B6738, 1);
    osCreateMesgQueue(&gCurrAudioFrameDmaQueue, gCurrAudioFrameDmaMesgBufs, ARRAY_COUNT(gCurrAudioFrameDmaMesgBufs));
    gCurrAudioFrameDmaCount = 0;
    gSampleDmaNumListItems = 0;

    sound_init_main_pools(gAudioInitPoolSize);

    for (i = 0; i < NUMAIBUFFERS; i++) {
        gAiBuffers[i] = soundAlloc(&gAudioInitPool, AIBUFFER_LEN);

        for (j = 0; j < (s32) (AIBUFFER_LEN / sizeof(s16)); j++) {
            gAiBuffers[i][j] = 0;
        }
    }

    gAudioResetPresetIdToLoad = 0;
    gAudioResetStatus = one;
    audio_shut_down_and_reset_step();
    gSequenceCount = GameEngine_GetSequenceCount();
#ifdef TARGET_N64
    gSeqFileHeader = (ALSeqFile*) sp60;
    test = &_sequencesSegmentRomStart;
    audio_dma_copy_immediate(test, gSeqFileHeader, 0x00000010U);
    gSequenceCount = gSeqFileHeader->seqCount;
    size = gSequenceCount * sizeof(ALSeqData) + 4;
    size = ALIGN16(size);
    gSeqFileHeader = soundAlloc(&gAudioInitPool, size);
    audio_dma_copy_immediate(test, gSeqFileHeader, size);
    func_800BB43C(gSeqFileHeader, test);
    gAlCtlHeader = (ALSeqFile*) sp60;
    test = &_audio_banksSegmentRomStart;
    audio_dma_copy_immediate(test, gAlCtlHeader, 0x00000010U);
    aaa = gAlCtlHeader->seqCount;
    size = ALIGN16(aaa * sizeof(ALSeqData) + 4);
    gAlCtlHeader = soundAlloc(&gAudioInitPool, size);
    audio_dma_copy_immediate(test, gAlCtlHeader, size);
    func_800BB43C(gAlCtlHeader, test);
    gCtlEntries = soundAlloc(&gAudioInitPool, aaa * 0xC);
    for (i = 0; i < aaa; i++) {
        audio_dma_copy_immediate(gAlCtlHeader->seqArray[i].offset, sp60, 0x00000010U);

        gCtlEntries[i].numInstruments = sp60[0];
        gCtlEntries[i].numDrums = sp60[1];
    }
    gAlTbl = (ALSeqFile*) sp60;
    test = &_audio_tablesSegmentRomStart;
    audio_dma_copy_immediate(test, gAlTbl, 0x00000010U);
    size = gAlTbl->seqCount * sizeof(ALSeqData) + 4;
    size = ALIGN16(size);
    gAlTbl = soundAlloc(&gAudioInitPool, size);
    audio_dma_copy_immediate(test, gAlTbl, size);
    func_800BB43C(gAlTbl, test);
    gAlBankSets = soundAlloc(&gAudioInitPool, 0x00000100U);
    audio_dma_copy_immediate((u32) &_instrument_setsSegmentRomStart, gAlBankSets, 0x00000100U);
#endif
    sound_alloc_pool_init(&gUnkPool1.pool, soundAlloc(&gAudioInitPool, (u32) D_800EA5D8), (u32) D_800EA5D8);
    init_sequence_players();
    gAudioLoadLock = 0x76557364;

    audio_set_player_volume(SEQ_PLAYER_LEVEL, CVarGetFloat("gMainMusicVolume", 1.0f));
    audio_set_player_volume(SEQ_PLAYER_ENV, CVarGetFloat("gEnvironmentVolume", 1.0f));
    audio_set_player_volume(SEQ_PLAYER_SFX, CVarGetFloat("gSFXMusicVolume", 1.0f));
}
#else
#ifdef VERSION_EU
GLOBAL_ASM("asm/eu_nonmatchings/audio_init.s")
#else
GLOBAL_ASM("asm/non_matchings/audio/load/audio_init.s")
#endif
#endif
