#define MK64

#include "libultra_internal.h"
#include "macros.h"

#include "new_func.h"

void osYieldThread(void);

void __osDevMgrMain(void* args) {
    OSIoMesg* mb;
    OSMesg em;
    OSMesg dummy;
    s32 ret;
    OSMgrArgs* sp34;
    UNUSED u32 sp30;
    u32 sp2c;
    __OSBlockInfo* sp28;
    __OSTranxInfo* sp24;
    sp30 = 0;
    sp2c = 0;
    mb = NULL;
    ret = 0;
    sp34 = (OSMgrArgs*) args;
    while (true) {
        osRecvMesg(sp34->cmdQueue, (OSMesg) &mb, OS_MESG_BLOCK);
        if (mb->piHandle != NULL && mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 || mb->piHandle->transferInfo.cmdType == 1)) {
            sp24 = &mb->piHandle->transferInfo;
            sp28 = &sp24->block[sp24->blockNum];
            sp24->sectorNum = -1;
            if (sp24->transferMode != 3) {
                sp28->dramAddr = (void*) ((u32) sp28->dramAddr - sp28->sectorSize);
            }
            if (sp24->transferMode == 2 && mb->piHandle->transferInfo.cmdType == 0) {
                sp2c = 1;
            } else {
                sp2c = 0;
            }
            osRecvMesg(sp34->accessQueue, &dummy, OS_MESG_BLOCK);
            __osResetGlobalIntMask(0x00100401); // remove magic constant!
            __osEPiRawWriteIo(mb->piHandle, 0x05000510, (sp24->bmCtlShadow | 0x80000000));
            while (true) {
                osRecvMesg(sp34->eventQueue, &em, OS_MESG_BLOCK);
                sp30 = osSendMesg(mb->hdr.retQueue, (OSMesg) mb, OS_MESG_NOBLOCK);
                if (sp2c != 1 || mb->piHandle->transferInfo.errStatus != 0) {
                    break;
                }
                sp2c = 0;
            }
            osSendMesg(sp34->accessQueue, NULL, OS_MESG_NOBLOCK);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                osYieldThread();
            }
        } else {
            switch (mb->hdr.type) {
                case 11:
                    osRecvMesg(sp34->accessQueue, &dummy, OS_MESG_BLOCK);
                    ret = sp34->piDmaCallback(OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 12:
                    osRecvMesg(sp34->accessQueue, &dummy, OS_MESG_BLOCK);
                    ret = sp34->piDmaCallback(OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 15:
                    osRecvMesg(sp34->accessQueue, &dummy, OS_MESG_BLOCK);
                    ret = sp34->epiDmaCallback(mb->piHandle, OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 16:
                    osRecvMesg(sp34->accessQueue, &dummy, OS_MESG_BLOCK);
                    ret = sp34->epiDmaCallback(mb->piHandle, OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 10:
                    osSendMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
                    ret = -1;
                    break;
                    break;
                default:
                    ret = -1;
                    break;
            }
            if (ret == 0) {
                osRecvMesg(sp34->eventQueue, &em, OS_MESG_BLOCK);
                sp30 = osSendMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
                osSendMesg(sp34->accessQueue, NULL, OS_MESG_NOBLOCK);
            }
        }
    }
}

#undef MK64
