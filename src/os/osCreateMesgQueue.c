#include "libultra_internal.h"

void osCreateMesgQueue(OSMesgQueue* mq, OSMesg* msgBuf, s32 count) {
    mq->mtqueue = (OSThread*) &__osThreadTail.next;
    mq->fullqueue = (OSThread*) &__osThreadTail.next;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = count;
    mq->msg = msgBuf;
}
