#include <libultraship.h>
#include "mixer.h"
#include <macros.h>
#include "audio/synthesis.h"
#include "audio/heap.h"
#include "audio/data.h"
#include "audio/load.h"
#include "audio/seqplayer.h"
#include "audio/internal.h"
#include "port/Engine.h"
#include <libultra/abi.h>

#define aSetLoadBufferPair(pkt, c, off)                                               \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_LEFT_CH, 0, DEFAULT_LEN_1CH - c);            \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.left + (off))); \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_RIGHT_CH, 0, DEFAULT_LEN_1CH - c);           \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.right + (off)))

#define aSetSaveBufferPair(pkt, c, d, off)                                            \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_LEFT_CH, d);                              \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.left + (off))); \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_RIGHT_CH, d);                             \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.right + (off)));

struct VolumeChange {
    u16 sourceLeft;
    u16 sourceRight;
    u16 targetLeft;
    u16 targetRight;
};

u64* process_envelope(u64* cmd, struct NoteSubEu* noteSubEu, struct NoteSynthesisState* synthesisState, s32 nSamples,
                      u16 inBuf, s32 headsetPanSettings, u32 flags);

struct SynthesisReverb gSynthesisReverbs[4];
u8 sAudioSynthesisPad[0x10];

// s16 gVolume;
// s8 gUseReverb;
// s8 gNumSynthesisReverbs;
// struct NoteSubEu *gNoteSubsEu;

// f32 gLeftVolRampings;
// f32 gRightVolRampings[3][1024];
// f32 *gCurrentLeftVolRamping; // Points to any of the three left buffers above
// f32 *gCurrentRightVolRamping; // Points to any of the three right buffers above

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be used in some sort of printf variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char synthesisAudioString0[] = "Terminate-Canceled Channel %d,Phase %d\n";
char synthesisAudioString1[] = "Copy %d\n";
char synthesisAudioString2[] = "%d->%d\n";
char synthesisAudioString3[] = "pitch %x: delaybytes %d : olddelay %d\n";
char synthesisAudioString4[] = "cont %x: delaybytes %d : olddelay %d\n";

// Equivalent functionality as the US/JP version,
// just that the reverb structure is chosen from an array with index
void prepare_reverb_ring_buffer(s32 chunkLen, u32 updateIndex, s32 reverbIndex) {
    struct ReverbRingBufferItem* item;
    struct SynthesisReverb* reverb = &gSynthesisReverbs[reverbIndex];
    s32 srcPos;
    s32 dstPos;
    s32 nSamples;
    s32 excessiveSamples;
    s32 UNUSED pad[3];
    if (reverb->downsampleRate != 1) {
        if (reverb->framesLeftToIgnore == 0) {
            // Now that the RSP has finished, downsample the samples produced two frames ago by skipping
            // samples.
            item = &reverb->items[reverb->curFrame][updateIndex];

            // Touches both left and right since they are adjacent in memory
            osInvalDCache(item->toDownsampleLeft, 0x300);

            for (srcPos = 0, dstPos = 0; dstPos < item->lengthA / 2; srcPos += reverb->downsampleRate, dstPos++) {
                reverb->ringBuffer.left[item->startPos + dstPos] = item->toDownsampleLeft[srcPos];
                reverb->ringBuffer.right[item->startPos + dstPos] = item->toDownsampleRight[srcPos];
            }
            for (dstPos = 0; dstPos < item->lengthB / 2; srcPos += reverb->downsampleRate, dstPos++) {
                reverb->ringBuffer.left[dstPos] = item->toDownsampleLeft[srcPos];
                reverb->ringBuffer.right[dstPos] = item->toDownsampleRight[srcPos];
            }
        }
    }

    item = &reverb->items[reverb->curFrame][updateIndex];
    nSamples = chunkLen / reverb->downsampleRate;
    excessiveSamples = (nSamples + reverb->nextRingBufferPos) - reverb->bufSizePerChannel;
    if (excessiveSamples < 0) {
        // There is space in the ring buffer before it wraps around
        item->lengthA = nSamples * 2;
        item->lengthB = 0;
        item->startPos = (s32) reverb->nextRingBufferPos;
        reverb->nextRingBufferPos += nSamples;
    } else {
        // Ring buffer wrapped around
        item->lengthA = (nSamples - excessiveSamples) * 2;
        item->lengthB = excessiveSamples * 2;
        item->startPos = reverb->nextRingBufferPos;
        reverb->nextRingBufferPos = excessiveSamples;
    }
    // These fields are never read later
    item->numSamplesAfterDownsampling = nSamples;
    item->chunkLen = chunkLen;
}

