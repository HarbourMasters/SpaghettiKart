#include <libultraship.h>
#include "World.h"
#include "Cup.h"
#include "courses/Course.h"
#include "vehicles/Vehicle.h"
#include "objects/BombKart.h"
#include "TrainCrossing.h"
#include <memory>
#include "objects/Object.h"

extern "C" {
   #include "camera.h"
   #include "objects.h"
   #include "main.h"
   #include "engine/Engine.h"
   #include "defines.h"
   #include "audio/external.h"
   #include "menus.h"
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


AVehicle* World::AddVehicle(AVehicle* vehicle) {
    Vehicles.push_back(vehicle);
    return Vehicles.back();
}

void World::ClearVehicles(void) {
    Vehicles.clear();
}

TrainCrossing* World::AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius) {
    auto crossing = std::make_shared<TrainCrossing>(position, waypointMin, waypointMax, approachRadius, exitRadius);
    Crossings.push_back(crossing);
    return crossing.get();
}

void World::AddBombKart(Vec3f pos, TrackWaypoint* waypoint, uint16_t waypointIndex, uint16_t state, f32 unk_3C) {
    BombKarts.push_back(new OBombKart(pos, waypoint, waypointIndex, state, unk_3C));
}

u32 World::GetCupIndex() {
    return this->CupIndex;
}

u32 World::NextCup() {
    s32 hack = 1;

    // Do not display battle mode on GP, TT, or VS
    if (gModeSelection != BATTLE) {
        hack = 2;
    }

    if (CupIndex < Cups.size() - hack) {
        CupIndex++;
        CurrentCup = Cups[CupIndex];
        CurrentCup->CursorPosition = 0;
        reset_cycle_flash_menu();
        play_sound2(SOUND_MENU_CURSOR_MOVE);
        return CupIndex;
    }
    return Cups.size() - hack;
}

u32 World::PreviousCup() {
    if (CupIndex > 0) {
        CupIndex--;
        CurrentCup = Cups[CupIndex];
        CurrentCup->CursorPosition = 0;
        reset_cycle_flash_menu();
        play_sound2(SOUND_MENU_CURSOR_MOVE);
        return CupIndex;
    }
    return 0;
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

OObject* World::AddObject(OObject* object) {
    Objects.push_back(object);
    return Objects.back();
}

void World::TickObjects() {
    for (const auto& object : Objects) {
        object->Tick();
    }
}

// Some objects such as lakitu are ticked in process_game_tick.
// This is a fallback to support those objects. Probably don't use this.
void World::TickObjects60fps() {
    for (const auto& object : Objects) {
       object->Tick60fps();
    }
}

ParticleEmitter* World::AddEmitter(ParticleEmitter* emitter) {
    Emitters.push_back(emitter);
    return Emitters.back();
}

void World::DrawObjects(s32 cameraId) {
    for (const auto& object : Objects) {
       object->Draw(cameraId);
    }
}

void World::TickParticles() {
    for (const auto& emitter : Emitters) {
       emitter->Tick();
    }
}

void World::DrawParticles(s32 cameraId) {
    for (const auto& emitter : Emitters) {
       emitter->Draw(cameraId);
    }
}

Object* World::GetObjectByIndex(size_t index) {
    //if (index < this->Objects.size()) {
        // Assuming GameActor::a is accessible, use reinterpret_cast if needed
    //    return reinterpret_cast<Object*>(&this->Objects[index]->o);
    //}
    return nullptr; // Or handle the error as needed
}
