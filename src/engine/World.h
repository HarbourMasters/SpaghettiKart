#pragma once

#include <libultraship.h>
#include "CoreMath.h"
#include "engine/courses/Course.h"
#include "objects/Object.h"
#include "Cup.h"
#include "vehicles/Train.h"
#include "vehicles/Car.h"
#include "objects/BombKart.h"
#include "PlayerBombKart.h"
#include "vehicles/Train.h"
#include "TrainCrossing.h"
#include "objects/Thwomp.h"
#include "objects/Penguin.h"
#include "objects/Seagull.h"
#include "objects/Lakitu.h"
#include <memory>
#include <unordered_map>
#include "Actor.h"
#include "StaticMeshActor.h"
#include "particles/ParticleEmitter.h"

#include "editor/Editor.h"
#include "editor/GameObject.h"

extern "C" {
#include "camera.h"
#include "objects.h"
};

class Cup; // <-- Forward declaration
class OObject;
class Course;
class StaticMeshActor;
class AVehicle;
class OBombKart;
class TrainCrossing;
class OLakitu;
class GameObject; // <-- Editor

class World {
    typedef struct {
        std::vector<Mtx> Hud;
        std::vector<Mtx> Objects;
        std::vector<Mtx> Shadows;
        std::vector<Mtx> Karts;
        std::vector<Mtx> Effects;
    } Matrix;

public:
    explicit World();

    void AddCourse(Course* course);

    AActor* AddActor(AActor* actor);
    struct Actor* AddBaseActor();
    void AddEditorObject(Actor* actor, const char* name);
    AActor* GetActor(size_t index);

    void TickActors();
    AActor* ConvertActorToAActor(Actor* actor);
    Actor* ConvertAActorToActor(AActor* actor);

    void DrawStaticMeshActors();
    StaticMeshActor* AddStaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision);
    void DeleteStaticMeshActors();

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
    void SetCup(Cup* cup);
    const char* GetCupName();
    u32 GetCupIndex();
    u32 NextCup();
    u32 PreviousCup();
    void SetCourseFromCup();

    World* GetWorld(void);
    void ClearWorld(void);


    // These are only for browsing through the course list
    void SetCourse(const char*);
    void NextCourse(void);
    void PreviousCourse(void);

    Matrix Mtx;


    Course* CurrentCourse;
    Cup* CurrentCup;

    std::vector<Cup*> Cups;
    size_t CupIndex = 1;

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
    std::vector<Course*> Courses;
    size_t CourseIndex = 0; // For browsing courses.
private:

};

extern World gWorldInstance;