Acmd* synthesis_load_reverb_ring_buffer(Acmd* acmd, u16 addr, u16 srcOffset, s32 len, s32 reverbIndex) {
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(&gSynthesisReverbs[reverbIndex].ringBuffer.left[srcOffset]), addr, len);
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(&gSynthesisReverbs[reverbIndex].ringBuffer.right[srcOffset]), addr + 0x180,
                len);
    return acmd;
}

Acmd* synthesis_save_reverb_ring_buffer(Acmd* acmd, u16 addr, u16 destOffset, s32 len, s32 reverbIndex) {
    aSaveBuffer(acmd++, addr, VIRTUAL_TO_PHYSICAL2(&gSynthesisReverbs[reverbIndex].ringBuffer.left[destOffset]), len);
    aSaveBuffer(acmd++, addr + 0x180,
                VIRTUAL_TO_PHYSICAL2(&gSynthesisReverbs[reverbIndex].ringBuffer.right[destOffset]), len);
    return acmd;
}

void func_800B6FB4(s32 updateIndexStart, s32 noteIndex) {
    s32 i;

    for (i = updateIndexStart + 1; i < gAudioBufferParameters.updatesPerFrame; i++) {
        if (!gNoteSubsEu[gMaxSimultaneousNotes * i + noteIndex].needsInit) {
            gNoteSubsEu[gMaxSimultaneousNotes * i + noteIndex].enabled = false;
        } else {
            break;
        }
    }
}

void synthesis_load_note_subs_eu(s32 updateIndex) {
    struct NoteSubEu* src;
    struct NoteSubEu* dest;
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        src = &gNotes[i].noteSubEu;
        dest = &gNoteSubsEu[gMaxSimultaneousNotes * updateIndex + i];
        if (src->enabled) {
            *dest = *src;
            src->needsInit = false;
        } else {
            dest->enabled = false;
        }
    }
}

Acmd* synthesis_execute(Acmd* acmd, s32* writtenCmds, s16* aiBuf, s32 bufLen) {
    s32 i, j;
    u32* aiBufPtr;
    Acmd* cmd = acmd;
    s32 chunkLen;

    for (i = gAudioBufferParameters.updatesPerFrame; i > 0; i--) {
        process_sequences(i - 1);
        synthesis_load_note_subs_eu(gAudioBufferParameters.updatesPerFrame - i);
    }
    aSegment(cmd++, 0, 0);
    aiBufPtr = (u32*) aiBuf;
    for (i = gAudioBufferParameters.updatesPerFrame; i > 0; i--) {
        if (i == 1) {
            chunkLen = bufLen;
        } else {
            if (bufLen / i >= gAudioBufferParameters.samplesPerUpdateMax) {
                chunkLen = gAudioBufferParameters.samplesPerUpdateMax;
            } else if (bufLen / i <= gAudioBufferParameters.samplesPerUpdateMin) {
                chunkLen = gAudioBufferParameters.samplesPerUpdateMin;
            } else {
                chunkLen = gAudioBufferParameters.samplesPerUpdate;
            }
        }
        for (j = 0; j < gNumSynthesisReverbs; j++) {
            if (gSynthesisReverbs[j].useReverb != 0) {
                prepare_reverb_ring_buffer(chunkLen, gAudioBufferParameters.updatesPerFrame - i, j);
            }
        }
        cmd = synthesis_do_one_audio_update((s16*) aiBufPtr, chunkLen, cmd, gAudioBufferParameters.updatesPerFrame - i);
        bufLen -= chunkLen;
        aiBufPtr += chunkLen;
    }

    for (j = 0; j < gNumSynthesisReverbs; j++) {
        if (gSynthesisReverbs[j].framesLeftToIgnore != 0) {
            gSynthesisReverbs[j].framesLeftToIgnore--;
        }
        gSynthesisReverbs[j].curFrame ^= 1;
    }
    *writtenCmds = cmd - acmd;
    return cmd;
}

