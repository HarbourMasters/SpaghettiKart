#include <libultraship.h>
#include <libultraship/libultra.h>
#include <string.h>
#include <stubs.h>

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
    arg1 = arg0;
}

s32 mio0encode(s32 input, s32 arg1, s32 arg2) {
    return 1;
}

void tkmk00decode(u8* src, u8* arg1, u8* dest, s32 n) {
    memcpy(dest, src, 0x1000);
}

void osStartThread(OSThread* thread) {
}

void osCreateThread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
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
}

s32 osPfsDeleteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name) {
}

s32 osPfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8* data_buffer) {
}

s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, int file_size_in_bytes,
                      s32* file_no) {
}

void osSetTime(OSTime time) {
}

s32 osPfsIsPlug(OSMesgQueue* queue, u8* pattern) {
}

s32 osPfsInit(OSMesgQueue* queue, OSPfs* pfs, int channel) {
}

s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    return 0;
}

s32 osPfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state) {
}

s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
}

s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no) {
}
