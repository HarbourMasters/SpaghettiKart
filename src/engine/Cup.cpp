#include "Cup.h"

Cup::Cup() {}

// Function for subclasses to set the tracks
void Cup::SetCourses(std::vector<std::shared_ptr<Course>>& courses) {
    if (this->Courses.size() != 4) {
        throw std::invalid_argument("A cup must contain exactly 4 courses.");
    }
    Courses = courses;
}

// Print details of all courses in the cup
void Cup::PrintCupInfo() const {
   // std::cout << "Cup: " << CupName << "\n";
    for (const auto& course : Courses) {
       // course->PrintInfo();
    }
}

// Function to shuffle the courses randomly
void Cup::ShuffleCourses() {
    // std::random_device rd;
    // std::mt19937 g(rd());
    //std::shuffle(Courses.begin(), Courses.end(), g);
}

Cup* currentCup;

Cup* CupProps() {
    return currentCup;
}