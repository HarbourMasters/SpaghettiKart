#include <libultraship.h>

#include "Course.h"
#include "MarioRaceway.h"

extern "C" {
    #include "main.h"
    #include "memory.h"
    #include "common_structs.h"
    #include "course_offsets.h"
}

Course::Course() {}

void Course::Load(const char* vtx, const char* gfx) {
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
}

void Course::Load() {

    size_t vtxSize = (ResourceGetSizeByName(this->cvtx) / sizeof(CourseVtx)) * sizeof(Vtx);
    size_t texSegSize;

    // Convert course vtx to vtx
    Vtx* vtx = reinterpret_cast<Vtx*>(allocate_memory(vtxSize));
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    printf("\nVtxsize: 0x%X\n\n",vtxSize);
    func_802A86A8(reinterpret_cast<CourseVtx*>(LOAD_ASSET_RAW(this->cvtx)), vtx, vtxSize / sizeof(Vtx));

    // Load and allocate memory for course textures
    course_texture* asset = textures;
    u8* freeMemory = NULL;
    u8* texture = NULL;
    size_t size = 0;
    texSegSize = 0;
    while (asset->addr) {
        size = ResourceGetTexSizeByName(asset->addr);
        freeMemory = (u8*) allocate_memory(size);

        texture = reinterpret_cast<u8*>(LOAD_ASSET_RAW(asset->addr));
        if (texture) {
            if (asset == &textures[0]) {
                gSegmentTable[5] = reinterpret_cast<uintptr_t>(&freeMemory[0]);
            }
            memcpy(freeMemory, texture, size);
            texSegSize += size;
            // printf("Texture Addr: 0x%llX, size 0x%X\n", &freeMemory[0], size);
        }
        asset++;
    }

    // Extract packed DLs
    u8* packed = reinterpret_cast<u8*>(LOAD_ASSET_RAW(this->displaylists));
    Gfx* gfx = (Gfx*) allocate_memory(sizeof(Gfx) * this->dlSize); // Size of unpacked DLs
    assert(gfx != NULL);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
    displaylist_unpack(reinterpret_cast<uintptr_t *>(gfx), reinterpret_cast<uintptr_t>(packed), 0);
}

void Course::LoadTextures() { }
void Course::Init() {  }
void Course::BeginPlay() {  }
void Course::Render(Camera *camera) { }
void Course::Collision() {}
void Course::Destroy() { }

extern "C" {
    void CourseManager_LoadCourse(const char* courseVtx, 
                      course_texture* textures, const char* displaylists, size_t dlSize) {
    CourseMarioRaceway* course = new CourseMarioRaceway();
        course->Load();

    }
}