#include <libultraship.h>
#include <libultra/types.h>
#include <align_asset_macro.h>
#include <macros.h>
#include <string.h>
#include <common_structs.h>
#include <segments.h>
#include <decode.h>
#include <stubs.h>

#include "memory.h"
#include "main.h"
#include "code_800029B0.h"
#include "defines.h"

#include "course_offsets.h"

#include "engine/courses/Course.h"

#include <stdio.h>

#include "port/Game.h"

s32 sGfxSeekPosition;
s32 sPackedSeekPosition;

static u8 sMemoryPool[0xFFFFFFF]; // Stock memory pool size: 0xAB630
uintptr_t sPoolEnd = sMemoryPool + sizeof(sMemoryPool);

uintptr_t sPoolFreeSpace;
struct MainPoolBlock* sPoolListHeadL;
struct MainPoolBlock* sPoolListHeadR;

struct MainPoolState* gMainPoolState = NULL;

struct UnkStruct_802B8CD4 D_802B8CD4[] = { 0 };
s32 D_802B8CE4 = 0; // pad
s32 memoryPadding[2];

#define PRINT_MEMPOOL                                                                                                       \
    printf("\nPool Start: %p, Pool End: %p, size: 0x%lX\ngNextFreeMemoryAddress: 0x%lX\n\n", (void*) sMemoryPool,   \
           (void*) (sMemoryPool + sizeof(sMemoryPool)),                                                                      \
           (unsigned long) (((sMemoryPool + sizeof(sMemoryPool)) - sMemoryPool)),                                           \
           (unsigned long) gNextFreeMemoryAddress)

/**
 * @brief Returns the address of the next available memory location and updates the memory pointer
 * to reference the next location of available memory based provided size to allocate.
 * @param size of memory to allocate.
 * @return Address of free memory
 */
void* get_next_available_memory_addr(uintptr_t size) {
    uintptr_t freeSpace = (uintptr_t) gNextFreeMemoryAddress;
    size = ALIGN16(size);
    gNextFreeMemoryAddress += size;

    if (gNextFreeMemoryAddress > sPoolEnd) {
        printf("[memory.c] get_next_available_memory_addr(): Memory Pool Out of Bounds! Out of memory!\n");
        PRINT_MEMPOOL;
    }

    return (void*) freeSpace;
}

static uintptr_t get_texture2(size_t offset, const course_texture* textures) {
    if (!((offset >= 0x5000000) && (offset < 0x6000000))) {
        return NULL;
    }
    size_t totalOffset = 0x5000000;

    while (textures->addr) {
        if (totalOffset == offset) {
            return (uintptr_t) (textures->addr);
        }
        totalOffset += textures->data_size;
        textures++;
    }

    printf("memory.c: get_texture()\n  TEXTURE NOT FOUND DURING DISPLAYLIST EXTRACT\n");
    printf("  offset: 0x%zX\n", offset);
    return NULL;
}

/**
 * @brief Sets the starting location for allocating memory and calculates pool size.
 *
 * Default memory size, 701.984 Kilobytes.
 */
void initialize_memory_pool() {

    uintptr_t poolStart = sMemoryPool;
    // uintptr_t sPoolEnd = sMemoryPool + sizeof(sMemoryPool);

    bzero(sMemoryPool, sizeof(sMemoryPool));

    poolStart = ALIGN16(poolStart);
    // Truncate to a 16-byte boundary.
    sPoolEnd &= ~0xF;

    gFreeMemorySize = (sPoolEnd - poolStart) - 0x10;
    gNextFreeMemoryAddress = poolStart;

    PRINT_MEMPOOL;
}

/**
 * @brief Allocates memory and adjusts gFreeMemorySize.
 */
