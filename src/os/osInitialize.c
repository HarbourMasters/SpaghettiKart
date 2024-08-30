#include "libultra_internal.h"
#include <stubs.h>
#include "hardware.h"
#include <macros.h>

#define PIF_ADDR_START (void*) 0x1FC007FC

typedef struct {
    u32 instr00;
    u32 instr01;
    u32 instr02;
    u32 instr03;
} exceptionPreamble;

u32 D_80194040;

u64 osClockRate = 62500000;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
u32 D_800EA5F0 = 0;

#define EXCEPTION_TLB_MISS 0x80000000
#define EXCEPTION_XTLB_MISS 0x80000080
#define EXCEPTION_CACHE_ERROR 0x80000100
#define EXCEPTION_GENERAL 0x80000180

extern u32 osResetType;
extern exceptionPreamble __osExceptionPreamble;

void osInitialize(void) {
    u32 sp34;
    u32 sp30 = 0;

    UNUSED u32 eu_sp34;
    UNUSED u32 eu_sp30;
    UNUSED u32 sp2c;
    D_80194040 = true;
    __osSetSR(__osGetSR() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    while (__osSiRawReadIo(PIF_ADDR_START, &sp34)) {
        ;
    }
    while (__osSiRawWriteIo(PIF_ADDR_START, sp34 | 8)) {
        ;
    }
    *(exceptionPreamble*) EXCEPTION_TLB_MISS = __osExceptionPreamble;
    *(exceptionPreamble*) EXCEPTION_XTLB_MISS = __osExceptionPreamble;
    *(exceptionPreamble*) EXCEPTION_CACHE_ERROR = __osExceptionPreamble;
    *(exceptionPreamble*) EXCEPTION_GENERAL = __osExceptionPreamble;
    osWritebackDCache((void*) 0x80000000, EXCEPTION_GENERAL + sizeof(exceptionPreamble) - EXCEPTION_TLB_MISS);
    osInvalICache((void*) 0x80000000, EXCEPTION_GENERAL + sizeof(exceptionPreamble) - EXCEPTION_TLB_MISS);
    osMapTLBRdb();
    osPiRawReadIo(4, &sp30);
    sp30 &= ~0xf;
    if (sp30) {
        osClockRate = sp30;
    }
    osClockRate = osClockRate * 3 / 4;
    if (osResetType == RESET_TYPE_COLD_RESET) {
        // bzero(osAppNmiBuffer, sizeof(osAppNmiBuffer));
    }

    eu_sp30 = HW_REG(PI_STATUS_REG, u32);
    while (eu_sp30 & PI_STATUS_ERROR) {
        eu_sp30 = HW_REG(PI_STATUS_REG, u32);
    };
    if (!((eu_sp34 = HW_REG(ASIC_STATUS, u32)) & _64DD_PRESENT_MASK)) {
        D_800EA5F0 = 1;
        __osSetHWIntrRoutine(1, &__osLeoInterrupt);
    } else {
        D_800EA5F0 = 0;
    }
}
