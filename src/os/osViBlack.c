#include <libultraship.h>
#include "libultra_internal.h"

extern OSViContext* __osViNext;

//! @todo name magic constants
void osViBlack(u8 active) {
    register u32 int_disabled = __osDisableInt();
    if (active) {
        __osViNext->state |= 0x20;
    } else {
        __osViNext->state &= ~0x20;
    }
    __osRestoreInt(int_disabled);
}
