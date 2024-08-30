#include "libultra_internal.h"
#include "controller.h"

s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no) {
    s32 j;
    int i;
    __OSDir dir;
    s32 ret;
    int fail;
    ret = 0;
    PFS_CHECK_ID;
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*) &dir));
        if ((dir.company_code == company_code) && dir.game_code == game_code) {
            fail = false;
            if (game_name != NULL) {
                for (i = 0; i < ARRLEN(dir.game_name); i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = true;
                        break;
                    }
                }
            }
            if (ext_name != NULL && !fail) {

                for (i = 0; i < ARRLEN(dir.ext_name); i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = true;
                        break;
                    }
                }
            }
            if (!fail) {
                *file_no = j;
                return ret;
            }
        }
    }
    *file_no = -1;
    return PFS_ERR_INVALID;
}
