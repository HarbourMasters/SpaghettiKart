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
    #include "path_spawn_metadata.h"
    #include "code_800029B0.h"
}

class MarioRaceway : public Track {
public:
    // Constructor
    explicit MarioRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void BeginPlay() override;
    virtual void InitTrackObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void SetStaffGhost() override;
    virtual void Draw(ScreenContext*) override;
    virtual void DrawCredits() override;    
    virtual void CreditsSpawnActors() override;
    virtual void Destroy() override;
};
