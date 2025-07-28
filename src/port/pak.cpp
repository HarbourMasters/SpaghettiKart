#include <filesystem>
#include <fstream>

#include <libultraship.h>
#include <libultraship/libultra.h>
#include <save.h>

#define MAX_FILES 16
#define EXT_NAME_SIZE 4
#define GAME_NAME_SIZE 16

typedef struct ControllerPak {
    std::fstream header;
    std::fstream file;
} ControllerPak;

bool Pfs_PakHeader_Write(u32* file_size, u32* game_code, u16* company_code, u8* ext_name, u8* game_name, u8 fileIndex) {
    ControllerPak pak;

    pak.header.open("controllerPak_header.sav", std::ios::binary | std::ios::in | std::ios::out);

    if (!pak.header.good()) {
        return false;
    }
    if (!pak.header.is_open()) {
        return false;
    }

    /* Set file parameters to header */
    u32 seek = fileIndex * sizeof(OSPfsState);

    // file_size
    pak.header.seekp(seek + 0x0, std::ios::beg);
    pak.header.write((char*) file_size, 4);
    // game_code
    pak.header.seekp(seek + 0x4, std::ios::beg);
    pak.header.write((char*) game_code, 4);
    // company_code
    pak.header.seekp(seek + 0x08, std::ios::beg);
    pak.header.write((char*) company_code, 2);
    // ext_name
    pak.header.seekp(seek + 0x0C, std::ios::beg);
    pak.header.write((char*) ext_name, EXT_NAME_SIZE);
    // game_name
    pak.header.seekp(seek + 0x10, std::ios::beg);
    pak.header.write((char*) game_name, GAME_NAME_SIZE);

    pak.header.close();
    return true;
}

bool Pfs_PakHeader_Read(u32* file_size, u32* game_code, u16* company_code, char* ext_name, char* game_name,
                        u8 fileIndex) {
    ControllerPak pak;

    pak.header.open("controllerPak_header.sav", std::ios::binary | std::ios::in | std::ios::out);

    if (!pak.header.good()) {
        return false;
    }
    if (!pak.header.is_open()) {
        return false;
    }

    /* Set file parameters to header */
    u32 seek = fileIndex * sizeof(OSPfsState);

    // file_size
    pak.header.seekg(seek + 0x0, std::ios::beg);
    pak.header.read((char*) file_size, 4);
    // game_code
    pak.header.seekg(seek + 0x4, std::ios::beg);
    pak.header.read((char*) game_code, 4);
    // company_code
    pak.header.seekg(seek + 0x08, std::ios::beg);
    pak.header.read((char*) company_code, 2);
    // ext_name
    pak.header.seekg(seek + 0x0C, std::ios::beg);
    pak.header.read((char*) ext_name, EXT_NAME_SIZE);
    // game_name
    pak.header.seekg(seek + 0x10, std::ios::beg);
    pak.header.read((char*) game_name, GAME_NAME_SIZE);

    pak.header.close();
    return true;
}

extern "C" s32 osPfsIsPlug(OSMesgQueue* queue, u8* pattern) {
    *pattern = 1;
    return PFS_NO_ERROR;
}

