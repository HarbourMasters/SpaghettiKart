#ifndef _LIBULTRA_INTERNAL_H_
#define _LIBULTRA_INTERNAL_H_
#include <libultraship.h>
#include "macros.h"

#ifdef TARGET_N64
typedef struct __OSEventState {
    OSMesgQueue* messageQueue;
    OSMesg message;
} __OSEventState;
#endif

typedef struct // __osThreadTail
{
    /* 0x00 */ OSThread* next;
    /* 0x04 */ OSPri priority;
    /* 0x08 */ OSThread* queue;
    /* 0x0c */ OSThread* tlnext;

} OSThreadTail;

/*
 * This define is needed because the original definitions in __osDequeueThread.c are declared
 * seperately instead of part of a single struct, however some code alises over this memory
 * assuming a unified structure. To fix this, we declare the full type here and then alias the
 * symbol names to the correct members in AVOID_UB.
 */
typedef struct {
    /*0x00*/ struct OSThread_s* next;
    /*0x04*/ OSPri priority;
    /*0x08*/ struct OSThread_s* queue;
    /*0x0C*/ struct OSThread_s* tlnext;
    /*0x10*/ struct OSThread_s* unk10;
    /*0x14*/ u32 unk14;
} OSThread_ListHead;

// Original OSThread_ListHead definitions
extern OSThread* __osThreadTail;
extern OSThread* __osActiveQueue;
extern OSThread* __osRunQueue;
extern OSThread* __osRunningThread;

// Original EEPROM definitions
extern u32 D_80365E00[15];
extern u32 D_80365E3C;

#ifdef TARGET_N64
typedef struct {
    u32 initialized; // probably something like initialized?
    OSThread* mgrThread;
    OSMesgQueue* cmdQueue;
    OSMesgQueue* eventQueue;
    OSMesgQueue* accessQueue;
    s32 (*dma_func)(s32, u32, void*, size_t);
    s32 (*edma_func)(OSPiHandle*, s32, u32, void*, size_t);
} OSMgrArgs;
#endif

s32 __osDisableInt(void);
void __osRestoreInt(s32);
void __osEnqueueAndYield(OSThread**);
void __osDequeueThread(OSThread**, OSThread*);
void __osEnqueueThread(OSThread**, OSThread*);
OSThread* __osPopThread(OSThread**);
s32 __osSiRawStartDma(s32, void*);
void __osSiCreateAccessQueue(void);
void __osSiGetAccess(void);
void __osSiRelAccess(void);
u32 __osProbeTLB(void*);
void __osPiCreateAccessQueue(void);
void __osPiGetAccess(void);
void __osSetSR(u32);
u32 __osGetSR(void);
void __osSetFpcCsr(u32);
s32 __osSiRawReadIo(void*, u32*);
s32 __osSiRawWriteIo(void*, u32);
s32 osPiRawReadIo(u32 a0, u32* a1);
void __osSpSetStatus(u32);
u32 __osSpGetStatus(void);
s32 __osSpSetPc(void*);
s32 __osSpDeviceBusy(void);
s32 __osSiDeviceBusy(void);
s32 __osSpRawStartDma(u32 dir, void* sp_ptr, void* dram_ptr, size_t size);
void __osSetHWIntrRoutine(OSHWIntr interrupt, s32 (*callback)(void), void* sp);
s32 __osLeoInterrupt();
void __osViInit(void);
OSViContext* __osViGetCurrentContext(void);
OSViContext* __osViGetCurrentContext2(void);
void __osViSwapContext(void);
void __osSetTimerIntr(u64);
u64 __osInsertTimer(OSTimer*);
void __osSetCompare(u32);
s32 __osAiDeviceBusy(void);
void __osDispatchThread(void);
u32 __osGetCause(void);
s32 __osAtomicDec(u32*);
#endif
