#include <libultraship.h>
#include <macros.h>

#include "audio/internal.h"
#include "audio/playback.h"
#include "audio/load.h"
#include "audio/heap.h"
#include "audio/external.h"
#include "audio/effects.h"
#include "audio/data.h"
#include "audio/seqplayer.h"

void note_set_vel_pan_reverb(struct Note* note, f32 velocity, u8 pan, u8 reverbVol) {
    struct NoteSubEu* sub = &note->noteSubEu;
    f32 volRight, volLeft;
    u8 strongRight;
    u8 strongLeft;
    s32 smallPanIndex;
    u16 unkMask = ~0x80;

    pan &= unkMask;

    if (note->noteSubEu.stereoHeadsetEffects && gAudioLibSoundMode == SOUND_MODE_HEADSET) {
        smallPanIndex = pan >> 3;
        if (smallPanIndex >= ARRAY_COUNT(gHeadsetPanQuantization)) {
            smallPanIndex = ARRAY_COUNT(gHeadsetPanQuantization) - 1;
        }

        sub->headsetPanLeft = gHeadsetPanQuantization[smallPanIndex];
        sub->headsetPanRight = gHeadsetPanQuantization[ARRAY_COUNT(gHeadsetPanQuantization) - 1 - smallPanIndex];
        sub->stereoStrongRight = false;
        sub->stereoStrongLeft = false;
        sub->usesHeadsetPanEffects = true;

        volLeft = gHeadsetPanVolume[pan];
        volRight = gHeadsetPanVolume[127 - pan];
    } else if (sub->stereoHeadsetEffects && gAudioLibSoundMode == SOUND_MODE_STEREO) {
        strongRight = false;
        strongLeft = false;
        sub->headsetPanRight = 0;
        sub->headsetPanLeft = 0;

        sub->usesHeadsetPanEffects = false;

        volLeft = gStereoPanVolume[pan];
        volRight = gStereoPanVolume[127 - pan];
        if (pan < 0x20) {
            strongLeft = true;
        } else if (pan > 0x60) {
            strongRight = true;
        }

        sub->stereoStrongRight = strongRight;
        sub->stereoStrongLeft = strongLeft;

    } else if (gAudioLibSoundMode == SOUND_MODE_MONO) {
        volLeft = 0.707f;
        volRight = 0.707f;
    } else {
        volLeft = gDefaultPanVolume[pan];
        volRight = gDefaultPanVolume[127 - pan];
    }

    if (velocity < 0.0f) {
        stubbed_printf("Audio: setvol: volume minus %f\n", velocity);
        velocity = 0.0f;
    }
    if (velocity > 1.0f) {
        stubbed_printf("Audio: setvol: volume overflow %f\n", velocity);
        velocity = 1.0f;
    }

    sub->targetVolLeft = ((s32) (velocity * volLeft * 4095.999f));
    sub->targetVolRight = ((s32) (velocity * volRight * 4095.999f));

    //! @bug for the change to UQ0.7, the if statement should also have been changed accordingly
    if (sub->reverbVol != reverbVol) {
        sub->reverbVol = reverbVol;
        sub->envMixerNeedsInit = true;
        return;
    }

    if (sub->needsInit) {
        sub->envMixerNeedsInit = true;
    } else {
        sub->envMixerNeedsInit = false;
    }
}

void note_set_resampling_rate(struct Note* note, f32 resamplingRateInput) {
    f32 resamplingRate = 0.0f;
    struct NoteSubEu* tempSub = &note->noteSubEu;

    if (resamplingRateInput < 0.0f) {
        stubbed_printf("Audio: setpitch: pitch minus %f\n", resamplingRateInput);
        resamplingRateInput = 0.0f;
    }
    if (resamplingRateInput < 2.0f) {
        tempSub->hasTwoAdpcmParts = 0;

        if (1.9999599f < resamplingRateInput) {
            resamplingRate = 1.9999599f;
        } else {
            resamplingRate = resamplingRateInput;
        }

    } else {
        tempSub->hasTwoAdpcmParts = 1;
        if (2 * 1.9999599f < resamplingRateInput) {
            resamplingRate = 1.9999599f;
        } else {
            resamplingRate = resamplingRateInput * 0.5f;
        }
    }
    note->noteSubEu.resamplingRateFixedPoint = (s32) (resamplingRate * 32768.0f);
}

