#include <libultraship.h>
#include "World.h"
#include "Cup.h"
#include "tracks/Track.h"
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

std::shared_ptr<Track> mTrack;
Cup* CurrentCup;

World::World() {
    RaceManagerInstance = std::make_unique<RaceManager>(*this);
}

World::~World() {
    CleanWorld();
}

std::shared_ptr<Track> World::AddTrack(std::shared_ptr<Track> track) {
    gWorldInstance.Tracks.push_back(track);
    return track;
}

void World::AddCup(Cup* cup) {
    Cups.push_back(cup);
}

void World::SetCurrentTrack(std::shared_ptr<Track> track) {
    if (mTrack) {
        UnLoadTrack();
    }
    if (mTrack == track) {
        return;
    }
    mTrack = std::move(track);
}

void World::SetTrackFromCup() {
    SetCurrentTrack(CurrentCup->GetTrack());
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

void World::SetTrack(const char* name) {
    //! @todo Use content dictionary instead
    for (size_t i = 0; i < Tracks.size(); i++) {
        if (strcmp(Tracks[i]->Props.Name, name) == 0) {
            SetCurrentTrack(Tracks[i]);
            break;
        }
    }
    std::runtime_error("[World] [SetTrack()] Track name not found in Track list");
}

void World::NextTrack() {
    if (TrackIndex < Tracks.size() - 1) {
        TrackIndex++;
    } else {
        TrackIndex = 0;
    }
    gWorldInstance.SetCurrentTrack(Tracks[TrackIndex]);
}

void World::PreviousTrack() {
    if (TrackIndex > 0) {
        TrackIndex--;
    } else {
        TrackIndex = Tracks.size() - 1;
    }
    gWorldInstance.SetCurrentTrack(Tracks[TrackIndex]);
}

void World::TickCameras() {

    for (size_t i = 0; i < 4; i++) {
        ScreenContext* screen = &gScreenContexts[i];
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
