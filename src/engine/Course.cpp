#include <libultraship.h>

#include "Course.h"

extern "C" {
    #include "memory.h"
    #include "common_structs.h"
    #include "course_offsets.h"
    extern uintptr_t gSegmentTable[];
}

Course::Course() {}

void Course::Load(Vtx *vtx, 
                  Gfx* gfx) {

    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
}

void Course::LoadTextures() { }
void Course::Init() {  }
void Course::BeginPlay() {  }
void Course::Render(Camera *camera) { }
void Course::Collision() {}
void Course::Expire() { }
void Course::Destroy() { }


StockCourse::StockCourse() {}

void StockCourse::Load(const char* courseVtx, 
                  course_texture* textures, const char* displaylists, size_t dlSize) {

    size_t vtxSize = (ResourceGetSizeByName(courseVtx) / sizeof(CourseVtx)) * sizeof(Vtx);

    // Convert course vtx to vtx
    Vtx* vtx = (Vtx*) allocate_memory(vtxSize);
    gSegmentTable[4] = &vtx[0];
    func_802A86A8(LOAD_ASSET(courseVtx), vtx, vtxSize);
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
void StockCourse::LoadTextures() { }
void StockCourse::Init() {  }
void StockCourse::BeginPlay() {  }
void StockCourse::Render(Camera *camera) { }
void StockCourse::Collision() {}
void StockCourse::Expire() { }
void StockCourse::Destroy() { }