void* allocate_memory(size_t size) {
    uintptr_t freeSpace;

    size = ALIGN16(size);
    if (gFreeMemorySize < size) {
        printf("[memory.c] allocate_memory(): gFreeMemorySize below zero!\n");
        printf("gFreeMemorySize before alloc: 0x%zX (requested: 0x%zX)\n", gFreeMemorySize, size);
        PRINT_MEMPOOL;
    }
    gFreeMemorySize -= size;

    freeSpace = (uintptr_t) gNextFreeMemoryAddress;
    gNextFreeMemoryAddress += size;

    if (gNextFreeMemoryAddress > sPoolEnd) {
        printf("[memory.c] allocate_memory(): Memory Pool Out of Bounds! Out of memory!\n");
        PRINT_MEMPOOL;
    }

    return (void*) freeSpace;
}

UNUSED void func_802A7D54(s32 arg0, s32 arg1) {
    gD_80150158[arg0].unk0 = arg0;
    gD_80150158[arg0].unk8 = arg1;
}

/**
 * @brief Allocate and DMA.
 */
void* load_data(uintptr_t startAddr, uintptr_t endAddr) {
    void* allocated;
    uintptr_t size = endAddr - startAddr;

    allocated = allocate_memory(size);
    if (allocated != 0) {
        dma_copy((u8*) allocated, (u8*) startAddr, size);
    }
    return (void*) allocated;
}

UNUSED void main_pool_init(uintptr_t start, uintptr_t end) {
    start = ALIGN16(start);
    end = ALIGN16(end - 15);

    sPoolFreeSpace = (end - start) - 16;

    sPoolListHeadL = (struct MainPoolBlock*) start;
    sPoolListHeadR = (struct MainPoolBlock*) end;
    sPoolListHeadL->prev = NULL;
    sPoolListHeadL->next = NULL;
    sPoolListHeadR->prev = NULL;
    sPoolListHeadR->next = NULL;
}

/**
 * Allocate a block of memory from the pool of given size, and from the
 * specified side of the pool (MEMORY_POOL_LEFT or MEMORY_POOL_RIGHT).
 * If there is not enough space, return NULL.
 */
UNUSED void* main_pool_alloc(uintptr_t size, uintptr_t side) {
    struct MainPoolBlock* newListHead;
    void* addr = NULL;

    size = ALIGN16(size) + 8;
    if (sPoolFreeSpace >= size) {
        sPoolFreeSpace -= size;
        if (side == MEMORY_POOL_LEFT) {
            newListHead = (struct MainPoolBlock*) ((u8*) sPoolListHeadL + size);
            sPoolListHeadL->next = newListHead;
            newListHead->prev = sPoolListHeadL;
            addr = (u8*) sPoolListHeadL + 8;
            sPoolListHeadL = newListHead;
        } else {
            newListHead = (struct MainPoolBlock*) ((u8*) sPoolListHeadR - size);
            sPoolListHeadR->prev = newListHead;
            newListHead->next = sPoolListHeadR;
            sPoolListHeadR = newListHead;
            addr = (u8*) sPoolListHeadR + 8;
        }
    }
    return addr;
}
/**
 * Free a block of memory that was allocated from the pool. The block must be
 * the most recently allocated block from its end of the pool, otherwise all
 * newer blocks are freed as well.
 * Return the amount of free space left in the pool.
 */
UNUSED uintptr_t main_pool_free(void* addr) {
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) addr - 8);
    struct MainPoolBlock* oldListHead = (struct MainPoolBlock*) ((u8*) addr - 8);

    if (oldListHead < sPoolListHeadL) {
        while (oldListHead->next != NULL) {
            oldListHead = oldListHead->next;
        }
        sPoolListHeadL = block;
        sPoolListHeadL->next = NULL;
        sPoolFreeSpace += (uintptr_t) oldListHead - (uintptr_t) sPoolListHeadL;
    } else {
        while (oldListHead->prev != NULL) {
            oldListHead = oldListHead->prev;
        }
        sPoolListHeadR = block->next;
        sPoolListHeadR->prev = NULL;
        sPoolFreeSpace += (uintptr_t) sPoolListHeadR - (uintptr_t) oldListHead;
    }
    return sPoolFreeSpace;
}
// main_pool_realloc
UNUSED void* main_pool_realloc(void* addr, uintptr_t size) {
    void* newAddr = NULL;
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) addr - 8);

    if (block->next == sPoolListHeadL) {
        main_pool_free(addr);
        newAddr = main_pool_alloc(size, MEMORY_POOL_LEFT);
    }
    return newAddr;
}

