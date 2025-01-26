#include <libultraship.h>
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
    char filename[1024];
    sprintf(filename, "channel_%d_%hu_%hd_%s.sav", pfs->channel, company_code, game_code, game_name);
    remove(filename);
    return PFS_NO_ERROR;
}

s32 osPfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8* data_buffer) {
    if (flag == PFS_READ) {
        fseek(openFile[file_no].file, offset, SEEK_SET);
        fread(data_buffer, size_in_bytes, 1, openFile[file_no].file);
    } else {
        fseek(openFile[file_no].file, offset, SEEK_SET);
        fwrite(data_buffer, size_in_bytes, 1, openFile[file_no].file);
    }
    return PFS_NO_ERROR;
}

s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, int file_size_in_bytes,
                      s32* file_no) {
    char filename[1024];
    sprintf(filename, "channel_%d_%hu_%hd_%s.sav", pfs->channel, company_code, game_code, game_name);
    *file_no = fileIndex++;
    openFile[*file_no].file = fopen(filename, "w+");
    fwrite("\0", 1, file_size_in_bytes, openFile[*file_no].file);
    openFile[*file_no].state.company_code = company_code;
    openFile[*file_no].state.game_code = game_code;
    strcpy(openFile[*file_no].state.game_name, game_name);
    strcpy(openFile[*file_no].state.ext_name, ext_name);
    return PFS_NO_ERROR;
}

void osSetTime(OSTime time) {
}

s32 osPfsIsPlug(OSMesgQueue* queue, u8* pattern) {
}

s32 osPfsInit(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->status |= PFS_INITIALIZED;
    return PFS_NO_ERROR;
}

s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    *max_files = 16;
    *files_used = fileIndex;
    return 0;
}

s32 osPfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state) {
}

s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
}

s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no) {
    char filename[1024];
    sprintf(filename, "channel_%d_%hu_%hd_%s.sav", pfs->channel, company_code, game_code, game_name);
    for (int i = 0; i < 16; i++) {
        if (openFile[i].state.game_code == game_code && openFile[i].state.company_code == company_code &&
            strcmp(openFile[i].state.game_name, game_name) == 0 && strcmp(openFile[i].state.ext_name, ext_name) == 0) {
            *file_no = i;
            return PFS_NO_ERROR;
        }
    }
    *file_no = fileIndex++;
    openFile[*file_no].file = fopen(filename, "r+");
    if (openFile[*file_no].file == NULL) {
        openFile[*file_no].file = fopen(filename, "w+");
    }
    openFile[*file_no].state.company_code = company_code;
    openFile[*file_no].state.game_code = game_code;
    strcpy(openFile[*file_no].state.game_name, game_name);
    strcpy(openFile[*file_no].state.ext_name, ext_name);
    return PFS_NO_ERROR;
}