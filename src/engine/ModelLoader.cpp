#include "ModelLoader.h"
#include "port/Engine.h"
#include "engine/courses/Course.h"

extern "C" {
#include "memory.h"
}

void ModelLoader::Add(LoadModelList list) {
    _deferredList.push_back(list);
}

void ModelLoader::Load() {
    if (_hasRan) { // Ensure this does not get ran multiple times
        printf("ModelLoader.cpp has already been ran once.\n");
        return;
    }
    _hasRan = true;

    // Set to track processed courses
    std::unordered_set<Course*> processedCourses;

    for (auto& list : _deferredList) {
        // Check if the course has already been processed
        if (processedCourses.find(list.course) != processedCourses.end()) {
            continue; // Skip if already processed
        }

        // Process the course
        ModelLoader::Extract(list.course);

        // Mark this course as processed
        processedCourses.insert(list.course);
    }
}

void ModelLoader::Extract(Course* course) {
    Course* saveCourse = gWorldInstance.CurrentCourse;
    gWorldInstance.CurrentCourse = course; // Quick hack so that `get_texture` will find the right textures.

    size_t vtxSize = (ResourceGetSizeByName(course->vtx) / sizeof(CourseVtx)) * sizeof(Vtx);
    size_t texSegSize;

    // Convert course vtx to vtx
    Vtx* vtx = reinterpret_cast<Vtx*>(malloc(vtxSize));
    if (vtx == NULL) {
        printf("ModelLoader: Failed to allocate vertex buffer for course: %s\n", course->Props.Name);
        return;
    }
    func_802A86A8(reinterpret_cast<CourseVtx*>(LOAD_ASSET_RAW(course->vtx)), vtx, vtxSize / sizeof(Vtx));

    // Extract packed DLs
    u8* packed = reinterpret_cast<u8*>(LOAD_ASSET_RAW(course->gfx));
    Gfx* gfx = reinterpret_cast<Gfx*>(malloc(sizeof(Gfx) * course->gfxSize)); // Size of unpacked DLs
    if (gfx == NULL) {
        printf("ModelLoader: Failed to allocate course displaylist memory for course: %s\n", course->Props.Name);
        return;
    }

    displaylist_unpack(reinterpret_cast<uintptr_t*>(gfx), reinterpret_cast<uintptr_t>(packed), 0);

    // Now that the data has been retrieved, extract the model data and save to provided buffers.
    for (auto& list : _deferredList) {
        // Ensure the list matches the course being extracted.
        if (list.course != course) {
            return;
        }

        memcpy(list.vtxBuffer, &vtx[list.vtxStart], list.vtxBufferSize * sizeof(Vtx));
        memcpy(list.gfxBuffer, &gfx[list.gfxStart], list.gfxBufferSize * sizeof(Gfx));

        UpdateVtx(list);
    }

    free(vtx);
    free(gfx);

    _deferredList.clear();
    gWorldInstance.CurrentCourse = saveCourse;
}

void ModelLoader::UpdateVtx(LoadModelList list) {
    for (size_t i = 0; i < list.gfxStart + list.gfxBufferSize; i++) {
        Gfx* gfx = &list.gfxBuffer[i];

        // Check if the current Gfx command is G_VTX
        if (GFX_GET_OPCODE(gfx->words.w0) == (G_VTX << 24)) {
            // Re-write the vtx to point at the provided vtx buffer.
            size_t vtxIndex = (gfx->words.w1 / sizeof(Vtx)) - list.vtxStart;
            gfx->words.w1 = reinterpret_cast<uintptr_t>(&list.vtxBuffer[vtxIndex]);
        }
    }
}
