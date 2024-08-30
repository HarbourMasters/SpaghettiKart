#include "libultra_internal.h"
#include "controller.h"

extern u8 D_80365D20;
extern OSTimer D_80196548;
extern OSMesgQueue _osContMesgQueue;
extern OSMesg _osContMesgBuff[4];

// exactly the same as osEepromLongRead except for osEepromWrite call
s32 osEepromLongWrite(OSMesgQueue* mq, u8 address, u8* buffer, s32 nbytes) {
    s32 result = 0;
    if (address > 0x40) {
        return -1;
    }

    while (nbytes > 0) {
        result = osEepromWrite(mq, address, buffer);
        if (result != 0) {
            return result;
        }

        nbytes -= EEPROM_BLOCK_SIZE;
        address++;
        buffer += EEPROM_BLOCK_SIZE;
        osSetTimer(&D_80196548, 12000 * osClockRate / 1000000, 0, &_osContMesgQueue, _osContMesgBuff);
        osRecvMesg(&_osContMesgQueue, NULL, OS_MESG_BLOCK);
    }

    return result;
}
