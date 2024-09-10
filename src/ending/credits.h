#ifndef CREDITS_H
#define CREDITS_H

#include <libultra/types.h>

#define SLIDE_RIGHT 0
#define SLIDE_LEFT 1

// In some way dictates how the text is written during the credit sequence
typedef struct {
    // Scaling factor that affects the x/y scaling and tracking of printed text
    /* 0x00 */ f32 textScaling;
    // Column to start sliding in from
    /* 0x04 */ s16 startingColumn;
    /* 0x06 */ s16 row;
    // Extra distance added to the destination column
    /* 0x08 */ s16 columnExtra;
    /* 0x0A */ s16 unknown;       // No idea what this is for, has a value but never seems to be read
    /* 0x0C */ s8 slideDirection; // 0 for slide right, 1 for slide left. May have other uses/effects
    /* 0x0D */ s8 textColor;
    /* 0x0E */ s16 padding; // Always seems to be 0, never read (that I can see)
} struct_802850C0_entry;    // size = 0x10

extern struct_802850C0_entry D_802850C0[]; // D_802850C0
extern char* D_802854B0[];

#endif
