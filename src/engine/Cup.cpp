#include "Cup.h"
#include "courses/Course.h"

Cup::Cup(const char* name, std::vector<Course*> courses) {
    this->Name = name;
    this->Courses = courses;

    if (Courses.size() != 4) {
        throw std::invalid_argument("A cup must contain exactly 4 courses.");
    }
}

void Cup::Next() {
    if (CursorPosition < Courses.size() - 1) {
        CursorPosition++;
    }
}

void Cup::Previous() {
    if (CursorPosition > 0) {
        CursorPosition--;
    }
}

Course* Cup::GetCourse() {
    return Courses[CursorPosition];
}

size_t Cup::GetSize() {
    return Courses.size();
}

// Function to shuffle the courses randomly
void Cup::ShuffleCourses() {
    // std::random_device rd;
    // std::mt19937 g(rd());
    //std::shuffle(Courses.begin(), Courses.end(), g);
}
