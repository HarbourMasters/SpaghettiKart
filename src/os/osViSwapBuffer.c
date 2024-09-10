#include "libultra_internal.h"

extern OSViContext* __osViNext;

void osViSwapBuffer(void* vaddr) {
    u32 int_disabled = __osDisableInt();
    __osViNext->buffer = vaddr;
    //! @todo figure out what this flag means
    __osViNext->state |= 0x10;
    __osRestoreInt(int_disabled);
}
