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
#include "math_util.h"
#include "courses/courseTable.h"
#include "courses/all_course_data.h"
#include "courses/all_course_packed.h"
#include "courses/all_course_model.h"
#include "courses/all_course_offsets.h"
#include "defines.h"

#include <assert.h>
#include <course_offsets.h>

#include <stdio.h>

s32 sGfxSeekPosition;
s32 sPackedSeekPosition;

static u8 sMemoryPool[0xFFFFF]; // Stock memory pool size: 0xAB630
uintptr_t sPoolEnd = sMemoryPool + sizeof(sMemoryPool);

uintptr_t sPoolFreeSpace;
struct MainPoolBlock* sPoolListHeadL;
struct MainPoolBlock* sPoolListHeadR;

struct MainPoolState* gMainPoolState = NULL;

struct UnkStruct_802B8CD4 D_802B8CD4[] = { 0 };
s32 D_802B8CE4 = 0; // pad
s32 memoryPadding[2];

#define PRINT_MEMPOOL                                                                                               \
    printf("\nPool Start: 0x%llX, Pool End: 0x%llX, size: 0x%llX\ngNextFreeMemoryAddress: 0x%llX\n\n", sMemoryPool, \
           sMemoryPool + sizeof(sMemoryPool), (sMemoryPool + sizeof(sMemoryPool)) - sMemoryPool,                    \
           gNextFreeMemoryAddress)

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
        assert(false);
    }

    return (void*) freeSpace;
}

/**
 * @brief Stores the physical memory addr for segmented memory in `gSegmentTable` using the segment number as an index.
 *
 * This function takes a segment number and a pointer to a memory address, and stores the address in the `gSegmentTable`
 * array at the specified segment index. The stored address is truncated to a 29-bit value to ensure that it fits within
 * the memory address. This allows converting between segmented memory and physical memory.
 *
 * @param segment A segment number from 0x0 to 0xF to set the base address.
 * @param addr A pointer containing the physical memory address of the data.
 * @return The stored base address, truncated to a 29-bit value.
 */
uintptr_t set_segment_base_addr(s32 segment, void* addr) {
    gSegmentTable[segment] = (uintptr_t) addr & 0x1FFFFFFF;
    return gSegmentTable[segment];
}

uintptr_t set_segment_base_addr_x64(s32 segment, void* addr) {
    gSegmentTable[segment] = (uintptr_t) addr;
    return gSegmentTable[segment];
}

/**
 * @brief Returns the physical memory location of a segment.
 * @param permits segment numbers from 0x0 to 0xF.
 */
void* get_segment_base_addr(s32 segment) {
    return (void*) (gSegmentTable[segment] | 0x80000000);
}

/**
 * @brief converts an RSP segment + offset address to a normal memory address
 */
void* segmented_to_virtual(const void* addr) {
    return addr;
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((gSegmentTable[segment] + offset));
}

void* segment_offset_to_virtual(uint32_t segment, uint32_t offset) {
    return (void*) (gSegmentTable[segment] + ((offset / 8) * sizeof(Gfx)));
}

void* segment_vtx_to_virtual(size_t offset) {
    // printf("seg_vtx_to_virt: 0x%llX to 0x%llX\n", offset, (gSegmentTable[0x04] + offset));

    return (void*) (gSegmentTable[0x04] + (offset));
}

void* segment5_to_virtual(size_t offset) {
    // printf("seg_texture_to_virt: 0x%llX to 0x%llX\n", offset, (gSegmentTable[0x05] + offset));
    return (void*) (gSegmentTable[0x05] + (offset));
}

void* segmented_texture_to_virtual(uintptr_t addr) {
    uint32_t segment = SEGMENT_NUMBER(addr);
    size_t offset = SEGMENT_OFFSET(addr);
    // printf("seg_texture_to_virt: 0x%llX to 0x%llX\n", offset, (gSegmentTable[segment] + offset));
    return (void*) (gSegmentTable[segment] + (offset));
}

void* segmented_uintptr_t_to_virtual(uintptr_t addr) {
    uint32_t newAddr = (uint32_t) addr;
    size_t segment = (uintptr_t) newAddr >> 24;
    size_t offset = (uintptr_t) newAddr & 0x00FFFFFF;

    uint32_t numCommands = offset / 8;
    offset = numCommands * sizeof(Gfx);

    // printf("seg_uintptr_t_to_virt: 0x%llX to 0x%llX\n", newAddr, (gSegmentTable[segment] + offset));

    return (void*) ((gSegmentTable[segment] + offset));
}

void* segmented_gfx_to_virtual(const void* addr) {
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    uint32_t numCommands = offset / 8;
    offset = numCommands * sizeof(Gfx);

    // printf("seg_gfx_to_virt: 0x%llX to 0x%llX\n", addr, (gSegmentTable[segment] + offset));

    return (void*) ((gSegmentTable[segment] + offset));
}