Acmd* synthesis_resample_and_mix_reverb(Acmd* acmd, s32 bufLen, s16 reverbIndex, s16 updateIndex) {
    struct ReverbRingBufferItem* item;
    s16 startPad;
    s16 paddedLengthA;

    item = &gSynthesisReverbs[reverbIndex].items[gSynthesisReverbs[reverbIndex].curFrame][updateIndex];
    aClearBuffer(acmd++, 0x840, 0x300);
    if (gSynthesisReverbs[reverbIndex].downsampleRate == 1) {

        acmd = synthesis_load_reverb_ring_buffer(acmd, 0x840, item->startPos, item->lengthA, reverbIndex);
        if (item->lengthB != 0) {
            acmd = synthesis_load_reverb_ring_buffer(acmd, item->lengthA + 0x840, 0U, item->lengthB, reverbIndex);
        }

        aMix(acmd++, 0x7fff, 0x840, 0x540, 0x300);
        aMix(acmd++, 0x8000 + gSynthesisReverbs[reverbIndex].reverbGain, 0x840, 0x840, 0x300);
    } else {
        startPad = (item->startPos % 8U) * 2;
        paddedLengthA = ALIGN(startPad + item->lengthA, 4);

        acmd =
            synthesis_load_reverb_ring_buffer(acmd, 0x0020, item->startPos - (startPad / 2), 0x00000180, reverbIndex);
        if (item->lengthB != 0) {
            acmd = synthesis_load_reverb_ring_buffer(acmd, paddedLengthA + 0x20, 0, 0x180 - paddedLengthA, reverbIndex);
        }

        aSetBuffer(acmd++, 0, 0x20 + startPad, 0x840, bufLen * 2);
        aResample(acmd++, gSynthesisReverbs[reverbIndex].resampleFlags, gSynthesisReverbs[reverbIndex].resampleRate,
                  VIRTUAL_TO_PHYSICAL2(gSynthesisReverbs[reverbIndex].resampleStateLeft));
        aSetBuffer(acmd++, 0, 0x1A0 + startPad, 0x9C0, bufLen * 2);
        aResample(acmd++, gSynthesisReverbs[reverbIndex].resampleFlags, gSynthesisReverbs[reverbIndex].resampleRate,
                  VIRTUAL_TO_PHYSICAL2(gSynthesisReverbs[reverbIndex].resampleStateRight));
        aMix(acmd++, 0x7fff, 0x840, 0x540, 0x300);
        aMix(acmd++, 0x8000 + gSynthesisReverbs[reverbIndex].reverbGain, 0x840, 0x840, 0x300);
    }
    return acmd;
}

Acmd* synthesis_save_reverb_samples(Acmd* acmd, s16 reverbIndex, s16 updateIndex) {
    struct ReverbRingBufferItem* item;

    item = &gSynthesisReverbs[reverbIndex].items[gSynthesisReverbs[reverbIndex].curFrame][updateIndex];
    if (gSynthesisReverbs[reverbIndex].useReverb != 0) {
        switch (gSynthesisReverbs[reverbIndex].downsampleRate) {
            case 1:
                acmd = synthesis_save_reverb_ring_buffer(acmd, 0x840, item->startPos, item->lengthA, reverbIndex);
                if (item->lengthB != 0) {
                    acmd =
                        synthesis_save_reverb_ring_buffer(acmd, 0x840 + item->lengthA, 0, item->lengthB, reverbIndex);
                }
                break;
            default:
                aSaveBuffer(acmd++, 0x840,
                            VIRTUAL_TO_PHYSICAL2(gSynthesisReverbs[reverbIndex]
                                                     .items[gSynthesisReverbs[reverbIndex].curFrame][updateIndex]
                                                     .toDownsampleLeft),
                            0x300);
                gSynthesisReverbs[reverbIndex].resampleFlags = 0;
                break;
        }
    }
    return acmd;
}

