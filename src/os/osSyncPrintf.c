#include "libultra_internal.h"
#include "libc/stdarg.h"
#include "printf.h"

// These funcs defined in is_debug.c
#ifndef DEBUG

char* osSyncPrintf(UNUSED char* arg0, UNUSED const char* arg1, UNUSED size_t size) {
    // ifdef'd formatting code?
    return (char*) (1);
}

void rmonPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _Printf(osSyncPrintf, NULL, fmt, args);
    va_end(args);
}

#endif // DEBUG
