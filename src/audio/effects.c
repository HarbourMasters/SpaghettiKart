#include <libultraship.h>
#include <macros.h>

#include "audio/seqplayer.h"
#include "audio/effects.h"
#include "audio/internal.h"
#include "audio/load.h"
#include "audio/data.h"

void sequence_channel_process_sound(struct SequenceChannel* seqChannel, s32 recalculateVolume) {
    f32 channelVolume;
    s32 i;

    if (seqChannel->changes.as_bitfields.volume || recalculateVolume) {
        channelVolume = (seqChannel->volume * (seqChannel->volumeScale * seqChannel->seqPlayer->fadeVolume)) *
                        seqChannel->seqPlayer->gameVolume;
        if (seqChannel->seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_SOFTEN) != 0) {
            channelVolume = seqChannel->seqPlayer->muteVolumeScale * channelVolume;
        }
        seqChannel->appliedVolume = channelVolume * channelVolume;
    }

    if (seqChannel->changes.as_bitfields.pan) {
        seqChannel->pan = seqChannel->newPan * seqChannel->panChannelWeight;
    }

    for (i = 0; i < 4; ++i) {
        struct SequenceChannelLayer* layer = seqChannel->layers[i];
        if (layer != NULL && layer->enabled && layer->note != NULL) {
            if (layer->notePropertiesNeedInit) {
                layer->noteFreqScale = layer->freqScale * seqChannel->freqScale;
                layer->noteVelocity = layer->velocitySquare * seqChannel->appliedVolume;
                layer->notePan = (seqChannel->pan + layer->pan * (0x80 - seqChannel->panChannelWeight)) >> 7;
                layer->notePropertiesNeedInit = false;
            } else {
                if (seqChannel->changes.as_bitfields.freqScale) {
                    layer->noteFreqScale = layer->freqScale * seqChannel->freqScale;
                }
                if (seqChannel->changes.as_bitfields.volume || recalculateVolume) {
                    layer->noteVelocity = layer->velocitySquare * seqChannel->appliedVolume;
                }
                if (seqChannel->changes.as_bitfields.pan) {
                    layer->notePan = (seqChannel->pan + layer->pan * (0x80 - seqChannel->panChannelWeight)) >> 7;
                }
            }
        }
    }
    seqChannel->changes.as_u8 = 0;
}

void sequence_player_process_sound(struct SequencePlayer* seqPlayer) {
    s32 i;

    if (seqPlayer->fadeRemainingFrames != 0) {
        seqPlayer->fadeVolume += seqPlayer->fadeVelocity;
        seqPlayer->recalculateVolume = true;

        if (seqPlayer->fadeVolume > US_FLOAT2(1)) {
            seqPlayer->fadeVolume = US_FLOAT2(1);
        }
        if (seqPlayer->fadeVolume < 0) {
            seqPlayer->fadeVolume = 0;
        }

        if (--seqPlayer->fadeRemainingFrames == 0) {
            if (seqPlayer->state == 2) {
                sequence_player_disable(seqPlayer);
                return;
            }
        }
    }

    if (seqPlayer->recalculateVolume) {
        seqPlayer->appliedFadeVolume = seqPlayer->fadeVolume * seqPlayer->fadeVolumeScale;
    }

    // Process channels
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->channels[i]) == true && seqPlayer->channels[i]->enabled == true) {
            sequence_channel_process_sound(seqPlayer->channels[i], seqPlayer->recalculateVolume);
        }
    }

    seqPlayer->recalculateVolume = false;
}

f32 get_portamento_freq_scale(struct Portamento* p) {
    u32 v0;
    f32 result;

    p->cur += p->speed;
    v0 = (u32) p->cur;

    if (v0 > 127) {
        v0 = 127;
    }

    result = US_FLOAT(1.0) + p->extent * (gPitchBendFrequencyScale[v0 + 128] - US_FLOAT(1.0));
    return result;
}

