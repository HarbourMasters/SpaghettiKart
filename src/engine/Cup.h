// Base Cup class
#include <vector>
#include <memory>
#include "Course.h"

class Cup {
public:
    const char* CupName;
    std::vector<std::shared_ptr<Course>> Courses;

    Cup() {}

    virtual void SetCourses(std::vector<std::shared_ptr<Course>>&);
    virtual void ShuffleCourses();
    virtual void PrintCupInfo() const;

    // Virtual function that allows subclasses to define their courses
    virtual void InitializeCourses() = 0;
};