Acmd* synthesis_do_one_audio_update(s16* aiBuf, s32 bufLen, Acmd* acmd, s32 updateIndex) {
    struct NoteSubEu* noteSubEu;
    u8 noteIndices[56];
    s32 temp;
    s32 i;
    s16 j;
    s16 notePos = 0;

    if (gNumSynthesisReverbs == 0) {
        for (i = 0; i < gMaxSimultaneousNotes; i++) {
            if (gNoteSubsEu[gMaxSimultaneousNotes * updateIndex + i].enabled) {
                noteIndices[notePos++] = i;
            }
        }
    } else {
        for (j = 0; j < gNumSynthesisReverbs; j++) {
            for (i = 0; i < gMaxSimultaneousNotes; i++) {
                noteSubEu = &gNoteSubsEu[gMaxSimultaneousNotes * updateIndex + i];
                if (noteSubEu->enabled && j == noteSubEu->reverbIndex) {
                    noteIndices[notePos++] = i;
                }
            }
        }

        for (i = 0; i < gMaxSimultaneousNotes; i++) {
            noteSubEu = &gNoteSubsEu[gMaxSimultaneousNotes * updateIndex + i];
            if (noteSubEu->enabled && noteSubEu->reverbIndex >= gNumSynthesisReverbs) {
                noteIndices[notePos++] = i;
            }
        }
    }
    aClearBuffer(acmd++, DMEM_ADDR_LEFT_CH, DEFAULT_LEN_2CH);
    i = 0;
    for (j = 0; j < gNumSynthesisReverbs; j++) {
        gUseReverb = gSynthesisReverbs[j].useReverb;
        if (gUseReverb != 0) {
            acmd = synthesis_resample_and_mix_reverb(acmd, bufLen, j, updateIndex);
        }
        for (; i < notePos; i++) {
            temp = updateIndex * gMaxSimultaneousNotes;
            if (j == gNoteSubsEu[temp + noteIndices[i]].reverbIndex) {
                acmd = synthesis_process_note(noteIndices[i], &gNoteSubsEu[temp + noteIndices[i]],
                                              &gNotes[noteIndices[i]].synthesisState, aiBuf, bufLen, acmd, updateIndex);
                continue;
            } else {
                break;
            }
        }
        if (gSynthesisReverbs[j].useReverb != 0) {
            acmd = synthesis_save_reverb_samples(acmd, j, updateIndex);
        }
    }
    for (; i < notePos; i++) {
        temp = updateIndex * gMaxSimultaneousNotes;
        if (IS_BANK_LOAD_COMPLETE(gNoteSubsEu[temp + noteIndices[i]].bankId) == true) {
            acmd = synthesis_process_note(noteIndices[i], &gNoteSubsEu[temp + noteIndices[i]],
                                          &gNotes[noteIndices[i]].synthesisState, aiBuf, bufLen, acmd, updateIndex);
        } else {
            gAudioErrorFlags = (gNoteSubsEu[temp + noteIndices[i]].bankId + (i << 8)) + 0x10000000;
        }
    }

    temp = bufLen * 2;
    aSetBuffer(acmd++, 0, 0, DMEM_ADDR_TEMP, temp);
    // UTODO: Stubbed
    aInterleave(acmd++, 0, DMEM_ADDR_LEFT_CH, DMEM_ADDR_RIGHT_CH, 0);
    aSaveBuffer(acmd++, DMEM_ADDR_TEMP, VIRTUAL_TO_PHYSICAL2(aiBuf), temp * 2);
    return acmd;
}

