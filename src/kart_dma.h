#ifndef KART_DMA_H
#define KART_DMA_H

#include "macros.h"
#include <common_structs.h>

/* Function Prototypes */

void load_kart_texture(Player*, s8, s8, s8, s8);
void load_kart_texture_non_blocking(Player*, s8, s8, s8, s8);
void load_kart_palette(Player*, s8, s8, s8);
void load_player_data(Player*, s32, void*, u16);
void load_wheel_palette_non_blocking(Player*, const char*, void*, u16);

/* This is where I'd put my static data, if I had any */

extern u16 D_800DDEB0[];

// end of kart_dma.h variables

// extern u8 _kart_texturesSegmentRomStart[];

#endif
