#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "Course.h"

class Course; // <-- Forward declare

class Cup {
public:
    const char* CupName;
    u8 *Thumbnail;
    u32 Index = 0;
    std::vector<std::shared_ptr<Course>> Courses;

    explicit Cup();


    virtual void SetCourses(std::vector<std::shared_ptr<Course>>&);
    virtual void ShuffleCourses();

    virtual void Next();
    virtual void Previous();

    // Virtual function that allows subclasses to define their courses
    virtual void InitializeCourses() = 0;
};