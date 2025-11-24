#include <filesystem>
#include <fstream>
#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
#define fmt(...) fmt::format(__VA_ARGS__)
#else
#include <format>
#define fmt(...) std::format(__VA_ARGS__)
#endif

#include <libultraship.h>
#include <libultraship/libultra.h>
#include <save.h>
#include <cstdio>

#define MAX_FILES 16
#define EXT_NAME_SIZE 4
#define GAME_NAME_SIZE 16

typedef struct ControllerPak {
    FILE* header;
    FILE* file;
} ControllerPak;

std::string Pfs_PakFile_GetPath(u8 file_no) {
    return Ship::Context::GetPathRelativeToAppDirectory(fmt("controllerPak_file_{}.sav", file_no));
}

std::string Pfs_PakHeader_GetPath() {
    return Ship::Context::GetPathRelativeToAppDirectory("controllerPak_header.sav");
}

bool Pfs_PakHeader_Write(u32* file_size, u32* game_code, u16* company_code, u8* ext_name, u8* game_name, u8 fileIndex) {
    ControllerPak pak;

    std::string filename = Pfs_PakHeader_GetPath();
    pak.header = fopen(filename.c_str(), "w+b");

    if (!pak.header) {
        return false;
    }

    /* Set file parameters to header */
    u32 seek = fileIndex * sizeof(OSPfsState);

    // file_size
    fseek(pak.header, seek + 0x00, SEEK_SET);
    fwrite(file_size, 1, 4, pak.header);
    // game_code
    fseek(pak.header, seek + 0x04, SEEK_SET);
    fwrite(game_code, 1, 4, pak.header);
    // company_code
    fseek(pak.header, seek + 0x08, SEEK_SET);
    fwrite(company_code, 1, 2, pak.header);
    // ext_name
    fseek(pak.header, seek + 0x0C, SEEK_SET);
    fwrite(ext_name, 1, EXT_NAME_SIZE, pak.header);
    // game_name
    fseek(pak.header, seek + 0x10, SEEK_SET);
    fwrite(game_name, 1, GAME_NAME_SIZE, pak.header);
    fclose(pak.header);

    return true;
}

bool Pfs_PakHeader_Read(u32* file_size, u32* game_code, u16* company_code, char* ext_name, char* game_name,
                        u8 fileIndex) {
    ControllerPak pak;

    std::string filename = Pfs_PakHeader_GetPath();
    pak.header = fopen(filename.c_str(), "rb");

    if(!pak.header) {
        return false;
    }

    /* Set file parameters to header */
    u32 seek = fileIndex * sizeof(OSPfsState);

    // file_size
    fseek(pak.header, seek + 0x00, SEEK_SET);
    fread(file_size, 1, 4, pak.header);
    // game_code
    fseek(pak.header, seek + 0x04, SEEK_SET);
    fread(game_code, 1, 4, pak.header);
    // company_code
    fseek(pak.header, seek + 0x08, SEEK_SET);
    fread(company_code, 1, 2, pak.header);
    // ext_name
    fseek(pak.header, seek + 0x0C, SEEK_SET);
    fread(ext_name, 1, EXT_NAME_SIZE, pak.header);
    // game_name
    fseek(pak.header, seek + 0x10, SEEK_SET);
    fread(game_name, 1, GAME_NAME_SIZE, pak.header);
    fclose(pak.header);

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
    std::string header_file = Pfs_PakHeader_GetPath();

    pak.header = fopen(header_file.c_str(), "rb");

    // If a header file doesn't exist, create it.
    if(!pak.header) {
        pak.header = fopen(header_file.c_str(), "w+b");
        if (!pak.header) {
            return PFS_ERR_INVALID;
        }
        fclose(pak.header);
    }

    return PFS_NO_ERROR;
}

extern "C" s32 osPfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
    ControllerPak pak;
    std::string header_file = Pfs_PakHeader_GetPath();

    pak.header = fopen(header_file.c_str(), "rb");

    if (!pak.header) {
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

    fclose(pak.header);

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
    std::string filename = Pfs_PakFile_GetPath(freeFileIndex);
    pak.file = fopen(filename.c_str(), "w+b");

    if (!pak.file) {
        return PFS_ERR_INVALID;
    }

    file_size_in_bytes = (file_size_in_bytes + 31) & ~31;

    char* zero_block = (char*) malloc(file_size_in_bytes);
    for (size_t i = 0; i < file_size_in_bytes; i++) {
        zero_block[i] = 0;
    }

    fseek(pak.file, 0, SEEK_SET);
    fwrite(zero_block, 1, file_size_in_bytes, pak.file);
    free(zero_block);
    fclose(pak.file);

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

    std::string filename = Pfs_PakFile_GetPath(file_no);
    FILE* file = fopen(filename.c_str(), "rb");
    if (file) {
        fclose(file);
    } else {
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
    std::string filename = Pfs_PakFile_GetPath(file_no);

    pak.file = fopen(filename.c_str(), flag == 0 ? "r+b" : "w+b");

    if (!pak.file) {
        return PFS_ERR_INVALID;
    }

    if (flag == 0) {
        fseek(pak.file, offset, SEEK_SET);
        fread(data_buffer, 1, size_in_bytes, pak.file);
    } else {
        fseek(pak.file, offset, SEEK_SET);
        fwrite(data_buffer, 1, size_in_bytes, pak.file);
    }

    fclose(pak.file);

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
        char ext_name_[EXT_NAME_SIZE] = { 0 };
        char game_name_[GAME_NAME_SIZE] = { 0 };

        if (!Pfs_PakHeader_Read(&file_size_, &game_code_, &company_code_, ext_name_, game_name_, i)) {
            return PFS_ERR_INVALID;
        }

        if ((company_code_ == 0) || (game_code_ == 0)) {
            continue;
        } else {
            if ((game_code == game_code_) && (strcmp((const char*) game_name, (const char*) game_name_) == 0) &&
                strcmp((const char*) ext_name, (const char*) ext_name_) == 0) {
                // File found
                std::string header_file = Pfs_PakHeader_GetPath();
                pak.header = fopen(header_file.c_str(), "w+b");

                if(!pak.header) {
                    return PFS_ERR_INVALID;
                }

                u32 seek = i * sizeof(OSPfsState);

                // Zero out the header for this file.
                u8* zero_block = (u8*) malloc(sizeof(OSPfsState));
                for (size_t i = 0; i < sizeof(OSPfsState); i++) {
                    zero_block[i] = 0;
                }

                fseek(pak.header, seek + 0x0, SEEK_SET);
                fwrite(zero_block, 1, sizeof(OSPfsState), pak.header);

                free(zero_block);

                fclose(pak.header);

                std::string filename = Pfs_PakFile_GetPath(i);
                remove(filename.c_str());

                return PFS_NO_ERROR;
            }
        }
    }

    // File not found
    return PFS_ERR_INVALID;
}
