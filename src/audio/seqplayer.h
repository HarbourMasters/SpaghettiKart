#ifndef AUDIO_SEQPLAYER_H
#define AUDIO_SEQPLAYER_H

#include <libultra/types.h>

#include "audio/internal.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5

void sequence_channel_init(struct SequenceChannel* seqChannel);
s32 seq_channel_set_layer(struct SequenceChannel* seqChannel, s32 layerIndex);
void seq_channel_layer_disable(struct SequenceChannelLayer* layer);
void seq_channel_layer_free(struct SequenceChannel* seqChannel, s32 layerIndex);
void sequence_channel_disable(struct SequenceChannel* seqChannel);
struct SequenceChannel* allocate_sequence_channel(void);
void sequence_player_init_channels(struct SequencePlayer* seqPlayer, u16 channelBits);
void sequence_player_disable_channels(struct SequencePlayer* seqPlayer, u16 channelBits);
void sequence_channel_enable(struct SequencePlayer* seqPlayer, u8 channelIndex, void* script);
void sequence_player_disable(struct SequencePlayer* seqPlayer);
void func_800BEF2C(struct SequencePlayer* seqPlayer);
void audio_list_push_back(struct AudioListItem* list, struct AudioListItem* item);
void* audio_list_pop_back(struct AudioListItem* list);
void init_layer_freelist(void);
u8 m64_read_u8(struct M64ScriptState* state);
s16 m64_read_s16(struct M64ScriptState* state);
u16 m64_read_compressed_u16(struct M64ScriptState* state);
u8 get_instrument(struct SequenceChannel* seqChannel, u8 instId, struct Instrument** instOut,
                  struct AdsrSettings* adsr);
void process_sequences(s32);
void set_instrument(struct SequenceChannel* seqChannel, u8 instId);
void sequence_channel_set_volume(struct SequenceChannel* seqChannel, u8 volume);
void seq_channel_layer_process_script(struct SequenceChannelLayer* layer);
void sequence_player_process_sequence(struct SequencePlayer*);
void sequence_channel_process_script(struct SequenceChannel*);
void process_sequences(s32);
void init_sequence_player(u32 player);
void init_sequence_players(void);

#endif // AUDIO_SEQPLAYER_H
