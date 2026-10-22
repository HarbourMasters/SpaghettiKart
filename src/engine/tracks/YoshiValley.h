#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_vertices.h"
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_displaylists.h"
    #include "assets/models/tracks/yoshi_valley/yoshi_valley_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture yoshi_valley_textures[];
}

class YoshiValley : public Track {
public:
    // Constructor
    explicit YoshiValley();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    void Load() override;
    void BeginPlay() override;
    void InitTrackObjects() override;
    void TickTrackObjects() override;
    void DrawTrackObjects(s32 cameraId) override;
    void SomeSounds() override;
    void WhatDoesThisDo(Player* player, int8_t playerId) override;
    void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void Waypoints(Player* player, int8_t playerId) override;
    void CreditsSpawnActors() override;
    void Destroy() override;
};
