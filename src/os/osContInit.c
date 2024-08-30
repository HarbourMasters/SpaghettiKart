#include "libultra_internal.h"
#include "osContInternal.h"

void __osPackRequestData(u8);
void __osContGetInitData(u8*, OSContStatus*);

u32 _osContInitialized = 0;

extern u64 osClockRate;

// these probably belong in EEPROMlongread or something
u8 __osContLastCmd;
u8 _osContNumControllers;
OSTimer D_80196548;
OSMesgQueue _osContMesgQueue;
OSMesg _osContMesgBuff[4];
s32 osContInit(OSMesgQueue* mq, u8* bitpattern, OSContStatus* status) {
    OSMesg mesg;
    u32 ret = 0;
    OSTime currentTime;
    OSTimer timer;
    OSMesgQueue timerMesgQueue;

    if (_osContInitialized) {
        return 0;
    }
    _osContInitialized = 1;
    currentTime = osGetTime();
    if (500000 * osClockRate / 1000000 > currentTime) {
        osCreateMesgQueue(&timerMesgQueue, &mesg, 1);
        osSetTimer(&timer, 500000 * osClockRate / 1000000 - currentTime, 0, &timerMesgQueue, &mesg);
        osRecvMesg(&timerMesgQueue, &mesg, OS_MESG_BLOCK);
    }
    //! @todo figure out what it means
    _osContNumControllers = 4;
    __osPackRequestData(0);
    ret = __osSiRawStartDma(OS_WRITE, _osContCmdBuf);
    osRecvMesg(mq, &mesg, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, _osContCmdBuf);
    osRecvMesg(mq, &mesg, OS_MESG_BLOCK);
    __osContGetInitData(bitpattern, status);
    __osContLastCmd = 0;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&_osContMesgQueue, _osContMesgBuff, 1);
    return ret;
}
void __osContGetInitData(u8* bitpattern, OSContStatus* status) {
    OSContPackedRequest* cmdBufPtr;
    OSContPackedRequest response;
    s32 i;
    u8 sp7;

    sp7 = 0;
    cmdBufPtr = &(_osContCmdBuf[0].request);
    for (i = 0; i < _osContNumControllers; i++, cmdBufPtr++, status++) {
        response = *(OSContPackedRequest*) cmdBufPtr;
        status->errnum = (response.rxLen & 0xc0) >> 4;
        if (status->errnum == 0) {
            status->type = response.data2 << 8 | response.data1;
            status->status = response.data3;

            sp7 |= 1 << i;
        }
    }
    *bitpattern = sp7;
}
void __osPackRequestData(u8 command) {
    OSContPackedRequest* cmdBufPtr;
    OSContPackedRequest request;
    s32 i;

    // some kind of weird zeroing code
    for (i = 0; i < 0x10; i++) {
        *((u32*) &_osContCmdBuf + i) = 0;
    }

    _osContPifCtrl = 1;
    cmdBufPtr = &_osContCmdBuf[0].request;
    request.padOrEnd = 255;
    request.txLen = 1;
    request.rxLen = 3;
    request.command = command;
    request.data1 = 255;
    request.data2 = 255;
    request.data3 = 255;
    request.data4 = 255;

    for (i = 0; i < _osContNumControllers; i++) {
        *cmdBufPtr++ = request;
    }
    cmdBufPtr->padOrEnd = 254;
}
