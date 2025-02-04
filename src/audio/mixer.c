#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <align_asset_macro.h>

#include "mixer.h"

#ifndef __clang__
#pragma GCC optimize("unroll-loops")
#endif

#if defined(__SSE2__) || defined(__aarch64__)
#define SSE2_AVAILABLE
#else
#pragma message("Warning: SSE2 support is not available. Code will not compile")
#endif

#if defined(__SSE2__)
#include <emmintrin.h>
#elif defined(__aarch64__)
#include "sse2neon.h"
#endif

#ifdef SSE2_AVAILABLE
typedef struct {
    __m128i lo, hi;
} m256i;

static m256i m256i_mul_epi16(__m128i a, __m128i b) {
    m256i res;
    res.lo = _mm_mullo_epi16(a, b);
    res.hi = _mm_mulhi_epi16(a, b);

    m256i ret;
    ret.lo = _mm_unpacklo_epi16(res.lo, res.hi);
    ret.hi = _mm_unpackhi_epi16(res.lo, res.hi);
    return ret;
}

static m256i m256i_add_m256i_epi32(m256i a, m256i b) {
    m256i res;
    res.lo = _mm_add_epi32(a.lo, b.lo);
    res.hi = _mm_add_epi32(a.hi, b.hi);
    return res;
}

static m256i m256i_add_m128i_epi32(m256i a, __m128i b) {
    m256i res;
    res.lo = _mm_add_epi32(a.lo, b);
    res.hi = _mm_add_epi32(a.hi, b);
    return res;
}

static m256i m256i_srai(m256i a, int b) {
    m256i res;
    res.lo = _mm_srai_epi32(a.lo, b);
    res.hi = _mm_srai_epi32(a.hi, b);
    return res;
}

static __m128i m256i_clamp_to_m128i(m256i a) {
    return _mm_packs_epi32(a.lo, a.hi);
}
#endif

#define ROUND_UP_64(v) (((v) + 63) & ~63)
#define ROUND_UP_32(v) (((v) + 31) & ~31)
#define ROUND_UP_16(v) (((v) + 15) & ~15)
#define ROUND_UP_8(v) (((v) + 7) & ~7)
#define ROUND_DOWN_16(v) ((v) & ~0xf)

// #define DMEM_BUF_SIZE (0x1000 - 0x0330 - 0x10 - 0x40)
#define DMEM_BUF_SIZE 0x17D0
#define BUF_U8(a) (rspa.buf.as_u8 + (a))
#define BUF_S16(a) (rspa.buf.as_s16 + (a) / sizeof(int16_t))

static struct {
    uint16_t in;
    uint16_t out;
    uint16_t nbytes;

    uint16_t vol[2];
    uint16_t rate[2];
    uint16_t vol_wet;
    uint16_t rate_wet;

    ADPCM_STATE* adpcm_loop_state;

    int16_t adpcm_table[8][2][8];

    uint16_t filter_count;
    int16_t filter[8];

    union {
        int16_t as_s16[DMEM_BUF_SIZE / sizeof(int16_t)];
        uint8_t as_u8[DMEM_BUF_SIZE];
    } buf;
} rspa;

