#pragma once

#include <libultraship.h>

#include "CoreMath.h"
#include "engine/courses/Course.h"
#include "engine/cameras/GameCamera.h"
#include "objects/Object.h"
#include "Cup.h"
#include "PlayerBombKart.h"
#include "TrainCrossing.h"
#include <memory>
#include <unordered_map>
#include <utility>
#include "RaceManager.h"
#include "Actor.h"
#include "StaticMeshActor.h"
#include "particles/ParticleEmitter.h"

#include "editor/Editor.h"
#include "editor/GameObject.h"
#include "port/Game.h"

extern "C" {
#include "camera.h"
#include "objects.h"
};

class Cup; // <-- Forward declaration
class OObject;
class GameCamera;
class Course;
class StaticMeshActor;
class OBombKart;
class TrainCrossing;
class OLakitu;
class GameObject; // <-- Editor

class World {
typedef struct Matrix {
    Mtx Screen2D; // Orthogonal projection for UI, skybox, and such
    Mtx Ortho;
    std::array<Mtx,5> Persp;
    std::array<Mtx,5> LookAt;
    std::array<Mtx, 8 * 4> Karts; // Eight players * four screens
    std::array<Mtx, 8 * 4> Shadows; // Eight players * four screens
    std::deque<Mtx> Hud;
    std::deque<Mtx> Objects;

    Matrix()
        : Hud(200), Objects(1000)
    {}
};
private:
    std::shared_ptr<Course> CurrentCourse;
    Cup* CurrentCup;

public:
    explicit World();
    ~World();

    RaceManager& GetRaceManager() { return *RaceManagerInstance; }
    void SetRaceManager(std::unique_ptr<RaceManager> manager) { RaceManagerInstance = std::move(manager); }

    std::shared_ptr<Course> AddCourse(std::shared_ptr<Course> course);

    void TickCameras();

    AActor* AddActor(AActor* actor);
    struct Actor* AddBaseActor();
    void ActorBeginPlay(Actor* actor);
    AActor* GetActor(size_t index);

    void TickActors();
    AActor* ConvertActorToAActor(Actor* actor);
    Actor* ConvertAActorToActor(AActor* actor);

    void DrawStaticMeshActors();
    StaticMeshActor* AddStaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision);

    OObject* AddObject(OObject* object);

    void TickObjects();
    void TickObjects60fps();
    void DrawObjects(s32 cameraId);
    Object *GetObjectByIndex(size_t);

    void TickParticles();
    void DrawParticles(s32 cameraId);
    ParticleEmitter* AddEmitter(ParticleEmitter* emitter);
    void Reset(void); // Sets OObjects or AActors static member variables back to default values

    void AddCup(Cup*);
    void SetCurrentCup(Cup* cup);
    Cup* GetCurrentCup() {
        return CurrentCup;
    }
    void SetCupIndex(size_t index);
    const char* GetCupName();
    u32 GetCupIndex();
    u32 NextCup();
    u32 PreviousCup();
    void SetCourseFromCup();

    World* GetWorld(void);
    void CleanWorld(void);

    // getter/setter for current course
    std::shared_ptr<Course> GetCurrentCourse() {
        return CurrentCourse;
    }

    void SetCurrentCourse(std::shared_ptr<Course> course);

    // These are only for browsing through the course list
    void SetCourse(const char*);
    template<typename T>
    void SetCourseByType() {
        for (const auto& course : Courses) {
            if (dynamic_cast<T*>(course.get())) {
                SetCurrentCourse(course);
                return;
            }
        }
        printf("World::SetCourseByType() No course by the type found");
    }
    void NextCourse(void);
    void PreviousCourse(void);

    Matrix Mtx;

    std::vector<Cup*> Cups;
    size_t CupIndex = 1;

    std::vector<GameCamera*> Cameras;

    std::vector<StaticMeshActor*> StaticMeshActors;
    std::vector<AActor*> Actors;
    std::vector<OObject*> Objects;
    std::vector<ParticleEmitter*> Emitters;

    std::unordered_map<s32, OLakitu*> Lakitus;

    /** Objects **/
    PlayerBombKart playerBombKart[4]; // Used in battle mode

    TrainCrossing* AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius);
    std::vector<std::shared_ptr<TrainCrossing>> Crossings;

    // Holds all available courses
    std::vector<std::shared_ptr<Course>> Courses;
    size_t CourseIndex = 0; // For browsing courses.
private:
    std::unique_ptr<RaceManager> RaceManagerInstance;
};

extern World gWorldInstance;
