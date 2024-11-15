#pragma once

#include <libultraship.h>
#include "GameObject.h"
#include "Cup.h"
#include "vehicles/Vehicle.h"
#include "vehicles/Train.h"
#include "vehicles/Car.h"
#include "vehicles/OBombKart.h"
#include "TrainCrossing.h"
#include <memory>
#include "Actor.h"

extern "C" {
#include "camera.h"
#include "objects.h"
#include "engine/Engine.h"
};

class Cup; // <-- Forward declaration
class Course;
class AVehicle;
class ATrain;
class ACar;
class TrainCrossing;

class World {

    typedef struct {
        uint8_t r, g, b;
    } RGB8;

    typedef struct {
        RGB8 TopRight;
        RGB8 BottomRight;
        RGB8 BottomLeft;
        RGB8 TopLeft;
        RGB8 FloorTopRight;
        RGB8 FloorBottomRight;
        RGB8 FloorBottomLeft;
        RGB8 FloorTopLeft;
    } SkyboxColours;


    typedef struct {
        const char* Name;
        const char* DebugName;
        const char* CourseLength;
        const char* AIBehaviour;
        float AIMaximumSeparation;
        float AIMinimumSeparation;
        int16_t *SomePtr;
        uint32_t AISteeringSensitivity;
        _struct_gCoursePathSizes_0x10 PathSizes;
        Vec4f D_0D009418;
        Vec4f D_0D009568;
        Vec4f D_0D0096B8;
        Vec4f D_0D009808;
        const char* PathTable[4];
        const char* PathTable2[4];
        CloudData *Clouds;
        CloudData *CloudList;
        int32_t MinimapFinishlineX;
        int32_t MinimapFinishlineY;
        SkyboxColours Skybox;
    } Properties;

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
    AActor* GetActor(size_t index);

    void TickActors();
    void RemoveExpiredActors();
    AActor* ConvertActorToAActor(Actor* actor);
    Actor* ConvertAActorToActor(AActor* actor);

    Object* AddObject(std::unique_ptr<GameObject> object);

    CProperties* GetCourseProps();
    void TickObjects();
    void DrawObjects(Camera *camera);
    void ExpiredObjects();
    void DestroyObjects();
    Object *GetObjectByIndex(size_t);

    void AddCup(Cup*);
    void SetCup(Cup* cup);
    const char* GetCupName();
    u32 GetCupIndex();
    u32 NextCup();
    u32 PreviousCup();
    void SetCourseFromCup();

    World* GetWorld(void);


    // These are only for browsing through the course list
    void SetCourse(const char*);
    void NextCourse(void);
    void PreviousCourse(void);

    Matrix Mtx;

    // Holds all available courses

    Course* CurrentCourse;
    Cup* CurrentCup;

    std::vector<Cup*> Cups;
    size_t CupIndex = 1;

    std::vector<std::unique_ptr<GameObject>> GameObjects;
    std::vector<AActor*> Actors;

    /** Actors */
    void AddBoat(f32 speed, uint32_t waypoint);
    void AddTrain(size_t numCarriages, f32 speed, uint32_t waypoint);
    void AddTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddBus(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddTankerTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddCar(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    std::vector<std::unique_ptr<AVehicle>> Vehicles;
    void ClearVehicles(void);

    /** Objects **/
    void AddBombKart(Vec3f pos, TrackWaypoint* waypoint, uint16_t waypointIndex, uint16_t state, f32 unk_3C);
    std::vector<std::unique_ptr<OBombKart>> BombKarts;

    TrainCrossing* AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius);
    std::vector<std::shared_ptr<TrainCrossing>> Crossings;

    std::vector<Course*> Courses;
    size_t CourseIndex = 0; // For browsing courses.
private:

};

extern World gWorldInstance;