#ifdef NON_MATCHING
// generated by m2c commit beb457dabfc7a01ec6540a5404a6a05097a13602 on Nov-01-2023
Acmd* synthesis_process_note(s32 noteIndex, struct NoteSubEu* noteSubEu, struct NoteSynthesisState* synthesisState,
                             UNUSED s16* aiBuf, s32 inBuf, Acmd* cmd, s32 updateIndex) {
    UNUSED s32 pad[4];
    struct AudioBankSample* audioBookSample;
    s16* curLoadedBook;
    struct AdpcmLoop* loopInfo;
    s32 noteFinished;
    s32 restart;
    s32 flags;
    u16 resamplingRateFixedPoint;
    s32 nSamplesToLoad;

    s32 spFC = 0;
    UNUSED s32 pad2[2];

    s32 loopInfo_2;
    u8* sampleAddr;
    s32 a1;

    s32 samplesLenAdjusted;
    s32 nAdpcmSamplesProcessed;
    s32 endPos;
    s32 nSamplesToProcess;

    UNUSED s32 pad3;

    s32 s3;
    s32 leftRight;
    s32 s4;
    u32 samplesLenFixedPoint;
    s32 var_s6;
    s32 nSamplesInThisIteration;
    u32 var_t2;
    u8* var_a0_2;

    s32 s5Aligned;
    s32 temp_t6;
    s32 nParts;
    s32 curPart;
    s32 aligned;
    UNUSED s32 pad4;
    s32 resampledTempLen; /* compiler-managed */
    u16 noteSamplesDmemAddrBeforeResampling;
    UNUSED s32 pad6[1];
    struct Note* note;
    s16 addr;

    curLoadedBook = NULL;
    note = &gNotes[noteIndex];
    flags = 0;
    if (noteSubEu->needsInit == true) {
        flags = A_INIT;
        synthesisState->restart = 0;
        synthesisState->samplePosInt = 0;
        synthesisState->samplePosFrac = 0;
        synthesisState->curVolLeft = 0;
        synthesisState->curVolRight = 0;
        synthesisState->prevHeadsetPanRight = 0;
        synthesisState->prevHeadsetPanLeft = 0;
    }
    resamplingRateFixedPoint = noteSubEu->resamplingRateFixedPoint;

    nParts = noteSubEu->hasTwoAdpcmParts + 1;
    samplesLenFixedPoint = (resamplingRateFixedPoint * inBuf * 2) + synthesisState->samplePosFrac;
    nSamplesToLoad = (samplesLenFixedPoint >> 0x10);
    synthesisState->samplePosFrac = samplesLenFixedPoint & 0xFFFF; // m2c didn't have & 0xFFFF

    if (noteSubEu->isSyntheticWave) {
        cmd = load_wave_samples(cmd, noteSubEu, synthesisState, nSamplesToLoad);

        noteSamplesDmemAddrBeforeResampling = (synthesisState->samplePosInt * 2) + 0x1A0; // DMEM_ADDR_UNCOMPRESSED_NOTE
        synthesisState->samplePosInt += nSamplesToLoad;
    } else {
        audioBookSample = noteSubEu->sound.audioBankSound->sample;
        loopInfo = audioBookSample->loop;
        endPos = loopInfo->end;
        sampleAddr = audioBookSample->sampleAddr; // book?
        resampledTempLen = 0;

        for (curPart = 0; curPart < nParts; curPart++) {
            nAdpcmSamplesProcessed = 0;
            s4 = 0;

            if (nParts == 1) {
                samplesLenAdjusted = nSamplesToLoad;
            } else if (nSamplesToLoad & 1) {
                samplesLenAdjusted = (nSamplesToLoad & ~1) + (curPart * 2);
            } else {
                samplesLenAdjusted = nSamplesToLoad;
            }
            // sm64 also has if (audioBookSample->codec == CODEC_A) {
            if (curLoadedBook != (*audioBookSample->book).book) {
                u32 nEntries;
                curLoadedBook = audioBookSample->book->book; // or just one book?
                nEntries = 16 * audioBookSample->book->order * audioBookSample->book->npredictors;
                aLoadADPCM(cmd++, nEntries, VIRTUAL_TO_PHYSICAL2(noteSubEu->bookOffset + curLoadedBook));
                //                                                    noteSubEu here unsure but seems better?
            }
            if (noteSubEu->bookOffset != 0) {
                curLoadedBook = &gUnknownData_800F6290[0];
            }
            while (nAdpcmSamplesProcessed != samplesLenAdjusted) {

                s32 samplesRemaining;
                s32 s1;
                noteFinished = false;
                restart = false;

                s3 = synthesisState->samplePosInt & 0xF;
                samplesRemaining = endPos - synthesisState->samplePosInt;
                nSamplesToProcess = samplesLenAdjusted - nAdpcmSamplesProcessed;
                if ((s3 == 0) && (synthesisState->restart == false)) {
                    s3 = 16;
                }

                a1 = 16 - s3;

                if (nSamplesToProcess < samplesRemaining) {
                    loopInfo_2 = ((nSamplesToProcess - a1) + 0xF) / 16;
                    s1 = loopInfo_2 * 16;
                    var_s6 = (a1 + s1) - nSamplesToProcess;
                } else {
                    s1 = samplesRemaining - a1;
                    var_s6 = 0;
                    if (s1 <= 0) {
                        s1 = 0;
                        a1 = samplesRemaining;
                    }
                    loopInfo_2 = (s1 + 0xF) / 16;
                    if (loopInfo->count != 0) {
                        restart = 1;
                    } else {
                        noteFinished = 1;
                    }
                }
                // var_t2 = 0; // unsure
                if (loopInfo_2 != 0) {
                    temp_t6 = ((synthesisState->samplePosInt - s3) + 16) / 16; // diff from sm64 sh
                    if (audioBookSample->loaded == 0x81) {                     // sm64 has audioBookSample->medium
                        var_a0_2 = (temp_t6 * 9) + sampleAddr;
                    } else {
                        var_a0_2 =
                            dma_sample_data((uintptr_t) (temp_t6 * 9) + sampleAddr, ALIGN(((loopInfo_2 * 9) + 16), 4),
                                            flags, &synthesisState->sampleDmaIndex);
                        //                                                                        unsure   flags?
                    }

                    var_t2 = ((uintptr_t) var_a0_2 & 0xF);

                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    addr = (0x540 - aligned); // DMEM_ADDR_COMPRESSED_ADPCM_DATA

                    aLoadBuffer(cmd++, VIRTUAL_TO_PHYSICAL2(var_a0_2 - var_t2), addr, aligned);
                } else {
                    s1 = 0; // ?
                    var_t2 = 0;
                }

                if (synthesisState->restart != false) {
                    aSetLoop(cmd++, VIRTUAL_TO_PHYSICAL2(audioBookSample->loop->state));

                    flags = A_LOOP;
                    synthesisState->restart = false;
                }
                nSamplesInThisIteration = (s1 + a1) - var_s6;
                s5Aligned = ALIGN(s4 + 16, 4);
                if (nAdpcmSamplesProcessed == 0) {

                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    addr = (0x540 - aligned);
                    aSetBuffer(cmd++, 0, addr + var_t2, 0x1A0,
                               s1 * 2); // unsure
                                        //                                        s1 or s3 here?
                    aADPCMdec(cmd++, flags, VIRTUAL_TO_PHYSICAL2(synthesisState->synthesisBuffers->adpcmdecState));
                    spFC = s3 * 2;

                } else {

                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    addr = (0x540 - aligned);
                    aSetBuffer(cmd++, 0, addr + var_t2, 0x1A0 + s5Aligned, s1 * 2); // unsure

                    aADPCMdec(cmd++, flags, VIRTUAL_TO_PHYSICAL2(synthesisState->synthesisBuffers->adpcmdecState));

                    aDMEMMove(cmd++, 0x1A0 + s5Aligned + (s3 * 2), 0x1A0 + s4, nSamplesInThisIteration * 2);
                }

                nAdpcmSamplesProcessed += nSamplesInThisIteration;
                switch (flags) {
                    case 1:
                        spFC = 0x20;
                        s4 = (s1 * 2) + 0x20;
                        break;
                    case 2:
                        s4 += nSamplesInThisIteration * 2;
                        break;
                    default:
                        if (s4 != 0) {
                            s4 += nSamplesInThisIteration * 2;
                        } else {
                            s4 = (s3 + nSamplesInThisIteration) * 2;
                        }
                        break;
                }
                flags = 0;

                if (noteFinished) {
                    aClearBuffer(cmd++, 0x1A0 + s4, (samplesLenAdjusted - nAdpcmSamplesProcessed) * 2);
                    noteSubEu->finished = 1; // sm64 says = 1
                    note->noteSubEu.finished = 1;
                    note->noteSubEu.enabled = 0;

                    func_800B6FB4(updateIndex, noteIndex);
                    break;
                } else {
                    if (restart) {
                        synthesisState->restart = true;
                        synthesisState->samplePosInt = loopInfo->start;
                    } else {
                        synthesisState->samplePosInt += nSamplesToProcess;
                    }
                }
            }

            switch (nParts) {
                case 1:
                    noteSamplesDmemAddrBeforeResampling = 0x1A0 + spFC;
                    break;
                case 2:
                    switch (curPart) {
                        case 0:
                            aDownsampleHalf(cmd++, ALIGN(samplesLenAdjusted / 2, 3), 0x1A0 + spFC, DMEM_ADDR_RESAMPLED);
                            resampledTempLen = samplesLenAdjusted;
                            noteSamplesDmemAddrBeforeResampling = DMEM_ADDR_RESAMPLED;
                            if (noteSubEu->finished != false) {
                                aClearBuffer(cmd++, noteSamplesDmemAddrBeforeResampling + resampledTempLen,
                                             samplesLenAdjusted + 0x10);
                            }
                            break;
                        case 1:
                            aDownsampleHalf(cmd++, ALIGN(samplesLenAdjusted / 2, 3), DMEM_ADDR_RESAMPLED2 + spFC,
                                            resampledTempLen + DMEM_ADDR_RESAMPLED);
                            break;
                    }
                    // break;
            }
            if (noteSubEu->finished != false) {
                break;
            }
        }
    }
    flags = 0;
    if (noteSubEu->needsInit == true) {
        flags = A_INIT;
        noteSubEu->needsInit = false;
    }

    cmd = final_resample(cmd, synthesisState, inBuf * 2, resamplingRateFixedPoint, noteSamplesDmemAddrBeforeResampling,
                         flags);

    if ((noteSubEu->headsetPanRight) || (synthesisState->prevHeadsetPanRight)) {
        leftRight = 1; // According to m2c this is a new var. Perhaps flags?
    } else if ((noteSubEu->headsetPanLeft) || (synthesisState->prevHeadsetPanLeft)) {
        leftRight = 2;
    } else {
        leftRight = 0;
    }
    cmd = func_800B86A0(cmd, noteSubEu, synthesisState, inBuf, 0, leftRight, flags);

    if (noteSubEu->usesHeadsetPanEffects) {
        // synthesisState->restart = 1;
        cmd = note_apply_headset_pan_effects(cmd, noteSubEu, synthesisState, inBuf * 2, flags, leftRight);
    }
    return cmd;
}
#else
GLOBAL_ASM("asm/non_matchings/audio/synthesis/synthesis_process_note.s")
#endif