static int16_t resample_table[64][4] = {
    { 0x0c39, 0x66ad, 0x0d46, 0xffdf }, { 0x0b39, 0x6696, 0x0e5f, 0xffd8 }, { 0x0a44, 0x6669, 0x0f83, 0xffd0 },
    { 0x095a, 0x6626, 0x10b4, 0xffc8 }, { 0x087d, 0x65cd, 0x11f0, 0xffbf }, { 0x07ab, 0x655e, 0x1338, 0xffb6 },
    { 0x06e4, 0x64d9, 0x148c, 0xffac }, { 0x0628, 0x643f, 0x15eb, 0xffa1 }, { 0x0577, 0x638f, 0x1756, 0xff96 },
    { 0x04d1, 0x62cb, 0x18cb, 0xff8a }, { 0x0435, 0x61f3, 0x1a4c, 0xff7e }, { 0x03a4, 0x6106, 0x1bd7, 0xff71 },
    { 0x031c, 0x6007, 0x1d6c, 0xff64 }, { 0x029f, 0x5ef5, 0x1f0b, 0xff56 }, { 0x022a, 0x5dd0, 0x20b3, 0xff48 },
    { 0x01be, 0x5c9a, 0x2264, 0xff3a }, { 0x015b, 0x5b53, 0x241e, 0xff2c }, { 0x0101, 0x59fc, 0x25e0, 0xff1e },
    { 0x00ae, 0x5896, 0x27a9, 0xff10 }, { 0x0063, 0x5720, 0x297a, 0xff02 }, { 0x001f, 0x559d, 0x2b50, 0xfef4 },
    { 0xffe2, 0x540d, 0x2d2c, 0xfee8 }, { 0xffac, 0x5270, 0x2f0d, 0xfedb }, { 0xff7c, 0x50c7, 0x30f3, 0xfed0 },
    { 0xff53, 0x4f14, 0x32dc, 0xfec6 }, { 0xff2e, 0x4d57, 0x34c8, 0xfebd }, { 0xff0f, 0x4b91, 0x36b6, 0xfeb6 },
    { 0xfef5, 0x49c2, 0x38a5, 0xfeb0 }, { 0xfedf, 0x47ed, 0x3a95, 0xfeac }, { 0xfece, 0x4611, 0x3c85, 0xfeab },
    { 0xfec0, 0x4430, 0x3e74, 0xfeac }, { 0xfeb6, 0x424a, 0x4060, 0xfeaf }, { 0xfeaf, 0x4060, 0x424a, 0xfeb6 },
    { 0xfeac, 0x3e74, 0x4430, 0xfec0 }, { 0xfeab, 0x3c85, 0x4611, 0xfece }, { 0xfeac, 0x3a95, 0x47ed, 0xfedf },
    { 0xfeb0, 0x38a5, 0x49c2, 0xfef5 }, { 0xfeb6, 0x36b6, 0x4b91, 0xff0f }, { 0xfebd, 0x34c8, 0x4d57, 0xff2e },
    { 0xfec6, 0x32dc, 0x4f14, 0xff53 }, { 0xfed0, 0x30f3, 0x50c7, 0xff7c }, { 0xfedb, 0x2f0d, 0x5270, 0xffac },
    { 0xfee8, 0x2d2c, 0x540d, 0xffe2 }, { 0xfef4, 0x2b50, 0x559d, 0x001f }, { 0xff02, 0x297a, 0x5720, 0x0063 },
    { 0xff10, 0x27a9, 0x5896, 0x00ae }, { 0xff1e, 0x25e0, 0x59fc, 0x0101 }, { 0xff2c, 0x241e, 0x5b53, 0x015b },
    { 0xff3a, 0x2264, 0x5c9a, 0x01be }, { 0xff48, 0x20b3, 0x5dd0, 0x022a }, { 0xff56, 0x1f0b, 0x5ef5, 0x029f },
    { 0xff64, 0x1d6c, 0x6007, 0x031c }, { 0xff71, 0x1bd7, 0x6106, 0x03a4 }, { 0xff7e, 0x1a4c, 0x61f3, 0x0435 },
    { 0xff8a, 0x18cb, 0x62cb, 0x04d1 }, { 0xff96, 0x1756, 0x638f, 0x0577 }, { 0xffa1, 0x15eb, 0x643f, 0x0628 },
    { 0xffac, 0x148c, 0x64d9, 0x06e4 }, { 0xffb6, 0x1338, 0x655e, 0x07ab }, { 0xffbf, 0x11f0, 0x65cd, 0x087d },
    { 0xffc8, 0x10b4, 0x6626, 0x095a }, { 0xffd0, 0x0f83, 0x6669, 0x0a44 }, { 0xffd8, 0x0e5f, 0x6696, 0x0b39 },
    { 0xffdf, 0x0d46, 0x66ad, 0x0c39 }
};

static inline int16_t clamp16(int32_t v) {
    if (v < -0x8000) {
        return -0x8000;
    } else if (v > 0x7fff) {
        return 0x7fff;
    }
    return (int16_t) v;
}

static inline int32_t clamp32(int64_t v) {
    if (v < -0x7fffffff - 1) {
        return -0x7fffffff - 1;
    } else if (v > 0x7fffffff) {
        return 0x7fffffff;
    }
    return (int32_t) v;
}

void aClearBufferImpl(uint16_t addr, int nbytes) {
    nbytes = ROUND_UP_16(nbytes);
    memset(BUF_U8(addr), 0, nbytes);
}

void aLoadBufferImpl(const void* source_addr, uint16_t dest_addr, uint16_t nbytes) {
#if __SANITIZE_ADDRESS__
    for (size_t i = 0; i < ROUND_DOWN_16(nbytes); i++) {
        BUF_U8(dest_addr)[i] = ((const unsigned char*) source_addr)[i];
    }
#else
    memcpy(BUF_U8(dest_addr), source_addr, ROUND_DOWN_16(nbytes));
#endif
}

void aSaveBufferImpl(uint16_t source_addr, int16_t* dest_addr, uint16_t nbytes) {
    // printf("source_addr: %x\n dest_addr; %x\n nbytes: %d\n", source_addr, dest_addr, nbytes);
    // if (nbytes > 704) {nbytes = 704;}
    memcpy(dest_addr, BUF_S16(source_addr), ROUND_DOWN_16(nbytes));
}

void aLoadADPCMImpl(int num_entries_times_16, const int16_t* book_source_addr) {
    memcpy(rspa.adpcm_table, book_source_addr, num_entries_times_16);
}

void aSetBufferImpl(uint8_t flags, uint16_t in, uint16_t out, uint16_t nbytes) {
    rspa.in = in;
    rspa.out = out;
    rspa.nbytes = nbytes;
}

