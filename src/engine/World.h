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
    std::shared_ptr<Track> mTrack;
    Cup* CurrentCup;

public:
    explicit World();
    ~World();

    RaceManager& GetRaceManager() { return *RaceManagerInstance; }
    void SetRaceManager(std::unique_ptr<RaceManager> manager) { RaceManagerInstance = std::move(manager); }

    std::shared_ptr<Track> AddTrack(std::shared_ptr<Track> track);

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
    void SetTrackFromCup();

    World* GetWorld(void);
    void CleanWorld(void);

    // getter/setter for current track
    std::shared_ptr<Track> GetTrack() {
        return mTrack;
    }

    void SetCurrentTrack(std::shared_ptr<Track> track);

    // These are only for browsing through the track list
    void SetTrack(const char*);
    template<typename T>
    void SetTrackByType() {
        for (const auto& track : Tracks) {
            if (dynamic_cast<T*>(track.get())) {
                SetCurrentTrack(track);
                return;
            }
        }
        printf("World::SetTrackByType() No track by the type found");
    }
    void NextTrack(void);
    void PreviousTrack(void);

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

    // Holds all available tracks
    std::vector<std::shared_ptr<Track>> Tracks;
    size_t TrackIndex = 0; // For browsing tracks.
private:
    std::unique_ptr<RaceManager> RaceManagerInstance;
};

extern World gWorldInstance;
