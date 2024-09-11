#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>

extern "C" {
    #include "camera.h"
}

/**
 * Three classes are to support stock courses which require decompression
 * and custom courses which do not need decompression.
 */

class BaseCourse {
public:
    virtual void load() = 0; // Require derived classes to implement
    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
};

class Course : BaseCourse {
public:
    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load(Vtx*, Gfx*);
    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
};

class StockCourse : BaseCourse{
public:
    virtual ~StockCourse() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit StockCourse();  // UUID should be passed in constructor

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