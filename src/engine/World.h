#pragma once

#include <libultraship.h>
#include "GameObject.h"
#include "Cup.h"
#include "vehicles/Vehicle.h"
#include "vehicles/Train.h"
#include "vehicles/Car.h"
#include "TrainCrossing.h"
#include <memory>

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

public:
    //Actor actors;
    virtual ~World() = default;
    explicit World();

    // virtual Actor* SpawnActor(std::unique_ptr<GameActor> actor);

    virtual Object* SpawnObject(std::unique_ptr<GameObject> object);


    virtual CProperties* GetCourseProps();
    virtual void UpdateObjects();
    virtual void RenderObjects(Camera *camera);
    virtual void ExpiredObjects();
    virtual void DestroyObjects();
    virtual Object *GetObjectByIndex(size_t);

    Cup* AddCup(const char* name, std::vector<Course*> courses);
    Cup* GetCup();
    const char* GetCupName();
    virtual u32 GetCupIndex();
    virtual void SetCupIndex(int16_t courseId);
    virtual u32 NextCup();
    virtual u32 PreviousCup();
    virtual void SetCourseFromCup();
    void SetCup();

    World* GetWorld(void);


    // These are only for browsing through the course list
    void SetCourse(const char*);
    void NextCourse(void);
    void PreviousCourse(void);

    // Holds all available courses

    Course* CurrentCourse;
    Cup* CurrentCup;

    std::vector<std::shared_ptr<Cup>> Cups;
    size_t CupIndex = 1;

    std::vector<std::unique_ptr<GameObject>> GameObjects;
    // std::vector<std::unique_ptr<GameActor>> GameActors;

    void AddBoat(f32 speed, uint32_t waypoint);
    void AddTrain(size_t numCarriages, f32 speed, uint32_t waypoint);
    void AddTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddBus(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddTankerTruck(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    void AddCar(f32 speedA, f32 speedB, TrackWaypoint* path, uint32_t waypoint);
    std::vector<std::unique_ptr<AVehicle>> Vehicles;
    void ResetVehicles(void);

    TrainCrossing* AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius);
    std::vector<std::shared_ptr<TrainCrossing>> Crossings;

    std::vector<Course*> Courses;
    size_t CourseIndex = 0; // For browsing courses.
private:

};

extern World gWorldInstance;
