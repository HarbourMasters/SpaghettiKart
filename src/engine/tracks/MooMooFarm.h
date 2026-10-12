#pragma once

#include <libultraship.h>
#include "Track.h"
#include "engine/objects/Mole.h"

extern "C" {
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_vertices.h"
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_displaylists.h"
    #include "assets/models/tracks/moo_moo_farm/moo_moo_farm_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "data/path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture moo_moo_farm_textures[];
}

class OMole;

class MooMooFarm : public Track {
public:
    // Constructor
    explicit MooMooFarm();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    void Load() override;
    void BeginPlay() override;
    void WhatDoesThisDo(Player* player, int8_t playerId) override;
    void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    void Draw(ScreenContext*) override;
    void DrawCredits() override;
    void CreditsSpawnActors() override;
    void Destroy() override;
};
