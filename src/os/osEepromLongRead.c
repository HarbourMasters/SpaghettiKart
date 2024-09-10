#include "libultra_internal.h"

extern u64 osClockRate;
extern u8 D_80365D20;
extern u8 _osContNumControllers;
extern OSTimer D_80196548; // not sure what this is yet
extern OSMesgQueue _osContMesgQueue;
extern OSMesg _osContMesgBuff[4];

s32 osEepromLongRead(OSMesgQueue* mq, u8 address, u8* buffer, s32 nbytes) {
    s32 status = 0;
    if (address > 0x40) {
        return -1;
    }

    while (nbytes > 0) {
        status = osEepromRead(mq, address, buffer);
        if (status != 0) {
            return status;
        }

        nbytes -= EEPROM_BLOCK_SIZE;
        address++;
        buffer += EEPROM_BLOCK_SIZE;
        osSetTimer(&D_80196548, 12000 * osClockRate / 1000000, 0, &_osContMesgQueue, _osContMesgBuff);
        osRecvMesg(&_osContMesgQueue, NULL, OS_MESG_BLOCK);
    }

    return status;
}
