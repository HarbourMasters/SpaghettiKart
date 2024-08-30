#ifndef SEGMENTS_H
#define SEGMENTS_H

#include <libultraship.h>
#include <macros.h>

extern u8 _memoryPoolSegmentNoloadStart[];
extern u8 _memoryPoolSegmentNoloadEnd[];

extern u8 _endingSegmentStart[];
extern u8 _endingSegmentRomStart[];
extern u8 _endingSegmentRomEnd[];
extern u8 _endingSegmentNoloadEnd[];

extern u8 _racingSegmentStart[];
extern u8 _racingSegmentNoloadEnd[];
extern u8 _racingSegmentRomStart[];
extern u8 _racingSegmentRomEnd[];

extern u8 _trigTablesSegmentStart[];
extern u8 _trigTablesSegmentSize[];
extern u8 _trigTablesSegmentRomStart[];

extern u8 _data_segment2SegmentRomStart[];
extern u8 _data_segment2SegmentRomEnd[];

extern u8 _common_texturesSegmentRomStart[];
extern u8 _common_texturesSegmentRomEnd[];

extern u8 _ceremonyDataSegmentRomStart[];
extern u8 _ceremonyDataSegmentRomEnd[];

extern u8 _startupLogoSegmentRomStart[];
extern u8 _startupLogoSegmentRomEnd[];

/**
 * mk64 has three code segments:
 *     main   0x80000400
 *     ending 0x80280000    size 0xDF00
 *     race   0x8028DF00    size 0x2C470
 **/

#define SEG_START 0x80000000

// Defines duplicated even if the same for both for easier readability
#ifdef AVOID_UB
#if 0
#define MEMORY_POOL_START (uintptr_t) & _memoryPoolSegmentNoloadStart[0]
#define MEMORY_POOL_END (uintptr_t) & _memoryPoolSegmentNoloadEnd[0]

#define SEG_ENDING (uintptr_t) & _endingSegmentStart[0]
#define SEG_ENDING_ROM_START (uintptr_t) & _endingSegmentRomStart[0]
/**
 * Ending segment original size is 0xDF00 but much of that remains unused.
 * This auto fits the segment to its proper size.
 */
#define SEG_ENDING_SIZE (size_t) ALIGN16((ptrdiff_t) (&_endingSegmentNoloadEnd[0] - &_endingSegmentStart[0]))
#define SEG_ENDING_ROM_SIZE (size_t) ALIGN16((ptrdiff_t) (&_endingSegmentRomEnd[0] - &_endingSegmentRomStart[0]))

#define SEG_RACING (uintptr_t) & _racingSegmentStart[0]
#define SEG_RACING_ROM_START (uintptr_t) & _racingSegmentRomStart[0]
#define SEG_RACING_SIZE (size_t) ALIGN16((ptrdiff_t) (&_racingSegmentNoloadEnd[0] - &_racingSegmentStart[0]))
#define SEG_RACING_ROM_SIZE (size_t) ALIGN16((ptrdiff_t) (&_racingSegmentRomEnd[0] - &_racingSegmentRomStart[0]))

#define TRIG_TABLES (uintptr_t) & _trigTablesSegmentStart[0]
#define TRIG_TABLES_ROM_START (uintptr_t) & _trigTablesSegmentRomStart[0]
#define TRIG_TABLES_SIZE (size_t) _trigTablesSegmentSize

#define SEG_DATA_START (uintptr_t) & _data_segment2SegmentRomStart[0]
#define SEG_DATA_END (uintptr_t) & _data_segment2SegmentRomEnd[0]

#define COMMON_TEXTURES_ROM_START (uintptr_t) & _common_texturesSegmentRomStart[0]
#define COMMON_TEXTURES_SIZE (ptrdiff_t)(&_common_texturesSegmentRomEnd[0] - &_common_texturesSegmentRomStart[0])

#define CEREMONY_DATA_ROM_START &_ceremonyDataSegmentRomStart[0]
#define CEREMONY_DATA_ROM_END &_ceremonyDataSegmentRomEnd[0]

#define STARTUP_LOGO_ROM_START &_startupLogoSegmentRomStart[0]
#define STARTUP_LOGO_ROM_END &_startupLogoSegmentRomEnd[0]
#endif
#else // Required for ok

#define MEMORY_POOL_START (uintptr_t) & _memoryPoolSegmentNoloadStart
#define MEMORY_POOL_END (uintptr_t) 0x80242F00

#define SEG_ENDING (uintptr_t) 0x80280000
#define SEG_ENDING_ROM_START (u8*) &_endingSegmentRomStart[0]

#define SEG_ENDING_SIZE (size_t) 0xDF00
#define SEG_ENDING_ROM_SIZE (size_t) ALIGN16((ptrdiff_t) (&_endingSegmentRomEnd[0] - &_endingSegmentRomStart[0]))

#define SEG_RACING (uintptr_t) 0x8028DF00
#define SEG_RACING_ROM_START (u8*) &_racingSegmentRomStart[0]
#define SEG_RACING_SIZE (size_t) 0x2C470

#define SEG_RACING_ROM_SIZE (size_t) ALIGN16((ptrdiff_t) (&_racingSegmentRomEnd[0] - &_racingSegmentRomStart[0]))

#define TRIG_TABLES (uintptr_t) 0x802BA370
#define TRIG_TABLES_ROM_START (uintptr_t) & _trigTablesSegmentRomStart[0]
#define TRIG_TABLES_SIZE (size_t) 0x5810

#define SEG_DATA_START (uintptr_t) & _data_segment2SegmentRomStart[0]
#define SEG_DATA_END (uintptr_t) & _data_segment2SegmentRomEnd[0]

#define COMMON_TEXTURES_ROM_START (uintptr_t) & _common_texturesSegmentRomStart[0]
#define COMMON_TEXTURES_SIZE \
    (ptrdiff_t)((uintptr_t) & _common_texturesSegmentRomEnd - (uintptr_t) & _common_texturesSegmentRomStart)

#define CEREMONY_DATA_ROM_START &_ceremonyDataSegmentRomStart[0]
#define CEREMONY_DATA_ROM_END &_ceremonyDataSegmentRomEnd[0]

#define STARTUP_LOGO_ROM_START &_startupLogoSegmentRomStart[0]
#define STARTUP_LOGO_ROM_END &_startupLogoSegmentRomEnd[0]

#endif /* AVOID_UB */

#endif /* SEGMENTS_H */
