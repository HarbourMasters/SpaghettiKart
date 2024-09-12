#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "course_offsets.h"
    #include "camera.h"
}

class CourseMarioRaceway : public StockCourse {
public:
    virtual ~CourseMarioRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit CourseMarioRaceway();

    virtual void Load(const char* courseVtx, 
                  course_texture* textures, const char* displaylists, size_t dlSize) override;
    virtual void LoadTextures() override;
    virtual void Init() override;
    virtual void BeginPlay() override;
    virtual void Render(Camera*) override;
    virtual void Collision() override;
    virtual void Destroy() override;
};