void aInterleaveImpl(uint16_t left, uint16_t right) {
    int count = ROUND_UP_16(rspa.nbytes) / sizeof(int16_t) / 8;
    int16_t* l = BUF_S16(left);
    int16_t* r = BUF_S16(right);
    int16_t* d = BUF_S16(rspa.out);

    while (count > 0) {
        int16_t l0 = *l++;
        int16_t l1 = *l++;
        int16_t l2 = *l++;
        int16_t l3 = *l++;
        int16_t l4 = *l++;
        int16_t l5 = *l++;
        int16_t l6 = *l++;
        int16_t l7 = *l++;
        int16_t r0 = *r++;
        int16_t r1 = *r++;
        int16_t r2 = *r++;
        int16_t r3 = *r++;
        int16_t r4 = *r++;
        int16_t r5 = *r++;
        int16_t r6 = *r++;
        int16_t r7 = *r++;
        *d++ = l0;
        *d++ = r0;
        *d++ = l1;
        *d++ = r1;
        *d++ = l2;
        *d++ = r2;
        *d++ = l3;
        *d++ = r3;
        *d++ = l4;
        *d++ = r4;
        *d++ = l5;
        *d++ = r5;
        *d++ = l6;
        *d++ = r6;
        *d++ = l7;
        *d++ = r7;
        --count;
    }
}

void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes) {
    nbytes = ROUND_UP_16(nbytes);
    memmove(BUF_U8(out_addr), BUF_U8(in_addr), nbytes);
}

void aSetLoopImpl(ADPCM_STATE* adpcm_loop_state) {
    rspa.adpcm_loop_state = adpcm_loop_state;
}

// https://godbolt.org/z/eMo5ad6n6

#ifndef SSE2_AVAILABLE

void aADPCMdecImpl(uint8_t flags, ADPCM_STATE state) {
    uint8_t* in = BUF_U8(rspa.in);
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_32(rspa.nbytes);
    if (flags & A_INIT) {
        memset(out, 0, 16 * sizeof(int16_t));
    } else if (flags & A_LOOP) {
        memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
    } else {
        memcpy(out, state, 16 * sizeof(int16_t));
    }
    out += 16;

    while (nbytes > 0) {
        int shift = *in >> 4;          // should be in 0..12 or 0..14
        int table_index = *in++ & 0xf; // should be in 0..7
        int16_t(*tbl)[8] = rspa.adpcm_table[table_index];
        int i;

        for (i = 0; i < 2; i++) {
            int16_t ins[8];
            int16_t prev1 = out[-1];
            int16_t prev2 = out[-2];
            int j, k;
            for (j = 0; j < 4; j++) {
                ins[j * 2] = (((*in >> 4) << 28) >> 28) << shift;
                ins[j * 2 + 1] = (((*in++ & 0xf) << 28) >> 28) << shift;
            }
            for (j = 0; j < 8; j++) {
                int32_t acc = tbl[0][j] * prev2 + tbl[1][j] * prev1 + (ins[j] << 11);
                for (k = 0; k < j; k++) {
                    acc += tbl[1][((j - k) - 1)] * ins[k];
                }
                acc >>= 11;
                *out++ = clamp16(acc);
            }
        }
        nbytes -= 16 * sizeof(int16_t);
    }
    memcpy(state, out - 16, 16 * sizeof(int16_t));
}

#else

static uint16_t lower_bit[] = {
    0xf,
    0xf,
    0xf,
    0xf,
};

