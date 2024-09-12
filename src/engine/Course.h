#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>


// C-compatible function declaration
#ifdef __cplusplus
extern "C" {
#endif

    #include "camera.h"
    #include "course_offsets.h"
void load_stock_course(const char* courseVtx, 
                      course_texture* textures, const char* displaylists, size_t dlSize);

#ifdef __cplusplus
}
#endif


/**
 * Three classes are to support stock courses which require decompression
 * and custom courses which do not need decompression.
 */

class BaseCourse { /* Do not extend this class, use the subclass 'Course' */
public:
    virtual void Load() = 0; // Require derived classes to implement
    virtual void LoadTextures() = 0;
    virtual void Init() = 0;
    virtual void BeginPlay() = 0;
    virtual void Render(Camera*) = 0;
    virtual void Collision() = 0;
    virtual void Destroy() = 0;
};

class Course : public BaseCourse {
public:
    virtual ~Course() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit Course();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Load() override;
    virtual void Load(Vtx*, Gfx*);
    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Destroy();
};

class StockCourse : public BaseCourse{
public:
    virtual ~StockCourse() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit StockCourse();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
   // virtual void Load() override;
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