#ifndef MEMORY_H
#define MEMORY_H

#include "common_structs.h"
#include "course_offsets.h"

struct MainPoolBlock {
    struct MainPoolBlock* prev;
    struct MainPoolBlock* next;
};

struct MainPoolState {
    uintptr_t freeSpace;
    struct MainPoolBlock* listHeadL;
    struct MainPoolBlock* listHeadR;
    struct MainPoolState* prev;
};

struct UnkStruct802AF7B4 {
    s32 unk0;
    s32 unk4;
};

struct UnkStruct_802B8CD4 {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s32 fill;
};

struct AllocOnlyPool {
    s32 totalSpace;
    s32 usedSpace;
    u8* startPtr;
    u8* freePtr;
};

#define MEMORY_POOL_LEFT 0
#define MEMORY_POOL_RIGHT 1

#define ALIGN4(val) (((val) + 0x3) & ~0x3)

u8* load_lakitu_tlut_x64(const char** textureList, size_t length);
void* get_next_available_memory_addr(uintptr_t);
void initialize_memory_pool(void);
void* allocate_memory(size_t);
void* load_data(uintptr_t, uintptr_t);
void func_802A7D54(s32, s32);

void func_802A86A8(CourseVtx* data, Vtx* vtx, size_t arg1);

void main_pool_init(uintptr_t, uintptr_t);
void* main_pool_alloc(uintptr_t, uintptr_t);
uintptr_t main_pool_free(void*);
void* main_pool_realloc(void*, uintptr_t);
uintptr_t main_pool_available(void);
uintptr_t main_pool_push_state(void);
uintptr_t main_pool_pop_state(void);
void* func_802A80B0(u8*, u8*, u8*);
uintptr_t MIO0_0F(u8*, uintptr_t, uintptr_t);
void load_course(s32);

extern u8 _other_texturesSegmentRomStart[];

#endif // MEMORY_H
