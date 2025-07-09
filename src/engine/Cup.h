#pragma once

// Base Cup class
#include <vector>
#include <memory>
#include "courses/Course.h"

class Course; // <-- Forward declare

class Cup {
public:
    std::string Id;
    const char* Name;
    u8 *Thumbnail;
    size_t CursorPosition = 0; // Course index in cup
    std::vector<std::shared_ptr<Course>> Courses;

    explicit Cup(std::string id, const char* name, std::vector<std::shared_ptr<Course>> courses);

    virtual void ShuffleCourses();

    virtual void Next();
    virtual void Previous();
    virtual void SetCourse(size_t position);
    virtual std::shared_ptr<Course> GetCourse();
    virtual size_t GetSize();
};