struct AudioBankSound* instrument_get_audio_bank_sound(struct Instrument* instrument, s32 semitone) {
    struct AudioBankSound* sound;
    if (semitone < instrument->normalRangeLo) {
        sound = &instrument->lowNotesSound;
    } else if (semitone <= instrument->normalRangeHi) {
        sound = &instrument->normalNotesSound;
    } else {
        sound = &instrument->highNotesSound;
    }
    return sound;
}

struct Instrument* get_instrument_inner(s32 bankId, s32 instId) {
    struct Instrument* inst;

    if (IS_BANK_LOAD_COMPLETE(bankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", bankId);
        gAudioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (instId >= gCtlEntries[bankId].numInstruments) {
        stubbed_printf("Audio: voiceman: progNo. overflow %d,%d\n", instId, gCtlEntries[bankId].numInstruments);
        gAudioErrorFlags = ((bankId << 8) + instId) + 0x3000000;
        return NULL;
    }

    inst = gCtlEntries[bankId].instruments[instId];
    if (inst == NULL) {
        stubbed_printf("Audio: voiceman: progNo. undefined %d,%d\n", bankId, instId);
        gAudioErrorFlags = ((bankId << 8) + instId) + 0x1000000;
        return inst;
    }
    return inst;
}

struct Drum* get_drum(s32 bankId, s32 drumId) {
    struct Drum* drum;

    if (IS_BANK_LOAD_COMPLETE(bankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", bankId);
        gAudioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (drumId >= gCtlEntries[bankId].numDrums) {
        stubbed_printf("Audio: voiceman: Percussion Overflow %d,%d\n", drumId, gCtlEntries[bankId].numDrums);
        gAudioErrorFlags = ((bankId << 8) + drumId) + 0x4000000;
        return NULL;
    }

    if ((uintptr_t) gCtlEntries[bankId].drums < 0x80000000U) {
        stubbed_printf("Audio: voiceman: Percussion table pointer (bank %d) is irregular.\n");
        return NULL;
    }

    drum = gCtlEntries[bankId].drums[drumId];
    if (drum == NULL) {
        stubbed_printf("Audio: voiceman: Percpointer NULL %d,%d\n", bankId, drumId);
        gAudioErrorFlags = ((bankId << 8) + drumId) + 0x5000000;
    }
    // Don't know where these actually belong, putting them here because why not
    // Really, we just need these so the format strings could put into the .data section
    stubbed_printf("--4 %x\n", gAudioErrorFlags);
    stubbed_printf("Stoped Voice\n");
    return drum;
}

void note_init(struct Note* note) {
    if (note->parentLayer->adsr.releaseRate == 0) {
        adsr_init(&note->adsr, note->parentLayer->seqChannel->adsr.envelope, &note->adsrVolScale);
    } else {
        adsr_init(&note->adsr, note->parentLayer->adsr.envelope, &note->adsrVolScale);
    }
    note->adsr.state = ADSR_STATE_INITIAL;
    note->noteSubEu = gDefaultNoteSub;
}

void note_disable(struct Note* note) {
    if (note->noteSubEu.needsInit == true) {
        note->noteSubEu.needsInit = false;
    } else {
        note_set_vel_pan_reverb(note, 0, 0x40, 0);
    }
    note->priority = NOTE_PRIORITY_DISABLED;
    note->parentLayer = NO_LAYER;
    note->prevParentLayer = NO_LAYER;
    note->noteSubEu.enabled = false;
    note->noteSubEu.finished = false;
}

void process_notes(void) {
    f32 scale;
    f32 frequency;
    f32 velocity;
    struct Note* note;
    struct NotePlaybackState* playbackState;
    struct NoteSubEu* noteSubEu;
    UNUSED u8 pad[12];
    u8 reverbVol;
    UNUSED u8 pad3;
    u8 pan;
    u8 bookOffset;
    struct NoteAttributes* attributes;
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        playbackState = (struct NotePlaybackState*) &note->priority;
        if (note->parentLayer != NO_LAYER) {
#ifndef NO_SEGMENTED_MEMORY
            if ((uintptr_t) playbackState->parentLayer < 0x7fffffffU) {
                continue;
            }
#endif

#ifdef VERSION_EU_1_0
            // Just threw these in here. They are probably wrong place and variables.
            stubbed_printf("----------------------Double-Error CH: %x %f\n", &note, note->parentLayer->seqChannel);
            stubbed_printf("----------------------Double-Error NT: %x\n", &note);
            if (note != playbackState->parentLayer->note && playbackState->priority != 1) {
                playbackState->adsr.action |= ADSR_ACTION_RELEASE;
                playbackState->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
                playbackState->priority = 1;
                goto d;
            }
#endif
            if (!playbackState->parentLayer->enabled && playbackState->priority >= NOTE_PRIORITY_MIN) {
                goto c;
            } else if (playbackState->parentLayer->seqChannel->seqPlayer == NULL) {
                eu_stubbed_printf_0("CAUTION:SUB IS SEPARATED FROM GROUP");
                sequence_channel_disable(playbackState->parentLayer->seqChannel);
                playbackState->priority = NOTE_PRIORITY_STOPPING;
                continue;
            } else if (playbackState->parentLayer->seqChannel->seqPlayer->muted) {
                if ((playbackState->parentLayer->seqChannel->muteBehavior &
                     (MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES))) {
                    goto c;
                }
            }
            goto d;
        c:
            seq_channel_layer_note_release(playbackState->parentLayer);
            audio_list_remove(&note->listItem);
            audio_list_push_front(&note->listItem.pool->decaying, &note->listItem);
            playbackState->priority = NOTE_PRIORITY_STOPPING;
        } else if (playbackState->priority >= NOTE_PRIORITY_MIN) {
            continue;
        }
    d:
        if (playbackState->priority != NOTE_PRIORITY_DISABLED) {
            noteSubEu = &note->noteSubEu;
            if (playbackState->priority == NOTE_PRIORITY_STOPPING || noteSubEu->finished) {
                if (playbackState->adsr.state == ADSR_STATE_DISABLED || noteSubEu->finished) {
                    if (playbackState && playbackState) {}
                    if (playbackState->wantedParentLayer != NO_LAYER) {
                        note_disable(note);
                        if (playbackState->wantedParentLayer->seqChannel != NULL) {
                            note_init_for_layer(note, playbackState->wantedParentLayer);
                            note_vibrato_init(note);
                            audio_list_remove(&note->listItem);
                            audio_list_push_back(&note->listItem.pool->active, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            // don't skip
                        } else {
                            eu_stubbed_printf_0("Error:Wait Track disappear\n");
                            note_disable(note);
                            audio_list_remove(&note->listItem);
                            audio_list_push_back(&note->listItem.pool->disabled, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            goto skip;
                        }
                    } else {
                        note_disable(note);
                        audio_list_remove(&note->listItem);
                        audio_list_push_back(&note->listItem.pool->disabled, &note->listItem);
                        goto skip;
                    }
                }
            } else if (playbackState->adsr.state == ADSR_STATE_DISABLED) {
                note_disable(note);
                audio_list_remove(&note->listItem);
                audio_list_push_back(&note->listItem.pool->disabled, &note->listItem);
                goto skip;
            }

            scale = adsr_update(&playbackState->adsr);
            note_vibrato_update(note);
            attributes = &playbackState->attributes;
            if (playbackState->priority == NOTE_PRIORITY_STOPPING) {
                frequency = attributes->freqScale;
                velocity = attributes->velocity;
                pan = attributes->pan;
                reverbVol = attributes->reverbVol;
                if (1) {}
                bookOffset = noteSubEu->bookOffset;
            } else {
                frequency = playbackState->parentLayer->noteFreqScale;
                velocity = playbackState->parentLayer->noteVelocity;
                pan = playbackState->parentLayer->notePan;
                reverbVol = playbackState->parentLayer->seqChannel->reverbVol;
                bookOffset = playbackState->parentLayer->seqChannel->bookOffset & 0x7;
            }

            frequency *= playbackState->vibratoFreqScale * playbackState->portamentoFreqScale;
            velocity = velocity * scale;
            note_set_resampling_rate(note, frequency);
            note_set_vel_pan_reverb(note, velocity, pan, reverbVol);
            noteSubEu->bookOffset = bookOffset;
        skip:;
        }
    }
}

void seq_channel_layer_decay_release_internal(struct SequenceChannelLayer* seqLayer, s32 target) {
    struct Note* note;
    struct NoteAttributes* attributes;

    if ((seqLayer == NO_LAYER) || (seqLayer->note == NULL)) {
        return;
    }

    note = seqLayer->note;
    attributes = &note->attributes;

    if (note->wantedParentLayer == seqLayer) {
        note->wantedParentLayer = NO_LAYER;
    }

    if (note->parentLayer != seqLayer) {
        if (note->parentLayer == NO_LAYER && note->wantedParentLayer == NO_LAYER && note->prevParentLayer == seqLayer &&
            target != ADSR_STATE_DECAY) {
            // Just guessing that this printf goes here... it's hard to parse.
            eu_stubbed_printf_0("Slow Release Batting\n");
            note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
            note->adsr.action |= ADSR_ACTION_RELEASE;
        }
    } else {
        seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
        if (note->adsr.state != ADSR_STATE_DECAY) {
            attributes->freqScale = seqLayer->noteFreqScale;
            attributes->velocity = seqLayer->noteVelocity;
            attributes->pan = seqLayer->notePan;
            if (seqLayer->seqChannel != NULL) {
                attributes->reverbVol = seqLayer->seqChannel->reverbVol;
            }
            note->priority = NOTE_PRIORITY_STOPPING;
            note->prevParentLayer = note->parentLayer;
            note->parentLayer = NO_LAYER;
            if (target == ADSR_STATE_RELEASE) {
                note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
                note->adsr.action |= ADSR_ACTION_RELEASE;
            } else {
                note->adsr.action |= ADSR_ACTION_DECAY;
                if (seqLayer->adsr.releaseRate == 0) {
                    note->adsr.fadeOutVel =
                        seqLayer->seqChannel->adsr.releaseRate * gAudioBufferParameters.unkUpdatesPerFrameScaled;
                } else {
                    note->adsr.fadeOutVel =
                        seqLayer->adsr.releaseRate * gAudioBufferParameters.unkUpdatesPerFrameScaled;
                }
                note->adsr.sustain = (FLOAT_CAST(seqLayer->seqChannel->adsr.sustain) * note->adsr.current) / 256.0f;
            }
        }

        if (target == ADSR_STATE_DECAY) {
            audio_list_remove(&note->listItem);
            audio_list_push_front(&note->listItem.pool->decaying, &note->listItem);
        }
    }
}

void seq_channel_layer_note_decay(struct SequenceChannelLayer* seqLayer) {
    seq_channel_layer_decay_release_internal(seqLayer, ADSR_STATE_DECAY);
}

void seq_channel_layer_note_release(struct SequenceChannelLayer* seqLayer) {
    seq_channel_layer_decay_release_internal(seqLayer, ADSR_STATE_RELEASE);
}

const u8 D_800E98F4[4] = { 0x40, 0x20, 0x10, 0x08 };

s32 build_synthetic_wave(struct Note* note, struct SequenceChannelLayer* seqLayer, s32 waveId) {
    f32 freqScale;
    f32 ratio;
    u8 sampleCountIndex;

    if (waveId < 128) {
        stubbed_printf("Audio:Wavemem: Bad voiceno (%d)\n", waveId);
        waveId = 128;
    }

    freqScale = seqLayer->freqScale;
    if (seqLayer->portamento.mode != 0 && 0.0f < seqLayer->portamento.extent) {
        freqScale *= (seqLayer->portamento.extent + 1.0f);
    }
    if (freqScale < 1.0f) {
        sampleCountIndex = 0;
        ratio = 1.0465f;
    } else if (freqScale < 2.0f) {
        sampleCountIndex = 1;
        ratio = 0.52325f;
    } else if (freqScale < 4.0f) {
        sampleCountIndex = 2;
        ratio = 0.26263f;
    } else {
        sampleCountIndex = 3;
        ratio = 0.13081f;
    }
    seqLayer->freqScale *= ratio;
    note->waveId = waveId;
    note->sampleCountIndex = sampleCountIndex;

    note->noteSubEu.sound.samples = &gWaveSamples[waveId - 128][sampleCountIndex * 64];

    return sampleCountIndex;
}

void init_synthetic_wave(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    s32 sampleCountIndex;
    s32 waveSampleCountIndex;
    s32 waveId = seqLayer->instOrWave;
    if (waveId == 0xff) {
        waveId = seqLayer->seqChannel->instOrWave;
    }
    sampleCountIndex = note->sampleCountIndex;
    waveSampleCountIndex = build_synthetic_wave(note, seqLayer, waveId);
    note->synthesisState.samplePosInt =
        note->synthesisState.samplePosInt * D_800E98F4[waveSampleCountIndex] / D_800E98F4[sampleCountIndex];
}

void init_note_list(struct AudioListItem* list) {
    list->prev = list;
    list->next = list;
    list->u.count = 0;
}

void init_note_lists(struct NotePool* pool) {
    init_note_list(&pool->disabled);
    init_note_list(&pool->decaying);
    init_note_list(&pool->releasing);
    init_note_list(&pool->active);
    pool->disabled.pool = pool;
    pool->decaying.pool = pool;
    pool->releasing.pool = pool;
    pool->active.pool = pool;
}

void init_note_free_list(void) {
    s32 i;

    init_note_lists(&gNoteFreeLists);
    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        gNotes[i].listItem.u.value = &gNotes[i];
        gNotes[i].listItem.prev = NULL;
        audio_list_push_back(&gNoteFreeLists.disabled, &gNotes[i].listItem);
    }
}

void note_pool_clear(struct NotePool* pool) {
    s32 i;
    struct AudioListItem* source;
    struct AudioListItem* cur;
    struct AudioListItem* dest;
    UNUSED s32 j; // unused in EU

    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                source = &pool->disabled;
                dest = &gNoteFreeLists.disabled;
                break;

            case 1:
                source = &pool->decaying;
                dest = &gNoteFreeLists.decaying;
                break;

            case 2:
                source = &pool->releasing;
                dest = &gNoteFreeLists.releasing;
                break;

            case 3:
                source = &pool->active;
                dest = &gNoteFreeLists.active;
                break;
        }

        for (;;) {
            cur = source->next;
            if (cur == source) {
                break;
            }
            if (cur == NULL) {
                eu_stubbed_printf_0("Audio: C-Alloc : Dealloc voice is NULL\n");
                break;
            }
            audio_list_remove(cur);
            audio_list_push_back(dest, cur);
        }
    }
}

void note_pool_fill(struct NotePool* pool, s32 count) {
    s32 i;
    s32 j;
    struct Note* note;
    struct AudioListItem* source;
    struct AudioListItem* dest;

    note_pool_clear(pool);

    for (i = 0, j = 0; j < count; i++) {
        if (i == 4) {
            eu_stubbed_printf_1("Alloc Error:Dim voice-Alloc %d", count);
            return;
        }

        switch (i) {
            case 0:
                source = &gNoteFreeLists.disabled;
                dest = &pool->disabled;
                break;

            case 1:
                source = &gNoteFreeLists.decaying;
                dest = &pool->decaying;
                break;

            case 2:
                source = &gNoteFreeLists.releasing;
                dest = &pool->releasing;
                break;

            case 3:
                source = &gNoteFreeLists.active;
                dest = &pool->active;
                break;
        }

        while (j < count) {
            note = audio_list_pop_back(source);
            if (note == NULL) {
                break;
            }
            audio_list_push_back(dest, &note->listItem);
            j++;
        }
    }
}

void audio_list_push_front(struct AudioListItem* list, struct AudioListItem* item) {
    // add 'item' to the front of the list given by 'list', if it's not in any list
    if (item->prev != NULL) {
        eu_stubbed_printf_0("Error:Same List Add\n");
    } else {
        item->prev = list;
        item->next = list->next;
        list->next->prev = item;
        list->next = item;
        list->u.count++;
        item->pool = list->pool;
    }
}

void audio_list_remove(struct AudioListItem* item) {
    // remove 'item' from the list it's in, if any
    if (item->prev == NULL) {
        eu_stubbed_printf_0("Already Cut\n");
    } else {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        item->prev = NULL;
    }
}

struct Note* pop_node_with_lower_prio(struct AudioListItem* list, s32 limit) {
    struct AudioListItem* cur = list->next;
    struct AudioListItem* best;

    if (cur == list) {
        return NULL;
    }

    for (best = cur; cur != list; cur = cur->next) {
        if (((struct Note*) best->u.value)->priority >= ((struct Note*) cur->u.value)->priority) {
            best = cur;
        }
    }

    if (best == NULL) {
        return NULL;
    }

    if (limit <= ((struct Note*) best->u.value)->priority) {
        return NULL;
    }

    audio_list_remove(best);
    return best->u.value;
}

void note_init_for_layer(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    UNUSED s32 pad[4];
    s16 instId;
    struct NoteSubEu* sub = &note->noteSubEu;

    note->prevParentLayer = NO_LAYER;
    note->parentLayer = seqLayer;
    note->priority = seqLayer->seqChannel->notePriority;
    seqLayer->notePropertiesNeedInit = true;
    seqLayer->status = SOUND_LOAD_STATUS_DISCARDABLE; // "loaded"
    seqLayer->note = note;
    seqLayer->seqChannel->noteUnused = note;
    seqLayer->seqChannel->layerUnused = seqLayer;
    seqLayer->noteVelocity = 0.0f;
    note_init(note);
    instId = seqLayer->instOrWave;
    if (instId == 0xff) {
        instId = seqLayer->seqChannel->instOrWave;
    }
    sub->sound.audioBankSound = seqLayer->sound;

    if (instId >= 0x80) {
        sub->isSyntheticWave = true;
    } else {
        sub->isSyntheticWave = false;
    }

    if (sub->isSyntheticWave) {
        build_synthetic_wave(note, seqLayer, instId);
    }
    sub->bankId = seqLayer->seqChannel->bankId;
    sub->stereoHeadsetEffects = seqLayer->seqChannel->stereoHeadsetEffects;
    sub->reverbIndex = seqLayer->seqChannel->reverbIndex & 3;
}

void func_800BD8F4(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    seq_channel_layer_note_release(note->parentLayer);
    note->wantedParentLayer = seqLayer;
}

void note_release_and_take_ownership(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    note->wantedParentLayer = seqLayer;
    note->priority = NOTE_PRIORITY_STOPPING;
    note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
    note->adsr.action |= ADSR_ACTION_RELEASE;
}

struct Note* alloc_note_from_disabled(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* note = audio_list_pop_back(&pool->disabled);
    if (note != NULL) {
        note_init_for_layer(note, seqLayer);
        audio_list_push_front(&pool->active, &note->listItem);
    }
    return note;
}

struct Note* alloc_note_from_decaying(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* note = audio_list_pop_back(&pool->decaying);
    if (note != NULL) {
        note_release_and_take_ownership(note, seqLayer);
        audio_list_push_back(&pool->releasing, &note->listItem);
    }
    return note;
}

struct Note* alloc_note_from_active(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* aNote;

    aNote = pop_node_with_lower_prio(&pool->active, seqLayer->seqChannel->notePriority);

    if (aNote == NULL) {
        eu_stubbed_printf_0("Audio: C-Alloc : lowerPrio is NULL\n");
    } else {
        func_800BD8F4(aNote, seqLayer);
        audio_list_push_back(&pool->releasing, &aNote->listItem);
    }

    return aNote;
}

struct Note* alloc_note(struct SequenceChannelLayer* seqLayer) {
    struct Note* ret;
    u32 policy = seqLayer->seqChannel->noteAllocPolicy;

    if (policy & NOTE_ALLOC_LAYER) {
        ret = seqLayer->note;
        if (ret != NULL && ret->prevParentLayer == seqLayer && ret->wantedParentLayer == NO_LAYER) {
            note_release_and_take_ownership(ret, seqLayer);
            audio_list_remove(&ret->listItem);
            audio_list_push_back(&ret->listItem.pool->releasing, &ret->listItem);
            return ret;
        }
    }

    if (policy & NOTE_ALLOC_CHANNEL) {
        if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer))) {
            eu_stubbed_printf_0("Sub Limited Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_SEQ) {
        if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = alloc_note_from_disabled(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = alloc_note_from_active(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_GLOBAL_FREELIST) {
        if (!(ret = alloc_note_from_disabled(&gNoteFreeLists, seqLayer)) &&
            !(ret = alloc_note_from_decaying(&gNoteFreeLists, seqLayer)) &&
            !(ret = alloc_note_from_active(&gNoteFreeLists, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = alloc_note_from_disabled(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = alloc_note_from_disabled(&gNoteFreeLists, seqLayer)) &&
        !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = alloc_note_from_decaying(&gNoteFreeLists, seqLayer)) &&
        !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = alloc_note_from_active(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = alloc_note_from_active(&gNoteFreeLists, seqLayer))) {
        eu_stubbed_printf_0("Warning: Drop Voice");
        seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
        return NULL;
    }
    return ret;
}

void note_init_all(void) {
    struct Note* note;
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        note->noteSubEu = gZeroNoteSub;
        note->priority = NOTE_PRIORITY_DISABLED;
        note->parentLayer = NO_LAYER;
        note->wantedParentLayer = NO_LAYER;
        note->prevParentLayer = NO_LAYER;
        note->waveId = 0;
        note->attributes.velocity = 0.0f;
        note->adsrVolScale = 0;
        note->adsr.state = ADSR_STATE_DISABLED;
        note->adsr.action = 0;
        note->vibratoState.active = false;
        note->portamento.cur = 0.0f;
        note->portamento.speed = 0.0f;
        // This only works if NoteSynthesisBuffers are size 0xA0. See internal.h
        note->synthesisState.synthesisBuffers = soundAlloc(&gNotesAndBuffersPool, sizeof(struct NoteSynthesisBuffers));
    }
}
