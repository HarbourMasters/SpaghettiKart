#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>


// C-compatible function declaration
#ifdef __cplusplus
extern "C" {
#endif

    #include "camera.h"
    #include "course_offsets.h"
void CourseManager_LoadCourse(const char* courseVtx, 
                      course_texture* textures, const char* displaylists, size_t dlSize);

#ifdef __cplusplus
}
#endif


class Course {

public:
    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load(const char* vtx, const char *gfx);
    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(struct UnkStruct_800DC5EC*);
    virtual void Collision();
    virtual void Destroy();
};

#endif // COURSE_H