void move_segment_table_to_dmem(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        __gSPSegment(gDisplayListHead++, i, gSegmentTable[i]);
    }
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
    gFreeMemorySize -= size;

    if (gFreeMemorySize < 0) {
        printf("[memory.c] allocate_memory(): gFreeMemorySize below zero!\n");
        printf("gFreeMemorySize: 0x%X", gFreeMemorySize);
        PRINT_MEMPOOL;
        assert(false);
    }

    freeSpace = (uintptr_t) gNextFreeMemoryAddress;
    gNextFreeMemoryAddress += size;

    if (gNextFreeMemoryAddress > sPoolEnd) {
        printf("[memory.c] allocate_memory(): Memory Pool Out of Bounds! Out of memory!\n");
        PRINT_MEMPOOL;
        assert(false);
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

// replaces call to dynamic_dma_read with dma_read.
UNUSED void* load_segment(s32 segment, u8* srcStart, u8* srcEnd, u8* side) {
    void* addr = func_802A80B0(srcStart, srcEnd, side);

    if (addr != NULL) {
        set_segment_base_addr(segment, addr);
    }
    return addr;
}

// Similar to sm64 load_to_fixed_pool_addr?
UNUSED void* func_802A8190(s32 arg0, u8* arg1) {
    // uintptr_t srcSize = ALIGN16(srcEnd - srcStart);
    // uintptr_t destSize = ALIGN16((u8 *) sPoolListHeadR - destAddr);
    void* addr;
    uintptr_t temp_v0 = D_802B8CD4[arg0].unk4;
    uintptr_t temp_v1 = D_802B8CD4[arg0].unk8;
    uintptr_t temp_v2 = D_802B8CD4[arg0].unk2;
    addr = func_802A80B0((u8*) temp_v0, (u8*) temp_v1, arg1);

    // dest = main_pool_alloc(destSize, MEMORY_POOL_RIGHT);
    if (addr != 0) {
        set_segment_base_addr(temp_v2, addr);
    }
    return (void*) addr;
}

UNUSED void func_802A81EC(void) {
    s32 temp_s0;
    s16* phi_s1;
    s32 phi_s0;

    phi_s1 = (s16*) &D_802B8CD4;
    phi_s0 = 0;
    do {
        if ((*phi_s1 & 1) != 0) {
            func_802A8190(phi_s0, 0);
        }
        temp_s0 = phi_s0 + 1;
        phi_s1 += 8;
        phi_s0 = temp_s0;
    } while (phi_s0 != 3);
}

UNUSED struct AllocOnlyPool* alloc_only_pool_init(uintptr_t size, uintptr_t side) {
    void* addr;
    struct AllocOnlyPool* subPool = NULL;

    size = ALIGN4(size);
    addr = main_pool_alloc(size + sizeof(struct AllocOnlyPool), side);
    if (addr != NULL) {
        subPool = (struct AllocOnlyPool*) addr;
        subPool->totalSpace = size;
        subPool->usedSpace = (s32) addr + sizeof(struct AllocOnlyPool);
        subPool->startPtr = 0;
        subPool->freePtr = (u8*) addr + sizeof(struct AllocOnlyPool);
    }
    return subPool;
}

UNUSED uintptr_t func_802A82AC(s32 arg0) {
    uintptr_t temp_v0;
    uintptr_t phi_v1;

    temp_v0 = D_801502A0 - arg0;
    phi_v1 = 0;
    if (temp_v0 >= (uintptr_t) gDisplayListHead) {
        D_801502A0 = temp_v0;
        phi_v1 = temp_v0;
    }
    return phi_v1;
}

/**
 * @brief Returns pointer to mio0 compressed Vtx.
 */
u8* dma_compressed_vtx(u8* start, u8* end) {
    u8* freeSpace;
    uintptr_t size;

    size = ALIGN16(end - start);
    freeSpace = (u8*) gNextFreeMemoryAddress;
    dma_copy(freeSpace, start, size);
    gNextFreeMemoryAddress += size;
    return freeSpace;
}

// unused mio0 decode func.
UNUSED uintptr_t func_802A8348(s32 arg0, s32 arg1, s32 arg2) {
    uintptr_t offset;
    UNUSED void* pad;
    uintptr_t oldAddr;
    void* newAddr;

    offset = ALIGN16(arg1 * arg2);
    oldAddr = gNextFreeMemoryAddress;
    newAddr = (void*) (oldAddr + offset);
    pad = &newAddr;
#ifdef TARGET_N64
    osInvalDCache(newAddr, offset);
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], newAddr, offset,
                 &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
#endif

    func_80040030((u8*) newAddr, (u8*) oldAddr);
    gNextFreeMemoryAddress += offset;
    return oldAddr;
}

UNUSED u8* func_802A841C(u8* arg0, s32 arg1, s32 arg2) {
    u8* temp_v0;
    void* temp_a0;
    temp_v0 = (u8*) gNextFreeMemoryAddress;
    temp_a0 = temp_v0 + arg2;
    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);

    osInvalDCache(temp_a0, arg1);
#ifdef TARGET_N64
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], temp_a0, arg1,
                 &gDmaMesgQueue);
#endif
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    func_80040030((u8*) temp_a0, temp_v0);
    gNextFreeMemoryAddress += arg2;
    return temp_v0;
}

u8* dma_textures(const char* texture, size_t arg1, size_t arg2) {
    u8* temp_v0;
    void* temp_a0;
#ifdef TARGET_N64
    temp_v0 = (u8*) gNextFreeMemoryAddress;
#else
    u8* tex = (u8*) LOAD_ASSET(texture);

    temp_v0 = (u8*) allocate_memory(arg2);
#endif
    temp_a0 = temp_v0 + arg2;
    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);
