#include "libultra_internal.h"

NO_REORDER OSThread* __osThreadTail = NULL;
NO_REORDER u32 __osTest = -1;
NO_REORDER OSThread* __osRunQueue = (OSThread*) &__osThreadTail;
NO_REORDER OSThread* __osActiveQueue = (OSThread*) &__osThreadTail;
OSThread* __osRunningThread = NULL;
OSThread* __osFaultedThread = NULL;

void __osDequeueThread(OSThread** queue, OSThread* thread) {
    register OSThread** a2;
    register OSThread* a3;
    a2 = queue;
    a3 = *a2;
    while (a3 != NULL) {
        if (a3 == thread) {
            *a2 = thread->next;
            return;
        }
        a2 = &a3->next;
        a3 = *a2;
    }
}
