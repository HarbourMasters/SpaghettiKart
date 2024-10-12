#include <libultraship.h>
#include "World.h"
#include "Cup.h"
#include "courses/Course.h"
#include "vehicles/Vehicle.h"
#include "vehicles/Train.h"
#include "vehicles/Boat.h"
#include "vehicles/Truck.h"
#include "vehicles/Bus.h"
#include "vehicles/TankerTruck.h"
#include "vehicles/Car.h"
#include "TrainCrossing.h"
#include <memory>


extern "C" {
   #include "camera.h"
   #include "objects.h"
   #include "main.h"
   #include "engine/Engine.h"
   #include "defines.h"
}

World::World() {}

Course* CurrentCourse;
Cup* CurrentCup;

Cup* World::AddCup(const char* name, std::vector<Course*> courses) {
    // Create a new unique_ptr for Cup
    auto cup = std::make_shared<Cup>(name, courses);

    // Get raw pointer before moving the ownership
    Cup* tmp = cup.get();

    // Add the Cup to the container
    Cups.push_back(std::move(cup));

    // Return the raw pointer to the Cup
    return tmp;
}

Cup* World::GetCup() {
    return Cups[CupIndex].get();
}

void World::SetCourseFromCup() {
    CurrentCourse = CurrentCup->GetCourse();
}

// Required for spawning vehicles in divisions across path points
static size_t trains;
static size_t trucks;
static size_t busses;
static size_t tankerTrucks;
static size_t cars;
static size_t boats;
void World::AddTrain(size_t numCarriages, f32 speed, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ATrain>(trains, numCarriages, speed, waypoint));
    trains++;
}

void World::AddBoat(f32 speed, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ABoat>(boats, speed, waypoint));
    boats++;
}

void World::AddTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ATruck>(trucks, speedA, speedB, path, waypoint));
    trucks++;
}

void World::AddBus(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ABus>(busses, speedA, speedB, path, waypoint));
    busses++;
}

void World::AddTankerTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ATankerTruck>(tankerTrucks, speedA, speedB, path, waypoint));
    tankerTrucks++;
}

void World::AddCar(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint) {
    Vehicles.push_back(std::make_unique<ACar>(cars, speedA, speedB, path, waypoint));
    cars++;
}

void World::ResetVehicles(void) {
    trains = trucks = busses = tankerTrucks = cars = boats = 0;
    Vehicles.clear();
}

TrainCrossing* World::AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius) {
    auto crossing = std::make_shared<TrainCrossing>(position, waypointMin, waypointMax, approachRadius, exitRadius);
    Crossings.push_back(crossing);
    return crossing.get();
}

//const char* World::GetCupName() {
//    //return this->Cups[CupIndex].Name;
//}

void World::SetCupIndex(int16_t courseId) {

    this->CupIndex = courseId;
}

u32 World::GetCupIndex() {
    return this->CupIndex;
}

u32 World::NextCup() {
    s32 hack = 1;

    // Prevent battle mode
    if (gModeSelection == GRAND_PRIX) {
        hack = 2;
    }

    if (this->CupIndex < Cups.size() - 2) {
        CupIndex++;
        CurrentCup = Cups[CupIndex].get();
        return CupIndex;
    }
}

u32 World::PreviousCup() {
    if (CupIndex > 0) {
        CupIndex--;
        CurrentCup = Cups[CupIndex].get();
        return CupIndex;
    }
}

void World::SetCup() {
    CurrentCup = Cups[CupIndex].get();
    CurrentCup->CursorPosition = 0;
}

CProperties* World::GetCourseProps() {
    if (Courses[CourseIndex]) {
        return (CProperties*) &Courses[CourseIndex]->Props;
    }
    return nullptr;
}

void World::SetCourse(const char*name) {
    for (size_t i = 0; i < Courses.size(); i++) {
        if (Courses[i]->Props.Name == name) {
            CurrentCourse = Courses[i];
            break;
        }
    }
    std::runtime_error("SetCourse() Course name not found in Courses list");
}

void World::NextCourse() {
    if (CourseIndex < Courses.size() - 1) {
        CourseIndex++;
    } else {
        CourseIndex = 0;
    }
    gWorldInstance.CurrentCourse = Courses[CourseIndex];
}

void World::PreviousCourse() {
    if (CourseIndex > 0) {
        CourseIndex--;
    } else {
        CourseIndex = Courses.size() - 1;
    }
    gWorldInstance.CurrentCourse = Courses[CourseIndex];
}

Object* World::SpawnObject(std::unique_ptr<GameObject> object) {
    GameObject* rawPtr = object.get();
    this->GameObjects.push_back(std::move(object));
    return &rawPtr->o;
}

void World::UpdateObjects() {
    for (const auto& object : this->GameObjects) {
        object->Update();
    }
}

void World::RenderObjects(Camera *camera) {
    for (const auto& object : this->GameObjects) {
        object->Render(camera);
    }
}

void World::ExpiredObjects() {
    this->GameObjects.erase(
        std::remove_if(this->GameObjects.begin(), this->GameObjects.end(),
                        [](const std::unique_ptr<GameObject>& object) { return object->uuid == 0; }), // Example condition
        this->GameObjects.end());
}

void World::DestroyObjects() {

}

Object* World::GetObjectByIndex(size_t index) {
    if (index < this->GameObjects.size()) {
        // Assuming GameActor::a is accessible, use reinterpret_cast if needed
        return reinterpret_cast<Object*>(&this->GameObjects[index]->o);
    }
    return nullptr; // Or handle the error as needed
}
