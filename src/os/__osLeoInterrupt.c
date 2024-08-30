#include <libultraship.h>
#include "osint.h"
#include "piint.h"
#include "libultra_internal.h"

static void __osLeoResume(void);
static void __osLeoAbnormalResume(void);
extern u8 leoDiskStack[OS_PIM_STACKSIZE]; // technically should have a OS_LEO_STACKSIZE or something..

extern u32 D_800EA5F0;

s32 __osLeoInterrupt() {
    u32 stat;
    volatile u32 pistat;
    u32 bmstat;
    __OSTranxInfo* info;
    __OSBlockInfo* blockInfo;

    if (!D_800EA5F0) {
        return 0;
    }

    info = &__osDiskHandle->transferInfo;
    blockInfo = &info->block[info->blockNum];

    pistat = IO_READ(PI_STATUS_REG);
    if (pistat & PI_STATUS_DMA_BUSY) {
        IO_WRITE(PI_STATUS_REG, PI_STATUS_RESET | PI_STATUS_CLR_INTR);
        WAIT_ON_LEO_IO_BUSY(pistat);
        stat = IO_READ(LEO_STATUS);
        if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
            WAIT_ON_LEO_IO_BUSY(pistat);
            IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        }
        info->errStatus = LEO_ERROR_75;
        __osLeoAbnormalResume();
        return 1;
    }
    WAIT_ON_LEO_IO_BUSY(pistat);

    stat = IO_READ(LEO_STATUS);
    if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
        WAIT_ON_LEO_IO_BUSY(pistat);
        IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        info->errStatus = LEO_ERROR_GOOD;
        return 0;
    }
    if (stat & LEO_STATUS_BUFFER_MANAGER_ERROR) {
        info->errStatus = LEO_ERROR_3;
        __osLeoAbnormalResume();
        return 1;
    }
    if (info->cmdType == LEO_CMD_TYPE_1) {
        if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
            if (info->sectorNum + 1 != info->transferMode * 85) {
                info->errStatus = LEO_ERROR_6;
                __osLeoAbnormalResume();
                return 1;
            }
            IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
            __OSGlobalIntMask |= OS_IM_PI;
            info->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
            return 1;
        }

        blockInfo->dramAddr = (void*) ((u32) blockInfo->dramAddr + blockInfo->sectorSize);
        info->sectorNum++;
        osEPiRawStartDma(__osDiskHandle, OS_WRITE, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
        return 1;
    }
    if (info->cmdType == LEO_CMD_TYPE_0) {
        if (info->transferMode == LEO_SECTOR_MODE) {
            if ((s32) blockInfo->C1ErrNum + 17 < info->sectorNum) {
                info->errStatus = LEO_ERROR_GOOD;
                __osLeoAbnormalResume();
                return 1;
            }
            if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
                info->errStatus = LEO_ERROR_17;
                __osLeoAbnormalResume();
                return 1;
            }
        } else {
            blockInfo->dramAddr = (void*) ((u32) blockInfo->dramAddr + blockInfo->sectorSize);
        }
        bmstat = IO_READ(LEO_BM_STATUS);
        if ((bmstat & LEO_BM_STATUS_C1SINGLE && bmstat & LEO_BM_STATUS_C1DOUBLE) || bmstat & LEO_BM_STATUS_MICRO) {
            if (blockInfo->C1ErrNum > 3) {
                if (info->transferMode != LEO_SECTOR_MODE || info->sectorNum > 0x52) {
                    info->errStatus = LEO_ERROR_17;
                    __osLeoAbnormalResume();
                    return 1;
                }
            } else {
                s32 errNum = blockInfo->C1ErrNum;
                blockInfo->C1ErrSector[errNum] = info->sectorNum + 1;
            }
            blockInfo->C1ErrNum++;
        }

        if (stat & LEO_STATUS_C2_TRANSFER) {
            if (info->sectorNum != 87) {
                info->errStatus = LEO_ERROR_6;
                __osLeoAbnormalResume();
            }
            if (info->transferMode == LEO_TRACK_MODE && info->blockNum == 0) {
                info->blockNum = 1;
                info->sectorNum = -1;
                info->block[1].dramAddr = (void*) ((u32) info->block[1].dramAddr - info->block[1].sectorSize);
            } else {
                IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
                __OSGlobalIntMask |= OS_IM_PI;
            }
            osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_C2_BUFF, blockInfo->C2Addr, blockInfo->sectorSize * 4);
            info->errStatus = LEO_ERROR_GOOD;
            return 1;
        }
        if (info->sectorNum == -1 && info->transferMode == LEO_TRACK_MODE && info->blockNum == 1) {
            __OSBlockInfo* bptr = &info->block[0];
            if (bptr->C1ErrNum == 0) {
                if (((u32*) bptr->C2Addr)[0] | ((u32*) bptr->C2Addr)[1] | ((u32*) bptr->C2Addr)[2] |
                    ((u32*) bptr->C2Addr)[3]) {
                    info->errStatus = LEO_ERROR_6;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }
            info->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
        }
        info->sectorNum++;
        if (stat & LEO_STATUS_DATA_REQUEST) {
            if (info->sectorNum > 0x54) {
                info->errStatus = LEO_ERROR_6;
                __osLeoAbnormalResume();
                return 1;
            }
            osEPiRawStartDma(__osDiskHandle, 0, LEO_SECTOR_BUFF, blockInfo->dramAddr, blockInfo->sectorSize);
            info->errStatus = LEO_ERROR_GOOD;
            return 1;
        }
        if (info->sectorNum <= 0x54) {
            info->errStatus = LEO_ERROR_6;
            __osLeoAbnormalResume();
            return 1;
        }
        return 1;
    }

    info->errStatus = LEO_ERROR_75;

    __osLeoAbnormalResume();
    return 1;
}

static void __osLeoAbnormalResume(void) {
    __OSTranxInfo* info;
    u32 pistat;
    info = &__osDiskHandle->transferInfo;
    WAIT_ON_LEO_IO_BUSY(pistat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_RESET);
    WAIT_ON_LEO_IO_BUSY(pistat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow);
    __osLeoResume();
    IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
    __OSGlobalIntMask |= OS_IM_PI;
}

static void __osLeoResume(void) {
    __OSEventState* es;
    OSMesgQueue* mq;
    s32 last;
    es = &__osEventStateTab[OS_EVENT_PI];
    mq = es->messageQueue;
    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;
    if (mq->mtqueue->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&mq->mtqueue));
    }
}