#ifdef TARGET_N64
    osInvalDCache((void*) temp_a0, arg1);
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(texture)],
                 (void*) temp_a0, arg1, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, (int) 1);
    mio0decode((u8*) temp_a0, temp_v0);
    gNextFreeMemoryAddress += arg2;
#else
    memcpy(temp_v0, tex, arg2);
#endif
    return temp_v0;
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

void func_802A86A8(CourseVtx* data, Vtx* vtx, size_t arg1) {
    CourseVtx* courseVtx = data;
    s32 tmp = ALIGN16(arg1 * 0x10);
    size_t i;
    s8 temp_a0;
    s8 temp_a3;
    s8 flags;

#ifdef TARGET_N64
    gHeapEndPtr -= tmp;
    vtx = (Vtx*) gHeapEndPtr;
#endif

    // s32 to uintptr_t comparison required for matching.
    for (i = 0; i < arg1; i++) {
        if (gIsMirrorMode) {
            vtx->v.ob[0] = -courseVtx->ob[0];
        } else {
            vtx->v.ob[0] = courseVtx->ob[0];
        }

        vtx->v.ob[1] = (courseVtx->ob[1] * vtxStretchY);
        temp_a0 = courseVtx->ca[0];
        temp_a3 = courseVtx->ca[1];

        flags = temp_a0 & 3;
        flags |= (temp_a3 << 2) & 0xC;

        vtx->v.ob[2] = courseVtx->ob[2];
        vtx->v.tc[0] = courseVtx->tc[0];
        vtx->v.tc[1] = courseVtx->tc[1];
        vtx->v.cn[0] = (temp_a0 & 0xFC);
        vtx->v.cn[1] = (temp_a3 & 0xFC);
        vtx->v.cn[2] = courseVtx->ca[2];
        vtx->v.flag = flags;
        vtx->v.cn[3] = 0xFF;
        vtx++;
        courseVtx++;
    }
}

void decompress_vtx(CourseVtx* arg0, u32 vertexCount) {
    s32 size = ALIGN16(vertexCount * 0x18);
#ifdef TARGET_N64
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 offset = SEGMENT_OFFSET(arg0);
    void* freeSpace;
    u8* vtxCompressed = VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
#else
    void* freeSpace;
    u8* vtxCompressed = arg0;
#endif
    UNUSED s32 pad;

    freeSpace = (void*) gNextFreeMemoryAddress;
    gNextFreeMemoryAddress += size;

    mio0decode(vtxCompressed, (u8*) freeSpace);
#ifdef TARGET_N64
    func_802A86A8((CourseVtx*) freeSpace, vertexCount);
#endif
    set_segment_base_addr(4, (void*) gHeapEndPtr);
}

UNUSED void func_802A8844(void) {
}

void unpack_lights(Gfx* arg0, UNUSED u8* arg1, s8 arg2) {
    UNUSED s32 pad;
    s32 a = (arg2 * 0x18) + 0x9000008;
    s32 b = (arg2 * 0x18) + 0x9000000;
    Gfx macro[] = { gsSPNumLights(NUMLIGHTS_1) };

    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;

    sGfxSeekPosition++;
    arg0[sGfxSeekPosition].words.w0 = 0x3860010;

    arg0[sGfxSeekPosition].words.w1 = a;

    sGfxSeekPosition++;
    arg0[sGfxSeekPosition].words.w0 = 0x3880010;
    arg0[sGfxSeekPosition].words.w1 = b;
    sGfxSeekPosition++;
}

void unpack_displaylist(Gfx* arg0, u8* args, UNUSED s8 opcode) {
    uintptr_t temp_v0 = args[sPackedSeekPosition++];
    uintptr_t temp_t7 = ((args[sPackedSeekPosition++]) << 8 | temp_v0) * 8;
    arg0[sGfxSeekPosition].words.w0 = 0x06000000;
    // Segment seven addr
    arg0[sGfxSeekPosition].words.w1 = segment_offset_to_virtual(
        0x07, temp_t7); // (0x07000000 + temp_t7); // (gSegmentTable[segment] + ( (offset / 8) * sizeof(Gfx) ) );
    sGfxSeekPosition++;
}

// end displaylist
void unpack_end_displaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    arg0[sGfxSeekPosition].words.w0 = (uintptr_t) (uint8_t) G_ENDDL << 24;
    arg0[sGfxSeekPosition].words.w1 = 0;
    sGfxSeekPosition++;
}

