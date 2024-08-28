#include "libultra_internal.h"
#include "controller.h"

static s32 __osClearPage(OSPfs* pfs, int page_no, u8* data, u8 bank);
s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, int file_size_in_bytes,
                      s32* file_no) {
    int start_page;
    int decleared;
    int last_page;
    int old_last_page;
    int j;
    s32 ret;
    int file_size_in_pages;
    __OSInode inode;
    __OSInode backup_inode;
    __OSDir dir;
    u8 bank;
    u8 old_bank;
    int firsttime;
    s32 bytes;
    __OSInodeUnit fpage;

    old_last_page = 0;
    ret = 0;
    old_bank = 0;
    firsttime = 0;

    if (company_code == 0 || game_code == 0) {
        return PFS_ERR_INVALID;
    }

    file_size_in_pages = (file_size_in_bytes + 255) / (BLOCKSIZE * PFS_ONE_PAGE);
    if ((pfs->status & PFS_INITIALIZED) == false) {
        return PFS_ERR_INVALID;
    }

    PFS_CHECK_ID;

    ret = osPfsFindFile(pfs, company_code, game_code, game_name, ext_name, file_no);
    if (ret != 0 && ret != PFS_ERR_INVALID) {
        return ret;
    }

    if (*file_no != -1) {
        return PFS_ERR_EXIST;
    }

    ret = osPfsFreeBlocks(pfs, &bytes);
    if (file_size_in_bytes > bytes) {
        return PFS_DATA_FULL;
    }

    if (file_size_in_pages != 0) {
        ret = osPfsFindFile(pfs, 0, 0, NULL, NULL, file_no);
        if (ret != 0 && ret != PFS_ERR_INVALID) {
            return ret;
        }
        if (*file_no == -1) {
            return PFS_DIR_FULL;
        }

        for (bank = 0; bank < pfs->banks; bank++) {
            ERRCK(__osPfsRWInode(pfs, &inode, OS_READ, bank));
            ERRCK(__osPfsDeclearPage(pfs, &inode, file_size_in_pages, &start_page, bank, &decleared, &last_page));
            if (start_page != -1) {
                if (firsttime == 0) {
                    fpage.inode_t.page = start_page;
                    fpage.inode_t.bank = bank;
                } else {
                    backup_inode.inode_page[old_last_page].inode_t.bank = bank;
                    backup_inode.inode_page[old_last_page].inode_t.page = start_page;
                    ERRCK(__osPfsRWInode(pfs, &backup_inode, OS_WRITE, old_bank));
                }

                for (j = 0; j < ARRLEN(inode.inode_page); j++) {
                    backup_inode.inode_page[j].ipage = inode.inode_page[j].ipage;
                }
                old_last_page = last_page;
                old_bank = bank;
                firsttime++;
                if (file_size_in_pages > decleared) {
                    file_size_in_pages = file_size_in_pages - decleared;
                } else {
                    file_size_in_pages = 0;
                    break;
                }
            }
        }
        if (file_size_in_pages > 0 || start_page == -1) {
            return PFS_ERR_INCONSISTENT;
        }

        backup_inode.inode_page[old_last_page].inode_t.bank = bank;
        backup_inode.inode_page[old_last_page].inode_t.page = start_page;
        ERRCK(__osPfsRWInode(pfs, &backup_inode, OS_WRITE, old_bank));
        dir.start_page = fpage;
        dir.company_code = company_code;
        dir.game_code = game_code;
        dir.data_sum = 0;
        for (j = 0; j < ARRLEN(dir.game_name); j++) {
            dir.game_name[j] = *game_name++;
        }
        for (j = 0; j < ARRLEN(dir.ext_name); j++) {
            dir.ext_name[j] = *ext_name++;
        }
        ERRCK(__osContRamWrite(pfs->queue, pfs->channel, *file_no + pfs->dir_table, (u8*) &dir, false));
        return ret;
    }
    return PFS_ERR_INVALID;
}

s32 __osPfsDeclearPage(OSPfs* pfs, __OSInode* inode, int file_size_in_pages, int* first_page, u8 bank, int* decleared,
                       int* last_page) {
    int j;
    int spage;
    int old_page;
    u8 tmp_data[BLOCKSIZE];
    int i;
    s32 ret;
    int offset;

    ret = 0;
    if (bank > 0) {
        offset = 1;
    } else {
        offset = pfs->inode_start_page;
    }
    for (j = offset; j < ARRLEN(inode->inode_page); j++) {
        if (inode->inode_page[j].ipage == 3) {
            break;
        }
    }
    if (j == ARRLEN(inode->inode_page)) {
        *first_page = -1;
        return ret;
    }
    for (i = 0; i < ARRLEN(tmp_data); i++) {
        tmp_data[i] = 0;
    }
    spage = j;
    *decleared = 1;
    old_page = j++;
    while (file_size_in_pages > *decleared && j < ARRLEN(inode->inode_page)) {
        if (inode->inode_page[j].ipage == 3) {
            inode->inode_page[old_page].inode_t.bank = bank;
            inode->inode_page[old_page].inode_t.page = j;
            ERRCK(__osClearPage(pfs, old_page, (u8*) tmp_data, bank));
            old_page = j;
            (*decleared)++;
        }
        j++;
    }
    *first_page = spage;
    if (j == ARRLEN(inode->inode_page)) {
        if (file_size_in_pages > *decleared) {
            *last_page = old_page;
            return ret;
        }
    }
    inode->inode_page[old_page].ipage = 1;
    ret = __osClearPage(pfs, old_page, (u8*) tmp_data, bank);
    *last_page = 0;
    return ret;
}

static s32 __osClearPage(OSPfs* pfs, int page_no, u8* data, u8 bank) {
    int i;
    s32 ret;
    ret = 0;
    pfs->activebank = bank;
    ERRCK(__osPfsSelectBank(pfs));
    for (i = 0; i < PFS_ONE_PAGE; i++) {
        ret = __osContRamWrite(pfs->queue, pfs->channel, page_no * PFS_ONE_PAGE + i, data, false);
        if (ret != 0) {
            break;
        }
    }
    pfs->activebank = 0;
    ret = __osPfsSelectBank(pfs);
    return ret;
}
