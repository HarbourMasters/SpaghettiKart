#include "libultra_internal.h"
#include "controller.h"
#include <macros.h>

extern u8 __osContLastCmd;
extern u8 _osContNumControllers;

void __osPackReadData(void);
s32 osContStartReadData(OSMesgQueue* mesg) {
    s32 ret = 0;
    s32 i;

    __osSiGetAccess();

    if (__osContLastCmd != CONT_CMD_READ_BUTTON) {
        __osPackReadData();
        ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
        osRecvMesg(mesg, NULL, OS_MESG_BLOCK);
    }

    for (i = 0; i < ARRLEN(__osContPifRam.ramarray) + 1; i++) {
        __osContPifRam.ramarray[i] = CONT_CMD_NOP;
    }

    __osContPifRam.pifstatus = 0;
    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    __osContLastCmd = CONT_CMD_READ_BUTTON;

    __osSiRelAccess();

    return ret;
}
void osContGetReadData(OSContPad* pad) {
    u8* ptr = (u8*) __osContPifRam.ramarray;
    __OSContReadFormat readformat;
    s32 i;

    for (i = 0; i < _osContNumControllers; i++, ptr += sizeof(readformat), pad++) {
        readformat = *(__OSContReadFormat*) ptr;
        pad->errno = CHNL_ERR(readformat);

        if (pad->errno != 0) {
            continue;
        }

        pad->button = readformat.button;
        pad->stick_x = readformat.stick_x;
        pad->stick_y = readformat.stick_y;
    }
}
void __osPackReadData() {
    u8* ptr = (u8*) __osContPifRam.ramarray;
    __OSContReadFormat readformat;
    s32 i;

    for (i = 0; i < ARRLEN(__osContPifRam.ramarray) + 1; i++) {
        __osContPifRam.ramarray[i] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    readformat.dummy = CONT_CMD_NOP;
    readformat.txsize = CONT_CMD_READ_BUTTON_TX;
    readformat.rxsize = CONT_CMD_READ_BUTTON_RX;
    readformat.cmd = CONT_CMD_READ_BUTTON;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;

    for (i = 0; i < _osContNumControllers; i++) {
        *(__OSContReadFormat*) ptr = readformat;
        ptr += sizeof(readformat);
    }
    *ptr = CONT_CMD_END;
}
