#ifndef COURSE_H
#define COURSE_H

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "course_offsets.h"
}

class CourseMarioRaceway : public StockCourse {
public:
    virtual ~CourseMarioRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit CourseMarioRaceway();

    virtual void LoadTextures();
    virtual void Init();
    virtual void BeginPlay();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
};

#endif // COURSE_H