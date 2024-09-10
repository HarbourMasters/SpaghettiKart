#ifndef _MACROS_H_
#define _MACROS_H_

#include <math.h>
#include <libultraship.h>

#ifndef __sgi
#define GLOBAL_ASM(...)
#endif

#if !defined(__sgi) && (!defined(NON_MATCHING) || !defined(AVOID_UB))
// asm-process isn't supported outside of IDO, and undefined behavior causes
// crashes.
// #error Matching build is only possible on IDO; please build with NON_MATCHING=1.
#endif

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

#define GLUE(a, b) a##b
#define GLUE2(a, b) GLUE(a, b)

// Avoid compiler warnings for unused variables
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

// Avoid undefined behaviour for non-returning functions
#ifdef __GNUC__
#define NORETURN __attribute__((noreturn))
#else
#define NORETURN
#endif

// Avoid undefined behaviour for non-returning functions
#ifdef __GNUC__
#define NO_REORDER __attribute__((no_reorder))
#else
#define NO_REORDER
#endif

// Static assertions
#ifdef __GNUC__
#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#define STATIC_ASSERT(cond, msg) typedef char GLUE2(static_assertion_failed, __LINE__)[(cond) ? 1 : -1]
#endif

// Align to 8-byte boundary for DMA requirements
#ifdef __GNUC__
#define ALIGNED8
#else
#define ALIGNED8
#endif

// Align to 16-byte boundary for audio lib requirements
#ifdef __GNUC__
#define ALIGNED16
#else
#define ALIGNED16
#endif

// Fixed point macros
#define FTOFIX(f) ((s32) ((f) * 65536.0))
#define ITOFIX(i) ((s32) ((i) << 16))
#define FIXTOF(x) ((double) ((x) / 65536.0))
#define FIXTOI(x) ((s32) ((x) >> 16))

// Split fixed-point values into its integer or fractional parts.
#define toFixedInt(f) (FTOFIX(f) >> 16)
#define toFrac(f) (FTOFIX(f) & 0xFFFF)

// Setup a fixed-point matrix using floats or doubles. Recommend using doubles for more precision.
#define toFixedPointMatrix(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16)                    \
    {                                                                                                                \
        { ((toFixedInt(x1)) << 16) | toFixedInt(x2), ((toFixedInt(x3)) << 16) | toFixedInt(x4),                      \
          (toFixedInt(x5) << 16) | toFixedInt(x6), (toFixedInt(x7) << 16) | toFixedInt(x8) },                        \
            { ((toFixedInt(x9)) << 16) | toFixedInt(x10), ((toFixedInt(x11)) << 16) | toFixedInt(x12),               \
              (toFixedInt(x13) << 16) | toFixedInt(x14), (toFixedInt(x15) << 16) | toFixedInt(x16) },                \
            { ((toFrac(x1)) << 16) | toFrac(x2), ((toFrac(x3)) << 16) | toFrac(x4), (toFrac(x5) << 16) | toFrac(x6), \
              (toFrac(x7) << 16) | toFrac(x8) },                                                                     \
        {                                                                                                            \
            ((toFrac(x9)) << 16) | toFrac(x10), ((toFrac(x11)) << 16) | toFrac(x12),                                 \
                (toFrac(x13) << 16) | toFrac(x14), (toFrac(x15) << 16) | toFrac(x16)                                 \
        }                                                                                                            \
    }

// convert a virtual address to physical.
#define VIRTUAL_TO_PHYSICAL(addr) (addr)

// convert a physical address to virtual.
#define PHYSICAL_TO_VIRTUAL(addr) (addr)

// another way of converting virtual to physical
#define VIRTUAL_TO_PHYSICAL2(addr) (addr)

// aligns an address to the next 16 bytes
#define ALIGN16(val) (((val) + 0xF) & ~0xF)

// Envelopes are always stored as big endian, to match sequence files which are
// byte blobs and can embed envelopes. Hence this byteswapping macro.
#if IS_BIG_ENDIAN
#define BSWAP16(x) (x)
#else
#define BSWAP16(x) (((x) & 0xff) << 8 | (((x) >> 8) & 0xff))
#endif

/**
 * (u8*) dl : Cast array down to u8's
 * + sizeof(dl) : go to the end of the array
 * - sizeof(dl[0]) : walk back 1 Gfx macro
 * - 0x07000000 : get just the offset
 **/
#define GET_PACKED_END(dl) (((u8*) dl) + sizeof(dl) - sizeof(dl[0]) - 0x07000000)

#endif
