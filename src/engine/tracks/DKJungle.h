#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_vertices.h"
    #include "assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_displaylists.h"
    #include "assets/models/tracks/dks_jungle_parkway/dks_jungle_parkway_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture dks_jungle_parkway_textures[];
}

class DKJungle : public Track {
public:
    // Constructor
    explicit DKJungle();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    void Load() override;
    f32 GetWaterLevel(FVector pos, struct Collision* collision) override;
    void BeginPlay() override;
    void InitTrackObjects() override;
    void TickTrackObjects() override;
    void DrawTrackObjects(s32 cameraId) override;
    void SomeSounds() override;
    void WhatDoesThisDo(Player* player, int8_t playerId) override;
    void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                            f32* arg7) override;
    void Waypoints(Player* player, int8_t playerId) override;
    void Tick() override;
    void DrawTransparency(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot,
                          uint16_t playerDirection) override;
    void CreditsSpawnActors() override;
    void Destroy() override;
};