void aADPCMdecImpl(uint8_t flags, ADPCM_STATE state) {
    uint8_t* in = BUF_U8(rspa.in);
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_32(rspa.nbytes);
    if (flags & A_INIT) {
        memset(out, 0, 16 * sizeof(int16_t));
    } else if (flags & A_LOOP) {
        memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
    } else {
        memcpy(out, state, 16 * sizeof(int16_t));
    }
    out += 16;

    __m128i mask = _mm_loadl_epi64((__m128i*) lower_bit);

    while (nbytes > 0) {
        int shift = *in >> 4; // should be in 0..12 or 0..14
        __m128i shift_vec = _mm_set1_epi16(shift);
        int table_index = *in++ & 0xf; // should be in 0..7
        int16_t(*tbl)[8] = rspa.adpcm_table[table_index];

        for (int i = 0; i < 2; i++) {
            int16_t ins[8];
            int16_t prev1 = out[-1];
            int16_t prev2 = out[-2];
            __m128i prev1_vec = _mm_set1_epi16(prev1);
            __m128i prev2_vec = _mm_set1_epi16(prev2);

            __m128i ins_vec = _mm_loadu_si32((__m128i*) in);
            ins_vec = _mm_unpacklo_epi8(ins_vec, _mm_setzero_si128());
            __m128i in_vec_up4bit = _mm_srli_epi16(ins_vec, 4);
            __m128i in_vec_lower4bit = _mm_and_si128(ins_vec, mask);
            ins_vec = _mm_unpacklo_epi16(in_vec_up4bit, in_vec_lower4bit);
            ins_vec = _mm_slli_epi16(ins_vec, 12);
            ins_vec = _mm_srai_epi16(ins_vec, 12);
            ins_vec = _mm_slli_epi16(ins_vec, shift);
            _mm_storeu_si128((__m128i*) ins, ins_vec);

            in += 4;
            for (int j = 0; j < 2; j++) {
                __m128i tbl0_vec = _mm_loadu_si64((__m128i*) (tbl[0] + (j * 4)));
                __m128i tbl1_vec = _mm_loadu_si64((__m128i*) (tbl[1] + (j * 4)));

                m256i res;
                res.lo = _mm_mullo_epi16(tbl0_vec, prev2_vec);
                res.hi = _mm_mulhi_epi16(tbl0_vec, prev2_vec);

                tbl0_vec = _mm_unpacklo_epi16(res.lo, res.hi);

                res.lo = _mm_mullo_epi16(tbl1_vec, prev1_vec);
                res.hi = _mm_mulhi_epi16(tbl1_vec, prev1_vec);

                tbl1_vec = _mm_unpacklo_epi16(res.lo, res.hi);
                __m128i acc_vec = _mm_add_epi32(tbl0_vec, tbl1_vec);

                __m128i shift_ins = _mm_srai_epi32(j ? _mm_unpackhi_epi16(_mm_setzero_si128(), ins_vec)
                                                     : _mm_unpacklo_epi16(_mm_setzero_si128(), ins_vec),
                                                   5);
                acc_vec = _mm_add_epi32(acc_vec, shift_ins);

                tbl1_vec = _mm_loadu_si128((__m128i*) tbl[1]);
                if (j == 0) {
                    tbl1_vec = _mm_slli_si128(tbl1_vec, (1 - 0) * 8 + 2);
                } else {
                    tbl1_vec = _mm_slli_si128(tbl1_vec, (1 - 1) * 8 + 2);
                }
                for (int k = 0; k < ((j + 1) * 4); k++) {
                    __m128i ins_vec2 = _mm_set1_epi16(ins[k]);
                    res.lo = _mm_mullo_epi16(tbl1_vec, ins_vec2);
                    res.hi = _mm_mulhi_epi16(tbl1_vec, ins_vec2);

                    __m128i mult = _mm_unpackhi_epi16(res.lo, res.hi);
                    acc_vec = _mm_add_epi32(acc_vec, mult);
                    tbl1_vec = _mm_slli_si128(tbl1_vec, 2);
                }

                acc_vec = _mm_srai_epi32(acc_vec, 11);
                acc_vec = _mm_packs_epi32(acc_vec, _mm_setzero_si128());
                _mm_storeu_si64((__m128*) out, acc_vec);
                out += 4;
            }
        }
        nbytes -= 16 * sizeof(int16_t);
    }
    memcpy(state, out - 16, 16 * sizeof(int16_t));
}

#endif

// https://godbolt.org/z/jsYM3zooP

#ifndef SSE2_AVAILABLE

void aResampleImpl(uint8_t flags, uint16_t pitch, RESAMPLE_STATE state) {
    int16_t tmp[32];
    int16_t* in_initial = BUF_S16(rspa.in);
    int16_t* in = in_initial;
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_16(rspa.nbytes);
    uint32_t pitch_accumulator;
    int i;
    int16_t* tbl;
    int32_t sample;

    if (flags & A_INIT) {
        memset(tmp, 0, 5 * sizeof(int16_t));
    } else {
        memcpy(tmp, state, 16 * sizeof(int16_t));
    }
    if (flags & 2) {
        memcpy(in - 8, tmp + 8, 8 * sizeof(int16_t));
        in -= tmp[5] / sizeof(int16_t);
    }
    in -= 4;
    pitch_accumulator = (uint16_t) tmp[4];
    memcpy(in, tmp, 4 * sizeof(int16_t));

    do {
        for (i = 0; i < 8; i++) {
            tbl = resample_table[pitch_accumulator * 64 >> 16];
            sample = ((in[0] * tbl[0] + 0x4000) >> 15) + ((in[1] * tbl[1] + 0x4000) >> 15) +
                     ((in[2] * tbl[2] + 0x4000) >> 15) + ((in[3] * tbl[3] + 0x4000) >> 15);
            *out++ = clamp16(sample);

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;
        }
        nbytes -= 8 * sizeof(int16_t);
    } while (nbytes > 0);

    state[4] = (int16_t) pitch_accumulator;
    memcpy(state, in, 4 * sizeof(int16_t));
    i = (in - in_initial + 4) & 7;
    in -= i;
    if (i != 0) {
        i = -8 - i;
    }
    state[5] = i;
    memcpy(state + 8, in, 8 * sizeof(int16_t));
}

#else

static const ALIGN_ASSET(16) int32_t x4000[4] = {
    0x4000,
    0x4000,
    0x4000,
    0x4000,
};

