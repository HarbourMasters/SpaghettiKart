#ifndef _OSINT_H
#define _OSINT_H
#include "libultra_internal.h"
#include <macros.h>

// maybe should be in exceptasm.h?
extern void __osEnqueueAndYield(OSThread**);
extern void __osDequeueThread(OSThread**, OSThread*);
extern void __osEnqueueThread(OSThread**, OSThread*);
extern OSThread* __osPopThread(OSThread**);
extern void __osDispatchThread(void);

extern void __osSetTimerIntr(OSTime);
extern OSTime __osInsertTimer(OSTimer*);
extern void __osTimerInterrupt(void);
extern u32 __osProbeTLB(void*);
extern int __osSpDeviceBusy(void);

extern OSThread* __osRunningThread;
extern OSThread* __osActiveQueue;
extern OSThread* __osFaultedThread;
extern OSThread* __osRunQueue;

extern OSTimer* __osTimerList;
extern OSTimer __osBaseTimer;
extern OSTime __osCurrentTime;
extern u32 __osBaseCounter;
extern u32 __osViIntrCount;
extern u32 __osTimerCounter;

extern __OSEventState __osEventStateTab[OS_NUM_EVENTS];

// not sure if this should be here
extern s32 osViClock;
extern void __osTimerServicesInit(void);
extern s32 __osAiDeviceBusy(void);
extern int __osDpDeviceBusy(void);
#endif
