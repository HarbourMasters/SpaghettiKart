#include <libultraship.h>
#include <macros.h>
#include <libultraship/libultra.h>
#include <string.h>
#include <stdio.h>
#include <stubs.h>
#include "save.h"

struct state_pak {
    OSPfsState state;
    FILE* file;
};

struct state_pak openFile[16];

int fileIndex = 0;

u32 osTvType = OS_TV_NTSC;
u32 osResetType;

u8 osAppNmiBuffer[64];

void rmonPrintf(UNUSED const char* fmt, ...) {
}

void func_80040030(UNUSED u8* arg0, UNUSED u8* arg1) {
}

void func_80040174(UNUSED void* arg0, UNUSED s32 arg1, UNUSED s32 arg2) {
}

s32 osAiSetFrequency(UNUSED u32 freq) {
    return 1;
}

void mio0decode(UNUSED u8* arg0, UNUSED u8* arg1) {
}

s32 mio0encode(UNUSED s32 input, UNUSED s32 arg1, UNUSED s32 arg2) {
    return 1;
}

void osStartThread(UNUSED OSThread* thread) {
}

void osCreateThread(UNUSED OSThread* thread, UNUSED OSId id, void (UNUSED *entry)(void*), UNUSED void* arg, UNUSED void* sp, UNUSED OSPri pri) {
}

void osInitialize(void) {
}

void osSetThreadPri(UNUSED OSThread* thread, UNUSED OSPri pri) {
}

void osSpTaskLoad(UNUSED OSTask* task) {
}

void osSpTaskStartGo(UNUSED OSTask* task) {
}

void osSpTaskYield(void) {
}

OSYieldResult osSpTaskYielded(UNUSED OSTask* task) {
    return 0;
}