static void mm128_transpose(__m128i* r0, __m128i* r1, __m128i* r2, __m128i* r3) {
    __m128 tmp0, tmp1, tmp2, tmp3;
    __m128 row0, row1, row2, row3;

    row0 = _mm_castsi128_ps(*r0);
    row1 = _mm_castsi128_ps(*r1);
    row2 = _mm_castsi128_ps(*r2);
    row3 = _mm_castsi128_ps(*r3);

    tmp0 = _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(2, 0, 2, 0)); // 0 2 4 6
    tmp1 = _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(3, 1, 3, 1)); // 1 3 5 7
    tmp2 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(2, 0, 2, 0)); // 8 a c e
    tmp3 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(3, 1, 3, 1)); // 9 b d f

    row0 = _mm_shuffle_ps(tmp0, tmp2, _MM_SHUFFLE(2, 0, 2, 0)); // 0 4 8 c
    row1 = _mm_shuffle_ps(tmp1, tmp3, _MM_SHUFFLE(2, 0, 2, 0)); // 1 5 9 d
    row2 = _mm_shuffle_ps(tmp0, tmp2, _MM_SHUFFLE(3, 1, 3, 1)); // 2 6 a e
    row3 = _mm_shuffle_ps(tmp1, tmp3, _MM_SHUFFLE(3, 1, 3, 1)); // 3 7 b f

    *r0 = _mm_castps_si128(row0);
    *r1 = _mm_castps_si128(row1);
    *r2 = _mm_castps_si128(row2);
    *r3 = _mm_castps_si128(row3);
}

static __m128i move_two_4x16(int16_t* a, int16_t* b) {
    return _mm_set_epi64(_mm_movepi64_pi64(_mm_loadl_epi64((__m128i*) a)),
                         _mm_movepi64_pi64(_mm_loadl_epi64((__m128i*) b)));
}

void aResampleImpl(uint8_t flags, uint16_t pitch, RESAMPLE_STATE state) {
    int16_t tmp[32];
    int16_t* in_initial = BUF_S16(rspa.in);
    int16_t* in = in_initial;
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_16(rspa.nbytes);
    uint32_t pitch_accumulator;
    int i;

    if (flags & A_INIT) {
        memset(tmp, 0, 5 * sizeof(int16_t));
    } else {
        memcpy(tmp, state, 16 * sizeof(int16_t));
    }
    if (flags & 2) {
        memcpy(in - 8, tmp + 8, 8 * sizeof(int16_t));
        in -= tmp[5] / sizeof(int16_t);
    }
    in -= 4;
    pitch_accumulator = (uint16_t) tmp[4];
    memcpy(in, tmp, 4 * sizeof(int16_t));

    __m128i x4000Vec = _mm_load_si128((__m128i*) x4000);

    do {
        for (i = 0; i < 2; i++) {
            int16_t* tbl0 = resample_table[pitch_accumulator * 64 >> 16];

            int16_t* in0 = in;

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;

            int16_t* tbl1 = resample_table[pitch_accumulator * 64 >> 16];

            int16_t* in1 = in;

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;

            int16_t* tbl2 = resample_table[pitch_accumulator * 64 >> 16];

            int16_t* in2 = in;

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;

            int16_t* tbl3 = resample_table[pitch_accumulator * 64 >> 16];

            int16_t* in3 = in;

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;

            __m128i vec_in0 = move_two_4x16(in1, in0);

            __m128i vec_tbl0 = move_two_4x16(tbl1, tbl0);

            __m128i vec_in1 = move_two_4x16(in3, in2);

            __m128i vec_tbl1 = move_two_4x16(tbl3, tbl2);

            // we multiply in by tbl

            m256i res;
            res.lo = _mm_mullo_epi16(vec_in0, vec_tbl0);
            res.hi = _mm_mulhi_epi16(vec_in0, vec_tbl0);

            __m128i out0_vec = _mm_unpacklo_epi16(res.lo, res.hi);
            __m128i out1_vec = _mm_unpackhi_epi16(res.lo, res.hi);

            res.lo = _mm_mullo_epi16(vec_in1, vec_tbl1);
            res.hi = _mm_mulhi_epi16(vec_in1, vec_tbl1);

            __m128i out2_vec = _mm_unpacklo_epi16(res.lo, res.hi);
            __m128i out3_vec = _mm_unpackhi_epi16(res.lo, res.hi);

            // transpose to more easily make a sum at the end

            mm128_transpose(&out0_vec, &out1_vec, &out2_vec, &out3_vec);

            // add 0x4000

            out0_vec = _mm_add_epi32(out0_vec, x4000Vec);
            out1_vec = _mm_add_epi32(out1_vec, x4000Vec);
            out2_vec = _mm_add_epi32(out2_vec, x4000Vec);
            out3_vec = _mm_add_epi32(out3_vec, x4000Vec);

            // shift by 15

            out0_vec = _mm_srai_epi32(out0_vec, 15);
            out1_vec = _mm_srai_epi32(out1_vec, 15);
            out2_vec = _mm_srai_epi32(out2_vec, 15);
            out3_vec = _mm_srai_epi32(out3_vec, 15);

            // sum all to make sample
            __m128i sample_vec = _mm_add_epi32(_mm_add_epi32(_mm_add_epi32(out0_vec, out1_vec), out2_vec), out3_vec);

            // at the end we do this below but four time
            // sample = ((in[0] * tbl[0] + 0x4000) >> 15) + ((in[1] * tbl[1] + 0x4000) >> 15) +
            //          ((in[2] * tbl[2] + 0x4000) >> 15) + ((in[3] * tbl[3] + 0x4000) >> 15);
            sample_vec = _mm_packs_epi32(sample_vec, _mm_setzero_si128());
            _mm_storeu_si64(out, sample_vec);

            out += 4;
        }
        nbytes -= 8 * sizeof(int16_t);
    } while (nbytes > 0);

    state[4] = (int16_t) pitch_accumulator;
    memcpy(state, in, 4 * sizeof(int16_t));
    i = (in - in_initial + 4) & 7;
    in -= i;
    if (i != 0) {
        i = -8 - i;
    }
    state[5] = i;
    memcpy(state + 8, in, 8 * sizeof(int16_t));
}

