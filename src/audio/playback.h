#ifndef AUDIO_PLAYBACK_H
#define AUDIO_PLAYBACK_H

#include <libultra/types.h>

#include "internal.h"

// Mask bits denoting where to allocate notes from, according to a channel's
// noteAllocPolicy. Despite being checked as bitmask bits, the bits are not
// orthogonal; rather, the smallest bit wins, except for NOTE_ALLOC_LAYER,
// which *is* orthogonal to the other. SEQ implicitly includes CHANNEL.
// If none of the CHANNEL/SEQ/GLOBAL_FREELIST bits are set, all three locations
// are tried.
#define NOTE_ALLOC_LAYER 1
#define NOTE_ALLOC_CHANNEL 2
#define NOTE_ALLOC_SEQ 4
#define NOTE_ALLOC_GLOBAL_FREELIST 8

void note_set_vel_pan_reverb(struct Note* note, f32 velocity, u8 pan, u8 reverbVol);
void note_set_resampling_rate(struct Note* note, f32 resamplingRateInput);
s32 build_synthetic_wave(struct Note* note, struct SequenceChannelLayer* seqLayer, s32 waveId);
void init_synthetic_wave(struct Note* note, struct SequenceChannelLayer* seqLayer);
void init_note_list(struct AudioListItem* list);
void init_note_lists(struct NotePool* pool);
void init_note_free_list(void);
void note_pool_clear(struct NotePool* pool);
void note_pool_fill(struct NotePool* pool, s32 count);
void audio_list_push_front(struct AudioListItem* list, struct AudioListItem* item);
void audio_list_remove(struct AudioListItem* item);
struct Note* pop_node_with_lower_prio(struct AudioListItem* list, s32 limit);
void note_init_for_layer(struct Note* note, struct SequenceChannelLayer* seqLayer);
void note_disable(struct Note* note);
void process_notes(void);
struct AudioBankSound* instrument_get_audio_bank_sound(struct Instrument* instrument, s32 semitone);
struct Instrument* get_instrument_inner(s32 bankId, s32 instId);
struct Drum* get_drum(s32 bankId, s32 drumId);
void note_init(struct Note* note);
void seq_channel_layer_decay_release_internal(struct SequenceChannelLayer* seqLayer, s32 target);
void seq_channel_layer_note_decay(struct SequenceChannelLayer* seqLayer);
void seq_channel_layer_note_release(struct SequenceChannelLayer* seqLayer);
void func_800BD8F4(struct Note* note, struct SequenceChannelLayer* seqLayer);
void note_release_and_take_ownership(struct Note* note, struct SequenceChannelLayer* seqLayer);
struct Note* alloc_note_from_disabled(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* alloc_note_from_decaying(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* alloc_note_from_active(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* alloc_note(struct SequenceChannelLayer* seqLayer);
void note_init_all(void);

#endif // AUDIO_PLAYBACK_H
