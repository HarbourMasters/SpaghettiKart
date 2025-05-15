#include <libultraship.h>

#ifdef _WIN32
#define bzero(b, len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1, b2, len) (memmove((b2), (b1), (len)), (void) 0)
#endif

extern u32 osTvType;
extern u32 osResetType;

extern u8 osAppNmiBuffer[64];

void rmonPrintf(const char* fmt, ...);
void func_80040030(u8* arg0, u8* arg1);
void func_80040174(void*, s32, s32);
s32 osAiSetFrequency(u32 freq);
void mio0decode(u8* arg0, u8* arg1);
s32 mio0encode(s32 input, s32, s32);
void osStartThread(OSThread* thread);
void osCreateThread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri);
void osInitialize(void);
void osSetThreadPri(OSThread* thread, OSPri pri);
void osSpTaskLoad(OSTask* task);
void osSpTaskStartGo(OSTask* task);
void osSpTaskYield(void);
OSYieldResult osSpTaskYielded(OSTask* task);
s32 osPfsDeleteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name);
s32 osPfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8* data_buffer);
s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, int file_size_in_bytes,
                      s32* file_no);
s32 osPfsIsPlug(OSMesgQueue* queue, u8* pattern);
s32 osPfsInit(OSMesgQueue* queue, OSPfs* pfs, int channel);
s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used);
s32 osPfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state);
s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used);
s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no);
