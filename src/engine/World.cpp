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
#include "vehicles/OBombKart.h"
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

void World::AddCourse(Course* course) {
    gWorldInstance.Courses.push_back(course);
}

void World::AddCup(Cup* cup) {
    Cups.push_back(cup);
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

void World::ClearVehicles(void) {
    trains = trucks = busses = tankerTrucks = cars = boats = 0;
    Vehicles.clear();
}

TrainCrossing* World::AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius) {
    auto crossing = std::make_shared<TrainCrossing>(position, waypointMin, waypointMax, approachRadius, exitRadius);
    Crossings.push_back(crossing);
    return crossing.get();
}

void World::AddBombKart(Vec3f pos, TrackWaypoint* waypoint, uint16_t waypointIndex, uint16_t state, f32 unk_3C) {
    BombKarts.push_back(std::make_unique<OBombKart>(pos, waypoint, waypointIndex, state, unk_3C));
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

    if (CupIndex < Cups.size() - hack) {
        CupIndex++;
        CurrentCup = Cups[CupIndex];
        CurrentCup->CursorPosition = 0;
        return CupIndex;
    }
}

u32 World::PreviousCup() {
    if (CupIndex > 0) {
        CupIndex--;
        CurrentCup = Cups[CupIndex];
        CurrentCup->CursorPosition = 0;
        return CupIndex;
    }
}

void World::SetCup(Cup* cup) {
    if (cup) {
        CurrentCup = cup;
        CurrentCup->CursorPosition = 0;
    }
}

CProperties* World::GetCourseProps() {
    if (Courses[CourseIndex]) {
        return (CProperties*) &Courses[CourseIndex]->Props;
    }
    return nullptr;
}

void World::SetCourse(const char* name) {
    //! @todo Use content dictionary instead
    for (size_t i = 0; i < Courses.size(); i++) {
        if (strcmp(Courses[i]->Props.Name, name) == 0) {
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

Object* World::AddObject(std::unique_ptr<GameObject> object) {
    GameObject* rawPtr = object.get();
    GameObjects.push_back(std::move(object));
    return &rawPtr->o;
}

AActor* World::AddActor(AActor* actor) {
    Actors.push_back(actor);
    return Actors.back();
}

struct Actor* World::AddBaseActor() {
    Actors.push_back(new AActor());
    // Skip C++ vtable to access variables in C
    return reinterpret_cast<struct Actor*>(reinterpret_cast<char*>(Actors.back()) + sizeof(void*));
}

/**
 * Converts a C struct Actor* to its C++ AActor class
 */
AActor* World::ConvertActorToAActor(Actor* actor) {
    // Move the ptr back so that it points at the vtable.
    // Which is the initial item in the class, or in other words
    // Point to the class.
    return reinterpret_cast<AActor*>((char*)actor - sizeof(void*));
}

/**
 * Converts a C++ AActor class to a C Actor* struct.
 */
Actor* World::ConvertAActorToActor(AActor* actor) {
    // Move the ptr forward past the vtable.
    // This allows C to access the class variables like a normal Actor* struct.
    return reinterpret_cast<Actor*>((char*)actor + sizeof(void*));
}

AActor* World::GetActor(size_t index) {
    return Actors[index];
}

void World::TickActors() {
    for (AActor* actor : Actors) {
        if (actor->IsMod()) {
            actor->Tick();
        }
    }
}

void RemoveExpiredActors() {
    // Actors.erase(
    //    std::remove_if(Actors.begin(), Actors.end(),
    //                    [](const std::unique_ptr<AActor>& actor) { return actor->uuid == 0; }),
    //    Actors.end());
}

void World::TickObjects() {
    for (const auto& object : this->GameObjects) {
        object->Tick();
    }
}

void World::DrawObjects(Camera *camera) {
    for (const auto& object : this->GameObjects) {
        object->Draw(camera);
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
