#include <libultraship/libultraship.h>

extern "C" {
#include <raphnetraw/src/plugin_front.h>
}

typedef struct {
    /* 0x0 */ u8 dummy;
    /* 0x1 */ u8 txsize;
    /* 0x2 */ u8 rxsize;
    /* 0x3 */ u8 cmd;
    /* 0x4 */ u16 button;
    /* 0x6 */ s8 stick_x;
    /* 0x7 */ s8 stick_y;
} __OSContReadFormat;

u8 __osContLastCmd;
u8 _osContNumControllers;
OSPifRam __osContPifRam;
#define CHNL_ERR(format) ((format.rxsize & CHNL_ERR_MASK) >> 4)

#define RAPHNET_CONTROLLERS 4
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

extern "C" void Raphnet_osContGetInitData(u8* bitpattern, OSContStatus* status)
{
    u8* ptr;
    __OSContRequestHeader response;
    s32 i;
    u8 bits;

    bits = 0;
    ptr = (u8*) __osContPifRam.ram;
    for (i = 0; i < _osContNumControllers; i++, ptr += sizeof(response), status++) {
        response = *((__OSContRequestHeader*) (ptr));
        status->err_no = CHNL_ERR(response);
        if (status->err_no == 0) {
            status->type = response.typel << 8 | response.typeh;
            status->status = response.status;

            bits |= 1 << i;
        }
    }

    *bitpattern = 1;
    status->status |= 1;

}

void __osPackReadData(void);
extern "C" int32_t Raphnet_osContStartReadData(OSMesgQueue* mesg) {
    unsigned char* ptr = (unsigned char*)__osContPifRam.ram;

    __osPackReadData();

    for (int ch = 0; ch < _osContNumControllers; ch++) {
        ReadController(ch, ptr);

        int tx = ptr[0] & 0x3F;
        int rx = ptr[1] & 0x3F;

        ptr += 2 + tx + rx;
    }

    ReadController(-1, NULL); // poll the controllers and set data
    return 0;
}

extern "C" void Raphnet_osContGetReadData(OSContPad* pad) {
    u8* ptr = (u8*) __osContPifRam.ram;
    __OSContReadFormat readformat;
    s32 i;

    for (i = 0; i < _osContNumControllers; i++, ptr += sizeof(readformat), pad++) {
        readformat = *(__OSContReadFormat*) ptr;
        pad->err_no = CHNL_ERR(readformat);

        if (pad->err_no != 0) {
            printf("gamepad error\n");
            continue;
        }

        pad->button = readformat.button;
        pad->stick_x = readformat.stick_x;
        pad->stick_y = readformat.stick_y;
    }
}

void __osPackReadData() {
    u8* ptr = (u8*) __osContPifRam.ram;
    __OSContReadFormat readformat;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(__osContPifRam.ram) + 1; i++) {
        __osContPifRam.ram[i] = 0;
    }

    __osContPifRam.status = CONT_CMD_EXE;
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

void Raphnet_ScanControllers(void) {
    _osContNumControllers = ScanControllers();
}
