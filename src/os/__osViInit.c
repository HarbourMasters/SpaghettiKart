#include <libultraship.h>
#include <libultra/vi.h>
#include "libultra_internal.h"
#include "hardware.h"
#include <stubs.h>

OSViContext sViContexts[2] = { 0 };
OSViContext* __osViCurr = &sViContexts[0];
OSViContext* __osViNext = &sViContexts[1];

u32 osViClock = 0x02E6D354; // used for audio frequency calculations

extern OSViMode osViModePalLan1;
extern OSViMode osViModeMpalLan1;
extern OSViMode osViModeNtscLan1;

void __osViInit(void) {
    bzero(sViContexts, sizeof(sViContexts));
    __osViCurr = &sViContexts[0];
    __osViNext = &sViContexts[1];
    __osViNext->retraceCount = 1;
    __osViCurr->retraceCount = 1;

    if (osTvType == OS_TV_PAL) {
        __osViNext->modep = &osViModePalLan1;
        osViClock = 0x02F5B2D2;
    } else if (osTvType == OS_TV_MPAL) {
        __osViNext->modep = &osViModeMpalLan1;
        osViClock = 0x02E6025C;
    } else {
        __osViNext->modep = &osViModeNtscLan1;
        osViClock = 0x02E6D354;
    }

    __osViNext->state = 0x20;
    __osViNext->features = __osViNext->modep->comRegs.ctrl;
    while (HW_REG(VI_CURRENT_REG, u32) > 0xa) {
        ;
    }
    HW_REG(VI_STATUS_REG, u32) = 0;
    __osViSwapContext();
}