#endif

void aEnvSetup1Impl(uint8_t initial_vol_wet, uint16_t rate_wet, uint16_t rate_left, uint16_t rate_right) {
    rspa.vol_wet = (uint16_t) (initial_vol_wet << 8);
    rspa.rate_wet = 0;
    rspa.rate[0] = rate_left;
    rspa.rate[1] = rate_right;
}

void aEnvSetup2Impl(uint16_t initial_vol_left, uint16_t initial_vol_right) {
    rspa.vol[0] = initial_vol_left;
    rspa.vol[1] = initial_vol_right;
}

// https://godbolt.org/z/ohhbY96En

#ifndef SSE2_AVAILABLE

void aEnvMixerImpl(uint16_t in_addr, uint16_t n_samples, bool swap_reverb, bool neg_left, bool neg_right,
                   uint16_t dry_left_addr, uint16_t dry_right_addr, uint16_t wet_left_addr, uint16_t wet_right_addr) {
    swap_reverb = false;
    int16_t* in = BUF_S16(in_addr);
    int16_t* dry[2] = { BUF_S16(dry_left_addr), BUF_S16(dry_right_addr) };
    int16_t* wet[2] = { BUF_S16(wet_left_addr), BUF_S16(wet_right_addr) };
    int16_t negs[2] = { neg_left ? -1 : 0, neg_right ? -1 : 0 };
    int swapped[2] = { swap_reverb ? 1 : 0, swap_reverb ? 0 : 1 };
    int n = ROUND_UP_16(n_samples);

    uint16_t vols[2] = { rspa.vol[0], rspa.vol[1] };
    uint16_t rates[2] = { rspa.rate[0], rspa.rate[1] };
    uint16_t vol_wet = rspa.vol_wet;
    uint16_t rate_wet = rspa.rate_wet;

    do {
        for (int i = 0; i < 8; i++) {
            int16_t samples[2] = { *in, *in };
            in++;
            for (int j = 0; j < 2; j++) {
                samples[j] = (samples[j] * vols[j] >> 16) ^ negs[j];
                *dry[j] = clamp16(*dry[j] + samples[j]);
                dry[j]++;
                *wet[j] = clamp16(*wet[j] + (samples[swapped[j]] * vol_wet >> 16));
                wet[j]++;
            }
        }
        vols[0] += rates[0];
        vols[1] += rates[1];
        vol_wet += rate_wet;

        n -= 8;
    } while (n > 0);
}

#else

void aEnvMixerImpl(uint16_t in_addr, uint16_t n_samples, bool swap_reverb, bool neg_left, bool neg_right,
                   uint16_t dry_left_addr, uint16_t dry_right_addr, uint16_t wet_left_addr, uint16_t wet_right_addr) {
    swap_reverb = false;
    int16_t* in = BUF_S16(in_addr);
    int16_t* dry[2] = { BUF_S16(dry_left_addr), BUF_S16(dry_right_addr) };
    int16_t* wet[2] = { BUF_S16(wet_left_addr), BUF_S16(wet_right_addr) };
    int16_t negs[2] = { neg_left ? -1 : 0, neg_right ? -1 : 0 };
    int n = ROUND_UP_16(n_samples);
    const int n_aligned = n - (n % 8);

    uint16_t vols[2] = { rspa.vol[0], rspa.vol[1] };
    uint16_t rates[2] = { rspa.rate[0], rspa.rate[1] };
    uint16_t vol_wet = rspa.vol_wet;
    uint16_t rate_wet = rspa.rate_wet;

    const __m128i* in_ptr = (__m128i*) in;
    const __m128i* d_ptr[2] = { (__m128i*) dry[0], (__m128i*) dry[1] };
    const __m128i* w_ptr[2] = { (__m128i*) wet[0], (__m128i*) wet[1] };

    // Aligned loop
    for (int N = 0; N < n_aligned; N += 8) {

        // Init vectors
        const __m128i in_channels = _mm_loadu_si128(in_ptr++);
        __m128i d[2] = { _mm_loadu_si128(d_ptr[0]), _mm_loadu_si128(d_ptr[1]) };
        __m128i w[2] = { _mm_loadu_si128(w_ptr[0]), _mm_loadu_si128(w_ptr[1]) };

        // Compute base samples
        // sample = ((in * vols) >> 16) ^ negs
        __m128i s[2] = { _mm_xor_si128(_mm_mulhi_epi16(in_channels, _mm_set1_epi16(vols[0])), _mm_set1_epi16(negs[0])),
                         _mm_xor_si128(_mm_mulhi_epi16(in_channels, _mm_set1_epi16(vols[1])),
                                       _mm_set1_epi16(negs[1])) };

        // Compute left swapped samples
        // (sample * vol_wet) >> 16) ^ negs
        __m128i ss[2] = {
            _mm_mulhi_epi16(s[swap_reverb], _mm_set1_epi16(vol_wet)),
            _mm_mulhi_epi16(s[!swap_reverb], _mm_set1_epi16(vol_wet)),
        };

        // Store values to buffers
        for (int j = 0; j < 2; j++) {
            _mm_storeu_si128((__m128i*) d_ptr[j]++, _mm_adds_epi16(s[j], d[j]));
            _mm_storeu_si128((__m128i*) w_ptr[j]++, _mm_adds_epi16(ss[j], w[j]));
            vols[j] += rates[j];
        }
        vol_wet += rate_wet;
    }
}

