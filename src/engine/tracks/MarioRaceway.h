#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
    #include "assets/models/tracks/mario_raceway/mario_raceway_vertices.h"
    #include "assets/models/tracks/mario_raceway/mario_raceway_displaylists.h"
    #include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
}

class MarioRaceway : public Track {
public:
    // Constructor
    explicit MarioRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    void Load() override;
    void BeginPlay() override;
    void InitTrackObjects() override;
    void SomeSounds() override;
    void WhatDoesThisDo(Player* player, int8_t playerId) override;
    void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    void SetStaffGhost() override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void CreditsSpawnActors() override;
    void Destroy() override;
};
