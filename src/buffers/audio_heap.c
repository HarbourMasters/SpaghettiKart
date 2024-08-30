#include <libultra/types.h>
#include <macros.h>
#include <mk64.h>

#include "audio_heap.h"
#include "audio/data.h"

ALIGNED8 u8 gAudioHeap[DOUBLE_SIZE_ON_64_BIT(AUDIO_HEAP_SIZE)];
