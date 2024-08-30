#include <libultraship.h>
#include "mixer.h"

#pragma GCC optimize("unroll-loops")

#define ROUND_UP_64(v) (((v) + 63) & ~63)
#define ROUND_UP_32(v) (((v) + 31) & ~31)
#define ROUND_UP_16(v) (((v) + 15) & ~15)
#define ROUND_UP_8(v) (((v) + 7) & ~7)
#define ROUND_DOWN_16(v) ((v) & ~0xf)

#ifdef NEW_AUDIO_UCODE
#define BUF_SIZE 2880
#define BUF_U8(a) (rspa.buf.as_u8 + ((a) - 0x450))
#define BUF_S16(a) (rspa.buf.as_s16 + ((a) - 0x450) / sizeof(int16_t))
#else
#define BUF_SIZE 2512
#define BUF_U8(a) (rspa.buf.as_u8 + (a))
#define BUF_S16(a) (rspa.buf.as_s16 + (a) / sizeof(int16_t))
#endif

static struct {
    uint16_t in;
    uint16_t out;
    uint16_t nbytes;

#ifdef NEW_AUDIO_UCODE
    uint16_t vol[2];
    uint16_t rate[2];
    uint16_t vol_wet;
    uint16_t rate_wet;
#else
    int16_t vol[2];

    uint16_t dry_right;
    uint16_t wet_left;
    uint16_t wet_right;

    int16_t target[2];
    int32_t rate[2];

    int16_t vol_dry;
    int16_t vol_wet;
#endif

    ADPCM_STATE* adpcm_loop_state;

    int16_t adpcm_table[8][2][8];

#ifdef NEW_AUDIO_UCODE
    uint16_t filter_count;
    int16_t filter[8];
#endif

    union {
        int16_t as_s16[BUF_SIZE / sizeof(int16_t)];
        uint8_t as_u8[BUF_SIZE];
    } buf;
} rspa;

void aDownsampleHalfImpl(uint16_t n_samples, uint16_t in_addr, uint16_t out_addr) {
    int16_t* in = BUF_S16(in_addr);
    int16_t* out = BUF_S16(out_addr);
    int n = ROUND_UP_8(n_samples);

    do {
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;
        *out++ = *in++;
        in++;

        n -= 8;
    } while (n > 0);
}
