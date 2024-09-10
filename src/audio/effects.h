#ifndef AUDIO_EFFECTS_H
#define AUDIO_EFFECTS_H

#include "audio/internal.h"

#ifdef TARGET_N64
#define IS_BIG_ENDIAN 1
#endif

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

void sequence_channel_process_sound(struct SequenceChannel* seqChannel, s32 recalculateVolume);
void sequence_player_process_sound(struct SequencePlayer* seqPlayer);
f32 get_portamento_freq_scale(struct Portamento* p);
s16 get_vibrato_pitch_change(struct VibratoState* vib);
f32 get_vibrato_freq_scale(struct VibratoState* vib);
void note_vibrato_update(struct Note* note);
void note_vibrato_init(struct Note* note);
void adsr_init(struct AdsrState* adsr, struct AdsrEnvelope* envelope, s16* volOut);
f32 adsr_update(struct AdsrState* adsr);

#endif /* AUDIO_EFFECTS_H */
