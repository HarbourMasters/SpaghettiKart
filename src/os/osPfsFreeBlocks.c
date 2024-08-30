#include "libultra_internal.h"
#include "controller.h"

s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
    int j;
    int pages;
    __OSInode inode;
    s32 ret;
    u8 bank;
    int offset;
    pages = 0;
    ret = 0;
    PFS_CHECK_STATUS;
    PFS_CHECK_ID;
    for (bank = 0; bank < pfs->banks; bank++) {
        ERRCK(__osPfsRWInode(pfs, &inode, OS_READ, bank));
        if (bank > 0) {
            offset = 1;
        } else {
            offset = pfs->inode_start_page;
        }
        for (j = offset; j < ARRLEN(inode.inode_page); j++) {
            if (inode.inode_page[j].ipage == 3) {
                pages++;
            }
        }
    }
    *bytes_not_used = pages * PFS_ONE_PAGE * BLOCKSIZE;
    return 0;
}