void unpack_set_geometry_mode(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPSetGeometryMode(G_CULL_BACK) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_clear_geometry_mode(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPClearGeometryMode(G_CULL_BACK) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_cull_displaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPCullDisplayList(0, 7) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_combine_mode1(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_combine_mode2(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_combine_mode_shade(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_combine_mode4(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_combine_mode5(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_render_mode_opaque(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_render_mode_tex_edge(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_render_mode_translucent(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_render_mode_opaque_decal(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_DECAL) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_render_mode_translucent_decal(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL) };
    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_tile_sync(Gfx* gfx, u8* args, s8 opcode) {
    Gfx tileSync[] = { gsDPTileSync() };
    uintptr_t temp_a0;
    uintptr_t lo;
    uintptr_t hi;

    s32 width;
    s32 height;
    s32 fmt;
    s32 siz;
    s32 line;
    s32 tmem;
    s32 cms;
    s32 masks;
    s32 cmt;
    s32 maskt;
    s32 lrs;
    s32 lrt;
    UNUSED s32 pad[4];

    tmem = 0;
    switch (opcode) {
        case 26:
            width = 32;
            height = 32;
            fmt = 0;
            break;
        case 44:
            width = 32;
            height = 32;
            fmt = 0;
            tmem = 256;
            break;
        case 27:
            width = 64;
            height = 32;
            fmt = 0;
            break;
        case 28:
            width = 32;
            height = 64;
            fmt = 0;
            break;
        case 29:
            width = 32;
            height = 32;
            fmt = 3;
            break;
        case 30:
            width = 64;
            height = 32;
            fmt = 3;
            break;
        case 31:
            width = 32;
            height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    siz = G_IM_SIZ_16b_BYTES;
    line = ((((width * 2) + 7) >> 3));

    temp_a0 = args[sPackedSeekPosition++];
    cms = temp_a0 & 0xF;
    masks = (temp_a0 & 0xF0) >> 4;

    temp_a0 = args[sPackedSeekPosition++];
    cmt = temp_a0 & 0xF;
    maskt = (temp_a0 & 0xF0) >> 4;

    // Generate gfx

    gfx[sGfxSeekPosition].words.w0 = tileSync->words.w0;
    gfx[sGfxSeekPosition].words.w1 = tileSync->words.w1;
    sGfxSeekPosition++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | (line << 9) | tmem;
    hi = ((cmt) << 18) | ((maskt) << 14) | ((cms) << 8) | ((masks) << 4);

    gfx[sGfxSeekPosition].words.w0 = lo;
    gfx[sGfxSeekPosition].words.w1 = hi;
    sGfxSeekPosition++;

    lrs = (width - 1) << 2;
    lrt = (height - 1) << 2;

    lo = ((uintptr_t) (uint8_t) G_SETTILESIZE << 24);
    hi = (lrs << 12) | lrt;

    gfx[sGfxSeekPosition].words.w0 = lo;
    gfx[sGfxSeekPosition].words.w1 = hi;
    sGfxSeekPosition++;
}

void unpack_tile_load_sync(Gfx* gfx, u8* args, s8 opcode) {
    UNUSED uintptr_t var;
    Gfx tileSync[] = { gsDPTileSync() };
    Gfx loadSync[] = { gsDPLoadSync() };

    uintptr_t arg;
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t addr;
    uintptr_t width;
    uintptr_t height;
    uintptr_t fmt;
    uintptr_t siz;
    uintptr_t tmem;
    uintptr_t tile;
    size_t offset;

    switch (opcode) {
        case 32:
            width = 32;
            height = 32;
            fmt = 0;
            break;
        case 33:
            width = 64;
            height = 32;
            fmt = 0;
            break;
        case 34:
            width = 32;
            height = 64;
            fmt = 0;
            break;
        case 35:
            width = 32;
            height = 32;
            fmt = 3;
            break;
        case 36:
            width = 64;
            height = 32;
            fmt = 3;
            break;
        case 37:
            width = 32;
            height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    // Waa?
    var = args[sPackedSeekPosition];
    // Generates a texture address.
    offset = args[sPackedSeekPosition++] << 11;
    addr = SEGMENT_ADDR(0x05, offset);
    sPackedSeekPosition++;
    arg = args[sPackedSeekPosition++];
    siz = G_IM_SIZ_16b;
    tmem = (arg & 0xF);
    tile = (arg & 0xF0) >> 4;

    // Generate gfx

    lo = ((uintptr_t) (uint8_t) G_SETTIMG << 24) | (fmt << 21) | (siz << 19);
    gfx[sGfxSeekPosition].words.w0 = lo;
    gfx[sGfxSeekPosition].words.w1 = segment5_to_virtual(offset);
    sGfxSeekPosition++;

    gfx[sGfxSeekPosition].words.w0 = tileSync->words.w0;
    gfx[sGfxSeekPosition].words.w1 = tileSync->words.w1;
    sGfxSeekPosition++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | tmem;
    hi = tile << 24;

    gfx[sGfxSeekPosition].words.w0 = lo;
    gfx[sGfxSeekPosition].words.w1 = hi;
    sGfxSeekPosition++;

    gfx[sGfxSeekPosition].words.w0 = loadSync->words.w0;
    gfx[sGfxSeekPosition].words.w1 = loadSync->words.w1;
    sGfxSeekPosition++;

    lo = (uintptr_t) (uint8_t) G_LOADBLOCK << 24;
    hi = (tile << 24) | (MIN((width * height) - 1, 0x7FF) << 12) | CALC_DXT(width, G_IM_SIZ_16b_BYTES);

    gfx[sGfxSeekPosition].words.w0 = lo;
    gfx[sGfxSeekPosition].words.w1 = hi;
    sGfxSeekPosition++;
}

void unpack_texture_on(Gfx* arg0, UNUSED u8* args, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON) };

    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_texture_off(Gfx* arg0, UNUSED u8* args, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPTexture(0x1, 0x1, 0, G_TX_RENDERTILE, G_OFF) };

    arg0[sGfxSeekPosition].words.w0 = macro->words.w0;
    arg0[sGfxSeekPosition].words.w1 = macro->words.w1;
    sGfxSeekPosition++;
}

void unpack_vtx1(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_t7;
    uintptr_t temp_t7_2;

    uintptr_t temp = args[sPackedSeekPosition++];
    uintptr_t temp2 = ((args[sPackedSeekPosition++] << 8) | temp) * 0x10;

    temp = args[sPackedSeekPosition++];
    temp_t7 = temp & 0x3F;
    temp = args[sPackedSeekPosition++];
    temp_t7_2 = temp & 0x3F;

    gfx[sGfxSeekPosition].words.w0 =
        ((uintptr_t) (uint8_t) G_VTX << 24) | (temp_t7_2 * 2 << 16) | (((temp_t7 << 10) + ((0x10 * temp_t7) - 1)));
    gfx[sGfxSeekPosition].words.w1 = (uintptr_t) segment_vtx_to_virtual(temp2);
    sGfxSeekPosition++;
}

void unpack_vtx2(Gfx* gfx, u8* args, s8 arg2) {
    uintptr_t temp_t9;
    uintptr_t temp_v1;
    uintptr_t temp_v2;

    temp_v1 = args[sPackedSeekPosition++];
    temp_v2 = ((args[sPackedSeekPosition++] << 8) | temp_v1) * 0x10;

    temp_t9 = arg2 - 50;

    gfx[sGfxSeekPosition].words.w0 = ((uintptr_t) (uint8_t) G_VTX << 24) | ((temp_t9 << 10) + (((temp_t9) * 0x10) - 1));
    gfx[sGfxSeekPosition].words.w1 = (uintptr_t) segment_vtx_to_virtual(temp_v2);
    sGfxSeekPosition++;
}

void unpack_triangle(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_a2;
    uintptr_t phi_a3;

    temp_v0 = args[sPackedSeekPosition++];

    if (gIsMirrorMode) {
        phi_a3 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a3 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[sGfxSeekPosition].words.w0 = ((uintptr_t) (uint8_t) G_TRI1 << 24);
    gfx[sGfxSeekPosition].words.w1 = ((phi_a0 * 2) << 16) | ((phi_a2 * 2) << 8) | (phi_a3 * 2);
    sGfxSeekPosition++;
}

void unpack_quadrangle(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a0;
    uintptr_t phi_t2;
    uintptr_t phi_t1;
    uintptr_t phi_a2;

    temp_v0 = args[sPackedSeekPosition++];

    if (gIsMirrorMode) {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_t0 = (temp_v0 >> 2) & 0x1F;
    }

    temp_v0 = args[sPackedSeekPosition++];

    if (gIsMirrorMode) {
        phi_a2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_t2 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_t2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[sPackedSeekPosition++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_a2 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[sGfxSeekPosition].words.w0 =
        ((uintptr_t) (uint8_t) G_TRI2 << 24) | ((phi_a0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_t0 * 2);
    gfx[sGfxSeekPosition].words.w1 = ((phi_t2 * 2) << 16) | ((phi_t1 * 2) << 8) | (phi_a2 * 2);
    sGfxSeekPosition++;
}

void unpack_spline_3D(Gfx* gfx, u8* arg1, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a2;

    temp_v0 = arg1[sPackedSeekPosition++];

    if (gIsMirrorMode != 0) {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[sPackedSeekPosition++];
        phi_a2 |= ((temp_v0 & 3) * 8);
        phi_a3 = (temp_v0 >> 2) & 0x1F;
        phi_t0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[sPackedSeekPosition++];
        phi_t0 |= (temp_v0 & 0xF) * 2;
    } else {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[sPackedSeekPosition++];
        phi_a3 |= ((temp_v0 & 3) * 8);
        phi_a2 = (temp_v0 >> 2) & 0x1F;
        phi_a0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[sPackedSeekPosition++];
        phi_a0 |= (temp_v0 & 0xF) * 2;
    }
    gfx[sGfxSeekPosition].words.w0 = ((uintptr_t) (uint8_t) G_QUAD << 24);
    gfx[sGfxSeekPosition].words.w1 = ((phi_a0 * 2) << 24) | ((phi_t0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_a2 * 2);
    sGfxSeekPosition++;
}

UNUSED void func_802A9AEC(void) {
}

/**
 * Unpacks course packed displaylists by iterating through each byte of the packed file.
 * Each packed displaylist entry has an opcode and any number of arguments.
 * The opcodes range from 0 to 87 which are used to run the relevant unpack function.
 * The file pointer increments when arguments are used. This way,
 * displaylist_unpack will always read an opcode and not an argument by accident.
 *
 * @warning opcodes that do not contain a definition in the switch are ignored. If an undefined opcode
 * contained arguments the unpacker might try to unpack those arguments.
 * This issue is prevented so long as the packed file adheres to correct opcodes and unpack code
 * increments the file pointer the correct number of times.
 */
void displaylist_unpack(uintptr_t* data, uintptr_t finalDisplaylistOffset, u32 arg2) {
#ifdef TARGET_N64
    uintptr_t segment = SEGMENT_NUMBER2(data);
    uintptr_t offset = SEGMENT_OFFSET(data);
    u8* packed_dl = VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
#else
    u8* packed_dl = finalDisplaylistOffset;
    Gfx* gfx = data;
#endif

#ifdef TARGET_N64
    Gfx* gfx;
    u32 addr;
#endif

    u8 opcode;
#ifdef TARGET_N64
    finalDisplaylistOffset = ALIGN16(finalDisplaylistOffset) + 8;
    gHeapEndPtr -= finalDisplaylistOffset;
    addr = gHeapEndPtr;
    gfx = (Gfx*) gHeapEndPtr;
#endif
    sGfxSeekPosition = 0;
    sPackedSeekPosition = 0;

    while (true) {

        // Seek to the next byte
        opcode = packed_dl[sPackedSeekPosition++];

        // Break when the eof has been reached denoted by opcode 0xFF
        if (opcode == 0xFF) {
            break;
        }

        switch (opcode) {
            case 0x0:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x1:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x2:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x3:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x4:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x5:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x6:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x7:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x8:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x9:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xA:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xB:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xC:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xD:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xE:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0xF:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x10:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x11:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x12:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x13:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x14:
                unpack_lights(gfx, packed_dl, opcode);
                break;
            case 0x15:
                unpack_combine_mode1(gfx, packed_dl, arg2);
                break;
            case 0x16:
                unpack_combine_mode2(gfx, packed_dl, arg2);
                break;
            case 0x17:
                unpack_combine_mode_shade(gfx, packed_dl, arg2);
                break;
            case 0x2E:
                unpack_combine_mode4(gfx, packed_dl, arg2);
                break;
            case 0x53:
                unpack_combine_mode5(gfx, packed_dl, arg2);
                break;
            case 0x18:
                unpack_render_mode_opaque(gfx, packed_dl, arg2);
                break;
            case 0x19:
                unpack_render_mode_tex_edge(gfx, packed_dl, arg2);
                break;
            case 0x2F:
                unpack_render_mode_translucent(gfx, packed_dl, arg2);
                break;
            case 0x54:
                unpack_render_mode_opaque_decal(gfx, packed_dl, arg2);
                break;
            case 0x55:
                unpack_render_mode_translucent_decal(gfx, packed_dl, arg2);
                break;
            case 0x1A:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x2C:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x1B:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x1C:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x1D:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x1E:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x1F:
                unpack_tile_sync(gfx, packed_dl, opcode);
                break;
            case 0x20:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x21:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x22:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x23:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x24:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x25:
                unpack_tile_load_sync(gfx, packed_dl, opcode);
                break;
            case 0x26:
                unpack_texture_on(gfx, packed_dl, opcode);
                break;
            case 0x27:
                unpack_texture_off(gfx, packed_dl, opcode);
                break;
            case 0x28:
                unpack_vtx1(gfx, packed_dl, opcode);
                break;
            case 0x33:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x34:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x35:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x36:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x37:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x38:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x39:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3A:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3B:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3C:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3D:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3E:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x3F:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x40:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x41:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x42:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x43:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x44:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x45:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x46:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x47:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x48:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x49:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4A:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4B:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4C:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4D:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4E:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x4F:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x50:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x51:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x52:
                unpack_vtx2(gfx, packed_dl, opcode);
                break;
            case 0x29:
                unpack_triangle(gfx, packed_dl, opcode);
                break;
            case 0x58:
                unpack_quadrangle(gfx, packed_dl, opcode);
                break;
            case 0x30:
                unpack_spline_3D(gfx, packed_dl, opcode);
                break;
            case 0x2D:
                unpack_cull_displaylist(gfx, packed_dl, opcode);
                break;
            case 0x2A:
                unpack_end_displaylist(gfx, packed_dl, opcode);
                break;
            case 0x56:
                unpack_set_geometry_mode(gfx, packed_dl, opcode);
                break;
            case 0x57:
                unpack_clear_geometry_mode(gfx, packed_dl, opcode);
                break;
            case 0x2B:
                unpack_displaylist(gfx, packed_dl, opcode);
                break;
            default:
                // Skip unknown values
                break;
        }
    }
#ifdef TARGET_N64
    set_segment_base_addr(0x7, (void*) addr);
#endif
}

struct UnkStr_802AA7C8 {
    u8* unk0;
    uintptr_t unk4;
    uintptr_t unk8;
    uintptr_t unkC;
};

void decompress_textures(u32* arg0) {
#ifdef TARGET_N64
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 offset = SEGMENT_OFFSET(arg0);
    struct UnkStr_802AA7C8* phi_s0 = (struct UnkStr_802AA7C8*) VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
#else
    struct UnkStr_802AA7C8* phi_s0 = (struct UnkStr_802AA7C8*) arg0;
#endif
    struct UnkStr_802AA7C8* temp_s0;
    uintptr_t temp_t2;
    u8* temp_a0;
    uintptr_t phi_v0;
    uintptr_t sp20;

    phi_v0 = 0;
    temp_s0 = phi_s0;
    while (true) {
        temp_a0 = phi_s0->unk0;
        if ((temp_a0) == 0) {
            break;
        }
        phi_v0 += phi_s0->unk8;
        phi_s0++;
    }
    phi_s0 = temp_s0;
    gHeapEndPtr -= phi_v0;
    sp20 = gHeapEndPtr;

    while (true) {
        temp_a0 = phi_s0->unk0;
        if ((temp_a0) == 0) {
            break;
        }
        MIO0_0F(temp_a0, phi_s0->unk4, phi_s0->unk8);
        phi_s0++;
    }
    gHeapEndPtr = sp20;
    temp_t2 = gHeapEndPtr;
    set_segment_base_addr(0x5, (void*) temp_t2);
}

void* decompress_segments(u8* start, u8* end) {
    return;
    UNUSED u32 pad;
    u32 sp28;
    u32 size = ALIGN16(end - start);
    u8* heapEnd;
    u32* freeSpace;

    heapEnd = (u8*) gHeapEndPtr - size;
    // sp20 = temp_a0;
    dma_copy(heapEnd, start, size);
    sp28 = *(u32*) (heapEnd + 4);
    sp28 = ALIGN16(sp28);
    freeSpace = (u32*) gNextFreeMemoryAddress;
    mio0decode(heapEnd, (u8*) freeSpace);
    gNextFreeMemoryAddress += sp28;
    return (void*) freeSpace;
}

extern const course_texture mario_raceway_textures[30];

/* To help verify if ptrs are pointing within segments see gfx_pc.cpp gfx_step() */
uintptr_t vtxSegEnd;
uintptr_t dlSegEnd;
uintptr_t texSegEnd;
size_t texSegSize;
Gfx* testaaa;

/**
 * @brief Loads & DMAs course data. Vtx, textures, displaylists, etc.
 * @param courseId
 */

typedef struct {
    char* data;
    char* vtx;
    size_t vtxSize;
    course_texture* textures;
    char* displaylists;
    size_t dlSize;
} NewCourseTable;

NewCourseTable gNewCourseTable[] = { { // mario_raceway
                                       .data = d_course_mario_raceway_dl_0,
                                       .vtx = d_course_mario_raceway_vertex,
                                       .vtxSize = 5757,
                                       .textures = mario_raceway_textures,
                                       .displaylists = d_course_mario_raceway_packed_dls,
                                       .dlSize = 3367 },
                                     { // choco_mountain
                                       .data = d_course_choco_mountain_dl_0,
                                       .vtx = d_course_choco_mountain_vertex,
                                       .vtxSize = 5560,
                                       .textures = choco_mountain_textures,
                                       .displaylists = d_course_choco_mountain_packed_dls,
                                       .dlSize = 2910 },
                                     { // bowser_castle
                                       .data = d_course_bowsers_castle_dl_0,
                                       .vtx = d_course_bowsers_castle_vertex,
                                       .vtxSize = 9527,
                                       .textures = bowsers_castle_textures,
                                       .displaylists = d_course_bowsers_castle_packed_dls,
                                       .dlSize = 4900 },
                                     { // banshee_boardwalk
                                       .data = d_course_banshee_boardwalk_dl_0,
                                       .vtx = d_course_banshee_boardwalk_vertex,
                                       .vtxSize = 4945,
                                       .textures = banshee_boardwalk_textures,
                                       .displaylists = d_course_banshee_boardwalk_packed_dls,
                                       .dlSize = 3689 },
                                     { // maze
                                       .data = d_course_yoshi_valley_dl_0,
                                       .vtx = d_course_yoshi_valley_vertex,
                                       .vtxSize = 3720,
                                       .textures = yoshi_valley_textures,
                                       .displaylists = d_course_yoshi_valley_packed_dls,
                                       .dlSize = 4140 },
                                     { // snow
                                       .data = d_course_frappe_snowland_dl_0,
                                       .vtx = d_course_frappe_snowland_vertex,
                                       .vtxSize = 5529,
                                       .textures = frappe_snowland_textures,
                                       .displaylists = d_course_frappe_snowland_packed_dls,
                                       .dlSize = 3274 },
                                     { // koopa_troopa_beach
                                       .data = d_course_koopa_troopa_beach_dl_0,
                                       .vtx = d_course_koopa_troopa_beach_vertex,
                                       .vtxSize = 9376,
                                       .textures = koopa_troopa_beach_textures,
                                       .displaylists = d_course_koopa_troopa_beach_packed_dls,
                                       .dlSize = 5720 },
                                     { // royal_raceway
                                       .data = d_course_royal_raceway_dl_0,
                                       .vtx = d_course_royal_raceway_vertex,
                                       .vtxSize = 8306,
                                       .textures = royal_raceway_textures,
                                       .displaylists = d_course_royal_raceway_packed_dls,
                                       .dlSize = 5670 },
                                     { // luigi_raceway
                                       .data = d_course_luigi_raceway_dl_0,
                                       .vtx = d_course_luigi_raceway_vertex,
                                       .vtxSize = 5936,
                                       .textures = luigi_raceway_textures,
                                       .displaylists = d_course_luigi_raceway_packed_dls,
                                       .dlSize = 6377 },
                                     { // moo_moo_farm
                                       .data = d_course_moo_moo_farm_dl_0,
                                       .vtx = d_course_moo_moo_farm_vertex,
                                       .vtxSize = 7972,
                                       .textures = moo_moo_farm_textures,
                                       .displaylists = d_course_moo_moo_farm_packed_dls,
                                       .dlSize = 3304 },
                                     { // highway
                                       .data = d_course_toads_turnpike_dl_0,
                                       .vtx = d_course_toads_turnpike_vertex,
                                       .vtxSize = 6359,
                                       .textures = toads_turnpike_textures,
                                       .displaylists = d_course_toads_turnpike_packed_dls,
                                       .dlSize = 3427 },
                                     { // kalimari_desert
                                       .data = d_course_kalimari_desert_dl_0,
                                       .vtx = d_course_kalimari_desert_vertex,
                                       .vtxSize = 6393,
                                       .textures = kalimari_desert_textures,
                                       .displaylists = d_course_kalimari_desert_packed_dls,
                                       .dlSize = 5328 },
                                     { // sherbet
                                       .data = d_course_sherbet_land_dl_0,
                                       .vtx = d_course_sherbet_land_vertex,
                                       .vtxSize = 2678,
                                       .textures = sherbet_land_textures,
                                       .displaylists = d_course_sherbet_land_packed_dls,
                                       .dlSize = 1803 },
                                     { // rainbow
                                       .data = d_course_rainbow_road_dl_0,
                                       .vtx = d_course_rainbow_road_vertex,
                                       .vtxSize = 3111,
                                       .textures = rainbow_road_textures,
                                       .displaylists = d_course_rainbow_road_packed_dls,
                                       .dlSize = 1057 },
                                     { // wario
                                       .data = d_course_wario_stadium_dl_0,
                                       .vtx = d_course_wario_stadium_vertex,
                                       .vtxSize = 6067,
                                       .textures = wario_stadium_textures,
                                       .displaylists = d_course_wario_stadium_packed_dls,
                                       .dlSize = 5272 },
                                     { // block fort
                                       .data = d_course_block_fort_dl,
                                       .vtx = d_course_block_fort_vertex,
                                       .vtxSize = 1088,
                                       .textures = block_fort_textures,
                                       .displaylists = d_course_block_fort_packed_dls,
                                       .dlSize = 699 },
                                     { // skyscraper
                                       .data = d_course_skyscraper_dl,
                                       .vtx = d_course_skyscraper_vertex,
                                       .vtxSize = 1086,
                                       .textures = skyscraper_textures,
                                       .displaylists = d_course_skyscraper_packed_dls,
                                       .dlSize = 548 },
                                     { // double decker
                                       .data = d_course_double_deck_dl,
                                       .vtx = d_course_double_deck_vertex,
                                       .vtxSize = 555,
                                       .textures = double_deck_textures,
                                       .displaylists = d_course_double_deck_packed_dls,
                                       .dlSize = 234 },
                                     { // dk jungle
                                       .data = d_course_dks_jungle_parkway_dl_0,
                                       .vtx = d_course_dks_jungle_parkway_vertex,
                                       .vtxSize = 5679,
                                       .textures = dks_jungle_parkway_textures,
                                       .displaylists = d_course_dks_jungle_parkway_packed_dls,
                                       .dlSize = 4997 },
                                     { // big donut
                                       .data = d_course_big_donut_dl,
                                       .vtx = d_course_big_donut_vertex,
                                       .vtxSize = 1165,
                                       .textures = big_donut_textures,
                                       .displaylists = d_course_big_donut_packed_dls,
                                       .dlSize = 528 } };

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
        u8* tex = (u8*) LOAD_ASSET(textureList[i]);
        size_t texSize = ResourceGetTexSizeByName(textureList[i]);
        // printf("\nTEX SIZE: %X\n\n", texSize);
        memcpy(&textures[offset], tex, texSize);
        offset += texSize;
    }
    return textures;
}

void load_course(s32 courseId) {
    printf("Loading Course Data\n");

    char* data = gNewCourseTable[courseId].data;
    char* vtxData = gNewCourseTable[courseId].vtx;
    size_t vtxSize = gNewCourseTable[courseId].vtxSize;
    course_texture* textures = gNewCourseTable[courseId].textures;
    char* displaylists = gNewCourseTable[courseId].displaylists;
    size_t dlSize = gNewCourseTable[courseId].dlSize;

    // Convert course vtx to vtx
    CourseVtx* cvtx = (CourseVtx*) LOAD_ASSET(vtxData);
    Vtx* vtx = (Vtx*) allocate_memory(sizeof(Vtx) * vtxSize);
    gSegmentTable[4] = &vtx[0];
    func_802A86A8(cvtx, vtx, vtxSize);
    vtxSegEnd = &vtx[vtxSize];

    // Load and allocate memory for course textures
    course_texture* asset = textures;
    u8* freeMemory = NULL;
    u8* texture = NULL;
    size_t size = 0;
    texSegSize = 0;
    while (asset->addr) {
        size = ResourceGetTexSizeByName(asset->addr);
        freeMemory = (u8*) allocate_memory(size);

        texture = (u8*) LOAD_ASSET(asset->addr);
        if (texture) {
            if (asset == &textures[0]) {
                gSegmentTable[5] = &freeMemory[0];
            }
            memcpy(freeMemory, texture, size);
            texSegSize += size;
            // printf("Texture Addr: 0x%llX, size 0x%X\n", &freeMemory[0], size);
        }
        asset++;
    }

    texSegEnd = &((u8*) gSegmentTable[5])[texSegSize];

    // Extract packed DLs
    u8* packed = (u8*) LOAD_ASSET(displaylists);
    Gfx* gfx = (Gfx*) allocate_memory(sizeof(Gfx) * dlSize); // Size of unpacked DLs
    assert(gfx != NULL);
    gSegmentTable[7] = &gfx[0];
    displaylist_unpack(gfx, packed, 0);
    dlSegEnd = &gfx[dlSize];
}