UNUSED uintptr_t main_pool_available(void) {
    return sPoolFreeSpace - 8;
}

UNUSED uintptr_t main_pool_push_state(void) {
    struct MainPoolState* prevState = gMainPoolState;
    uintptr_t freeSpace = sPoolFreeSpace;
    struct MainPoolBlock* lhead = sPoolListHeadL;
    struct MainPoolBlock* rhead = sPoolListHeadR;

    gMainPoolState = main_pool_alloc(sizeof(*gMainPoolState), MEMORY_POOL_LEFT);
    gMainPoolState->freeSpace = freeSpace;
    gMainPoolState->listHeadL = lhead;
    gMainPoolState->listHeadR = rhead;
    gMainPoolState->prev = prevState;
    return sPoolFreeSpace;
}

/**
 * Restore pool state from a previous call to main_pool_push_state. Return the
 * amount of free space left in the pool.
 */
UNUSED uintptr_t main_pool_pop_state(void) {
    sPoolFreeSpace = gMainPoolState->freeSpace;
    sPoolListHeadL = gMainPoolState->listHeadL;
    sPoolListHeadR = gMainPoolState->listHeadR;
    gMainPoolState = gMainPoolState->prev;
    return sPoolFreeSpace;
}
// similar to sm64 dma_read
UNUSED void* func_802A80B0(u8* dest, u8* srcStart, u8* srcEnd) {
    void* addr;
    uintptr_t size = srcStart - dest;
    addr = main_pool_alloc(size, (uintptr_t) srcEnd);

    if (addr != 0) {

        osInvalDCache(addr, size);
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) dest, addr, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    return addr;
}

uintptr_t MIO0_0F(u8* arg0, uintptr_t arg1, uintptr_t arg2) {
    uintptr_t oldHeapEndPtr;
    void* temp_v0;

    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);
    oldHeapEndPtr = gHeapEndPtr;
    temp_v0 = (void*) gNextFreeMemoryAddress;

    osInvalDCache(temp_v0, arg1);
#ifdef TARGET_N64
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], temp_v0, arg1,
                 &gDmaMesgQueue);
#endif
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    mio0decode((u8*) temp_v0, (u8*) oldHeapEndPtr);
    gHeapEndPtr += arg2;
    return oldHeapEndPtr;
}


/* To help verify if ptrs are pointing within segments see gfx_pc.cpp gfx_step() */
uintptr_t vtxSegEnd;
uintptr_t dlSegEnd;
uintptr_t texSegEnd;
size_t texSegSize;
Gfx* testaaa;

u8* load_lakitu_tlut_x64(const char** textureList, size_t length) {
    // Calculate lakitu texture size to allocate
    size_t size = 0;
    for (size_t i = 0; i < length; i++) {
        size += ResourceGetTexSizeByName(textureList[i]);
    }

    u8* textures = (u8*) gNextFreeMemoryAddress;
    gNextFreeMemoryAddress += size;
    size_t offset = 0;
    for (size_t i = 0; i < length; i++) {
        u8* tex = (u8*) LOAD_ASSET_RAW(textureList[i]);
        size_t texSize = ResourceGetTexSizeByName(textureList[i]);
        // printf("\nTEX SIZE: %X\n\n", texSize);
        memcpy(&textures[offset], tex, texSize);
        offset += texSize;
    }
    return textures;
}

/**
 * @brief Loads & DMAs course data. Vtx, textures, displaylists, etc.
 * @param courseId
 */
void load_course(s32 courseId) {
    printf("Loading Course %d\n", courseId);
    gNextFreeMemoryAddress = gFreeMemoryResetAnchor;
    CM_CleanWorld();
    LoadCourse();
    CM_Editor_SetLevelDimensions(gCourseMinX, gCourseMaxX, gCourseMinZ, gCourseMaxZ, gCourseMinY, gCourseMaxY);
}
