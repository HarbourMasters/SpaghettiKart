#include <libultraship.h>
#include "World.h"
#include "Cup.h"
#include "courses/Course.h"
#include "objects/BombKart.h"
#include "TrainCrossing.h"
#include <memory>
#include "objects/Object.h"
#include "port/Game.h"

#include "editor/GameObject.h"

extern "C" {
#include "camera.h"
#include "objects.h"
#include "main.h"
#include "defines.h"
#include "audio/external.h"
#include "menus.h"
#include "code_800029B0.h"
#include "assets/models/common_data.h"
#include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
}

#include "engine/cameras/GameCamera.h"
#include "engine/cameras/FreeCamera.h"
#include "engine/cameras/TourCamera.h"
#include "engine/cameras/LookBehindCamera.h"

std::shared_ptr<Course> CurrentCourse;
Cup* CurrentCup;

World::World() {
    RaceManagerInstance = std::make_unique<RaceManager>(*this);
}

World::~World() {
    CleanWorld();
}

std::shared_ptr<Course> World::AddCourse(std::shared_ptr<Course> course) {
    gWorldInstance.Courses.push_back(course);
    return course;
}

void World::AddCup(Cup* cup) {
    Cups.push_back(cup);
}

void World::SetCurrentCourse(std::shared_ptr<Course> course) {
    if (CurrentCourse) {
        UnLoadCourse();
    }
    if (CurrentCourse == course) {
        return;
    }
    CurrentCourse = std::move(course);
}

void World::SetCourseFromCup() {
    SetCurrentCourse(CurrentCup->GetCourse());
}

TrainCrossing* World::AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius,
                                  f32 exitRadius) {
    auto crossing = std::make_shared<TrainCrossing>(position, waypointMin, waypointMax, approachRadius, exitRadius);
    Crossings.push_back(crossing);
    return crossing.get();
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

void World::SetCupIndex(size_t index) {
    CupIndex = index;
}

void World::SetCurrentCup(Cup* cup) {
    if (cup) {
        CurrentCup = cup;
        CurrentCup->CursorPosition = 0;
    }
}

void World::SetCourse(const char* name) {
    //! @todo Use content dictionary instead
    for (size_t i = 0; i < Courses.size(); i++) {
        if (strcmp(Courses[i]->Props.Name, name) == 0) {
            SetCurrentCourse(Courses[i]);
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
    gWorldInstance.SetCurrentCourse(Courses[CourseIndex]);
}

void World::PreviousCourse() {
    if (CourseIndex > 0) {
        CourseIndex--;
    } else {
        CourseIndex = Courses.size() - 1;
    }
    gWorldInstance.SetCurrentCourse(Courses[CourseIndex]);
}

void World::TickCameras() {

    for (size_t i = 0; i < 4; i++) {
        struct UnkStruct_800DC5EC* screen = &D_8015F480[i];
        if (NULL == screen->pendingCamera) { continue; }
        if (screen->pendingCamera != screen->camera) {
            screen->camera = screen->pendingCamera;
            screen->pendingCamera = nullptr;
        }
    }

    for (GameCamera* camera : Cameras) {
        if (camera->IsActive()) {
            camera->Tick();
        }
    }
}

AActor* World::AddActor(AActor* actor) {
    Actors.push_back(actor);
    actor->BeginPlay();
    return Actors.back();
}

struct Actor* World::AddBaseActor() {
    Actors.push_back(new AActor());

    AActor* actor = Actors.back();

    // Skip C++ vtable to access variables in C
    return reinterpret_cast<struct Actor*>(reinterpret_cast<char*>(Actors.back()) + sizeof(void*));
}

void World::ActorBeginPlay(Actor* actor) {
    AActor* act = ConvertActorToAActor(actor);
    act->BeginPlay();
}

/**
 * Converts a C struct Actor* to its C++ AActor class
 */
AActor* World::ConvertActorToAActor(Actor* actor) {
    // Move the ptr back so that it points at the vtable.
    // Which is the initial item in the class, or in other words
    // Point to the class.
    return reinterpret_cast<AActor*>((char*) actor - sizeof(void*));
}

/**
 * Converts a C++ AActor class to a C Actor* struct.
 */
Actor* World::ConvertAActorToActor(AActor* actor) {
    // Move the ptr forward past the vtable.
    // This allows C to access the class variables like a normal Actor* struct.
    return reinterpret_cast<Actor*>((char*) actor + sizeof(void*));
}

AActor* World::GetActor(size_t index) {
    return Actors[index];
}

void World::TickActors() {
    // This only ticks modded actors
    for (AActor* actor : Actors) {
        if (actor->IsMod()) {
            actor->Tick();
        }
    }
}

StaticMeshActor* World::AddStaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision) {
    StaticMeshActors.push_back(new StaticMeshActor(name, pos, rot, scale, model, collision));
    auto actor = StaticMeshActors.back();
    return actor;
}

void World::DrawStaticMeshActors() {
    for (const auto& actor : StaticMeshActors) {
        actor->Draw();
    }
}

OObject* World::AddObject(OObject* object) {
    Objects.push_back(object);

    // This is an example of how to get the C object.
    // However, nothing is being done with it, so it's been commented out.
    // if (object->_objectIndex != -1) {
    //     Object* cObj = &gObjectList[object->_objectIndex];
    // }

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

// Sets OObjects or AActors static member variables back to default values
void World::Reset() {
    for (const auto& object : Objects) {
        object->Reset(); // Used for OPenguin
    }
}

Object* World::GetObjectByIndex(size_t index) {
    // if (index < this->Objects.size()) {
    //  Assuming GameActor::a is accessible, use reinterpret_cast if needed
    //    return reinterpret_cast<Object*>(&this->Objects[index]->o);
    //}
    return nullptr; // Or handle the error as needed
}

// Deletes all objects from the world
void World::CleanWorld(void) {
    printf("[Game.cpp] Clean World\n");
    World* world = &gWorldInstance;
    for (auto& actor : world->Actors) {
        delete actor;
    }

    gWorldInstance.Reset(); // Reset OObjects
    for (auto& object : world->Objects) {
        delete object;
    }

    for (auto& emitter : world->Emitters) {
        delete emitter;
    }

    for (auto& actor : world->StaticMeshActors) {
        delete actor;
    }

    for (size_t i = 0; i < ARRAY_COUNT(gWorldInstance.playerBombKart); i++) {
        gWorldInstance.playerBombKart[i].state = PlayerBombKart::PlayerBombKartState::DISABLED;
        gWorldInstance.playerBombKart[i]._primAlpha = 0;
    }

    gEditor.ClearObjects();
    gWorldInstance.Actors.clear();
    gWorldInstance.StaticMeshActors.clear();
    gWorldInstance.Objects.clear();
    gWorldInstance.Emitters.clear();
    gWorldInstance.Lakitus.clear();
}