Acmd* load_wave_samples(Acmd* acmd, struct NoteSubEu* noteSubEu, struct NoteSynthesisState* synthesisState,
                        s32 nSamplesToLoad) {
    s32 a3;
    s32 repeats;
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(noteSubEu->sound.samples), 0x1A0, 128);

    synthesisState->samplePosInt &= 0x3f;
    a3 = 64 - synthesisState->samplePosInt;
    if (a3 < nSamplesToLoad) {
        repeats = (nSamplesToLoad - a3 + 63) / 64;
        if (repeats != 0) {
            // UTODO: Stubbed
            aDMEMMove2(acmd++, repeats, 0x1A0, 0x1A0 + 128, 128);
        }
    }
    return acmd;
}

Acmd* final_resample(Acmd* acmd, struct NoteSynthesisState* synthesisState, s32 count, u16 pitch, u16 dmemIn,
                     u32 flags) {
    aSetBuffer(acmd++, /*flags*/ 0, dmemIn, /*dmemout*/ 0, count);
    aResample(acmd++, flags, pitch, VIRTUAL_TO_PHYSICAL2(synthesisState->synthesisBuffers->finalResampleState));
    return acmd;
}

#ifdef NON_MATCHING
// I wish I knew what was up with this function
// It doesn't quite match either the EU or Shindou versions
// of process_envelope.
// What's here appears to have the right "shape" based on
// the M2C output, but beyond that I can't really tell what's
// going on
Acmd* func_800B86A0(Acmd* cmd, struct NoteSubEu* note, struct NoteSynthesisState* synthesisState, s32 nSamples,
                    u16 inBuf, s32 headsetPanSettings, UNUSED u32 flags) {
    u16 sourceRight;
    u16 sourceLeft;
    u16 targetLeft;
    u16 targetRight;
    s32 rampLeft;
    s32 rampRight;
    UNUSED s32 sourceReverbVol;
    UNUSED s16 rampReverb;
    UNUSED s32 reverbVolDiff = 0;

    sourceLeft = synthesisState->curVolLeft;
    sourceRight = synthesisState->curVolRight;
    targetLeft = note->targetVolLeft;
    targetRight = note->targetVolRight;
    targetLeft <<= 4;
    targetRight <<= 4;

    rampLeft = (targetLeft - sourceLeft) / (nSamples >> 3);
    rampRight = (targetRight - sourceRight) / (nSamples >> 3);
    synthesisState->curVolLeft = sourceLeft + rampLeft * (nSamples >> 3);
    synthesisState->curVolRight = sourceRight + rampRight * (nSamples >> 3);

    if (note->usesHeadsetPanEffects) {
        aClearBuffer(cmd++, DMEM_ADDR_NOTE_PAN_TEMP, DEFAULT_LEN_1CH);
        aEnvSetup1(cmd++, note->reverbVol, (((sourceLeft & 0xFF) << 8) | (sourceRight & 0xFF)), rampRight, rampLeft);
        aEnvSetup2(cmd++, sourceLeft, sourceRight);

        switch (headsetPanSettings) {
            case 1:
                aEnvMixer(cmd++, inBuf, nSamples, 0, note->stereoStrongRight, note->stereoStrongLeft,
                          DMEM_ADDR_NOTE_PAN_TEMP, DMEM_ADDR_RIGHT_CH, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_WET_RIGHT_CH);
                break;
            case 2:
                aEnvMixer(cmd++, inBuf, nSamples, 0, note->stereoStrongRight, note->stereoStrongLeft, DMEM_ADDR_LEFT_CH,
                          DMEM_ADDR_NOTE_PAN_TEMP, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_WET_RIGHT_CH);
                break;
            default:
                aEnvMixer(cmd++, inBuf, nSamples, 0, note->stereoStrongRight, note->stereoStrongLeft, DMEM_ADDR_LEFT_CH,
                          DMEM_ADDR_RIGHT_CH, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_WET_RIGHT_CH);
                break;
        }
    } else {
        aEnvSetup1(cmd++, note->reverbVol, (((sourceLeft & 0xFF) << 8) | (sourceRight & 0xFF)), rampLeft, rampRight);
        aEnvSetup2(cmd++, sourceLeft, sourceRight);
        aEnvMixer(cmd++, inBuf, nSamples, 0, note->stereoStrongRight, note->stereoStrongLeft, DMEM_ADDR_LEFT_CH,
                  DMEM_ADDR_RIGHT_CH, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_WET_RIGHT_CH);
    }
    return cmd;
}
#else
GLOBAL_ASM("asm/non_matchings/audio/synthesis/func_800B86A0.s")
#endif

