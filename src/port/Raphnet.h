#ifndef PORT_RAPHNET_H
#define PORT_RAPHNET_H

#ifdef __cplusplus
extern "C" {
#endif
#include "libultraship/libultra/message.h"

void Raphnet_osContGetInitData(u8* bitpattern, OSContStatus* status);
int32_t Raphnet_osContStartReadData(OSMesgQueue* mesg);
void Raphnet_osContGetReadData(OSContPad* pad);

#ifdef __cplusplus
}
#endif

#endif // PORT_RAPHNET_H
