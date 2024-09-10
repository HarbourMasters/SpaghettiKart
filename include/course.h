#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>
#include <libultra/gbi.h>
#include <macros.h>

/**
 * @file Include for course gfx.inc.c.
 */

typedef struct {
    uintptr_t addr;
    u8 surfaceType;
    u8 sectionId;
    u16 flags;
} TrackSections;

#endif // COURSE_H
