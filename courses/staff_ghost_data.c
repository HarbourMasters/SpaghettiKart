#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>
#include "staff_ghost_data.h"

/*
 * This file is required for data alignment
 *
 */

StaffGhost d_mario_raceway_staff_ghost[] = {
#include "courses/mario_raceway/staff_ghost.inc.c"
};

StaffGhost d_luigi_raceway_staff_ghost[] = {
#include "courses/luigi_raceway/staff_ghost.inc.c"
};

StaffGhost d_royal_raceway_staff_ghost[] = {
#include "courses/royal_raceway/staff_ghost.inc.c"
};

// Unknown data. Perhaps colour or lights
s32 D_0F4FCAD0[] = { 0x00000000, 0xc8c8c8c8, 0xc8c80000, 0x00007b39, 0x6f6f6f6f,
                     0x6f6f6f6f, 0x3f7b0000, 0x0000c8c8, 0xc8c8c8c8, 0xc8000000 };
