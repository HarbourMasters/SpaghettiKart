#pragma once

#include <libultraship.h>

#include "CoreMath.h"
#include "engine/tracks/Track.h"
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
class Track;
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
    std::unique_ptr<Track> mTrack;
    Cup* CurrentCup;

public:
    static World* Instance;
    explicit World();
    ~World();

    RaceManager& GetRaceManager() { return *RaceManagerInstance; }
    void SetRaceManager(std::unique_ptr<RaceManager> manager) { RaceManagerInstance = std::move(manager); }

    void TickCameras();

    AActor* AddActor(std::unique_ptr<AActor> actor);
    struct Actor* AddBaseActor();
    void ActorBeginPlay(Actor* actor);
    AActor* GetActor(size_t index);

    void TickActors();
    AActor* ConvertActorToAActor(Actor* actor);
    Actor* ConvertAActorToActor(AActor* actor);

    void DrawStaticMeshActors();
    StaticMeshActor* AddStaticMeshActor(const std::string& name, FVector pos, IRotator rot, FVector scale, const std::string& model, int32_t* collision);

    OObject* AddObject(std::unique_ptr<OObject> object);

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

    World* GetWorld(void);
    void CleanWorld(void);

    // getter/setter for current track
    Track* GetTrack() {
        return mTrack.get();
    }

    void SetCurrentTrack(std::unique_ptr<Track> track);

    Matrix Mtx;

    std::vector<Cup*> Cups;
    size_t CupIndex = 1;

    std::vector<GameCamera*> Cameras;

    std::vector<std::unique_ptr<StaticMeshActor>> StaticMeshActors;
    std::vector<std::unique_ptr<AActor>> Actors;
    std::vector<std::unique_ptr<OObject>> Objects;
    std::vector<ParticleEmitter*> Emitters;

    std::unordered_map<s32, OLakitu*> Lakitus;

    /** Objects **/
    PlayerBombKart mPlayerBombKart[4]; // Used in battle mode

    TrainCrossing* AddCrossing(Vec3f position, u32 waypointMin, u32 waypointMax, f32 approachRadius, f32 exitRadius);
    std::vector<std::shared_ptr<TrainCrossing>> Crossings;
private:
    std::unique_ptr<RaceManager> RaceManagerInstance;
};