#endif

// https://godbolt.org/z/9a1qWvTee

#ifndef SSE2_AVAILABLE

void aMixImpl(int16_t gain, uint16_t in_addr, uint16_t out_addr, uint16_t count) {
    int nbytes = ROUND_UP_32(ROUND_DOWN_16(count));
    int16_t* in = BUF_S16(in_addr);
    int16_t* out = BUF_S16(out_addr);
    int i;
    int32_t sample;

    if (gain == -0x8000) {
        while (nbytes > 0) {
            for (i = 0; i < 16; i++) {
                sample = *out - *in++;
                *out++ = clamp16(sample);
            }
            nbytes -= 16 * sizeof(int16_t);
        }
    }

    while (nbytes > 0) {
        for (i = 0; i < 16; i++) {
            sample = ((*out * 0x7fff + *in++ * gain) + 0x4000) >> 15;
            *out++ = clamp16(sample);
        }

        nbytes -= 16 * sizeof(int16_t);
    }
}

#else

static const ALIGN_ASSET(16) int16_t x7fff[8] = {
    0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF,
};

void aMixImpl(int16_t gain, uint16_t in_addr, uint16_t out_addr, uint16_t count) {
    int nbytes = ROUND_UP_32(ROUND_DOWN_16(count));
    int16_t* in = BUF_S16(in_addr);
    int16_t* out = BUF_S16(out_addr);
    int i;
    int32_t sample;

    if (gain == -0x8000) {
        while (nbytes > 0) {
            for (unsigned int i = 0; i < 2; i++) {
                __m128i outVec = _mm_loadu_si128((__m128i*) out);
                __m128i inVec = _mm_loadu_si128((__m128i*) in);
                __m128i subsVec = _mm_subs_epi16(outVec, inVec);
                _mm_storeu_si128((__m128i*) out, subsVec);
                nbytes -= 8 * sizeof(int16_t);
                in += 8;
                out += 8;
            }
        }
    }

    __m128i x7fffVec = _mm_load_si128((__m128i*) x7fff);
    __m128i x4000Vec = _mm_load_si128((__m128i*) x4000);
    __m128i gainVec = _mm_set1_epi16(gain);

    while (nbytes > 0) {
        for (i = 0; i < 2; i++) {
            // Load input and output data into vectors
            __m128i outVec = _mm_loadu_si128((__m128i*) out);
            __m128i inVec = _mm_loadu_si128((__m128i*) in);
            // Multiply `out` by `0x7FFF` producing 32 bit results, and store the upper and lower bits in each vector.
            // Equivalent to `out[0..8] * 0x7FFF`
            m256i outx7fff = m256i_mul_epi16(outVec, x7fffVec);
            // Same as above but for in and gain. Equivalent to `in[0..8] * gain`
            m256i inxGain = m256i_mul_epi16(inVec, gainVec);
            in += 8;

            // Now we have 4 32 bit elements.  Continue the calculaton per the reference implementation.
            // We already did out + 0x7fff and in * gain.
            // *out * 0x7fff + *in++ * gain is the final result of these two calculations.
            m256i addVec = m256i_add_m256i_epi32(outx7fff, inxGain);
            // Add 0x4000 to each element
            addVec = m256i_add_m128i_epi32(addVec, x4000Vec);
            // Shift each element over by 15
            m256i shiftedVec = m256i_srai(addVec, 15);
            // Convert each 32 bit element to 16 bit with saturation (clamp) and store in `outVec`
            outVec = m256i_clamp_to_m128i(shiftedVec);
            // Write the final vector back to memory
            // The final calculation is ((out[0..8] * 0x7fff + in[0..8] * gain) + 0x4000) >> 15;
            _mm_storeu_si128((__m128i*) out, outVec);
            out += 8;
        }

        nbytes -= 16 * sizeof(int16_t);
    }
}

#endif

void aS8DecImpl(uint8_t flags, ADPCM_STATE state) {
    uint8_t* in = BUF_U8(rspa.in);
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_32(rspa.nbytes);
    if (flags & A_INIT) {
        memset(out, 0, 16 * sizeof(int16_t));
    } else if (flags & A_LOOP) {
        memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
    } else {
        memcpy(out, state, 16 * sizeof(int16_t));
    }
    out += 16;

    while (nbytes > 0) {
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);
        *out++ = (int16_t) (*in++ << 8);

        nbytes -= 16 * sizeof(int16_t);
    }

    memcpy(state, out - 16, 16 * sizeof(int16_t));
}

