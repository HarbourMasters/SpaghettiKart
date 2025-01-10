#pragma once

#include <libultraship.h>
#include "engine/courses/Course.h"

extern "C" {
#include "common_structs.h"
}

class Course;

/**
 * 
 * Lists are deferred until load time so that models that use the same course may all use the same extraction
 * 
 * Note ensure that the buffers passed to LoadModelList are big enough for the requested data.
 * 
 * This class should only be ran once.
 * 
 * 
 * Usage:
 * 
 *     ModelLoader::LoadModelList bowserStatueList = {
        .course = gBowsersCastle,
        .gfxBuffer = &gBowserStatueGfx[0],
        .gfxBufferSize = 162,
        .gfxStart = (0x2BB8 / 8), // This is 0x2BB8 / sizeof(N64Gfx) not sizeof(Gfx)
        .vtxBuffer = &gBowserStatueVtx[0],
        .vtxBufferSize = 717,
        .vtxStart = 1942,
    };
 * 
 * gModelLoader.Add(bowserStatueList);
 */

class ModelLoader {

public:
    struct LoadModelList {
        Course* course;

        Gfx* gfxBuffer; // buffer for output gfx
        size_t gfxBufferSize;
        size_t gfxStart; // The starting point to extract data in NumGfx

        Vtx* vtxBuffer; // buffer for output vtx
        size_t vtxBufferSize;
        size_t vtxStart; // The starting point to extract data in NumVtx
    };

    void Add(LoadModelList list);
    void Load();
private:
    struct CourseMap {

    };

    void Extract(Course* course);
    void UpdateVtx(LoadModelList list);

    std::vector<LoadModelList> _deferredList;
    bool _hasRan = false;
};
