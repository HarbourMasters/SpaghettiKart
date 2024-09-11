#include <libultraship.h>

#include "Course.h"

extern "C" {
    #include "course_offsets.h"
}

CourseMarioRaceway::CourseMarioRaceway() {}

    // Virtual functions to be overridden by derived classes

void CourseMarioRaceway::Init() {  }
void CourseMarioRaceway::BeginPlay() {  }
void CourseMarioRaceway::Render(Camera *camera) { }
void CourseMarioRaceway::Collision() {}
void CourseMarioRaceway::Destroy() { }