void aAddMixerImpl(uint16_t count, uint16_t in_addr, uint16_t out_addr) {
    int16_t* in = BUF_S16(in_addr);
    int16_t* out = BUF_S16(out_addr);
    int nbytes = ROUND_UP_64(ROUND_DOWN_16(count));

    do {
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;
        *out = clamp16(*out + *in++);
        out++;

        nbytes -= 16 * sizeof(int16_t);
    } while (nbytes > 0);
}

void aDuplicateImpl(uint16_t count, uint16_t in_addr, uint16_t out_addr) {
    uint8_t* in = BUF_U8(in_addr);
    uint8_t* out = BUF_U8(out_addr);

    uint8_t tmp[128];
    memcpy(tmp, in, 128);
    do {
        memcpy(out, tmp, 128);
        out += 128;
    } while (count-- > 0);
}

void aDMEMMove2Impl(uint8_t t, uint16_t in_addr, uint16_t out_addr, uint16_t count) {
    uint8_t* in = BUF_U8(in_addr);
    uint8_t* out = BUF_U8(out_addr);
    int nbytes = ROUND_UP_32(count);

    do {
        memmove(out, in, nbytes);
        in += nbytes;
        out += nbytes;
    } while (t-- > 0);
}

void aResampleZohImpl(uint16_t pitch, uint16_t start_fract) {
    int16_t* in = BUF_S16(rspa.in);
    int16_t* out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_8(rspa.nbytes);
    uint32_t pos = start_fract;
    uint32_t pitch_add = pitch << 2;

    do {
        *out++ = in[pos >> 17];
        pos += pitch_add;
        *out++ = in[pos >> 17];
        pos += pitch_add;
        *out++ = in[pos >> 17];
        pos += pitch_add;
        *out++ = in[pos >> 17];
        pos += pitch_add;

        nbytes -= 4 * sizeof(int16_t);
    } while (nbytes > 0);
}

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

void aInterlImpl(uint16_t in_addr, uint16_t out_addr, uint16_t n_samples) {
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

void aFilterImpl(uint8_t flags, uint16_t count_or_buf, int16_t* state_or_filter) {
    if (flags > A_INIT) {
        rspa.filter_count = ROUND_UP_16(count_or_buf);
        memcpy(rspa.filter, state_or_filter, sizeof(rspa.filter));
    } else {
        int16_t tmp[16], tmp2[8];
        int count = rspa.filter_count;
        int16_t* buf = BUF_S16(count_or_buf);

        if (flags == A_INIT) {
#ifndef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmemset-elt-size"
#endif
            memset(tmp, 0, 8 * sizeof(int16_t));
#ifndef __clang__
#pragma GCC diagnostic pop
#endif
            memset(tmp2, 0, 8 * sizeof(int16_t));
        } else {
            memcpy(tmp, state_or_filter, 8 * sizeof(int16_t));
            memcpy(tmp2, state_or_filter + 8, 8 * sizeof(int16_t));
        }

        for (int i = 0; i < 8; i++) {
            rspa.filter[i] = (tmp2[i] + rspa.filter[i]) / 2;
        }

        do {
            memcpy(tmp + 8, buf, 8 * sizeof(int16_t));
            for (int i = 0; i < 8; i++) {
                int64_t sample = 0x4000; // round term
                for (int j = 0; j < 8; j++) {
                    sample += tmp[i + j] * rspa.filter[7 - j];
                }
                buf[i] = clamp16((int32_t) (sample >> 15));
            }
            memcpy(tmp, tmp + 8, 8 * sizeof(int16_t));

            buf += 8;
            count -= 8 * sizeof(int16_t);
        } while (count > 0);

        memcpy(state_or_filter, tmp, 8 * sizeof(int16_t));
        memcpy(state_or_filter + 8, rspa.filter, 8 * sizeof(int16_t));
    }
}

void aHiLoGainImpl(uint8_t g, uint16_t count, uint16_t addr) {
    int16_t* samples = BUF_S16(addr);
    int nbytes = ROUND_UP_32(count);

    do {
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;
        *samples = clamp16((*samples * g) >> 4);
        samples++;

        nbytes -= 8;
    } while (nbytes > 0);
}

void aUnkCmd3Impl(uint16_t a, uint16_t b, uint16_t c) {
}

void aUnkCmd19Impl(uint8_t f, uint16_t count, uint16_t out_addr, uint16_t in_addr) {
    int nbytes = ROUND_UP_64(count);
    int16_t* in = BUF_S16(in_addr + f);
    int16_t* out = BUF_S16(out_addr);
    int16_t tbl[32];

    memcpy(tbl, in, 32 * sizeof(int16_t));
    do {
        for (int i = 0; i < 32; i++) {
            out[i] = clamp16(out[i] * tbl[i]);
        }
        out += 32;
        nbytes -= 32 * sizeof(int16_t);
    } while (nbytes > 0);
}