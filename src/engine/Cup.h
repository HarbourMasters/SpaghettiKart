#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "Course.h"

class Course; // <-- Forward declare

class Cup {
public:
    const char* Name;
    u8 *Thumbnail;
    size_t Index = 0;
    std::vector<std::shared_ptr<Course>> Courses;

    explicit Cup();


    virtual void SetCourses(std::vector<std::shared_ptr<Course>>&);
    virtual void ShuffleCourses();

    virtual void Next();
    virtual void Previous();
    virtual Course* GetCourse();
};