extern "C" s32 osPfsInit(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = PFS_INITIALIZED;

    ControllerPak pak;

    // If a header file doesn't exist, create it.
    if (!std::filesystem::exists("controllerPak_header.sav")) {
        pak.header.open("controllerPak_header.sav", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
        pak.header.close();
    }

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
    ControllerPak pak;

    pak.header.open("controllerPak_header.sav", std::ios::binary | std::ios::in | std::ios::out);

    if (!pak.header.good()) {
        return PFS_ERR_INVALID;
    }
    if (!pak.header.is_open()) {
        return PFS_ERR_INVALID;
    }

    s32 usedSpace = 0;
    for (size_t i = 0; i < MAX_FILES; i++) {
        u32 file_size = 0;
        u32 game_code = 0;
        u16 company_code = 0;
        char ext_name[EXT_NAME_SIZE] = { 0 };
        char game_name[GAME_NAME_SIZE] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size, &game_code, &company_code, ext_name, game_name, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code == 0) || (game_code == 0)) {
            continue;
        } else {
            usedSpace += file_size >> 8;
        }
    }

    pak.header.close();

    *bytes_not_used = (123 - usedSpace) << 8;

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsAllocateFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name,
                                 int file_size_in_bytes, s32* file_no) {

    if ((company_code == 0) || (game_code == 0)) {
        return PFS_ERR_INVALID;
    }

    ControllerPak pak;

    /* Search for a free slot */
    u8 freeFileIndex = 0;
    for (size_t i = 0; i < MAX_FILES; i++) {
        u32 file_size_ = 0;
        u32 game_code_ = 0;
        u16 company_code_ = 0;
        char ext_name_[EXT_NAME_SIZE] = { 0 };
        char game_name_[GAME_NAME_SIZE] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size_, &game_code_, &company_code_, ext_name_, game_name_, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code_ == 0) || (game_code_ == 0)) {
            freeFileIndex = i;
            break;
        }
    }

    if (freeFileIndex == MAX_FILES) {
        return PFS_DIR_FULL;
    }

    if (!Pfs_PakHeader_Write((u32*) &file_size_in_bytes, &game_code, &company_code, ext_name, game_name, freeFileIndex)) {
        return PFS_ERR_INVALID;
    }

    /* Create empty file */
    char filename[100];
    sprintf(filename, "controllerPak_file_%d.sav", freeFileIndex);
    pak.file.open(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

    file_size_in_bytes = (file_size_in_bytes + 31) & ~31;

    char* zero_block = (char*) malloc(file_size_in_bytes);
    for (size_t i = 0; i < file_size_in_bytes; i++) {
        zero_block[i] = 0;
    }

    pak.file.seekp(0, std::ios::beg);
    pak.file.write(zero_block, file_size_in_bytes);

    free(zero_block);

    pak.file.close();

    *file_no = freeFileIndex;

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state) {
    u32 file_size = 0;
    u32 game_code = 0;
    u16 company_code = 0;
    char ext_name[EXT_NAME_SIZE] = { 0 };
    char game_name[GAME_NAME_SIZE] = { 0 };

    // should pass the state of the requested file_no to the incoming state pointer,
    // games call this function 16 times, once per file
    // fills the incoming state with the information inside the header of the pak.

    char filename[100];
    sprintf(filename, "controllerPak_file_%d.sav", file_no);
    if (!std::filesystem::exists(filename)) {
        return PFS_ERR_INVALID;
    }

    /* Read game info from pak */
    if (!Pfs_PakHeader_Read(&file_size, &game_code, &company_code, ext_name, game_name, file_no)) {
        return PFS_ERR_INVALID;
    }

    state->file_size = file_size;
    state->company_code = game_code;
    state->game_code = game_code;

    for (size_t j = 0; j < GAME_NAME_SIZE; j++) {
        state->game_name[j] = game_name[j];
    }
    for (size_t j = 0; j < EXT_NAME_SIZE; j++) {
        state->ext_name[j] = ext_name[j];
    }

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsFindFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no) {
    ControllerPak pak;

    for (size_t i = 0; i < MAX_FILES; i++) {
        u32 file_size_ = 0;
        u32 game_code_ = 0;
        u16 company_code_ = 0;
        char ext_name_[EXT_NAME_SIZE] = { 0 };
        char game_name_[GAME_NAME_SIZE] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size_, &game_code_, &company_code_, ext_name_, game_name_, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code_ == 0) || (game_code_ == 0)) {
            continue;
        } else {
            if ((game_code == game_code_) && (company_code == company_code_) &&
                (strcmp((const char*) game_name, (const char*) game_name_) == 0) &&
                strcmp((const char*) ext_name, (const char*) ext_name_) == 0) {
                // File found
                *file_no = i;
                return PFS_NO_ERROR;
            }
        }
    }

    // File not found
    return PFS_ERR_INVALID;
}

extern "C" s32 osPfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8* data_buffer) {
    ControllerPak pak;

    char filename[100];
    sprintf(filename, "controllerPak_file_%d.sav", file_no);
    pak.file.open(filename, std::ios::binary | std::ios::in | std::ios::out);

    if (!std::filesystem::exists(filename)) {
        return PFS_ERR_INVALID;
    }
    if (!pak.file.good()) {
        return PFS_ERR_INVALID;
    }
    if (!pak.file.is_open()) {
        return PFS_ERR_INVALID;
    }

    if (flag == 0) {
        pak.file.seekg(offset, std::ios::beg);
        pak.file.read((char*) data_buffer, size_in_bytes);
    } else {
        pak.file.seekp(offset, std::ios::beg);
        pak.file.write((char*) data_buffer, size_in_bytes);
    }

    pak.file.close();

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    u8 files = 0;
    for (size_t i = 0; i < MAX_FILES; i++) {
        u32 file_size = 0;
        u32 game_code = 0;
        u16 company_code = 0;
        char ext_name[EXT_NAME_SIZE] = { 0 };
        char game_name[GAME_NAME_SIZE] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size, &game_code, &company_code, ext_name, game_name, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code != 0) || (game_code != 0)) {
            files++;
        }
    }

    *files_used = files;
    *max_files = MAX_FILES;

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsDeleteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name) {
    if (company_code == 0 || game_code == 0) {
        return PFS_ERR_INVALID;
    }

    ControllerPak pak;

    for (int i = 0; i < MAX_FILES; i++) {
        u32 file_size_ = 0;
        u32 game_code_ = 0;
        u16 company_code_ = 0;
        char ext_name_[4] = { 0 };
        char game_name_[16] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size_, &game_code_, &company_code_, ext_name_, game_name_, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code_ == 0) || (game_code_ == 0)) {
            continue;
        } else {
            if ((game_code == game_code_) && (strcmp((const char*) game_name, (const char*) game_name_) == 0) &&
                strcmp((const char*) ext_name, (const char*) ext_name_) == 0) {
                // File found

                pak.header.open("controllerPak_header.sav", std::ios::binary | std::ios::in | std::ios::out);

                if (!pak.header.good()) {
                    return PFS_ERR_INVALID;
                }
                if (!pak.header.is_open()) {
                    return PFS_ERR_INVALID;
                }

                u32 seek = i * sizeof(OSPfsState);

                // Zero out the header for this file.
                u8* zero_block = (u8*) malloc(sizeof(OSPfsState));
                for (size_t i = 0; i < sizeof(OSPfsState); i++) {
                    zero_block[i] = 0;
                }
                pak.header.seekp(seek + 0x0, std::ios::beg);
                pak.header.write((char*) zero_block, sizeof(OSPfsState));

                free(zero_block);

                pak.header.close();

                char filename[100];
                sprintf(filename, "controllerPak_file_%d.sav", i);
                remove(filename);

                return PFS_NO_ERROR;
            }
        }
    }

    // File not found
    return PFS_ERR_INVALID;
}
