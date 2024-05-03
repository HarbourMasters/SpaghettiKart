#include <libultraship.h>

#ifdef _WIN32
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1, b2, len) (memmove((b2), (b1), (len)), (void) 0)
#endif

extern u32 osTvType;
extern u32 osResetType;

extern u8 osAppNmiBuffer[64];

void rmonPrintf(const char* fmt, ...);
void func_80040030(u8* arg0, u8* arg1);
void func_80040174(void*, s32, s32);