Acmd* note_apply_headset_pan_effects(Acmd* acmd, struct NoteSubEu* noteSubEu, struct NoteSynthesisState* note,
                                     s32 bufLen, s32 flags, s32 leftRight) {
    u16 dest;
    u16 pitch;
    u8 prevPanShift;
    u8 panShift;
    UNUSED u8 unkDebug;

    switch (leftRight) {
        case 1:
            dest = 0x540;
            panShift = noteSubEu->headsetPanRight;
            note->prevHeadsetPanLeft = 0;
            prevPanShift = note->prevHeadsetPanRight;
            note->prevHeadsetPanRight = panShift;
            break;
        case 2:
            dest = 0x6C0;
            panShift = noteSubEu->headsetPanLeft;
            note->prevHeadsetPanRight = 0;

            prevPanShift = note->prevHeadsetPanLeft;
            note->prevHeadsetPanLeft = panShift;
            break;
        default:
            return acmd;
    }

    if (flags != 1) { // A_INIT?
        // Slightly adjust the sample rate in order to fit a change in pan shift
        if (prevPanShift == 0) {
            // Kind of a hack that moves the first samples into the resample state
            aDMEMMove(acmd++, 0x0200, 0x0000, 8);
            aClearBuffer(acmd++, 8, 8); // Set pitch accumulator to 0 in the resample state
            aDMEMMove(acmd++, 0x0200, 0x0000 + 0x10,
                      0x10); // No idea, result seems to be overwritten later

            aSaveBuffer(acmd++, 0x0000, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->panResampleState),
                        sizeof(note->synthesisBuffers->panResampleState));

            pitch = (bufLen << 0xf) / (bufLen + panShift - prevPanShift + 8);
            if (pitch) {}
            aSetBuffer(acmd++, 0, 0x0200 + 8, 0x0000, panShift + bufLen - prevPanShift);
            aResample(acmd++, 0, pitch, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->panResampleState));
        } else {
            if (panShift == 0) {
                pitch = (bufLen << 0xf) / (bufLen - prevPanShift - 4);
            } else {
                pitch = (bufLen << 0xf) / (bufLen + panShift - prevPanShift);
            }

            if (1) {}

            aSetBuffer(acmd++, 0, 0x0200, 0x0000, bufLen + panShift - prevPanShift);
            aResample(acmd++, 0, pitch, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->panResampleState));
        }

        if (prevPanShift != 0) {
            aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->panSamplesBuffer), 0x0200, prevPanShift);
            aDMEMMove(acmd++, 0x0000, 0x0200 + prevPanShift, panShift + bufLen - prevPanShift);
        } else {
            aDMEMMove(acmd++, 0x0000, 0x0200, panShift + bufLen - prevPanShift);
        }
    } else {
        // Just shift right
        aDMEMMove(acmd++, 0x0200, 0x0000, bufLen);
        aDMEMMove(acmd++, 0x0000, 0x0200 + panShift, bufLen);
        aClearBuffer(acmd++, 0x0200, panShift);
    }

    if (panShift) {
        // Save excessive samples for next iteration
        aSaveBuffer(acmd++, 0x0200 + bufLen, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->panSamplesBuffer), panShift);
    }

    aMix(acmd++, /*gain*/ 0x7FFF, /*in*/ 0x0200, /*out*/ dest, ALIGN(bufLen, 5));

    return acmd;
}