s16 get_vibrato_pitch_change(struct VibratoState* vib) {
    s32 index;
    vib->time += (s32) vib->rate;
    index = (vib->time >> 10) & 0x3F;
    return vib->curve[index] >> 8;
}

f32 get_vibrato_freq_scale(struct VibratoState* vib) {
    s32 pitchChange;
    f32 extent;
    f32 result;

    if (vib->delay != 0) {
        vib->delay--;
        return 1;
    }

    if (vib->extentChangeTimer) {
        if (vib->extentChangeTimer == 1) {
            vib->extent = (s32) vib->seqChannel->vibratoExtentTarget;
        } else {
            vib->extent += ((s32) vib->seqChannel->vibratoExtentTarget - vib->extent) / (s32) vib->extentChangeTimer;
        }

        vib->extentChangeTimer--;
    } else if (vib->seqChannel->vibratoExtentTarget != (s32) vib->extent) {
        if ((vib->extentChangeTimer = vib->seqChannel->vibratoExtentChangeDelay) == 0) {
            vib->extent = (s32) vib->seqChannel->vibratoExtentTarget;
        }
    }

    if (vib->rateChangeTimer) {
        if (vib->rateChangeTimer == 1) {
            vib->rate = (s32) vib->seqChannel->vibratoRateTarget;
        } else {
            vib->rate += ((s32) vib->seqChannel->vibratoRateTarget - vib->rate) / (s32) vib->rateChangeTimer;
        }

        vib->rateChangeTimer--;
    } else if (vib->seqChannel->vibratoRateTarget != (s32) vib->rate) {
        if ((vib->rateChangeTimer = vib->seqChannel->vibratoRateChangeDelay) == 0) {
            vib->rate = (s32) vib->seqChannel->vibratoRateTarget;
        }
    }

    if (vib->extent == 0) {
        return 1.0f;
    }

    pitchChange = get_vibrato_pitch_change(vib);
    extent = (f32) vib->extent / US_FLOAT(4096.0);

    result = US_FLOAT(1.0) + extent * (gPitchBendFrequencyScale[pitchChange + 128] - US_FLOAT(1.0));
    return result;
}

void note_vibrato_update(struct Note* note) {
    if (note->portamento.mode != 0) {
        note->portamentoFreqScale = get_portamento_freq_scale(&note->portamento);
    }
    if (note->vibratoState.active && note->parentLayer != NO_LAYER) {
        note->vibratoFreqScale = get_vibrato_freq_scale(&note->vibratoState);
    }
}

void note_vibrato_init(struct Note* note) {
    struct VibratoState* vib;
    UNUSED struct SequenceChannel* seqChannel;
    struct NotePlaybackState* seqPlayerState = (struct NotePlaybackState*) &note->priority;

    note->vibratoFreqScale = 1.0f;
    note->portamentoFreqScale = 1.0f;

    vib = &note->vibratoState;

    vib->active = true;
    vib->time = 0;

    vib->curve = gWaveSamples[2];
    vib->seqChannel = note->parentLayer->seqChannel;
    if ((vib->extentChangeTimer = vib->seqChannel->vibratoExtentChangeDelay) == 0) {
        vib->extent = FLOAT_CAST(vib->seqChannel->vibratoExtentTarget);
    } else {
        vib->extent = FLOAT_CAST(vib->seqChannel->vibratoExtentStart);
    }

    if ((vib->rateChangeTimer = vib->seqChannel->vibratoRateChangeDelay) == 0) {
        vib->rate = FLOAT_CAST(vib->seqChannel->vibratoRateTarget);
    } else {
        vib->rate = FLOAT_CAST(vib->seqChannel->vibratoRateStart);
    }
    vib->delay = vib->seqChannel->vibratoDelay;

    seqPlayerState->portamento = seqPlayerState->parentLayer->portamento;
}

void adsr_init(struct AdsrState* adsr, struct AdsrEnvelope* envelope, UNUSED s16* volOut) {
    adsr->action = 0;
    adsr->state = ADSR_STATE_DISABLED;
    adsr->delay = 0;
    adsr->envelope = envelope;
    adsr->sustain = 0.0f;
    adsr->current = 0.0f;
}

