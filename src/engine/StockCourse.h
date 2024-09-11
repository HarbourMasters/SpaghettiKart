#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>

extern "C" {
    #include "course_offsets.h"
}

class Course {
public:
    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load(const char* courseVtx, 
                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Destroy();
};

#endif // COURSE_H