#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "courses/Course.h"

class Course; // <-- Forward declare

class Cup {
public:
    const char* Name;
    u8 *Thumbnail;
    size_t CursorPosition = 0; // Course index in cup
    std::vector<Course*> Courses;

    explicit Cup(const char* name, std::vector<Course*> courses);

    virtual void ShuffleCourses();

    virtual void Next();
    virtual void Previous();
    virtual Course* GetCourse();
    virtual size_t GetSize();
};