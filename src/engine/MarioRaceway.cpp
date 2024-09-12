#include <libultraship.h>

#include "MarioRaceway.h"

extern "C" {
    #include "camera.h"
    #include "course_offsets.h"
}

CourseMarioRaceway::CourseMarioRaceway() : Course(cvtx, textures, displaylists, 3367) {}

    // Virtual functions to be overridden by derived classes
// void CourseMarioRaceway::Load(const char* courseVtx, 
//                   course_texture* textures, const char* displaylists, size_t dlSize) {
//     StockCourse::Load(courseVtx, textures, displaylists, dlSize);
// }
void CourseMarioRaceway::LoadTextures() {}
void CourseMarioRaceway::Init() {  }
void CourseMarioRaceway::BeginPlay() {  }
void CourseMarioRaceway::Render(Camera *camera) { }
void CourseMarioRaceway::Collision() {}
void CourseMarioRaceway::Destroy() { }
