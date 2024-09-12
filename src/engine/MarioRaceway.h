#pragma once

#include <libultraship.h>
#include "Course.h"
#include "mario_raceway_vertices.h"
#include "mario_raceway_displaylists.h"

extern "C" {
    #include "course_offsets.h"
    #include "camera.h"
    extern const course_texture mario_raceway_textures[];
}

class CourseMarioRaceway : public Course {
public:
    virtual ~CourseMarioRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit CourseMarioRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void LoadTextures() override;
    virtual void Init() override;
    virtual void BeginPlay() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void Collision() override;
    virtual void Destroy() override;
};
