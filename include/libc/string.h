#ifndef STRING_H
#define STRING_H

#ifndef TARGET_N64
#include <libultra/types.h>
#else
#include <PR/ultratypes.h>
#endif

void* memcpy(void* dst, const void* src, size_t size);
size_t strlen(const char* str);
char* strchr(const char* str, s32 ch);

#endif
