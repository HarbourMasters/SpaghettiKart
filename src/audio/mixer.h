#ifndef MIXER_H
#define MIXER_H

#include <libultraship.h>

void aDownsampleHalfImpl(uint16_t n_samples, uint16_t in_addr, uint16_t out_addr);

#undef aDownsampleHalf
#define aDownsampleHalf(pkt, nSamples, i, o) aDownsampleHalfImpl(nSamples, i, o)

#endif
