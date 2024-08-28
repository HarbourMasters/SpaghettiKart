#include "libultra_internal.h"
#include <libultraship.h>
#include "controller.h"

s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    int j;
    s32 ret;
    __OSDir dir;
    int files;
    files = 0;
    PFS_CHECK_STATUS;
    PFS_CHECK_ID;
    SET_ACTIVEBANK_TO_ZERO;
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*) &dir));
        if (dir.company_code != 0 && dir.game_code != 0) {
            files++;
        }
    }
    *files_used = files;
    *max_files = pfs->dir_size;
    return 0;
}
