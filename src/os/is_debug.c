#include "libultra_internal.h"
#include "libc/stdarg.h"
#include <string.h>
#include "printf.h"
#include <PR/os.h>
#include "hardware.h"

// Note: This file was very quickly strewn together from oot.
// Not great but it works.

OSPiHandle* sISVHandle; // official name : is_Handle

typedef struct {
    /* 0x00 */ u32 magic; // IS64
    /* 0x04 */ u32 get;
    /* 0x08 */ u8 unk_08[0x14 - 0x08];
    /* 0x14 */ u32 put;
    /* 0x18 */ u8 unk_18[0x20 - 0x18];
    /* 0x20 */ u8 data[0x10000 - 0x20];
} ISVDbg;

#define gISVDbgPrnAdrs ((ISVDbg*) 0xB3FF0000)
#define ASCII_TO_U32(a, b, c, d) ((u32) ((a << 24) | (b << 16) | (c << 8) | (d << 0)))

void* is_proutSyncPrintf(void* arg, const char* str, u32 count);
s32 __osEPiRawWriteIo(OSPiHandle* a0, u32 a1, u32 a2);
void __osPiRelAccess(void);

s32 __osEPiRawReadIo(OSPiHandle* arg0, u32 devAddr, u32* data) {
    register s32 stat;
    while (stat = HW_REG(PI_STATUS_REG, s32), stat & (PI_STATUS_BUSY | PI_STATUS_IOBUSY | PI_STATUS_ERROR)) {
        ;
    }
    *data = HW_REG(arg0->baseAddress | devAddr, s32);
    return 0;
}

s32 osEPiReadIo(OSPiHandle* handle, u32 devAddr, u32* data) {
    register s32 ret;

    __osPiGetAccess();
    ret = __osEPiRawReadIo(handle, devAddr, data);
    __osPiRelAccess();

    return ret;
}

s32 osEPiWriteIo(OSPiHandle* handle, u32 devAddr, u32 data) {
    register s32 ret;

    __osPiGetAccess();
    ret = __osEPiRawWriteIo(handle, devAddr, data);
    __osPiRelAccess();

    return ret;
}

void isPrintfInit(void) {
    sISVHandle = osCartRomInit();
    osEPiWriteIo(sISVHandle, (u32) &gISVDbgPrnAdrs->put, 0);
    osEPiWriteIo(sISVHandle, (u32) &gISVDbgPrnAdrs->get, 0);
    osEPiWriteIo(sISVHandle, (u32) &gISVDbgPrnAdrs->magic, ASCII_TO_U32('I', 'S', '6', '4'));
}

void osSyncPrintfUnused(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf((char* (*) (char*, const char*, size_t)) is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

void osSyncPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf((char* (*) (char*, const char*, size_t)) is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

void print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf((char* (*) (char*, const char*, size_t)) is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

// assumption
void rmonPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _Printf((char* (*) (char*, const char*, size_t)) is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

void* is_proutSyncPrintf(UNUSED void* arg, const char* str, u32 count) {
    u32 data;
    s32 pos;
    s32 start;
    s32 end;

    osEPiReadIo(sISVHandle, (u32) &gISVDbgPrnAdrs->magic, &data);
    if (data != ASCII_TO_U32('I', 'S', '6', '4')) {
        return (void*) 1;
    }
    osEPiReadIo(sISVHandle, (u32) &gISVDbgPrnAdrs->get, &data);
    pos = data;
    osEPiReadIo(sISVHandle, (u32) &gISVDbgPrnAdrs->put, &data);
    start = data;
    end = start + count;
    if (end >= 0xFFE0) {
        end -= 0xFFE0;
        if (pos < end || start < pos) {
            return (void*) 1;
        }
    } else {
        if (start < pos && pos < end) {
            return (void*) 1;
        }
    }
    while (count) {
        u32 addr = (u32) &gISVDbgPrnAdrs->data + (start & 0xFFFFFFC);
        s32 shift = ((3 - (start & 3)) * 8);

        if (*str) {
            osEPiReadIo(sISVHandle, addr, &data);
            osEPiWriteIo(sISVHandle, addr, (*str << shift) | (data & ~(0xFF << shift)));

            start++;
            if (start >= 0xFFE0) {
                start -= 0xFFE0;
            }
        }
        count--;
        str++;
    }
    osEPiWriteIo(sISVHandle, (u32) &gISVDbgPrnAdrs->put, start);

    return (void*) 1;
}
