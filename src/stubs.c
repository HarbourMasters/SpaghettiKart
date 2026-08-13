#include <libultraship.h>
#include <macros.h>
#include <libultraship/libultra.h>
#include <string.h>
#include <stdio.h>
#include <stubs.h>
#include "save.h"

// Everything here is a stub, so unused parameters are the norm; silence the
// warning once for the file instead of tagging every signature.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

struct state_pak {
    OSPfsState state;
    FILE* file;
};

struct state_pak openFile[16];

int fileIndex = 0;

u32 osTvType = OS_TV_NTSC;
u32 osResetType;

u8 osAppNmiBuffer[64];

void rmonPrintf(const char* fmt, ...) {
}

void func_80040030(u8* arg0, u8* arg1) {
}

void func_80040174(void* arg0, s32 arg1, s32 arg2) {
}

s32 osAiSetFrequency(u32 freq) {
    return 1;
}

void mio0decode(u8* arg0, u8* arg1) {
}

s32 mio0encode(uintptr_t input, s32 arg1, uintptr_t arg2) {
    return 1;
}

void osStartThread(OSThread* thread) {
}

void osCreateThread(OSThread* thread, OSId id, void(UNUSED* entry)(void*), void* arg, void* sp, OSPri pri) {
}

void osInitialize(void) {
}

void osSetThreadPri(OSThread* thread, OSPri pri) {
}

void osSpTaskLoad(OSTask* task) {
}

void osSpTaskStartGo(OSTask* task) {
}

void osSpTaskYield(void) {
}

OSYieldResult osSpTaskYielded(OSTask* task) {
    return 0;
}
