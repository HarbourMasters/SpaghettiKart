#ifndef MEMORY_H
#define MEMORY_H

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

extern f32 vtxStretchY;

u8* load_lakitu_textures_x64(const char** textureList, size_t length);
u8* load_lakitu_tlut_x64(const char** textureList, size_t length);
void* get_next_available_memory_addr(uintptr_t);
uintptr_t set_segment_base_addr(s32, void*);
void* get_segment_base_addr(s32);
void* segmented_to_virtual(const void*);
void* segment_offset_to_virtual(uint32_t, uint32_t);
void* segment_vtx_to_virtual(size_t offset);
void* segmented_texture_to_virtual(uintptr_t addr);
void* segmented_uintptr_t_to_virtual(uintptr_t);
void* segmented_gfx_to_virtual(const void*);
void move_segment_table_to_dmem(void);
void initialize_memory_pool(void);
void* decompress_segments(u8*, u8*);
void* allocate_memory(size_t);
void* load_data(uintptr_t, uintptr_t);
void func_802A7D54(s32, s32);

void main_pool_init(uintptr_t, uintptr_t);
void* main_pool_alloc(uintptr_t, uintptr_t);
uintptr_t main_pool_free(void*);
void* main_pool_realloc(void*, uintptr_t);
uintptr_t main_pool_available(void);
uintptr_t main_pool_push_state(void);
uintptr_t main_pool_pop_state(void);
void* func_802A80B0(u8*, u8*, u8*);
void func_802A81EC(void);
struct AllocOnlyPool* alloc_only_pool_init(uintptr_t, uintptr_t);
uintptr_t func_802A82AC(s32);
uintptr_t func_802A8348(s32, s32, s32);
u8* dma_textures(const char*, size_t, size_t);
uintptr_t MIO0_0F(u8*, uintptr_t, uintptr_t);
void func_802A8844(void);
void unpack_lights(Gfx*, u8*, s8);
void unpack_displaylist(Gfx*, u8*, s8);
void unpack_end_displaylist(Gfx*, u8*, s8);
void unpack_set_geometry_mode(Gfx*, u8*, s8);
void unpack_clear_geometry_mode(Gfx*, u8*, s8);
void unpack_cull_displaylist(Gfx*, u8*, s8);
void unpack_combine_mode1(Gfx*, u8*, uintptr_t);
void unpack_combine_mode2(Gfx*, u8*, uintptr_t);
void unpack_combine_mode_shade(Gfx*, u8*, uintptr_t);
void unpack_combine_mode4(Gfx*, u8*, uintptr_t);
void unpack_combine_mode5(Gfx*, u8*, uintptr_t);
void unpack_render_mode_opaque(Gfx*, u8*, uintptr_t);
void unpack_render_mode_tex_edge(Gfx*, u8*, uintptr_t);
void unpack_render_mode_translucent(Gfx*, u8*, uintptr_t);
void unpack_render_mode_opaque_decal(Gfx*, u8*, uintptr_t);
void unpack_render_mode_translucent_decal(Gfx*, u8*, uintptr_t);
void unpack_tile_sync(Gfx*, u8*, s8);
void unpack_tile_load_sync(Gfx*, u8*, s8);
void unpack_texture_on(Gfx*, u8*, s8);
void unpack_texture_off(Gfx*, u8*, s8);
void load_course(s32);
uintptr_t set_segment_base_addr_x64(s32, void*);

extern u8 _other_texturesSegmentRomStart[];

#endif // MEMORY_H
