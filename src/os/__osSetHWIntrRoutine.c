#include "libultra_internal.h"

// Its not clear how big this array should be.
// In order for file alignment to be correct, it needs to be
// at least 5 but no more than 8. Beyond that its not clear
// what its size should be
s32 (*__osHwIntTable[5])(void) = { NULL };
void __osSetHWIntrRoutine(OSHWIntr interrupt, s32 (*handler)(void)) {
    register u32 saveMask;
    saveMask = __osDisableInt();
    __osHwIntTable[interrupt] = handler;
    __osRestoreInt(saveMask);
}