f32 adsr_update(struct AdsrState* adsr) {
    u8 action = adsr->action;
    u8 state = adsr->state;
    switch (state) {
        case ADSR_STATE_DISABLED:
            return 0;

        case ADSR_STATE_INITIAL: {
            if (action & ADSR_ACTION_HANG) {
                adsr->state = ADSR_STATE_HANG;
                break;
            }
            // fallthrough
        }

        case ADSR_STATE_START_LOOP:
            adsr->envIndex = 0;
            adsr->state = ADSR_STATE_LOOP;
            // fallthrough

        restart:
        case ADSR_STATE_LOOP:
            adsr->delay = BSWAP16(adsr->envelope[adsr->envIndex].delay);
            switch (adsr->delay) {
                case ADSR_DISABLE:
                    adsr->state = ADSR_STATE_DISABLED;
                    break;
                case ADSR_HANG:
                    adsr->state = ADSR_STATE_HANG;
                    break;
                case ADSR_GOTO:
                    adsr->envIndex = BSWAP16(adsr->envelope[adsr->envIndex].arg);
                    goto restart;
                case ADSR_RESTART:
                    adsr->state = ADSR_STATE_INITIAL;
                    break;

                default:
                    if (adsr->delay >= 4) {
                        adsr->delay = adsr->delay * gAudioBufferParameters.updatesPerFrame /
                                      gAudioBufferParameters.presetUnk4 / 4;
                    }
                    if (adsr->delay == 0) {
                        adsr->delay = 1;
                    }
                    adsr->target = (f32) BSWAP16(adsr->envelope[adsr->envIndex].arg) / 32767.0f;
                    adsr->target = adsr->target * adsr->target;
                    adsr->velocity = (adsr->target - adsr->current) / adsr->delay;
                    adsr->state = ADSR_STATE_FADE;
                    adsr->envIndex++;
                    break;
            }
            if (adsr->state != ADSR_STATE_FADE) {
                break;
            }
            // fallthrough

        case ADSR_STATE_FADE:
            adsr->current += adsr->velocity;
            if (--adsr->delay <= 0) {
                adsr->state = ADSR_STATE_LOOP;
            }
            // fallthrough

        case ADSR_STATE_HANG:
            break;

        case ADSR_STATE_DECAY:
        case ADSR_STATE_RELEASE: {
            adsr->current -= adsr->fadeOutVel;
            if (adsr->sustain != 0.0f && state == ADSR_STATE_DECAY) {
                if (adsr->current < adsr->sustain) {
                    adsr->current = adsr->sustain;
                    adsr->delay = 128;
                    adsr->state = ADSR_STATE_SUSTAIN;
                }
                break;
            }

            if (adsr->current < 0.00001f) {
                adsr->current = 0.0f;
                adsr->state = ADSR_STATE_DISABLED;
            }
            break;
        }

        case ADSR_STATE_SUSTAIN:
            adsr->delay -= 1;
            if (adsr->delay == 0) {
                adsr->state = ADSR_STATE_RELEASE;
            }
            break;
    }

    if ((action & ADSR_ACTION_DECAY)) {
        adsr->state = ADSR_STATE_DECAY;
        adsr->action = action & ~ADSR_ACTION_DECAY;
    }

    if ((action & ADSR_ACTION_RELEASE)) {
        adsr->state = ADSR_STATE_RELEASE;
        adsr->action = action & ~ADSR_ACTION_RELEASE;
    }

    if (adsr->current < 0.0f) {
        // Don't actually know where this goes, but we need that string
        // somewhere in the .data section for this file
        eu_stubbed_printf_0("Env-Clear 0\n");
        return 0.0f;
    }
    if (adsr->current > 1.0f) {
        eu_stubbed_printf_1("Audio:Envp: overflow  %f\n", adsr->current);
        return 1.0f;
    }
    return adsr->current;
}
